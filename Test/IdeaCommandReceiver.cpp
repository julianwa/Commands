//
//  IdeaCommandReceiver.cpp
//  Test
//
//  Created by Julian Walker on 10/30/15.
//  Copyright © 2015 Julian Walker. All rights reserved.
//

#include "IdeaCommandReceiver.h"

#include <iostream>
#include "IdeaCommandReceiver.h"
#include "CommandReceiverImpl.hpp"
#include "Commands.h"
#include "ModelRegistry.h"
#include "Model.h"

using namespace std;

struct IdeaCommandReceiverImpl : public IdeaCommandReceiver
{
    const shared_ptr<Idea> _Idea;

    IdeaCommandReceiverImpl(const string &ideaId)
    : _Idea(ModelRegistry::GetOrCreate<Idea>(ideaId))
    {
    }
    
    void Execute(const shared_ptr<SetIdeaAttributes> &command)
    {
        _Idea->Text = command->Attributes.Text;
    }
};

shared_ptr<IdeaCommandReceiver> IdeaCommandReceiver::New(const string &ideaId)
{
    return make_shared<IdeaCommandReceiverImpl>(ideaId);
}

COMMAND_RECEIVER_IMPL(IdeaCommandReceiver);