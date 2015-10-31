//
//  SyncClient.cpp
//  Test
//
//  Created by Julian Walker on 10/31/15.
//  Copyright © 2015 Julian Walker. All rights reserved.
//

#include "SyncClient.h"
#include <map>
#include "assert.h"
#include "Channel.h"

using namespace std;

struct SyncClientImpl : public SyncClient
{
    map<string, shared_ptr<Channel>> _Channels;
    
    SyncClientImpl()
    {
    }
    
    shared_ptr<Channel> OpenChannel(const string &id,
                                    Channel::ParseCommandJSONFunc parseCommandJSON,
                                    Channel::ReceiveCommandFunc receiveCommand)
    {
        assert(_Channels.count(id) == 0);
        auto channel = Channel::New(id, parseCommandJSON, receiveCommand);
        _Channels[id] = channel;
        return channel;
    }
    
    shared_ptr<Channel> GetChannel(const std::string &id)
    {
        if (_Channels.count(id) > 0) {
            return _Channels[id];
        }
        return nullptr;
    }
};

shared_ptr<SyncClient> SyncClient::New()
{
    return make_shared<SyncClientImpl>();
}