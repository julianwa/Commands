//
//  Test.cpp
//  
//
//  Created by Julian Walker on 10/23/15.
//
//

#include <iostream>
#include "SpaceCommandReceiver.h"
#include "CommandReceiverImpl.hpp"
#include "Commands.h"
#include "Model.h"
#include "ModelRegistry.h"

using namespace std;

struct SpaceCommandReceiverImpl : public SpaceCommandReceiver
{
    const shared_ptr<Space> _Space;

    SpaceCommandReceiverImpl(const string &spaceId)
    : _Space(ModelRegistry::GetOrCreate<Space>(spaceId))
    {
    }
    
    void Execute(const shared_ptr<InsertIdea> &command)
    {
        _Space->Ideas.push_back(ModelRegistry::GetOrCreate<Idea>(command->IdeaId));
        sort(_Space->Ideas.begin(), _Space->Ideas.end(),
             [](const shared_ptr<Idea> &a, const shared_ptr<Idea> &b) -> bool {
                 return a->Id < b->Id;
             });
    }
};

shared_ptr<SpaceCommandReceiver> SpaceCommandReceiver::New(const string &spaceId)
{
    return make_shared<SpaceCommandReceiverImpl>(spaceId);
}

COMMAND_RECEIVER_IMPL(SpaceCommandReceiver);
