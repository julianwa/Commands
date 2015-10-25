//
//  Test.cpp
//  
//
//  Created by Julian Walker on 10/23/15.
//
//

#include <iostream>
#include "CommandReceiverA.h"
#include "CommandReceiverImpl.hpp"
#include "Commands.h"

#define PRINT() printf("%s\n", __PRETTY_FUNCTION__)

struct CommandReceiverAImpl : public CommandReceiverA
{
    void Handle(const shared_ptr<CommandA> &command)
    {
        PRINT();
    }
    
    void Handle(const shared_ptr<CommandB> &command)
    {
        PRINT();
    }
    
    void Handle(const shared_ptr<CommandC> &command)
    {
        PRINT();
    }
};

shared_ptr<CommandReceiverA> CommandReceiverA::New()
{
    return make_shared<CommandReceiverAImpl>();
}

COMMAND_RECEIVER_IMPL(CommandReceiverA);
