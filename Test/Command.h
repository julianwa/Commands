//
//  Command.h
//  Test
//
//  Created by Julian Walker on 10/24/15.
//  Copyright (c) 2015 Julian Walker. All rights reserved.
//

#pragma once

struct Command
{
    // Adding a virtual destructor will make the Command type polymorphic
    virtual ~Command() {}
};
