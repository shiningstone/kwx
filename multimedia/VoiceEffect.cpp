
#include "VoiceEffect.h"

const char * VoiceCards[SEX_MAX][CARD_MAX] = {
    {
        "Music/1tiao.ogg","Music/2tiao.ogg","Music/3tiao.ogg","Music/4tiao.ogg","Music/5tiao.ogg",
        "Music/6tiao.ogg","Music/7tiao.ogg","Music/8tiao.ogg","Music/9tiao.ogg",
        "Music/1tong.ogg","Music/2tong.ogg","Music/3tong.ogg","Music/4tong.ogg","Music/5tong.ogg",
        "Music/6tong.ogg","Music/7tong.ogg","Music/8tong.ogg","Music/9tong.ogg",
        "Music/zhong.ogg","Music/fa.ogg","Music/bai.ogg"
    },
    {
        "Music/g_1tiao.ogg","Music/g_2tiao.ogg","Music/g_3tiao.ogg","Music/g_4tiao.ogg","Music/g_5tiao.ogg",
        "Music/g_6tiao.ogg","Music/g_7tiao.ogg","Music/g_8tiao.ogg","Music/g_9tiao.ogg",
        "Music/g_1tong.ogg","Music/g_2tong.ogg","Music/g_3tong.ogg","Music/g_4tong.ogg","Music/g_5tong.ogg",
        "Music/g_6tong.ogg","Music/g_7tong.ogg","Music/g_8tong.ogg","Music/g_9tong.ogg",
        "Music/g_zhong.ogg","Music/g_fa.ogg","Music/g_bai.ogg"
    },
};

const char * VoiceActions[SEX_MAX][ACTION_MAX] = {
    {"Music/peng.ogg","Music/gang.ogg","Music/ting2.ogg","Music/hu.ogg"},
    {"Music/g_peng.ogg","Music/g_gang.ogg","Music/g_ting1.ogg","Music/g_hu.ogg"},
};

CallFunc* VoiceEffect::SpeakCard(Card_t card,Sex_t sex) {
    return CallFunc::create([=](){
        SimpleAudioEngine::sharedEngine()->playEffect(
            VoiceCards[sex][card]);});
}

CallFunc* VoiceEffect::SpeakAction(Action_t action,Sex_t sex) {
    return CallFunc::create([=](){
        SimpleAudioEngine::sharedEngine()->playEffect(
            VoiceActions[sex][action]);});
}

static int VoiceEffect::_motionToFile(char *file, const char *motion) {
    const char *DIR = "Music/";
    const char *EXT  = ".ogg";

    memcpy(file, DIR, strlen(DIR));
    memcpy(file+strlen(DIR), motion, strlen(motion));
    memcpy(file+strlen(DIR)+strlen(motion), EXT, strlen(EXT));
}


CallFunc* VoiceEffect::Speak(const char *motion) {
    char file[32] = {0};
    
    _motionToFile(file,motion);
    
	return CallFunc::create([=](){
        SimpleAudioEngine::sharedEngine()->playEffect(file);});
}

/*************************************
    singleton
*************************************/
VoiceEffect *VoiceEffect::getInstance() {
    if (_instance==NULL) {
        _instance = new VoiceEffect();
    }

    return _instance;
}

void VoiceEffect::destroyInstance() {
    delete _instance;
    _instance = NULL;
}

