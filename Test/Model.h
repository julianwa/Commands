//
//  Model.h
//  Test
//
//  Created by Julian Walker on 10/30/15.
//  Copyright © 2015 Julian Walker. All rights reserved.
//

#pragma once

#include "ModelObject.h"
#include <vector>
#include <sstream>

struct Idea : public ModelObject
{
    std::string Text;
    Idea(const std::string &ideaId) : ModelObject(ideaId) {}
    
    void Print(std::stringstream &ss,
               const std::string &prefix)
    {
        (ss << prefix << "Idea Id:\"" << Id << "\""
         << " IsImpostor:" << IsImpostor
         << " Text:\"" << Text << "\""
         << std::endl);
    }
};

struct Space : public ModelObject
{
    std::vector<std::shared_ptr<Idea>> Ideas;
    Space(const std::string &spaceId) : ModelObject(spaceId) {}
    
    void Print(std::stringstream &ss,
               const std::string &prefix)
    {
        (ss << prefix <<
         "Space Id:\"" << Id << "\""
         << " IsImpostor:" << IsImpostor
         << std::endl);
        
        for (auto idea : Ideas) {
            idea->Print(ss, prefix + "   ");
        }
    }
};

struct User : public ModelObject
{
    std::vector<std::shared_ptr<Space>> Spaces;
    User(const std::string &userId) : ModelObject(userId) {}
    
    void Print(std::stringstream &ss)
    {
        (ss << "User Id:\"" << Id << "\""
         << " IsImpostor:" << IsImpostor
         << std::endl);

        for (auto space : Spaces) {
            space->Print(ss, "   ");
        }
    }
};
