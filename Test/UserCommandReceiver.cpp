//
//  UserCommandReceiver.cpp
//  Test
//
//  Created by Julian Walker on 10/30/15.
//  Copyright © 2015 Julian Walker. All rights reserved.
//

#include <iostream>
#include "UserCommandReceiver.h"
#include "CommandReceiverImpl.hpp"
#include "Commands.h"
#include "Model.h"
#include "ModelRegistry.h"

using namespace std;

struct UserCommandReceiverImpl : public UserCommandReceiver
{
    const shared_ptr<User> _User;

    UserCommandReceiverImpl(const string &userId)
    : _User(ModelRegistry::GetOrCreate<User>(userId))
    {
    }
    
    void Execute(const shared_ptr<InsertSpace> &command)
    {
        auto space = ModelRegistry::GetOrCreate<Space>(command->SpaceId);
        _User->Spaces.push_back(space);
        sort(_User->Spaces.begin(), _User->Spaces.end(),
             [](const shared_ptr<Space> &a, const shared_ptr<Space> &b) -> bool {
                 return a->Id < b->Id;
             });
    }
};

shared_ptr<UserCommandReceiver> UserCommandReceiver::New(const string &userId)
{
    return make_shared<UserCommandReceiverImpl>(userId);
}

COMMAND_RECEIVER_IMPL(UserCommandReceiver);
