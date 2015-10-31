//
//  ModelCommand.h
//  Test
//
//  Created by Julian Walker on 10/30/15.
//  Copyright © 2015 Julian Walker. All rights reserved.
//

#pragma once

#include "Command.h"
#include <string>

struct ModelCommand : public Command
{
    const std::string ReceiverId;
    
    enum class Origin { Local, Remote };
    const Origin Origin;
    
    virtual ~ModelCommand() {}
    
    ModelCommand(const std::string &receiverId, enum Origin origin=Origin::Local)
    : ReceiverId(receiverId)
    , Origin(origin)
    {
    }
    
    virtual std::string ToJSON() = 0;
};
