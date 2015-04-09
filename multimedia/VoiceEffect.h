
#ifndef __VOICE_EFFECT__
#define __VOICE_EFFECT__

#include "ui/CocosGUI.h"
using namespace CocosDenshion;
#include "cocos2d.h"
USING_NS_CC;
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace ui;

#include "./../utils/BasicType.h"

class VoiceEffect {
public:
    static VoiceEffect *getInstance();
    static void destroyInstance();
    
    CallFunc *SpeakCard(Card_t card,Sex_t sex = BOY);
    CallFunc *SpeakAction(ActionId_t id,Sex_t sex = BOY);
    CallFunc *Speak(const char *file);
private:
    VoiceEffect();
    static VoiceEffect *_instance;

    static Action_t _actionIdx(ActionId_t action);
    static int _motionToFile(char *file,const char *motion);
};

#endif
