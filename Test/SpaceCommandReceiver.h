//
//  Test.h
//  
//
//  Created by Julian Walker on 10/23/15.
//
//

#pragma once

#include <memory>
#include "CommandReceiver.h"

struct InsertIdea;

struct SpaceCommandReceiver : public CommandReceiverT<SpaceCommandReceiver>
{
    using Commands = std::tuple<InsertIdea>;
    
    static std::shared_ptr<SpaceCommandReceiver> New(const std::string &spaceId);
};

