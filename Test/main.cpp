//
//  main.m
//  Test
//
//  Created by Julian Walker on 10/23/15.
//  Copyright (c) 2015 Julian Walker. All rights reserved.
//

#include "Test.h"
#include <vector>

using namespace std;

int main(int argc, const char * argv[])
{
    auto receiver = CommandReceiverA::New();
    
    auto commands = vector<shared_ptr<Command>> {
        make_shared<CommandA>(),
        make_shared<CommandB>(),
    };
    
    for (auto command : commands) {
        receiver->Execute(command);
    }
    
    receiver->Execute(make_shared<CommandC>());
    
    auto genericReceiver = GenericCommandReceiver(receiver);
    genericReceiver.Execute(make_shared<CommandA>());
    
    return 0;
}
