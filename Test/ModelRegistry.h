//
//  ModelRegistry.h
//  Test
//
//  Created by Julian Walker on 10/30/15.
//  Copyright © 2015 Julian Walker. All rights reserved.
//

#pragma once

#include <map>
#include <string>
#include "ModelObject.h"

class ModelRegistry
{
    using ObjectsByIdMap = std::map<std::string, std::shared_ptr<ModelObject>>;
    static ObjectsByIdMap &Map()
    {
        static ObjectsByIdMap map;
        return map;
    }
    
public:
    template<class T>
    static std::shared_ptr<T> GetOrCreate(const std::string &id)
    {
        auto it = Map().find(id);
        if (it != Map().end()) {
            return std::static_pointer_cast<T>(it->second);
        } else {
            auto modelObject = std::make_shared<T>(id);
            Map()[id] = modelObject;
            return modelObject;
        }
    }
};
