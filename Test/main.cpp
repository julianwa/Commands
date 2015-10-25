//
//  main.m
//  Test
//
//  Created by Julian Walker on 10/23/15.
//  Copyright (c) 2015 Julian Walker. All rights reserved.
//

#include "CommandReceiverA.h"
#include "Commands.h"
#include <vector>

using namespace std;

struct Payload {
    int Value;
};

int main(int argc, const char * argv[])
{
    auto receiver = CommandReceiverA::New();
    
    // 1) You can execute a typed command directly on the receiver
    receiver->Execute(make_shared<CommandA>());
    
    // 2) This would result in a static compile-time error because the receiver does
    //    not support CommmandD
    //receiver->Execute(make_shared<CommandD>());
    
    // 3) Or you can execute generic commands on the receiver. However, in this case
    //    you don't know whether the receiver supports the command until run-time.
    auto commands = vector<shared_ptr<Command>> {
        make_shared<CommandB>(),
        make_shared<CommandC>(),
    };
    for (auto command : commands) {
        receiver->Execute(command);
    }
    
    // 4) Or, in situations where the receiver's type needs to be factored out (for example,
    //    if you wanted to create a map of reciever id -> receiver), then you can use the
    //    OpaqueCommandReceiver helper.
    auto opaqueReceiver = OpaqueCommandReceiver(receiver);
    opaqueReceiver.Execute(make_shared<CommandA>());

    // 5) Lastly, you can, without too much hassle, wrap up an object that doesn't derive
    //    from Command and treat it as a commmand.
    auto command = Commandifier<NoBaseClassCommand>::New(NoBaseClassCommand());
    receiver->Execute(command);
    receiver->Execute(dynamic_pointer_cast<Command>(command));
    
    return 0;
}
