//
//  Commands.h
//  Test
//
//  Created by Julian Walker on 10/24/15.
//  Copyright (c) 2015 Julian Walker. All rights reserved.
//

#pragma once

#include "ModelCommand.h"
#include <string>
#include <sstream>

struct CreateUser : public ModelCommand
{
    CreateUser(const std::string &userId,
               enum Origin origin=Origin::Local)
    : ModelCommand(userId, origin) {}
    
    std::string ToJSON()
    {
        std::stringstream ss;
        ss << "{ \"type\":\"createUser\", \"receiverId\":\"" << ReceiverId << "\"}";
        return ss.str();
    }
};

struct CreateSpace : public ModelCommand
{
    CreateSpace(const std::string &spaceId,
                enum Origin origin=Origin::Local)
    : ModelCommand(spaceId, origin) {}
    
    std::string ToJSON()
    {
        std::stringstream ss;
        ss << "{ \"type\":\"createSpace\", \"receiverId\":\"" << ReceiverId << "\"}";
        return ss.str();
    }
};

struct InsertSpace : public ModelCommand
{
    const std::string SpaceId;
    
    InsertSpace(const std::string &userId,
                const std::string &spaceId,
                enum Origin origin=Origin::Local)
    : ModelCommand(userId, origin)
    , SpaceId(spaceId)
    {
    }
    
    std::string ToJSON()
    {
        std::stringstream ss;
        ss << "{ \"type\":\"insertSpace\", \"receiverId\":\"" << ReceiverId << "\", \"spaceId\":\"" << SpaceId << "\"}";
        return ss.str();
    }
};

struct CreateIdea : public ModelCommand
{
    CreateIdea(const std::string &ideaId,
               enum Origin origin=Origin::Local)
    : ModelCommand(ideaId, origin) {}
    
    std::string ToJSON()
    {
        std::stringstream ss;
        ss << "{ \"type\":\"createIdea\", \"receiverId\":\"" << ReceiverId << "\"}";
        return ss.str();
    }
};

struct InsertIdea : public ModelCommand
{
    const std::string IdeaId;
    
    InsertIdea(const std::string &spaceId,
               const std::string &ideaId,
               enum Origin origin=Origin::Local)
    : ModelCommand(spaceId, origin)
    , IdeaId(ideaId)
    {
    }
    
    std::string ToJSON()
    {
        std::stringstream ss;
        ss << "{ \"type\":\"insertIdea\", \"receiverId\":\"" << ReceiverId << "\", \"ideaId\":\"" << IdeaId << "\"}";
        return ss.str();
    }
};


struct IdeaAttributes
{
    std::string Text;
};

struct SetIdeaAttributes : public ModelCommand
{
    const IdeaAttributes Attributes;
    
    SetIdeaAttributes(const std::string &ideaId,
                      const IdeaAttributes &ideaAttributes,
                      enum Origin origin=Origin::Local)
    : ModelCommand(ideaId, origin)
    , Attributes(ideaAttributes)
    {
    }
    
    std::string ToJSON()
    {
        std::stringstream ss;
        ss << "{ \"type\":\"setIdeaAttributes\", \"receiverId\":\"" << ReceiverId << "\", \"ideaAttributes\":{" << "\"text\":\"" << Attributes.Text << "\"}}";
        return ss.str();
    }
};

