#ifndef __ENTERROOM_SCENE_H__
#define __ENTERROOM_SCENE_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

class EnterRoom : public cocos2d::Layer
{
    public:
        EnterRoom();
        virtual ~EnterRoom();
    public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
#define MAX_CONTINE_LOAD_DAYS 7
   float s_scale;
   int s_no;
   int cur_days;
   int cur_status;
   ParticleSnow* _emitter;
   ParticleSnow* _emitter1;
   ParticleSnow* _emitter2;
   ParticleSnow* _emitter3;
   ParticleSnow* _emitter4;
   ParticleSnow* _emitter5;
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
   virtual bool init();
   void removeDayReward();
   void startEmitter();
   void stopEmitter(float t);
   void onButtonSignField(cocos2d::Ref* pSender,Widget::TouchEventType type);
   void message_callback(cocos2d::Ref* pSender,Widget::TouchEventType type);
   void prize_callback(cocos2d::Ref* pSender,Widget::TouchEventType type);
   void rank_callback(cocos2d::Ref* pSender,Widget::TouchEventType type);
   void active_callback(cocos2d::Ref* pSender,Widget::TouchEventType type);
   void mall_callback(cocos2d::Ref* pSender,Widget::TouchEventType type);
   void set_callback(cocos2d::Ref* pSender,Widget::TouchEventType type);
   void normal_callback(cocos2d::Ref* pSender,Widget::TouchEventType type);
   void compete_callback(cocos2d::Ref* pSender,Widget::TouchEventType type);
   void fastRace_callback(cocos2d::Ref* pSender,Widget::TouchEventType type);
   void myBaseInfo_callback(cocos2d::Ref* pSender,Widget::TouchEventType type);
   void back_callback(cocos2d::Ref* pSender,Widget::TouchEventType type);
   void rankBack_callback(cocos2d::Ref* pSender,Widget::TouchEventType type);
   void entranceToCommonField(float t);
   void commonFieldAction();
   void entranceToCompeteField(float t);
   void competeFieldAction();
   void entranceToFastRace(float t);
   void fastRaceAction();
   void allButtonAction(float t);
   void allButtonAction2(float t);
   void deletePaddle(float t);
   void deletePaddle1(float t);
   void call_gold_prize();
   void goldreceiveCallback(cocos2d::Ref* pSender);
   CREATE_FUNC(EnterRoom);
};
#endif  //__ENTERROOM_SCENE_H__

