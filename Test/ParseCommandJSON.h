//
//  ParseJSON.h
//  Test
//
//  Created by Julian Walker on 10/31/15.
//  Copyright © 2015 Julian Walker. All rights reserved.
//

#pragma once

#include <string>
#include <memory>

struct ModelCommand;

std::shared_ptr<ModelCommand> ParseCommandJSON(const std::string &json);

