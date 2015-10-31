//
//  UserCommandReceiver.hpp
//  Test
//
//  Created by Julian Walker on 10/30/15.
//  Copyright © 2015 Julian Walker. All rights reserved.
//

#pragma once

#include <memory>
#include "CommandReceiver.h"

struct InsertSpace;

struct UserCommandReceiver : public CommandReceiverT<UserCommandReceiver>
{
    using Commands = std::tuple<InsertSpace>;
    
    static std::shared_ptr<UserCommandReceiver> New(const std::string &userId);
};