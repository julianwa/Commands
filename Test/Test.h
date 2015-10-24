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

using namespace std;

#define PRINT() printf("%s\n", __PRETTY_FUNCTION__)

struct Command {
    // Adding a virtual destructor will make the Command type polymorphic
    virtual ~Command() {}
};

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

struct CommandA : public Command {};
struct CommandB : public Command {};
struct CommandC : public Command {};
struct CommandD : public Command {};

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
    GenericCommandReceiver(const shared_ptr<ReceiverT> &receiver)
    {
        _F = [receiver](const std::shared_ptr<Command> &command) {
            receiver->Execute(command);
        };
    }
    
    void operator()(const shared_ptr<Command> &command)
    {
        _F(command);
    }
private:
    std::function<void(const shared_ptr<Command> &command)> _F;
};
//template<typename CommandReceiverT, typename CommandT>
//std::function<void(const std::shared_ptr<CommandT> &command)> ExecutePromise(const std::shared_ptr<CommandReceiverT> &receiver)
//{
//    return [receiver](const shared_ptr<CommandT> &command) {
//        receiver->Execute(command);
//    };
//}

struct CommandReceiverA : public CommandReceiverT<CommandReceiverA>
{
    using Commands = std::tuple<CommandA, CommandB, CommandC>;
    
    static shared_ptr<CommandReceiverA> New();
};

