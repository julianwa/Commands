//
//  SyncClient.hpp
//  Test
//
//  Created by Julian Walker on 10/31/15.
//  Copyright © 2015 Julian Walker. All rights reserved.
//

#pragma once

#include <memory>
#include <string>
#include "Channel.h"

struct SyncClient
{
    // Opens a channel. If the channel doesn't yet exist, it will be created when the first command
    // is published to it.
    virtual std::shared_ptr<Channel> OpenChannel(const std::string &id,
                                                 Channel::ParseCommandJSONFunc parseCommandJSON,
                                                 Channel::ReceiveCommandFunc receiveCommand) = 0;
    
    // Gets an open channel. If the channel hasn't been opened, returns nullptr.
    virtual std::shared_ptr<Channel> GetChannel(const std::string &id) = 0;
    
    static std::shared_ptr<SyncClient> New();
};