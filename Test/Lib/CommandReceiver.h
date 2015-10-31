//
//  CommandReceiver.h
//  Test
//
//  Created by Julian Walker on 10/24/15.
//  Copyright (c) 2015 Julian Walker. All rights reserved.
//

#pragma once

#include <functional>
#include "Command.h"

///////////////////////////////////

template <typename Type, typename Collection>
struct CanExecuteCommand;

template <typename Type>
struct CanExecuteCommand<Type, std::tuple<>>
{
    // If we reach this point, then we didn't match any command types in the list
    typedef std::false_type result;
};

template <typename Type, typename ... Others>
struct CanExecuteCommand<Type, std::tuple<Type, Others...>>
{
    // If we reach this point, then we have a direct match to a command type in the list
    typedef std::true_type result;
};

template <typename First, typename ... Others>
struct CanExecuteCommand<Command, std::tuple<First, Others...>>
{
    // We always accept the Command type. Whether the command is supported is determined
    // at runtime.
    typedef std::true_type result;
};

template <typename Type, typename First, typename ... Others>
struct CanExecuteCommand<Type, std::tuple<First, Others...>>
{
    // Recurse to see if we find a match for Type in the list of command types
    typedef typename CanExecuteCommand<Type, std::tuple<Others...>>::result result;
};

///////////////////////////////////

struct CommandReceiver
{
    virtual void Execute(const std::shared_ptr<Command> &command) = 0;
};

template<typename DerivedT>
struct CommandReceiverT
{
    template<typename CommandT>
    bool Execute(const std::shared_ptr<CommandT> &command)
    {
        static_assert(CanExecuteCommand<CommandT, typename DerivedT::Commands>::result::value,
                      "Receiver is not spec'd to receive command");
        return _Execute(command);
    }
    
private:
    
    template<typename CommandT>
    bool _Execute(const std::shared_ptr<CommandT> &);
};

// This class can be used to abstract away a given command receiver so that
// the party executing the command doesn't need to know the receiver's type.
class OpaqueCommandReceiver
{
public:
    
    OpaqueCommandReceiver() {}
    
    template<class ReceiverT>
    OpaqueCommandReceiver(const std::shared_ptr<ReceiverT> &receiver)
    {
        _Execute = [receiver](const std::shared_ptr<Command> &command) {
            return receiver->Execute(command);
        };
    }
    
    bool IsInitialized() const
    {
        return _Execute != nullptr;
    }
    
    bool Execute(const std::shared_ptr<Command> &command)
    {
        if (_Execute) {
            return _Execute(command);
        }
        return false;
    }
private:
    std::function<bool(const std::shared_ptr<Command> &command)> _Execute;
};
