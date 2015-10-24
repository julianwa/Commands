//
//  main.m
//  Test
//
//  Created by Julian Walker on 10/23/15.
//  Copyright (c) 2015 Julian Walker. All rights reserved.
//

#include "Test.h"

int main(int argc, const char * argv[])
{
    CommandReceiverA::New()->Execute(make_shared<CommandA>());
    CommandReceiverA::New()->Execute(make_shared<CommandB>());
    return 0;
}
