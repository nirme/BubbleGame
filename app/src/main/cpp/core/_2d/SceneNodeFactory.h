#pragma once

#include "../ScriptNode.h"
#include "../ScriptParser.h"

#include "SceneNode.h"
#include "Entity.h"




namespace core
{

    enum SCENE_NODE_TYPE
    {
        SNT_SCENE_NODE,
        SNT_ENTITY,
    };


    class SceneNodeFactory
    {

        SceneNodeUPtr createSceneNode(ScriptNodePtr _scriptNode)
        {
            SceneNode node();
            _scriptNode->getValue("NodeName")

            ScriptParser::getSingleton()

            void setScale(const Vector2 &_scale);
            void serRotation(const Quaternion &_rotation);
            void setPosition(const Vector2 &_position);


            //std::string getName()
            //std::string getValue(const std::string& _attribute = "")
            //ScriptNodeListPtr getChildList()


        };


    public:

        SceneNodeUPtr createNode(ScriptNodePtr)
        {};


    };
}