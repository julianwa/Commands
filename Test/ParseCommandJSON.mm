//
//  ParseJSON.cpp
//  Test
//
//  Created by Julian Walker on 10/31/15.
//  Copyright © 2015 Julian Walker. All rights reserved.
//

#include "ParseCommandJSON.h"
#include "Commands.h"
#import <Foundation/Foundation.h>

using namespace std;

template <typename T>
inline T *SafeCast(id obj)
{
    if ([obj isKindOfClass:[T class]]) {
        return static_cast<T *>(obj);
    }
    return nil;
}

std::shared_ptr<ModelCommand> ParseCommandJSON(const std::string &json)
{
    shared_ptr<ModelCommand> command;
    
    NSError *error;
    id JSON = [NSJSONSerialization JSONObjectWithData:[@(json.c_str()) dataUsingEncoding:NSUTF8StringEncoding]
                                              options:0
                                                error:&error];
    if (auto type = SafeCast<NSString>(JSON[@"type"])) {
        if (auto receiverId = SafeCast<NSString>(JSON[@"receiverId"])) {
            if ([type isEqualToString:@"createUser"]) {
                command = make_shared<CreateUser>(receiverId.UTF8String,
                                                  ModelCommand::Origin::Remote);
            } else if ([type isEqualToString:@"createSpace"]) {
                command = make_shared<CreateSpace>(receiverId.UTF8String,
                                                   ModelCommand::Origin::Remote);
            } else if ([type isEqualToString:@"createIdea"]) {
                command = make_shared<CreateIdea>(receiverId.UTF8String,
                                                  ModelCommand::Origin::Remote);
            } else if ([type isEqualToString:@"insertSpace"]) {
                if (auto spaceId = SafeCast<NSString>(JSON[@"spaceId"])) {
                    command = make_shared<InsertSpace>(receiverId.UTF8String,
                                                       spaceId.UTF8String,
                                                       ModelCommand::Origin::Remote);
                }
            } else if ([type isEqualToString:@"insertIdea"]) {
                if (auto ideaId = SafeCast<NSString>(JSON[@"ideaId"])) {
                    command = make_shared<InsertIdea>(receiverId.UTF8String,
                                                      ideaId.UTF8String,
                                                      ModelCommand::Origin::Remote);
                }
            } else if ([type isEqualToString:@"setIdeaAttributes"]) {
                if (auto ideaAttributes = SafeCast<NSDictionary>(JSON[@"ideaAttributes"])) {
                    if (auto text = SafeCast<NSString>(ideaAttributes[@"text"])) {
                        command = make_shared<SetIdeaAttributes>(receiverId.UTF8String,
                                                                 IdeaAttributes { .Text = text.UTF8String },
                                                                 ModelCommand::Origin::Remote);
                    }
                }
            }
        }
    }
    
    return command;
}
