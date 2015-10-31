//
//  CommandReceiverFactory.cpp
//  Test
//
//  Created by Julian Walker on 11/1/15.
//  Copyright © 2015 Julian Walker. All rights reserved.
//

#include "CommandReceiverFactory.h"
#include "IdeaCommandReceiver.h"
#include "SpaceCommandReceiver.h"
#include "UserCommandReceiver.h"
#include "Commands.h"
#include "Model.h"
#include "ModelRegistry.h"
#include <assert.h>

using namespace std;

OpaqueCommandReceiver CreateCommandReceiver(const shared_ptr<ModelCommand> &command)
{
    if (auto createUser = dynamic_pointer_cast<CreateUser>(command)) {
        auto user = ModelRegistry::GetOrCreate<User>(command->ReceiverId);
        assert(user->IsImpostor);
        user->IsImpostor = false;
        return OpaqueCommandReceiver(UserCommandReceiver::New(command->ReceiverId));
    } else if (auto createSpace = dynamic_pointer_cast<CreateSpace>(command)) {
        auto space = ModelRegistry::GetOrCreate<Space>(command->ReceiverId);
        assert(space->IsImpostor);
        space->IsImpostor = false;
        return OpaqueCommandReceiver(SpaceCommandReceiver::New(command->ReceiverId));
    } else if (auto createIdea = dynamic_pointer_cast<CreateIdea>(command)) {
        auto idea = ModelRegistry::GetOrCreate<Idea>(command->ReceiverId);
        assert(idea->IsImpostor);
        idea->IsImpostor = false;
        return OpaqueCommandReceiver(IdeaCommandReceiver::New(command->ReceiverId));
    }
    
    return OpaqueCommandReceiver();
}
