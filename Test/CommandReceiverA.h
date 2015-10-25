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

// Commands can be forward declared
struct CommandA;
struct CommandB;
struct CommandC;
struct NoBaseClassCommand;

struct CommandReceiverA : public CommandReceiverT<CommandReceiverA>
{
    // The commands that are supported by this class. If you try to execute a
    // typed command against this interface you'll get a static assert in the
    // compilation unit that is invoking the execute. If you try to execute a
    // generic command against this interface you'll get a runtime error / warning.
    using Commands = std::tuple<CommandA, CommandB, CommandC, Commandifier<NoBaseClassCommand>>;
    
    static std::shared_ptr<CommandReceiverA> New();
};

