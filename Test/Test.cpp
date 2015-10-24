//
//  Test.cpp
//  
//
//  Created by Julian Walker on 10/23/15.
//
//

#include "Test.h"

struct CommandReceiverImplA : public CommandReceiverA
{
    void Handle(const shared_ptr<CommandA> &command)
    {
        PRINT();
    }
    
    void Handle(const shared_ptr<CommandB> &command)
    {
        PRINT();
    }
};

shared_ptr<CommandReceiverA> CommandReceiverA::New()
{
    return make_shared<CommandReceiverImplA>();
}

template <typename CommandReceiverT, typename Commands>
struct instantiate_method;
template <typename CommandReceiverT>
struct instantiate_method<CommandReceiverT, std::tuple<>>
{
};
template <typename CommandReceiverT, typename First, typename ... Others>
struct instantiate_method<CommandReceiverT, std::tuple<First, Others...>>
{
    instantiate_method()
    {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-value"
        &CommandReceiverT::template CommandReceiver<CommandReceiverT>::template _Execute<First>;
#pragma clang diagnostic pop
    }
    instantiate_method<CommandReceiverT, std::tuple<Others...>> next;
};

void Blah()
{
    instantiate_method<CommandReceiverA, CommandReceiverA::Commands> a;
}

template<>
template<class CommandT>
void CommandReceiver<CommandReceiverA>::_Execute(const shared_ptr<CommandT> &command)
{
    static_cast<CommandReceiverImplA *>(this)->Handle(command);
}
