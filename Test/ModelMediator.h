//
//  ModelMediator.hpp
//  Test
//
//  Created by Julian Walker on 10/30/15.
//  Copyright © 2015 Julian Walker. All rights reserved.
//

#pragma once

#include "CommandReceiver.h"
#include "ModelCommand.h"
#include <memory>
#include <functional>

struct Command;

struct ModelMediator
{
    // For every constructor command, should create and return a command receiver; if not a constructor command,
    // should return an uninitialized command receiver
    std::function<OpaqueCommandReceiver(const std::shared_ptr<ModelCommand> &)> CreateCommandReceiver = nullptr;
    
    // Called to parse a json string to turn it into a model command
    std::function<std::shared_ptr<ModelCommand>(const std::string &json)> ParseCommandJSON = nullptr;

    // Called whenever the model was changed
    std::function<void()> ModelDidChange = nullptr;
    
    virtual void EnsureLoadReceiver(const std::string &receiverId) = 0;
    
    virtual void ProcessCommand(const std::shared_ptr<Command> &command) = 0;
    
    static std::shared_ptr<ModelMediator> New();
};
