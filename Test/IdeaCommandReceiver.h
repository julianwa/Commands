//
//  IdeaCommandReceiver.h
//  Test
//
//  Created by Julian Walker on 10/30/15.
//  Copyright © 2015 Julian Walker. All rights reserved.
//

#pragma once

#include <memory>
#include "CommandReceiver.h"

struct SetIdeaAttributes;

struct IdeaCommandReceiver : public CommandReceiverT<IdeaCommandReceiver>
{
    using Commands = std::tuple<SetIdeaAttributes>;
    
    static std::shared_ptr<IdeaCommandReceiver> New(const std::string &ideaId);
};

