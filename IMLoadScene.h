#ifndef __IMLOAD_SCENE_H__
#define __IMLOAD_SCENE_H__

#include "cocos2d.h"

class IMLoad : public cocos2d::Layer
{
    public:
        IMLoad();
        virtual ~IMLoad();
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
   void enterRoom(float dt);
   void addRes(float t);
   float s_scale;
   int s_no;
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
   virtual bool init();
   CREATE_FUNC(IMLoad);
};
#endif  //__IMLOAD_SCENE_H__


