//
//  ModelObject.h
//  Test
//
//  Created by Julian Walker on 10/30/15.
//  Copyright © 2015 Julian Walker. All rights reserved.
//

#pragma once

#include <string>

struct ModelObject
{
    const std::string Id;
    
    // If true, the object is known to exist, but we have not yet loaded it formally
    bool IsImpostor = true;
    
    ModelObject(const std::string &id) : Id(id) {}
};
