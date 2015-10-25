//
//  CommandReceiverImpl.h
//  
//
//  Created by Julian Walker on 10/24/15.
//
//

#pragma once

#include <memory>
#include <functional>
#include <assert.h>

// This class is used to recursively expand the command types list and instantiate
// any necessary template functions in order to satisfy the linker. By simply referring
// to the function pointers, the template functions will be instantiated.
template <typename CommandReceiverT, typename Commands>
struct InstantiateReceiverMethods;

template <typename CommandReceiverT>
struct InstantiateReceiverMethods<CommandReceiverT, std::tuple<>>
{
};

template <typename CommandReceiverT, typename First, typename ... Others>
struct InstantiateReceiverMethods<CommandReceiverT, std::tuple<First, Others...>>
{
    InstantiateReceiverMethods()
    {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-value"
        &CommandReceiverT::template CommandReceiverT<CommandReceiverT>::template _Execute<First>;
#pragma clang diagnostic pop
    }
    InstantiateReceiverMethods<CommandReceiverT, std::tuple<Others...>> Next;
};

// This class is used to recursively expand the command types list and attempt to dynamic_cast
// the generic command to each type. Rather than execute the command upon finding a match, we
// return a lambda so that the call stack of the command handler does not contain the command
// types list expansion.
template <typename T, typename Commands>
struct PossiblyExecute;

template<typename T>
struct PossiblyExecute<T, std::tuple<>>
{
    std::function<void(void)> operator()(const T *receiver,
                                         const shared_ptr<Command> &command)
    {
        return [] {
            printf("Command not supported\n");
            assert(0);
        };
    }
};

template <typename T, typename First, typename ... Others>
struct PossiblyExecute<T, std::tuple<First, Others...>>
{
    std::function<void(void)> operator()(T *receiver,
                                         const shared_ptr<Command> &command)
    {
        auto castCommand = dynamic_pointer_cast<First>(command);
        if (castCommand) {
            return [receiver, castCommand] {
                receiver->Handle(castCommand);
            };
        } else {
            return Next(receiver, command);
        }
    }
    PossiblyExecute<T, std::tuple<Others...>> Next;
};


// This macro needs to be included for every impl. It instantiates any template methods and
// provides the glue that binds the Execute template methods with the impl's Handle methods.

#define COMMAND_RECEIVER_IMPL(T)                                                            \
InstantiateReceiverMethods<T, T::Commands> sInstantiateReceiverMethods;                     \
                                                                                            \
template<>                                                                                  \
template<class CommandT>                                                                    \
void CommandReceiverT<T>::_Execute(const shared_ptr<CommandT> &command)                     \
{                                                                                           \
    static_cast<T##Impl *>(this)->Handle(command);                                          \
}                                                                                           \
                                                                                            \
template<>                                                                                  \
template<>                                                                                  \
void CommandReceiverT<T>::_Execute(const shared_ptr<Command> &command)                      \
{                                                                                           \
    PossiblyExecute<T##Impl, T::Commands>()(static_cast<T##Impl *>(this), command)();       \
}