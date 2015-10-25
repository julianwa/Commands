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

template <class T>
struct Commandifier : public Command
{
    const T &Payload;
    Commandifier(const T &payload) : Payload(payload) {}
    virtual ~Commandifier() {}
    
    static std::shared_ptr<Commandifier> New(const T &payload)
    {
        return std::make_shared<Commandifier>(payload);
    }
};
