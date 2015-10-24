//
//  Test.h
//  
//
//  Created by Julian Walker on 10/23/15.
//
//

#pragma once

#include <memory>
#include <functional>
#include "CommandReceiver.h"

using namespace std;

struct CommandA : public Command {};
struct CommandB : public Command {};
struct CommandC : public Command {};
struct CommandD : public Command {};

struct CommandReceiverA : public CommandReceiverT<CommandReceiverA>
{
    using Commands = std::tuple<CommandA, CommandB, CommandC>;
    
    static shared_ptr<CommandReceiverA> New();
};

