//
//  Test.h
//  
//
//  Created by Julian Walker on 10/23/15.
//
//

#pragma once

#include <memory>

using namespace std;

#define PRINT() printf("%s\n", __PRETTY_FUNCTION__)

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

template <typename Type, typename First, typename ... Others>
struct contains<Type, std::tuple<First, Others...>>
{
    typedef typename contains<Type, std::tuple<Others...>>::result result;
};

///////////////////////////////////

struct CommandA {};
struct CommandB {};

template<typename DerivedT>
struct CommandReceiver
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

struct CommandReceiverA : public CommandReceiver<CommandReceiverA>
{
    using Commands = std::tuple<CommandA, CommandB>;
    
    static shared_ptr<CommandReceiverA> New();
};

