
#ifndef __VOICE_EFFECT__
#define __VOICE_EFFECT__

#include "ui/CocosGUI.h"
using namespace CocosDenshion;

#include "./../utils/BasicType.h"

class VoiceEffect {
public:
    static VoiceEffect *getInstance();
    static void destroyInstance();
    
    CallFunc *SpeakCard(Card_t card,Sex_t sex = BOY);
    CallFunc *SpeakAction(Action_t id,Sex_t sex = BOY);
    CallFunc *Speak(const char *file);
private:
    VoiceEffect();
    static VoiceEffect *_instance;

    static int _motionToFile(const char *motion);
};

#endif