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
struct contains;

template <typename Type>
struct contains<Type, std::tuple<>>
{
    typedef std::false_type result;
};

template <typename Type, typename ... Others>
struct contains<Type, std::tuple<Type, Others...>>
{
    typedef std::true_type result;
};

template <typename First, typename ... Others>
struct contains<Command, std::tuple<First, Others...>>
{
    typedef std::true_type result;
};

template <typename Type, typename First, typename ... Others>
struct contains<Type, std::tuple<First, Others...>>
{
    typedef typename contains<Type, std::tuple<Others...>>::result result;
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
    void Execute(const std::shared_ptr<CommandT> &command)
    {
        static_assert(contains<CommandT, typename DerivedT::Commands>::result::value,
                      "Receiver is not spec'd to receive command");
        _Execute(command);
    }
    
private:
    
    template<typename CommandT>
    void _Execute(const std::shared_ptr<CommandT> &);
};

class GenericCommandReceiver
{
public:
    template<class ReceiverT>
    GenericCommandReceiver(const std::shared_ptr<ReceiverT> &receiver)
    {
        _F = [receiver](const std::shared_ptr<Command> &command) {
            receiver->Execute(command);
        };
    }
    
    void operator()(const std::shared_ptr<Command> &command)
    {
        _F(command);
    }
private:
    std::function<void(const std::shared_ptr<Command> &command)> _F;
};
