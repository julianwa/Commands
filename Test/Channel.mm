//
//  Channel.cpp
//  Test
//
//  Created by Julian Walker on 10/31/15.
//  Copyright © 2015 Julian Walker. All rights reserved.
//

#import "Channel.h"
#import <PubNub/PubNub.h>
#include <mutex>
#include <map>
#include "ModelCommand.h"
#include <time.h>

using namespace std;

int SenderId()
{
    static int senderId = 0;
    static once_flag once;
    call_once(once, [&] {
        senderId = rand();
    });
    return senderId;
}

using ReceiveMessage = function<void(string json)>;

void ProcessMessage(NSDictionary *message,
                    ReceiveMessage receiveMessage)
{
    
    NSInteger senderId = ((NSNumber *)message[@"senderId"]).intValue;
    if (senderId == SenderId())
    {
        // this client sent the message -- ignore
        return;
    }
    
    NSString *json = (NSString *)message[@"json"];
    if (![json isKindOfClass:NSString.class]) {
        return;
    }
    
    NSLog(@"receiveMessage: %@", message[@"json"]);
    receiveMessage(string(json.UTF8String));
}

@interface PubNubListener : NSObject <PNObjectEventListener>

- (void)addMessageReceiverId:(string)id
    messageReceiver:(ReceiveMessage)messageReceiver;


@end

@implementation PubNubListener {
    map<string, ReceiveMessage> _MessageReceivers;
}

- (void)addMessageReceiverId:(string)id
messageReceiver:(ReceiveMessage)messageReceiver
{
    _MessageReceivers[id] = messageReceiver;
}



- (void)client:(PubNub *)client didReceiveMessage:(PNMessageResult *)message
{
    if (message.data.subscribedChannel.UTF8String) {
        string channel = string(message.data.subscribedChannel.UTF8String);
        if (_MessageReceivers.count(channel)) {
            ProcessMessage(message.data.message, _MessageReceivers[channel]);
        }
    }
}

@end

PubNub *PubNubClient()
{
    static PubNub *client;
    static once_flag once;
    call_once(once, [&] {
        [PNLog enabled:NO];
        PNConfiguration *configuration = [PNConfiguration configurationWithPublishKey:@"pub-c-08289968-addd-423a-99ff-7c2e2388c3ca"
                                                                         subscribeKey:@"sub-c-c3b96a76-7ff9-11e5-9720-0619f8945a4f"];
        client = [PubNub clientWithConfiguration:configuration];
    });
    return client;
}

void RequestHistoryFromChannel(const string &channelId,
                               ReceiveMessage receiveMessage,
                               function<void(void)> completion)
{
    PubNub *client = PubNubClient();
    [client historyForChannel:@(channelId.c_str())
               withCompletion:^(PNHistoryResult *result, PNErrorStatus *status) {
                   
                   NSLog(@"History for channel:\"%s\" count:%d error:%@",
                         channelId.c_str(),
                         (int)result.data.messages.count,
                         status.errorData);
                   
                   if (result.data.messages.count == 0) {
                       RequestHistoryFromChannel(channelId, receiveMessage, nullptr);
                       return;
                   }
                   
                   // Unfortunately, PubNub doesn't guarantee strict ordering of published messages, history, etc.
                   // So, use timestamps to sort the history.
                   NSArray *sortedMessages = [result.data.messages sortedArrayUsingComparator:^NSComparisonResult(id  _Nonnull obj1, id  _Nonnull obj2) {
                       NSNumber *first = obj1[@"timestamp"];
                       NSNumber *second = obj2[@"timestamp"];
                       return [first compare:second];
                   }];
                   
                   for (NSDictionary *message in sortedMessages) {
                       ProcessMessage(message, receiveMessage);
                   }
                   
                   if (completion) {
                       completion();
                   }
               }];

}

void SubscribeToPubNubChannel(const string &channelId,
                              ReceiveMessage receiveMessage)
{
    PubNub *client = PubNubClient();
    static PubNubListener *listener;
    static once_flag once;
    call_once(once, [&] {
        listener = [[PubNubListener alloc] init];
        [client addListener:listener];
    });
    
    [listener addMessageReceiverId:channelId messageReceiver:receiveMessage];

    RequestHistoryFromChannel(channelId,
                              receiveMessage,
                              [client, channelId] {
                                  [client subscribeToChannels:@[@(channelId.c_str())]
                                                 withPresence:NO];
                              });
}

struct ChannelImpl : public Channel, public enable_shared_from_this<ChannelImpl>
{
    const string Id;
    const ParseCommandJSONFunc ParseCommandJSON;
    const ReceiveCommandFunc ReceiveCommand;
    
    ChannelImpl(const std::string &id,
                ParseCommandJSONFunc parseCommandJSON,
                ReceiveCommandFunc receiveCommand)
    : Id(id)
    , ParseCommandJSON(parseCommandJSON)
    , ReceiveCommand(receiveCommand)
    {
    }
    
    void Subscribe()
    {
        auto weakSelf = weak_ptr<ChannelImpl>(shared_from_this());
        SubscribeToPubNubChannel(Id, [weakSelf](string json) {
            auto strongSelf = weakSelf.lock();
            if (strongSelf) {
                auto command = strongSelf->ParseCommandJSON(json);
                if (!command) {
                    printf("ERROR: unable to parse command");
                    return;
                }
                strongSelf->ReceiveCommand(command);
            }
        });
    }
    
    void SubmitCommand(const std::shared_ptr<ModelCommand> &command)
    {
        auto client = PubNubClient();
        NSString *channel = @(Id.c_str());
        NSDictionary *message = @{ @"senderId" : @(SenderId()),
                                   @"timestamp" : @(NSDate.date.timeIntervalSince1970),
                                   @"json" : @(command->ToJSON().c_str())
                                   };
        [client publish:message
              toChannel:channel
         withCompletion:^(PNPublishStatus *status) {
             
             if (!status.isError) {
                 //NSLog(@"SUCCESS channel:\"%@\" message:\"%@\"", channel, message);
             }
             else {
                 NSLog(@"FAIL channel:\"%@\" message:\"%@\"", channel, message);
                 // Request can be resent using: [status retry];
             }
         }];
    }
};

shared_ptr<Channel> Channel::New(const std::string &id,
                                 ParseCommandJSONFunc parseCommandJSON,
                                 ReceiveCommandFunc receiveCommand)
{
    auto channel = make_shared<ChannelImpl>(id, parseCommandJSON, receiveCommand);
    channel->Subscribe();
    return channel;
}
