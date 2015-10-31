//
//  Channel.hpp
//  Test
//
//  Created by Julian Walker on 10/31/15.
//  Copyright © 2015 Julian Walker. All rights reserved.
//

#pragma once

#include <functional>
#include <memory>
#include <string>

struct ModelCommand;

struct Channel
{
    // Parse the given jason to turn it into a ModelCommand
    using ParseCommandJSONFunc = std::function<std::shared_ptr<ModelCommand>(const std::string &json)>;
    
    // Called when a command is received on the channel, both from history playback and from the
    // channel subscription
    using ReceiveCommandFunc = std::function<void(const std::shared_ptr<ModelCommand> &)>;
    
    // Submit a command to the channel
    virtual void SubmitCommand(const std::shared_ptr<ModelCommand> &command) = 0;
    
    static std::shared_ptr<Channel> New(const std::string &id,
                                        ParseCommandJSONFunc parseCommandJSON,
                                        ReceiveCommandFunc receiveCommand);
};
