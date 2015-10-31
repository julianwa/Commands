//
//  CommandReceiverFactory.hpp
//  Test
//
//  Created by Julian Walker on 11/1/15.
//  Copyright © 2015 Julian Walker. All rights reserved.
//


#pragma once

#include <memory>
#include "CommandReceiver.h"

struct ModelCommand;

OpaqueCommandReceiver CreateCommandReceiver(const std::shared_ptr<ModelCommand> &);