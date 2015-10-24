//
//  Test.cpp
//  
//
//  Created by Julian Walker on 10/23/15.
//
//

#include <typeinfo>
#include <iostream>
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
    
    void Handle(const shared_ptr<CommandC> &command)
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
        &CommandReceiverT::template CommandReceiverT<CommandReceiverT>::template _Execute<First>;
#pragma clang diagnostic pop
    }
    instantiate_method<CommandReceiverT, std::tuple<Others...>> next;
};

instantiate_method<CommandReceiverA, CommandReceiverA::Commands> a;


template<>
template<class CommandT>
void CommandReceiverT<CommandReceiverA>::_Execute(const shared_ptr<CommandT> &command)
{
    static_cast<CommandReceiverImplA *>(this)->Handle(command);
}

template <typename T, typename Commands>
struct PossiblyExecute;

template<typename T>
struct PossiblyExecute<T, std::tuple<>>
{
    void operator()(const CommandReceiverT<T> *receiver,
                    const shared_ptr<Command> &command)
    {
    }
};

template <typename T, typename First, typename ... Others>
struct PossiblyExecute<T, std::tuple<First, Others...>>
{
    void operator()(CommandReceiverT<T> *receiver,
                    const shared_ptr<Command> &command)
    {
        auto castCommand = dynamic_pointer_cast<First>(command);
        if (castCommand) {
            receiver->Execute(castCommand);
        } else {
            Next(receiver, command);
        }
    }
    PossiblyExecute<T, std::tuple<Others...>> Next;
};


template<>
template<>
void CommandReceiverT<CommandReceiverA>::_Execute(const shared_ptr<Command> &command)
{
    PossiblyExecute<CommandReceiverA, CommandReceiverA::Commands>()(this, command);
}
