//
//  Test.cpp
//  
//
//  Created by Julian Walker on 10/23/15.
//
//

#include <assert.h>
#include <typeinfo>
#include <iostream>
#include "Test.h"
#include "CommandReceiverImpl.hpp"

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
