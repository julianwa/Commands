//
//  View.m
//  Test
//
//  Created by Julian Walker on 10/31/15.
//  Copyright © 2015 Julian Walker. All rights reserved.
//

#import "View.h"
#include "SpaceCommandReceiver.h"
#include "IdeaCommandReceiver.h"
#include "UserCommandReceiver.h"
#include "Commands.h"
#include "ModelMediator.h"
#include <vector>
#include "ModelRegistry.h"
#include "Model.h"
#include "CommandReceiverFactory.h"
#include "ParseCommandJSON.h"
#include <sstream>
#include <iostream>
#include <experimental/optional>

using namespace std;

// The user id that all model content would be bound to
static const string kUserId = "user-8";

// If the above is a new user, then set this to true for one run of the app
static const bool kIsNewUser = false;

@interface View ()
@property (nonatomic) shared_ptr<ModelMediator> modelMediator;
@end

@implementation View

- (id)initWithCoder:(NSCoder *)coder
{
    self = [super initWithCoder:coder];
    
    [self addTextView];
    [self initializeModel];
    
    return self;
}

#pragma mark - Text View

- (void)addTextView
{
    NSTextView *textView = [[NSTextView alloc] initWithFrame:self.bounds];
    textView.editable = NO;
    [textView setTranslatesAutoresizingMaskIntoConstraints:NO];
    [self addSubview:textView];
    
    [self addConstraint:
     [NSLayoutConstraint constraintWithItem:textView
                                  attribute:NSLayoutAttributeWidth
                                  relatedBy:NSLayoutRelationEqual
                                     toItem:self
                                  attribute:NSLayoutAttributeWidth
                                 multiplier:1
                                   constant:0]];
    [self addConstraint:
     [NSLayoutConstraint constraintWithItem:textView
                                  attribute:NSLayoutAttributeHeight
                                  relatedBy:NSLayoutRelationEqual
                                     toItem:self
                                  attribute:NSLayoutAttributeHeight
                                 multiplier:0.9
                                   constant:0]];
}

- (void)setText:(string)text
{
    [[((NSTextView *)self.subviews[0]).textStorage mutableString] setString:@(text.c_str())];
}

- (void)refreshTextView
{
    stringstream ss;
    ModelRegistry::GetOrCreate<User>(kUserId)->Print(ss);
    [self setText:ss.str()];
}

#pragma mark - Initialization

- (void)initializeModel
{
    __weak View *weakSelf = self;
    self.modelMediator = ModelMediator::New();
    self.modelMediator->CreateCommandReceiver = CreateCommandReceiver;
    self.modelMediator->ParseCommandJSON = ParseCommandJSON;
    self.modelMediator->ModelDidChange = [weakSelf] {
        
        // User loaded
        if (auto user = ModelRegistry::GetOrCreate<User>(kUserId)) {
            if (!user->IsImpostor) {
                for (auto space : user->Spaces) {
                    weakSelf.modelMediator->EnsureLoadReceiver(space->Id);
                    if (!space->IsImpostor) {
                        for (auto idea : space->Ideas) {
                            weakSelf.modelMediator->EnsureLoadReceiver(idea->Id);
                        }
                    }
                }
            }
        }
        [weakSelf refreshTextView];
    };
    
    if (kIsNewUser) {
        self.modelMediator->ProcessCommand(make_shared<CreateUser>(kUserId));
    } else {
        self.modelMediator->EnsureLoadReceiver(kUserId);
    }
}

#pragma mark - User actions

- (void)addSpace
{
    if (auto user = ModelRegistry::GetOrCreate<User>(kUserId)) {
        if (!user->IsImpostor) {
            string spaceId = [self randomIdWithPrefix:"space-"];
            self.modelMediator->ProcessCommand(make_shared<CreateSpace>(spaceId));
            self.modelMediator->ProcessCommand(make_shared<InsertSpace>(kUserId, spaceId));
        }
    }
}

- (void)addIdea
{
    string spaceId;
    if ([self randomSpaceId:spaceId]) {
        const string ideaId = [self randomIdWithPrefix:"idea-"];
        self.modelMediator->ProcessCommand(make_shared<CreateIdea>(ideaId));
        self.modelMediator->ProcessCommand(make_shared<InsertIdea>(spaceId, ideaId));
    }
}

- (void)setIdeaText
{
    string ideaId;
    if ([self randomIdeaId:ideaId]) {
        auto attributes = IdeaAttributes { .Text = [self randomIdWithPrefix:"text-"] };
        self.modelMediator->ProcessCommand(make_shared<SetIdeaAttributes>(ideaId, attributes));
    }
}

#pragma mark - Mouse handler

// Every mouse click invokes a user action, rotating through
// (a) adding a space, (b) adding an idea, and (c) setting the text on
// an idea.
- (void)mouseDown:(NSEvent *)theEvent
{
    static int addIndex = 0;
    switch (addIndex) {
        case 0:
        {
            [self addSpace];
            break;
        }
        case 1:
        {
            [self addIdea];
            break;
        }
        default:
        {
            [self setIdeaText];
            break;
        }
    }
    addIndex = (addIndex + 1) % 10;
}

#pragma mark - Misc

// Of the spaces that are not currently impostors, return the id of a randomly
// selected space.
- (bool)randomSpaceId:(string &)spaceId
{
    vector<string> possibleSpaceIds;
    if (auto user = ModelRegistry::GetOrCreate<User>(kUserId)) {
        if (!user->IsImpostor) {
            for (auto space : user->Spaces) {
                if (!space->IsImpostor) {
                    possibleSpaceIds.push_back(space->Id);
                }
            }
        }
    }
    
    if (possibleSpaceIds.size() > 0) {
        spaceId = [self selectRandomIdFromList:possibleSpaceIds];
        return true;
    }
    return false;
}

// Of the ideas that are not currently impostors, return the id of a randomly
// selected idea.
- (bool)randomIdeaId:(string &)ideaId
{
    vector<string> possibleIdeaIds;
    
    if (auto user = ModelRegistry::GetOrCreate<User>(kUserId)) {
        if (!user->IsImpostor) {
            for (auto space : user->Spaces) {
                if (!space->IsImpostor) {
                    for (auto idea : space->Ideas) {
                        if (!idea->IsImpostor) {
                            possibleIdeaIds.push_back(idea->Id);
                        }
                    }
                }
            }
        }
    }
    
    if (possibleIdeaIds.size() > 0) {
        ideaId = [self selectRandomIdFromList:possibleIdeaIds];
        return true;
    }
    return false;
}

// Returns a new random id with the given prefix
- (string)randomIdWithPrefix:(string)prefix
{
    stringstream ss;
    while (ss.str().length() < 5)
    {
        char c = (char)rand();
        if (c >= 97 && c <= 122) {
            ss << c;
        }
    }
    return prefix + ss.str();
}

// Selects an id at random from the given list
- (string)selectRandomIdFromList:(vector<string>)ids
{
    if (ids.size() == 0) {
        return 0;
    }
    return ids[rand() % ids.size()];
}

@end
