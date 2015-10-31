//
//  ModelMediator.cpp
//  Test
//
//  Created by Julian Walker on 10/30/15.
//  Copyright © 2015 Julian Walker. All rights reserved.
//

#include <assert.h>
#include <map>
#include "ModelMediator.h"
#include "ModelCommand.h"
#include "CommandReceiver.h"
#include "Channel.h"
#include "SyncClient.h"

using namespace std;

struct ModelMediatorImpl : public ModelMediator, public enable_shared_from_this<ModelMediatorImpl>
{
    shared_ptr<SyncClient> _SyncClient;
    map<string, OpaqueCommandReceiver> _Receivers;
    
    ModelMediatorImpl()
    : _SyncClient(SyncClient::New())
    {
    }
    
    string ChannelIdFromReceiverId(const string &receiverId)
    {
        return receiverId;
    }
    
    void EnsureLoadReceiver(const string &receiverId) override
    {
        if (!_SyncClient->GetChannel(receiverId)) {
            OpenChannel(ChannelIdFromReceiverId(receiverId));
        }
    }
    
    void ProcessCommand(const std::shared_ptr<Command> &command) override
    {
        const auto &modelCommand = dynamic_pointer_cast<ModelCommand>(command);
        const auto &receiverId = modelCommand->ReceiverId;
        
        // First, determine if this is a constructor command. If so, we'll have an initialized
        // command receiver corresponding to the receiver specified by the command.
        auto receiver = CreateCommandReceiver(modelCommand);
        if (receiver.IsInitialized()) {
            
            // Store the receiver
            assert(_Receivers.count(receiverId) == 0);
            _Receivers[receiverId] = receiver;

            // If the command originated locally, then we need to open a channel and submit the command to it.
            // Otherwise, the command came from the channel, so no further action need be taken.
            auto channelId = ChannelIdFromReceiverId(receiverId);
            if (modelCommand->Origin == ModelCommand::Origin::Local) {
                auto channel = OpenChannel(channelId);
                channel->SubmitCommand(modelCommand);
            } else {
                // If this command came from the remote, there should already exist a channel for it
                assert(_SyncClient->GetChannel(channelId));
            }
        } else {
            
            // This isn't a constructor command, so find the command receiver that should receive the
            // command and execute it against that receiver.
            
            if (_Receivers.count(receiverId) > 0) {
                receiver = _Receivers[receiverId];
                if (receiver.IsInitialized()) {
                    
                    // Execute the command against the receiver
                    receiver.Execute(modelCommand);
                    
                    // If this command originated locally, then we need to submit it to the channel.
                    // Otherwise, it came from the channel, so no further action need be taken.
                    if (modelCommand->Origin == ModelCommand::Origin::Local) {
                        auto channel = _SyncClient->GetChannel(receiverId);
                        assert(channel);
                        channel->SubmitCommand(modelCommand);
                    }
                } else {
                    // We should never have stored an un-inintialized receiver
                    assert(0);
                }
            } else {
                // We should not be receiving commands for a receiver that has not yet
                // been created. What's up?
                printf("Error: cannot accept command with no receiver\n");
            }
        }
        
        // Notify the listener that the model changed
        if (ModelDidChange) {
            ModelDidChange();
        }
    }
    
    shared_ptr<Channel> OpenChannel(const string &channelId)
    {
        auto weakSelf = weak_ptr<ModelMediatorImpl>(shared_from_this());
        auto channel = _SyncClient->OpenChannel(channelId,
                                                ParseCommandJSON,
                                                [weakSelf](const shared_ptr<ModelCommand> &command) {
                                                    auto strongSelf = weakSelf.lock();
                                                    if (!strongSelf) {
                                                        return;
                                                    }
                                                    strongSelf->ProcessCommand(command);
                                                });
        return channel;
    }
};

shared_ptr<ModelMediator> ModelMediator::New()
{
    return make_shared<ModelMediatorImpl>();
}


