#include "NetRaceLayer.h"
#include "HelloWorldScene.h"

#include "NetPlayer.h"
#include "./../Me.h"

using namespace cocos2d::ui;
USING_NS_CC;

NetRaceLayer::NetRaceLayer()
{
	s_scale=1.189;
	s_no=1;

    _roundManager = new RoundManager();
    _logger = LOGGER_REGISTER("NetRaceLayer");
}

NetRaceLayer::~NetRaceLayer()
{
	g_my_free->release();
	g_my_peng->release();
	g_my_angang->release();
	g_my_out->release();
	g_my_in->release();
	g_left_right_in->release();
	g_left_free->release();
	g_left_right_peng_out->release();
	g_left_right_angang->release();
	g_right_free->release();
	g_my_mask->release();
	g_my_ming_canPlay->release();
	g_my_ming_ting->release();
	g_my_kou->release();
	for(int i=0;i<TOTAL_CARD_KIND;i++)
	{
		g_small_card_kind[i]->release();
		g_small_cardKind_black[i]->release();
		g_mid_card_kind[i]->release();
		g_card_kind[i]->release();
	}

	//SimpleAudioEngine::sharedEngine()->end();
	_eventDispatcher->removeAllEventListeners();

    delete _roundManager;
    LOGGER_DEREGISTER(_logger);
}

bool NetRaceLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}
	srand(time(0));

    _roundManager->InitPlayers();

	create_race();

	race_start_again();

	return true;
}

void NetRaceLayer::distribute_event(const std::string event_type,void* val)
{
    LOGGER_WRITE("%s : %s",__FUNCTION__,event_type.c_str());
	_eventDispatcher->dispatchCustomEvent(event_type,val);
}

void NetRaceLayer::_GenerateIds(int ids[]) {
    ids[0]=rand()%16;
    ids[1]=17;
    
    do {
        ids[2]=rand()%16;
    } while( ids[2]==ids[0] );
}

void NetRaceLayer::_LoadPlayerInfo() {
    Database *database = Database::getInstance();
    
    int  ids[3] = {0};
    _GenerateIds(ids);
    
	for(int i=0;i<3;i++)
	{	
        _roundManager->_players[i]->set_player_id( ids[i] );

        UserProfile_t user = {0};
        database->GetUserProfile(ids[i],user);

		_roundManager->_players[i]->set_nick_name(user.name);
		_roundManager->_players[i]->set_photo(user.photo);
		_roundManager->_players[i]->set_property(user.property);
		_roundManager->_players[i]->set_sex(user.sex);
		_roundManager->_players[i]->set_language(user.language);
	}
}

void NetRaceLayer::create_race()
{
    LOGGER_WRITE("%s",__FUNCTION__);

	for(int k=0;k<3;k++)
        _roundManager->_ready[k] = false;

    _roundManager->_distributedNum = 0;
    
	ifGameStart=false;
	ifUpdateDuringEffect=false;
	ifMingMybeError=false;
	MyCardChoosedNum=-1;
	ifInsertCardsTime=false;
	ifInsertStopped=false;
	ifResourcePrepared=false;
	ifTuoGuan=false;
	ifMingTime=false;
	ifEffectTime=false;
	ifMyShowCardTime=false;
    
	premiumLeast=200;
	distributeCardPos=Vec2::ZERO;
	//ifEndGameChoose=false;
	//tempEffectNode=Node::create();

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	Sprite *sprite;
	if(s_no==1)
		sprite=Sprite::create("racetable.png");
	else
		sprite=Sprite::create("racetable2.png");

	sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	sprite->setScaleX(s_scale);
	sprite->setScaleY(s_scale);
	this->addChild(sprite, 0);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("gameprepareImage.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("tools.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("userhead.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("xzdd_prepare_pop_res.plist");
    
	auto center_bkg=Sprite::createWithSpriteFrameName("kawuxing.png");
	center_bkg->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	this->addChild(center_bkg,1,CENTER_BKG_TAG_ID);

	auto menu_bkg=Sprite::createWithSpriteFrameName("gongnenganniu.png");
	menu_bkg->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height-menu_bkg->getTextureRect().size.height/2+origin.y));
	this->addChild(menu_bkg,4,MENU_BKG_TAG_ID);

	float space,x,y;
	auto flag=Sprite::createWithSpriteFrameName("baomingbisai2.png");
	auto robot=Sprite::createWithSpriteFrameName("tuoguan.png");
	auto set=Sprite::createWithSpriteFrameName("shezhi.png");
	auto mall=Sprite::createWithSpriteFrameName("shangcheng2.png");
	auto back=Sprite::createWithSpriteFrameName("fanhui.png");
	y = menu_bkg->getTextureRect().size.height*3/5;
	space = menu_bkg->getTextureRect().size.width/11.0f;

	x = space*4/5;
	auto flagButton=Button::create("baomingbisai2.png","baomingbisai2.png","baomingbisai2.png",UI_TEX_TYPE_PLIST);
	flagButton->setAnchorPoint(Vec2(0,0.5));
	flagButton->setPosition(Vec2(x,y));
	menu_bkg->addChild(flagButton,1,BAOMING_MENU_BUTTON);

	x += flag->getTextureRect().size.width+space;
	auto robotButton=Button::create("tuoguan.png","tuoguan1.png","tuoguan1.png",UI_TEX_TYPE_PLIST);
	robotButton->setTouchEnabled(false);
	robotButton->setAnchorPoint(Vec2(0,0.5));
	robotButton->setPosition(Vec2(x,y));
	robotButton->addTouchEventListener(CC_CALLBACK_2(NetRaceLayer::tuoGuanPressed,this));
	menu_bkg->addChild(robotButton,1,TUOGUAN_MENU_BUTTON);

	x += robot->getTextureRect().size.width+space;
	auto setButton=Button::create("shezhi.png","shezhi.png","shezhi.png",UI_TEX_TYPE_PLIST);
	setButton->setAnchorPoint(Vec2(0,0.5));
	setButton->setPosition(Vec2(x,y));
	menu_bkg->addChild(setButton,1,SHEZHI_MENU_BUTTON);

	x += set->getTextureRect().size.width+space;
	auto mallButton=Button::create("shangcheng2.png","shangcheng2.png","shangcheng2.png",UI_TEX_TYPE_PLIST);
	mallButton->setAnchorPoint(Vec2(0,0.5));
	mallButton->setPosition(Vec2(x,y));
	menu_bkg->addChild(mallButton,1,SHOP_MENU_BUTTON);

	x+=mall->getTextureRect().size.width+space;
	auto backButton=Button::create("fanhui.png","fanhui.png","fanhui.png",UI_TEX_TYPE_PLIST);
	backButton->setAnchorPoint(Vec2(0,0.5));
	backButton->setPosition(Vec2(x,y));
	backButton->addTouchEventListener(CC_CALLBACK_2(NetRaceLayer::backPressed,this));
	menu_bkg->addChild(backButton,1,GAMEBACK_MENU_BUTTON);

    create_residue_cards();
	update_residue_cards(TOTAL_CARD_NUM - _roundManager->_distributedNum);
    _LoadPlayerInfo();

	space = 5 + residue_card_bkg->getTextureRect().size.width/5;
	_playerBkg[0]=Sprite::createWithSpriteFrameName("touxiangxinxikuang2.png");
	_playerBkg[0]->setAnchorPoint(Point(0.0f,1.0f));
	y=visibleSize.height-menu_bkg->getTextureRect().size.height-5-69;
	_playerBkg[0]->setPosition(Vec2(origin.x+visibleSize.width*387/1218-_playerBkg[0]->getContentSize().width,origin.y+visibleSize.height/2+_playerBkg[0]->getContentSize().height/2));
	this->addChild(_playerBkg[0],1,LEFT_IMG_BKG_TAG_ID);

	base_point[0].x=29+_playerBkg[0]->getTextureRect().size.width*1.5+origin.x;
	base_point[0].y=origin.y+visibleSize.height-30;
	river_point[0].x=origin.x+visibleSize.width*0.33;
	river_point[0].y=origin.y+visibleSize.height*0.64;

	_playerBkg[2]=Sprite::createWithSpriteFrameName("touxiangxinxikuang2.png");
	_playerBkg[2]->setAnchorPoint(Point(1.0f,1.0f));
	_playerBkg[2]->setPosition(Vec2(origin.x+visibleSize.width*851/1218+_playerBkg[2]->getContentSize().width*0.8,origin.y+visibleSize.height/2+_playerBkg[2]->getContentSize().height/2));
	this->addChild(_playerBkg[2],1,RIGHT_IMG_BKG_TAG_ID);

	base_point[2].x=1190-_playerBkg[2]->getTextureRect().size.width*2+origin.x+visibleSize.width*0.019+10;
	base_point[2].y=220;
	river_point[2].x=origin.x+visibleSize.width*0.67;
	river_point[2].y=origin.y+visibleSize.height*0.43;

	_playerBkg[1]=Sprite::createWithSpriteFrameName("touxiangxinxikuang.png");
	_playerBkg[1]->setAnchorPoint(Point(0.0f,0.0f));
	_playerBkg[1]->setPosition(Vec2(origin.x+visibleSize.width/2-_playerBkg[1]->getContentSize().width/2,origin.y+visibleSize.height*144/716-_playerBkg[1]->getContentSize().height*0.5));
	this->addChild(_playerBkg[1],1,MID_IMG_BKG_TAG_ID);

	lastwinner_point[0].x=29+_playerBkg[0]->getTextureRect().size.width/2;
	lastwinner_point[0].y=536-_playerBkg[0]->getTextureRect().size.height;
	lastwinner_point[1].x=49+_playerBkg[1]->getTextureRect().size.width;
	lastwinner_point[1].y=129+_playerBkg[1]->getTextureRect().size.height/2;
	lastwinner_point[2].x=1190-_playerBkg[2]->getTextureRect().size.width/2;
	lastwinner_point[2].y=536-_playerBkg[2]->getTextureRect().size.height;

	auto mic=Sprite::createWithSpriteFrameName("maikefeng.png");
	mic->setAnchorPoint(Point(0.0f,0.0f));
	mic->setPosition(Vec2(49,129+_playerBkg[1]->getTextureRect().size.height));
	this->addChild(mic,1,MIC_TAG_ID);
	mic->setScale(0);

	base_point[1].x=visibleSize.width*0.06+origin.x;
	base_point[1].y=origin.y;
	river_point[1].x=origin.x+visibleSize.width*0.412;
	river_point[1].y=origin.y+visibleSize.height*0.38;

	auto danji_font=Sprite::createWithSpriteFrameName("danjiban.png");
	danji_font->setOpacity(100);
	danji_font->setAnchorPoint(Vec2(1,0));
	danji_font->setPosition(Vec2(origin.x+visibleSize.width-10,origin.y+10));
	this->addChild(danji_font,1,SINGLE_PLAY_TAG_ID);

	auto StartButton=Button::create("kaishiyouxi1.png","kaishiyouxi2.png","kaishiyouxi2.png",UI_TEX_TYPE_PLIST);
	StartButton->setAnchorPoint(Vec2(0.5,0.5));
	StartButton->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
	StartButton->addTouchEventListener(CC_CALLBACK_2(NetRaceLayer::start_touchCallBack,this));
	this->addChild(StartButton,2,START_GAME_TAG_ID);
    
	for(int id=0;id<3;id++)//更新玩家信息
	{
        std::string buffer;

		_roundManager->_players[id]->get_photo(buffer);
		update_headimage(id,buffer);
        
		_roundManager->_players[id]->get_nick_name(buffer);
		update_nickname(id,buffer);

        int score;
        _roundManager->_players[id]->get_property(score);
		GuiUpdateScore(id,score);
	}
    
	auto mapai_left=Sprite::createWithSpriteFrameName("shuban.png");//码牌
	mapai_left->setAnchorPoint(Vec2(0.5,0.5));
	mapai_left->setScale(0.8);
	mapai_left->setPosition(Vec2(origin.x+visibleSize.width*419/1218,origin.y+visibleSize.height*390/716));
	this->addChild(mapai_left,2,CARD_ARRAGE_LEFT_TAG_ID);

	auto mapai_top=Sprite::createWithSpriteFrameName("hengban.png");
	mapai_top->setAnchorPoint(Vec2(0.5,0.5));
	mapai_top->setScale(0.8);
	mapai_top->setPosition(Vec2(origin.x+visibleSize.width*650/1218,origin.y+visibleSize.height*498/716));
	this->addChild(mapai_top,2,CARD_ARRAGE_TOP_TAG_ID);

	auto mapai_right=Sprite::createWithSpriteFrameName("shuban.png");
	mapai_right->setAnchorPoint(Vec2(0.5,0.5));
	mapai_right->setScale(0.8);
	mapai_right->setPosition(Vec2(origin.x+visibleSize.width*799/1218,origin.y+visibleSize.height*326/716));
	this->addChild(mapai_right,2,CARD_ARRAGE_RIGHT_TAG_ID);

	auto mapai_bottom=Sprite::createWithSpriteFrameName("hengban.png");
	mapai_bottom->setAnchorPoint(Vec2(0.5,0.5));
	mapai_bottom->setScale(0.8);
	mapai_bottom->setPosition(Vec2(origin.x+visibleSize.width*568/1218,origin.y+visibleSize.height*218/716));
	this->addChild(mapai_bottom,2,CARD_ARRAGE_BOTTOM_TAG_ID);

	g_my_free=CCSpriteBatchNode::create("tileImage/me-handin.png");
	g_my_free->retain();	
	g_my_ming_canPlay=CCSpriteBatchNode::create("tileImage/Ming-bkg.png");
	g_my_ming_canPlay->retain();
	g_my_ming_ting=CCSpriteBatchNode::create("tileImage/me-ming.png");
	g_my_ming_ting->retain();
	g_my_peng=CCSpriteBatchNode::create("tileImage/me-peng.png");
	g_my_peng->retain();
	g_my_angang=CCSpriteBatchNode::create("tileImage/me-angang.png");
	g_my_angang->retain();
	g_my_out=CCSpriteBatchNode::create("tileImage/me-out.png");
	g_my_out->retain();
	g_my_in=CCSpriteBatchNode::create("tileImage/me-in.png");
	g_my_in->retain();
	g_left_free=CCSpriteBatchNode::create("tileImage/left-handin.png");
	g_left_free->retain();
	g_left_right_peng_out=CCSpriteBatchNode::create("tileImage/lr-out.png");
	g_left_right_peng_out->retain();
	g_left_right_in=CCSpriteBatchNode::create("tileImage/lr-in.png");
	g_left_right_in->retain();
	g_left_right_angang=CCSpriteBatchNode::create("tileImage/lr-angang.png");
	g_left_right_angang->retain();	
	g_right_free=CCSpriteBatchNode::create("tileImage/right-handin.png");
	g_right_free->retain();
	g_my_mask=CCSpriteBatchNode::create("tileImage/ming-mask.png");
	g_my_mask->retain();
	g_my_kou=CCSpriteBatchNode::create("tileImage/ming-kou.png");
	g_my_kou->retain();
    
	for(int i=0;i<TOTAL_CARD_KIND;i++)
	{
		g_small_card_kind[i]=CCSpriteBatchNode::create(String::createWithFormat("tileImage/tile_Up_%d.png",(int)(i+1))->getCString());
		g_small_cardKind_black[i]=CCSpriteBatchNode::create(String::createWithFormat("tileImage/tile_Upblack_%d.png",(int)(i+1))->getCString());
		g_mid_card_kind[i]=CCSpriteBatchNode::create(String::createWithFormat("tileImage/tile_Set_%d.png",(int)(i+1))->getCString());
		g_card_kind[i]=CCSpriteBatchNode::create(String::createWithFormat("tileImage/tile_%d.png",(int)(i+1))->getCString());
		g_small_card_kind[i]->retain();
		g_small_cardKind_black[i]->retain();
		g_mid_card_kind[i]->retain();
		g_card_kind[i]->retain();
	}
}

bool NetRaceLayer::resource_prepare()
{
	auto MingSignForLeft=Sprite::create("tileImage/Tile_mingOther.png");
	MingSignForLeft->setAnchorPoint(Vec2(0.5,0));
	MingSignForLeft->setPosition(Vec2(origin.x+visibleSize.width*85/1218,origin.y+visibleSize.height*536/716));
	MingSignForLeft->setVisible(false);
	this->addChild(MingSignForLeft,2,MING_STATUS_PNG_0);
	auto MingSignForRight=Sprite::create("tileImage/Tile_mingOther.png");
	MingSignForRight->setAnchorPoint(Vec2(0.5,0));
	MingSignForRight->setPosition(Vec2(origin.x+visibleSize.width*1140/1218,origin.y+visibleSize.height*536/716));
	MingSignForRight->setVisible(false);
	this->addChild(MingSignForRight,2,MING_STATUS_PNG_2);
	auto MingSignForMe=Sprite::create("tileImage/Tile_mingMe.png");
	MingSignForMe->setAnchorPoint(Vec2(0,0.5));
	MingSignForMe->setVisible(false);
	this->addChild(MingSignForMe,2,MING_STATUS_PNG_1);

	last_winner=Sprite::create("tileImage/zhuang.png");
	last_winner->setScale(0);
	this->addChild(last_winner,1,LAST_WINNER_TAG_ID);

	auto clock=Sprite::createWithSpriteFrameName("naozhongzhong.png");
	clock->setAnchorPoint(Vec2(0.5,0.5));
	clock->setPosition(Vec2(origin.x+visibleSize.width*0.5,origin.y+visibleSize.height*0.65));
	this->addChild(clock,3,ALARM_CLOCK_INDICATOR_TAG_ID);
	clock->setVisible(false);

	auto tag1=Sprite::createWithSpriteFrameName("naozhongzhishixia.png");
	tag1->setAnchorPoint(Vec2(0.5,0.5));
	tag1->setPosition(Vec2(clock->getPositionX(),clock->getPositionY()-60));
	this->addChild(tag1,3,ALARM_CLOCK_INDICATE_DOWN_TAG_ID);
	tag1->setVisible(false);

	auto tag2=Sprite::createWithSpriteFrameName("naozhongzhishiyou.png");
	tag2->setAnchorPoint(Vec2(0.5,0.5));
	tag2->setPosition(Vec2(clock->getPositionX()+60,clock->getPositionY()));
	this->addChild(tag2,4,ALARM_CLOCK_INDICATE_RIGHT_TAG_ID);
	tag2->setVisible(false);

	auto tag3=Sprite::createWithSpriteFrameName("naozhongzhishizuo.png");
	tag3->setAnchorPoint(Vec2(0.5,0.5));
	tag3->setPosition(Vec2(clock->getPositionX()-60,clock->getPositionY()));
	this->addChild(tag3,4,ALARM_CLOCK_INDICATE_LEFT_TAG_ID);
	tag3->setVisible(false);

	auto show_card_indicator=Sprite::create("tileImage/tile_pointer.png");
	show_card_indicator->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
	this->addChild(show_card_indicator,5,SHOWCARD_INDICATOR_TAG_ID);
	show_card_indicator->setVisible(false);

	return true;
}

void NetRaceLayer::startParticleSystem(float delta)
{
	auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
	auto m_Smoke = ParticleSystemQuad::create("Smoke.plist");
	m_Smoke->setBlendAdditive(true);
	m_Smoke->setPosition(Vec2(visibleSize.width/2,visibleSize.height/3));
	myframe->addChild(m_Smoke,20,PARTICLE_SOMKE);
	m_Smoke->setLife(0.6);
	m_Smoke->setEmissionRate(2400);
	m_Smoke->setDuration(0.18);
	m_Smoke->isAutoRemoveOnFinish();

	auto m_SmallParticle = CCParticleSystemQuad::create("star.plist");
	m_SmallParticle->setBlendAdditive(true);
	m_SmallParticle->setPosition(Vec2(visibleSize.width/2,visibleSize.height/3));
	myframe->addChild(m_SmallParticle, 20,PARTICLE_SMALL);
	m_SmallParticle->setLife(0.42);
	m_SmallParticle->setStartSize(40);
	m_SmallParticle->setEmissionRate(80);
	m_SmallParticle->setSpeed(500);
	m_SmallParticle->setSpeedVar(-5);
	m_SmallParticle->setDuration(0.18);
	m_SmallParticle->setStartColor(Color4F(1,1,1,1));
	m_SmallParticle->setStartColorVar(Color4F(0,0,0,0));
	m_SmallParticle->setEndColor(Color4F(0,0,0,0));
	m_SmallParticle->setEndColorVar(Color4F(1,1,1,1));
	m_SmallParticle->isAutoRemoveOnFinish();
}

void NetRaceLayer::PengPressed(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
{
	auto curButton=(Button*)pSender;
    
	switch (type)
	{
    	case cocos2d::ui::Widget::TouchEventType::BEGAN:
    		curButton->setScale(1.2);
    		break;
    	case cocos2d::ui::Widget::TouchEventType::MOVED:
    		break;
    	case cocos2d::ui::Widget::TouchEventType::ENDED:
    		{
                LOGGER_WRITE("%s",__FUNCTION__);
                
    			if(_roundManager->_isWaitDecision) {
    				_roundManager->_isWaitDecision = false;
    				_roundManager->_actionToDo = _roundManager->_tempActionToDo;
    				_roundManager->_tempActionToDo = a_JUMP;
    			}

    			curButton->setTouchEnabled(false);
                
    			auto effect = CCCallFuncN::create(this,callfuncN_selector(NetRaceLayer::PengEffect));
                curButton->_ID = 1;
    			curButton->runAction(Sequence::create(
                    ScaleTo::create(0.1,1),
                    effect,NULL));
    		}
    		break;
    	case cocos2d::ui::Widget::TouchEventType::CANCELED:
    		curButton->setScale(1);
    		break;
    	default:
    		break;
	}
}

void NetRaceLayer::HuPressed(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
{
	_roundManager->_actionToDo = a_HU;
    
	auto curButton=(Button*)pSender;
	int no = pSender->_ID;

    switch(type) {
    	case cocos2d::ui::Widget::TouchEventType::BEGAN:
    		curButton->setScale(1.2);
    		break;
    	case cocos2d::ui::Widget::TouchEventType::MOVED:
    		break;
    	case cocos2d::ui::Widget::TouchEventType::ENDED:
    		{
                LOGGER_WRITE("%s",__FUNCTION__);
                
    			if(_roundManager->_isWaitDecision) {
    				_roundManager->_isWaitDecision = false;
    				_roundManager->_actionToDo = _roundManager->_tempActionToDo;
    				_roundManager->_tempActionToDo = a_JUMP;
    			}
                
    			curButton->setTouchEnabled(false);

    			if(_roundManager->_isQiangGangAsking) {
    				_roundManager->_lastActionWithGold = a_QIANG_GANG;
                    
    				curButton->_ID = pSender->_ID;
    				auto clear  = CCCallFunc::create(this,callfunc_selector(NetRaceLayer::delete_act_tip));
    				auto effect = CCCallFuncN::create(this,callfuncN_selector(NetRaceLayer::hu_tip_effect));
                    
    				curButton->runAction(Sequence::create(
                        clear,
                        ScaleTo::create(0.1,1),
                        effect,NULL));
    			} 
                else if(_roundManager->_isDoubleHuAsking) {
    				auto ButtonAct = TargetedAction::create(curButton,ScaleTo::create(0.1,1));

    				auto myframe = this->getChildByTag(GAME_BKG_TAG_ID);
    				auto shade_act = (Sprite*)myframe->getChildByTag(HU_REMIND_ACT_TAG_ID);
    				auto fadeOut = FadeOut::create(0.3);
    				auto easeBounce = ScaleTo::create(0.3,1.5);
    				auto spawn = Spawn::create(fadeOut,easeBounce,NULL);
    				auto seq = Sequence::create(ScaleTo::create(0,1),spawn,NULL);
    				auto shadeAction = TargetedAction::create(shade_act,seq);

    				auto clear = CCCallFunc::create(this,callfunc_selector(NetRaceLayer::delete_act_tip));

    				auto huFunc = CallFunc::create([=](){	
    					hu_effect_tip(3);
    					distribute_event(DOUBLE_HU_WITH_ME,NULL);});
                        
					myframe->runAction(Sequence::create(
                        ButtonAct,
                        shadeAction,
                        clear,
                        huFunc,NULL));
    			} 
                else {
    				curButton->_ID = pSender->_ID;
    				auto effect = CCCallFuncN::create(this,callfuncN_selector(NetRaceLayer::hu_tip_effect));
                    
    				curButton->runAction(Sequence::create(
                        ScaleTo::create(0.1,1),
                        effect,NULL));
    			}
    			break;
    		}
    	case cocos2d::ui::Widget::TouchEventType::CANCELED:
    		curButton->setScale(1);
    		break;
	}
}

void NetRaceLayer::GangPressed(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
{
	auto curButton=(Button*)pSender;

	switch (type){
    	case cocos2d::ui::Widget::TouchEventType::BEGAN:
    		curButton->setScale(1.2);
    		break;
    	case cocos2d::ui::Widget::TouchEventType::MOVED:
    		break;
    	case cocos2d::ui::Widget::TouchEventType::ENDED:
    		{
                LOGGER_WRITE("%s",__FUNCTION__);

    			if(_roundManager->_isGangAsking)//is this judgement neccessary?
    				_roundManager->_isGangAsking = false;
                
    			if(_roundManager->_isWaitDecision) {
    				_roundManager->_isWaitDecision=false;
    				_roundManager->_actionToDo = _roundManager->_tempActionToDo;
    				_roundManager->_tempActionToDo = a_JUMP;
    			}
                
    			curButton->setTouchEnabled(false);
    			curButton->_ID=1;
    			if( _roundManager->_actionToDo & a_AN_GANG || _roundManager->_actionToDo & a_SHOU_GANG ) {
    				auto angangEffect = CCCallFuncN::create(this,callfuncN_selector(NetRaceLayer::an_gang_tip_effect));
    				curButton->runAction(Sequence::create(
                        ScaleTo::create(0.1,1),
                        angangEffect,NULL));
    			}
    			else if( _roundManager->_actionToDo & a_MING_GANG ) {
    				auto minggangEffect = CCCallFuncN::create(this,callfuncN_selector(NetRaceLayer::ming_gang_tip_effect));
    				curButton->runAction(Sequence::create(
                        ScaleTo::create(0.1,1),
                        minggangEffect,NULL));
    			}
    		}
    		break;
    	case cocos2d::ui::Widget::TouchEventType::CANCELED:
    		curButton->setScale(1);
    		break;
    	default:
    		break;
	}
}

void NetRaceLayer::QiPressed(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
{
	auto curButton=(Button*)pSender;

	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		curButton->setScale(1.2);
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
            LOGGER_WRITE("%s",__FUNCTION__);

			curButton->setTouchEnabled(false);
			curButton->_ID=1;
			auto effect = CCCallFuncN::create(this,callfuncN_selector(NetRaceLayer::qi_tip_effect));
			curButton->runAction(Sequence::create(
                ScaleTo::create(0.1,1),
                effect,NULL));
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		curButton->setScale(1);
		break;
	default:
		break;
	}
}

BezierTo* NetRaceLayer::BizerMove1(outCardList* outCard,Vec2 location)
{
	ccBezierConfig config;
	if((outCard->length-1)<6)
	{
		if(location.y<=visibleSize.height*0.4)
		{
			if(location.x<visibleSize.width*0.4)
			{
				config.endPosition=Vec2(river_point[1].x+((outCard->length-1)*36),river_point[1].y);
				config.controlPoint_1=Vec2(location.x+30,location.y+30);
				config.controlPoint_2=Vec2(river_point[1].x+((outCard->length-1)*36)+100,river_point[1].y-100);
			}
			else if(location.x>=visibleSize.width*0.6)
			{
				config.endPosition=Vec2(river_point[1].x+((outCard->length-1)*36),river_point[1].y);
				config.controlPoint_1=Vec2(location.x-30,location.y-30);
				config.controlPoint_2=Vec2(river_point[1].x+((outCard->length-1)*36)+100,river_point[1].y-100);
			}
			else if(location.x>visibleSize.width*0.4 && location.x<visibleSize.width*0.6)
			{
				config.endPosition=Vec2(river_point[1].x+((outCard->length-1)*36),river_point[1].y);
				config.controlPoint_1=Vec2(location.x,location.y);
				config.controlPoint_2=Vec2(river_point[1].x+((outCard->length-1)*36),river_point[1].y);
			}
		}
		else if(location.y>=visibleSize.height*0.4)
		{
			if(location.x<=visibleSize.width*0.4)
			{
				config.endPosition=Vec2(river_point[1].x+((outCard->length-1)*36),river_point[1].y);
				config.controlPoint_1=Vec2(location.x+100,location.y-100);
				config.controlPoint_2=Vec2(river_point[1].x+((outCard->length-1)*36)+100,river_point[1].y+100);
			}
			else if(location.x>=visibleSize.width*0.6)
			{
				config.endPosition=Vec2(river_point[1].x+((outCard->length-1)*36),river_point[1].y);
				config.controlPoint_1=Vec2(location.x-100,location.y-100);
				config.controlPoint_2=Vec2(river_point[1].x+((outCard->length-1)*36)+100,river_point[1].y+100);
			}
			else if(location.x>visibleSize.width*0.4&&location.x<visibleSize.width*0.6)
			{
				config.endPosition=Vec2(river_point[1].x+((outCard->length-1)*36),river_point[1].y);
				config.controlPoint_1=Vec2(location.x,location.y);
				config.controlPoint_2=Vec2(river_point[1].x+((outCard->length-1)*36),river_point[1].y);
			}
		}
	}
	else if((outCard->length-1)<14)
	{
		if(location.y<=visibleSize.height*0.4)
		{
			if(location.x<visibleSize.width*0.4)
			{
				config.endPosition=Vec2(river_point[1].x+((outCard->length-8)*36),river_point[1].y-41);
				config.controlPoint_1=Vec2(location.x+30,location.y+30);
				config.controlPoint_2=Vec2(river_point[1].x+((outCard->length-8)*36)-100,river_point[1].y-141);
			}
			else if(location.x>=visibleSize.width*0.6)
			{
				config.endPosition=Vec2(river_point[1].x+((outCard->length-8)*36),river_point[1].y-41);
				config.controlPoint_1=Vec2(location.x-30,location.y-30);
				config.controlPoint_2=Vec2(river_point[1].x+((outCard->length-8)*36)+100,river_point[1].y-141);
			}
			else if(location.x>visibleSize.width*0.4 && location.x<visibleSize.width*0.6)
			{
				config.endPosition=Vec2(river_point[1].x+((outCard->length-8)*36),river_point[1].y-41);
				config.controlPoint_1=Vec2(location.x,location.y);
				config.controlPoint_2=Vec2(river_point[1].x+((outCard->length-8)*36),river_point[1].y-41);
			}
		}
		else if(location.y>=visibleSize.height*0.4)
		{
			if(location.x<visibleSize.width*0.4)
			{
				config.endPosition=Vec2(river_point[1].x+((outCard->length-8)*36),river_point[1].y-41);
				config.controlPoint_1=Vec2(location.x+30,location.y-30);
				config.controlPoint_2=Vec2(river_point[1].x+((outCard->length-8)*36)-100,river_point[1].y+141);
			}
			else if(location.x>=visibleSize.width*0.6)
			{
				config.endPosition=Vec2(river_point[1].x+((outCard->length-8)*36),river_point[1].y-41);
				config.controlPoint_1=Vec2(location.x-30,location.y-30);
				config.controlPoint_2=Vec2(river_point[1].x+((outCard->length-8)*36)+100,river_point[1].y+141);
			}
			else if(location.x>visibleSize.width*0.4 && location.x<visibleSize.width*0.6)
			{
				config.endPosition=Vec2(river_point[1].x+((outCard->length-8)*36),river_point[1].y-41);
				config.controlPoint_1=Vec2(location.x,location.y);
				config.controlPoint_2=Vec2(river_point[1].x+((outCard->length-8)*36),river_point[1].y-41);
			}
		}
	}
	else
	{
		if(location.y<=visibleSize.height*0.4)
		{
			if(location.x<visibleSize.width*0.4)
			{
				config.endPosition=Vec2(river_point[1].x+((outCard->length-1)-16)*36,river_point[1].y-82);
				config.controlPoint_1=Vec2(location.x+30,location.y+30);
				config.controlPoint_2=Vec2(river_point[1].x+((outCard->length-16)*36)-100,river_point[1].y-182);
			}
			else if(location.x>=visibleSize.width*0.6)
			{
				config.endPosition=Vec2(river_point[1].x+((outCard->length-1)-16)*36,river_point[1].y-82);
				config.controlPoint_1=Vec2(location.x-30,location.y-30);
				config.controlPoint_2=Vec2(river_point[1].x+((outCard->length-16)*36)+100,river_point[1].y-182);
			}
			else if(location.x>visibleSize.width*0.4 && location.x<visibleSize.width*0.6)
			{
				config.endPosition=Vec2(river_point[1].x+((outCard->length-1)-16)*36,river_point[1].y-82);
				config.controlPoint_1=Vec2(location.x,location.y);
				config.controlPoint_2=Vec2(river_point[1].x+((outCard->length-16)*36),river_point[1].y-82);
			}
		}
		else if(location.y>=visibleSize.height*0.4)
		{
			if(location.x<visibleSize.width*0.4)
			{
				config.endPosition=Vec2(river_point[1].x+((outCard->length-1)-16)*36,river_point[1].y-82);
				config.controlPoint_1=Vec2(location.x+30,location.y-30);
				config.controlPoint_2=Vec2(river_point[1].x+((outCard->length-16)*36)-100,river_point[1].y+182);
			}
			else if(location.x>=visibleSize.width*0.6)
			{
				config.endPosition=Vec2(river_point[1].x+((outCard->length-1)-16)*36,river_point[1].y-82);
				config.controlPoint_1=Vec2(location.x-30,location.y-30);
				config.controlPoint_2=Vec2(river_point[1].x+((outCard->length-16)*36)+100,river_point[1].y+182);
			}
			else if(location.x>visibleSize.width*0.4 && location.x<visibleSize.width*0.6)
			{
				config.endPosition=Vec2(river_point[1].x+((outCard->length-1)-16)*36,river_point[1].y-82);
				config.controlPoint_1=Vec2(location.x,location.y);
				config.controlPoint_2=Vec2(river_point[1].x+((outCard->length-16)*36),river_point[1].y-82);
			}
		}
	}
	BezierTo *action;
	if(ccpDistance(location,config.endPosition)<=200)
	{
		action=BezierTo::create(0.18,config);
	}
	else if(ccpDistance(location,config.endPosition)>200)
	{
		action=BezierTo::create(0.3,config);
	}
	return action;
}	

BezierTo* NetRaceLayer::BizerMove2(outCardList* outCard,Vec2 location,int time)
{
	ccBezierConfig config;
	if((outCard->length-1)<6)
	{
		if(location.x<visibleSize.width*0.4)
		{
			config.endPosition=Vec2(river_point[1].x+((outCard->length-1)*36),river_point[1].y);
			config.controlPoint_1=Vec2(location.x,200);
			config.controlPoint_2=Vec2(river_point[1].x+((outCard->length-1)*36)-100,river_point[1].y-100);
		}
		else if(location.x>=visibleSize.width*0.6)
		{
			config.endPosition=Vec2(river_point[1].x+((outCard->length-1)*36),river_point[1].y);
			config.controlPoint_1=Vec2(location.x,200);
			config.controlPoint_2=Vec2(river_point[1].x+((outCard->length-1)*36)+100,river_point[1].y-100);
		}
		else if(location.x>visibleSize.width*0.4 && location.x<visibleSize.width*0.6)
		{
			config.endPosition=Vec2(river_point[1].x+((outCard->length-1)*36),river_point[1].y);
			config.controlPoint_1=Vec2(location.x,200);
			config.controlPoint_2=Vec2(river_point[1].x+((outCard->length-1)*36),river_point[1].y);
		}
	}
	else if((outCard->length-1)<14)
	{
		if(location.x<visibleSize.width*0.4)
		{
			config.endPosition=Vec2(river_point[1].x+((outCard->length-8)*36),river_point[1].y-41);
			if(time==2)
				config.controlPoint_1=Vec2(location.x,200);
			else
				config.controlPoint_1=Vec2(location.x+30,200);
			config.controlPoint_2=Vec2(river_point[1].x+((outCard->length-8)*36)-100,river_point[1].y-141);
		}
		else if(location.x>=visibleSize.width*0.6)
		{
			config.endPosition=Vec2(river_point[1].x+((outCard->length-8)*36),river_point[1].y-41);
			config.controlPoint_1=Vec2(location.x,200);
			config.controlPoint_2=Vec2(river_point[1].x+((outCard->length-8)*36)+100,river_point[1].y-141);
		}
		else if(location.x>visibleSize.width*0.4 && location.x<visibleSize.width*0.6)
		{
			config.endPosition=Vec2(river_point[1].x+((outCard->length-8)*36),river_point[1].y-41);
			config.controlPoint_1=Vec2(location.x,200);
			config.controlPoint_2=Vec2(river_point[1].x+((outCard->length-8)*36),river_point[1].y-41);
		}
	}
	else
	{
		if(location.x<visibleSize.width*0.4)
		{
			config.endPosition=Vec2(river_point[1].x+((outCard->length-1)-16)*36,river_point[1].y-82);
			if(time==2)
				config.controlPoint_1=Vec2(location.x,200);
			else
				config.controlPoint_1=Vec2(location.x+30,200);
			config.controlPoint_2=Vec2(river_point[1].x+((outCard->length-16)*36)-100,river_point[1].y-182);
		}
		else if(location.x>=visibleSize.width*0.6)
		{
			config.endPosition=Vec2(river_point[1].x+((outCard->length-1)-16)*36,river_point[1].y-82);
			config.controlPoint_1=Vec2(location.x,200);
			config.controlPoint_2=Vec2(river_point[1].x+((outCard->length-16)*36)+100,river_point[1].y-182);
		}
		else if(location.x>visibleSize.width*0.4 && location.x<visibleSize.width*0.6)
		{
			config.endPosition=Vec2(river_point[1].x+((outCard->length-1)-16)*36,river_point[1].y-82);
			config.controlPoint_1=Vec2(location.x,200);
			config.controlPoint_2=Vec2(river_point[1].x+((outCard->length-16)*36),river_point[1].y-82);
		}
	}
	BezierTo *action;
	if(ccpDistance(location,config.endPosition)<=200)
	{
		if(time==3)
			action=BezierTo::create(0.18,config);
		else
			action=BezierTo::create(0.18,config);
	}
	else if(ccpDistance(location,config.endPosition)>200)
	{
		if(time==3)
			action=BezierTo::create(0.3,config);
		else
			action=BezierTo::create(0.3,config);
	}
	return action;
}

BezierTo* NetRaceLayer::OthersBizerMove(int no,outCardList* outCard)
{
	ccBezierConfig config;
	auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
    
	if(no==2)
	{
		int lenForPlayerTwo=_roundManager->_players[2]->get_parter()->get_card_list()->len;
		auto BizerPosForPlayerTwoTemp=myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+no*20+lenForPlayerTwo-1)->getPosition();
		auto BizerPosForPlayerTwo=Vec2(BizerPosForPlayerTwoTemp.x,BizerPosForPlayerTwoTemp.y);
		if((outCard->length-1)<6)
		{
			config.controlPoint_1=Vec2(BizerPosForPlayerTwo.x-100,BizerPosForPlayerTwo.y);
			config.controlPoint_2=Vec2(river_point[no].x,river_point[no].y+30*(outCard->length-1));
			config.endPosition=Vec2(river_point[no].x,river_point[no].y+30*(outCard->length-1));
		}
		else if((outCard->length-1)<14)
		{
			config.controlPoint_1=Vec2(BizerPosForPlayerTwo.x-100,BizerPosForPlayerTwo.y);
			config.controlPoint_2=Vec2(river_point[no].x+48,river_point[no].y+30*((outCard->length-1)-7));
			config.endPosition=Vec2(river_point[no].x+48,river_point[no].y+30*((outCard->length-1)-7));
		}
		else
		{
			config.controlPoint_1=Vec2(BizerPosForPlayerTwo.x-100,BizerPosForPlayerTwo.y);
			config.controlPoint_2=Vec2(river_point[no].x+96,river_point[no].y+30*((outCard->length-1)-16));
			config.endPosition=Vec2(river_point[no].x+96,river_point[no].y+30*((outCard->length-1)-16));
		}
	}
	else if(no==0)
	{
		auto l_list_len=_roundManager->_players[no]->get_parter()->get_card_list()->len-1;
		auto l_card=myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+no*20+l_list_len);
		if((outCard->length-1)<6)
		{
			config.controlPoint_1=Vec2(base_point[no].x+110,l_card->getPosition().y-35);
			config.controlPoint_2=Vec2(river_point[no].x,river_point[no].y-30*(outCard->length-1));
			config.endPosition=Vec2(river_point[no].x,river_point[no].y-30*(outCard->length-1));
		}
		else if((outCard->length-1)<14)
		{
			config.controlPoint_1=Vec2(base_point[no].x+110,l_card->getPosition().y-35);
			config.controlPoint_2=Vec2(river_point[no].x-48,river_point[no].y-30*((outCard->length-1)-7));
			config.endPosition=Vec2(river_point[no].x-48,river_point[no].y-30*((outCard->length-1)-7));
		}
		else
		{
			config.controlPoint_1=Vec2(base_point[no].x+110,l_card->getPosition().y-35);
			config.controlPoint_2=Vec2(river_point[no].x-96,river_point[no].y-30*((outCard->length-1)-16));
			config.endPosition=Vec2(river_point[no].x-96,river_point[no].y-30*((outCard->length-1)-16));
		}
	}
	auto action=BezierTo::create(0.3,config);
	return action;
}

void NetRaceLayer::_CollectResouce(HAH *res) {
	memset(res,0,sizeof(HAH));
	memset(res->card_in_river,ck_NOT_DEFINED,sizeof(CARD_KIND)*TOTAL_CARD_NUM);

    res->reserved_card_num = TOTAL_CARD_NUM - _roundManager->_distributedNum;
    
	CARD s_card;
	int i = 1;
	while(_roundManager->_river->getCard(s_card,i++)==true)
		res->card_in_river[res->river_len++]=s_card.kind;
}

void NetRaceLayer::collect_resources(HAH *res,CARD_KIND target1[],CARD_KIND target2[],int *len1,int *len2)
{
    LOGGER_WRITE("%s",__FUNCTION__);

	_CollectResouce(res);

	_roundManager->_players[(_roundManager->_curPlayer+1)%3]->get_parter()->get_hu_cards(target1,len1);
	_roundManager->_players[(_roundManager->_curPlayer+2)%3]->get_parter()->get_hu_cards(target2,len2);

    for(int i=_roundManager->_players[_roundManager->_curPlayer]->get_parter()->get_card_list()->atcvie_place;
        i<_roundManager->_players[_roundManager->_curPlayer]->get_parter()->get_card_list()->len;i++) {
		int time = res->list[_roundManager->_players[_roundManager->_curPlayer]->get_parter()->get_card_list()->data[i].kind].same_times++;
		res->list[_roundManager->_players[_roundManager->_curPlayer]->get_parter()->get_card_list()->data[i].kind].place[time]=i;
	}

	/*init hu target*/
	if( !_roundManager->IsTing(_roundManager->_curPlayer) ) {
		_roundManager->_players[_roundManager->_curPlayer]->init_target(&res->target,*len1,*len2);
    }
}

void NetRaceLayer::ListenToTingButton()
{
    LOGGER_WRITE("%s",__FUNCTION__);

	auto TingListener = EventListenerTouchOneByOne::create();
	TingListener->setSwallowTouches(false);
    
	TingListener->onTouchBegan=[=](Touch* touch, Event* event) {
		return true;
	};
    
	TingListener->onTouchMoved=[=](Touch* touch, Event* event) {
	};
    
	TingListener->onTouchEnded=[=](Touch* touch, Event* event) {
		auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
		auto button = (Sprite*)myframe->getChildByTag(TING_SING_BUTTON);

        if( !_IsClickedOn(button,touch) ) {
            ifTingSignBarVisible=false;
		} else {
			ifTingSignBarVisible = !ifTingSignBarVisible;
		}

        _Show(myframe,TING_SING_BAR,ifTingSignBarVisible);
	};
    
	auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(TingListener,myframe);
}

TargetedAction *NetRaceLayer::ShowBigMing(Node *myframe) {
    auto damingFont = Sprite::createWithSpriteFrameName("daming.png");
    
    damingFont->setAnchorPoint(Vec2(0.5,0.5));
    damingFont->setOpacity(0);
    damingFont->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2+50));

    myframe->addChild(damingFont,31,MING_EFFECT_DAMING);

    return TargetedAction::create(damingFont,Sequence::create(
        FadeIn::create(0.18),
        DelayTime::create(0.36),Spawn::create(
        FadeOut::create(0.18),
        ScaleTo::create(0.18,1.2),NULL),NULL));
}

void NetRaceLayer::update_outcard(Node *myframe,Vec2 location,int time)
{
    LOGGER_WRITE("%s : %x",__FUNCTION__,myframe);

	if(ifMingTime) {
		ifMingMybeError=true;
		ifMingTime=false;
	}
    
	if(_roundManager->_isWaitDecision) {
		_roundManager->_isWaitDecision=false;
		_roundManager->_tempActionToDo=a_JUMP;
	}

	if( (_roundManager->_actionToDo&a_MING) && (!ifMingMybeError) ) {
		ifMingMybeError = false;
		_roundManager->_actionToDo = a_JUMP;
	}

    _Remove(myframe, MING_CANCEL);

    if(myframe->getChildByTag(OUT_CARD_FRAME_TAG_ID)!=NULL)
		myframe->removeChildByTag(OUT_CARD_FRAME_TAG_ID);// why without true/false

    _Show(myframe, TING_SING_BAR, false);
    _Remove(myframe, MING_CANCEL);// why called twice ???


	auto cardOut = Sprite::createWithTexture(g_my_out->getTexture());
	auto show_card_kind = Sprite::createWithTexture(
        g_small_card_kind[_roundManager->_lastHandedOutCard]->getTexture()
    );
	show_card_kind->setPosition(Vec2(
        cardOut->getTextureRect().size.width/2,
        cardOut->getTextureRect().size.height*0.65));
	cardOut->addChild(show_card_kind);
	cardOut->setAnchorPoint(Vec2(0,1));
	if(time==1)
		cardOut->setPosition(location);
	else
		cardOut->setPosition(location.x,100);
	myframe->addChild(cardOut,30,OUT_CARD_FRAME_TAG_ID);

	BezierTo *action;
	outCardList* outCard = _roundManager->_players[_roundManager->_curPlayer]->get_parter()->getOutCardList();
	if(time==1)
		action = BizerMove1(outCard,location);
	else
		action = BizerMove2(outCard,location,time);

    CallFunc* BizerVoice = _Speak("Music/give.ogg");
	CallFunc* voiceCall  = _SpeakCard();

	Sequence* voiceEffect;
	Spawn* allEffect = Spawn::create(NULL);
    
	if(_roundManager->_actionToDo==a_MING && 
        !_roundManager->IsTing(_roundManager->_curPlayer) ) {
		_eventDispatcher->removeEventListenersForTarget(myframe,true);
        
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ming-tx.plist");
        
		_roundManager->_players[_roundManager->_curPlayer]->get_parter()->LockAllCards();
		_roundManager->_players[_roundManager->_curPlayer]->get_parter()->set_ting_status(1);

		while(myframe->getChildByTag(MING_EFFECT_DAMING))
			myframe->removeChildByTag(MING_EFFECT_DAMING);
        
		while(myframe->getChildByTag(MING_EFFECT_ANIMATE))
			myframe->removeChildByTag(MING_EFFECT_ANIMATE);

		auto mingEfeect=Sprite::createWithSpriteFrameName("ming_1.png");
		mingEfeect->setAnchorPoint(Vec2(0.5,0.5));
		mingEfeect->setScale(0);
		mingEfeect->setPosition(Vec2(origin.x+visibleSize.width*0.5,origin.y+visibleSize.height*0.5));
		myframe->addChild(mingEfeect,30,MING_EFFECT_ANIMATE);
        
		auto animation = Animation::create();
		animation->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ming_1.png"));
		animation->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ming_2.png"));
		animation->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ming_3.png"));
		animation->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ming_4.png"));
		animation->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ming_5.png"));
		animation->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ming_6.png"));
		animation->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ming_7.png"));
		animation->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ming_8.png"));
		animation->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ming_9.png"));
		animation->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ming_10.png"));
		animation->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ming_11.png"));
		animation->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ming_12.png"));
		animation->setDelayPerUnit(0.1f);
		animation->setRestoreOriginalFrame(true);

		auto MingAnimate = Animate::create(animation);
		auto MingActionseq=Sequence::create(
            ScaleTo::create(0,2),
            MingAnimate,
            ScaleTo::create(0,0),NULL);
		auto mingEffectTarget=TargetedAction::create(
            mingEfeect,
            MingActionseq);
		voiceEffect=Sequence::create(Spawn::create(
            action,
            voiceCall,NULL),
            BizerVoice,NULL);
		auto targetAction=TargetedAction::create(
            cardOut,
            voiceEffect);

        auto damingShow = ShowBigMing(myframe);
        
		allEffect = Spawn::create(
            damingShow,
            mingEffectTarget,
            targetAction,CallFunc::create([=](){
            ifInsertCardsTime=true;}),CallFunc::create([=](){
            delete_ActionRemind();}),NULL);
	}
	else
	{
		allEffect=Spawn::create( TargetedAction::create(cardOut,Sequence::create(Spawn::create(
            action,
            voiceCall,NULL),
            BizerVoice,NULL)),CallFunc::create([=](){
            ifInsertCardsTime=true;}),CallFunc::create([=](){
            delete_ActionRemind();}),NULL);
	}
    
	auto riverUpdate = CCCallFuncN::create(this,callfuncN_selector(NetRaceLayer::update_card_in_river_list));
	auto waitForResponse = CCCallFuncN::create(this,callfuncN_selector(NetRaceLayer::waitfor_response));
	auto sim = Sequence::create(
        riverUpdate,CCCallFunc::create([=]() {
		_roundManager->_players[1]->get_parter()->action(_roundManager->_isCardFromOthers,a_JUMP);
        _Show(myframe,TING_SING_BUTTON,true);
	}),CallFunc::create([=](){
		if(ifInsertStopped)
			ifInsertStopped = false;
		else {
            _Show(this,MING_STATUS_PNG_1,_roundManager->IsTing(1));
			card_list_update(1);
		}
	}),NULL);//callFunc1可优化一下ifInsertStopped ifInsertCardsTime=true;
	
	auto seq=Sequence::create(
        allEffect,
        DelayTime::create(0.12),CallFunc::create([=](){
        ifInsertCardsTime=false;}),
        sim,
        waitForResponse,
        NULL);
        
	myframe->_ID=1;
	_roundManager->_isCardFromOthers = true;
	myframe->runAction(seq);
}

void NetRaceLayer::choose_and_insert_cards(Node *myframe,CARD_ARRAY *list,int chosenCard,Touch* touch,int time)
{
    LOGGER_WRITE("%s (ifMingTime=%d, ifMyActionHint=%d)",__FUNCTION__,ifMingTime,_roundManager->_isWaitDecision);

	if(ifMingTime) {
		ifMingMybeError=true;
		ifMingTime=false;
	}
    
	if(_roundManager->_isWaitDecision) {
		_roundManager->_isWaitDecision=false;
		_roundManager->_tempActionToDo=a_JUMP;
	}

    _Remove(myframe,MING_CANCEL);
    _Show(myframe,TING_SING_BAR,false);

    if( chosenCard == list->len-1 ) {
		myframe->getChildByTag(HAND_IN_CARDS_TAG_ID + _roundManager->_curPlayer*20 + chosenCard)->setScale(0);
	} else {
        auto firstCard = (Sprite*)myframe->getChildByTag(HAND_IN_CARDS_TAG_ID + _roundManager->_curPlayer*20);
        const auto SIZE      = firstCard->getTextureRect().size;

        const int  LAST      = list->len-1;
        /* maybe need to be put into the circulation */
        const auto LAST_CARD = myframe->getChildByTag(HAND_IN_CARDS_TAG_ID + _roundManager->_curPlayer*20 + LAST);
        const auto LAST_POS  = myframe->getChildByTag(HAND_IN_CARDS_TAG_ID + _roundManager->_curPlayer*20 + LAST)->getPosition();

        /* need to be optimized !!! */
		for(int i=list->atcvie_place; i<LAST; i++) {
			auto card = (Sprite*)myframe->getChildByTag(HAND_IN_CARDS_TAG_ID + 
                _roundManager->_curPlayer*20 + i);
			auto curPos   = card->getPosition();
            
			auto RightMove = MoveTo::create(0.3,Vec2(curPos.x+SIZE.width,curPos.y));
			auto LeftMove  = MoveTo::create(0.3,Vec2(curPos.x-SIZE.width*1.2,curPos.y));

			auto RightInsertSeq   = Sequence::create(
                MoveTo::create(0.06,Vec2(LAST_POS.x,LAST_POS.y+100)),
                MoveTo::create(0.24,Vec2(curPos.x,LAST_POS.y+100)),
                MoveTo::create(0.06,Vec2(curPos)),NULL);
			auto LeftInsertSeq   = Sequence::create(
                MoveTo::create(0.06,Vec2(LAST_POS.x,LAST_POS.y+100)),
                MoveTo::create(0.24,Vec2(curPos.x-SIZE.width*0.2,LAST_POS.y+100)),
                MoveTo::create(0.06,Vec2(curPos.x-SIZE.width*0.2,curPos.y)),NULL);
						
			if(i < chosenCard) {
				if(list->data[i].kind <= list->data[LAST].kind)
					continue;
				else {
					if( i==list->atcvie_place || list->data[i-1].kind<=list->data[LAST].kind )
						LAST_CARD->runAction(RightInsertSeq);
                    
					card->runAction(RightMove);
				}
			} else if(i==chosenCard) {
				if(i==list->atcvie_place) {
					if(list->data[i+1].kind>list->data[LAST].kind)
						LAST_CARD->runAction(RightInsertSeq);
				} else if(i==LAST-1) {
					if(list->data[i-1].kind<=list->data[LAST].kind)
						LAST_CARD->runAction(MoveTo::create(0.3,Vec2(curPos.x,LAST_POS.y)));
 				} else {
					if(list->data[i-1].kind<=list->data[LAST].kind&&list->data[i+1].kind>list->data[LAST].kind)
						LAST_CARD->runAction(RightInsertSeq);
				}
                
				card->setScale(0);
			} else {/* i>chosenCard */
				if(list->data[i].kind>list->data[LAST].kind)
					card->runAction(MoveTo::create(0.3,Vec2(curPos.x-SIZE.width*0.2,curPos.y)));
				else {
					if(i==LAST-1)
						LAST_CARD->runAction(MoveTo::create(0.3,Vec2(curPos.x-SIZE.width*0.2,curPos.y)));
					else if(list->data[i+1].kind>list->data[LAST].kind)
						LAST_CARD->runAction(LeftInsertSeq);					
					card->runAction(LeftMove);
				}
			}
		}
	}
    
	if(time==1) {
		if(myframe->getChildByTag(CHOOSE_CARD_TAG_ID))
			myframe->removeChildByTag(CHOOSE_CARD_TAG_ID);
	}

    _roundManager->RecordOutCard(_roundManager->_players[_roundManager->_curPlayer]->get_parter()->get_card_list()->data[chosenCard]);
	_roundManager->_lastHandedOutCard = _roundManager->_players[_roundManager->_curPlayer]->get_parter()->hand_out(chosenCard);

	update_outcard(myframe,touch->getLocation(),time);
}

typedef enum {
    LEFT,
    MIDDLE,
    RIGHT,
}PlayerDir_t;

void NetRaceLayer::waitfor_MyShowCardInstruct()
{
    LOGGER_WRITE("%s isCardFromOthers=%d",__FUNCTION__,_roundManager->_isCardFromOthers);

	auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
    
	if(!_roundManager->_isCardFromOthers) {
		if( ifTuoGuan ||
            (_roundManager->IsTing(_roundManager->_curPlayer) 
            && !myframe->getChildByTag(GANG_REMING_ACT_TAG_ID)) ) {
			int last = _roundManager->_players[_roundManager->_curPlayer]->get_parter()->get_card_list()->len-1;
			Vec2 location = myframe->getChildByTag(HAND_IN_CARDS_TAG_ID + MIDDLE*20 + last)->getPosition();
            
			if(myframe->getChildByTag(HAND_IN_CARDS_TAG_ID + MIDDLE*20 + last))
				myframe->removeChildByTag(HAND_IN_CARDS_TAG_ID + MIDDLE*20 + last);
            
            _roundManager->RecordHandOut(last);
			update_outcard(myframe,location,2);
            
		} else if( myframe->getChildByTag(GANG_REMING_ACT_TAG_ID) ) {
			_roundManager->_isGangAsking = true;
		}
		else {
			ifMyShowCardTime = true;
        }
	}
}

bool NetRaceLayer::_CardTouchBegan(Touch* touch, Event* event) {
    auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
    
    if( ifInsertCardsTime ) {
        ifInsertCardsTime=false;
        ifInsertStopped=true;
        card_list_update(MIDDLE);
    }
    
    if( ifEffectTime && ifUpdateDuringEffect ) {
        ifUpdateDuringEffect=false;
        card_list_update(MIDDLE);
    }

    ifChosed=false;
    
    if( touch->getLocation().y > visibleSize.height*0.173 ) {
        while(myframe->getChildByTag(TING_SING_BAR) && (!_roundManager->IsTing(1)))
            myframe->removeChildByTag(TING_SING_BAR);
        
        float x1,y1;
        x1 = base_point[MIDDLE].x+10;
        y1 = base_point[MIDDLE].y+10;

        MyCardChoosedNum = -1;
        
        auto cardsInHand   = _roundManager->_players[MIDDLE]->get_parter()->get_card_list();
        const int start    = cardsInHand->atcvie_place;
        const int last     = cardsInHand->len - 1;
        const int residualNum = (cardsInHand->len - cardsInHand->atcvie_place)%3;
        const auto startPos = myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+MIDDLE*20+start)->getPosition();
        const auto cardSize = (Sprite::createWithTexture(g_my_free->getTexture()))->getTextureRect().size;

        for(int k=start; k<=last; k++) {
            auto card = myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+MIDDLE*20+k);
            card->_ID=1;
            card->setScale(1);
            card->setAnchorPoint(Vec2(0,0));

            if(k==last && residualNum==2) {
                card->setPosition(Vec2(startPos.x + cardSize.width*(k-cardsInHand->atcvie_place)+30,y1));
            } else {
                card->setPosition(Vec2(startPos.x + cardSize.width*(k-cardsInHand->atcvie_place),y1));
            }
        }
    }
    
    return true;
}

void NetRaceLayer::_CardTouchMove(Touch* touch, Event* event) {
    auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
    
	auto cardsInHand= _roundManager->_players[1]->get_parter()->get_card_list();
	int  start      = cardsInHand->atcvie_place;
    int  last       = cardsInHand->len-1;
	int  residualNum= (cardsInHand->len - start)%3;
	auto startPos   = myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+1*20+start)->getPosition();
	auto VoiceEffect= _Speak("Music/select.ogg");
    
	if(myframe->getChildByTag(CHOOSE_CARD_TAG_ID)!=NULL && touch->getLocation().y>visibleSize.height*0.173) {
		MyCardChoosedNum = touched;
		myframe->getChildByTag(CHOOSE_CARD_TAG_ID)->setPosition(touch->getLocation());
		return;
	} 
    
	for(int i=start; i<last+1; i++) {
		myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+1*20+i)->setOpacity(255);
	}
    
	if(myframe->getChildByTag(CHOOSE_CARD_TAG_ID)!=NULL)
		myframe->removeChildByTag(CHOOSE_CARD_TAG_ID);
    
    auto cardSize = Sprite::createWithTexture(g_my_free->getTexture())->getTextureRect().size;
    float x1,y1;
    x1 = base_point[1].x+10;
    y1 = base_point[1].y+10;

	for(int i=start; i<last+1; i++){
		if( _IsClickedOn((Sprite*)myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+1*20+i),touch) ) {
			if(cardsInHand->data[i].can_play==cps_YES) {						
				for(int k=start; k<last+1; k++) {
					auto loopCard=myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+1*20+k);
                    
					if(k<i) {//restore the original size & position?
						loopCard->setPosition(Vec2(startPos.x + cardSize.width*(k-start), y1));
						loopCard->setScale(1);
					} else if(k==i) {
						MyCardChoosedNum = k;

                        //logic changed !!!
						if( k==last && residualNum==2 ) {
							loopCard->setPosition(Vec2(startPos.x+cardSize.width*(k-start)+30,y1+10));
                        } else {
							loopCard->setPosition(Vec2(startPos.x+cardSize.width*(k-start),   y1+10));
                        }

                        if(_roundManager->_actionToDo==a_MING && ifMingTime && (!_roundManager->IsTing(MIDDLE))) {
                            if( loopCard->getScaleX()==1 ) {
                                loopCard->setScale(1.2);
                                loopCard->runAction(VoiceEffect);

								while(myframe->getChildByTag(TING_SING_BAR))
									myframe->removeChildByTag(TING_SING_BAR);

								auto appearPoint=loopCard->getPosition();
								tingHintCreate(appearPoint,k);
                             } else if (!myframe->getChildByTag(TING_SING_BAR)) {
								auto appearPoint=loopCard->getPosition();
								tingHintCreate(appearPoint,k);
                             }
                             
                             ifChosed = true;
                             touched = i;
                        }                        
					} else {
						if( k==last && residualNum==2 ) {
                            loopCard->setPosition(Vec2(startPos.x+cardSize.width*(k-cardsInHand->atcvie_place)+30+14,y1));
						} else {
							loopCard->setPosition(Vec2(startPos.x+cardSize.width*(k-start)+14,y1));
						}
                        
                        loopCard->setScale(1);
					}
				}
			}
			break;
		} else if(touch->getLocation().y>visibleSize.height*0.173) {
			if(myframe->getChildByTag(CHOOSE_CARD_TAG_ID)==NULL && ifChosed==true) {
				int  kind = _roundManager->_players[1]->get_parter()->get_card_list()->data[touched].kind;
				auto card = Sprite::createWithTexture(g_card_kind[kind]->getTexture());
				auto freeCard = Sprite::createWithTexture(g_my_free->getTexture());
                
				freeCard->addChild(card);
				card->setPosition(Vec2(
                    freeCard->getTextureRect().size.width/2,
                    freeCard->getTextureRect().size.height*0.4));
				freeCard->setScale(1.2);
                
				MyCardChoosedNum = touched;
                
				freeCard->setPosition(touch->getLocation());
				myframe->addChild(freeCard,35,CHOOSE_CARD_TAG_ID);
				myframe->getChildByTag(HAND_IN_CARDS_TAG_ID + 1*20 + touched)->setOpacity(150);

				while(myframe->getChildByTag(TING_SING_BAR) && (!_roundManager->IsTing(MIDDLE)))
					myframe->removeChildByTag(TING_SING_BAR,true);
			}
			break;
		}
	}
}

void NetRaceLayer::_CardTouchEnd(Touch* touch, Event* event) {
    auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
    
    LOGGER_WRITE("%s",__FUNCTION__);
	//========================PossibleCondition=================//
	auto cardsInHand = _roundManager->_players[1]->get_parter()->get_card_list();
	int start        = cardsInHand->atcvie_place;
    int last         = cardsInHand->len - 1;
	int residualNum  = (last - cardsInHand->atcvie_place + 1)%3;
	auto startPos    = myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+1*20+start)->getPosition();
	//========================PossibleCondition=================//
	auto VoiceEffect = _Speak("Music/select.ogg");
    
	if(myframe->getChildByTag(CHOOSE_CARD_TAG_ID)!=NULL && (touch->getLocation().y>visibleSize.height*0.2)) {
		if(ifMyShowCardTime) {
			ifMyShowCardTime=false;
			MyCardChoosedNum=-1;
			choose_and_insert_cards(myframe,cardsInHand,touched,touch,1);
		} else {
			MyCardChoosedNum=touched;
			myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+1*20+touched)->setOpacity(255);
			myframe->removeChildByTag(CHOOSE_CARD_TAG_ID,true);
		}
		return;
	} else {
		for(int i=start; i<=last; i++)
			myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+1*20+i)->setOpacity(255);
        
		if(myframe->getChildByTag(CHOOSE_CARD_TAG_ID)!=NULL)
			myframe->removeChildByTag(CHOOSE_CARD_TAG_ID,true);
	}
    
	int touchedCard = -1;
    
	for(int i=start; i<=last; i++) {
        if ( _IsClickedOn((Button *)myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+1*20+i),touch)
            && cardsInHand->data[i].can_play==cps_YES ) {
			touchedCard = i;
			break;
		}
	}
    
	if(touchedCard>=0) {
		bool ifInsertCardsTime = false;
        
        float x,y;
		x = base_point[1].x+10;
		y = base_point[1].y+10;
        
		auto cardSize = Sprite::createWithTexture(g_my_free->getTexture())->getTextureRect().size;
        
		for(int k=start; k<=last; k++) {
			auto loopCard = (Sprite*)myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+1*20+k);
            
			if(k<touchedCard) {
				loopCard->_ID = 1;
				loopCard->setPosition(Vec2(startPos.x + cardSize.width*(k-start),y));
				loopCard->setScale(1);
			} else if(k>touchedCard) {
				loopCard->_ID=1;
                loopCard->setScale(1);

				if(k==last && residualNum==2){
					loopCard->setPosition(Vec2(startPos.x + cardSize.width*(k-start)+30+14,y));
				} else {
					loopCard->setPosition(Vec2(startPos.x + cardSize.width*(k-start)+14,y));
				}
			} else {
				MyCardChoosedNum = k;
                
				if(k==last) {
					if(loopCard->_ID==100) {
						if(ifMyShowCardTime) {
							ifMyShowCardTime=false;
							MyCardChoosedNum=-1;
							touched=k;
							choose_and_insert_cards(myframe,cardsInHand,touched,touch,2);
						}
					} else {
						loopCard->_ID=100;
                        
						if(residualNum==1)
							loopCard->setPosition(Vec2(startPos.x + cardSize.width*(k-start),y+10));
						else if(residualNum==2)
							loopCard->setPosition(Vec2(startPos.x + cardSize.width*(k-start)+30,y+10));
                        
						if(loopCard->getScale()==1)
							loopCard->runAction(VoiceEffect);
                        
						loopCard->setScale(1.2);
					}
				} else {
					if(loopCard->_ID==100)
						ifInsertCardsTime=true;
					else {
						loopCard->_ID=100;
						loopCard->setPosition(Vec2(startPos.x+cardSize.width*(k-start),y+10));
						if(loopCard->getScale()==1)
							loopCard->runAction(VoiceEffect);
						loopCard->setScale(1.2);
					}
				}
                
				if(_roundManager->_actionToDo==a_MING
                    && ifMingTime && !_roundManager->IsTing(MIDDLE)) {
					while( myframe->getChildByTag(TING_SING_BAR) && (!_roundManager->IsTing(MIDDLE)) )
						myframe->removeChildByTag(TING_SING_BAR);
                    
					Point curPos=loopCard->getPosition();
					tingHintCreate(curPos,k);
				}
			}
		}
        
		if(ifInsertCardsTime && ifMyShowCardTime)
		{
			ifMyShowCardTime=false;
			MyCardChoosedNum=-1;
			touched = touchedCard;
			choose_and_insert_cards(myframe,cardsInHand,touched,touch,3);
		}
	}
}

void NetRaceLayer::ListenToCardTouch() {
    auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
    
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
    
	listener->onTouchBegan = CC_CALLBACK_2(NetRaceLayer::_CardTouchBegan,this);
	listener->onTouchMoved = CC_CALLBACK_2(NetRaceLayer::_CardTouchMove,this);
	listener->onTouchEnded = CC_CALLBACK_2(NetRaceLayer::_CardTouchEnd,this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,myframe);
}

void NetRaceLayer::waitfor_MyTouchShowCard()//正常情况下的出牌监听（非托管和明牌）
{
    _roundManager->AllowMovement();
	if( !_roundManager->IsTing(MIDDLE) && !ifTuoGuan ) {
        ListenToCardTouch();
    }
}

void NetRaceLayer::waitfor_ShowCardWithoutTouch()
{
    if ( _roundManager->_curPlayer==1 ) {
        return;
    }

    LOGGER_WRITE("%s (player=%d)",__FUNCTION__,_roundManager->_curPlayer);
    /*******************
        robot action 
    *******************/
    HAH *s_res = new HAH;
	int index;
    
	CARD_KIND list1[9];
	CARD_KIND list2[9];
	int len1;
	int len2;
    
	if(_roundManager->_players[_roundManager->_curPlayer]->get_parter()->get_role_type()==SINGLE_BOARD_ROBOT) {
		collect_resources(s_res,list1,list2,&len1,&len2);
		_roundManager->_players[_roundManager->_curPlayer]->set_robot_hu_target(s_res->target);
	}
    
	if(_roundManager->_players[_roundManager->_curPlayer]->get_parter()->get_role_type()==INTERNET_PLAYER) {
        LOGGER_WRITE("NETWORK : NetPlayer action here, %s %d",__FILE__,__LINE__);
		collect_resources(s_res,list1,list2,&len1,&len2);
		_roundManager->_players[_roundManager->_curPlayer]->set_robot_hu_target(s_res->target);
	}

    if( !_roundManager->IsTing(_roundManager->_curPlayer) ) {
		index = _roundManager->_players[_roundManager->_curPlayer]->chose_card(
            s_res,TOTAL_CARD_NUM - _roundManager->_distributedNum,list1,list2,len1,len2);

		if( index==-1 || index>_roundManager->_players[_roundManager->_curPlayer]->get_parter()->get_card_list()->len-1 ) {
			index=_roundManager->_players[_roundManager->_curPlayer]->get_parter()->get_card_list()->len-1;
		}
        
		if(s_res->hu_nums>=6 && _roundManager->_actionToDo==a_MING && 
        !_roundManager->IsTing(_roundManager->_curPlayer) ) {
			RototHandOutIndex = _roundManager->_players[_roundManager->_curPlayer]->get_parter()->get_card_list()->data[index].kind;

			KouCardsCheck(_roundManager->_curPlayer);
			if(Kou_kindLen>0)
				ming_kou_Choose(_roundManager->_curPlayer);
		}
	} else
		index = _roundManager->_players[_roundManager->_curPlayer]->get_parter()->get_card_list()->len-1;

    _roundManager->RecordOutCard(_roundManager->_players[_roundManager->_curPlayer]->get_parter()->get_card_list()->data[index]);
	_roundManager->_lastHandedOutCard=_roundManager->_players[_roundManager->_curPlayer]->get_parter()->hand_out(index);

    /*******************
        effect 
    *******************/
    auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
    int curPlayer = _roundManager->_curPlayer;
    
	auto smallCard = Sprite::createWithTexture(g_small_card_kind[_roundManager->_lastHandedOutCard]->getTexture());
	smallCard->runAction(RotateTo::create(0,_RotateAngleOfOutcard(curPlayer)));
    
	if(myframe->getChildByTag(SHOW_CARD_INIDCATOR_TAD_ID)) {
		myframe->removeChildByTag(SHOW_CARD_INIDCATOR_TAD_ID);
	}
    
	auto cardInHand = _roundManager->_players[_roundManager->_curPlayer]->get_parter()->get_card_list();
	auto LastCard   = myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+_roundManager->_curPlayer*20 + cardInHand->len);
	auto hideLastInHand = CallFunc::create([=](){
		if(LastCard)
			LastCard->setScale(0);
	});

	CallFunc* showAndHideOutcardNotice = CallFunc::create([=](){
		auto cardFrame = Sprite::create("tileImage/tile_lastTileBG.png");
		cardFrame->setAnchorPoint(_AnchorOfOutcard(curPlayer));
		cardFrame->setPosition(_PositionOfOutcard(curPlayer,visibleSize,origin));
		myframe->addChild(cardFrame,35,SHOW_CARD_INIDCATOR_TAD_ID);
        
		auto cardBg = Sprite::createWithTexture(g_my_free->getTexture());
		cardBg->setAnchorPoint(Vec2(0.5,0.5));
		cardBg->setPosition( Vec2(cardFrame->getTextureRect().size.width*0.515,cardFrame->getTextureRect().size.height*0.515) );
		cardFrame->addChild(cardBg);
        
		auto card = Sprite::createWithTexture(g_card_kind[_roundManager->_lastHandedOutCard]->getTexture());
		card->setAnchorPoint(Vec2(0.5,0.5));
		card->setPosition(Vec2(cardBg->getTextureRect().size.width/2,cardBg->getTextureRect().size.height*0.4));
		cardBg->addChild(card);
        
		cardFrame->runAction(Sequence::create(
            DelayTime::create(1.5),
            ScaleTo::create(0,0),NULL));
	});
    
	if(myframe->getChildByTag(OUT_CARD_FRAME_TAG_ID))
		myframe->removeChildByTag(OUT_CARD_FRAME_TAG_ID);

	Sprite   * cardOut = Sprite::createWithTexture(g_left_right_peng_out->getTexture());
	smallCard->setPosition(Vec2(
        cardOut->getTextureRect().size.width/2,
        cardOut->getTextureRect().size.height*0.65));
	smallCard->setRotation(-90);/* !!! player0 is 90 in the old source, but it seems ok to set it as -90*/
	smallCard->setScale(0.9);
	cardOut->addChild(smallCard);
	cardOut->setAnchorPoint(Vec2(0.5,0.5));

    Vec2 curOutPos;/* here must be something I have not known */
    if(_roundManager->_curPlayer==2) {
		auto curOutPosTemp = myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+_roundManager->_curPlayer*20+cardInHand->len-1)->getPosition();
		curOutPos = Vec2(
            curOutPosTemp.x,
            curOutPosTemp.y + cardOut->getTextureRect().size.height*1.5);
	}
	else if(_roundManager->_curPlayer==0) {
		auto cardOut_Yposition = myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+_roundManager->_curPlayer*20+cardInHand->len-1)->getPosition().y;
        curOutPos = Vec2(
            base_point[_roundManager->_curPlayer].x + 10,
            cardOut_Yposition - 20 - 35);
	}

    cardOut->setPosition(curOutPos);
    cardOut->setVisible(false);
    myframe->addChild(cardOut,0,OUT_CARD_FRAME_TAG_ID);/* !!! player2 is 20 in the old source, but it seems ok to set it as 0*/

    BezierTo *inHandMoveToOutHand = OthersBizerMove(_roundManager->_curPlayer,
                                        _roundManager->_players[_roundManager->_curPlayer]->get_parter()->getOutCardList());
    
	Sequence *voiceEffect;	

	if(s_res->hu_nums>=6
        && _roundManager->_actionToDo==a_MING && !_roundManager->IsTing(_roundManager->_curPlayer))
	{
		OtherTingHintBar(_roundManager->_curPlayer,index);
		if(Kou_kindLen>0)
			_roundManager->_players[_roundManager->_curPlayer]->get_parter()->action(_roundManager->_isCardFromOthers,a_KOU);
		_roundManager->_players[_roundManager->_curPlayer]->get_parter()->action(_roundManager->_isCardFromOthers,a_MING);
		_roundManager->_players[_roundManager->_curPlayer]->get_parter()->LockAllCards();
		_roundManager->_players[_roundManager->_curPlayer]->get_parter()->set_ting_status(1);

		auto simple_seq = simple_tip_effect( getEffectVec(_roundManager->_curPlayer),"daming.png" );
		voiceEffect = Sequence::create(
                        simple_seq,
                        hideLastInHand,CallFunc::create([=](){ 
                        _Show(myframe,OUT_CARD_FRAME_TAG_ID,true);} ),Spawn::create(
                            showAndHideOutcardNotice,
                            inHandMoveToOutHand,
                            _SpeakCard(),NULL),
                        _Speak("Music/give.ogg"),NULL);
	}
	else
		voiceEffect = Sequence::create(
                		hideLastInHand,CallFunc::create([=](){ 
                        _Show(myframe,OUT_CARD_FRAME_TAG_ID,true);} ),Spawn::create(
                    		showAndHideOutcardNotice,
                    		inHandMoveToOutHand,
                    		_SpeakCard(),NULL),
                		_Speak("Music/give.ogg"),NULL);

	delete s_res;

	auto seq = Sequence::create(TargetedAction::create(cardOut,
        voiceEffect), CCCallFuncN::create(this,callfuncN_selector(
        NetRaceLayer::update_card_in_river_list)), CCCallFunc::create([=]() {
		_roundManager->_players[_roundManager->_curPlayer]->get_parter()->action(_roundManager->_isCardFromOthers,a_JUMP);}),CCCallFuncN::create(this,callfuncN_selector(
        NetRaceLayer::update_card_list)), CCCallFuncN::create(this,callfuncN_selector(
        NetRaceLayer::waitfor_response)),NULL);

	myframe->_ID=_roundManager->_curPlayer;
	_roundManager->_isCardFromOthers=true;
	myframe->runAction(seq);
}

void NetRaceLayer::DispatchAction(Node *psender,ARRAY_ACTION action) {
    LOGGER_WRITE("%s",__FUNCTION__);

	int no = psender->_ID;
}

void NetRaceLayer::peng_dispatch(Node *psender){
    LOGGER_WRITE("%s",__FUNCTION__);

	int no=psender->_ID;
    
	_roundManager->_isCardFromOthers=true;
	_roundManager->_players[no]->get_parter()->action(_roundManager->_isCardFromOthers,a_PENG);
}

void NetRaceLayer::minggang_dispatch(Node *psender)
{
    LOGGER_WRITE("%s",__FUNCTION__);

	int no=psender->_ID;

    _roundManager->_players[no]->get_parter()->action(_roundManager->_isCardFromOthers,a_MING_GANG);
}

void NetRaceLayer::angang_dispatch(Node *psender)
{
    LOGGER_WRITE("%s",__FUNCTION__);

	int no=psender->_ID;

    if(_roundManager->_actionToDo&a_AN_GANG)
		_roundManager->_players[no]->get_parter()->action(_roundManager->_isCardFromOthers,a_AN_GANG);
	else if(_roundManager->_actionToDo&a_SHOU_GANG)
		_roundManager->_players[no]->get_parter()->action(_roundManager->_isCardFromOthers,a_SHOU_GANG);
}

Sprite *NetRaceLayer::_GetEffectCardInHand(Node *myframe, int i,CARD_KIND value ) {
    auto card = (Sprite*)myframe->getChildByTag(HAND_IN_CARDS_TAG_ID + 1*20 + i);
    auto pos  = card->getPosition();
    
    auto effectCard = Sprite::createWithTexture( g_my_free->getTexture() );
    effectCard->setAnchorPoint( Vec2(0,0) );
    effectCard->setScale( card->getScale() );
    effectCard->setPosition( Vec2(pos.x,pos.y) );
    
    auto kind = Sprite::createWithTexture(g_card_kind[value]->getTexture());
    kind->setAnchorPoint(Vec2(0.5,0.5));
    kind->setPosition(Vec2(

        effectCard->getTextureRect().size.width/2,
        effectCard->getTextureRect().size.height*0.4));

    effectCard->addChild(kind,1);

    return effectCard;
}

void NetRaceLayer::PengEffect(Node *psender)//效果逻辑分离
{
	int no = psender->_ID;
	auto myframe = this->getChildByTag(GAME_BKG_TAG_ID);
	myframe->_ID = no;
    LOGGER_WRITE("%s %d",__FUNCTION__,no);

	continue_gang_times = 0;
	_roundManager->_lastAction=a_PENG;
    
	const int last =_roundManager->_players[_roundManager->_curPlayer]->get_parter()->getOutCardList()->length;
	Card card;
	_roundManager->_players[_roundManager->_curPlayer]->get_parter()->getOutCardList()->getCard(card,last);
	_roundManager->_players[_roundManager->_curPlayer]->get_parter()->getOutCardList()->deleteItem();
    
	if(myframe->getChildByTag(HAND_OUT_CARDS_TAG_ID+_roundManager->_curPlayer*25+last-1)) {
		myframe->removeChildByTag(HAND_OUT_CARDS_TAG_ID+_roundManager->_curPlayer*25+last-1);
	}

    _roundManager->RecordOutCard(card);
    _roundManager->RecordOutCard(card);

	_roundManager->_curPlayer=no;
    
	auto outCard = myframe->getChildByTag(OUT_CARD_FRAME_TAG_ID);
    auto hideOutcard = Spawn::create(CCCallFunc::create([=](){
			_Show(this,SHOWCARD_INDICATOR_TAG_ID,false);}),TargetedAction::create(
                outCard,Sequence::create(
                DelayTime::create(0.1),
                ScaleTo::create(0,0),NULL)),NULL);
		
	if(no!=1) {
		myframe->runAction(Sequence::create( 
                            Spawn::create(
                                _SpeakAction(PENG),
                                simple_tip_effect( getEffectVec(_roundManager->_curPlayer),"peng.png" ),NULL), 
                            hideOutcard, 
                            Sequence::create(CCCallFunc::create(this,callfunc_selector(
                                NetRaceLayer::delete_act_tip)),   CCCallFuncN::create(this,callfuncN_selector(
                                NetRaceLayer::peng_dispatch)),    CCCallFuncN::create(this,callfuncN_selector(
                                NetRaceLayer::update_card_list)), CCCallFunc::create([=](){
                    			_roundManager->_actionToDo = _roundManager->_players[_roundManager->_curPlayer]->get_parter()->ActiontodoCheckAgain();
                				waitfor_otheraction(no);}),NULL),NULL));
	} else {
		if(myframe->getChildByTag(PENG_EFFECT_NODE_ID)) {
			myframe->removeChildByTag(PENG_EFFECT_NODE_ID,true);
			if(ifEffectTime) {
				ifEffectTime=false;
				ifUpdateDuringEffect=false;
                
				_roundManager->_curEffectCardKind=ck_NOT_DEFINED;
				_roundManager->_curEffectCardStatus=c_NOT_DEFINDED;
                
				delete_ActionEffect();
				card_list_update(no);
			}
		}
		_roundManager->_curEffectCardKind=card.kind;
		_roundManager->_curEffectCardStatus=c_PENG;
        
		ifEffectTime=true;
		ifUpdateDuringEffect=true;

        /****************
            hide reminder
        ****************/
		auto shadeAction = TargetedAction::create((Sprite*)myframe->getChildByTag(PENG_REMIND_ACT_BKG_TAG_ID),
            Sequence::create(
                ScaleTo::create(0,1), Spawn::create(
                FadeOut::create(0.18),
                ScaleTo::create(0.18,1.2),NULL),NULL));

		auto hideReminder = Sequence::create( Spawn::create(
                        shadeAction, CCCallFunc::create([=]() {
            			for(int i=0; i<11; i++) {
                            if(i==PENG_REMIND_ACT_BKG_TAG_ID) {
                                continue;
                            } else {
                                _Remove(myframe,REMIND_ACT_TAG_ID+i);}}),NULL),CCCallFunc::create([=](){
                            }
                        _Remove(myframe,PENG_REMIND_ACT_BKG_TAG_ID);
                		}),NULL);
	
        /****************
            move 3 cards
        ****************/
        Sprite *showCards[3] = {0};
        for(int i=0;i<3;i++) {
            showCards[i] = Sprite::createWithTexture(g_my_peng->getTexture());
            auto kind = Sprite::createWithTexture(g_mid_card_kind[card.kind]->getTexture());
            kind->setAnchorPoint(Vec2(0.5,0.5));
            kind->setPosition(Vec2(
                showCards[i]->getTextureRect().size.width/2,
                showCards[i]->getTextureRect().size.height*0.65));
            showCards[i]->addChild(kind);
            showCards[i]->setAnchorPoint(Vec2(0,0));
            showCards[i]->setScale(0);
            myframe->addChild(showCards[i],20,SINGLE_ACTION_EFFECT_TAG_ID+i);
        }

        const Vec2 REFERENCE = Vec2(origin.x+visibleSize.width*0.445, origin.y+visibleSize.height*0.255);
        const Size SIZE      = showCards[0]->getTextureRect().size;

		showCards[0]->setPosition( outCard->getPosition() );
		showCards[1]->setPosition( REFERENCE );
		showCards[2]->setPosition( Vec2(REFERENCE.x + SIZE.width*0.98, REFERENCE.y) );

		auto action1 = Sequence::create(
            DelayTime::create(0.48),
            ScaleTo::create(0,1),
            MoveTo::create(0.12,Vec2(REFERENCE.x + SIZE.width*1.96,       REFERENCE.y)),
			MoveTo::create(0.12,Vec2(REFERENCE.x + SIZE.width*1.96 + 150, REFERENCE.y)),
			MoveTo::create(0.12,Vec2(REFERENCE.x + SIZE.width*1.96,       REFERENCE.y)),NULL);
		auto p_seq1 = TargetedAction::create(showCards[0],action1);

		auto action2=Sequence::create(
            DelayTime::create(0.48),
            ScaleTo::create(0,1),
            DelayTime::create(0.12),
			MoveTo::create(0.12,Vec2(REFERENCE.x - 150, REFERENCE.y)),
			MoveTo::create(0.12,REFERENCE),NULL);
		auto p_seq2 = TargetedAction::create(showCards[1],action2);

        auto action3=Sequence::create(
            DelayTime::create(0.48),
            ScaleTo::create(0,1),
            DelayTime::create(0.12),
			MoveTo::create(0.12,Vec2(REFERENCE.x + SIZE.width*0.98 - 150, REFERENCE.y)),
			MoveTo::create(0.12,Vec2(REFERENCE.x + SIZE.width*0.98,       REFERENCE.y)),NULL);
		auto p_seq3 = TargetedAction::create(showCards[2],action3);

        auto move3PengCards = Spawn::create(p_seq1,p_seq2,p_seq3,NULL);

        /****************
            move 2 peng cards in hand
        ****************/
		auto list = _roundManager->_players[1]->get_parter()->get_card_list();
        
		int firstMatch = 0;
		int secondMatch = 0;
		for(int i=list->atcvie_place;i<list->len;i++) {
			if(card.kind==list->data[i].kind) {
				if(i==0) {
					firstMatch = 0;
				} else if(card.kind!=list->data[i-1].kind) {
					firstMatch = i;
				} else if(card.kind==list->data[i-1].kind) {
					secondMatch = i;
					break;
				}
			}
		}

		int ifZeroPointTwo=0;
		if(MyCardChoosedNum!=-1) {
			if(MyCardChoosedNum > secondMatch)
				MyCardChoosedNum += 1;
			else if(MyCardChoosedNum== secondMatch || MyCardChoosedNum==firstMatch) {
				ifZeroPointTwo = 1;
				MyCardChoosedNum = -1;
			}
			else if(MyCardChoosedNum<firstMatch)
				MyCardChoosedNum += 3;
		}


		auto LeftPengCard = _GetEffectCardInHand(myframe, firstMatch, card.kind);
		myframe->addChild(LeftPengCard,20,EFFET_NEWCATD1_TAG);
		auto moveLeftCardInHand = TargetedAction::create(LeftPengCard,Sequence::create(
            DelayTime::create(0.18),
            ScaleTo::create(0,0.6),
            MoveTo::create(0.18,Vec2(
                origin.x+visibleSize.width*0.45,
                origin.y+visibleSize.height*0.26)),
			DelayTime::create(0.12),
			ScaleTo::create(0,0),NULL));

		auto RightPengCard = _GetEffectCardInHand(myframe,secondMatch, card.kind);
		myframe->addChild(RightPengCard,20,EFFET_NEWCATD2_TAG);

        /* in case these values are changed during being moved */
		auto OldRightPos = RightPengCard->getPosition();
		auto OldCardSize = RightPengCard->getTextureRect().size;
        
		auto moveRightCardInHand = TargetedAction::create(RightPengCard,Sequence::create(
            DelayTime::create(0.18),
            Spawn::create(
                ScaleTo::create(0,0.6),
                MoveTo::create(0,Vec2(
                    OldRightPos.x-OldCardSize.width*0.4,
                    OldRightPos.y)),NULL),
			MoveTo::create(0.18,Vec2(
			    origin.x+visibleSize.width*0.45+OldCardSize.width*0.6,
			    origin.y+visibleSize.height*0.26)),
			DelayTime::create(0.12),
			ScaleTo::create(0,0),NULL));

        /****************
            delete 2 peng cards in hand 
        ****************/
		auto cardPeng = Sprite::createWithTexture(g_my_peng->getTexture());
		auto cardPengSize = cardPeng->getTextureRect().size;
        
        const float GAP = (list->atcvie_place==0)? 0.5 : 0;
        
		Vector<FiniteTimeAction *> hide2CardsInhand;

		for(int i=list->atcvie_place;i<list->len;i++) {
			auto s_card = (Sprite*)myframe->getChildByTag(HAND_IN_CARDS_TAG_ID + 1*20 + i);
            Sequence *seq;

            if(i==firstMatch||i==secondMatch) {
				seq = Sequence::create(
                    ScaleTo::create(0,0),NULL);
            } else {
				auto curPos   = s_card->getPosition();
				auto cardSize = s_card->getTextureRect().size;

                if (i<firstMatch) {
                    seq = Sequence::create(
                        DelayTime::create(0.18),
                        MoveTo::create(0.3,Vec2(
                            curPos.x + cardPengSize.width*(3.5+GAP),
                            curPos.y)),NULL);
                } else {
                    seq = Sequence::create(
                        DelayTime::create(0.18),
                        MoveTo::create(0.3,Vec2(
                            curPos.x + (cardPengSize.width*(3.5+GAP)) - cardSize.width*(2 + ifZeroPointTwo*0.2),
                            curPos.y)),NULL);
                }
            }
            
            hide2CardsInhand.insert(i-list->atcvie_place,TargetedAction::create(s_card,seq));
		}
        
        /****************
            tag update 
        ****************/
		for(int a=list->len-1; a>=list->atcvie_place; a--) {
			int curTag = HAND_IN_CARDS_TAG_ID + 1*20 + a;
            
			if(!myframe->getChildByTag(curTag))
				continue;
            
			auto curCard = (Sprite*)myframe->getChildByTag(curTag);
            
			if(a>secondMatch)
				curCard->setTag(curTag+1);
			else if(a==secondMatch)
				curCard->setTag(EFFECT_TEMP_CARD_TAG_ONE);
			else if(a==firstMatch)
				curCard->setTag(EFFECT_TEMP_CARD_TAG_TWO);
			else if(a<firstMatch && a>list->atcvie_place)
				curCard->setTag(curTag+3);
            else if(a==list->atcvie_place) {
				curCard->setTag(curTag+3);
            }
		}
        /* why not set this value directly??? */
        ((Sprite*)myframe->getChildByTag(EFFECT_TEMP_CARD_TAG_ONE))->setTag(HAND_IN_CARDS_TAG_ID+1*20 + list->atcvie_place);
        ((Sprite*)myframe->getChildByTag(EFFECT_TEMP_CARD_TAG_TWO))->setTag(HAND_IN_CARDS_TAG_ID+1*20 + list->atcvie_place+1);

        auto EmptyCard=Sprite::createWithTexture(g_my_free->getTexture());
        EmptyCard->setAnchorPoint(Vec2(1,1));
        EmptyCard->setScale(0);
        EmptyCard->setPosition(Vec2::ZERO);
        myframe->addChild(EmptyCard,1,HAND_IN_CARDS_TAG_ID + 1*20 + list->atcvie_place+2);

        /****************
            background effect
        ****************/
		auto light = Sprite::createWithSpriteFrameName("Q3.png");
		light->setPosition(Vec2(origin.x+visibleSize.width*0.445+showCards[0]->getTextureRect().size.width*1.96,origin.y+visibleSize.height*0.315));
		myframe->addChild(light,23,IMG_Q3_EFFECT_TAG_ID);
		light->setOpacity(60);
		light->setScale(0);
		BlendFunc tmp_oBlendFunc = {GL_SRC_ALPHA, GL_ONE};
		light->setBlendFunc(tmp_oBlendFunc);
		auto lightEffect = TargetedAction::create(light, Sequence::create(
    		DelayTime::create(0.84),
    		ScaleTo::create(0,1),
    		DelayTime::create(0.48),
    		FadeOut::create(0),NULL));

		auto circle=Sprite::createWithSpriteFrameName("Q15.png");
		circle->setPosition(Vec2(origin.x+visibleSize.width*0.445+showCards[0]->getTextureRect().size.width*1.96,origin.y+visibleSize.height*0.315));
		circle->setTag(4);
		circle->setScale(2.0);
		circle->setScale(0);
		myframe->addChild(circle,21,IMG_Q15_EFFECT_TAG_ID);
		auto circleEffect = TargetedAction::create(circle,Sequence::create(
            DelayTime::create(0.84),
            ScaleTo::create(0,1.8), Spawn::create(
                ScaleTo::create(0.48,2.0),
                FadeOut::create(0.48),NULL),NULL));

		auto fire = Sprite::createWithSpriteFrameName("G1.png");
		fire->setPosition( Vec2(
            origin.x + visibleSize.width*0.445 + showCards[0]->getTextureRect().size.width*1.96,
            origin.y + visibleSize.height*0.315));
		fire->setTag(4);
		fire->setScale(0);
		myframe->addChild(fire,23,IMG_G1_EFFECT_TAG_ID);

		auto animation = Animation::create();
		animation->addSpriteFrameWithFile("G1.png");
		animation->addSpriteFrameWithFile("G2.png");
		animation->addSpriteFrameWithFile("G3.png");
		animation->addSpriteFrameWithFile("G4.png");
		animation->setDelayPerUnit(0.1f);
		animation->setRestoreOriginalFrame(true);
		auto fireAnimation = Animate::create(animation);
		auto fireEffect = TargetedAction::create(fire,Sequence::create(
            DelayTime::create(0.84),
            ScaleTo::create(0,1),
            fireAnimation,
            ScaleTo::create(0,0),NULL));
        
		auto backGoundEffect = Spawn::create(
            lightEffect,
            circleEffect,
            fireEffect,NULL);
        
		Spawn *simple_seq = simple_tip_effect( getEffectVec(_roundManager->_curPlayer),"peng.png" );
        
		auto PengEffectNode = Node::create();
		PengEffectNode->_ID=1;
		myframe->addChild(PengEffectNode,1,PENG_EFFECT_NODE_ID);
        
		auto callFunc_update_list = CCCallFunc::create([=](){
			if(ifEffectTime) {
				ifEffectTime=false;
				if(ifUpdateDuringEffect) {
					ifUpdateDuringEffect=false;
					_roundManager->_curEffectCardKind=ck_NOT_DEFINED;
					_roundManager->_curEffectCardStatus=c_NOT_DEFINDED;
					card_list_update(no);
				} else  {
					int sameCardNum=0;
					for(int a=list->atcvie_place-1;a>=0;a--) {
						if(list->data[a].kind==_roundManager->_curEffectCardKind
                            &&list->data[a].status==_roundManager->_curEffectCardStatus){
							sameCardNum++;
							((Sprite*)myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+1*20+a))->setVisible(true);
							if(sameCardNum==3){
								_roundManager->_curEffectCardKind=ck_NOT_DEFINED;
								_roundManager->_curEffectCardStatus=c_NOT_DEFINDED;
								break;
							}
						}
					}
				}
			}
		});

		PengEffectNode->runAction(Sequence::create(
            hideReminder,Spawn::create(
                simple_seq,
                _SpeakAction(PENG),Spawn::create(
                hide2CardsInhand),Spawn::create(
                moveRightCardInHand,
                moveLeftCardInHand,NULL),Sequence::create(
                    DelayTime::create(0.42),
                    hideOutcard,NULL),Spawn::create(
                backGoundEffect,
                move3PengCards,NULL),Sequence::create(
                DelayTime::create(0.84),
                _Speak("Music/paizhuangji.ogg"),NULL),NULL),Sequence::create(CCCallFunc::create(this,callfunc_selector(
            NetRaceLayer::delete_ActionEffect)),
            callFunc_update_list,CallFunc::create([=](){
			if(myframe->getChildByTag(PENG_EFFECT_NODE_ID))
				myframe->removeChildByTag(PENG_EFFECT_NODE_ID,true);}),/*actcheckagain,*/NULL),NULL));
        
        
		myframe->runAction( Sequence::create( 
            CCCallFuncN::create(this,callfuncN_selector(
                NetRaceLayer::peng_dispatch)), 
            CCCallFunc::create([=](){
    			_roundManager->_actionToDo = _roundManager->_players[_roundManager->_curPlayer]->get_parter()->ActiontodoCheckAgain();
    			waitfor_myaction(no);}), 
            CallFunc::create([=](){
                ifMyShowCardTime=true;}),
            NULL));
	}
}

void NetRaceLayer::an_gang_tip_effect(Node *psender)
{
    LOGGER_WRITE("%s",__FUNCTION__);

	int no=psender->_ID;
	auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
	myframe->_ID=no;

	continue_gang_times++;
	_roundManager->_lastActionSource = no;
    
	if(_roundManager->_actionToDo&a_AN_GANG) {
		_roundManager->_actionToDo=a_AN_GANG;
		_roundManager->_lastAction=a_AN_GANG;
		_roundManager->_lastActionWithGold=a_AN_GANG;
	} else if(_roundManager->_actionToDo&a_SHOU_GANG) {
		_roundManager->_actionToDo=a_SHOU_GANG;
		_roundManager->_lastAction=a_SHOU_GANG;
		_roundManager->_lastActionWithGold=a_SHOU_GANG;
	}

    if(no!=1) {
		myframe->runAction(Sequence::create(
            Spawn::create(
                _SpeakAction(GANG),
                simple_tip_effect(getEffectVec(_roundManager->_curPlayer),"gang.png"),NULL), CallFunc::create([=](){
			GoldNumInsert(no,1,_roundManager->_curPlayer);}), Sequence::create(CCCallFunc::create(this,callfunc_selector(
            NetRaceLayer::delete_act_tip)), CCCallFuncN::create(this,callfuncN_selector(
            NetRaceLayer::angang_dispatch)), CCCallFuncN::create(this,callfuncN_selector(
            NetRaceLayer::update_card_list)),
            _Speak("down.ogg"), CCCallFunc::create([=](){
			_roundManager->_curPlayer=no;}),CCCallFunc::create(this,callfunc_selector(
            NetRaceLayer::call_distribute_card)),NULL),NULL));
	}
	else {
		for(int NodeNum=0;NodeNum<3;NodeNum++) {
            _Remove(myframe,PENG_EFFECT_NODE_ID+NodeNum);
            
			if(ifEffectTime) {
				ifEffectTime=false;
				ifUpdateDuringEffect=false;
				_roundManager->_curEffectCardKind=ck_NOT_DEFINED;
				_roundManager->_curEffectCardStatus=c_NOT_DEFINDED;
				delete_ActionEffect();
				card_list_update(no);
			}
		}
        
        /**********************
            clear screen
        **********************/
		auto shadeAction = TargetedAction::create((Sprite*)myframe->getChildByTag(GANG_REMING_ACT_BKG_TAG_ID),
            Sequence::create(
                ScaleTo::create(0,1),
            Spawn::create(
                FadeOut::create(0.18),
                ScaleTo::create(0.18,1.3),NULL),NULL));
        
		auto hideReminder = Sequence::create(Spawn::create(shadeAction,CCCallFunc::create([=](){
		    for (int i=0;i<11;i++) {
                if(i==GANG_REMING_ACT_BKG_TAG_ID) {
                    continue;
                } else {
                    _Remove(myframe,REMIND_ACT_TAG_ID+i);
                }
            }}),NULL),CCCallFunc::create([=]() {
			if(myframe->getChildByTag(GANG_REMING_ACT_BKG_TAG_ID))
				myframe->removeChildByTag(GANG_REMING_ACT_BKG_TAG_ID,true);}),NULL);

        /**********************
            logical
        **********************/
		auto list = _roundManager->_players[no]->get_parter()->get_card_list();
		int gang[4];
        Card outCard;

        if( !_roundManager->IsTing(_roundManager->_curPlayer) ) {/* is no equals _curPlayer ??? */
			_roundManager->FindGangCards(no,gang);
            outCard = list->data[gang[0]];
            
			_roundManager->_curEffectCardKind = outCard.kind;
			_roundManager->_curEffectCardStatus = c_AN_GANG;
			ifEffectTime=true;
			ifUpdateDuringEffect=true;
		} else if(no==1) { /* get gang card from kou cards */
			gang[3] = list->len-1;
			int p = 0;
			for(int i=0; i<list->atcvie_place; i++){
				if(list->data[i].kind==list->data[gang[3]].kind)
					gang[p++]=i;
			}
            outCard = list->data[gang[0]];
		}

		int ifZeroPointTwo=0;
		int ifLeft=0;
		if(MyCardChoosedNum!=-1) {
			if(MyCardChoosedNum<list->len-5)
				ifLeft=1;
			if(MyCardChoosedNum==gang[3]||MyCardChoosedNum==gang[2]||MyCardChoosedNum==gang[1]||MyCardChoosedNum==gang[0]) {
				if(MyCardChoosedNum!=(list->len-1))
					ifZeroPointTwo=1;
				MyCardChoosedNum=-1;
			} else if(MyCardChoosedNum<gang[3]&&MyCardChoosedNum>gang[2])
				MyCardChoosedNum+=1;
			else if(MyCardChoosedNum<gang[0])
				MyCardChoosedNum+=4;
		}

        /**********************
            move 4 cards
        **********************/
        TargetedAction *moveGangCardEffect[4];
        _CreateGangCardsMotion(moveGangCardEffect);

        /**********************
            move 4 cards in hand
        **********************/
        Size FreeCardSize = Sprite::createWithTexture(g_my_free->getTexture())->getTextureRect().size;
		auto curKindOfGang = list->data[gang[0]].kind;

        TargetedAction *moveGangCardInHand[4];
        _CreateGangCardInHandMotion(moveGangCardInHand, gang, curKindOfGang);

		auto moveAngangCards = Spawn::create(
            moveGangCardEffect[0],moveGangCardEffect[1],moveGangCardEffect[2],moveGangCardEffect[4],
            moveGangCardInHand[0],moveGangCardInHand[1],moveGangCardInHand[2],moveGangCardInHand[3],NULL);

        /**********************
            move free cards in hand
        **********************/
        /* is there any possible that a card is activated and its status is FREE ??? */
        int actionStartPlace=0;
		for(int i=0;i<=list->atcvie_place;i++)
			if(list->data[i].status==c_MING_KOU || list->data[i].status==c_FREE) {
				actionStartPlace = i;
				break;
			}

        const float GAP = (list->atcvie_place==0)?(0.5):(0.0);
        
		Vector<FiniteTimeAction *>gang_list_seq;
		Spawn* moveFreeCards;
        
		if( outCard.kind==list->data[list->len-1].kind ) {
			for(int i=actionStartPlace;i<list->len-1;i++)
			{
                auto curPos=myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+no*20+i)->getPosition();
                TargetedAction *motion;
                
				if(i<gang[0]&&(list->data[i].status==c_MING_KOU||list->data[i].status==c_FREE))
				{
					motion=Sequence::create(
                        DelayTime::create(0.18),
                        MoveTo::create(0.3,Vec2(
                            curPos.x+GangCardSize.width*(3.5+GAP),
                            curPos.y)),NULL);
				}
				else if(i==gang[0]||i==gang[1]||i==gang[2])
				{
					motion=TargetedAction::create(myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+no*20+i),
                        Sequence::create(
                            ScaleTo::create(0,0),NULL));
				}
				else if(i>gang[2])
				{
					motion = TargetedAction::create(myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+no*20+i),
                        Sequence::create(
                            DelayTime::create(0.18),
                            MoveTo::create(0.3,Vec2(
                                curPos.x+(GangCardSize.width*(3.5+GAP)-FreeCardSize.width*(3+ifZeroPointTwo*0.2)),
                                curPos.y)),NULL));
				}

                gang_list_seq.insert(i-actionStartPlace,motion);
			}
            
			TargetedAction *motion = TargetedAction::create( myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+no*20+list->len-1),
                Sequence::create(
                    DelayTime::create(0.18),
                    ScaleTo::create(0,0),NULL));
            
            gang_list_seq.insert(i-actionStartPlace,motion);
		} else {
			for(int i=actionStartPlace;i<=list->len-1;i++)
			{
                auto curPos=myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+no*20+i)->getPosition();
                TargetedAction *motion;
                
				if(i<gang[0]&&(list->data[i].status==c_MING_KOU||list->data[i].status==c_FREE)) {
					motion = TargetedAction::create(myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+no*20+i),
                        Sequence::create(
                            DelayTime::create(0.18),
                            MoveTo::create(0.3,Vec2(
                                curPos.x + GangCardSize.width*(3.5+GAP),
                                curPos.y)),NULL));
				} else if(i==gang[0]||i==gang[1]||i==gang[2]) {
					motion = TargetedAction::create(myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+no*20+i),
                        Sequence::create(
                            ScaleTo::create(0,0),NULL));
				} else if(i==gang[3]) {
					motion = TargetedAction::create(myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+no*20+i),
                        Sequence::create(
                            ScaleTo::create(0,0),NULL));
                } else if(i>gang[3]) {
                    MoveTo* actionMove;
                    if( (i==list->len-1) && (outCard.kind!=list->data[list->len-1].kind)) {
						auto curLeftPos = myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+no*20+i-1)->getPosition();
						int instanceBetween = curPos.x-curLeftPos.x-FreeCardSize.width;
                        
						actionMove = MoveTo::create(0.3,Vec2(
                            curPos.x+(GangCardSize.width*(3.5+GAP)-FreeCardSize.width*(4+ifZeroPointTwo*0.2))-instanceBetween,
                            curPos.y));
					}
					else
						actionMove = MoveTo::create(0.3,Vec2(
						    curPos.x+(GangCardSize.width*(3.5+GAP)-FreeCardSize.width*(4+ifZeroPointTwo*0.2)),
						    curPos.y));
                    
					motion = TargetedAction::create(myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+no*20+i),
                        Sequence::create(
                            DelayTime::create(0.18),
                            actionMove,NULL));
				}

                gang_list_seq.insert(i-actionStartPlace,ll_action);
			}
		}
        
        moveFreeCards = Spawn::create(gang_list_seq);

        /**********************
            update tag
        **********************/
		for(int i=list->len-1;i>=0;i--) {
			int curTag=HAND_IN_CARDS_TAG_ID+1*20+i;
            
			if(!myframe->getChildByTag(curTag))
				continue;
            
			auto EveryCard=(Sprite*)myframe->getChildByTag(curTag);
            
			if( gang[0]<list->atcvie_place ) {/* peng cards transfer to gang cards */
				if(i==gang[3])
					EveryCard->setTag(EFFECT_TEMP_CARD_TAG_FOUR);
				else if(i>gang[2] && i<gang[3]) {
                    EveryCard->setTag(curTag+1);
                    
                    if(i==gang[2]+1) {       /* insert 1 addition gang card*/
                        ((Sprite*)myframe->getChildByTag(EFFECT_TEMP_CARD_TAG_FOUR))->setTag(HAND_IN_CARDS_TAG_ID+1*20+gang[2]+1);
                        break;
                    }
                }
			} else {/* gang directly */
				if(i>gang[2] && i<gang[3]) {
					EveryCard->setTag(curTag+1);      /*right shift*/
                } else if(i==gang[3]||i==gang[2]||i==gang[1]||i==gang[0]) {
					if(i==gang[3])
						EveryCard->setTag(EFFECT_TEMP_CARD_TAG_FOUR);
					if(i==gang[2])
						EveryCard->setTag(EFFECT_TEMP_CARD_TAG_THREE);
					if(i==gang[1])
						EveryCard->setTag(EFFECT_TEMP_CARD_TAG_TWO);
					if(i==gang[0])
						EveryCard->setTag(EFFECT_TEMP_CARD_TAG_ONE);
				} else if(i>=list->atcvie_place && i<gang[0]) {
					EveryCard->setTag(curTag+4);      /* leave for 4 space for gang cards*/
                    
                    if(i==list->atcvie_place) {       /* insert 4 gang cards*/
                        for(int b=0;b<4;b++)
                            ((Sprite*)myframe->getChildByTag(EFFECT_TEMP_CARD_TAG_ONE+b))->setTag(HAND_IN_CARDS_TAG_ID+1*20+list->atcvie_place+b);
                        break;
                    }
                }
			}
		}

        /**********************
            background effect
        **********************/
		auto lightCircle = Sprite::createWithSpriteFrameName("4.png");
		lightCircle->setPosition(Vec2(
            origin.x+visibleSize.width*0.5+GangCardSize.width*0.98,
            origin.y+visibleSize.height*0.315));
		myframe->addChild(lightCircle,32,IMG_4_EFFECT_TAG_ID);
		lightCircle->setScale(0);
		BlendFunc tmp_oBlendFunc ={GL_SRC_ALPHA, GL_ONE};
		lightCircle->setBlendFunc(tmp_oBlendFunc);
		auto light1_action = TargetedAction::create(lightCircle, 
            Sequence::create(
                DelayTime::create(0.66),
                ScaleTo::create(0,0.4),
                ScaleTo::create(0.18,1.0),
            Spawn::create(
                ScaleTo::create(0.06,1.2),
                FadeOut::create(0.06),NULL),NULL));

		auto lightCircle1=Sprite::createWithSpriteFrameName("Q4.png");
		lightCircle1->setAnchorPoint(Vec2(0.5,0));
		lightCircle1->setPosition(Vec2(
            origin.x+visibleSize.width*0.5+GangCardSize.width*0.98,
            origin.y+visibleSize.height*0.315));
		myframe->addChild(lightCircle1,32,IMG_Q4_EFFECT_TAG_ID);
		lightCircle1->setScale(0);
		lightCircle1->setBlendFunc(tmp_oBlendFunc);
		auto light2_action=TargetedAction::create(lightCircle1,
            Sequence::create(
                DelayTime::create(0.9),
                ScaleTo::create(0,0.1),
                ScaleTo::create(0.18,0.6),
            Spawn::create(
                ScaleTo::create(0.06,0.7),
                FadeOut::create(0.06),NULL),NULL));

		auto lightCircle2=Sprite::createWithSpriteFrameName("Q4.png");
		lightCircle2->setAnchorPoint(Vec2(0.5,0));
		lightCircle2->setPosition(Vec2(
            origin.x+visibleSize.width*0.5+GangCardSize.width*0.98,
            origin.y+visibleSize.height*0.315));
		myframe->addChild(lightCircle2,32,IMG_Q4_EFFECT_TAG_ID+1);
		lightCircle2->setScale(0);
		lightCircle2->setBlendFunc(tmp_oBlendFunc);
		auto light3_action = TargetedAction::create(lightCircle2,
            Sequence::create(
                DelayTime::create(1.14),
                ScaleTo::create(0,0.1),
                ScaleTo::create(0.18,0.4),
            Spawn::create(
                ScaleTo::create(0.06,0.5),
                FadeOut::create(0.06),NULL),NULL));

		auto yellowlight=Sprite::createWithSpriteFrameName("c33.png");
		yellowlight->setPosition(Vec2(
            origin.x+visibleSize.width*0.5+GangCardSize.width*0.98,
            origin.y+visibleSize.height*0.315));
		myframe->addChild(yellowlight,30,IMG_C33_EFFECT_TAG_ID);
		yellowlight->setScale(0);
		auto yellow_action=TargetedAction::create(yellowlight,
            Sequence::create(
                DelayTime::create(0.66),
                ScaleTo::create(0.12,1),
            Spawn::create(
                ScaleTo::create(0.18,0),
                FadeOut::create(0.18),NULL),NULL));

		auto light = Sprite::createWithSpriteFrameName("c3.png");
		light->setPosition(Vec2(
            origin.x+visibleSize.width*0.5+GangCardSize.width*0.98,
            origin.y+visibleSize.height*0.315));
		light->setScale(0);
		myframe->addChild(light,31,IMG_C3_EFFECT_TAG_ID);
		light->setOpacity(200);
		auto light_action=TargetedAction::create(light,Sequence::create(
                DelayTime::create(0.66),
                ScaleTo::create(0.12,0.5),
            Spawn::create(
                ScaleTo::create(0.18,0),
                FadeOut::create(0.18),NULL),NULL));

        /* final effect */
		auto callFunc_update_list=CCCallFunc::create([=](){
			if(ifEffectTime) {
				ifEffectTime=false;
				if(ifUpdateDuringEffect) {
					ifUpdateDuringEffect=false;
					_roundManager->_curEffectCardKind=ck_NOT_DEFINED;
					_roundManager->_curEffectCardStatus=c_NOT_DEFINDED;
					card_list_update(no);
				} else {
					int sameCardNum=0;
					for(int a=list->atcvie_place-1;a>=0;a--)
					{
						if(list->data[a].kind==_roundManager->_curEffectCardKind&&list->data[a].status==_roundManager->_curEffectCardStatus)
						{
							sameCardNum++;
							((Sprite*)myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+1*20+a))->setVisible(true);
							if(sameCardNum==4)
							{
								_roundManager->_curEffectCardKind=ck_NOT_DEFINED;
								_roundManager->_curEffectCardStatus=c_NOT_DEFINDED;
								break;
							}
						}
					}
				}
			}
		});

		auto AnGangEffectNode = Node::create();
		AnGangEffectNode->_ID=1;
		myframe->addChild(AnGangEffectNode,1,AN_GANG_EFFECT_NODE);
        
		AnGangEffectNode->runAction( 
    		Sequence::create(
                hideReminder,
                Spawn::create(
                    simple_tip_effect(getEffectVec(_roundManager->_curPlayer),"gang.png"),
                    _SpeakAction(GANG),
                    moveFreeCards,
                    Spawn::create(
            		    moveAngangCards,
            		    Sequence::create(
            		        DelayTime::create(0.66),CCCallFunc::create([=]() {
                    		startParticleSystem(0.3);
                    		}),NULL),
                        light1_action,
                        light2_action,
                        light3_action,
                        yellow_action,
                        light_action,NULL),
            		Sequence::create(
                        DelayTime::create(0.66),CallFunc::create([=](){
                        SimpleAudioEngine::sharedEngine()->playEffect("Music/paizhuangji.ogg");}),NULL),NULL),
                Sequence::create(CCCallFunc::create(this,callfunc_selector(
                    NetRaceLayer::delete_ActionEffect)),
                    callFunc_update_list,
                    _Speak("lanpai.ogg"),NULL),CallFunc::create([=](){
                _Remove(myframe,AN_GANG_EFFECT_NODE);}),NULL));
        
		myframe->_ID=1;
		myframe->runAction(Sequence::create(CCCallFuncN::create(this,callfuncN_selector(
            NetRaceLayer::angang_dispatch)),
            DelayTime::create(0.48), CallFunc::create([=](){
			GoldNumInsert(no,1,_roundManager->_curPlayer);}), CCCallFunc::create([=]() {
			_roundManager->_curPlayer=no;}),CCCallFunc::create(this,callfunc_selector(
            NetRaceLayer::call_distribute_card)),NULL));
	}
}

void NetRaceLayer::QiangGangHuJudge()
{
    LOGGER_WRITE("%s",__FUNCTION__);

	_roundManager->_isCardFromOthers=true;
	int no=(_roundManager->_curPlayer+1)%3;
	unsigned char curTingStatus=_roundManager->_players[_roundManager->_curPlayer]->get_parter()->get_ting_status();
	unsigned char action1=_roundManager->_players[no]->get_parter()->hand_in(
        _roundManager->_lastHandedOutCard,
        _roundManager->_isCardFromOthers,
        curTingStatus,
        false,
        a_QIANG_GANG,
        continue_gang_times,
        _roundManager->_isGangHua
    );

	int no1=(_roundManager->_curPlayer+2)%3;
	unsigned char action2=_roundManager->_players[no1]->get_parter()->hand_in(
        _roundManager->_lastHandedOutCard,
        _roundManager->_isCardFromOthers,
        curTingStatus,
        false,
        a_QIANG_GANG,
        continue_gang_times,
        _roundManager->_isGangHua
    );
	//action1=a_HU;
	//action2=a_HU;
	if((action1&a_HU)&&(action2&a_HU))//双响
	{
		_roundManager->_lastActionWithGold=a_QIANG_GANG;
		HideClock();
		if((no!=1&&no1!=1)||((no==1||no1==1)&&_roundManager->_players[1]->get_parter()->get_ting_status()==1))
		{
			hu_effect_tip(3);
			distribute_event(DOUBLE_HU_WITH_ME,NULL);
		}
		else if((no==1||no1==1) && _roundManager->_players[1]->get_parter()->get_ting_status()==0)
		{
			_roundManager->_isDoubleHuAsking = true;
            
			if(no==1) {
				_roundManager->_actionToDo=action1;
				_roundManager->_otherOneForDouble = no1;
			} else {
				_roundManager->_actionToDo=action2;
				_roundManager->_otherOneForDouble = no;
			}
            
			waitfor_myaction(1);
			return;
		}
	}
	else if(action1&a_HU||action2&a_HU)//点炮
	{
        HideClock();
		if((no==1&&(action1&a_HU))||(no1==1&&(action2&a_HU)))
		{
			if(_roundManager->_players[1]->get_parter()->get_ting_status()==1)//&&_roundManager->_actionToDo&a_HU)
			{
				_roundManager->_lastActionWithGold=a_QIANG_GANG;
				auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
				myframe->_ID=1;
				auto huCallFunc=CallFunc::create([=](){hu_effect_tip(1);});
				myframe->runAction(huCallFunc);
			}
			else
			{
				_roundManager->_isQiangGangAsking=true;
				if(no==1)
					_roundManager->_actionToDo=action1;
				else
					_roundManager->_actionToDo=action2;
				waitfor_myaction(1);
				return;
			}
		}
		else if(no!=1&&(action1&a_HU))
		{
			_roundManager->_lastActionWithGold=a_QIANG_GANG;
			hu_effect_tip(no);
		}
		else if(no1!=1&&(action2&a_HU))
		{
			_roundManager->_lastActionWithGold=a_QIANG_GANG;
			hu_effect_tip(no1);
		}
	}
	else
	{
		_roundManager->_isCardFromOthers=false;
		auto GoldAccount=CallFunc::create([=](){
			GoldNumInsert(_roundManager->_qiangGangTargetNo,2,_roundManager->_curPlayer);	
		});
		auto distributeFunc=CallFunc::create([=](){call_distribute_card();});
		auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
		myframe->runAction(Sequence::create(GoldAccount,distributeFunc,NULL));
	}
}
void NetRaceLayer::ming_gang_tip_effect(Node *psender)
{
    LOGGER_WRITE("%s",__FUNCTION__);

	int no=psender->_ID;
	continue_gang_times++;
	auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
	myframe->_ID=no;
	_roundManager->_actionToDo=a_MING_GANG;
	_roundManager->_lastAction=a_MING_GANG;
	_roundManager->_lastActionWithGold=a_MING_GANG;
	_roundManager->_lastActionSource=no;
	float delayTime=0.18;
	Card kind_out_card;
	//auto QiangGangJudge=CallFunc::create([=](){});
	if(_roundManager->_isCardFromOthers) {
		int outcard_place = _roundManager->_players[_roundManager->_curPlayer]->get_parter()->getOutCardList()->length;
		_roundManager->_players[_roundManager->_curPlayer]->get_parter()->getOutCardList()->getCard(kind_out_card,outcard_place);
		_roundManager->_players[_roundManager->_curPlayer]->get_parter()->getOutCardList()->deleteItem();
		if(myframe->getChildByTag(HAND_OUT_CARDS_TAG_ID+_roundManager->_curPlayer*25 + outcard_place - 1))
		{
			myframe->removeChildByTag(HAND_OUT_CARDS_TAG_ID+_roundManager->_curPlayer*25 + outcard_place - 1);
		}

        _roundManager->RecordOutCard(kind_out_card);
        _roundManager->RecordOutCard(kind_out_card);
        _roundManager->RecordOutCard(kind_out_card);
	} else {
		auto list=_roundManager->_players[no]->get_parter()->get_card_list();
		kind_out_card=list->data[list->len-1];

        _roundManager->RecordOutCard(kind_out_card);
	}
    
	CallFunc*GangVoice = _SpeakAction(GANG);
    
    if(no!=1)
	{

		auto callFunc1=CCCallFunc::create(this,callfunc_selector(NetRaceLayer::delete_act_tip));
		auto minggang_action=CCCallFuncN::create(this,callfuncN_selector(NetRaceLayer::minggang_dispatch));
		auto callFunc_update_list=CCCallFuncN::create(this,callfuncN_selector(NetRaceLayer::update_card_list));
		CallFunc* dis_action;
		if(!_roundManager->_isCardFromOthers)
		{
			_roundManager->_qiangGangTargetNo=no;
			dis_action=CCCallFunc::create(this,callfunc_selector(NetRaceLayer::QiangGangHuJudge));
		}
		else//card from others
			dis_action=CCCallFunc::create(this,callfunc_selector(NetRaceLayer::call_distribute_card));
        
		auto update_list_seq=Sequence::create(callFunc1,minggang_action,callFunc_update_list,CCCallFunc::create([=]()
		{
			_roundManager->_curPlayer=no;
		}),dis_action,NULL);//dis_action

		Spawn *simple_seq=simple_tip_effect(getEffectVec(_roundManager->_curPlayer),"gang.png");///墨迹等。。。update_list_seq 最后处理，转换cur_player
		auto GoldAccount=CallFunc::create([=](){
			GoldNumInsert(no,2,_roundManager->_curPlayer);	
		});
		Sequence *gang_seq;
		if(_roundManager->_isCardFromOthers)
		{
			auto curOutCard=myframe->getChildByTag(OUT_CARD_FRAME_TAG_ID);
			auto action=Sequence::create(DelayTime::create(0.06),ScaleTo::create(0,0),NULL);
			auto no1seq1=TargetedAction::create(curOutCard,action);
			auto no1_seq1=Sequence::create(CCCallFunc::create([=]()
			{
				auto show_card_indicator=this->getChildByTag(SHOWCARD_INDICATOR_TAG_ID);
				show_card_indicator->setVisible(false);
			}),no1seq1,NULL);
			gang_seq=Sequence::create(no1_seq1,Spawn::create(simple_seq,GangVoice,NULL),GoldAccount,update_list_seq,NULL);
		}
		else
			gang_seq=Sequence::create(Spawn::create(GangVoice,simple_seq,NULL),update_list_seq,NULL);
		myframe->runAction(gang_seq);
	}
	else
	{
		for(int NodeNum=0;NodeNum<3;NodeNum++)//PENG_EFFECT_NODE_ID
		{
			if(myframe->getChildByTag(PENG_EFFECT_NODE_ID+NodeNum))
				myframe->removeChildByTag(PENG_EFFECT_NODE_ID+NodeNum,true);
			if(ifEffectTime)
			{
				ifEffectTime=false;
				ifUpdateDuringEffect=false;
				_roundManager->_curEffectCardKind=ck_NOT_DEFINED;
				_roundManager->_curEffectCardStatus=c_NOT_DEFINDED;
				delete_ActionEffect();
				card_list_update(no);
			}
		}
		_roundManager->_curEffectCardKind=kind_out_card.kind;
		_roundManager->_curEffectCardStatus=c_MING_GANG;
		ifEffectTime=true;
		ifUpdateDuringEffect=true;
		auto shade_act=(Sprite*)myframe->getChildByTag(GANG_REMING_ACT_BKG_TAG_ID);
		auto fadeOut=FadeOut::create(0.18);
		auto easeBounce=ScaleTo::create(0.18,1.3);
		auto spawn=Spawn::create(fadeOut,easeBounce,NULL);
		auto seq=Sequence::create(ScaleTo::create(0,1),spawn,NULL);
		auto shadeAction=TargetedAction::create(shade_act,seq);
		auto spa1=Spawn::create(shadeAction,CCCallFunc::create([=]()
		{
			for(int i=0;i<4;i++)
			{
				if(myframe->getChildByTag(REMIND_ACT_TAG_ID+i))
					myframe->removeChildByTag(REMIND_ACT_TAG_ID+i,true);
			}
			for(int i=5;i<11;i++)
			{
				if(myframe->getChildByTag(REMIND_ACT_TAG_ID+i))
					myframe->removeChildByTag(REMIND_ACT_TAG_ID+i,true);
			}
		}),NULL);
		auto g_seq1=Sequence::create(spa1,CCCallFunc::create([=]()
		{
			if(myframe->getChildByTag(GANG_REMING_ACT_BKG_TAG_ID))
				myframe->removeChildByTag(GANG_REMING_ACT_BKG_TAG_ID,true);
		}),NULL);

		Size cardPengSize;
		auto cardInHand=Sprite::createWithTexture(g_my_free->getTexture());
		Size cardSize=cardInHand->getTextureRect().size;
		int gang2,gang3,gang4;
		auto list=_roundManager->_players[1]->get_parter()->get_card_list();
		/////////////////////////////////
		Sprite *s_curOutCard;
		Sequence *g_out_card_action;
		int l_len;
		if(!_roundManager->_isCardFromOthers)
		{
			auto Gang1Card=(Sprite*)myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+no*20+list->len-1);//gang1
			Gang1Card->runAction(ScaleTo::create(0,0));
			//s_curOutCard=(Sprite*)myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+no*20+list->len-1);
			s_curOutCard=Sprite::createWithTexture(g_my_free->getTexture());
			s_curOutCard->setAnchorPoint(Vec2(0,0));
			s_curOutCard->setPosition(Vec2(Gang1Card->getPosition().x,Gang1Card->getPosition().y));
			auto s_curKind=Sprite::createWithTexture(g_card_kind[kind_out_card.kind]->getTexture());
			s_curKind->setAnchorPoint(Vec2(0.5,0.5));
			s_curKind->setPosition(Vec2(s_curOutCard->getTextureRect().size.width/2,s_curOutCard->getTextureRect().size.height*0.4));
			s_curOutCard->addChild(s_curKind,1);
			myframe->addChild(s_curOutCard,29,EFFET_NEWCATD1_TAG);
			Gang1Card->runAction(ScaleTo::create(0,0));
			if(s_curOutCard)
			{
				g_out_card_action=Sequence::create(DelayTime::create(0.06),NULL);
			}
		}
		else
		{
			s_curOutCard=(Sprite*)myframe->getChildByTag(OUT_CARD_FRAME_TAG_ID);
			auto action=Sequence::create(DelayTime::create(0.06),ScaleTo::create(0,0),NULL);
			auto outcardaction=TargetedAction::create(s_curOutCard,action);
			auto s_out_card_action=Spawn::create(CCCallFunc::create([=]()
			{
				auto show_card_indicator=this->getChildByTag(SHOWCARD_INDICATOR_TAG_ID);
				show_card_indicator->setVisible(false);
			}),outcardaction,NULL);
			g_out_card_action=Sequence::create(s_out_card_action,NULL);
		}
		if(!_roundManager->_isCardFromOthers)
			l_len=list->len-1;
		else
			l_len=list->len;
		for(int i=0;i<l_len;i++)
		{
			if(kind_out_card.kind==list->data[i].kind)
			{
				if(i==0)
				{
					gang2=i;
				}
				else if(i>0 && kind_out_card.kind!=list->data[i-1].kind)
				{
					gang2=i;
				}
				else if(i==1 && kind_out_card.kind==list->data[i-1].kind)
				{
					gang3=i;
				}
				else if(i>1 && kind_out_card.kind==list->data[i-1].kind && kind_out_card.kind!=list->data[i-2].kind)
				{
					gang3=i;
				}
				else if(i>1 && kind_out_card.kind==list->data[i-1].kind && kind_out_card.kind==list->data[i-2].kind)
				{
					gang4=i;
				}
			}
		}

		int ifZeroPointTwo=0;
		int ifLeft=0;
		if(MyCardChoosedNum!=-1)
		{
			if(MyCardChoosedNum<list->len-4)
				ifLeft=1;
			if(!_roundManager->_isCardFromOthers)
			{
				if(MyCardChoosedNum>gang4)
					MyCardChoosedNum+=1;
			}
			else
			{
				if(MyCardChoosedNum>gang4)
					MyCardChoosedNum+=1;
				else if(MyCardChoosedNum==gang2||MyCardChoosedNum==gang3||MyCardChoosedNum==gang4)
				{
					ifZeroPointTwo=1;
					MyCardChoosedNum=-1;
				}
				else if(MyCardChoosedNum<gang2)
					MyCardChoosedNum+=4;
			}
		}

		auto tempMyGangCard=Sprite::createWithTexture(g_my_peng->getTexture());
		auto Pengsize=tempMyGangCard->getTextureRect().size;
		Vec2 basePos;
		Sprite* baseCard;
		if(!_roundManager->_isCardFromOthers)
		{
			baseCard=(Sprite*)myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+1*20+list->len-2);
			basePos=baseCard->getPosition();
			distributeCardPos=Vec2(basePos.x+baseCard->getBoundingBox().size.width+30-cardSize.width*ifZeroPointTwo*0.2,basePos.y);
		}
		else if(_roundManager->_isCardFromOthers)
		{
			baseCard=(Sprite*)myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+1*20+list->len-4);
			basePos=baseCard->getPosition();
			if(list->atcvie_place>0)
				distributeCardPos=Vec2(basePos.x+Pengsize.width*3.5+30+baseCard->getBoundingBox().size.width-cardSize.width*ifZeroPointTwo*0.2*ifLeft,basePos.y);
			else
				distributeCardPos=Vec2(basePos.x+Pengsize.width*4+30+baseCard->getBoundingBox().size.width-cardSize.width*ifZeroPointTwo*0.2*ifLeft,basePos.y);
		}

		auto show_card=Sprite::createWithTexture(g_my_peng->getTexture());
		cardPengSize=show_card->getTextureRect().size;
		auto show_card_kind=Sprite::createWithTexture(g_mid_card_kind[kind_out_card.kind]->getTexture());
		show_card_kind->setAnchorPoint(Vec2(0.5,0.5));
		show_card_kind->setPosition(Vec2(show_card->getTextureRect().size.width/2,show_card->getTextureRect().size.height*0.65));
		show_card->addChild(show_card_kind);
		show_card->setAnchorPoint(Vec2(0,0));
		if(!_roundManager->_isCardFromOthers)
		{
			show_card->setPosition(Vec2(origin.x+visibleSize.width*0.5+cardPengSize.width*0.98,origin.y+visibleSize.height*0.255));
		}
		else
		{
			auto curPos=s_curOutCard->getPosition();
			show_card->setPosition(curPos);
		}
		show_card->setScale(0);
		myframe->addChild(show_card,29,SINGLE_ACTION_EFFECT_TAG_ID);
		Sequence* l_action1;
		if(_roundManager->_isCardFromOthers)
		{
			auto seq1=Sequence::create(DelayTime::create(0.42),ScaleTo::create(0,1),MoveTo::create(0.12,Vec2(origin.x+visibleSize.width*0.5+cardPengSize.width*0.98,origin.y+visibleSize.height*0.255)),
				MoveTo::create(0.12,Vec2(origin.x+visibleSize.width*0.5+cardPengSize.width*0.98+150,origin.y+visibleSize.height*0.255)),
				MoveTo::create(0.12,Vec2(origin.x+visibleSize.width*0.5+cardPengSize.width*0.98,origin.y+visibleSize.height*0.255)),NULL);
			auto l_action1Temp=TargetedAction::create(show_card,seq1);
			l_action1=Sequence::create(l_action1Temp,NULL);
		}
		else
		{
			auto move1=MoveTo::create(0.12,Vec2(origin.x+visibleSize.width*0.5+cardPengSize.width*0.98,origin.y+visibleSize.height*0.255));
			auto seq=Sequence::create(DelayTime::create(0.42),ScaleTo::create(0,0.6),move1,ScaleTo::create(0,0),NULL);
			auto s_out_card_action=TargetedAction::create(s_curOutCard,seq);
			auto seq1=Sequence::create(ScaleTo::create(0,1),MoveTo::create(0.12,Vec2(origin.x+visibleSize.width*0.5+cardPengSize.width*0.98+150,origin.y+visibleSize.height*0.255)),
				MoveTo::create(0.12,Vec2(origin.x+visibleSize.width*0.5+cardPengSize.width*0.98,origin.y+visibleSize.height*0.255)),NULL);
			auto l_action1Temp=TargetedAction::create(show_card,seq1);
			l_action1=Sequence::create(s_out_card_action,l_action1Temp,NULL);
		}
		auto show_card1=Sprite::createWithTexture(g_my_peng->getTexture());
		auto show_card_kind1=Sprite::createWithTexture(g_mid_card_kind[kind_out_card.kind]->getTexture());
		show_card_kind1->setAnchorPoint(Vec2(0.5,0.5));
		show_card_kind1->setPosition(Vec2(show_card->getTextureRect().size.width/2,show_card->getTextureRect().size.height*0.65));
		show_card1->addChild(show_card_kind1);
		show_card1->setAnchorPoint(Vec2(0,0));
		show_card1->setScale(0);
		show_card1->setPosition(Vec2(origin.x+visibleSize.width*0.5-cardPengSize.width*1.96,origin.y+visibleSize.height*0.255));
		myframe->addChild(show_card1,29,SINGLE_ACTION2_EFFECT_TAG_ID);
		auto seq2=Sequence::create(DelayTime::create(0.42),ScaleTo::create(0,1),DelayTime::create(0.12),MoveTo::create(0.12,Vec2(origin.x+visibleSize.width*0.5-cardPengSize.width*1.96-150,origin.y+visibleSize.height*0.255)),
			MoveTo::create(0.12,Vec2(origin.x+visibleSize.width*0.5-cardPengSize.width*1.96,origin.y+visibleSize.height*0.255)),NULL);
		auto l_action2=TargetedAction::create(show_card1,seq2);

		auto show_card2=Sprite::createWithTexture(g_my_peng->getTexture());
		auto show_card_kind2=Sprite::createWithTexture(g_mid_card_kind[kind_out_card.kind]->getTexture());
		show_card_kind2->setAnchorPoint(Vec2(0.5,0.5));
		show_card_kind2->setPosition(Vec2(show_card->getTextureRect().size.width/2,show_card->getTextureRect().size.height*0.65));
		show_card2->addChild(show_card_kind2);
		show_card2->setAnchorPoint(Vec2(0,0));
		show_card2->setScale(0);
		show_card2->setPosition(Vec2(origin.x+visibleSize.width*0.5-cardPengSize.width*0.98,origin.y+visibleSize.height*0.255));
		myframe->addChild(show_card2,29,SINGLE_ACTION3_EFFECT_TAG_ID);
		auto seq3=Sequence::create(DelayTime::create(0.42),ScaleTo::create(0,1),DelayTime::create(0.12),MoveTo::create(0.12,Vec2(origin.x+visibleSize.width*0.5-cardPengSize.width*0.98-150,origin.y+visibleSize.height*0.255)),
			MoveTo::create(0.12,Vec2(origin.x+visibleSize.width*0.5-cardPengSize.width*0.98,origin.y+visibleSize.height*0.255)),NULL);
		auto l_action3=TargetedAction::create(show_card2,seq3);

		auto show_card3=Sprite::createWithTexture(g_my_peng->getTexture());
		auto show_card_kind3=Sprite::createWithTexture(g_mid_card_kind[kind_out_card.kind]->getTexture());
		show_card_kind3->setAnchorPoint(Vec2(0.5,0.5));
		show_card_kind3->setPosition(Vec2(show_card3->getTextureRect().size.width/2,show_card->getTextureRect().size.height*0.65));		
		show_card3->addChild(show_card_kind3);
		show_card3->setAnchorPoint(Vec2(0,0));
		show_card3->setScale(0);
		show_card3->setPosition(Vec2(origin.x+visibleSize.width*0.5,origin.y+visibleSize.height*0.255));
		myframe->addChild(show_card3,29,SINGLE_ACTION4_EFFECT_TAG_ID);
		auto seq4=Sequence::create(DelayTime::create(0.42),ScaleTo::create(0,1),DelayTime::create(0.12),MoveTo::create(0.12,Vec2(origin.x+visibleSize.width*0.5-150,origin.y+visibleSize.height*0.255)),
			MoveTo::create(0.12,Vec2(origin.x+visibleSize.width*0.5,origin.y+visibleSize.height*0.255)),NULL);
		auto l_action4=TargetedAction::create(show_card3,seq4);
		//auto ming_gang_spawn=Spawn::create(l_action1,l_action2,l_action3,l_action4,NULL);
		///////////////////////////////////////////////////////////////////////////////////////
		Sprite* Gang2Card;
		Sprite* Gang3Card;
		Sprite* Gang4Card;
		Sequence*Gang2Card_seq;
		TargetedAction* Gang2TarAction;
		Sequence* Gang3Card_seq;
		TargetedAction *Gang3TarAction;
		Sequence* Gang4Card_seq;
		TargetedAction* Gang4TarAction;
		auto OldGang2Card=(Sprite*)myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+1*20+gang2);//gang2
		auto OldGang2Pos=OldGang2Card->getPosition();
		auto OldGang2Size=OldGang2Card->getTextureRect().size;
		auto OldGang3Card=(Sprite*)myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+1*20+gang3);//gang3
		auto OldGang3Pos=OldGang3Card->getPosition();
		auto OldGang3Size=OldGang3Card->getTextureRect().size;
		auto OldGang4Card=(Sprite*)myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+1*20+gang4);//gang4
		auto OldGang4Pos=OldGang4Card->getPosition();
		auto OldGang4Size=OldGang4Card->getTextureRect().size;
		if(!_roundManager->_isCardFromOthers)
		{
			Gang2Card=Sprite::createWithTexture(g_my_peng->getTexture());//gang2
			Gang2Card->setAnchorPoint(Vec2(0,0));
			Gang2Card->setPosition(Vec2(OldGang2Pos.x,OldGang2Pos.y));
			auto Gang2Kind=Sprite::createWithTexture(g_mid_card_kind[kind_out_card.kind]->getTexture());
			Gang2Kind->setAnchorPoint(Vec2(0.5,0.5));
			Gang2Kind->setPosition(Vec2(OldGang2Size.width/2,OldGang2Size.height*0.65));
			Gang2Card->addChild(Gang2Kind,1);
			myframe->addChild(Gang2Card,20,EFFET_NEWCATD2_TAG);
			auto actionMove2=MoveTo::create(0.18,Vec2(origin.x+visibleSize.width*0.5-Gang2Card->getTextureRect().size.width*1.96,origin.y+visibleSize.height*0.26));
			Gang2Card_seq=Sequence::create(DelayTime::create(delayTime),actionMove2,DelayTime::create(0.06),ScaleTo::create(0,0),NULL);
			Gang2TarAction=TargetedAction::create(Gang2Card,Gang2Card_seq);

			Gang3Card=Sprite::createWithTexture(g_my_peng->getTexture());//gang3
			Gang3Card->setAnchorPoint(Vec2(0,0));
			Gang3Card->setPosition(Vec2(OldGang3Pos.x,OldGang3Pos.y));
			auto Gang3Kind=Sprite::createWithTexture(g_mid_card_kind[kind_out_card.kind]->getTexture());
			Gang3Kind->setAnchorPoint(Vec2(0.5,0.5));
			Gang3Kind->setPosition(Vec2(OldGang3Size.width/2,OldGang3Size.height*0.65));
			Gang3Card->addChild(Gang3Kind,1);
			myframe->addChild(Gang3Card,20,EFFET_NEWCATD3_TAG);
			auto actionMove3=MoveTo::create(0.18,Vec2(origin.x+visibleSize.width*0.5-Gang3Card->getTextureRect().size.width*0.98,origin.y+visibleSize.height*0.26));
			Gang3Card_seq=Sequence::create(DelayTime::create(delayTime),actionMove3,DelayTime::create(0.06),ScaleTo::create(0,0),NULL);
			Gang3TarAction=TargetedAction::create(Gang3Card,Gang3Card_seq);

			Gang4Card=Sprite::createWithTexture(g_my_peng->getTexture());//gang4
			Gang4Card->setAnchorPoint(Vec2(0,0));
			Gang4Card->setPosition(Vec2(OldGang4Pos.x,OldGang4Pos.y));
			auto Gang4Kind=Sprite::createWithTexture(g_mid_card_kind[kind_out_card.kind]->getTexture());
			Gang4Kind->setAnchorPoint(Vec2(0.5,0.5));
			Gang4Kind->setPosition(Vec2(OldGang4Size.width/2,OldGang4Size.height*0.65));
			Gang4Card->addChild(Gang4Kind,1);
			myframe->addChild(Gang4Card,20,EFFET_NEWCATD4_TAG);
			auto actionMove4=MoveTo::create(0.18,Vec2(origin.x+visibleSize.width*0.5,origin.y+visibleSize.height*0.26));
			Gang4Card_seq=Sequence::create(DelayTime::create(delayTime),actionMove4,DelayTime::create(0.06),ScaleTo::create(0,0),NULL);
			Gang4TarAction=TargetedAction::create(Gang4Card,Gang4Card_seq);
		}
		else 
		{
			Gang2Card=Sprite::createWithTexture(g_my_free->getTexture());//gang2
			Gang2Card->setScale(OldGang2Card->getScale());
			Gang2Card->setAnchorPoint(Vec2(0,0));
			Gang2Card->setPosition(Vec2(OldGang2Pos.x,OldGang2Pos.y));
			auto Gang2Kind=Sprite::createWithTexture(g_card_kind[kind_out_card.kind]->getTexture());
			Gang2Kind->setAnchorPoint(Vec2(0.5,0.5));
			Gang2Kind->setPosition(Vec2(OldGang2Size.width/2,OldGang2Size.height*0.4));
			Gang2Card->addChild(Gang2Kind,1);
			myframe->addChild(Gang2Card,20,EFFET_NEWCATD2_TAG);

			Gang3Card=Sprite::createWithTexture(g_my_free->getTexture());//gang3
			Gang3Card->setScale(OldGang3Card->getScale());
			Gang3Card->setAnchorPoint(Vec2(0,0));
			Gang3Card->setPosition(Vec2(OldGang3Pos.x,OldGang3Pos.y));
			auto Gang3Kind=Sprite::createWithTexture(g_card_kind[kind_out_card.kind]->getTexture());
			Gang3Kind->setAnchorPoint(Vec2(0.5,0.5));
			Gang3Kind->setPosition(Vec2(OldGang3Size.width/2,OldGang3Size.height*0.4));
			Gang3Card->addChild(Gang3Kind,1);
			myframe->addChild(Gang3Card,20,EFFET_NEWCATD3_TAG);

			Gang4Card=Sprite::createWithTexture(g_my_free->getTexture());//gang4
			Gang4Card->setScale(OldGang4Card->getScale());
			Gang4Card->setAnchorPoint(Vec2(0,0));
			Gang4Card->setPosition(Vec2(OldGang4Pos.x,OldGang4Pos.y));
			auto Gang4Kind=Sprite::createWithTexture(g_card_kind[kind_out_card.kind]->getTexture());
			Gang4Kind->setAnchorPoint(Vec2(0.5,0.5));
			Gang4Kind->setPosition(Vec2(OldGang4Size.width/2,OldGang4Size.height*0.4));
			Gang4Card->addChild(Gang4Kind,1);
			myframe->addChild(Gang4Card,20,EFFET_NEWCATD4_TAG);
			if(list->atcvie_place>0)
			{
				//gang2
				auto actionMove2=MoveTo::create(0.18,Vec2(origin.x+visibleSize.width*0.5-Gang2Card->getTextureRect().size.width*0.6*1.96,origin.y+visibleSize.height*0.26));
				Gang2Card_seq=Sequence::create(DelayTime::create(delayTime),ScaleTo::create(0,0.6),actionMove2,DelayTime::create(0.06),ScaleTo::create(0,0),NULL);
				Gang2TarAction=TargetedAction::create(Gang2Card,Gang2Card_seq);
				//gang3
				auto actionMove3=MoveTo::create(0.18,Vec2(origin.x+visibleSize.width*0.5-Gang3Card->getTextureRect().size.width*0.6*0.98,origin.y+visibleSize.height*0.26));
				Gang3Card_seq=Sequence::create(DelayTime::create(delayTime),Spawn::create(ScaleTo::create(0,0.6),MoveTo::create(0,Vec2(OldGang3Pos.x-cardSize.width*0.4,OldGang3Pos.y)),NULL),actionMove3,DelayTime::create(0.06),ScaleTo::create(0,0),NULL);
				Gang3TarAction=TargetedAction::create(Gang3Card,Gang3Card_seq);
				//gang4
				auto actionMove4=MoveTo::create(0.18,Vec2(origin.x+visibleSize.width*0.5,origin.y+visibleSize.height*0.26));
				Gang4Card_seq=Sequence::create(DelayTime::create(delayTime),Spawn::create(ScaleTo::create(0,0.6),MoveTo::create(0,Vec2(OldGang4Pos.x-cardSize.width*0.8,OldGang4Pos.y)),NULL),actionMove4,DelayTime::create(0.06),ScaleTo::create(0,0),NULL);
				Gang4TarAction=TargetedAction::create(Gang4Card,Gang4Card_seq);
			}
			else if(list->atcvie_place==0)
			{
				//gang2
				auto actionMove2=MoveTo::create(0.18,Vec2(origin.x+visibleSize.width*0.5-Gang2Card->getTextureRect().size.width*0.6*1.96,origin.y+visibleSize.height*0.26));
				Gang2Card_seq=Sequence::create(DelayTime::create(delayTime),ScaleTo::create(0,0.6),actionMove2,DelayTime::create(0.06),ScaleTo::create(0,0),NULL);
				Gang2TarAction=TargetedAction::create(Gang2Card,Gang2Card_seq);
				//gang3
				auto actionMove3=MoveTo::create(0.18,Vec2(origin.x+visibleSize.width*0.5-Gang3Card->getTextureRect().size.width*0.6*0.98,origin.y+visibleSize.height*0.26));
				Gang3Card_seq=Sequence::create(DelayTime::create(delayTime),Spawn::create(ScaleTo::create(0,0.6),MoveTo::create(0,Vec2(OldGang3Pos.x-cardSize.width*0.4,OldGang3Pos.y)),NULL),actionMove3,DelayTime::create(0.06),ScaleTo::create(0,0),NULL);
				Gang3TarAction=TargetedAction::create(Gang3Card,Gang3Card_seq);
				//gang4
				auto actionMove4=MoveTo::create(0.18,Vec2(origin.x+visibleSize.width*0.5,origin.y+visibleSize.height*0.26));
				Gang4Card_seq=Sequence::create(DelayTime::create(delayTime),Spawn::create(ScaleTo::create(0,0.6),MoveTo::create(0,Vec2(OldGang4Pos.x-cardSize.width*0.8,OldGang4Pos.y)),NULL),actionMove4,DelayTime::create(0.06),ScaleTo::create(0,0),NULL);
				Gang4TarAction=TargetedAction::create(Gang4Card,Gang4Card_seq);
			}
		}
		auto ming_gang_spawn=Spawn::create(l_action1,Gang2TarAction,Gang3TarAction,Gang4TarAction,l_action2,l_action3,l_action4,NULL);
		////////////////////////////////////////////////////////////////////////////////////

		int actionStartPlace=0;
		for(int a=0;a<=list->atcvie_place;a++)
			if(list->data[a].status==c_MING_KOU||list->data[a].status==c_FREE)
			{
				actionStartPlace=a;
				break;
			}
		Vector<FiniteTimeAction *>gang_list_seq;
		if(!_roundManager->_isCardFromOthers)
		{
			int startPlace;
			if(actionStartPlace>=gang2)
				startPlace=gang2;
			else
				startPlace=actionStartPlace;
			for(int i=startPlace;i<list->atcvie_place;i++)
			{
				if(i<gang2)
				{
					auto curPos=myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+no*20+i)->getPosition();
					auto actionMove=MoveTo::create(0.3,Vec2(curPos.x+cardPengSize.width*3.5,curPos.y));
					auto seq=Sequence::create(DelayTime::create(delayTime),actionMove,NULL);
					auto action=TargetedAction::create(myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+no*20+i),seq);
					gang_list_seq.insert(i-startPlace,action);
				}
				else if(i==gang2||i==gang3||i==gang4)
				{
					auto seq=Sequence::create(/*DelayTime::create(delayTime),*/ScaleTo::create(0,0),NULL);
					auto action=TargetedAction::create(myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+no*20+i),seq);
					gang_list_seq.insert(i-startPlace,action);
					if(i==gang4)
						break;
				}
			}
		}
		else 
		{
			int startPlace;
			if(actionStartPlace>=gang2)
				startPlace=gang2;
			else
				startPlace=actionStartPlace;
			for(int i=startPlace;i<list->len;i++)
			{
				if(list->atcvie_place>0)
				{
					if(i<gang2)
					{
						auto curPos=myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+no*20+i)->getPosition();
						auto actionMove=MoveTo::create(0.3,Vec2(curPos.x+cardPengSize.width*3.5,curPos.y));
						auto seq=Sequence::create(DelayTime::create(delayTime),actionMove,NULL);
						auto action=TargetedAction::create(myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+no*20+i),seq);
						gang_list_seq.insert(i-startPlace,action);
					}
					else if(i==gang2||i==gang3||i==gang4)
					{
						auto seq=Sequence::create(ScaleTo::create(0,0),NULL);
						auto action=TargetedAction::create(myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+no*20+i),seq);
						gang_list_seq.insert(i-startPlace,action);
					}
					else if(i>gang4)
					{
						auto curPos=myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+no*20+i)->getPosition();
						auto actionMove=MoveTo::create(0.3,Vec2(curPos.x+(cardPengSize.width*3.5-cardSize.width*(3+ifZeroPointTwo*0.2)),curPos.y));
						auto seq=Sequence::create(DelayTime::create(delayTime),actionMove,NULL);
						auto action=TargetedAction::create(myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+no*20+i),seq);
						gang_list_seq.insert(i-startPlace,action);
					}
				}
				else if(list->atcvie_place==0)
				{
					if(i<gang2)
					{
						auto curPos=myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+no*20+i)->getPosition();
						auto actionMove=MoveTo::create(0.3,Vec2(curPos.x+cardSize.width*3+(cardPengSize.width*4-cardSize.width*3),curPos.y));
						auto seq=Sequence::create(DelayTime::create(delayTime),actionMove,NULL);
						auto action=TargetedAction::create(myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+no*20+i),seq);
						gang_list_seq.insert(i-startPlace,action);
					}
					else if(i==gang2||i==gang3||i==gang4)
					{
						auto seq=Sequence::create(ScaleTo::create(0,0),NULL);
						auto action=TargetedAction::create(myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+no*20+i),seq);
						gang_list_seq.insert(i-startPlace,action);
					}
					else if(i>gang4)
					{
						auto curPos=myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+no*20+i)->getPosition();
						auto actionMove=MoveTo::create(0.3,Vec2(curPos.x+(cardPengSize.width*4-cardSize.width*(3+ifZeroPointTwo*0.2)),curPos.y));
						auto seq=Sequence::create(DelayTime::create(delayTime),actionMove,NULL);
						auto action=TargetedAction::create(myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+no*20+i),seq);
						gang_list_seq.insert(i-startPlace,action);
					}
				}
			}
		}

		for(int a=list->len-1;a>=0;a--)
		{
			int curTag=HAND_IN_CARDS_TAG_ID+1*20+a;
			if(!myframe->getChildByTag(curTag))
				continue;
			auto EveryCard=(Sprite*)myframe->getChildByTag(curTag);
			if(!_roundManager->_isCardFromOthers)
			{
				if(a==list->len-1)
					EveryCard->setTag(EFFECT_TEMP_CARD_TAG_FOUR);
				else if(a<list->len-1&&a>gang4)
					EveryCard->setTag(curTag+1);
				if(a==(gang4+1))
				{
					((Sprite*)myframe->getChildByTag(EFFECT_TEMP_CARD_TAG_FOUR))->setTag(HAND_IN_CARDS_TAG_ID+1*20+gang4+1);
				}
			}
			else
			{
				if(gang2<list->atcvie_place)
				{
					if(a>gang4)
						EveryCard->setTag(curTag+1);
					if(a==gang4+1)
					{
						auto EmptyCard=Sprite::createWithTexture(g_my_free->getTexture());
						EmptyCard->setAnchorPoint(Vec2(1,1));
						EmptyCard->setScale(0);
						EmptyCard->setPosition(Vec2::ZERO);
						myframe->addChild(EmptyCard,1,HAND_IN_CARDS_TAG_ID+1*20+gang4+1);
					}
				}
				else
				{

					if(a>gang4)
						EveryCard->setTag(curTag+1);
					else if(a<=gang4&&a>=gang2)
					{
						if(a==gang4)
							EveryCard->setTag(EFFECT_TEMP_CARD_TAG_THREE);
						if(a==gang3)
							EveryCard->setTag(EFFECT_TEMP_CARD_TAG_TWO);
						if(a==gang2)
							EveryCard->setTag(EFFECT_TEMP_CARD_TAG_ONE);
					}
					else if(a<gang2&&a>=list->atcvie_place)
						EveryCard->setTag(curTag+4);
					if(a==list->atcvie_place)
					{
						for(int b=0;b<3;b++)
							((Sprite*)myframe->getChildByTag(EFFECT_TEMP_CARD_TAG_ONE+b))->setTag(HAND_IN_CARDS_TAG_ID+1*20+list->atcvie_place+b);
						auto EmptyCard=Sprite::createWithTexture(g_my_free->getTexture());
						EmptyCard->setAnchorPoint(Vec2(1,1));
						EmptyCard->setScale(0);
						EmptyCard->setPosition(Vec2::ZERO);
						myframe->addChild(EmptyCard,1,HAND_IN_CARDS_TAG_ID+1*20+list->atcvie_place+3);
					}
				}
			}
		}

		auto lightCircle=Sprite::createWithSpriteFrameName("4.png");
		lightCircle->setPosition(Vec2(origin.x+visibleSize.width*0.5+cardPengSize.width*0.98,origin.y+visibleSize.height*0.315));
		myframe->addChild(lightCircle,32,IMG_4_EFFECT_TAG_ID);
		lightCircle->setScale(0);
		BlendFunc tmp_oBlendFunc ={GL_SRC_ALPHA, GL_ONE};
		lightCircle->setBlendFunc(tmp_oBlendFunc);
		auto lightCircle_seq=Sequence::create(DelayTime::create(0.78),ScaleTo::create(0,0.4),ScaleTo::create(0.18,1.0),Spawn::create(ScaleTo::create(0.06,1.2),FadeOut::create(0.06),NULL),NULL);
		auto light1_action=TargetedAction::create(lightCircle,lightCircle_seq);

		auto lightCircle1=Sprite::createWithSpriteFrameName("Q4.png");
		lightCircle1->setAnchorPoint(Vec2(0.5,0));
		lightCircle1->setPosition(Vec2(origin.x+visibleSize.width*0.5+cardPengSize.width*0.98,origin.y+visibleSize.height*0.315));
		myframe->addChild(lightCircle1,32,IMG_Q4_EFFECT_TAG_ID);
		lightCircle1->setScale(0);
		lightCircle1->setBlendFunc(tmp_oBlendFunc);
		auto lightCircle1_seq=Sequence::create(DelayTime::create(1.02),ScaleTo::create(0,0.1),ScaleTo::create(0.18,0.6),Spawn::create(ScaleTo::create(0.06,0.7),FadeOut::create(0.06),NULL),NULL);
		auto light2_action=TargetedAction::create(lightCircle1,lightCircle1_seq);

		auto lightCircle2=Sprite::createWithSpriteFrameName("Q4.png");
		lightCircle2->setAnchorPoint(Vec2(0.5,0));
		lightCircle2->setPosition(Vec2(origin.x+visibleSize.width*0.5+cardPengSize.width*0.98,origin.y+visibleSize.height*0.315));
		myframe->addChild(lightCircle2,32,IMG_Q4_EFFECT_TAG_ID+1);
		lightCircle2->setScale(0);
		lightCircle2->setBlendFunc(tmp_oBlendFunc);
		auto lightCircle2_seq=Sequence::create(DelayTime::create(1.26),ScaleTo::create(0,0.1),ScaleTo::create(0.18,0.4),Spawn::create(ScaleTo::create(0.06,0.5),FadeOut::create(0.06),NULL),NULL);
		auto light3_action=TargetedAction::create(lightCircle2,lightCircle2_seq);

		auto yellowlight=Sprite::createWithSpriteFrameName("c33.png");
		yellowlight->setPosition(Vec2(origin.x+visibleSize.width*0.5+cardPengSize.width*0.98,origin.y+visibleSize.height*0.315));
		myframe->addChild(yellowlight,30,IMG_C33_EFFECT_TAG_ID);
		yellowlight->setScale(0);
		auto yellowlight_seq=Sequence::create(DelayTime::create(0.78),ScaleTo::create(0.12,1),Spawn::create(ScaleTo::create(0.18,0),FadeOut::create(0.18),NULL),NULL);
		auto yellow_action=TargetedAction::create(yellowlight,yellowlight_seq);

		auto light=Sprite::createWithSpriteFrameName("c3.png");
		light->setPosition(Vec2(origin.x+visibleSize.width*0.5+cardPengSize.width*0.98,origin.y+visibleSize.height*0.315));
		light->setScale(0);
		myframe->addChild(light,31,IMG_C3_EFFECT_TAG_ID);
		light->setOpacity(200);
		auto light_seq=Sequence::create(DelayTime::create(0.78),ScaleTo::create(0.12,0.5),Spawn::create(ScaleTo::create(0.18,0),FadeOut::create(0.18),NULL),NULL);
		auto light_action=TargetedAction::create(light,light_seq);

		auto g_spa0=Spawn::create(gang_list_seq);
		auto g_spa1=Spawn::create(ming_gang_spawn,Sequence::create(DelayTime::create(0.78),CCCallFunc::create([=]()
		{
			startParticleSystem(0.3);
		}),NULL),light1_action,light2_action,light3_action,yellow_action,light_action,NULL);
		//g_seq1 **clear	
		//simple_seq **moji
		//g_out_card_action **杠的那张牌
		//g_spa0 **手上的三张牌
		//g_spa1  **碰的三张牌
		//update_list_seq  **最后的清理
		Spawn *simple_seq=simple_tip_effect(getEffectVec(_roundManager->_curPlayer),"gang.png");///墨迹等。。。update_list_seq 最后处理，转换cur_player
		//auto GoldAccount=CallFunc::create([=](){
		//	GoldNumInsert(no,2,_roundManager->_curPlayer);	
		//});
		//auto minggang_seq=Sequence::create(g_seq1,simple_seq,g_out_card_action,g_spa0,g_spa1,update_list_seq,NULL);
		auto no1_seq1_Delay=Sequence::create(DelayTime::create(0.42),g_out_card_action,NULL);
		auto VoiceEffect=CallFunc::create([=](){SimpleAudioEngine::sharedEngine()->playEffect("Music/paizhuangji.ogg");});
		auto DelayVoice=Sequence::create(DelayTime::create(0.78),VoiceEffect,NULL);
		auto MingGangEffectNode=Node::create();
		myframe->addChild(MingGangEffectNode,1,MING_GANG_EFFECT_NODE);

		auto callFunc1=CCCallFunc::create(this,callfunc_selector(NetRaceLayer::delete_ActionEffect));
		//auto callFunc1=CCCallFunc::create(this,callfunc_selector(NetRaceLayer::delete_act_tip));
		auto minggang_action=CCCallFuncN::create(this,callfuncN_selector(NetRaceLayer::minggang_dispatch));
		auto callFunc_update_list=CCCallFunc::create([=](){
			if(ifEffectTime)
			{
				ifEffectTime=false;
				if(ifUpdateDuringEffect)
				{
					ifUpdateDuringEffect=false;
					_roundManager->_curEffectCardKind=ck_NOT_DEFINED;
					_roundManager->_curEffectCardStatus=c_NOT_DEFINDED;
					card_list_update(no);
				}
				else 
				{
					int sameCardNum=0;
					for(int a=list->atcvie_place-1;a>=0;a--)
					{
						if(list->data[a].kind==_roundManager->_curEffectCardKind&&list->data[a].status==_roundManager->_curEffectCardStatus)
						{
							sameCardNum++;
							((Sprite*)myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+1*20+a))->setVisible(true);
							if(sameCardNum==4)
							{
								_roundManager->_curEffectCardKind=ck_NOT_DEFINED;
								_roundManager->_curEffectCardStatus=c_NOT_DEFINDED;
								break;
							}
						}
					}
				}
			}
		});
		CallFunc* dis_action;
		if(!_roundManager->_isCardFromOthers)
		{
			_roundManager->_qiangGangTargetNo=no;
			dis_action=CCCallFunc::create(this,callfunc_selector(NetRaceLayer::QiangGangHuJudge));
		}
		else
			dis_action=CCCallFunc::create(this,callfunc_selector(NetRaceLayer::call_distribute_card));
		auto update_list_seq=Sequence::create(callFunc1,/*minggang_action,*/callFunc_update_list,CallFunc::create([=](){
			if(myframe->getChildByTag(MING_GANG_EFFECT_NODE))
				myframe->removeChildByTag(MING_GANG_EFFECT_NODE,true);
		}),/*CCCallFunc::create([=]()
		{
			_roundManager->_curPlayer=no;
		}),dis_action,*/NULL);//dis_action

		auto minggang_seq=Sequence::create(g_seq1,Spawn::create(simple_seq,GangVoice,g_out_card_action,g_spa0,g_spa1,DelayVoice,NULL),/*GoldAccount,*/update_list_seq,NULL);
		MingGangEffectNode->_ID=1;
		MingGangEffectNode->runAction(minggang_seq);

		myframe->runAction(Sequence::create(minggang_action,DelayTime::create(0.48),CallFunc::create([=](){
			if(_roundManager->_isCardFromOthers)
				GoldNumInsert(no,2,_roundManager->_curPlayer);	
		}),/*CallFunc::create([=](){card_list_update(1);}),*/CCCallFunc::create([=](){_roundManager->_curPlayer=no;}),dis_action,NULL));
	}
}
void NetRaceLayer::qi_tip_effect(Node *psender)
{
    LOGGER_WRITE("%s",__FUNCTION__);

	int no=psender->_ID;
	auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
	myframe->_ID=1;
	if(_roundManager->_lastAction==a_JUMP)
		continue_gang_times=0;
	_roundManager->_lastAction=a_JUMP;
	_roundManager->_actionToDo=a_JUMP;
	if(_roundManager->_isWaitDecision)
	{
		_roundManager->_isWaitDecision=false;
		//_roundManager->_actionToDo=_roundManager->_tempActionToDo;
		_roundManager->_tempActionToDo=a_JUMP;
	}
	auto shade_act=(Sprite*)myframe->getChildByTag(QI_REMIND_ACT_BKG_TAG_ID);
	auto fadeOut=FadeOut::create(0.3);
	auto easeBounce=ScaleTo::create(0.3,1.3);
	auto spawn=Spawn::create(fadeOut,easeBounce,NULL);
	auto seq=Sequence::create(ScaleTo::create(0,1),spawn,NULL);
	auto shadeAction=TargetedAction::create(shade_act,seq);
	auto DeleteActTip=CCCallFunc::create(this,callfunc_selector(NetRaceLayer::delete_ActionRemind));
	auto shadeFunc=Sequence::create(shadeAction,DeleteActTip,NULL);
	if(myframe->getChildByTag(QI_REMIND_ACT_BKG_TAG_ID)!=NULL&&no==1)
	{
		if(!_roundManager->_isCardFromOthers)
		{
			if(_roundManager->_isGangAsking)
			{
				auto showCardFunc=CallFunc::create([=](){
					_roundManager->_isGangAsking = false;
					auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
					int last_one=_roundManager->_players[_roundManager->_curPlayer]->get_parter()->get_card_list()->len-1;
					Vec2 location=myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+1*20+last_one)->getPosition();
					if(myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+1*20+last_one))
						myframe->removeChildByTag(HAND_IN_CARDS_TAG_ID+1*20+last_one);

                    _roundManager->RecordOutCard(_roundManager->_players[_roundManager->_curPlayer]->get_parter()->get_card_list()->data[last_one]);

					_roundManager->_lastHandedOutCard=_roundManager->_players[_roundManager->_curPlayer]->get_parter()->hand_out(last_one);
					update_outcard(myframe,location,2);
				});
				myframe->runAction(Sequence::create(shadeFunc,showCardFunc,NULL));

			}
			else
				myframe->runAction(shadeFunc);
		}
		else
		{
			if(_roundManager->_isQiangGangAsking)//抢杠胡请求
			{
				_roundManager->_isQiangGangAsking=false;
				auto GoldAccount=CallFunc::create([=](){
					GoldNumInsert(_roundManager->_qiangGangTargetNo,2,_roundManager->_curPlayer);
					_roundManager->_qiangGangTargetNo=-1;
				});
				auto DistributeFunc=CCCallFunc::create(this,callfunc_selector(NetRaceLayer::call_distribute_card));
				myframe->runAction(Sequence::create(shadeFunc,Spawn::create(GoldAccount,DistributeFunc,NULL),NULL));
			}
			else if(_roundManager->_isDoubleHuAsking)//双响请求
			{
				_roundManager->_isDoubleHuAsking=false;
				auto huFunc=CallFunc::create([=](){hu_effect_tip(_roundManager->_otherOneForDouble);});
				myframe->runAction(Sequence::create(shadeFunc,huFunc,NULL));
			}
			else
			{
				_roundManager->_curPlayer=(_roundManager->_curPlayer+1)%3;
				auto DistributeFunc=CCCallFunc::create(this,callfunc_selector(NetRaceLayer::call_distribute_card));
				myframe->runAction(Sequence::create(shadeFunc,DistributeFunc,NULL));
			}
		}
	}
}
void NetRaceLayer::update_residue_TingCards(int no)
{
    LOGGER_WRITE("%s",__FUNCTION__);

    _roundManager->_players[no]->get_parter()->get_ming_reserved_cards_num(_roundManager->_river);
    
	//int huTiemsForEveryOne[MAX_HANDIN_NUM][9];//番型--
	int hu_residueForEvery[MAX_HANDIN_NUM][9];//剩余牌数
	int hu_NumForEveryCard[MAX_HANDIN_NUM];//胡张数
	//CARD_KIND hu_cards[MAX_HANDIN_NUM][9];//胡哪几张牌
	_roundManager->_players[no]->get_parter()->get_hu_NumForEveryCard(hu_NumForEveryCard);//张数
	_roundManager->_players[no]->get_parter()->get_hu_residueForEvery(hu_residueForEvery);//剩余牌数
	//_roundManager->_players[no]->get_parter()->get_huTiemsForEveryOne(huTiemsForEveryOne);//番型
	//_roundManager->_players[no]->get_parter()->get_hu_cards(hu_cards);//胡哪几张牌


	int cardNum=hu_NumForEveryCard[Hu_cardOut_place];

	auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
	for(int k=0;k<cardNum;k++)
	{
		Sprite* curCardBar;
		switch (no)
		{
		case 0:
			curCardBar=(Sprite*)myframe->getChildByTag(TING_SING_LEFTBAR)->getChildByTag(k+3);
			break;
		case 1:
			curCardBar=(Sprite*)myframe->getChildByTag(TING_SING_BAR)->getChildByTag(k+3);
			break;
		case 2:
			curCardBar=(Sprite*)myframe->getChildByTag(TING_SING_RIGHTBAR)->getChildByTag(k+3);
			break;
		default:
			break;
		}
		char tempForChar[10];
		std::string tempForStr;
		
		curCardBar->removeChildByTag(2);

		if(hu_residueForEvery[Hu_cardOut_place][k]==0)
		{
			sprintf(tempForChar,"%d",hu_residueForEvery[Hu_cardOut_place][k]);
			tempForStr=tempForChar;
			auto residueNum=LabelAtlas::create(tempForStr,"fonts/NumMingNotice_grey.png",17, 20, '0');
			residueNum->setAnchorPoint(Vec2(0.5,0.5));
			residueNum->setPosition(Vec2(78,24));
			curCardBar->addChild(residueNum,1,2);
		}
		else
		{
			sprintf(tempForChar,"%d",hu_residueForEvery[Hu_cardOut_place][k]);
			tempForStr=tempForChar;
			auto residueNum=LabelAtlas::create(tempForStr,"fonts/NumMingNotice.png",17, 20, '0');
			residueNum->setAnchorPoint(Vec2(0.5,0.5));
			residueNum->setPosition(Vec2(78,24));
			curCardBar->addChild(residueNum,1,2);
		}
		//if(hu_residueForEvery[Hu_cardOut_place][k]==0)
		//	residueNum->runAction(TintTo::create(0,255,47,47));
	}
}
void NetRaceLayer::OtherTingHintBar(int curNo,int CardPlace)
{
    LOGGER_WRITE("%s",__FUNCTION__);

	Vec2 curPos;
	if(curNo==0)
		curPos=Vec2(origin.x+visibleSize.width*266/1218,origin.y+visibleSize.height*0.65);//0.69 0.23
	else
		curPos=Vec2(origin.x+visibleSize.width*952/1218,origin.y+visibleSize.height*0.62);//0.77 0.66
	auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);

	Hu_cardOut_place=CardPlace;
	//int huTiemsForEveryOne[MAX_HANDIN_NUM][9];//番型--
	//int hu_residueForEvery[MAX_HANDIN_NUM][9];//剩余牌数
	int hu_NumForEveryCard[MAX_HANDIN_NUM];//胡张数
	CARD_KIND hu_cards[MAX_HANDIN_NUM][9];//胡哪几张牌
	_roundManager->_players[curNo]->get_parter()->get_hu_NumForEveryCard(hu_NumForEveryCard);//张数
	//_roundManager->_players[curNo]->get_parter()->get_hu_residueForEvery(hu_residueForEvery);//剩余牌数
	//_roundManager->_players[curNo]->get_parter()->get_huTiemsForEveryOne(huTiemsForEveryOne);//番型
	_roundManager->_players[curNo]->get_parter()->get_hu_cards(hu_cards);//胡哪几张牌

	int cardNum=hu_NumForEveryCard[CardPlace];

	auto tishiLeft=Sprite::createWithSpriteFrameName("Leftcorner-other.png");
	auto LeftSize=tishiLeft->getTextureRect().size;
	auto tishiCard=Sprite::createWithSpriteFrameName("Husign-other.png");
	auto CardSize=tishiCard->getTextureRect().size;
	auto tishiEnd=Sprite::createWithSpriteFrameName("Rightangle-other.png");
	auto RightSize=tishiEnd->getTextureRect().size;

	Size SignBarSize=Size(LeftSize.width,LeftSize.height+CardSize.height*(cardNum+1)+RightSize.height);
	
	float BarHeight=SignBarSize.height;
	float y=curPos.y;
	float curScale=1;
	if((y+SignBarSize.height/2)>(origin.y+visibleSize.height-10))
		curScale=(origin.y+visibleSize.height-10-y)/(SignBarSize.height/2);

	auto TingSignBar=LayerColor::create();
	TingSignBar->setAnchorPoint(Vec2(0.5,0.5));
	TingSignBar->setContentSize(SignBarSize);
	TingSignBar->ignoreAnchorPointForPosition(false);
	TingSignBar->setPosition(Vec2(curPos.x,curPos.y));
	myframe->addChild(TingSignBar,30,TING_SING_LEFTBAR+curNo/2);

	if(curNo==0)
	{
		auto SignLeft=Sprite::createWithSpriteFrameName("Rightangle-other.png");
		SignLeft->setAnchorPoint(Vec2(0,1));
		SignLeft->setPosition(Vec2(0,SignBarSize.height));
		TingSignBar->addChild(SignLeft,1,1);

		auto SignHuBKG=Sprite::createWithSpriteFrameName("Husign-other.png");
		SignHuBKG->setAnchorPoint(Vec2(0,1));
		SignHuBKG->setPosition(Vec2(0,SignBarSize.height-LeftSize.height));
		auto SignHuFont=Sprite::create("smallHu.png");
		SignHuFont->setRotation(90);
		SignHuFont->setScale(0.7);
		SignHuFont->setAnchorPoint(Vec2(0.5,0.5));
		SignHuFont->setPosition(Vec2(CardSize.width/2,CardSize.height/2));
		SignHuBKG->addChild(SignHuFont,1);
		TingSignBar->addChild(SignHuBKG,1,2);

		int k=0;
		for(k=0;k<cardNum;k++)
		{
			auto SingForCards=Sprite::createWithSpriteFrameName("Husign-other.png");
			SingForCards->setAnchorPoint(Vec2(0,1));
			SingForCards->setPosition(Vec2(0,SignBarSize.height-LeftSize.height-CardSize.height*(k+1)));

			auto WinCard=Sprite::createWithTexture(g_left_right_peng_out->getTexture());
			WinCard->setAnchorPoint(Vec2(0.5,0.5));
			WinCard->setPosition(Vec2(CardSize.width/2,CardSize.height/2));//  /2  0.65
			auto s_card=Sprite::createWithTexture(g_small_card_kind[hu_cards[CardPlace][k]]->getTexture());
			s_card->setRotation(90);
			s_card->setScale(0.9);
			s_card->setAnchorPoint(Vec2(0.5,0.5));
			s_card->setPosition(Vec2(WinCard->getTextureRect().size.width/2,WinCard->getTextureRect().size.height*0.65));
			WinCard->addChild(s_card,1);
			SingForCards->addChild(WinCard,1);
			TingSignBar->addChild(SingForCards,1,k+3);
		}

		auto SignRight=Sprite::createWithSpriteFrameName("Leftcorner-other.png");
		SignRight->setAnchorPoint(Vec2(0,1));
		SignRight->setPosition(Vec2(0,SignBarSize.height-LeftSize.height-CardSize.height*(k+1)));
		TingSignBar->addChild(SignRight,1,k+3);
	}
	else if(curNo==2)
	{
		auto SignLeft=Sprite::createWithSpriteFrameName("Leftcorner-other.png");
		SignLeft->setAnchorPoint(Vec2(0,0));
		SignLeft->setPosition(Vec2::ZERO);
		TingSignBar->addChild(SignLeft,1,1);

		auto SignHuBKG=Sprite::createWithSpriteFrameName("Husign-other.png");
		SignHuBKG->setAnchorPoint(Vec2(0,0));
		SignHuBKG->setPosition(Vec2(0,LeftSize.height));
		auto SignHuFont=Sprite::create("smallHu.png");
		SignHuFont->setRotation(-90);
		SignHuFont->setScale(0.7);
		SignHuFont->setAnchorPoint(Vec2(0.5,0.5));
		SignHuFont->setPosition(Vec2(CardSize.width/2,CardSize.height/2));
		SignHuBKG->addChild(SignHuFont,1);
		TingSignBar->addChild(SignHuBKG,1,2);

		int k=0;
		for(k=0;k<cardNum;k++)
		{
			auto SingForCards=Sprite::createWithSpriteFrameName("Husign-other.png");
			SingForCards->setAnchorPoint(Vec2(0,0));
			SingForCards->setPosition(Vec2(0,LeftSize.height+CardSize.height*(k+1)));

			auto WinCard=Sprite::createWithTexture(g_left_right_peng_out->getTexture());
			WinCard->setAnchorPoint(Vec2(0.5,0.5));
			WinCard->setPosition(Vec2(CardSize.width/2,CardSize.height/2));//  /2  0.65
			auto s_card=Sprite::createWithTexture(g_small_card_kind[hu_cards[CardPlace][k]]->getTexture());
			s_card->setRotation(-90);
			s_card->setScale(0.9);
			s_card->setAnchorPoint(Vec2(0.5,0.5));
			s_card->setPosition(Vec2(WinCard->getTextureRect().size.width/2,WinCard->getTextureRect().size.height*0.65));
			WinCard->addChild(s_card,1);
			SingForCards->addChild(WinCard,1);
			TingSignBar->addChild(SingForCards,1,k+3);
		}

		auto SignRight=Sprite::createWithSpriteFrameName("Rightangle-other.png");
		SignRight->setAnchorPoint(Vec2(0,0));
		SignRight->setPosition(Vec2(0,LeftSize.height+CardSize.height*(k+1)));
		TingSignBar->addChild(SignRight,1,k+3);
	}
}
void NetRaceLayer::tingHintCreate(Point curPos,int CardPlace)
{
    LOGGER_WRITE("%s",__FUNCTION__);

	auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
    
	_roundManager->_players[1]->get_parter()->get_ming_reserved_cards_num(_roundManager->_river);
	Hu_cardOut_place=CardPlace;
	int huTiemsForEveryOne[MAX_HANDIN_NUM][9];//番型--
	int hu_residueForEvery[MAX_HANDIN_NUM][9];//剩余牌数
	int hu_NumForEveryCard[MAX_HANDIN_NUM];//胡张数
	CARD_KIND hu_cards[MAX_HANDIN_NUM][9];//胡哪几张牌
	_roundManager->_players[1]->get_parter()->get_hu_NumForEveryCard(hu_NumForEveryCard);//张数
	_roundManager->_players[1]->get_parter()->get_hu_residueForEvery(hu_residueForEvery);//剩余牌数
	_roundManager->_players[1]->get_parter()->get_huTiemsForEveryOne(huTiemsForEveryOne);//番型
	_roundManager->_players[1]->get_parter()->get_hu_cards(hu_cards);//胡哪几张牌

	int cardNum=hu_NumForEveryCard[CardPlace];

	auto tishiLeft=Sprite::createWithSpriteFrameName("Left corner.png");
	auto tishiHu=Sprite::createWithSpriteFrameName("Hu sign.png");
	auto tishiCard=Sprite::createWithSpriteFrameName("Card and Zhang.png");
	auto tishiEnd=Sprite::createWithSpriteFrameName("Right angle.png");

	Size SignBarSize=Size(tishiLeft->getTextureRect().size.width+tishiHu->getTextureRect().size.width+tishiCard->getTextureRect().size.width*cardNum+tishiEnd->getTextureRect().size.width,tishiLeft->getTextureRect().size.height);
	float BarWith=SignBarSize.width;
	float x=curPos.x;
	Vec2 PositionAdjust=Vec2::ZERO;
	Vec2 PointAdjust=Vec2::ZERO;
	if((x-BarWith/2)>=10&&(x+BarWith/2)<=(visibleSize.width-10))
	{
		PointAdjust=Vec2(0.5,0.5);
		PositionAdjust=curPos;
	}
	else if((x-BarWith/2)<10)
	{
		PointAdjust=Vec2((x-10)/BarWith,0.5);
		PositionAdjust=curPos;
	}
	else if((x+BarWith/2)>(visibleSize.width-10))
	{
		PointAdjust=Vec2(1-(visibleSize.width-10-x)/BarWith,0.5);
		PositionAdjust=curPos;
	}

	auto TingSignBar=LayerColor::create();
	TingSignBar->setAnchorPoint(PointAdjust);
	TingSignBar->setContentSize(SignBarSize);
	TingSignBar->ignoreAnchorPointForPosition(false);
	TingSignBar->setPosition(Vec2(PositionAdjust.x,origin.y+visibleSize.height*0.3));
	myframe->addChild(TingSignBar,30,TING_SING_BAR);

	auto tipPoint=Sprite::createWithSpriteFrameName("Small triangle.png");
	tipPoint->setAnchorPoint(Vec2(0.5,1));
	//tipPoint->getTexture()->setAliasTexParameters();
	tipPoint->setPosition(Vec2(PointAdjust.x*TingSignBar->getContentSize().width,0.5));
	TingSignBar->addChild(tipPoint,1);

	auto SignHead=Sprite::createWithSpriteFrameName("Left corner.png");//左
	//SignHead->getTexture()->setAliasTexParameters();
	SignHead->setAnchorPoint(Vec2(0,0));
	SignHead->setPosition(Vec2::ZERO);
	TingSignBar->addChild(SignHead,1,1);

	auto SignHuBKG=Sprite::createWithSpriteFrameName("Hu sign.png");//胡
	SignHuBKG->setAnchorPoint(Vec2(0,0));
	SignHuBKG->setPosition(Vec2(SignHead->getTextureRect().size.width,0));
	auto SignHuFont=Sprite::create("smallHu.png");
	SignHuFont->setScale(0.8);
	SignHuFont->setAnchorPoint(Vec2(0.5,0.5));
	SignHuFont->setPosition(Vec2(origin.x+SignHuBKG->getTextureRect().size.width/2,origin.y+SignHuBKG->getTextureRect().size.height/2));
	SignHuBKG->addChild(SignHuFont,1);
	TingSignBar->addChild(SignHuBKG,1,2);

	int k;
	for(k=0;k<cardNum;k++)//牌
	{
		auto SignForCards=Sprite::createWithSpriteFrameName("Card and Zhang.png");
		//SignForCards->getTexture()->setAliasTexParameters();
		SignForCards->setAnchorPoint(Vec2(0,0));
		SignForCards->setPosition(Vec2(SignHead->getTextureRect().size.width+SignHuBKG->getTextureRect().size.width+SignForCards->getTextureRect().size.width*k,0));

		auto WinCard=Sprite::createWithTexture(g_my_peng->getTexture());
		WinCard->setAnchorPoint(Vec2(0.5,0.5));
		WinCard->setPosition(Vec2(10+WinCard->getTextureRect().size.width/2,43));
		auto s_card=Sprite::createWithTexture(g_mid_card_kind[hu_cards[CardPlace][k]]->getTexture());
		s_card->setAnchorPoint(Vec2(0.5,0.5));
		s_card->setPosition(WinCard->getTextureRect().size.width/2,WinCard->getTextureRect().size.height*0.6);
		WinCard->addChild(s_card,1);
		SignForCards->addChild(WinCard,1);

		char tempForChar[10];
		std::string tempForStr;

		sprintf(tempForChar,"");
		sprintf(tempForChar,"%d",huTiemsForEveryOne[CardPlace][k]);
		tempForStr=tempForChar;
		auto timesNum=LabelAtlas::create(tempForStr,"fonts/NumMingNotice.png",17, 20, '0');
		timesNum->setAnchorPoint(Vec2(0.5,0.5));
		timesNum->setPosition(Vec2(78,62));
		SignForCards->addChild(timesNum,1,1);

		auto fanFont=Sprite::createWithSpriteFrameName("fan.png");
		fanFont->setAnchorPoint(Vec2(0.5,0.5));
		fanFont->setPosition(Vec2(108,62));
		SignForCards->addChild(fanFont,1);

		sprintf(tempForChar,"");
		sprintf(tempForChar,"%d",hu_residueForEvery[CardPlace][k]);
		tempForStr=tempForChar;
		auto residueNum=LabelAtlas::create(tempForStr,"fonts/NumMingNotice.png",17,20, '0');
		residueNum->setAnchorPoint(Vec2(0.5,0.5));
		residueNum->setPosition(Vec2(78,24));
		SignForCards->addChild(residueNum,1,2);

		auto zhangFont=Sprite::createWithSpriteFrameName("zhang.png");
		zhangFont->setAnchorPoint(Vec2(0.5,0.5));
		zhangFont->setPosition(Vec2(108,24));
		SignForCards->addChild(zhangFont,1);

		TingSignBar->addChild(SignForCards,1,3+k);
	}
	auto SignEnd=Sprite::createWithSpriteFrameName("Right angle.png");
	//SignEnd->getTexture()->setAliasTexParameters();
	SignEnd->setAnchorPoint(Vec2(1,0));
	SignEnd->setPosition(Vec2(TingSignBar->getContentSize().width,0));
	TingSignBar->addChild(SignEnd,1,k+3);
}

void NetRaceLayer::card_list_update(int no)
{
	float x,y;
	Sprite *p_list[MAX_HANDIN_NUM];
	auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
	CARD_ARRAY *list=_roundManager->_players[no]->get_parter()->get_card_list();
	for(int ik=0;ik<MAX_HANDIN_NUM;ik++)
	{
		if(myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+no*20+ik))
			myframe->removeChildByTag(HAND_IN_CARDS_TAG_ID+no*20+ik,true);
	}
	x=base_point[no].x+10; 
	y=base_point[no].y+10;
	int residualCardsNum=(list->len-list->atcvie_place)%3;
	unsigned char ting_flag=_roundManager->_players[no]->get_parter()->get_ting_status();
    LOGGER_WRITE("NETWORK : %s : %d(Ting %d)",__FUNCTION__,no,ting_flag);
    
	if( ting_flag==1 && _roundManager->_firstMingNo==-1 )
		_roundManager->_firstMingNo=no;

    for(int i=0;i<list->len;i++)
	{
		if(list->data[i].kind!=ck_NOT_DEFINED )
		{
			if(no==0)
			{
				if(list->data[i].status==c_FREE)
				{
					if(ting_flag==1)
					{
						p_list[i]=Sprite::createWithTexture(g_left_right_peng_out->getTexture());
						p_list[i]->setAnchorPoint(Vec2(0.3f,1.0f));
						p_list[i]->setPosition(Vec2(x-10,y-8));
					}
					else if(ting_flag!=1&&_roundManager->_players[1]->get_parter()->get_ting_status()==1)
					{
						p_list[i]=Sprite::createWithTexture(g_left_right_angang->getTexture());
						p_list[i]->setAnchorPoint(Vec2(0.3f,1.0f));
						p_list[i]->setPosition(Vec2(x-10,y-8));
					}
					else
					{
						p_list[i]=Sprite::createWithTexture(g_left_free->getTexture());
						p_list[i]->setAnchorPoint(Vec2(0.0f,1.0f));
						p_list[i]->setPosition(Vec2(x-10,y-8));
					}
				}
				else if(list->data[i].status==c_AN_GANG)
				{
					if(list->data[i].kind==list->data[i+1].kind&&list->data[i].kind!=list->data[i+2].kind&&ting_flag!=1&&_roundManager->_players[1]->get_parter()->get_ting_status()==1)
						p_list[i]=Sprite::createWithTexture(g_left_right_peng_out->getTexture());
					else
						p_list[i]=Sprite::createWithTexture(g_left_right_angang->getTexture());
					p_list[i]->setAnchorPoint(Vec2(0.3f,1.0f));
					p_list[i]->setPosition(Vec2(x-10,y-8));
				}
				else if(list->data[i].status==c_MING_KOU)
				{
					p_list[i]=Sprite::createWithTexture(g_left_right_angang->getTexture());
					p_list[i]->setAnchorPoint(Vec2(0.3f,1.0f));
					p_list[i]->setPosition(Vec2(x-10,y-8));
				}
				else if(list->data[i].status==c_PENG||list->data[i].status==c_MING_GANG )
				{
					p_list[i]=Sprite::createWithTexture(g_left_right_peng_out->getTexture());
					p_list[i]->setAnchorPoint(Vec2(0.3f,1.0f));
					p_list[i]->setPosition(Vec2(x-10,y-8));
				}
				if(list->data[i].status==c_FREE)
				{
					if(ting_flag==1||_roundManager->_players[1]->get_parter()->get_ting_status()==1)
					{
						Sprite* s_card;
						if(ting_flag==1)
							s_card=Sprite::createWithTexture(g_small_card_kind[list->data[i].kind]->getTexture());
						else if(ting_flag!=1&&_roundManager->_players[1]->get_parter()->get_ting_status()==1)
							s_card=Sprite::createWithTexture(g_small_cardKind_black[list->data[i].kind]->getTexture());
							//s_card=Sprite::create("tileImage/tile_Upblack_14.png");
						s_card->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height*0.65));
						s_card->setRotation(90);
						s_card->setScale(0.9);
						p_list[i]->addChild(s_card);
						y-=((p_list[i]->getTextureRect().size.height)*0.65);
					}
					else
						y-=((p_list[i]->getTextureRect().size.height)*0.5);
				}
				else if(list->data[i].status==c_MING_KOU)
					y-=((p_list[i]->getTextureRect().size.height)*0.65);
				else if(list->data[i].status==c_PENG)
				{
					auto s_card=Sprite::createWithTexture(g_small_card_kind[list->data[i].kind]->getTexture());
					s_card->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height*0.65));
					s_card->setRotation(90);
					s_card->setScale(0.9);
					p_list[i]->addChild(s_card);
					if(_roundManager->_players[1]->get_parter()->get_ting_status()==1||ting_flag==1)
					{
						if(list->data[i+1].status==c_FREE||list->data[i+1].status==c_MING_KOU)
							y-=((p_list[i]->getTextureRect().size.height)*1);
						else if(list->data[i+1].kind!=list->data[i].kind)
							y-=((p_list[i]->getTextureRect().size.height)*0.65+5);
						else 
							y-=((p_list[i]->getTextureRect().size.height)*0.65);
					}
					else
					{
						if(list->data[i+1].status==c_FREE)
							y-=((p_list[i]->getTextureRect().size.height)*0.4);
						else if(list->data[i+1].kind!=list->data[i].kind)
							y-=((p_list[i]->getTextureRect().size.height)*0.65+5);
						else
							y-=((p_list[i]->getTextureRect().size.height)*0.65);
					}
				}
				else if(list->data[i].status==c_MING_GANG)
				{
					auto s_card=Sprite::createWithTexture(g_small_card_kind[list->data[i].kind]->getTexture());
					s_card->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height*0.65));
					s_card->setRotation(90);
					s_card->setScale(0.9);
					p_list[i]->addChild(s_card);
					if((list->data[i].kind==list->data[i+1].kind)&&(list->data[i].kind==list->data[i+2].kind)&&(list->data[i].kind==list->data[i+3].kind)&&(list->data[i].kind!=list->data[i+4].kind))//1
						y-=((p_list[i]->getTextureRect().size.height)*0.65);
					else if((list->data[i].kind==list->data[i+1].kind)&&(list->data[i].kind==list->data[i+2].kind)&&(list->data[i].kind!=list->data[i+3].kind))//2
						y+=13;
					else if(list->data[i].kind==list->data[i+1].kind&&list->data[i].kind!=list->data[i+2].kind)//3
						y-=(((p_list[i]->getTextureRect().size.height)*0.65)+13);
					else if(list->data[i].kind!=list->data[i+1].kind)//4
					{
						if(_roundManager->_players[1]->get_parter()->get_ting_status()==1||ting_flag==1)
						{
							if(list->data[i+1].status==c_FREE||list->data[i+1].status==c_MING_KOU)
								y-=((p_list[i]->getTextureRect().size.height)*1);
							else /*if(list->data[i+1].kind!=list->data[i].kind)*/
								y-=((p_list[i]->getTextureRect().size.height)*0.65+5);
							//else
							//	y-=((p_list[i]->getTextureRect().size.height)*0.65);
						}
						else
						{
							if(list->data[i+1].status==c_FREE)
								y-=((p_list[i]->getTextureRect().size.height)*0.4);
							else /*if(list->data[i+1].kind!=list->data[i].kind)*/
								y-=((p_list[i]->getTextureRect().size.height)*0.65+5);
							//else
							//	y-=((p_list[i]->getTextureRect().size.height)*0.65);
						}
					}
				}
				else if(list->data[i].status==c_AN_GANG)
				{
					if((list->data[i].kind==list->data[i+1].kind)&&(list->data[i].kind==list->data[i+2].kind)&&(list->data[i].kind==list->data[i+3].kind)&&(list->data[i].kind!=list->data[i+4].kind))//1
						y-=((p_list[i]->getTextureRect().size.height)*0.65);
					else if((list->data[i].kind==list->data[i+1].kind)&&(list->data[i].kind==list->data[i+2].kind)&&(list->data[i].kind!=list->data[i+3].kind))//2
						y+=13;
					else if(list->data[i].kind==list->data[i+1].kind&&list->data[i].kind!=list->data[i+2].kind)//3
					{
						if(ting_flag!=1&&_roundManager->_players[1]->get_parter()->get_ting_status()==1)
						{
							Sprite* s_card;
							if(ting_flag==1)
								s_card=Sprite::createWithTexture(g_small_card_kind[list->data[i].kind]->getTexture());
							else if(ting_flag!=1&&_roundManager->_players[1]->get_parter()->get_ting_status()==1)
								s_card=Sprite::createWithTexture(g_small_cardKind_black[list->data[i].kind]->getTexture());
							s_card->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height*0.65));
							s_card->setRotation(90);
							s_card->setScale(0.9);
							p_list[i]->addChild(s_card);
						}
						y-=(((p_list[i]->getTextureRect().size.height)*0.65)+13);
					}
					else if(list->data[i].kind!=list->data[i+1].kind)//4
					{
						if(_roundManager->_players[1]->get_parter()->get_ting_status()==1||ting_flag==1)
						{
							if(list->data[i+1].status==c_FREE||list->data[i+1].status==c_MING_KOU)
								y-=((p_list[i]->getTextureRect().size.height)*1);
							else
								y-=((p_list[i]->getTextureRect().size.height)*0.65+5);
						}
						else
						{
							if(list->data[i+1].status==c_FREE)
								y-=((p_list[i]->getTextureRect().size.height)*0.4);
							else
								y-=((p_list[i]->getTextureRect().size.height)*0.65+5);
						}
					}
				}
			}
			else if(no==1)
			{
				if(list->data[i].status==c_FREE||list->data[i].status==c_MING_KOU)
				{
					if(ting_flag==1)
						p_list[i]=Sprite::createWithTexture(g_my_ming_ting->getTexture());
					else
						p_list[i]=Sprite::createWithTexture(g_my_free->getTexture());
				}
				else if(list->data[i].status==c_PENG||list->data[i].status==c_MING_GANG)
				{
					p_list[i]=Sprite::createWithTexture(g_my_peng->getTexture());
				}
				else if(list->data[i].status==c_AN_GANG)
				{
					if(list->data[i].kind==list->data[i+1].kind && list->data[i].kind!=list->data[i+2].kind)
						p_list[i]=Sprite::createWithTexture(g_my_peng->getTexture());
					else
						p_list[i]=Sprite::createWithTexture(g_my_angang->getTexture());
				}
				p_list[i]->setAnchorPoint(Vec2(0,0));
				p_list[i]->setPosition(Vec2(x,y));
				if(MyCardChoosedNum==i)
				{
					p_list[i]->setScale(1.2);
					p_list[i]->setPosition(Vec2(x,y+10));
					x+=p_list[i]->getTextureRect().size.width*0.2;
				}
				if(residualCardsNum==2&&i==list->len-2)
					x+=30;
				if(list->data[i].kind==_roundManager->_curEffectCardKind&&list->data[i].status==_roundManager->_curEffectCardStatus)
					p_list[i]->setVisible(false);
				if(list->data[i].status==c_FREE)
				{
					if(ting_flag==1)
					{
						auto s_card=Sprite::createWithTexture(g_card_kind[list->data[i].kind]->getTexture());
						s_card->setAnchorPoint(Vec2(0.5,0.5));
						s_card->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height*0.6));
						p_list[i]->addChild(s_card,1);
					}
					else
					{
						auto s_card=Sprite::createWithTexture(g_card_kind[list->data[i].kind]->getTexture());
						s_card->setAnchorPoint(Vec2(0.5,0.5));
						s_card->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height*0.4));
						p_list[i]->addChild(s_card,1);						
					}
					if(ting_flag==1||(_roundManager->_actionToDo==a_MING&&ting_flag==0))
					{
						if(list->data[i].can_play==cps_YES)
						{
							auto MingCps_yesMask=Sprite::createWithTexture(g_my_ming_canPlay->getTexture());
							MingCps_yesMask->setAnchorPoint(Vec2(0.5,0.5));
							MingCps_yesMask->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height/2));
							p_list[i]->addChild(MingCps_yesMask,2,MING_KOU_MASK);
						}
						else if(list->data[i].can_play==cps_NO)
						{
							Sprite* mask;
							mask=Sprite::createWithTexture(g_my_mask->getTexture());
							mask->setAnchorPoint(Vec2(0.5,1));
							mask->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height));
							p_list[i]->addChild(mask,2,MING_KOU_MASK);
						}
					}
					x+=p_list[i]->getTextureRect().size.width*1.0;
				}
				else if(list->data[i].status==c_MING_KOU)
				{
					if(ting_flag!=1)
					{
						auto KouSign=Sprite::createWithTexture(g_my_kou->getTexture());
						KouSign->setAnchorPoint(Vec2(0.5,0.5));
						KouSign->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height/2));
						p_list[i]->addChild(KouSign,2);

						//auto str_card_kind=String::createWithFormat("majiang/%d.png",(int)(list->data[i].kind+1));
						auto s_card=Sprite::createWithTexture(g_card_kind[list->data[i].kind]->getTexture());
						s_card->setAnchorPoint(Vec2(0.5,0.5));
						s_card->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height*0.4));
						p_list[i]->addChild(s_card,1);
						x += p_list[i]->getTextureRect().size.width*1;
					}
					else
					{
						auto s_card=Sprite::createWithTexture(g_card_kind[list->data[i].kind]->getTexture());
						s_card->setAnchorPoint(Vec2(0.5,0.5));
						s_card->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height*0.6));
						p_list[i]->addChild(s_card,1);
						x += p_list[i]->getTextureRect().size.width*1;
					}
				}
				else if(list->data[i].status==c_PENG)
				{
					auto s_card=Sprite::createWithTexture(g_mid_card_kind[list->data[i].kind]->getTexture());
					s_card->setAnchorPoint(Vec2(0.5,0.5));
					s_card->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height*0.6));
					p_list[i]->addChild(s_card,1);
					if(list->data[i+1].status==c_FREE)
						x += p_list[i]->getTextureRect().size.width*2;
					else if(list->data[i].kind!=list->data[i+1].kind &&(list->data[i+1].status!=c_FREE))
						x += p_list[i]->getTextureRect().size.width*1.5;
					else
						x += p_list[i]->getTextureRect().size.width*1.0;
				}
				else if(list->data[i].status==c_MING_GANG)
				{
					auto s_card=Sprite::createWithTexture(g_mid_card_kind[list->data[i].kind]->getTexture());
					s_card->setAnchorPoint(Vec2(0.5,0.5));
					s_card->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height*0.6));
					p_list[i]->addChild(s_card,1);
					if((list->data[i].kind==list->data[i+1].kind)&&(list->data[i].kind==list->data[i+2].kind)&&(list->data[i].kind==list->data[i+3].kind)&&(list->data[i].kind!=list->data[i+4].kind))//1
						x+=p_list[i]->getTextureRect().size.width*1.0;
					else if((list->data[i].kind==list->data[i+1].kind)&&(list->data[i].kind==list->data[i+2].kind)&&(list->data[i].kind!=list->data[i+3].kind))//2
						y+=17;
					else if(list->data[i].kind==list->data[i+1].kind&&list->data[i].kind!=list->data[i+2].kind)//3
					{
						x+=p_list[i]->getTextureRect().size.width*1.0;
						y-=17;
					}
					else if(list->data[i].kind!=list->data[i+1].kind)//4
					{
						if(list->data[i+1].status==c_FREE)
							x += p_list[i]->getTextureRect().size.width*2;
						else if(list->data[i].kind!=list->data[i+1].kind &&(list->data[i+1].status!=c_FREE))
							x += p_list[i]->getTextureRect().size.width*1.5;
						else
							x += p_list[i]->getTextureRect().size.width*1.0;
					}
				}
				else if(list->data[i].status==c_AN_GANG)
				{
					if((list->data[i].kind==list->data[i+1].kind)&&(list->data[i].kind==list->data[i+2].kind)&&(list->data[i].kind==list->data[i+3].kind)&&(list->data[i].kind!=list->data[i+4].kind))//1
						x+=p_list[i]->getTextureRect().size.width*1.0;
					else if((list->data[i].kind==list->data[i+1].kind)&&(list->data[i].kind==list->data[i+2].kind)&&(list->data[i].kind!=list->data[i+3].kind))//2
						y+=17;
					else if(list->data[i].kind==list->data[i+1].kind&&list->data[i].kind!=list->data[i+2].kind)//3
					{
						auto s_card=Sprite::createWithTexture(g_mid_card_kind[list->data[i].kind]->getTexture());
						s_card->setAnchorPoint(Vec2(0.5,0.5));
						s_card->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height*0.6));
						p_list[i]->addChild(s_card,1);
						x+=p_list[i]->getTextureRect().size.width*1.0;
						y-=17;
					}
					else if(list->data[i].kind!=list->data[i+1].kind)//4
					{
						if(list->data[i+1].status==c_FREE)
							x += p_list[i]->getTextureRect().size.width*2;
						else if(list->data[i].kind!=list->data[i+1].kind &&(list->data[i+1].status!=c_FREE))
							x += p_list[i]->getTextureRect().size.width*1.5;
						else
							x += p_list[i]->getTextureRect().size.width*1.0;
					}
				}
			}
			else if(no==2)
			{
				if(list->data[i].status==c_FREE)
				{
					if(ting_flag==1)
					{
						p_list[i]=Sprite::createWithTexture(g_left_right_peng_out->getTexture());
						p_list[i]->setAnchorPoint(Vec2(0.3f,0));
						p_list[i]->setPosition(Vec2(x-10,y-8));
					}
					else if(ting_flag!=1&&_roundManager->_players[1]->get_parter()->get_ting_status()==1)
					{
						p_list[i]=Sprite::createWithTexture(g_left_right_angang->getTexture());
						p_list[i]->setAnchorPoint(Vec2(0.3f,0));
						p_list[i]->setPosition(Vec2(x-10,y-8));
					}
					else
					{
						p_list[i]=Sprite::createWithTexture(g_right_free->getTexture());
						p_list[i]->setAnchorPoint(Vec2(0.0f,0));
						p_list[i]->setPosition(Vec2(x-10,y-8));
					}
				}
				else if(list->data[i].status==c_AN_GANG)
				{
					if(list->data[i].kind==list->data[i+1].kind&&list->data[i].kind!=list->data[i+2].kind&&ting_flag!=1&&_roundManager->_players[1]->get_parter()->get_ting_status()==1)
						p_list[i]=Sprite::createWithTexture(g_left_right_peng_out->getTexture());
					else
						p_list[i]=Sprite::createWithTexture(g_left_right_angang->getTexture());
					p_list[i]->setAnchorPoint(Vec2(0.3f,0));
					p_list[i]->setPosition(Vec2(x-10,y-8));
				}
				else if(list->data[i].status==c_MING_KOU)
				{
					p_list[i]=Sprite::createWithTexture(g_left_right_angang->getTexture());
					p_list[i]->setAnchorPoint(Vec2(0.3f,0));
					p_list[i]->setPosition(Vec2(x-10,y-8));
				}
				else if(list->data[i].status==c_PENG||list->data[i].status==c_MING_GANG )
				{
					p_list[i]=Sprite::createWithTexture(g_left_right_peng_out->getTexture());
					p_list[i]->setAnchorPoint(Vec2(0.3f,0));
					p_list[i]->setPosition(Vec2(x-10,y-8));
				}
				if(list->data[i].status==c_FREE)
				{
					if(ting_flag==1||_roundManager->_players[1]->get_parter()->get_ting_status()==1)
					{
						Sprite* s_card;
						if(ting_flag==1)
							s_card=Sprite::createWithTexture(g_small_card_kind[list->data[i].kind]->getTexture());
						else if(ting_flag!=1&&_roundManager->_players[1]->get_parter()->get_ting_status()==1)
							s_card=Sprite::createWithTexture(g_small_cardKind_black[list->data[i].kind]->getTexture());
						s_card->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height*0.65));
						s_card->setRotation(-90);
						s_card->setScale(0.9);
						p_list[i]->addChild(s_card);
						y+=((p_list[i]->getTextureRect().size.height)*0.65);

					}
					else
						y+=((p_list[i]->getTextureRect().size.height)*0.5);
				}
				else if(list->data[i].status==c_MING_KOU)
					y+=((p_list[i]->getTextureRect().size.height)*0.65);
				else if(list->data[i].status==c_PENG)
				{
					auto s_card=Sprite::createWithTexture(g_small_card_kind[list->data[i].kind]->getTexture());
					s_card->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height*0.65));
					s_card->setRotation(-90);
					s_card->setScale(0.9);
					p_list[i]->addChild(s_card);
					if(_roundManager->_players[1]->get_parter()->get_ting_status()==1||ting_flag==1)
					{
						if(list->data[i+1].status==c_FREE||list->data[i+1].status==c_MING_KOU)
							y+=((p_list[i]->getTextureRect().size.height)*1);
						else if(list->data[i+1].kind!=list->data[i].kind)
							y+=((p_list[i]->getTextureRect().size.height)*0.65+5);
						else
							y+=((p_list[i]->getTextureRect().size.height)*0.65);
					}
					else
					{
						if(list->data[i+1].status==c_FREE)
							y+=((p_list[i]->getTextureRect().size.height)*0.8);
						else if(list->data[i+1].kind!=list->data[i].kind)
							y+=((p_list[i]->getTextureRect().size.height)*0.65+5);
						else
							y+=((p_list[i]->getTextureRect().size.height)*0.65);
					}
				}
				else if(list->data[i].status==c_MING_GANG)
				{
					auto s_card=Sprite::createWithTexture(g_small_card_kind[list->data[i].kind]->getTexture());
					s_card->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height*0.65));
					s_card->setRotation(-90);
					s_card->setScale(0.9);
					p_list[i]->addChild(s_card);
					if((list->data[i].kind==list->data[i+1].kind)&&(list->data[i].kind==list->data[i+2].kind)&&(list->data[i].kind==list->data[i+3].kind)&&(list->data[i].kind!=list->data[i+4].kind))//1
						y+=(p_list[i]->getTextureRect().size.height*0.65+13);
					else if((list->data[i].kind==list->data[i+1].kind)&&(list->data[i].kind==list->data[i+2].kind)&&(list->data[i].kind!=list->data[i+3].kind))//2
						y-=13;
					else if(list->data[i].kind==list->data[i+1].kind&&list->data[i].kind!=list->data[i+2].kind)//3
						y+=p_list[i]->getTextureRect().size.height*0.65;
					else if(list->data[i].kind!=list->data[i+1].kind)//4
					{
						if(_roundManager->_players[1]->get_parter()->get_ting_status()==1||ting_flag==1)
						{
							if(list->data[i+1].status==c_FREE||list->data[i+1].status==c_MING_KOU)
								y+=((p_list[i]->getTextureRect().size.height)*1);
							else
								y+=((p_list[i]->getTextureRect().size.height)*0.65+5);
						}
						else
						{
							if(list->data[i+1].status==c_FREE)
								y+=((p_list[i]->getTextureRect().size.height)*0.8);
							else
								y+=((p_list[i]->getTextureRect().size.height)*0.65+5);
						}
					}
				}
				else if(list->data[i].status==c_AN_GANG)
				{
					if((list->data[i].kind==list->data[i+1].kind)&&(list->data[i].kind==list->data[i+2].kind)&&(list->data[i].kind==list->data[i+3].kind)&&(list->data[i].kind!=list->data[i+4].kind))//1
						y+=(p_list[i]->getTextureRect().size.height*0.65+13);
					else if((list->data[i].kind==list->data[i+1].kind)&&(list->data[i].kind==list->data[i+2].kind)&&(list->data[i].kind!=list->data[i+3].kind))//2
					{
						if(ting_flag!=1&&_roundManager->_players[1]->get_parter()->get_ting_status()==1)
						{
							Sprite* s_card;
							if(ting_flag==1)
								s_card=Sprite::createWithTexture(g_small_card_kind[list->data[i].kind]->getTexture());
							else if(ting_flag!=1&&_roundManager->_players[1]->get_parter()->get_ting_status()==1)
								s_card=Sprite::createWithTexture(g_small_cardKind_black[list->data[i].kind]->getTexture());
							s_card->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height*0.65));
							s_card->setRotation(-90);
							s_card->setScale(0.9);
							p_list[i]->addChild(s_card);
						}
						y-=13;
					}
					else if(list->data[i].kind==list->data[i+1].kind&&list->data[i].kind!=list->data[i+2].kind)//3
						y+=((p_list[i]->getTextureRect().size.height)*0.65);
					else if(list->data[i].kind!=list->data[i+1].kind)//4
					{
						if(_roundManager->_players[1]->get_parter()->get_ting_status()==1||ting_flag==1)
						{
							if(list->data[i+1].status==c_FREE||list->data[i+1].status==c_MING_KOU)
								y+=((p_list[i]->getTextureRect().size.height)*1);
							else
								y+=((p_list[i]->getTextureRect().size.height)*0.65+5);
						}
						else
						{
							if(list->data[i+1].status==c_FREE)
								y+=((p_list[i]->getTextureRect().size.height)*0.8);
							else
								y+=((p_list[i]->getTextureRect().size.height)*0.65+5);
						}
					}
				}
			}
			if(no!=2)
				myframe->addChild(p_list[i],i+1,HAND_IN_CARDS_TAG_ID+no*20+i);
			else if(no==2)
				myframe->addChild(p_list[i],list->len-i,HAND_IN_CARDS_TAG_ID+no*20+i);
			if(no==1&&_roundManager->_actionToDo==a_MING&&_roundManager->_players[1]->get_parter()->get_ting_status()!=1&&list->data[i].can_play==cps_YES)
				p_list[i]->setZOrder(30);
			p_list[i]->_ID=1;
			if(no==1 && MyCardChoosedNum==i)
				p_list[i]->_ID=100;
		}
	}
	if(no==1&&ting_flag!=0)
	{
		if(!myframe->getChildByTag(TING_SING_BUTTON))
		{
			Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(myframe,true);//
			//auto Ting_Sign=Button::create("fanxingtishi.png","fanxingtishi.png","fanxingtishi.png",UI_TEX_TYPE_PLIST);
			//Ting_Sign->setAnchorPoint(Vec2(0,0));
			//Ting_Sign->setPosition(Vec2(origin.x+visibleSize.width*0.9,origin.y+visibleSize.height*0.2));
			//myframe->addChild(Ting_Sign,1,TING_SING_BUTTON);
			auto Ting_Sign=Sprite::createWithSpriteFrameName("fanxingtishi.png");
			Ting_Sign->setAnchorPoint(Vec2(0,0));
			Ting_Sign->setPosition(Vec2(origin.x+visibleSize.width*0.9,origin.y+visibleSize.height*0.2));
			myframe->addChild(Ting_Sign,1,TING_SING_BUTTON);
			while(myframe->getChildByTag(TING_SING_BAR))
				myframe->removeChildByTag(TING_SING_BAR);
			tingHintCreate(Ting_Sign->getPosition(),Hu_cardOut_place);
			myframe->getChildByTag(TING_SING_BAR)->setVisible(false);
			//ifTingSignBarVisible=false;
			ifTingSignBarVisible=false;
			ListenToTingButton();
			card_list_update(0);
			card_list_update(2);
		}
	}
	if(no==1&&myframe->getChildByTag(TING_SING_BUTTON))//||(no!=1&&_roundManager->_players[no]->get_parter()->get_ting_status()))
		update_residue_TingCards(1);
	//Vec2 cardsPosition[MAX_HANDIN_NUM];
	//for(int count=0;count<list->len;count++)
	//{
	//	if(p_list[count]->getPosition().x&&p_list[count]->getPosition().y)
	//		cardsPosition[count]=p_list[count]->getPosition();
	//}
}

void NetRaceLayer::update_card_list(Node *psender)
{
    int no = psender->_ID;
    
	if(_roundManager->IsTing(no)) {
        _Show(this,MING_STATUS_PNG_0+no,true);
	}
    
	card_list_update(no);
}

void NetRaceLayer::update_card_in_river_list(Node* sender)
{
    LOGGER_WRITE("%s : %x",__FUNCTION__,(int)sender);

	auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);

	outCardList* outCard=_roundManager->_players[sender->_ID]->get_parter()->getOutCardList();

	for(int ik=0;ik<outCard->length;ik++)
	{
		if(myframe->getChildByTag(HAND_OUT_CARDS_TAG_ID+sender->_ID*25+ik))
		{
			myframe->removeChildByTag(HAND_OUT_CARDS_TAG_ID+sender->_ID*25+ik,true);
		}
	}
	auto show_card_indicator=this->getChildByTag(SHOWCARD_INDICATOR_TAG_ID);
	show_card_indicator->setVisible(true);

	for(int i=0;i<outCard->length;i++)
	{
		if(sender->_ID==1)
		{
			auto show_card=Sprite::createWithTexture(g_my_out->getTexture());
			Card oCard;
			outCard->getCard(oCard,i+1);
			auto show_card_kind=Sprite::createWithTexture(g_small_card_kind[oCard.kind]->getTexture());
			show_card_kind->setPosition(show_card->getTextureRect().size.width/2,show_card->getTextureRect().size.height*0.65);
			show_card->addChild(show_card_kind);
			show_card->setAnchorPoint(Vec2(0,1));
			int zOrder;
			show_card_indicator->stopAllActions();
			if(i<6)
			{
				show_card_indicator->setPosition(Vec2(river_point[sender->_ID].x+((i)*36)+20,river_point[sender->_ID].y+5));						
				show_card_indicator->runAction(RepeatForever::create(Sequence::create(MoveTo::create(0.5,Vec2(river_point[sender->_ID].x+((i)*36)+20,river_point[sender->_ID].y+15)),MoveTo::create(0.5,Vec2(river_point[sender->_ID].x+((i)*36)+20,river_point[sender->_ID].y+5)),NULL)));
				show_card->setPosition(Vec2(river_point[sender->_ID].x+i*36,river_point[sender->_ID].y));
				zOrder=0;
			}
			else if(i<14)
			{
				show_card_indicator->setPosition(Vec2(river_point[sender->_ID].x+(i-7)*36+20,river_point[sender->_ID].y-36));
				show_card_indicator->runAction(RepeatForever::create(Sequence::create(MoveTo::create(0.5,Vec2(river_point[sender->_ID].x+(i-7)*36+20,river_point[sender->_ID].y-26)),MoveTo::create(0.5,Vec2(river_point[sender->_ID].x+(i-7)*36+20,river_point[sender->_ID].y-36)),NULL)));		
				show_card->setPosition(Vec2(river_point[sender->_ID].x+(i-7)*36,river_point[sender->_ID].y-41));
				zOrder=1;
			}
			else
			{
				show_card_indicator->setPosition(Vec2(river_point[sender->_ID].x+(i-16)*36+20,river_point[sender->_ID].y-77));
				show_card_indicator->runAction(RepeatForever::create(Sequence::create(MoveTo::create(0.5,Vec2(river_point[sender->_ID].x+(i-16)*36+20,river_point[sender->_ID].y-67)),MoveTo::create(0.5,Vec2(river_point[sender->_ID].x+(i-16)*36+20,river_point[sender->_ID].y-77)),NULL)));
				show_card->setPosition(Vec2(river_point[sender->_ID].x+(i-16)*36,river_point[sender->_ID].y-82));
				zOrder=2;
			}
			myframe->addChild(show_card,zOrder+1,HAND_OUT_CARDS_TAG_ID+sender->_ID*25+i);
			//update_clock(true,0,sender->_ID);//////////-----网络版需要获取时间----///////////
		}

		if(sender->_ID==2)
		{
			auto show_card=Sprite::createWithTexture(g_left_right_peng_out->getTexture());
			Card oCard;
			outCard->getCard(oCard,i+1);
			auto show_card_kind=Sprite::createWithTexture(g_small_card_kind[oCard.kind]->getTexture());
			show_card_kind->setRotation(-90);
			show_card_kind->setAnchorPoint(Vec2(0.5,0.5));
			show_card_kind->setScale(0.9);
			show_card_kind->setPosition(show_card->getTextureRect().size.width/2,show_card->getTextureRect().size.height*0.65);
			show_card->addChild(show_card_kind);
			show_card->setAnchorPoint(Vec2(0.5,0.5));
			show_card_indicator->stopAllActions();
			if(i<6)
			{
				show_card_indicator->setPosition(Vec2(river_point[sender->_ID].x,river_point[sender->_ID].y+30*(i+1)));						
				show_card_indicator->runAction(RepeatForever::create(Sequence::create(MoveTo::create(0.5,Vec2(river_point[sender->_ID].x,river_point[sender->_ID].y+30*(i+1)+10)),MoveTo::create(0.5,Vec2(river_point[sender->_ID].x,river_point[sender->_ID].y+30*(i+1))),NULL)));
				show_card->setPosition(Vec2(river_point[sender->_ID].x,river_point[sender->_ID].y+30*i));
			}
			else if(i<14)
			{
			show_card_indicator->setPosition(Vec2(river_point[sender->_ID].x+48,river_point[sender->_ID].y+30*(i-6)));
			show_card_indicator->runAction(RepeatForever::create(Sequence::create(MoveTo::create(0.5,Vec2(river_point[sender->_ID].x+48,river_point[sender->_ID].y+30*(i-6)+10)),MoveTo::create(0.5,Vec2(river_point[sender->_ID].x+48,river_point[sender->_ID].y+30*(i-6))),NULL)));
			show_card->setPosition(Vec2(river_point[sender->_ID].x+48,river_point[sender->_ID].y+30*(i-7)));
			}
			else
			{
				show_card_indicator->setPosition(Vec2(river_point[sender->_ID].x+96,river_point[sender->_ID].y+30*(i-15)));
				show_card_indicator->runAction(RepeatForever::create(Sequence::create(MoveTo::create(0.5,Vec2(river_point[sender->_ID].x+96,river_point[sender->_ID].y+30*(i-15)+10)),MoveTo::create(0.5,Vec2(river_point[sender->_ID].x+96,river_point[sender->_ID].y+30*(i-15))),NULL)));
				show_card->setPosition(Vec2(river_point[sender->_ID].x+96,river_point[sender->_ID].y+30*(i-16)));
			}
			myframe->addChild(show_card,20-i,HAND_OUT_CARDS_TAG_ID+sender->_ID*25+i);
			//update_clock(true,0,sender->_ID);//////////-----网络版需要获取时间----///////////
		}

		if(sender->_ID==0)
		{
			auto show_card=Sprite::createWithTexture(g_left_right_peng_out->getTexture());
			Card oCard;
			outCard->getCard(oCard,i+1);
			auto show_card_kind=Sprite::createWithTexture(g_small_card_kind[oCard.kind]->getTexture());
			show_card_kind->setRotation(90);
			show_card_kind->setAnchorPoint(Vec2(0.5,0.5));
			show_card_kind->setScale(0.9);
			show_card_kind->setPosition(show_card->getTextureRect().size.width/2,show_card->getTextureRect().size.height*0.65);
			show_card->addChild(show_card_kind);
			show_card->setAnchorPoint(Vec2(0.5,0.5));
			show_card_indicator->stopAllActions();
			if(i<6)
			{
				show_card_indicator->setPosition(Vec2(river_point[sender->_ID].x,river_point[sender->_ID].y-30*(i-1)));						
				show_card_indicator->runAction(RepeatForever::create(Sequence::create(MoveTo::create(0.5,Vec2(river_point[sender->_ID].x,river_point[sender->_ID].y-30*(i-1)+10)),MoveTo::create(0.5,Vec2(river_point[sender->_ID].x,river_point[sender->_ID].y-30*(i-1))),NULL)));
				show_card->setPosition(Vec2(river_point[sender->_ID].x,river_point[sender->_ID].y-30*i));
			}
			else if(i<14)
			{
				show_card_indicator->setPosition(Vec2(river_point[sender->_ID].x-48,river_point[sender->_ID].y-30*(i-8)));
				show_card_indicator->runAction(RepeatForever::create(Sequence::create(MoveTo::create(0.5,Vec2(river_point[sender->_ID].x-48,river_point[sender->_ID].y-30*(i-8)+10)),MoveTo::create(0.5,Vec2(river_point[sender->_ID].x-48,river_point[sender->_ID].y-30*(i-8))),NULL)));
				show_card->setPosition(Vec2(river_point[sender->_ID].x-48,river_point[sender->_ID].y-30*(i-7)));
			}
			else
			{
				show_card_indicator->setPosition(Vec2(river_point[sender->_ID].x-96,river_point[sender->_ID].y-30*(i-17)));
				show_card_indicator->runAction(RepeatForever::create(Sequence::create(MoveTo::create(0.5,Vec2(river_point[sender->_ID].x-96,river_point[sender->_ID].y-30*(i-17)+10)),MoveTo::create(0.5,Vec2(river_point[sender->_ID].x-96,river_point[sender->_ID].y-30*(i-17))),NULL)));
				show_card->setPosition(Vec2(river_point[sender->_ID].x-96,river_point[sender->_ID].y-30*(i-16)));
			}
			myframe->addChild(show_card,i+1,HAND_OUT_CARDS_TAG_ID+sender->_ID*25+i);
			//update_clock(true,0,sender->_ID);
		}
	}
}

void NetRaceLayer::race_begin_prepare()
{
    LOGGER_WRITE("%s",__FUNCTION__);

	//SimpleAudioEngine::sharedEngine()->preloadEffect("Music/lanpai.ogg");	

	int i,j,temp;
	int list[3][13];
	int aim[3];

	if(this->getChildByTag(READY_INDICATE_LEFT_TAG_ID))
		this->removeChildByTag(READY_INDICATE_LEFT_TAG_ID,true);

	if(this->getChildByTag(READY_INDICATE_RIGHT_TAG_ID))
		this->removeChildByTag(READY_INDICATE_RIGHT_TAG_ID,true);

	if(this->getChildByTag(READY_INDICATE_MID_TAG_ID))
		this->removeChildByTag(READY_INDICATE_MID_TAG_ID,true);

	(Sprite*)this->getChildByTag(LEFT_IMG_BKG_TAG_ID)->runAction(EaseIn::create(MoveTo::create(0.6,Vec2(29,536)),1.5));
	(Sprite*)this->getChildByTag(RIGHT_IMG_BKG_TAG_ID)->runAction(EaseIn::create(MoveTo::create(0.6,Vec2(1190,536)),1.5));
	(Sprite*)this->getChildByTag(MID_IMG_BKG_TAG_ID)->runAction(EaseIn::create(MoveTo::create(0.6,Vec2(49,129)),1.5));
	(Sprite*)this->getChildByTag(MIC_TAG_ID)->runAction(Sequence::create(DelayTime::create(0.65),EaseBounceOut::create(ScaleTo::create(0.3,1)),NULL));
	for(int i=CARD_ARRAGE_LEFT_TAG_ID;i<=CARD_ARRAGE_TOP_TAG_ID;i++)
		(Sprite*)this->getChildByTag(i)->runAction(EaseIn::create(FadeOut::create(0.2),2));

	auto colorLayer=LayerColor::create(Color4B(0,0,0,150),visibleSize.width,visibleSize.height*200/716);
	colorLayer->setAnchorPoint(Vec2(0.5,0.5));
	colorLayer->ignoreAnchorPointForPosition(false);
	colorLayer->setScale(0);
	colorLayer->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
	this->addChild(colorLayer,4,COLOR_LAYER_TAG_ID);

	auto VoiceEffect=CallFunc::create([=](){SimpleAudioEngine::sharedEngine()->playEffect("Music/duijukaishi.ogg");});
	auto paijuStart=Sprite::create("paijukaishi.png");
	paijuStart->setScale(0);
	paijuStart->setAnchorPoint(Vec2(0.5,0.5));
	paijuStart->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
	this->addChild(paijuStart,5,RACE_START_TAG_ID);

	auto paijuStart_guang=Sprite::create("paijuStart_guang.png");
	paijuStart_guang->setAnchorPoint(Vec2(0.5,0.5));
	paijuStart_guang->setScale(0);
	paijuStart_guang->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
	this->addChild(paijuStart_guang,6,RACE_LIGHT_TAG_ID);

	auto StartActionIn=FadeIn::create(0.2);
	auto StartActionScale=ScaleTo::create(0.2,1);
	auto StartActionFirst=Spawn::create(StartActionIn,StartActionScale,NULL);
	auto StartActionOut=Spawn::create(ScaleTo::create(0.3,4),FadeOut::create(0.3),NULL);

	paijuStart->runAction(Sequence::create(DelayTime::create(0.7),ScaleTo::create(0,4),StartActionFirst,Spawn::create(VoiceEffect,Sequence::create(DelayTime::create(0.5),ScaleTo::create(0.15,0.5),StartActionOut,NULL),NULL),NULL));
	colorLayer->runAction(Sequence::create(DelayTime::create(0.9),ScaleTo::create(0,1),DelayTime::create(0.9),ScaleTo::create(0.4,1,0),NULL));
	paijuStart_guang->runAction(Sequence::create(DelayTime::create(1.7),Spawn::create(ScaleTo::create(0.3,8,3),FadeIn::create(0.3),NULL),Spawn::create(ScaleTo::create(0.5,8,0),FadeOut::create(0.5),NULL),NULL));

	if(this>getChildByTag(SHOWCARD_INDICATOR_TAG_ID))
		this->getChildByTag(SHOWCARD_INDICATOR_TAG_ID)->setVisible(false);

    int lastWinner = _roundManager->GetLastWinner();
    switch(lastWinner) {
        case 0:
            last_winner->setAnchorPoint(Vec2(0.5,1));
            last_winner->setPosition(lastwinner_point[0]);
            break;
        case 1:
            last_winner->setAnchorPoint(Vec2(0,0.5));
            last_winner->setPosition(lastwinner_point[1]);
            break;
        case 2:
            last_winner->setAnchorPoint(Vec2(0.5,1));
            last_winner->setPosition(lastwinner_point[2]);
            break;
    }
	last_winner->runAction(Sequence::create(DelayTime::create(0.65),EaseBounceOut::create(ScaleTo::create(0.3,1)),NULL));

	auto MingSignForMe=(Sprite*)this->getChildByTag(MING_STATUS_PNG_1);
	if(lastWinner!=1)
		MingSignForMe->setPosition(Vec2(origin.x+visibleSize.width*255/1218,origin.y+visibleSize.height*168/716));
	else
		MingSignForMe->setPosition(Vec2(origin.x+visibleSize.width*255/1218+last_winner->getTextureRect().size.width,origin.y+visibleSize.height*168/716));
}
void NetRaceLayer::KouCancle(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
{
    LOGGER_WRITE("%s",__FUNCTION__);

	auto curButton=(Button*)pSender;
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
			if (myframe->getChildByTag(MING_KOU_ENSURE))
				myframe->removeChildByTag(MING_KOU_ENSURE);			
			if (myframe->getChildByTag(MING_KOU_SIGN))
				myframe->removeChildByTag(MING_KOU_SIGN);
			Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(myframe,true);
			Sprite *p_list[MAX_HANDIN_NUM];
			auto list=_roundManager->_players[1]->get_parter()->get_card_list();
			for(int a=list->atcvie_place;a<list->len;a++)
			{
				list->data[a].status=c_FREE;
				p_list[a]=(Sprite*)myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+20+a);
				if(p_list[a]->getChildByTag(MING_KOU))
					p_list[a]->removeChildByTag(MING_KOU);
			}			
			curButton->setTouchEnabled(false);
			auto ButtonAction=TargetedAction::create(curButton,ScaleTo::create(0,0));
			auto callFunc=CallFuncN::create(this,callfuncN_selector(NetRaceLayer::ming_tip_effect));
			//auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
			myframe->_ID=1;
			myframe->runAction(Sequence::create(ButtonAction,callFunc,NULL));
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}	
}
void NetRaceLayer::KouEnsure(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
{
    LOGGER_WRITE("%s",__FUNCTION__);

	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
			if (myframe->getChildByTag(MING_KOU_SIGN))
				myframe->removeChildByTag(MING_KOU_SIGN);
			auto list=_roundManager->_players[1]->get_parter()->get_card_list();
			for(int a=list->atcvie_place;a<list->len;a++)
			{
				if(list->data[a].status==c_KOU_ENABLE)
					list->data[a].status=c_FREE;
			}	
			Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(myframe,true);
			auto curButton=(Button*)pSender;
			curButton->setTouchEnabled(false);
			auto buttonact=ScaleTo::create(0,0);
			_roundManager->_players[1]->get_parter()->action(_roundManager->_isCardFromOthers,a_KOU);
			auto ming_indexesCur=_roundManager->_players[1]->get_parter()->ming_check();
			_roundManager->_players[1]->get_parter()->set_ming_indexes(ming_indexesCur);
			auto buttonAction=TargetedAction::create(curButton,buttonact);
			auto callFunc0=CCCallFuncN::create(this,callfuncN_selector(NetRaceLayer::update_card_list));//????
			auto callFunc1=CCCallFunc::create(this,callfunc_selector(NetRaceLayer::delete_act_tip));
			auto callFunc2=CCCallFuncN::create(this,callfuncN_selector(NetRaceLayer::ming_tip_effect));
			auto seq1=Sequence::create(buttonAction,callFunc0,callFunc1,callFunc2,NULL);
			myframe->_ID=_roundManager->_curPlayer;
			myframe->runAction(seq1);
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}
void NetRaceLayer::MingCancle(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
{
    LOGGER_WRITE("%s",__FUNCTION__);

	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			ifMingTime=false;
			auto curButton=(Button*)pSender;
			auto buttonact=ScaleTo::create(0,0);
			auto ButtonAction=TargetedAction::create(curButton,buttonact);
			auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
			auto list=_roundManager->_players[1]->get_parter()->get_card_list();
			_roundManager->_players[1]->get_parter()->action(_roundManager->_isCardFromOthers,a_KOU_CANCEL);
			_roundManager->_actionToDo=a_JUMP;
			auto callFunc2=CallFunc::create([=](){
				_roundManager->_actionToDo=_roundManager->_players[1]->get_parter()->ActiontodoCheckAgain();
				waitfor_myaction(1);
			});
			_roundManager->_players[1]->get_parter()->MingCancel();
			_roundManager->_players[1]->get_parter()->set_ming_indexes(0);
			_roundManager->_players[1]->get_parter()->set_ting_status(0);
			auto callFunc0=CCCallFuncN::create(this,callfuncN_selector(NetRaceLayer::update_card_list));
			auto callFunc1=CCCallFunc::create(this,callfunc_selector(NetRaceLayer::delete_act_tip));
			auto seq1=Sequence::create(ButtonAction,callFunc0,callFunc1,callFunc2,NULL);
			myframe->_ID=_roundManager->_curPlayer;
			myframe->runAction(seq1);
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}
void NetRaceLayer::KouCardsCheck(int no)
{
    LOGGER_WRITE("%s",__FUNCTION__);

	int a,b,c,num;
	CARD_ARRAY *list = _roundManager->_players[no]->get_parter()->get_card_list();
    
	for(a=0;a<4;a++) {
		kouCards_kind[a].kind=ck_NOT_DEFINED;
		kouCards_kind[a].status=c_FREE;
	}
    
	Kou_kindLen=0;
	for(a=0;a<4;a++)
		for(b=0;b<3;b++)
			KouCardsPlace[a][b]=-1;
	int tempCardPlace[4]={-1,-1,-1,-1};
	int countA=0,countB;
	bool ifJudged=false;
	for(a=list->atcvie_place;a<list->len;a++)
	{
		for(c=0;c<4;c++)
		{
			if(kouCards_kind[c].kind==list->data[a].kind)
			{
				ifJudged=true;
				break;
			}
		}
		if(c==4)
			ifJudged=false;
		if(ifJudged)
			continue;

		num=0;
		countB=0;
		for(int b=list->atcvie_place;b<list->len;b++)
		{	
			if(list->data[b].kind==list->data[a].kind)
			{
				tempCardPlace[countB++]=b;
				num++;
				if(num==3)
				{
					if(_roundManager->_players[no]->get_parter()->judge_kou_cards(list->data[a].kind,no,RototHandOutIndex))
					{
						KouCardsPlace[countA][0]=tempCardPlace[0];
						KouCardsPlace[countA][1]=tempCardPlace[1];
						KouCardsPlace[countA][2]=tempCardPlace[2];

						list->data[tempCardPlace[0]].status=c_KOU_ENABLE;
						list->data[tempCardPlace[1]].status=c_KOU_ENABLE;
						list->data[tempCardPlace[2]].status=c_KOU_ENABLE;

						kouCards_kind[countA].kind=list->data[a].kind;
						kouCards_kind[countA++].status=c_KOU_ENABLE;
						Kou_kindLen++;
					}
				}
			}
		}
	}
}

Node *NetRaceLayer::_CreateKouChooseCancelButton() {
    auto ChooseCancel = Button::create("quxiao.png","quxiao.png","quxiao.png",UI_TEX_TYPE_PLIST);
    ChooseCancel->addTouchEventListener(CC_CALLBACK_2(NetRaceLayer::KouCancle,this));
    ChooseCancel->setAnchorPoint(Vec2(0.5,0.5));
    ChooseCancel->setPosition(Vec2(
        origin.x+visibleSize.width*0.15,
        origin.y+visibleSize.height*0.25));
    return ChooseCancel;
}

Node *NetRaceLayer::_CreateKouChooseConfirmButton() {
    auto ChooseEnsure=Button::create("wancheng.png","wancheng.png","wancheng.png",UI_TEX_TYPE_PLIST);
    ChooseEnsure->addTouchEventListener(CC_CALLBACK_2(NetRaceLayer::KouEnsure,this));
    ChooseEnsure->setAnchorPoint(Vec2(0.5,0.5));
    ChooseEnsure->setPosition(Vec2(
        origin.x+visibleSize.width*0.8,
        origin.y+visibleSize.height*0.25));
    return ChooseEnsure;
}


Node *NetRaceLayer::_CreateMingSign() {
    auto MingSign=Sprite::createWithSpriteFrameName("gpts.png");
    MingSign->setAnchorPoint(Vec2(0.5,0.5));
    MingSign->setPosition(Vec2(
        origin.x+visibleSize.width*0.5,
        origin.y+visibleSize.height*0.2));
    return MingSign;
}

Node *NetRaceLayer::_NonKouMask(Sprite *card) {
    auto mask = Sprite::createWithTexture(g_my_mask->getTexture());
    mask->setAnchorPoint(Vec2(0.5,0.5));
    mask->setPosition(card->getTextureRect().size.width/2,card->getTextureRect().size.height/2);
    card->addChild(mask,2,MING_KOU_MASK);

    return mask;
}

void NetRaceLayer::MaskNonKouCards() {
	auto myframe = this->getChildByTag(GAME_BKG_TAG_ID);
    CARD_ARRAY *cards = _roundManager->_players[1]->get_parter()->get_card_list();

    for(int i=cards->atcvie_place; i<cards->len; i++) {
        if(cards->data[i].status!=c_KOU_ENABLE) {
            Sprite *card = (Sprite*)myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+20+i);
            _NonKouMask(card);
        }
    }
}

void NetRaceLayer::ListenToKou(int no) {
	auto myframe = this->getChildByTag(GAME_BKG_TAG_ID);
    auto cards=_roundManager->_players[no]->get_parter()->get_card_list();
    auto KouListener = EventListenerTouchOneByOne::create();
    KouListener->setSwallowTouches(true);
    
    KouListener->onTouchBegan = [=](Touch* touch, Event* event) {
        Sprite *cardsInHand[MAX_HANDIN_NUM] = {0};
        int groupChosen=-1;
    
        for(int a=0;a<cards->len;a++) {
            cardsInHand[a]=(Sprite*)myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+20+a);
            cardsInHand[a]->_ID = 1;
        }

        /* BUG FOUND : only one group could be chosen */
        for(int group=0; group<Kou_kindLen; group++) {
            for(int i=0; i<3; i++) {
                if ( _IsClickedOn(cardsInHand[KouCardsPlace[group][i]], touch) ) {
                    groupChosen = group;
                }
            }
            if(groupChosen!=-1)
                break;
        }
        
        if(groupChosen!=-1) {
            for(int i=0; i<3; i++)
                cardsInHand[KouCardsPlace[groupChosen][i]]->_ID++;
        }
        
        return true;
    };
    
    KouListener->onTouchMoved = [=](Touch* touch, Event* event) {
    };

    KouListener->onTouchEnded = [=](Touch* touch, Event* event) {/* is begin neccessary ??? */
        Sprite *cardsInHand[MAX_HANDIN_NUM];
        int a,b,kindChosen=-1;

        for(int i=0; i<cards->len; i++)
            cardsInHand[i]=(Sprite*)myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+20+i);
        
        for(int kind=0; kind<Kou_kindLen; kind++) {
            if(kouCards_kind[kind].status==c_FREE)
                continue;
            
            for(int i=0;i<3;i++) {
                if ( _IsClickedOn(cardsInHand[KouCardsPlace[kind][i]],touch) ) {
                    if(cardsInHand[KouCardsPlace[kind][i]]->_ID!=1)
                        kindChosen = kind;
                }
            }
            if(kindChosen!=-1)
                break;
        }
        
        if(kindChosen!=-1) {
            if(kouCards_kind[kindChosen].status==c_MING_KOU) {
                kouCards_kind[kindChosen].status=c_KOU_ENABLE;
                cards->data[KouCardsPlace[kindChosen][0]].status=c_KOU_ENABLE;
                cards->data[KouCardsPlace[kindChosen][1]].status=c_KOU_ENABLE;
                cards->data[KouCardsPlace[kindChosen][2]].status=c_KOU_ENABLE;
            }
            else if(kouCards_kind[kindChosen].status==c_KOU_ENABLE)
            {
                kouCards_kind[kindChosen].status=c_MING_KOU;
                cards->data[KouCardsPlace[kindChosen][0]].status=c_MING_KOU;
                cards->data[KouCardsPlace[kindChosen][1]].status=c_MING_KOU;
                cards->data[KouCardsPlace[kindChosen][2]].status=c_MING_KOU;
            }
        }
        
        for( a=0; a<Kou_kindLen; a++ ) {
            if(kouCards_kind[a].status!=c_MING_KOU) {
                if(_roundManager->_players[1]->get_parter()->judge_kou_cards(
                    kouCards_kind[a].kind, no, RototHandOutIndex))
                {
                    cards->data[KouCardsPlace[a][0]].status=c_KOU_ENABLE;
                    cards->data[KouCardsPlace[a][1]].status=c_KOU_ENABLE;
                    cards->data[KouCardsPlace[a][2]].status=c_KOU_ENABLE;
                    kouCards_kind[a].status=c_KOU_ENABLE;
                } else {
                    cards->data[KouCardsPlace[a][0]].status=c_FREE;
                    cards->data[KouCardsPlace[a][1]].status=c_FREE;
                    cards->data[KouCardsPlace[a][2]].status=c_FREE;
                    kouCards_kind[a].status=c_FREE;
                }
            }
        }
        
        for(a=0;a<Kou_kindLen;a++) {
            for(b=0;b<3;b++) {
                if(cardsInHand[KouCardsPlace[a][b]]->getChildByTag(MING_KOU))
                    cardsInHand[KouCardsPlace[a][b]]->removeChildByTag(MING_KOU);
                if(cardsInHand[KouCardsPlace[a][b]]->getChildByTag(MING_KOU_MASK))
                    cardsInHand[KouCardsPlace[a][b]]->removeChildByTag(MING_KOU_MASK);
                if(cards->data[KouCardsPlace[a][b]].status==c_MING_KOU) {
                    auto MingKouMark=Sprite::createWithTexture(g_my_kou->getTexture());
                    MingKouMark->setAnchorPoint(Vec2(0.5,0.5));
                    MingKouMark->setPosition(Vec2(cardsInHand[KouCardsPlace[a][b]]->getTextureRect().size.width/2,cardsInHand[KouCardsPlace[a][b]]->getTextureRect().size.height/2));
                    cardsInHand[KouCardsPlace[a][b]]->addChild(MingKouMark,2,MING_KOU);
                } else if(cards->data[KouCardsPlace[a][b]].status!=c_KOU_ENABLE) {
                    auto KouNo=Sprite::createWithTexture(g_my_mask->getTexture());
                    KouNo->setAnchorPoint(Vec2(0.5,0.5));
                    KouNo->setPosition(Vec2(cardsInHand[KouCardsPlace[a][b]]->getTextureRect().size.width/2,cardsInHand[KouCardsPlace[a][b]]->getTextureRect().size.height/2));
                    cardsInHand[KouCardsPlace[a][b]]->addChild(KouNo,2,MING_KOU_MASK);
                }
            }
        }
        
        bool ifEnsureVisible=false;
        for(a=0;a<4;a++) {
            if(kouCards_kind[a].status==c_MING_KOU) {
                ifEnsureVisible=true;
                break;
            }
        }
        
        if(ifEnsureVisible)
            myframe->getChildByTag(MING_KOU_ENSURE)->setVisible(true);
        else
            myframe->getChildByTag(MING_KOU_ENSURE)->setVisible(false);
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(KouListener, myframe);
}

void NetRaceLayer::ming_kou_Choose(int no)
{
    LOGGER_WRITE("%s",__FUNCTION__);

	auto myframe = this->getChildByTag(GAME_BKG_TAG_ID);
    
	if(no==1) {
		myframe->addChild(_CreateKouChooseCancelButton(),20,MING_KOU_CANCEL);
		myframe->addChild(_CreateMingSign(),20,MING_KOU_SIGN);

		auto ChooseEnsure = _CreateKouChooseConfirmButton();
		ChooseEnsure->setVisible(false);
		myframe->addChild(ChooseEnsure,20,MING_KOU_ENSURE);

        MaskNonKouCards();
        ListenToKou(no);
	} else {
		auto list=_roundManager->_players[no]->get_parter()->get_card_list();
        
		for(int a=0;a<Kou_kindLen;a++)
		{
			if(_roundManager->_players[no]->get_parter()->judge_kou_cards(
                kouCards_kind[a].kind,no,RototHandOutIndex))
			{
				for(int b=0;b<3;b++)
				{
					list->data[KouCardsPlace[a][b]].status=c_MING_KOU;
				}
			}
		}
	}
}
void NetRaceLayer::ming_tip_effect(Node *psender)
{
    LOGGER_WRITE("%s",__FUNCTION__);

	auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
	_roundManager->_actionToDo=a_MING;
	//continue_gang_times=0;
	//_roundManager->_lastAction=a_MING;
	if(psender->_ID==1)
	{
		ifMingTime=true;
		_roundManager->_players[_roundManager->_curPlayer]->get_parter()->action(_roundManager->_isCardFromOthers,a_MING);
		if(myframe->getChildByTag(MING_KOU_ENSURE))
			myframe->removeChildByTag(MING_KOU_ENSURE);
		if(myframe->getChildByTag(MING_KOU_SIGN))
			myframe->removeChildByTag(MING_KOU_SIGN);
		auto MingCancel=Button::create("quxiao.png","quxiao.png","quxiao.png",UI_TEX_TYPE_PLIST);
		MingCancel->addTouchEventListener(CC_CALLBACK_2(NetRaceLayer::MingCancle,this));
		MingCancel->setAnchorPoint(Vec2(0.5,0.5));
		MingCancel->setPosition(Vec2(origin.x+visibleSize.width*0.15,origin.y+visibleSize.height*0.25));
		myframe->addChild(MingCancel,20,MING_CANCEL);

		auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
		if(myframe->getChildByTag(MING_KOU_CANCEL))
			myframe->removeChildByTag(MING_KOU_CANCEL,true);

		auto callFunc0=CCCallFuncN::create(this,callfuncN_selector(NetRaceLayer::update_card_list));
		auto callFunc2=CCCallFunc::create(this,callfunc_selector(NetRaceLayer::waitfor_MyTouchShowCard));
		auto seq1=Sequence::create(callFunc0,callFunc2,NULL);
		myframe->_ID=1;
		myframe->runAction(seq1);
	}
	else if(psender->_ID!=1)
	{
		//_roundManager->_lastAction=a_MING;
		int curNo=psender->_ID;
		auto callFunc2=CCCallFunc::create(this,callfunc_selector(NetRaceLayer::waitfor_ShowCardWithoutTouch));
		myframe->_ID=curNo;
		myframe->runAction(callFunc2);
	}
	/*active_place_indexes 是按照bit 位来只是哪些张牌可以出，如果bit0==1 表示左手起的第一张可以出*/
	//update_card_list(_roundManager->_curPlayer-1);
	//scheduleOnce(schedule_selector(NetRaceLayer::waitfor_ShowCardWithoutTouch),0.3);
	//scheduleOnce(schedule_selector(NetRaceLayer::delete_act_tip),0.3);
}

void NetRaceLayer::MingPressed(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
{

	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		((Button*)pSender)->setScale(1.2);
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
            LOGGER_WRITE("%s",__FUNCTION__);
            
            if(_roundManager->_isWaitDecision) {
				_roundManager->_isWaitDecision = false;
				_roundManager->_actionToDo =_roundManager->_tempActionToDo;
				_roundManager->_tempActionToDo = a_JUMP;
			}
            
			auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);

			for(int dir=0;dir<3;dir++) {//only once is enough?
			    _Remove(myframe,PENG_EFFECT_NODE_ID+dir);
                
				if(ifEffectTime) {
					ifEffectTime=false;
					ifUpdateDuringEffect=false;
                    
					_roundManager->_curEffectCardKind=ck_NOT_DEFINED;
					_roundManager->_curEffectCardStatus=c_NOT_DEFINDED;
					delete_ActionEffect();
                    
					card_list_update(1);
				}
			}

            Button *button = (Button*)pSender;
            button->_ID=1;
			button->setTouchEnabled(false);
			auto hideButton = TargetedAction::create(button,ScaleTo::create(0,0));

			myframe->_ID=1;

			auto icon = (Sprite*)myframe->getChildByTag(MING_REMIND_ACT_BKG_TAG_ID);
			auto iconShade = TargetedAction::create(icon,Sequence::create(
                ScaleTo::create(0,1),Spawn::create(
                FadeOut::create(0.3),
                ScaleTo::create(0.3,1.3),NULL),NULL));

			auto clickEffect = Spawn::create(hideButton,iconShade,NULL);

			_eventDispatcher->removeEventListenersForTarget(myframe,true);
            
			auto clearTips = CCCallFunc::create(this,callfunc_selector(NetRaceLayer::delete_act_tip));
            
			KouCardsCheck(1);
            if(Kou_kindLen>0) {
				myframe->runAction(Sequence::create(
                    clickEffect,
                    clearTips, CallFunc::create([=](){
                    ming_kou_Choose(1);}),NULL));
            } else {
				myframe->runAction(Sequence::create(
                    clickEffect,
                    clearTips, CallFuncN::create(this,callfuncN_selector(
                    NetRaceLayer::ming_tip_effect)),NULL));
            }
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		((Button*)pSender)->setScale(1);
		break;
	default:
		break;
	}
}
void NetRaceLayer::first_response(int no)
{
    LOGGER_WRITE("%s from zhuang(%d)",__FUNCTION__,no);
    _roundManager->WaitForAction();

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("BlockOtherImage.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("gametrayImage.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("MingTips.plist");

	waitfor_MyTouchShowCard();

	ifGameStart=true;
    
	((Button*)this->getChildByTag(MENU_BKG_TAG_ID)->getChildByTag(TUOGUAN_MENU_BUTTON))->setTouchEnabled(true);
	_roundManager->_actionToDo=_roundManager->_players[no]->get_parter()->ActiontodoCheckAgain();

	if(no==1)
		waitfor_myaction(no);
	else
		waitfor_otheraction(no);
}

void NetRaceLayer::waitfor_myaction(int no)
{
    LOGGER_WRITE("%s : %d, _roundManager->_actionToDo = %d",__FUNCTION__,no,_roundManager->_actionToDo);

	float x,y;
	auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
	//_eventDispatcher->removeEventListenersForTarget(myframe,true);//????
	if(myframe->getChildByTag(TING_SING_BAR))
		myframe->getChildByTag(TING_SING_BAR)->setVisible(false);
	//if(myframe->getChildByTag(TING_SING_BUTTON))
	//{
	//	ifTriangleHintEnable=false;
	//	myframe->getChildByTag(TING_SING_BUTTON)->setVisible(false);
	//}

	y=origin.y+visibleSize.height*0.25;
	x=origin.x+visibleSize.width*0.85;
	auto act_qi = Sprite::createWithSpriteFrameName("qi.png");//弃牌
	auto act_hu =Sprite::createWithSpriteFrameName("hu1.png");//胡牌	
	auto act_ming = Sprite::createWithSpriteFrameName("ming.png");//明牌
	auto act_gang = Sprite::createWithSpriteFrameName("gang1.png");//杠牌
	auto act_peng = Sprite::createWithSpriteFrameName("peng1.png");//碰牌

	if(_roundManager->_actionToDo!=a_JUMP)
	{
		auto myact_qi=Button::create("qi.png","qi.png","qi.png",UI_TEX_TYPE_PLIST);
		myact_qi->_ID=no;
		myact_qi->addTouchEventListener(CC_CALLBACK_2(NetRaceLayer::QiPressed,this));
		myact_qi->setAnchorPoint(Vec2(0.5,0.5));
		myact_qi->setPosition(Vec2(x,y));
		myframe->addChild(myact_qi,35,QI_REMIND_ACT_TAG_ID);

		auto qi_act=Sprite::createWithSpriteFrameName("qi.png");
		qi_act->setAnchorPoint(Vec2(0.5,0.5));
		qi_act->setOpacity(150);
		qi_act->setPosition(Vec2(x,y));
		qi_act->setScale(0);
		myframe->addChild(qi_act,34,QI_REMIND_ACT_BKG_TAG_ID);
		x=x-act_qi->getContentSize().width/2-36;
	}
	if(_roundManager->_actionToDo&a_HU)//胡牌
	{	
		x=x-act_hu->getContentSize().width/2;
		auto myact_hu=Button::create("hu1.png","hu1.png","hu1.png",UI_TEX_TYPE_PLIST);
		myact_hu->_ID=no;
		myact_hu->addTouchEventListener(CC_CALLBACK_2(NetRaceLayer::HuPressed,this));
		myact_hu->setAnchorPoint(Vec2(0.5,0.5));
		myact_hu->setPosition(Vec2(x,y+6));
		myframe->addChild(myact_hu,35,HU_REMIND_ACT_TAG_ID);
		auto hu1_act=Sprite::createWithSpriteFrameName("hu1.png");
		hu1_act->setAnchorPoint(Vec2(0.5,0.5));
		hu1_act->setPosition(Vec2(x,y+6));	
		hu1_act->setOpacity(150);
		hu1_act->setScale(0);
		myframe->addChild(hu1_act,34,HU_REMIND_ACT_BKG_TAG_ID);
		x=x-act_hu->getContentSize().width/2-18;
	}
	if(_roundManager->_actionToDo&a_MING)//明牌
	{
		x=x-act_ming->getContentSize().width/2;
		auto myact_ming=Button::create("ming.png","ming.png","ming.png",UI_TEX_TYPE_PLIST);
		myact_ming->addTouchEventListener(CC_CALLBACK_2(NetRaceLayer::MingPressed,this));
		myact_ming->setAnchorPoint(Vec2(0.5,0.5));
		myact_ming->setPosition(Vec2(x,y+11));
		myframe->addChild(myact_ming,35,MING_REMIND_ACT_TAG_ID);

		auto ming_act=Sprite::createWithSpriteFrameName("ming.png");
		ming_act->setAnchorPoint(Vec2(0.5,0.5));
		ming_act->setPosition(Vec2(x,y+11));	
		ming_act->setOpacity(150);
		ming_act->setScale(0);
		myframe->addChild(ming_act,34,MING_REMIND_ACT_BKG_TAG_ID);
		x=x-act_ming->getContentSize().width/2-18;
	}
	if(_roundManager->_actionToDo&a_AN_GANG || _roundManager->_actionToDo&a_MING_GANG||_roundManager->_actionToDo&a_SHOU_GANG)//暗杠和明杠,手杠
	{
		x=x-act_gang->getContentSize().width/2;
		auto myact_gang=Button::create("gang1.png","gang1.png","gang1.png",UI_TEX_TYPE_PLIST);
		myact_gang->addTouchEventListener(CC_CALLBACK_2(NetRaceLayer::GangPressed,this));
		myact_gang->setAnchorPoint(Vec2(0.5,0.5));
		myact_gang->setPosition(Vec2(x,y));
		myframe->addChild(myact_gang,35,GANG_REMING_ACT_TAG_ID);

		auto gang1_act=Sprite::createWithSpriteFrameName("gang1.png");
		gang1_act->setAnchorPoint(Vec2(0.5,0.5));
		gang1_act->setPosition(Vec2(x,y));	
		gang1_act->setOpacity(150);
		gang1_act->setScale(0);
		myframe->addChild(gang1_act,34,GANG_REMING_ACT_BKG_TAG_ID);
		x=x-act_gang->getContentSize().width/2-18;
	}
	if(_roundManager->_actionToDo&a_PENG)//碰牌
	{		
		x=x-act_peng->getContentSize().width/2;
		auto myact_peng=Button::create("peng1.png","peng1.png","peng1.png",UI_TEX_TYPE_PLIST);
		myact_peng->addTouchEventListener(CC_CALLBACK_2(NetRaceLayer::PengPressed,this));
		myact_peng->setAnchorPoint(Vec2(0.5,0.5));
		myact_peng->setPosition(Vec2(x,y+13));
		myframe->addChild(myact_peng,35,PENG_REMIND_ACT_TAG_ID);

		auto peng1_act=Sprite::createWithSpriteFrameName("peng1.png");
		peng1_act->setAnchorPoint(Vec2(0.5,0.5));
		peng1_act->setPosition(Vec2(x,y+13));	
		peng1_act->setOpacity(150);
		peng1_act->setScale(0);
		myframe->addChild(peng1_act,34,PENG_REMIND_ACT_BKG_TAG_ID);
		x=x-act_peng->getContentSize().width/2-18;
	}
	if(_roundManager->_actionToDo!=a_JUMP)
	{
		_roundManager->_isWaitDecision = true;
		_roundManager->_tempActionToDo=_roundManager->_actionToDo;
		_roundManager->_actionToDo=a_JUMP;
	}
	if(!_roundManager->_isCardFromOthers)//||(_roundManager->_isCardFromOthers&&_roundManager->_lastAction!=a_JUMP))
	{
		if(_roundManager->_lastAction==a_JUMP&&!(_roundManager->_lastActionSource==1&&continue_gang_times!=0))
			continue_gang_times=0;
		_roundManager->_lastAction=a_JUMP;
		waitfor_MyShowCardInstruct();
	}
}

void NetRaceLayer::waitfor_otheraction(int no)
{
    LOGGER_WRITE("%s (%d) perform action %d",__FUNCTION__,no,_roundManager->_actionToDo);

	auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
	myframe->_ID=no;
	
	if(_roundManager->_actionToDo&a_HU)
	{
		hu_effect_tip(no);
	}
	else if(_roundManager->_actionToDo&a_AN_GANG||_roundManager->_actionToDo&a_SHOU_GANG)
	{
		auto callFunc=CCCallFuncN::create(this,callfuncN_selector(NetRaceLayer::an_gang_tip_effect));
		myframe->runAction(callFunc);
	}
	else if(_roundManager->_actionToDo&a_MING_GANG)
	{
		auto callFunc=CCCallFuncN::create(this,callfuncN_selector(NetRaceLayer::ming_gang_tip_effect));
		myframe->runAction(callFunc);
	}
	else if(_roundManager->_actionToDo&a_MING)
	{
		auto callFunc=CCCallFuncN::create(this,callfuncN_selector(NetRaceLayer::ming_tip_effect));
		myframe->runAction(callFunc);
	}
	else if(_roundManager->_actionToDo&a_PENG)
	{
		auto callFunc=CCCallFuncN::create(this,callfuncN_selector(NetRaceLayer::PengEffect));
		myframe->runAction(callFunc);
	}
	else if(_roundManager->_actionToDo==a_JUMP)
	{
		if(_roundManager->_lastAction==a_JUMP)
			continue_gang_times=0;
		_roundManager->_lastAction=a_JUMP;
		waitfor_ShowCardWithoutTouch();
	}
}

void NetRaceLayer::waitfor_response(Node* sender)
{
    LOGGER_WRITE("%s",__FUNCTION__);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("race3.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("race4.plist");
	unsigned char curTingStatus=_roundManager->_players[sender->_ID]->get_parter()->get_ting_status();
	if(!_roundManager->_isCardFromOthers)
	{
		_roundManager->_isGangHua=false;
		if(_roundManager->_lastActionSource==sender->_ID&&continue_gang_times!=0)
			_roundManager->_isGangHua=true;
		else
			continue_gang_times=0;
        
		_roundManager->_actionToDo = 
            _roundManager->_players[sender->_ID]->get_parter()->hand_in(
                _roundManager->_lastHandedOutCard,
                _roundManager->_isCardFromOthers,
                curTingStatus,
                (_roundManager->_distributedNum==TOTAL_CARD_NUM),
                _roundManager->_lastActionWithGold,
                continue_gang_times,
                _roundManager->_isGangHua
            );
        
		if(sender->_ID==1)
		{
			auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
			float x=base_point[1].x;
			float y=base_point[1].y;
			if(_roundManager->_players[1]->get_parter()->get_ting_status()==1)
			{
				x += myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+_roundManager->_curPlayer*20+(_roundManager->_players[1]->get_parter()->get_card_list()->len-2))->getPosition().x+30;
				y += 60+13;
			}
			else
			{
				x += myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+_roundManager->_curPlayer*20+(_roundManager->_players[1]->get_parter()->get_card_list()->len-2))->getPosition().x+30;
				y += 60;
			}
		}
		if(_roundManager->_players[sender->_ID]->get_parter()->get_role_type()==SINGLE_BOARD_ROBOT)
		{
			if(_roundManager->_players[sender->_ID]->get_robot_hu_target()==SAME_TIAO_TARGET)
			{
				if(_roundManager->_lastHandedOutCard/9!=0&&!(_roundManager->_actionToDo&a_HU)&&!(_roundManager->_actionToDo&a_AN_GANG)&&!(_roundManager->_actionToDo&a_SHOU_GANG)&&!(_roundManager->_actionToDo&a_MING_GANG))
					_roundManager->_actionToDo=a_JUMP;
			}
			else if(_roundManager->_players[sender->_ID]->get_robot_hu_target()==SAME_TONG_TARGET)
			{
				if(_roundManager->_lastHandedOutCard/9!=1&&!(_roundManager->_actionToDo&a_HU)&&!(_roundManager->_actionToDo&a_AN_GANG)&&!(_roundManager->_actionToDo&a_SHOU_GANG)&&!(_roundManager->_actionToDo&a_MING_GANG))
					_roundManager->_actionToDo=a_JUMP;
			}
			else if(_roundManager->_players[sender->_ID]->get_robot_hu_target()==SEVEN_COUPLES_TARGET)
				if(!(_roundManager->_actionToDo&a_HU)&&!(_roundManager->_actionToDo&a_AN_GANG)&&!(_roundManager->_actionToDo&a_SHOU_GANG)&&!(_roundManager->_actionToDo&a_MING_GANG))
					_roundManager->_actionToDo=a_JUMP;
		}
		if(_roundManager->_players[sender->_ID]->get_parter()->get_role_type()==INTERNET_PLAYER)
		{
            LOGGER_WRITE("NETWORK : NetPlayer action here, %s %d",__FILE__,__LINE__);
			if(_roundManager->_players[sender->_ID]->get_robot_hu_target()==SAME_TIAO_TARGET)
			{
				if(_roundManager->_lastHandedOutCard/9!=0&&!(_roundManager->_actionToDo&a_HU)&&!(_roundManager->_actionToDo&a_AN_GANG)&&!(_roundManager->_actionToDo&a_SHOU_GANG)&&!(_roundManager->_actionToDo&a_MING_GANG))
					_roundManager->_actionToDo=a_JUMP;
			}
			else if(_roundManager->_players[sender->_ID]->get_robot_hu_target()==SAME_TONG_TARGET)
			{
				if(_roundManager->_lastHandedOutCard/9!=1&&!(_roundManager->_actionToDo&a_HU)&&!(_roundManager->_actionToDo&a_AN_GANG)&&!(_roundManager->_actionToDo&a_SHOU_GANG)&&!(_roundManager->_actionToDo&a_MING_GANG))
					_roundManager->_actionToDo=a_JUMP;
			}
			else if(_roundManager->_players[sender->_ID]->get_robot_hu_target()==SEVEN_COUPLES_TARGET)
				if(!(_roundManager->_actionToDo&a_HU)&&!(_roundManager->_actionToDo&a_AN_GANG)&&!(_roundManager->_actionToDo&a_SHOU_GANG)&&!(_roundManager->_actionToDo&a_MING_GANG))
					_roundManager->_actionToDo=a_JUMP;
		}
		if(sender->_ID==1)
		{
			if(_roundManager->_players[1]->get_parter()->get_ting_status()==1&&(_roundManager->_actionToDo&a_HU))
			{
				auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
				myframe->_ID=sender->_ID;
				auto huCallFunc=CallFunc::create([=](){hu_effect_tip(1);});
				myframe->runAction(huCallFunc);
			}
			else
			{
				if(ifTuoGuan)
					_roundManager->_actionToDo=a_JUMP;
				waitfor_myaction(sender->_ID);
				return;
			}
		}
		else
		{
			waitfor_otheraction(sender->_ID);
			return;
		}
	}
	else
	{
		int no=(sender->_ID+1)%3;
		unsigned char action1 = 
            _roundManager->_players[no]->get_parter()->hand_in(
                _roundManager->_lastHandedOutCard,
                _roundManager->_isCardFromOthers,
                curTingStatus,
                (_roundManager->_distributedNum==TOTAL_CARD_NUM),
                _roundManager->_lastActionWithGold,
                continue_gang_times,
                _roundManager->_isGangHua
            );
        
		if(no==1&&ifTuoGuan)
		{
			if(_roundManager->_players[1]->get_parter()->get_ting_status()==1&&(action1&a_HU))
				action1=a_HU;
			else
				action1=a_JUMP;
		}
		if(_roundManager->_players[no]->get_parter()->get_role_type()==SINGLE_BOARD_ROBOT)
		{
			if(_roundManager->_players[no]->get_robot_hu_target()==SAME_TIAO_TARGET)
			{
				if(_roundManager->_lastHandedOutCard/9!=0&&!(action1&a_HU)&&!(action1&a_AN_GANG)&&!(action1&a_SHOU_GANG)&&!(action1&a_MING_GANG))
					action1 = a_JUMP;
			}
			else if(_roundManager->_players[no]->get_robot_hu_target()==SAME_TONG_TARGET)
			{
				if(_roundManager->_lastHandedOutCard/9!=1&&!(action1&a_HU)&&!(action1&a_AN_GANG)&&!(action1&a_SHOU_GANG)&&!(action1&a_MING_GANG))
					action1 = a_JUMP;
			}
			else if(_roundManager->_players[no]->get_robot_hu_target()==SEVEN_COUPLES_TARGET)
				if(!(action1&a_HU)&&!(action1&a_AN_GANG)&&!(action1&a_SHOU_GANG)&&!(action1&a_MING_GANG))
					action1=a_JUMP;
		}
		if(_roundManager->_players[no]->get_parter()->get_role_type()==INTERNET_PLAYER)
		{
            LOGGER_WRITE("NETWORK : NetPlayer action here, %s %d",__FILE__,__LINE__);
			if(_roundManager->_players[no]->get_robot_hu_target()==SAME_TIAO_TARGET)
			{
				if(_roundManager->_lastHandedOutCard/9!=0&&!(action1&a_HU)&&!(action1&a_AN_GANG)&&!(action1&a_SHOU_GANG)&&!(action1&a_MING_GANG))
					action1 = a_JUMP;
			}
			else if(_roundManager->_players[no]->get_robot_hu_target()==SAME_TONG_TARGET)
			{
				if(_roundManager->_lastHandedOutCard/9!=1&&!(action1&a_HU)&&!(action1&a_AN_GANG)&&!(action1&a_SHOU_GANG)&&!(action1&a_MING_GANG))
					action1 = a_JUMP;
			}
			else if(_roundManager->_players[no]->get_robot_hu_target()==SEVEN_COUPLES_TARGET)
				if(!(action1&a_HU)&&!(action1&a_AN_GANG)&&!(action1&a_SHOU_GANG)&&!(action1&a_MING_GANG))
					action1=a_JUMP;
		}
		int no1=(sender->_ID+2)%3;
		unsigned char action2=
            _roundManager->_players[no1]->get_parter()->hand_in(
                _roundManager->_lastHandedOutCard,
                _roundManager->_isCardFromOthers,
                curTingStatus,
                (_roundManager->_distributedNum==TOTAL_CARD_NUM),
                _roundManager->_lastActionWithGold,
                continue_gang_times,
                _roundManager->_isGangHua
            );
		if(no1==1&&ifTuoGuan)
		{
			if(_roundManager->_players[1]->get_parter()->get_ting_status()==1&&(action1&a_HU))
				action2=a_HU;
			else
				action2=a_JUMP;
		}
		if(_roundManager->_players[no1]->get_parter()->get_role_type()==SINGLE_BOARD_ROBOT)
		{
			if(_roundManager->_players[no1]->get_robot_hu_target()==SAME_TIAO_TARGET)
			{
				if(_roundManager->_lastHandedOutCard/9!=0&&!(action2&a_HU)&&!(action2&a_AN_GANG)&&!(action2&a_SHOU_GANG)&&!(action2&a_MING_GANG))
					action2 = a_JUMP;
			}
			else if(_roundManager->_players[no1]->get_robot_hu_target()==SAME_TONG_TARGET)
			{
				if(_roundManager->_lastHandedOutCard/9!=1&&!(action2&a_HU)&&!(action2&a_AN_GANG)&&!(action2&a_SHOU_GANG)&&!(action2&a_MING_GANG))
					action2 = a_JUMP;
			}
			else if(_roundManager->_players[no1]->get_robot_hu_target()==SEVEN_COUPLES_TARGET)
				if(!(action2&a_HU)&&!(action2&a_AN_GANG)&&!(action2&a_SHOU_GANG)&&!(action2&a_MING_GANG))
					action2=a_JUMP;
		}
		if(_roundManager->_players[no1]->get_parter()->get_role_type()==INTERNET_PLAYER)
		{
            LOGGER_WRITE("NETWORK : NetPlayer action here, %s %d",__FILE__,__LINE__);
			if(_roundManager->_players[no1]->get_robot_hu_target()==SAME_TIAO_TARGET)
			{
				if(_roundManager->_lastHandedOutCard/9!=0&&!(action2&a_HU)&&!(action2&a_AN_GANG)&&!(action2&a_SHOU_GANG)&&!(action2&a_MING_GANG))
					action2 = a_JUMP;
			}
			else if(_roundManager->_players[no1]->get_robot_hu_target()==SAME_TONG_TARGET)
			{
				if(_roundManager->_lastHandedOutCard/9!=1&&!(action2&a_HU)&&!(action2&a_AN_GANG)&&!(action2&a_SHOU_GANG)&&!(action2&a_MING_GANG))
					action2 = a_JUMP;
			}
			else if(_roundManager->_players[no1]->get_robot_hu_target()==SEVEN_COUPLES_TARGET)
				if(!(action2&a_HU)&&!(action2&a_AN_GANG)&&!(action2&a_SHOU_GANG)&&!(action2&a_MING_GANG))
					action2=a_JUMP;
		}
		if((action1&a_HU)&&(action2&a_HU))//双响
		{
            HideClock();
			if((no!=1&&no1!=1)||(no==1||no1==1&&_roundManager->_players[1]->get_parter()->get_ting_status()==1))
			{
				hu_effect_tip(3);
				distribute_event(DOUBLE_HU_WITH_ME,NULL);
			}
			else if((no==1||no1==1)&&_roundManager->_players[1]->get_parter()->get_ting_status()==0)
			{
				_roundManager->_isDoubleHuAsking = true;
				if(no==1)
				{
					_roundManager->_otherOneForDouble = no1;
					_roundManager->_actionToDo=action1;
				}
				else
				{
					_roundManager->_otherOneForDouble = no;
					_roundManager->_actionToDo=action2;
				}					
				waitfor_myaction(1);
				return;
			}
		}
		else if(action1&a_HU||action2&a_HU)//点炮
		{
            HideClock();
			if((no==1&&(action1&a_HU))||(no1==1&&(action2&a_HU)))
			{
				if(_roundManager->_players[1]->get_parter()->get_ting_status()==1)//&&_roundManager->_actionToDo&a_HU)
				{
					auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
					myframe->_ID=1;
					auto huCallFunc=CallFunc::create([=](){hu_effect_tip(1);});
					myframe->runAction(huCallFunc);
				}
				else
				{
					if(no==1)
						_roundManager->_actionToDo=action1;
					else
						_roundManager->_actionToDo=action2;
					waitfor_myaction(1);
					return;
				}
			}
			else if(no!=1&&(action1&a_HU))
				hu_effect_tip(no);
			else if(no1!=1&&(action2&a_HU))
				hu_effect_tip(no1);
		}
		else if(action1!=a_JUMP)//下家
		{
			_roundManager->_actionToDo=action1;
			if(no==1)
			{
				UpdateClock(0,no);
				waitfor_myaction(no);
				return;
			}
			else
			{
				UpdateClock(0,no);
				waitfor_otheraction(no);
				return;
			}
		}
		else if(action2!=a_JUMP)//上家
		{
			_roundManager->_actionToDo=action2;
			if(no1==1)
			{
				UpdateClock(0,no1);
				waitfor_myaction(no1);
				return;
			}
			else
			{
				UpdateClock(0,no1);
				waitfor_otheraction(no1);
				return;
			}
		}
		else if(action1==action2&&action1==a_JUMP)
		{
			_roundManager->_actionToDo=action1;
			_roundManager->_curPlayer=(_roundManager->_curPlayer+1)%3;
			UpdateClock(0,_roundManager->_curPlayer);
			call_distribute_card();
		}
	}
}

void NetRaceLayer::distribute_card_effect()
{
    LOGGER_WRITE("%s",__FUNCTION__);

	auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
	auto list=_roundManager->_players[_roundManager->_curPlayer]->get_parter()->get_card_list();
	Sprite *list_last_one;
	float x=base_point[_roundManager->_curPlayer].x;
	float y=base_point[_roundManager->_curPlayer].y;
	unsigned char ting_flag=_roundManager->_players[_roundManager->_curPlayer]->get_parter()->get_ting_status();
	if(_roundManager->_curPlayer==1)
	{
		list_last_one=Sprite::createWithTexture(g_my_free->getTexture());
		list_last_one->setAnchorPoint(Point(0.0f,0.0f));
		if(ting_flag==1)
		{
			x += myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+_roundManager->_curPlayer*20+(list->len-1))->getPosition().x+30;
			y += 60+13;
		}
		else
		{
			x += myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+_roundManager->_curPlayer*20+(list->len-1))->getPosition().x+30;
			y += 60;
		}
		if( _roundManager->_lastActionSource==1&& 
            (_roundManager->_lastAction==a_AN_GANG||_roundManager->_lastAction==a_SHOU_GANG||_roundManager->_lastAction==a_MING_GANG) )
			x=distributeCardPos.x;
		auto s_card=Sprite::createWithTexture(g_card_kind[_roundManager->_lastHandedOutCard]->getTexture());
		s_card->setPosition(Vec2(list_last_one->getTextureRect().size.width/2,list_last_one->getTextureRect().size.height*0.4));
		list_last_one->addChild(s_card);
	}
	else if(_roundManager->_curPlayer==2)
	{
		list_last_one=Sprite::createWithTexture(g_right_free->getTexture());
		list_last_one->setAnchorPoint(Point(0,0));
		y=myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+_roundManager->_curPlayer*20+(list->len-1))->getPosition().y+86;
	}
	else
	{
		list_last_one=Sprite::createWithTexture(g_left_free->getTexture());
		list_last_one->setAnchorPoint(Point(0.0f,1.0f));
		y=myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+_roundManager->_curPlayer*20+(list->len-2))->getPosition().y-20;//+5;
	}
	list_last_one->setPosition(Vec2(x,y));
	update_residue_cards(TOTAL_CARD_NUM - _roundManager->_distributedNum);

	if(_roundManager->_curPlayer==0)
		myframe->addChild(list_last_one,30,HAND_IN_CARDS_TAG_ID+_roundManager->_curPlayer*20+(list->len));
	else if(_roundManager->_curPlayer==1)
		myframe->addChild(list_last_one,30,HAND_IN_CARDS_TAG_ID+_roundManager->_curPlayer*20+(list->len));
	else if(_roundManager->_curPlayer==2)
		myframe->addChild(list_last_one,0,HAND_IN_CARDS_TAG_ID+_roundManager->_curPlayer*20+(list->len));

	auto action1=MoveTo::create(0.3,Vec2(list_last_one->getPositionX(),list_last_one->getPositionY()-50));
	auto seq=Sequence::create(EaseElasticOut::create(action1),NULL);
	auto last_one_action=TargetedAction::create(list_last_one,seq);

	//auto callFunc=CallFuncN::create(this,callfuncN_selector(NetRaceLayer::update_card_list));
	auto callFunc1=CCCallFuncN::create(this,callfuncN_selector(NetRaceLayer::waitfor_response));
	Sequence* seq2;
	myframe->_ID=_roundManager->_curPlayer;
	if(_roundManager->_curPlayer!=1)
		seq2=Sequence::create(last_one_action,DelayTime::create(0.5),callFunc1,NULL);
	else
		seq2=Sequence::create(last_one_action,callFunc1,NULL);
	myframe->runAction(seq2);
}

void NetRaceLayer::call_distribute_card()
{
    LOGGER_WRITE("%s",__FUNCTION__);
    _roundManager->WaitForDistribute();
    
	distribute_event(DISTRIBUTE_CALL_EVENT_TYPE,NULL);
}

void NetRaceLayer::distribute_card()
{
    LOGGER_WRITE("%s",__FUNCTION__);

    LOGGER_WRITE("\tadd listener to %s",DISTRIBUTE_DONE_EVENT_TYPE);
	auto _distributedoneListener = EventListenerCustom::create(DISTRIBUTE_DONE_EVENT_TYPE, [this](EventCustom * event){
		auto userData = static_cast<DCI*>(event->getUserData());
		_roundManager->_lastHandedOutCard = userData->card;
		_roundManager->_distributedNum    = userData->num;
        _roundManager->_isCardFromOthers = false;
		distribute_card_effect();
	});
	_eventDispatcher->addEventListenerWithFixedPriority(_distributedoneListener,2);

    LOGGER_WRITE("\tadd listener to %s",NOONE_WIN_EVENT_TYPE);
	auto _noonewinListener = EventListenerCustom::create(NOONE_WIN_EVENT_TYPE, [this](EventCustom * event){

		((Button*)this->getChildByTag(MENU_BKG_TAG_ID)->getChildByTag(TUOGUAN_MENU_BUTTON))->setTouchEnabled(false);
		if(this->getChildByTag(ROBOT_TUO_GUAN))
			this->removeChildByTag(ROBOT_TUO_GUAN,true);	
		if(this->getChildByTag(TUOGUAN_CANCEL_BUTTON))
			this->removeChildByTag(TUOGUAN_CANCEL_BUTTON,true);
		//流局特效放在这里，结算也在这里，特效最后一个action dispatch WAIT_START_CALLBACK_EVENT_TYPE
		auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
        _roundManager->SetWin(NONE_WIN,_roundManager->_curPlayer);

		auto callfunc=CallFunc::create(this,callfunc_selector(NetRaceLayer::showall));
		auto Nowinner=CallFunc::create([=](){
			auto layer_color=LayerColor::create();
			layer_color->setColor(Color3B(0,0,0));
			//layer_color->setColor(Color3B(138,151,123));
			layer_color->ignoreAnchorPointForPosition(false);
			layer_color->setAnchorPoint(Vec2(0,0));
			layer_color->setPosition(Vec2(origin.x,origin.y));
			layer_color->setContentSize(Size(visibleSize.width,visibleSize.height));
			layer_color->setOpacity(150);
			this->addChild(layer_color,10,HUANG_ZHUANG_LAYER);

			auto HuangZhuang=Sprite::create("huangzhuang.png");
			HuangZhuang->setAnchorPoint(Vec2(0.5,0.5));
			HuangZhuang->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
			HuangZhuang->setScale(1);
			this->addChild(HuangZhuang,10,HUANG_ZHUANG_FONT);
			auto hzAction=FadeIn::create(0.5);
			auto FontAction=TargetedAction::create(HuangZhuang,hzAction);
			//HuangZhuang->runAction(FadeIn::create(0.5));
		});
		auto GoldAccount=CallFunc::create([=](){
			if(_roundManager->_firstMingNo!=-1)
				GoldNumInsert(4,3,_roundManager->_firstMingNo);
		});
		scheduleOnce(schedule_selector(NetRaceLayer::raceAccount),3);
		this->runAction(Sequence::create(DelayTime::create(0.5),GoldAccount,DelayTime::create(1),Spawn::create(Nowinner,callfunc,NULL),NULL));
	});
	_eventDispatcher->addEventListenerWithFixedPriority(_noonewinListener,3);
}
void NetRaceLayer::start_touchCallBack(Ref* pSender,ui::Widget::TouchEventType type)
{
    _roundManager->NotifyStart();

	auto curButton=(Button*)pSender;
	auto VoiceEffect=CallFunc::create([=](){
		SimpleAudioEngine::sharedEngine()->playEffect("Music/anniu.ogg");	
	});
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			curButton->setTouchEnabled(false);
			auto ShadEffect=Sprite::createWithSpriteFrameName("kaishiyouxi2.png");
			ShadEffect->setAnchorPoint(Vec2(0.5,0.5));
			ShadEffect->setOpacity(77);
			ShadEffect->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
			this->addChild(ShadEffect,2,START_GAME_TAG_ID+1);
			auto action1=Sequence::create(DelayTime::create(0.1),FadeOut::create(0.2),NULL);
			auto TargetAction1=TargetedAction::create(curButton,action1);
			auto action2=Spawn::create(ScaleTo::create(0.3,1.3),FadeOut::create(0.3),NULL);
			auto TargetAction2=TargetedAction::create(ShadEffect,action2);
			auto startGame=CallFunc::create(this,callfunc_selector(NetRaceLayer::start_callback));
			this->runAction(Sequence::create(Spawn::create(TargetAction1,TargetAction2,VoiceEffect,NULL),DelayTime::create(0.2),startGame,NULL));
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void NetRaceLayer::restart_touchCallBack(Ref* pSender,ui::Widget::TouchEventType type)
{
    LOGGER_WRITE("%s",__FUNCTION__);

	auto curButton=(Button*)pSender;
	auto VoiceEffect=CallFunc::create([=](){
		SimpleAudioEngine::sharedEngine()->playEffect("Music/anniu.ogg");	
	});
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		{
			((Button*)this->getChildByTag(MENU_BKG_TAG_ID)->getChildByTag(GAMEBACK_MENU_BUTTON))->setTouchEnabled(true);
			((Button*)this->getChildByTag(MENU_BKG_TAG_ID)->getChildByTag(TUOGUAN_MENU_BUTTON))->setHighlighted(false);
            
            refresh_residue_cards();
			update_residue_cards(84);
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			curButton->setTouchEnabled(false);
			auto ShadEffect=Sprite::createWithSpriteFrameName("zailaiyiju2.png");
			ShadEffect->setAnchorPoint(Vec2(0.5,0.5));
			ShadEffect->setOpacity(77);
			ShadEffect->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
			this->addChild(ShadEffect,2,RACE_RESTART_TAG_ID+1);
			//auto action1=Sequence::create(DelayTime::create(0.1),FadeOut::create(0.2),NULL);
			//auto TargetAction1=TargetedAction::create(curButton,action1);
			auto action2=Spawn::create(ScaleTo::create(0.3,1.3),FadeOut::create(0.3),NULL);
			auto TargetAction2=TargetedAction::create(ShadEffect,action2);
			auto startGame=CallFunc::create(this,callfunc_selector(NetRaceLayer::start_callback));
			this->runAction(Sequence::create(Spawn::create(/*TargetAction1,*/TargetAction2,VoiceEffect,NULL),DelayTime::create(0.2),startGame,NULL));
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}
void NetRaceLayer::start_callback()
{
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("tileImage.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("TimerImage.plist");
	if(!ifResourcePrepared)
	{
		ifResourcePrepared=true;
		resource_prepare();
	}
	if(this->getChildByTag(GAME_BKG_TAG_ID))
		this->removeChildByTag(GAME_BKG_TAG_ID,true);
	if(this->getChildByTag(START_GAME_TAG_ID))
		this->removeChildByTag(START_GAME_TAG_ID,true);
	if(this->getChildByTag(START_GAME_TAG_ID+1))
		this->removeChildByTag(START_GAME_TAG_ID+1,true);
	if(this->getChildByTag(RACE_RESTART_TAG_ID))
		this->removeChildByTag(RACE_RESTART_TAG_ID,true);
	if(this->getChildByTag(RACE_RESTART_TAG_ID+1))
		this->removeChildByTag(RACE_RESTART_TAG_ID+1,true);
	if(this->getChildByTag(SHINE_TAG_ID))
		this->removeChildByTag(SHINE_TAG_ID,true);
	if(this->getChildByTag(SHINE_TAG_ID+1))
		this->removeChildByTag(SHINE_TAG_ID+1,true);
	if(this->getChildByTag(RACE_ACCOUT_TAG_ID))
		this->removeChildByTag(RACE_ACCOUT_TAG_ID,true);
	while(this->getChildByTag(HUANG_ZHUANG_LAYER))
		this->removeChildByTag(HUANG_ZHUANG_LAYER,true);
	while(this->getChildByTag(HUANG_ZHUANG_FONT))
		this->removeChildByTag(HUANG_ZHUANG_FONT,true);
	if(this->getChildByTag(MING_STATUS_PNG_0))
		this->getChildByTag(MING_STATUS_PNG_0)->setVisible(false);
	if(this->getChildByTag(MING_STATUS_PNG_1))
		this->getChildByTag(MING_STATUS_PNG_1)->setVisible(false);
	if(this->getChildByTag(MING_STATUS_PNG_2))
		this->getChildByTag(MING_STATUS_PNG_2)->setVisible(false);
	for(int a=0;a<4;a++)
	{
		while(this->getChildByTag(GOLD_NUM_INSERT_JINBI+a))
			this->removeChildByTag(GOLD_NUM_INSERT_JINBI+a,true);
	}
    
    _roundManager->RenewOutCard();
    
    distribute_event(WAIT_START_CALLBACK_EVENT_TYPE,NULL);

	//SimpleAudioEngine::sharedEngine()->preloadEffect("Music/duijukaishi.ogg");

	auto myframe = LayerColor::create(Color4B(0,0,0,0),visibleSize.width,visibleSize.height);
	myframe->setPosition(Vec2(origin.x, origin.y));
	this->addChild(myframe,3,GAME_BKG_TAG_ID);

	distributeCardPos=Vec2::ZERO;
	ifGameStart=false;
	ifUpdateDuringEffect=false;
	//ifTriangleHintEnable=false;
	ifMingMybeError=false;
	MyCardChoosedNum=-1;
	ifInsertStopped=false;
	ifInsertCardsTime=false;
	//ifEndGameChoose=false;
	ifMyShowCardTime=false;
	ifEffectTime=false;
	ifMingTime=false;
	//ifMyTime=false;
	_roundManager->_lastActionSource=-1;
	ifTuoGuan=false;
	_roundManager->_isGangHua=false;
	_roundManager->_lastAction=a_JUMP;
	_roundManager->_lastActionWithGold=a_JUMP;
	_roundManager->_actionToDo=a_JUMP;
	continue_gang_times=0;

    int lastWinner = _roundManager->GetLastWinner();
	_roundManager->_curPlayer=lastWinner;
	RototHandOutIndex=ck_NOT_DEFINED;
	_roundManager->_qiangGangTargetNo=-1;

    _roundManager->_ready[1]=true;
	GuiShowReady(1);
    _roundManager->NotifyStart();

    _roundManager->WaitUntilAllReady();

	race_begin_prepare();                  //牌局开始效果
    
    _roundManager->_actionToDo = _roundManager->_players[(lastWinner)%3]->init(&(_roundManager->_unDistributedCards[0]),14,aim[lastWinner]);//玩家手牌初始化
	if(_roundManager->_actionToDo!=a_TIMEOUT) {
		_roundManager->_players[(lastWinner+1)%3]->init(&(_roundManager->_unDistributedCards[14]),13,aim[(lastWinner+1)%3]);
		_roundManager->_players[(lastWinner+2)%3]->init(&(_roundManager->_unDistributedCards[27]),13,aim[(lastWinner+2)%3]);
		//SimpleAudioEngine::sharedEngine()->preloadEffect("Music/sort.ogg");
		effect_Distribute_Card(lastWinner);//牌局开始发牌效果。
	}
}
	
void NetRaceLayer::start_dealCardsDelete()
{
    LOGGER_WRITE("%s",__FUNCTION__);

	auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
	for(int a=0;a<3;a++)
	{
		for(int b=0;b<4;b++)
		{
			if(myframe->getChildByTag(START_CARDS_IN_TAG_ID+a*b))
				myframe->removeChildByTag(START_CARDS_IN_TAG_ID+a*b);
		}
	}
}

void NetRaceLayer::effect_Distribute_Card(int zhuang)
{
    LOGGER_WRITE("%s",__FUNCTION__);

	auto VoiceEffect=CallFunc::create([=](){VoiceId=SimpleAudioEngine::sharedEngine()->playEffect("Music/sort.ogg");});	
	auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
	float timeXH[3];
	switch (zhuang)
	{
	case 1:
		timeXH[1]=0.7;
		timeXH[2]=0.9;
		timeXH[0]=1.1;
		break;
	case 2:
		timeXH[1]=1.1;
		timeXH[2]=0.7;
		timeXH[0]=0.9;
		break;
	case 0:
		timeXH[1]=0.9;
		timeXH[2]=1.1;
		timeXH[0]=0.7;
		break;
	default:
		break;
	}
	Sprite* p_list[4];
	Sprite* lastTwo[2];
	
	auto ListOfNo1=_roundManager->_players[1]->get_parter()->get_card_list();

	int a,b;
	float x,y;
	//自己
	auto Mecard_in=Sprite::createWithTexture(g_my_in->getTexture());	
	auto Mehand_in=Sprite::createWithTexture(g_my_free->getTexture());
	auto Card_inSize=Mecard_in->getTextureRect().size.width;
	auto Hand_inSize=Mehand_in->getTextureRect().size.width;
	Sprite* meHandCard[4];
	Vec2 meHandPosition[4];
	for (a=0;a<4;a++)
	{
		meHandCard[a]=Sprite::createWithTexture(g_my_in->getTexture());
		meHandCard[a]->setAnchorPoint(Vec2(0,0.5));
		meHandCard[a]->setPosition(Vec2(origin.x+visibleSize.width/2-Card_inSize*2+Card_inSize*a,origin.y+visibleSize.height*0.5));
		meHandCard[a]->setScale(0);
		myframe->addChild(meHandCard[a],3,START_CARDS_IN_TAG_ID+a);	
		meHandPosition[a]=meHandCard[a]->getPosition();
	}

	Point HandoutEnd=Vec2(base_point[1].x+10,base_point[1].y+110);
	float changingPosition[4];
	for(a=0;a<4;a++)
	{
		for (b=0;b<4;b++)
		{
			changingPosition[b]=HandoutEnd.x;
		}
		y=HandoutEnd.y;
		changingPosition[0]+=Hand_inSize*a;
		changingPosition[1]+=Hand_inSize*(a*2)+Hand_inSize;
		changingPosition[2]+=Hand_inSize*(a*3)+Hand_inSize*2;
		changingPosition[3]+=(Hand_inSize*13+a*Hand_inSize+30);
		auto outTime1=Sequence::create(DelayTime::create(timeXH[1]),ScaleTo::create(0,1),MoveTo::create(0.2f,Vec2(changingPosition[0],y)),VoiceEffect,Spawn::create(ScaleTo::create(0,0),MoveTo::create(0,Vec2(meHandPosition[a].x,meHandPosition[a].y)),NULL),NULL);
		auto outTime2=Sequence::create(DelayTime::create(0.4),ScaleTo::create(0,1),MoveTo::create(0.2f,Vec2(changingPosition[1],y)),VoiceEffect,Spawn::create(ScaleTo::create(0,0),MoveTo::create(0,Vec2(meHandPosition[a].x,meHandPosition[a].y)),NULL),NULL);
		auto outTime3=Sequence::create(DelayTime::create(0.4),ScaleTo::create(0,1),MoveTo::create(0.2f,Vec2(changingPosition[2],y)),VoiceEffect,Spawn::create(ScaleTo::create(0,0),MoveTo::create(0,Vec2(meHandPosition[a].x,meHandPosition[a].y)),NULL),NULL);
		Sequence* outTime4;
		if(zhuang==1&&a<2)
			outTime4=Sequence::create(DelayTime::create(0.4),ScaleTo::create(0,1),MoveTo::create(0.2f,Vec2(changingPosition[3],y)),VoiceEffect,Spawn::create(ScaleTo::create(0,0),MoveTo::create(0,Vec2(meHandPosition[a].x,meHandPosition[a].y)),NULL),NULL);
		else if(zhuang!=1&&a<1)
			outTime4=Sequence::create(DelayTime::create(0.4),ScaleTo::create(0,1),MoveTo::create(0.2f,Vec2(changingPosition[3],y)),VoiceEffect,Spawn::create(ScaleTo::create(0,0),MoveTo::create(0,Vec2(meHandPosition[a].x,meHandPosition[a].y)),NULL),NULL);
		else
			outTime4=Sequence::create(DelayTime::create(0.4),NULL);
		meHandCard[a]->runAction(Sequence::create(outTime1,outTime2,outTime3,outTime4,NULL));
	}
	auto xmeCardStart=base_point[1].x+10;
	auto ymeCardStart=base_point[1].y+10;
	for(a=0;a<4;a++)
	{
		if(a==0)//DealayTime::create(0.9)
		{
			x=base_point[1].x+10;
			y=base_point[1].y+10;
			for(int i=0;i<4;i++)
			{				
				p_list[i]=Sprite::createWithTexture(g_my_free->getTexture());
				p_list[i]->setAnchorPoint(Point(0.0f,0.0f));
				x+=p_list[i]->getTextureRect().size.width*i;
				p_list[i]->setPosition(Vec2(x,y));
				auto s_card=Sprite::createWithTexture(g_card_kind[ListOfNo1->data[0+3*i].kind]->getTexture());
				s_card->setAnchorPoint(Vec2(0.5,0.5));
				s_card->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height*0.4));
				p_list[i]->addChild(s_card);
				p_list[i]->setScale(0);
				myframe->addChild(p_list[i],i+1,HAND_IN_CARDS_TAG_ID+1*20+i);
				auto list_seq0=Sequence::create(DelayTime::create(timeXH[1]+0.2),ScaleTo::create(0,1),NULL);

				x=base_point[1].x+10;
				y=base_point[1].y+10;
				auto cardSize=Mehand_in->getTextureRect().size;
				auto mv1=MoveTo::create(0.2,Vec2(x+cardSize.width*i*2,y));
				auto delay1=DelayTime::create(0.4);
				auto mv2=MoveTo::create(0.2,Vec2(x+cardSize.width*i*3,y));
				auto delay2=DelayTime::create(0.4);
				auto list_seq1=Sequence::create(DelayTime::create(timeXH[1]+0.2),delay1,mv1,delay2,mv2,NULL);
				auto list_spa=Spawn::create(list_seq0,list_seq1,NULL);
				p_list[i]->runAction(list_spa);
			}
		}
		else if(a==1)//DelayTime::create(1.5)
		{
			for(int i=0;i<4;i++)
			{
				x=base_point[1].x+10;
				y=base_point[1].y+10;
				auto cardSize=Mehand_in->getTextureRect().size;
				x += cardSize.width*1.0*i*2+cardSize.width;
				p_list[i]=Sprite::createWithTexture(g_my_free->getTexture());
				p_list[i]->setAnchorPoint(Point(0.0f,0.0f));
				p_list[i]->setPosition(Vec2(x,y));
				auto s_card=Sprite::createWithTexture(g_card_kind[ListOfNo1->data[1+3*i].kind]->getTexture());
				s_card->setAnchorPoint(Vec2(0.5,0.5));
				s_card->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height*0.4));
				p_list[i]->addChild(s_card);
				p_list[i]->setScale(0);
				myframe->addChild(p_list[i],i+5,HAND_IN_CARDS_TAG_ID+1*20+4+i);
				auto list_seq0=Sequence::create(DelayTime::create(timeXH[1]+0.8),ScaleTo::create(0,1),NULL);
				
				x=base_point[1].x+10;
				y=base_point[1].y+10;
				auto mv1=MoveTo::create(0.2,Vec2(x+cardSize.width*i*3+cardSize.width,y));
				auto delay1=DelayTime::create(0.4);
				auto list_seq1=Sequence::create(DelayTime::create(timeXH[1]+0.8),delay1,mv1,NULL);
				auto list_spa=Spawn::create(list_seq0,list_seq1,NULL);
				p_list[i]->runAction(list_spa);
			}
		}
		else if(a==2)//DelayTime::create(2.1)
		{
			for(int i=0;i<4;i++)
			{
				x=base_point[1].x+10;
				y=base_point[1].y+10;
				x += Mehand_in->getTextureRect().size.width*(i*3)+Mehand_in->getTextureRect().size.width*1.0*2;
				p_list[i]=Sprite::createWithTexture(g_my_free->getTexture());
				p_list[i]->setAnchorPoint(Point(0.0f,0.0f));
				p_list[i]->setPosition(Vec2(x,y));
				auto s_card=Sprite::createWithTexture(g_card_kind[ListOfNo1->data[2+3*i].kind]->getTexture());
				s_card->setAnchorPoint(Vec2(0.5,0.5));
				s_card->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height*0.4));
				p_list[i]->addChild(s_card);
				p_list[i]->setScale(0);
				myframe->addChild(p_list[i],i+9,HAND_IN_CARDS_TAG_ID+1*20+8+i);
				p_list[i]->runAction(Sequence::create(DelayTime::create(timeXH[1]+1.4),ScaleTo::create(0,1),NULL));
			}

		}
		else if(a==3)//DelayTime(2.7)
		{
			x=base_point[1].x+10;
			y=base_point[1].y+10;
			x += Mehand_in->getTextureRect().size.width*1.0*12;
			lastTwo[0]=Sprite::createWithTexture(g_my_free->getTexture());
			lastTwo[0]->setAnchorPoint(Vec2(0.0f,0.0f));
			lastTwo[0]->setPosition(Vec2(x,y));
			auto s_card=Sprite::createWithTexture(g_card_kind[ListOfNo1->data[12].kind]->getTexture());
			s_card->setAnchorPoint(Vec2(0.5,0.5));
			s_card->setPosition(Vec2(lastTwo[0]->getTextureRect().size.width/2,lastTwo[0]->getTextureRect().size.height*0.4));
			lastTwo[0]->addChild(s_card);
			lastTwo[0]->setScale(0);
			myframe->addChild(lastTwo[0],0+13,HAND_IN_CARDS_TAG_ID+1*20+12);
			lastTwo[0]->runAction(Sequence::create(DelayTime::create(timeXH[1]+2),ScaleTo::create(0,1),NULL));	
			if(zhuang==1)
			{
				x=base_point[1].x+10;
				x += Mehand_in->getTextureRect().size.width*1.0*13+30;
				lastTwo[1]=Sprite::createWithTexture(g_my_free->getTexture());
				lastTwo[1]->setAnchorPoint(Vec2(0.0f,0.0f));
				lastTwo[1]->setPosition(Vec2(x,y));
				auto s_card=Sprite::createWithTexture(g_card_kind[ListOfNo1->data[13].kind]->getTexture());
				s_card->setAnchorPoint(Vec2(0.5,0.5));
				s_card->setPosition(Vec2(lastTwo[1]->getTextureRect().size.width/2,lastTwo[1]->getTextureRect().size.height*0.4));
				lastTwo[1]->addChild(s_card);
				lastTwo[1]->setScale(0);
				myframe->addChild(lastTwo[1],0+18,HAND_IN_CARDS_TAG_ID+1*20+13);
				lastTwo[1]->runAction(Sequence::create(DelayTime::create(timeXH[1]+2),ScaleTo::create(0,1),NULL));
			}
		}
	}
	//右手
	auto RightHandIn=Sprite::createWithTexture(g_right_free->getTexture());
	auto RightCardIn=Sprite::createWithTexture(g_left_right_in->getTexture());
	auto RightHandInSize=RightHandIn->getTextureRect().size.height;
	auto RightCardInSize=RightCardIn->getTextureRect().size.height;
	Sprite* RobotHandCard[4];
	Vec2 RightHandPosition[4];
	for (a=0;a<4;a++)
	{
		RobotHandCard[a]=Sprite::createWithTexture(g_left_right_in->getTexture());
		RobotHandCard[a]->setAnchorPoint(Vec2(0.5,0));
		RobotHandCard[a]->setPosition(Vec2(origin.x+visibleSize.width*0.5,origin.y+visibleSize.height*0.5-RightCardInSize*1.65+RightCardInSize*a*0.65));
		RobotHandCard[a]->setScale(0);
		myframe->addChild(RobotHandCard[a],6-a,START_CARDS_IN_TAG_ID+2*a);
		RightHandPosition[a]=RobotHandCard[a]->getPosition();
	}
	Point RinghtHandoutEnd=Vec2(base_point[2].x+10,base_point[2].y+10);
	for(a=0;a<4;a++)
	{
		for (b=0;b<4;b++)
		{
			changingPosition[b]=RinghtHandoutEnd.y;
		}
		x=RinghtHandoutEnd.x;
		changingPosition[0]+=RightHandInSize*(a*0.4+0.6);				
		changingPosition[1]+=RightHandInSize*((a+4)*0.4+0.6);			
		changingPosition[2]+=RightHandInSize*((a+8)*0.4+0.6);	
		changingPosition[3]+=RightHandInSize*((13+a)*0.4+0.6);
		auto outTime1=Sequence::create(DelayTime::create(timeXH[2]),ScaleTo::create(0,1),MoveTo::create(0.2f,Vec2(x,changingPosition[0])),VoiceEffect,Spawn::create(ScaleTo::create(0,0),MoveTo::create(0,Vec2(RightHandPosition[a].x,RightHandPosition[a].y)),NULL),NULL);
		auto outTime2=Sequence::create(DelayTime::create(0.4),ScaleTo::create(0,1),MoveTo::create(0.2f,Vec2(x,changingPosition[1])),VoiceEffect,Spawn::create(ScaleTo::create(0,0),MoveTo::create(0,Vec2(RightHandPosition[a].x,RightHandPosition[a].y)),NULL),NULL);
		auto outTime3=Sequence::create(DelayTime::create(0.4),ScaleTo::create(0,1),MoveTo::create(0.2f,Vec2(x,changingPosition[2])),VoiceEffect,Spawn::create(ScaleTo::create(0,0),MoveTo::create(0,Vec2(RightHandPosition[a].x,RightHandPosition[a].y)),NULL),NULL);
		Sequence* outTime4;
		if(zhuang==2&&a<2)
			outTime4=Sequence::create(DelayTime::create(0.4),ScaleTo::create(0,1),MoveTo::create(0.2f,Vec2(x,changingPosition[3])),VoiceEffect,Spawn::create(ScaleTo::create(0,0),MoveTo::create(0,Vec2(RightHandPosition[a].x,RightHandPosition[a].y)),NULL),NULL);
		else if(zhuang!=2&&a<1)
			outTime4=Sequence::create(DelayTime::create(0.4),ScaleTo::create(0,1),MoveTo::create(0.2f,Vec2(x,changingPosition[3])),VoiceEffect,Spawn::create(ScaleTo::create(0,0),MoveTo::create(0,Vec2(RightHandPosition[a].x,RightHandPosition[a].y)),NULL),NULL);
		else
			outTime4=Sequence::create(DelayTime::create(0.4),NULL);
		RobotHandCard[a]->runAction(Sequence::create(outTime1,outTime2,outTime3,outTime4,NULL));
	}
	int RightLen=_roundManager->_players[2]->get_parter()->get_card_list()->len+1;
	for(a=0;a<4;a++)
	{
		if(a==0)//DealayTime::create(1.1)
		{
			x=base_point[2].x+10;
			y=base_point[2].y+10;
			for(int i=0;i<4;i++)
			{
				p_list[i]=Sprite::createWithTexture(g_right_free->getTexture());
				p_list[i]->setAnchorPoint(Point(0,0));
				p_list[i]->setPosition(Vec2(x-10,y-8));
				y+=((p_list[i]->getTextureRect().size.height)*0.5);
				myframe->addChild(p_list[i],RightLen-i,HAND_IN_CARDS_TAG_ID+2*20+i);
				p_list[i]->setScale(0);
				p_list[i]->runAction(Sequence::create(DelayTime::create(timeXH[2]+0.2),ScaleTo::create(0,1),NULL));
			}
		}
		else if(a==1)//DelayTime::create(1.7)
		{
			x=base_point[2].x+10;
			y=base_point[2].y+10;
			y+=((RightHandIn->getTextureRect().size.height)*0.5)*4;
			for(int i=0;i<4;i++)
			{
				p_list[i]=Sprite::createWithTexture(g_right_free->getTexture());
				p_list[i]->setAnchorPoint(Vec2(0,0));
				p_list[i]->setPosition(Vec2(x-10,y-8));
				y+=((p_list[i]->getTextureRect().size.height)*0.5);
				myframe->addChild(p_list[i],RightLen-4-i,HAND_IN_CARDS_TAG_ID+2*20+4+i);
				p_list[i]->setScale(0);
				p_list[i]->runAction(Sequence::create(DelayTime::create(timeXH[2]+0.8),ScaleTo::create(0,1),NULL));
			}
		}
		else if(a==2)//DelayTime::create(2.3)
		{
			x=base_point[2].x+10;
			y=base_point[2].y+10;
			y+=((RightHandIn->getTextureRect().size.height)*0.5)*8;
			for(int i=0;i<4;i++)
			{
				p_list[i]=Sprite::createWithTexture(g_right_free->getTexture());
				p_list[i]->setAnchorPoint(Vec2(0,0));
				p_list[i]->setPosition(Vec2(x-10,y-8));
				y+=((p_list[i]->getTextureRect().size.height)*0.5);
				myframe->addChild(p_list[i],RightLen-8-i,HAND_IN_CARDS_TAG_ID+2*20+8+i);
				p_list[i]->setScale(0);
				p_list[i]->runAction(Sequence::create(DelayTime::create(timeXH[2]+1.4),ScaleTo::create(0,1),NULL));
			}
		}
		else if(a==3)//DelayTime(2.9)
		{	
			x=base_point[2].x+10;
			y=base_point[2].y+10;
			y+=((RightHandIn->getTextureRect().size.height)*0.5)*12;
			lastTwo[0]=Sprite::createWithTexture(g_right_free->getTexture());
			lastTwo[0]->setAnchorPoint(Vec2(0,0));
			lastTwo[0]->setPosition(Vec2(x-10,y-8));
			myframe->addChild(lastTwo[0],RightLen-12,HAND_IN_CARDS_TAG_ID+2*20+12);
			lastTwo[0]->setScale(0);	
			lastTwo[0]->runAction(Sequence::create(DelayTime::create(timeXH[2]+2),ScaleTo::create(0,1),NULL));
			if(zhuang==2)
			{
				y=base_point[2].y+10;
				y+=((RightHandIn->getTextureRect().size.height)*0.5)*13;
				lastTwo[1]=Sprite::createWithTexture(g_right_free->getTexture());
				lastTwo[1]->setAnchorPoint(Vec2(0,0));
				lastTwo[1]->setPosition(Vec2(x-10,y-8));
				myframe->addChild(lastTwo[1],RightLen-13,HAND_IN_CARDS_TAG_ID+2*20+13);
				lastTwo[1]->setScale(0);	
				lastTwo[1]->runAction(Sequence::create(DelayTime::create(timeXH[2]+2),ScaleTo::create(0,1),NULL));
			}
		}
	}
	//左手
	Point LeftHandoutEnd=Vec2(base_point[0].x+10,base_point[0].y+10);
	Sprite* LeftRobotHandCard[4];
	Vec2 LeftHandPosition[4];
	for (a=0;a<4;a++)
	{
		LeftRobotHandCard[a]=Sprite::createWithTexture(g_left_right_in->getTexture());
		LeftRobotHandCard[a]->setAnchorPoint(Vec2(0.5f,0.5f));
		LeftRobotHandCard[a]->setPosition(Vec2(origin.x+visibleSize.width*0.5,origin.y+visibleSize.height*0.5+RightCardInSize*1.2-RightCardInSize*a*0.6));
		LeftRobotHandCard[a]->setScale(0);
		myframe->addChild(LeftRobotHandCard[a],3,START_CARDS_IN_TAG_ID+3*a);
		LeftHandPosition[a]=LeftRobotHandCard[a]->getPosition();
	}
	//float changingPosition[4];
	for(a=0;a<4;a++)
	{
		for (b = 0; b < 4; b++)
		{
			changingPosition[b]=LeftHandoutEnd.y;
		}
		x=LeftHandoutEnd.x;
		changingPosition[0]-=RightHandInSize*a*0.6;				
		changingPosition[1]-=RightHandInSize*(a+4)*0.6;			
		changingPosition[2]-=RightHandInSize*(a+8)*0.6;	
		changingPosition[3]-=((RightHandInSize*13+a*RightHandInSize)*0.6+13.6);
		auto outTime1=Sequence::create(DelayTime::create(timeXH[0]),ScaleTo::create(0,1),MoveTo::create(0.2f,Vec2(x,changingPosition[0])),VoiceEffect,Spawn::create(ScaleTo::create(0,0),MoveTo::create(0,Vec2(LeftHandPosition[a].x,LeftHandPosition[a].y)),NULL),NULL);
		auto outTime2=Sequence::create(DelayTime::create(0.4),ScaleTo::create(0,1),MoveTo::create(0.2f,Vec2(x,changingPosition[1])),VoiceEffect,Spawn::create(ScaleTo::create(0,0),MoveTo::create(0,Vec2(LeftHandPosition[a].x,LeftHandPosition[a].y)),NULL),NULL);
		auto outTime3=Sequence::create(DelayTime::create(0.4),ScaleTo::create(0,1),MoveTo::create(0.2f,Vec2(x,changingPosition[2])),VoiceEffect,Spawn::create(ScaleTo::create(0,0),MoveTo::create(0,Vec2(LeftHandPosition[a].x,LeftHandPosition[a].y)),NULL),NULL);
		Sequence* outTime4;
		if(zhuang==0&&a<2)
			outTime4=Sequence::create(DelayTime::create(0.4),ScaleTo::create(0,1),MoveTo::create(0.2f,Vec2(x,changingPosition[3])),VoiceEffect,Spawn::create(ScaleTo::create(0,0),MoveTo::create(0,Vec2(LeftHandPosition[a].x,LeftHandPosition[a].y)),NULL),NULL);
		else if(zhuang!=0&&a<1)
			outTime4=Sequence::create(DelayTime::create(0.4),ScaleTo::create(0,1),MoveTo::create(0.2f,Vec2(x,changingPosition[3])),VoiceEffect,Spawn::create(ScaleTo::create(0,0),MoveTo::create(0,Vec2(LeftHandPosition[a].x,LeftHandPosition[a].y)),NULL),NULL);
		else
			outTime4=Sequence::create(DelayTime::create(0.4),NULL);
		LeftRobotHandCard[a]->runAction(Sequence::create(outTime1,outTime2,outTime3,outTime4,NULL));

	}
	for(a=0;a<4;a++)
	{
		if(a==0)//DealayTime::create(1.1)
		{
			x=base_point[0].x+10;
			y=base_point[0].y+10;
			for(int i=0;i<4;i++)
			{
				p_list[i]=Sprite::createWithTexture(g_left_free->getTexture());
				p_list[i]->setAnchorPoint(Vec2(0.0f,1.0f));
				p_list[i]->setPosition(Vec2(x-10,y-8));
				y -= ((p_list[i]->getTextureRect().size.height)*0.5);
				myframe->addChild(p_list[i],i+1,HAND_IN_CARDS_TAG_ID+0*20+i);
				p_list[i]->setScale(0);
				p_list[i]->runAction(Sequence::create(DelayTime::create(timeXH[0]+0.2),ScaleTo::create(0,1),NULL));
			}
		}
		else if(a==1)//DelayTime::create(1.7)
		{
			x=base_point[0].x+10;
			y=base_point[0].y+10;
			y-= (RightHandInSize*0.5)*4;
			for(int i=0;i<4;i++)
			{
				p_list[i]=Sprite::createWithTexture(g_left_free->getTexture());
				p_list[i]->setAnchorPoint(Vec2(0.0f,1.0f));
				p_list[i]->setPosition(Vec2(x-10,y-8));
				y -= ((p_list[i]->getTextureRect().size.height)*0.5);
				myframe->addChild(p_list[i],i+5,HAND_IN_CARDS_TAG_ID+0*20+4+i);
				p_list[i]->setScale(0);
				p_list[i]->runAction(Sequence::create(DelayTime::create(timeXH[0]+0.8),ScaleTo::create(0,1),NULL));
			}
		}
		else if(a==2)//DelayTime::create(2.3)
		{
			x=base_point[0].x+10;
			y=base_point[0].y+10;
			y-= (RightHandInSize*0.5)*8;
			for(int i=0;i<4;i++)
			{
				p_list[i]=Sprite::createWithTexture(g_left_free->getTexture());
				p_list[i]->setAnchorPoint(Vec2(0.0f,1.0f));
				p_list[i]->setPosition(Vec2(x-10,y-8));
				y-=((p_list[i]->getTextureRect().size.height)*0.5);
				myframe->addChild(p_list[i],i+9,HAND_IN_CARDS_TAG_ID+0*20+8+i);
				p_list[i]->setScale(0);
				p_list[i]->runAction(Sequence::create(DelayTime::create(timeXH[0]+1.4),ScaleTo::create(0,1),NULL));
			}
		}
		else if(a==3)//DelayTime(2.9)
		{	
			x=base_point[0].x+10;
			y=base_point[0].y+10;
			y-= (RightHandInSize*0.5)*12;
			lastTwo[0]=Sprite::createWithTexture(g_left_free->getTexture());
			lastTwo[0]->setAnchorPoint(Vec2(0.0f,1.0f));
			lastTwo[0]->setPosition(Vec2(x-10,y-8));
			myframe->addChild(lastTwo[0],13,HAND_IN_CARDS_TAG_ID+0*20+12);
			lastTwo[0]->setScale(0);	
			lastTwo[0]->runAction(Sequence::create(DelayTime::create(timeXH[0]+2),ScaleTo::create(0,1),NULL));
			if(zhuang==0)
			{
				y=base_point[0].y+10;
				y-=(RightHandInSize*0.5)*13;
				lastTwo[1]=Sprite::createWithTexture(g_left_free->getTexture());
				lastTwo[1]->setAnchorPoint(Vec2(0.0f,1.0f));
				lastTwo[1]->setPosition(Vec2(x-10,y-8));
				myframe->addChild(lastTwo[1],14,HAND_IN_CARDS_TAG_ID+0*20+13);
				lastTwo[1]->setScale(0);	
				lastTwo[1]->runAction(Sequence::create(DelayTime::create(timeXH[0]+2),ScaleTo::create(0,1),NULL));
			}

		}
	}

	auto callFunc0=CallFunc::create([=](){
		card_list_update(0);
		});
	auto callFunc1=CallFunc::create([=](){
		card_list_update(1);
		});	
	auto callFunc2=CallFunc::create([=](){
		card_list_update(2);
		});	
	auto update_lists_spa=Spawn::create(callFunc0,callFunc1,callFunc2,NULL);

	myframe->_ID=zhuang;
	_roundManager->_isCardFromOthers=false;

	distribute_card();

	_roundManager->_distributedNum=0;	
	auto updateFourCards=CallFunc::create([=](){
		_roundManager->_distributedNum += 4;
		update_residue_cards(TOTAL_CARD_NUM - _roundManager->_distributedNum);		
	});
	auto updateOneCards=CallFunc::create([=](){
		_roundManager->_distributedNum +=1;
		update_residue_cards(TOTAL_CARD_NUM - _roundManager->_distributedNum);		
	});
	auto updateTwoCards=CallFunc::create([=](){
		_roundManager->_distributedNum +=2;
		update_residue_cards(TOTAL_CARD_NUM - _roundManager->_distributedNum);		
	});
	auto updateClock=CallFunc::create([=](){
		UpdateClock(0,zhuang);		
	});
	auto StartDelay=DelayTime::create(0.7);
	auto HandDelay=DelayTime::create(0.2);
	auto HandFourSeq=Sequence::create(updateFourCards,HandDelay,NULL);
	auto HandOneSeq=Sequence::create(updateOneCards,HandDelay,NULL);
	auto HandTwoSeq=Sequence::create(updateTwoCards,HandDelay,NULL);
	auto seq=Sequence::create(StartDelay,HandFourSeq,HandFourSeq,HandFourSeq,HandFourSeq,
		HandFourSeq,HandFourSeq,HandFourSeq,HandFourSeq,HandFourSeq,HandTwoSeq,HandOneSeq,HandOneSeq,NULL);
	//DelayTime::create(3.1)
	_roundManager->_actionToDo=a_JUMP;
	myframe->runAction(Sequence::create(seq,update_lists_spa,updateClock,CallFunc::create(this,callfunc_selector(NetRaceLayer::start_dealCardsDelete)),
		CCCallFunc::create([=](){
		first_response(zhuang);
	}),NULL));
}

void NetRaceLayer:: display_callback(cocos2d::Ref* pSender)
{
    LOGGER_WRITE("%s",__FUNCTION__);


}
void NetRaceLayer::AccountShows(LayerColor* BarOfPlayer,int no)
{
    LOGGER_WRITE("%s",__FUNCTION__);

	int PropertyOfPlayer;
	std::string PhotoOfPlayer;
	std::string NickNameOfPlayer;
	_roundManager->_players[no]->get_property(PropertyOfPlayer);
	_roundManager->_players[no]->get_photo(PhotoOfPlayer);
	_roundManager->_players[no]->get_nick_name(NickNameOfPlayer);
	char buffer[80];
	auto Gold=Sprite::createWithSpriteFrameName("result_money.png");//金币1
	Gold->setAnchorPoint(Vec2(0,0.5));
	Gold->setPosition(Vec2(15,22));
	BarOfPlayer->addChild(Gold,2,ACCOUNT_JINBI);

	//===================================万，亿=====================================//
	LabelAtlas* labelOfProperty;
	std::string stringOfProperty;
	Sprite* propertyUnit;

	float NumQian=1000,NumWan=10000,NumShiWan=100000,NumBaiWan=1000000,NumQianWan=10000000,NumYi=100000000;
	float tempProperty;
	int tempXiapShu;
	if(PropertyOfPlayer<NumShiWan)
	{
		sprintf(buffer,"%d",PropertyOfPlayer);
	}
	else if(PropertyOfPlayer>=NumShiWan&&PropertyOfPlayer<NumQianWan)
	{
		tempXiapShu=PropertyOfPlayer%1000;
		if(tempXiapShu>=500)
			tempProperty=(float)(PropertyOfPlayer-500)/10000;
		else
			tempProperty=(float)PropertyOfPlayer/10000;
		tempXiapShu=PropertyOfPlayer%10000;
		if(tempXiapShu>=1000)
			sprintf(buffer,"%.1f",tempProperty);
		else
			sprintf(buffer,"%.0f",tempProperty);
	}
	else if(PropertyOfPlayer>=NumQianWan&&PropertyOfPlayer<NumYi)
	{
		tempXiapShu=PropertyOfPlayer%10000;
		if(tempXiapShu>=5000)
			tempProperty=(float)(PropertyOfPlayer-500)/10000;
		else
			tempProperty=(float)PropertyOfPlayer/10000;
		sprintf(buffer,"%.0f",tempProperty);
	}
	else if(PropertyOfPlayer>=NumYi)
	{
		tempXiapShu=PropertyOfPlayer%10000000;
		if(tempXiapShu>=5000000)
			tempProperty=(float)(PropertyOfPlayer-5000000)/100000000;
		else
			tempProperty=(float)PropertyOfPlayer/100000000;
		tempXiapShu=PropertyOfPlayer%100000000;
		if(tempXiapShu>=10000000)
			sprintf(buffer,"%.1f",tempProperty);
		else
			sprintf(buffer,"%.0f",tempProperty);
	}
	stringOfProperty=buffer;
	labelOfProperty=LabelAtlas::create(stringOfProperty,"fonts/result_money_number.png",17,23,'.');
	labelOfProperty->setAnchorPoint(Vec2(0,0.5));
	labelOfProperty->setPosition(Vec2(Gold->getPosition().x+Gold->getTextureRect().size.width+5,22));
	BarOfPlayer->addChild(labelOfProperty,2,ACCOUNT_PROPRETY);

	if(PropertyOfPlayer>=NumShiWan&&PropertyOfPlayer<NumYi)
	{
		propertyUnit=Sprite::createWithSpriteFrameName("wan-hand.png");
		propertyUnit->setAnchorPoint(Vec2(0,0.5));
		propertyUnit->setPosition(Vec2(labelOfProperty->getPosition().x+labelOfProperty->getContentSize().width+5,22));
		BarOfPlayer->addChild(propertyUnit,ACCOUNT_PROPRETY_UNIT);
	}
	else if(PropertyOfPlayer>=NumYi)
	{
		propertyUnit=Sprite::createWithSpriteFrameName("yi-hand.png");
		propertyUnit->setAnchorPoint(Vec2(0,0.5));
		propertyUnit->setPosition(Vec2(labelOfProperty->getPosition().x+labelOfProperty->getContentSize().width+5,22));
		BarOfPlayer->addChild(propertyUnit,ACCOUNT_PROPRETY_UNIT);
	}
	//===================================万，亿=====================================//
	auto NickName=Label::create(NickNameOfPlayer,"Arial",28);//昵称3
	NickName->setAnchorPoint(Vec2(0.5,0.5));
	NickName->setPosition(Vec2(70,60));
	BarOfPlayer->addChild(NickName,2,ACCOUNT_NIKENAME);

	auto headPhoto=Sprite::createWithSpriteFrameName(PhotoOfPlayer);//头像4
	headPhoto->setScaleX(100/headPhoto->getTextureRect().size.width);
	headPhoto->setScaleY(100/headPhoto->getTextureRect().size.height);
	headPhoto->setAnchorPoint(Vec2(0,0));
	headPhoto->setPosition(Vec2(20,80));
	BarOfPlayer->addChild(headPhoto,2,ACCOUNT_IMAGE);

	auto Win=Sprite::createWithSpriteFrameName("jiesuanhu.png");//胡5
	Win->setAnchorPoint(Vec2(0.5,0.5));
	Win->setPosition(Vec2(origin.x+visibleSize.width*0.9,origin.y+visibleSize.height*0.15363));
	Win->setVisible(false);
	BarOfPlayer->addChild(Win,2,ACCOUNT_HU_FONT);

	auto selfDrawn=Sprite::createWithSpriteFrameName("jiesuanzimo.png");//自摸6
	selfDrawn->setAnchorPoint(Vec2(0.5,0.5));
	selfDrawn->setPosition(Vec2(origin.x+visibleSize.width*0.9,origin.y+visibleSize.height*0.15363));
	selfDrawn->setVisible(false);
	BarOfPlayer->addChild(selfDrawn,2,ACCOUNT_ZIMO_FONT);

	auto PointGun1=Sprite::createWithSpriteFrameName("jiesuanfangpao.png");//点炮7
	PointGun1->setAnchorPoint(Vec2(0.5,0.5));
	PointGun1->setPosition(Vec2(origin.x+visibleSize.width*0.9,origin.y+visibleSize.height*0.15363));
	PointGun1->setVisible(false);
	BarOfPlayer->addChild(PointGun1,2,ACCOUNT_DIANPAO_FONT);

	auto signPlus=Sprite::createWithSpriteFrameName("fen_add.png");//+8
	signPlus->setAnchorPoint(Vec2(0,0.5));

	if( _roundManager->IsWinner(no, _roundManager->_curPlayer, _roundManager->_firstMingNo) )
		signPlus->setPosition(Vec2(origin.x+visibleSize.width*0.816+20,BarOfPlayer->getContentSize().height/2));
	else
		signPlus->setPosition(Vec2(origin.x+visibleSize.width*0.816+20,origin.y+30));
	signPlus->setVisible(false);
	BarOfPlayer->addChild(signPlus,2,ACCOUNT_ADD_SYMBOL);

	auto signMinus=Sprite::createWithSpriteFrameName("fen_sub.png");//9
	signMinus->setAnchorPoint(Vec2(0,0.5));
	if( _roundManager->IsWinner(no, _roundManager->_curPlayer, _roundManager->_firstMingNo) )
		signMinus->setPosition(Vec2(origin.x+visibleSize.width*0.816+20,BarOfPlayer->getContentSize().height/2));
	else
		signMinus->setPosition(Vec2(origin.x+visibleSize.width*0.816+20,origin.y+30));
	signMinus->setVisible(false);
	BarOfPlayer->addChild(signMinus,2,ACCOUNT_MINUS_SYMBOL);

	/*================================结算金钱========================================*/
    WinInfo_t win;
    _roundManager->GetWin(win);

	if( !(win.kind==NONE_WIN && _roundManager->_firstMingNo==-1) )
	{
		char char_AccoutnGold[80];
		std::string str_AccountGold;
		sprintf(char_AccoutnGold,"%d",abs(GoldAccountImmediate[no]));
		str_AccountGold=char_AccoutnGold;
		LabelAtlas* propertyOfIncrease;
		if(GoldAccountImmediate[no]>0)
		{
			signPlus->setVisible(true);
			auto propertyOfIncrease=LabelAtlas::create(str_AccountGold,"fonts/Score_add_number.png",26,32,'0');//加
			propertyOfIncrease->setAnchorPoint(Vec2(0,0.5));
			if( _roundManager->IsWinner(no, _roundManager->_curPlayer, _roundManager->_firstMingNo)  )
				propertyOfIncrease->setPosition(Vec2(origin.x+visibleSize.width*0.816+50+20,BarOfPlayer->getContentSize().height/2));
			else
				propertyOfIncrease->setPosition(Vec2(origin.x+visibleSize.width*0.816+50+20,origin.y+30));

			if(GoldAccountImmediate[no]!=0)
				propertyOfIncrease->setVisible(true);
			else
				propertyOfIncrease->setVisible(false);

			BarOfPlayer->addChild(propertyOfIncrease,2,ACCOUNT_WINGOLD_NUM);
		}
		else if(GoldAccountImmediate[no]<0)
		{
			signMinus->setVisible(true);
			auto propertyOfIncrease=LabelAtlas::create(str_AccountGold,"fonts/Score_sub_number.png",26,32,'0');//减
			propertyOfIncrease->setAnchorPoint(Vec2(0,0.5));
			if( _roundManager->IsWinner(no, _roundManager->_curPlayer, _roundManager->_firstMingNo)  )
				propertyOfIncrease->setPosition(Vec2(origin.x+visibleSize.width*0.816+50+20,BarOfPlayer->getContentSize().height/2));
			else
				propertyOfIncrease->setPosition(Vec2(origin.x+visibleSize.width*0.816+50+20,origin.y+30));
			if(GoldAccountImmediate[no]!=0)
				propertyOfIncrease->setVisible(true);
			else
				propertyOfIncrease->setVisible(false);
			BarOfPlayer->addChild(propertyOfIncrease,2,ACCOUNT_WINGOLD_NUM);
		}
	}
	/*================================结算金钱========================================*/
	//auto pos1=Vec2(20,22);//金币&&数字0，0.5
	//auto pos2=Vec2(70,60);//昵称//0.5，0.5
	//auto pos3=Vec2(20,80);//头像0，0
	auto BaoZhuang=Sprite::createWithSpriteFrameName("jiesuanbaozhuang.png");//包庄
	BaoZhuang->setAnchorPoint(Vec2(0.5,0.5));
	BaoZhuang->setPosition(Vec2(origin.x+visibleSize.width*0.9,origin.y+visibleSize.height*0.15363));
	BaoZhuang->setVisible(false);
	BarOfPlayer->addChild(BaoZhuang,2,ACCOUNT_BAOZHUANG_FONT);

	auto posOfCards=Vec2(162,180);//牌0，1

	Sprite *show_card_list[MAX_HANDIN_NUM];
	CARD_ARRAY *list=_roundManager->_players[no]->get_parter()->get_card_list();
	auto tempCard=Sprite::createWithTexture(g_my_peng->getTexture());
	float x=posOfCards.x;
	float y=posOfCards.y-tempCard->getTextureRect().size.height/2;
	for(int i=0;i<list->len;i++)
	{
		show_card_list[i]=Sprite::createWithTexture(g_my_peng->getTexture());
		show_card_list[i]->setAnchorPoint(Vec2(0,0.5));
		show_card_list[i]->setPosition(Vec2(x,y));
		BarOfPlayer->addChild(show_card_list[i],2);
		auto s_card=Sprite::createWithTexture(g_mid_card_kind[list->data[i].kind]->getTexture());
		s_card->setAnchorPoint(Vec2(0.5,0.5));
		s_card->setPosition(Vec2(show_card_list[i]->getTextureRect().size.width/2,show_card_list[i]->getTextureRect().size.height*0.6));
		show_card_list[i]->addChild(s_card,1);
		if(list->data[i].status==c_PENG||list->data[i].status==c_MING_KOU)
		{
			if(list->data[i].kind!=list->data[i+1].kind)
			{
				x+=show_card_list[i]->getTextureRect().size.width*1.1;
			}
			else if(list->data[i].kind==list->data[i+1].kind && list->data[i+1].status==c_FREE)
			{
				x+=show_card_list[i]->getTextureRect().size.width*1.1;
			}
			else
			{
				x+=show_card_list[i]->getTextureRect().size.width*0.95;
			}
		}
		else if(list->data[i].status==c_MING_GANG||list->data[i].status==c_AN_GANG)
		{
			if(list->data[i].kind!=list->data[i+1].kind)
			{
				x+=show_card_list[i]->getTextureRect().size.width*1.1;
			}
			else
			{
				x+=show_card_list[i]->getTextureRect().size.width*0.95;
			}
		}
		else if(list->data[i].status==c_FREE)
		{
			if( win.kind==SINGLE_WIN && win.player==no && win.player==_roundManager->_curPlayer && i==(list->len-2) )
				x=x+show_card_list[i]->getTextureRect().size.width*0.95+30;
			else
				x+=show_card_list[i]->getTextureRect().size.width*0.95;
		}
	}
	if((win.kind==SINGLE_WIN && win.player==no) || (win.kind==DOUBLE_WIN &&_roundManager->_curPlayer!=no))
	{
		if(_roundManager->_isCardFromOthers)
		{
			auto winCard=Sprite::createWithTexture(g_my_peng->getTexture());
			winCard->setAnchorPoint(Vec2(0,0.5));
			winCard->setPosition(Vec2(x+30,y));
			BarOfPlayer->addChild(winCard,2);

			auto s_card=Sprite::createWithTexture(g_mid_card_kind[list->data[list->len].kind]->getTexture());
			s_card->setAnchorPoint(Vec2(0.5,0.5));
			s_card->setPosition(Vec2(winCard->getTextureRect().size.width/2,winCard->getTextureRect().size.height*0.6));
			winCard->addChild(s_card,1);
		}
	}

	float Extra_x=162;
	//float Extra_x=origin.x+visibleSize.width*0.17;
	float Extra_y=origin.y+visibleSize.height*0.1256-10;
	int tagNum=BarOfPlayer->getTag();
	unsigned char tingStatus=_roundManager->_players[tagNum]->get_parter()->get_ting_status();
	if(tingStatus==1&&
        ((win.kind==SINGLE_WIN && ((win.player==_roundManager->_curPlayer&&tagNum!=win.player)||(win.player!=_roundManager->_curPlayer&&tagNum==_roundManager->_curPlayer)))||
        (win.kind==DOUBLE_WIN && tagNum==_roundManager->_curPlayer)))
	{
		auto mingFlag=Sprite::createWithSpriteFrameName("result_mpbh.png");
		mingFlag->setAnchorPoint(Vec2(0,0.5));
		mingFlag->setPosition(Vec2(Extra_x,Extra_y));
		BarOfPlayer->addChild(mingFlag);
	}
}
void NetRaceLayer::AccountHuKind(LayerColor* BarOfPlayer,int num)
{
    LOGGER_WRITE("%s",__FUNCTION__);

	//float x=origin.x+visibleSize.width*0.17;
	float x=162;
	float y=origin.y+visibleSize.height*0.1256-10;
	int tagNum=BarOfPlayer->getTag();
	unsigned char tingStatus=_roundManager->_players[tagNum]->get_parter()->get_ting_status();
	auto curScore=_roundManager->_players[tagNum]->get_parter()->get_card_score();

    WinInfo_t win;
    _roundManager->GetWin(win);
    
	if((win.kind==SINGLE_WIN&&(win.player==_roundManager->_curPlayer&&curScore==2)||(win.player!=_roundManager->_curPlayer&&tagNum==win.player&&curScore==1))
        ||(win.kind==DOUBLE_WIN&&tagNum!=_roundManager->_curPlayer&&curScore==1))
	{
		auto kindOfHuBkg = Sprite::createWithSpriteFrameName("result_fx_item_back.png");//背景
		kindOfHuBkg->setAnchorPoint(Point(0.0f,0.5f));
		kindOfHuBkg->setPosition(Vec2(x,y));
		BarOfPlayer->addChild(kindOfHuBkg);
		auto kindOfHu = Sprite::createWithSpriteFrameName("ph-js.png");//番型种类图片
		kindOfHu->setAnchorPoint(Point(0.0f,0.5f));
		kindOfHu->setPosition(Vec2(17,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(kindOfHu);
		auto timesOfHu=LabelAtlas::create("1","fonts/result_fan_number.png",22, 30, '0');//番数
		timesOfHu->setAnchorPoint(Vec2(0,0.5));
		timesOfHu->setPosition(Vec2(160,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(timesOfHu);//result_fx_text
		auto fanFont = Sprite::createWithSpriteFrameName("result_fx_text.png");//番字
		fanFont->setAnchorPoint(Point(0.0f,0.5f));
		fanFont->setPosition(Vec2(timesOfHu->getPosition().x+timesOfHu->getContentSize().width+6,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(fanFont);
		x+=kindOfHuBkg->getTextureRect().size.width+22;
		if(x>=900)
		{
			x=origin.x+visibleSize.width*0.17;
			y=origin.y+visibleSize.height*0.1256-10-56;
		}
	}
	if(num&RH_MING)
	{
		if(tingStatus==1&&((win.kind==SINGLE_WIN&&tagNum==win.player)||(win.kind==DOUBLE_WIN&&(tagNum!=_roundManager->_curPlayer))))
		{
			auto kindOfHuBkg = Sprite::createWithSpriteFrameName("result_fx_item_back.png");//背景
			kindOfHuBkg->setAnchorPoint(Point(0.0f,0.5f));
			kindOfHuBkg->setPosition(Vec2(x,y));
			BarOfPlayer->addChild(kindOfHuBkg);
			auto kindOfHu = Sprite::createWithSpriteFrameName("m-js.png");//番型种类图片
			kindOfHu->setAnchorPoint(Point(0.0f,0.5f));
			kindOfHu->setPosition(Vec2(17,kindOfHuBkg->getTextureRect().size.height/2));
			kindOfHuBkg->addChild(kindOfHu);
			auto timesOfHu=LabelAtlas::create("2","fonts/result_fan_number.png",22, 30, '0');//番数
			timesOfHu->setAnchorPoint(Vec2(0,0.5));
			timesOfHu->setPosition(Vec2(160,kindOfHuBkg->getTextureRect().size.height/2));
			kindOfHuBkg->addChild(timesOfHu);//result_fx_text
			auto fanFont = Sprite::createWithSpriteFrameName("result_fx_text.png");//番字
			fanFont->setAnchorPoint(Point(0.0f,0.5f));
			fanFont->setPosition(Vec2(timesOfHu->getPosition().x+timesOfHu->getContentSize().width+6,kindOfHuBkg->getTextureRect().size.height/2));
			kindOfHuBkg->addChild(fanFont);
			x+=kindOfHuBkg->getTextureRect().size.width+22;
			if(x>=900)
			{
				x=origin.x+visibleSize.width*0.17;
				y=origin.y+visibleSize.height*0.1256-10-56;
			}
		}
	}
	if((num&RH_HAIDILAO)&&win.kind==SINGLE_WIN)
	{
		auto kindOfHuBkg = Sprite::createWithSpriteFrameName("result_fx_item_back.png");//背景
		kindOfHuBkg->setAnchorPoint(Point(0.0f,0.5f));
		kindOfHuBkg->setPosition(Vec2(x,y));
		BarOfPlayer->addChild(kindOfHuBkg);
		auto kindOfHu = Sprite::createWithSpriteFrameName("hdl-js.png");//番型种类图片
		kindOfHu->setAnchorPoint(Point(0.0f,0.5f));
		kindOfHu->setPosition(Vec2(17,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(kindOfHu);
		auto timesOfHu=LabelAtlas::create("2","fonts/result_fan_number.png",22, 30, '0');//番数
		timesOfHu->setAnchorPoint(Vec2(0,0.5));
		timesOfHu->setPosition(Vec2(160,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(timesOfHu);//result_fx_text
		auto fanFont = Sprite::createWithSpriteFrameName("result_fx_text.png");//番字
		fanFont->setAnchorPoint(Point(0.0f,0.5f));
		fanFont->setPosition(Vec2(timesOfHu->getPosition().x+timesOfHu->getContentSize().width+6,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(fanFont);
		x+=kindOfHuBkg->getTextureRect().size.width+22;
		if(x>=900)
		{
			x=origin.x+visibleSize.width*0.17;
			y=origin.y+visibleSize.height*0.1256-10-56;
		}
	}
	if(num&RH_QINYISE)
	{
		auto kindOfHuBkg = Sprite::createWithSpriteFrameName("result_fx_item_back.png");//背景
		kindOfHuBkg->setAnchorPoint(Point(0.0f,0.5f));
		kindOfHuBkg->setPosition(Vec2(x,y));
		BarOfPlayer->addChild(kindOfHuBkg);
		auto kindOfHu = Sprite::createWithSpriteFrameName("qys-js.png");//番型种类图片
		kindOfHu->setAnchorPoint(Point(0.0f,0.5f));
		kindOfHu->setPosition(Vec2(17,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(kindOfHu);
		auto timesOfHu=LabelAtlas::create("4","fonts/result_fan_number.png",22, 30, '0');//番数
		timesOfHu->setAnchorPoint(Vec2(0,0.5));
		timesOfHu->setPosition(Vec2(160,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(timesOfHu);//result_fx_text
		auto fanFont = Sprite::createWithSpriteFrameName("result_fx_text.png");//番字
		fanFont->setAnchorPoint(Point(0.0f,0.5f));
		fanFont->setPosition(Vec2(timesOfHu->getPosition().x+timesOfHu->getContentSize().width+6,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(fanFont);
		x+=kindOfHuBkg->getTextureRect().size.width+22;
		if(x>=900)
		{
			x=origin.x+visibleSize.width*0.17;
			y=origin.y+visibleSize.height*0.1256-10-56;
		}
	}
	if(num&RH_SIPENG)
	{
		auto kindOfHuBkg = Sprite::createWithSpriteFrameName("result_fx_item_back.png");//背景
		kindOfHuBkg->setAnchorPoint(Point(0.0f,0.5f));
		kindOfHuBkg->setPosition(Vec2(x,y));
		BarOfPlayer->addChild(kindOfHuBkg);
		auto kindOfHu = Sprite::createWithSpriteFrameName("pph-js.png");//番型种类图片
		kindOfHu->setAnchorPoint(Point(0.0f,0.5f));
		kindOfHu->setPosition(Vec2(17,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(kindOfHu);
		auto timesOfHu=LabelAtlas::create("2","fonts/result_fan_number.png",22, 30, '0');//番数
		timesOfHu->setAnchorPoint(Vec2(0,0.5));
		timesOfHu->setPosition(Vec2(160,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(timesOfHu);//result_fx_text
		auto fanFont = Sprite::createWithSpriteFrameName("result_fx_text.png");//番字
		fanFont->setAnchorPoint(Point(0.0f,0.5f));
		fanFont->setPosition(Vec2(timesOfHu->getPosition().x+timesOfHu->getContentSize().width+6,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(fanFont);
		x+=kindOfHuBkg->getTextureRect().size.width+22;
		if(x>=900)
		{
			x=origin.x+visibleSize.width*0.17;
			y=origin.y+visibleSize.height*0.1256-10-56;
		}
	}
	if(num&RH_QIANGGANG)
	{
		auto kindOfHuBkg = Sprite::createWithSpriteFrameName("result_fx_item_back.png");//背景
		kindOfHuBkg->setAnchorPoint(Point(0.0f,0.5f));
		kindOfHuBkg->setPosition(Vec2(x,y));
		BarOfPlayer->addChild(kindOfHuBkg);
		auto kindOfHu = Sprite::createWithSpriteFrameName("qgh-js.png");//番型种类图片
		kindOfHu->setAnchorPoint(Point(0.0f,0.5f));
		kindOfHu->setPosition(Vec2(17,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(kindOfHu);
		auto timesOfHu=LabelAtlas::create("2","fonts/result_fan_number.png",22, 30, '0');//番数
		timesOfHu->setAnchorPoint(Vec2(0,0.5));
		timesOfHu->setPosition(Vec2(160,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(timesOfHu);//result_fx_text
		auto fanFont = Sprite::createWithSpriteFrameName("result_fx_text.png");//番字
		fanFont->setAnchorPoint(Point(0.0f,0.5f));
		fanFont->setPosition(Vec2(timesOfHu->getPosition().x+timesOfHu->getContentSize().width+6,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(fanFont);
		x+=kindOfHuBkg->getTextureRect().size.width+22;
		if(x>=900)
		{
			x=origin.x+visibleSize.width*0.17;
			y=origin.y+visibleSize.height*0.1256-10-56;
		}
	}
	if(num&RH_GANGHUA)
	{
		auto kindOfHuBkg = Sprite::createWithSpriteFrameName("result_fx_item_back.png");//背景
		kindOfHuBkg->setAnchorPoint(Point(0.0f,0.5f));
		kindOfHuBkg->setPosition(Vec2(x,y));
		BarOfPlayer->addChild(kindOfHuBkg);
		auto kindOfHu = Sprite::createWithSpriteFrameName("gskh-js.png");//番型种类图片
		kindOfHu->setAnchorPoint(Point(0.0f,0.5f));
		kindOfHu->setPosition(Vec2(17,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(kindOfHu);
		auto timesOfHu=LabelAtlas::create("2","fonts/result_fan_number.png",22, 30, '0');//番数
		timesOfHu->setAnchorPoint(Vec2(0,0.5));
		timesOfHu->setPosition(Vec2(160,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(timesOfHu);//result_fx_text
		auto fanFont = Sprite::createWithSpriteFrameName("result_fx_text.png");//番字
		fanFont->setAnchorPoint(Point(0.0f,0.5f));
		fanFont->setPosition(Vec2(timesOfHu->getPosition().x+timesOfHu->getContentSize().width+6,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(fanFont);
		x+=kindOfHuBkg->getTextureRect().size.width+22;
		if(x>=900)
		{
			x=origin.x+visibleSize.width*0.17;
			y=origin.y+visibleSize.height*0.1256-10-56;
		}
	}
	if(num&RH_GANGPAO) // undefinded
	{
		auto kindOfHuBkg = Sprite::createWithSpriteFrameName("result_fx_item_back.png");//背景
		kindOfHuBkg->setAnchorPoint(Point(0.0f,0.5f));
		kindOfHuBkg->setPosition(Vec2(x,y));
		BarOfPlayer->addChild(kindOfHuBkg);
		auto kindOfHu = Sprite::createWithSpriteFrameName("gsp-js.png");//番型种类图片
		kindOfHu->setAnchorPoint(Point(0.0f,0.5f));
		kindOfHu->setPosition(Vec2(17,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(kindOfHu);
		auto timesOfHu=LabelAtlas::create("2","fonts/result_fan_number.png",22, 30, '0');//番数
		timesOfHu->setAnchorPoint(Vec2(0,0.5));
		timesOfHu->setPosition(Vec2(160,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(timesOfHu);//result_fx_text
		auto fanFont = Sprite::createWithSpriteFrameName("result_fx_text.png");//番字
		fanFont->setAnchorPoint(Point(0.0f,0.5f));
		fanFont->setPosition(Vec2(timesOfHu->getPosition().x+timesOfHu->getContentSize().width+6,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(fanFont);
		x+=kindOfHuBkg->getTextureRect().size.width+22;
		if(x>=900)
		{
			x=origin.x+visibleSize.width*0.17;
			y=origin.y+visibleSize.height*0.1256-10-56;
		}
	}
	if(num&RH_SHOUYIZHUA)
	{
		auto kindOfHuBkg = Sprite::createWithSpriteFrameName("result_fx_item_back.png");//背景
		kindOfHuBkg->setAnchorPoint(Point(0.0f,0.5f));
		kindOfHuBkg->setPosition(Vec2(x,y));
		BarOfPlayer->addChild(kindOfHuBkg);
		auto kindOfHu = Sprite::createWithSpriteFrameName("szy-js.png");//番型种类图片
		kindOfHu->setAnchorPoint(Point(0.0f,0.5f));
		kindOfHu->setPosition(Vec2(17,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(kindOfHu);
		auto timesOfHu=LabelAtlas::create("4","fonts/result_fan_number.png",22, 30, '0');//番数
		timesOfHu->setAnchorPoint(Vec2(0,0.5));
		timesOfHu->setPosition(Vec2(160,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(timesOfHu);//result_fx_text
		auto fanFont = Sprite::createWithSpriteFrameName("result_fx_text.png");//番字
		fanFont->setAnchorPoint(Point(0.0f,0.5f));
		fanFont->setPosition(Vec2(timesOfHu->getPosition().x+timesOfHu->getContentSize().width+6,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(fanFont);
		x+=kindOfHuBkg->getTextureRect().size.width+22;
		if(x>=900)
		{
			x=origin.x+visibleSize.width*0.17;
			y=origin.y+visibleSize.height*0.1256-10-56;
		}
	}
	if(num&RH_DASANYUAN)
	{
		auto kindOfHuBkg = Sprite::createWithSpriteFrameName("result_fx_item_back.png");//背景
		kindOfHuBkg->setAnchorPoint(Point(0.0f,0.5f));
		kindOfHuBkg->setPosition(Vec2(x,y));
		BarOfPlayer->addChild(kindOfHuBkg);
		auto kindOfHu = Sprite::createWithSpriteFrameName("dsy-js.png");//番型种类图片
		kindOfHu->setAnchorPoint(Point(0.0f,0.5f));
		kindOfHu->setPosition(Vec2(17,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(kindOfHu);
		auto timesOfHu=LabelAtlas::create("4","fonts/result_fan_number.png",22, 30, '0');//番数
		timesOfHu->setAnchorPoint(Vec2(0,0.5));
		timesOfHu->setPosition(Vec2(160,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(timesOfHu);//result_fx_text
		auto fanFont = Sprite::createWithSpriteFrameName("result_fx_text.png");//番字
		fanFont->setAnchorPoint(Point(0.0f,0.5f));
		fanFont->setPosition(Vec2(timesOfHu->getPosition().x+timesOfHu->getContentSize().width+6,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(fanFont);
		x+=kindOfHuBkg->getTextureRect().size.width+22;
		if(x>=900)
		{
			x=origin.x+visibleSize.width*0.17;
			y=origin.y+visibleSize.height*0.1256-10-56;
		}
	}
	if(num&RH_XIAOSANYUAN)
	{
		auto kindOfHuBkg = Sprite::createWithSpriteFrameName("result_fx_item_back.png");//背景
		kindOfHuBkg->setAnchorPoint(Point(0.0f,0.5f));
		kindOfHuBkg->setPosition(Vec2(x,y));
		BarOfPlayer->addChild(kindOfHuBkg);
		auto kindOfHu = Sprite::createWithSpriteFrameName("xsy-js.png");//番型种类图片
		kindOfHu->setAnchorPoint(Point(0.0f,0.5f));
		kindOfHu->setPosition(Vec2(17,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(kindOfHu);
		auto timesOfHu=LabelAtlas::create("2","fonts/result_fan_number.png",22, 30, '0');//番数
		timesOfHu->setAnchorPoint(Vec2(0,0.5));
		timesOfHu->setPosition(Vec2(160,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(timesOfHu);//result_fx_text
		auto fanFont = Sprite::createWithSpriteFrameName("result_fx_text.png");//番字
		fanFont->setAnchorPoint(Point(0.0f,0.5f));
		fanFont->setPosition(Vec2(timesOfHu->getPosition().x+timesOfHu->getContentSize().width+6,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(fanFont);
		x+=kindOfHuBkg->getTextureRect().size.width+22;
		if(x>=900)
		{
			x=origin.x+visibleSize.width*0.17;
			y=origin.y+visibleSize.height*0.1256-10-56;
		}
	}
	if(num&RH_MINGSIGUI)
	{
		auto kindOfHuBkg = Sprite::createWithSpriteFrameName("result_fx_item_back.png");//背景
		kindOfHuBkg->setAnchorPoint(Point(0.0f,0.5f));
		kindOfHuBkg->setPosition(Vec2(x,y));
		BarOfPlayer->addChild(kindOfHuBkg);
		auto kindOfHu = Sprite::createWithSpriteFrameName("msg-js.png");//番型种类图片
		kindOfHu->setAnchorPoint(Point(0.0f,0.5f));
		kindOfHu->setPosition(Vec2(17,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(kindOfHu);
		auto timesOfHu=LabelAtlas::create("2","fonts/result_fan_number.png",22, 30, '0');//番数
		timesOfHu->setAnchorPoint(Vec2(0,0.5));
		timesOfHu->setPosition(Vec2(160,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(timesOfHu);//result_fx_text
		auto fanFont = Sprite::createWithSpriteFrameName("result_fx_text.png");//番字
		fanFont->setAnchorPoint(Point(0.0f,0.5f));
		fanFont->setPosition(Vec2(timesOfHu->getPosition().x+timesOfHu->getContentSize().width+6,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(fanFont);
		x+=kindOfHuBkg->getTextureRect().size.width+22;
		if(x>=900)
		{
			x=origin.x+visibleSize.width*0.17;
			y=origin.y+visibleSize.height*0.1256-10-56;
		}
	}
	if(num&RH_ANSIGUI)
	{
		auto kindOfHuBkg = Sprite::createWithSpriteFrameName("result_fx_item_back.png");//背景
		kindOfHuBkg->setAnchorPoint(Point(0.0f,0.5f));
		kindOfHuBkg->setPosition(Vec2(x,y));
		BarOfPlayer->addChild(kindOfHuBkg);
		auto kindOfHu = Sprite::createWithSpriteFrameName("asg-js.png");//番型种类图片
		kindOfHu->setAnchorPoint(Point(0.0f,0.5f));
		kindOfHu->setPosition(Vec2(17,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(kindOfHu);
		auto timesOfHu=LabelAtlas::create("4","fonts/result_fan_number.png",22, 30, '0');//番数
		timesOfHu->setAnchorPoint(Vec2(0,0.5));
		timesOfHu->setPosition(Vec2(160,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(timesOfHu);//result_fx_text
		auto fanFont = Sprite::createWithSpriteFrameName("result_fx_text.png");//番字
		fanFont->setAnchorPoint(Point(0.0f,0.5f));
		fanFont->setPosition(Vec2(timesOfHu->getPosition().x+timesOfHu->getContentSize().width+6,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(fanFont);
		x+=kindOfHuBkg->getTextureRect().size.width+22;
		if(x>=900)
		{
			x=origin.x+visibleSize.width*0.17;
			y=origin.y+visibleSize.height*0.1256-10-56;
		}
	}
	if(num&RH_QIDUI)
	{
		auto kindOfHuBkg = Sprite::createWithSpriteFrameName("result_fx_item_back.png");//背景
		kindOfHuBkg->setAnchorPoint(Point(0.0f,0.5f));
		kindOfHuBkg->setPosition(Vec2(x,y));
		BarOfPlayer->addChild(kindOfHuBkg);
		auto kindOfHu = Sprite::createWithSpriteFrameName("qd-js.png");//番型种类图片
		kindOfHu->setAnchorPoint(Point(0.0f,0.5f));
		kindOfHu->setPosition(Vec2(17,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(kindOfHu);
		auto timesOfHu=LabelAtlas::create("4","fonts/result_fan_number.png",22, 30, '0');//番数
		timesOfHu->setAnchorPoint(Vec2(0,0.5));
		timesOfHu->setPosition(Vec2(160,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(timesOfHu);//result_fx_text
		auto fanFont = Sprite::createWithSpriteFrameName("result_fx_text.png");//番字
		fanFont->setAnchorPoint(Point(0.0f,0.5f));
		fanFont->setPosition(Vec2(timesOfHu->getPosition().x+timesOfHu->getContentSize().width+6,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(fanFont);
		x+=kindOfHuBkg->getTextureRect().size.width+22;
		if(x>=900)
		{
			x=origin.x+visibleSize.width*0.17;
			y=origin.y+visibleSize.height*0.1256-10-56;
		}
	}
	if(num&RH_SANYUANQIDUI)
	{
		auto kindOfHuBkg = Sprite::createWithSpriteFrameName("result_fx_item_back.png");//背景
		kindOfHuBkg->setAnchorPoint(Point(0.0f,0.5f));
		kindOfHuBkg->setPosition(Vec2(x,y));
		BarOfPlayer->addChild(kindOfHuBkg);
		auto kindOfHu = Sprite::createWithSpriteFrameName("syqd-js.png");//番型种类图片
		kindOfHu->setAnchorPoint(Point(0.0f,0.5f));
		kindOfHu->setPosition(Vec2(17,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(kindOfHu);
		auto timesOfHu=LabelAtlas::create("32","fonts/result_fan_number.png",22, 30, '0');//番数
		timesOfHu->setAnchorPoint(Vec2(0,0.5));
		timesOfHu->setPosition(Vec2(160,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(timesOfHu);//result_fx_text
		auto fanFont = Sprite::createWithSpriteFrameName("result_fx_text.png");//番字
		fanFont->setAnchorPoint(Point(0.0f,0.5f));
		fanFont->setPosition(Vec2(timesOfHu->getPosition().x+timesOfHu->getContentSize().width+6,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(fanFont);
		x+=kindOfHuBkg->getTextureRect().size.width+22;
		if(x>=900)
		{
			x=origin.x+visibleSize.width*0.17;
			y=origin.y+visibleSize.height*0.1256-10-56;
		}
	}
	if(num&RH_KAWUXIN)
	{
		auto kindOfHuBkg = Sprite::createWithSpriteFrameName("result_fx_item_back.png");//背景
		kindOfHuBkg->setAnchorPoint(Point(0.0f,0.5f));
		kindOfHuBkg->setPosition(Vec2(x,y));
		BarOfPlayer->addChild(kindOfHuBkg);
		auto kindOfHu = Sprite::createWithSpriteFrameName("kwx-js.png");//番型种类图片
		kindOfHu->setAnchorPoint(Point(0.0f,0.5f));
		kindOfHu->setPosition(Vec2(17,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(kindOfHu);
		auto timesOfHu=LabelAtlas::create("4","fonts/result_fan_number.png",22, 30, '0');//番数
		timesOfHu->setAnchorPoint(Vec2(0,0.5));
		timesOfHu->setPosition(Vec2(160,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(timesOfHu);//result_fx_text
		auto fanFont = Sprite::createWithSpriteFrameName("result_fx_text.png");//番字
		fanFont->setAnchorPoint(Point(0.0f,0.5f));
		fanFont->setPosition(Vec2(timesOfHu->getPosition().x+timesOfHu->getContentSize().width+6,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(fanFont);
		x+=kindOfHuBkg->getTextureRect().size.width+22;
		if(x>=900)
		{
			x=origin.x+visibleSize.width*0.17;
			y=origin.y+visibleSize.height*0.1256-10-56;
		}
	}
	if(num&RH_HAOHUAQIDUI)//双龙七对
	{
		auto kindOfHuBkg = Sprite::createWithSpriteFrameName("result_fx_item_back.png");//背景
		kindOfHuBkg->setAnchorPoint(Point(0.0f,0.5f));
		kindOfHuBkg->setPosition(Vec2(x,y));
		BarOfPlayer->addChild(kindOfHuBkg);
		auto kindOfHu = Sprite::createWithSpriteFrameName("hhqd-js.png");//番型种类图片
		kindOfHu->setAnchorPoint(Point(0.0f,0.5f));
		kindOfHu->setPosition(Vec2(17,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(kindOfHu);
		auto timesOfHu=LabelAtlas::create("8","fonts/result_fan_number.png",22, 30, '0');//番数
		timesOfHu->setAnchorPoint(Vec2(0,0.5));
		timesOfHu->setPosition(Vec2(160,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(timesOfHu);//result_fx_text
		auto fanFont = Sprite::createWithSpriteFrameName("result_fx_text.png");//番字
		fanFont->setAnchorPoint(Point(0.0f,0.5f));
		fanFont->setPosition(Vec2(timesOfHu->getPosition().x+timesOfHu->getContentSize().width+6,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(fanFont);
		x+=kindOfHuBkg->getTextureRect().size.width+22;
		if(x>=900)
		{
			x=origin.x+visibleSize.width*0.17;
			y=origin.y+visibleSize.height*0.1256-10-56;
		}
	}
	if(num&RH_CHAOHAOHUAQIDUI)
	{
		auto kindOfHuBkg = Sprite::createWithSpriteFrameName("result_fx_item_back.png");//背景
		kindOfHuBkg->setAnchorPoint(Point(0.0f,0.5f));
		kindOfHuBkg->setPosition(Vec2(x,y));
		BarOfPlayer->addChild(kindOfHuBkg);
		auto kindOfHu = Sprite::createWithSpriteFrameName("slqd-js.png");//番型种类图片
		kindOfHu->setAnchorPoint(Point(0.0f,0.5f));
		kindOfHu->setPosition(Vec2(17,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(kindOfHu);
		auto timesOfHu=LabelAtlas::create("32","fonts/result_fan_number.png",22, 30, '0');//番数
		timesOfHu->setAnchorPoint(Vec2(0,0.5));
		timesOfHu->setPosition(Vec2(160,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(timesOfHu);//result_fx_text
		auto fanFont = Sprite::createWithSpriteFrameName("result_fx_text.png");//番字
		fanFont->setAnchorPoint(Point(0.0f,0.5f));
		fanFont->setPosition(Vec2(timesOfHu->getPosition().x+timesOfHu->getContentSize().width+6,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(fanFont);
		x+=kindOfHuBkg->getTextureRect().size.width+22;
		if(x>=900)
		{
			x=origin.x+visibleSize.width*0.17;
			y=origin.y+visibleSize.height*0.1256-10-56;
		}
	}
	if(num&RH_CHAOCHAOHAOHUAQIDUI)
	{
		auto kindOfHuBkg = Sprite::createWithSpriteFrameName("result_fx_item_back.png");//背景
		kindOfHuBkg->setAnchorPoint(Point(0.0f,0.5f));
		kindOfHuBkg->setPosition(Vec2(x,y));
		BarOfPlayer->addChild(kindOfHuBkg);
		auto kindOfHu = Sprite::createWithSpriteFrameName("cjhhqd-js.png");//番型种类图片
		kindOfHu->setAnchorPoint(Point(0.0f,0.5f));
		kindOfHu->setPosition(Vec2(17,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(kindOfHu);
		auto timesOfHu=LabelAtlas::create("128","fonts/result_fan_number.png",22, 30, '0');//番数
		timesOfHu->setAnchorPoint(Vec2(0,0.5));
		timesOfHu->setPosition(Vec2(160,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(timesOfHu);//result_fx_text
		auto fanFont = Sprite::createWithSpriteFrameName("result_fx_text.png");//番字
		fanFont->setAnchorPoint(Point(0.0f,0.5f));
		fanFont->setPosition(Vec2(timesOfHu->getPosition().x+timesOfHu->getContentSize().width+6,kindOfHuBkg->getTextureRect().size.height/2));
		kindOfHuBkg->addChild(fanFont);
		x+=kindOfHuBkg->getTextureRect().size.width+22;
		if(x>=900)
		{
			x=origin.x+visibleSize.width*0.17;
			y=origin.y+visibleSize.height*0.1256-10-56;
		}
	}
}
void NetRaceLayer::raceAccount(float delta)
{
    LOGGER_WRITE("%s",__FUNCTION__);

	auto show_card_indicator=this->getChildByTag(SHOWCARD_INDICATOR_TAG_ID);
	show_card_indicator->setVisible(false);
	while(this->getChildByTag(GAME_BACK_BAR))
		this->removeChildByTag(GAME_BACK_BAR);
	if(this->getChildByTag(ROBOT_TUO_GUAN))
		this->removeChildByTag(ROBOT_TUO_GUAN,true);	
	if(this->getChildByTag(TUOGUAN_CANCEL_BUTTON))
		this->removeChildByTag(TUOGUAN_CANCEL_BUTTON,true);
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ResultImage.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("erqs_resultbox_btn_res.plist");
	auto raceAccoutLayer=Layer::create();//account::create();
	//auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
	//myframe->addChild(raceAccoutLayer,30,RACE_ACCOUT_TAG_ID);
	this->addChild(raceAccoutLayer,11,RACE_ACCOUT_TAG_ID);
	//raceAccoutLayer->runAction(FadeIn::create(0.3));
	auto BackGround=Sprite::create("racetable.png");//结算背景
	BackGround->setScale(1.189);
	BackGround->setAnchorPoint(Vec2(0.5,0.5));
	BackGround->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2 ));
	raceAccoutLayer->addChild(BackGround,0);

	auto Striae1=Sprite::create("tiaotu4.png");//下条纹
	Striae1->setAnchorPoint(Vec2(0.5,0));
	Striae1->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+0));
	raceAccoutLayer->addChild(Striae1,1);

	auto Striae2=Sprite::create("tiaotu3.png");//上条纹
	Striae2->setAnchorPoint(Vec2(0.5,1));
	int i=Striae2->getTextureRect().size.height;
	Striae2->setScaleY(201.6/Striae2->getTextureRect().size.height);
	Striae2->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height*0.718435));
	raceAccoutLayer->addChild(Striae2,1);

	auto zhuangPic=Sprite::create("tileImage/zhuang.png");//119  23
	zhuangPic->setAnchorPoint(Vec2(0.5,0.5));
	zhuangPic->setScale(0.8);
	//zhuangPic->setPosition(Vec2(origin.x+visibleSize.width*156/1218,origin.y+visibleSize.height*575/716));
	zhuangPic->setPosition(Vec2(119,origin.y+visibleSize.height-25));
	raceAccoutLayer->addChild(zhuangPic,3);

	auto BarOfPlayer0=LayerColor::create();
	BarOfPlayer0->setContentSize(Size(visibleSize.width,visibleSize.height*196.5/716));
	raceAccoutLayer->addChild(BarOfPlayer0,2,ACCOUNTBAR_PLAYERZERO);

	auto BarOfPlayer1=LayerColor::create();
	BarOfPlayer1->setContentSize(Size(visibleSize.width,visibleSize.height*196.5/716));
	raceAccoutLayer->addChild(BarOfPlayer1,2,ACCOUNTBAR_PLAYERONE);

	auto BarOfPlayer2=LayerColor::create();
	BarOfPlayer2->setContentSize(Size(visibleSize.width,visibleSize.height*196.5/716));
	raceAccoutLayer->addChild(BarOfPlayer2,2,ACCOUNTBAR_PLAYERTWO);	

	Vec2 AccountPlace[3];
	AccountPlace[0]=Vec2(origin.x,origin.y+visibleSize.height*519.5/716);
	AccountPlace[1]=Vec2(origin.x,origin.y+visibleSize.height*315.25/716);
	AccountPlace[2]=Vec2(origin.x,origin.y+visibleSize.height*111/716);

    int lastWinner = _roundManager->GetLastWinner();
	raceAccoutLayer->getChildByTag(lastWinner)->setPosition(AccountPlace[0]);
	raceAccoutLayer->getChildByTag((lastWinner+1)%3)->setPosition(AccountPlace[1]);
	raceAccoutLayer->getChildByTag((lastWinner+2)%3)->setPosition(AccountPlace[2]);

	unsigned int num;
	unsigned int numDoubule;
    
    WinInfo_t win;
    _roundManager->GetWin(win);

    auto WinBar=(LayerColor*)raceAccoutLayer->getChildByTag(win.player); 
    auto WinBarPlus=(LayerColor*)raceAccoutLayer->getChildByTag((win.player+1)%3);
    auto WinBarMinus=(LayerColor*)raceAccoutLayer->getChildByTag((win.player+2)%3);

    switch(win.kind) {
        case SINGLE_WIN:
            _roundManager->_players[win.player]->get_parter()->get_Hu_Flag(&num);
            AccountShows(WinBar,win.player);
            AccountShows(WinBarPlus,(win.player+1)%3);
            AccountShows(WinBarMinus,(win.player+2)%3);
            if(win.player==_roundManager->_curPlayer)
                WinBar->getChildByTag(ACCOUNT_ZIMO_FONT)->setVisible(true);
            else
            {
                WinBar->getChildByTag(ACCOUNT_HU_FONT)->setVisible(true);
                raceAccoutLayer->getChildByTag(_roundManager->_curPlayer)->getChildByTag(ACCOUNT_DIANPAO_FONT)->setVisible(true);
            }
            AccountHuKind(WinBar,num);
            break;
        case DOUBLE_WIN:
            _roundManager->_players[(_roundManager->_curPlayer+1)%3]->get_parter()->get_Hu_Flag(&num);
            _roundManager->_players[(_roundManager->_curPlayer+2)%3]->get_parter()->get_Hu_Flag(&numDoubule);
            AccountShows(WinBar,_roundManager->_curPlayer);
            AccountShows(WinBarPlus,(_roundManager->_curPlayer+1)%3);
            AccountShows(WinBarMinus,(_roundManager->_curPlayer+2)%3);
            
            WinBar->getChildByTag(ACCOUNT_DIANPAO_FONT)->setVisible(true);
            WinBarPlus->getChildByTag(ACCOUNT_HU_FONT)->setVisible(true);
            WinBarMinus->getChildByTag(ACCOUNT_HU_FONT)->setVisible(true);
            AccountHuKind(WinBarPlus,num);
            AccountHuKind(WinBarMinus,numDoubule);
            break;
        case NONE_WIN:
            if(_roundManager->_firstMingNo!=-1)
            {
                AccountShows(WinBar,_roundManager->_firstMingNo);
                AccountShows(WinBarPlus,(_roundManager->_firstMingNo+1)%3);
                AccountShows(WinBarMinus,(_roundManager->_firstMingNo+2)%3);
            
                WinBar->getChildByTag(ACCOUNT_BAOZHUANG_FONT)->setVisible(true);
            
                //float BHx=origin.x+visibleSize.width*0.17;
                float BHx=162;
                float BHy=origin.y+visibleSize.height*0.1256-10;
            
                auto kindOfHuBkgPlus = Sprite::createWithSpriteFrameName("result_fx_item_back.png");//背景
                kindOfHuBkgPlus->setAnchorPoint(Point(0.0f,0.5f));
                kindOfHuBkgPlus->setPosition(Vec2(BHx,BHy));
                WinBarPlus->addChild(kindOfHuBkgPlus);
                auto kindOfHuPlus= Sprite::createWithSpriteFrameName("bh-js.png");//番型种类图片
                kindOfHuPlus->setAnchorPoint(Point(0.0f,0.5f));
                kindOfHuPlus->setPosition(Vec2(17,kindOfHuBkgPlus->getTextureRect().size.height/2));
                kindOfHuBkgPlus->addChild(kindOfHuPlus);
                auto timesOfHuPlus=LabelAtlas::create("1","fonts/result_fan_number.png",22, 30, '0');//番数
                timesOfHuPlus->setAnchorPoint(Vec2(0,0.5));
                timesOfHuPlus->setPosition(Vec2(160,kindOfHuBkgPlus->getTextureRect().size.height/2));
                kindOfHuBkgPlus->addChild(timesOfHuPlus);//result_fx_text
                auto fanFontPlus= Sprite::createWithSpriteFrameName("result_fx_text.png");//番字
                fanFontPlus->setAnchorPoint(Point(0.0f,0.5f));
                fanFontPlus->setPosition(Vec2(timesOfHuPlus->getPosition().x+timesOfHuPlus->getContentSize().width+6,kindOfHuBkgPlus->getTextureRect().size.height/2));
                kindOfHuBkgPlus->addChild(fanFontPlus);
            
                auto kindOfHuBkgMinus= Sprite::createWithSpriteFrameName("result_fx_item_back.png");//背景
                kindOfHuBkgMinus->setAnchorPoint(Point(0.0f,0.5f));
                kindOfHuBkgMinus->setPosition(Vec2(BHx,BHy));
                WinBarMinus->addChild(kindOfHuBkgMinus);
                auto kindOfHu = Sprite::createWithSpriteFrameName("bh-js.png");//番型种类图片
                kindOfHu->setAnchorPoint(Point(0.0f,0.5f));
                kindOfHu->setPosition(Vec2(17,kindOfHuBkgMinus->getTextureRect().size.height/2));
                kindOfHuBkgMinus->addChild(kindOfHu);
                auto timesOfHu=LabelAtlas::create("1","fonts/result_fan_number.png",22, 30, '0');//番数
                timesOfHu->setAnchorPoint(Vec2(0,0.5));
                timesOfHu->setPosition(Vec2(160,kindOfHuBkgMinus->getTextureRect().size.height/2));
                kindOfHuBkgMinus->addChild(timesOfHu);//result_fx_text
                auto fanFont = Sprite::createWithSpriteFrameName("result_fx_text.png");//番字
                fanFont->setAnchorPoint(Point(0.0f,0.5f));
                fanFont->setPosition(Vec2(timesOfHu->getPosition().x+timesOfHu->getContentSize().width+6,kindOfHuBkgMinus->getTextureRect().size.height/2));
                kindOfHuBkgMinus->addChild(fanFont);
            }
            else
            {
                AccountShows(WinBar,_roundManager->_curPlayer);
                AccountShows(WinBarPlus,(_roundManager->_curPlayer+1)%3);
                AccountShows(WinBarMinus,(_roundManager->_curPlayer+2)%3);
            }
            break;
    }

	_eventDispatcher->removeCustomEventListeners(DISTRIBUTE_DONE_EVENT_TYPE);
	_eventDispatcher->removeCustomEventListeners(NOONE_WIN_EVENT_TYPE);
	auto ShowoutOnce=Button::create("xuanyaoyixia1.png","xuanyaoyixia2.png","xuanyaoyixia2.png",UI_TEX_TYPE_PLIST);//炫耀一下
	ShowoutOnce->addTouchEventListener(CC_CALLBACK_1(NetRaceLayer::display_callback,this));
	ShowoutOnce->setAnchorPoint(Vec2(0.5,0.5));
	ShowoutOnce->setPosition(Vec2(origin.x+visibleSize.width*0.2996716,origin.y+visibleSize.height*0.075419));
	this->addChild(ShowoutOnce,11,SHINE_TAG_ID);

	auto PlayOnceAgin=Button::create("zailaiyiju1.png","zailaiyiju2.png","zailaiyiju2.png",UI_TEX_TYPE_PLIST);
	PlayOnceAgin->addTouchEventListener(CC_CALLBACK_2(NetRaceLayer::restart_touchCallBack,this));
	PlayOnceAgin->setAnchorPoint(Vec2(0.5,0.5));
	PlayOnceAgin->setPosition(Vec2(origin.x+visibleSize.width*0.6247948,origin.y+visibleSize.height*0.075419));
	this->addChild(PlayOnceAgin,11,RACE_RESTART_TAG_ID);
}

void  NetRaceLayer::showall()
{
    LOGGER_WRITE("%s",__FUNCTION__);

    HideClock();
	auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
	float x,y;
	Sprite *p_list[MAX_HANDIN_NUM];

    int winner = _roundManager->GetLastWinner();
	for(int a=0;a<3;a++)
	{
		int no = (winner+a)%3;
        
		CARD_ARRAY *list=_roundManager->_players[no]->get_parter()->get_card_list();
		for(int ik=0;ik<MAX_HANDIN_NUM;ik++)
		{
			if(myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+no*20+ik))
				myframe->removeChildByTag(HAND_IN_CARDS_TAG_ID+no*20+ik,true);
		}
		x=base_point[no].x+10; 
		y=base_point[no].y+10;
		for(int i=0;i<list->len;i++)
		{
			if(list->data[i].kind!=ck_NOT_DEFINED )
			{
				if(no==0)
				{
					if(list->data[i].status==c_AN_GANG)
					{
						if(list->data[i].kind==list->data[i+1].kind&&list->data[i].kind!=list->data[i+2].kind)
						{
							p_list[i]=Sprite::createWithTexture(g_left_right_peng_out->getTexture());
							p_list[i]->setAnchorPoint(Vec2(0.3f,1.0f));
							p_list[i]->setPosition(Vec2(x-10,y-8));
							auto s_card=Sprite::createWithTexture(g_small_card_kind[list->data[i].kind]->getTexture());
							s_card->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height*0.65));
							s_card->setRotation(90);
							s_card->setScale(0.9);
							p_list[i]->addChild(s_card);
						}
						else
						{
							p_list[i]=Sprite::createWithTexture(g_left_right_angang->getTexture());
							p_list[i]->setAnchorPoint(Vec2(0.3f,1.0f));
							p_list[i]->setPosition(Vec2(x-10,y-8));
						}
					}
					else
					{
						p_list[i]=Sprite::createWithTexture(g_left_right_peng_out->getTexture());
						p_list[i]->setAnchorPoint(Vec2(0.3f,1.0f));
						p_list[i]->setPosition(Vec2(x-10,y-8));
						auto s_card=Sprite::createWithTexture(g_small_card_kind[list->data[i].kind]->getTexture());
						s_card->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height*0.65));
						s_card->setRotation(90);
						s_card->setScale(0.9);
						p_list[i]->addChild(s_card);
					}
					if(list->data[i].status==c_FREE||list->data[i].status==c_MING_KOU)
						y-=((p_list[i]->getTextureRect().size.height)*0.65);
					else if(list->data[i].status==c_PENG)
					{
						if((list->data[i+1].kind!=list->data[i].kind)||((list->data[i+1].kind==list->data[i].kind)&&(list->data[i+1].status!=list->data[i].status)))
							y-=((p_list[i]->getTextureRect().size.height)*0.65+5);
						else
							y-=((p_list[i]->getTextureRect().size.height)*0.65);
					}
					else if(list->data[i].status==c_MING_GANG||list->data[i].status==c_AN_GANG)
					{
						if((list->data[i].kind==list->data[i+1].kind)&&(list->data[i].kind==list->data[i+2].kind)&&(list->data[i].kind==list->data[i+3].kind)&&(list->data[i].kind!=list->data[i+4].kind))//1
							y-=((p_list[i]->getTextureRect().size.height)*0.65);
						else if((list->data[i].kind==list->data[i+1].kind)&&(list->data[i].kind==list->data[i+2].kind)&&(list->data[i].kind!=list->data[i+3].kind))//2
							y+=13;
						else if(list->data[i].kind==list->data[i+1].kind&&list->data[i].kind!=list->data[i+2].kind)//3
							y-=(((p_list[i]->getTextureRect().size.height)*0.65)+13);
						else if(list->data[i].kind!=list->data[i+1].kind)//4
						{
							if(list->data[i+1].kind!=list->data[i].kind)
								y-=((p_list[i]->getTextureRect().size.height)*0.65+5);
							else
								y-=((p_list[i]->getTextureRect().size.height)*0.65);
						}
					}
				}
				else if(no==1)
				{
					if(list->data[i].status==c_FREE||list->data[i].status==c_MING_KOU)
						p_list[i]=Sprite::createWithTexture(g_my_ming_ting->getTexture());
					else if(list->data[i].status==c_PENG||list->data[i].status==c_MING_GANG)
						p_list[i]=Sprite::createWithTexture(g_my_peng->getTexture());
					else if(list->data[i].status==c_AN_GANG)
					{
						if(list->data[i].kind==list->data[i+1].kind && list->data[i].kind!=list->data[i+2].kind)
							p_list[i]=Sprite::createWithTexture(g_my_peng->getTexture());
						else
							p_list[i]=Sprite::createWithTexture(g_my_angang->getTexture());
					}
					p_list[i]->setAnchorPoint(Vec2(0,0));
					p_list[i]->setPosition(Vec2(x,y));
					if(list->data[i].status==c_FREE)
					{
						auto s_card=Sprite::createWithTexture(g_card_kind[list->data[i].kind]->getTexture());
						s_card->setAnchorPoint(Vec2(0.5,0.5));
						s_card->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height*0.6));
						p_list[i]->addChild(s_card,1);
						x+=p_list[i]->getTextureRect().size.width*1.0;
					}
					else if(list->data[i].status==c_MING_KOU)
					{
						auto s_card=Sprite::createWithTexture(g_card_kind[list->data[i].kind]->getTexture());
						s_card->setAnchorPoint(Vec2(0.5,0.5));
						s_card->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height*0.6));
						p_list[i]->addChild(s_card,1);
						x += p_list[i]->getTextureRect().size.width*1;
					}
					else if(list->data[i].status==c_PENG)
					{
						auto s_card=Sprite::createWithTexture(g_mid_card_kind[list->data[i].kind]->getTexture());
						s_card->setAnchorPoint(Vec2(0.5,0.5));
						s_card->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height*0.6));
						p_list[i]->addChild(s_card,1);
						if(list->data[i+1].status==c_FREE)
							x += p_list[i]->getTextureRect().size.width*2;
						else if(list->data[i].kind!=list->data[i+1].kind &&(list->data[i+1].status!=c_FREE))
							x += p_list[i]->getTextureRect().size.width*1.5;
						else
							x += p_list[i]->getTextureRect().size.width*1.0;
					}
					else if(list->data[i].status==c_MING_GANG)
					{
						auto s_card=Sprite::createWithTexture(g_mid_card_kind[list->data[i].kind]->getTexture());
						s_card->setAnchorPoint(Vec2(0.5,0.5));
						s_card->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height*0.6));
						p_list[i]->addChild(s_card,1);
						if((list->data[i].kind==list->data[i+1].kind)&&(list->data[i].kind==list->data[i+2].kind)&&(list->data[i].kind==list->data[i+3].kind)&&(list->data[i].kind!=list->data[i+4].kind))//1
							x+=p_list[i]->getTextureRect().size.width*1.0;
						else if((list->data[i].kind==list->data[i+1].kind)&&(list->data[i].kind==list->data[i+2].kind)&&(list->data[i].kind!=list->data[i+3].kind))//2
							y+=17;
						else if(list->data[i].kind==list->data[i+1].kind&&list->data[i].kind!=list->data[i+2].kind)//3
						{
							x+=p_list[i]->getTextureRect().size.width*1.0;
							y-=17;
						}
						else if(list->data[i].kind!=list->data[i+1].kind)//4
						{
							if(list->data[i+1].status==c_FREE)
								x += p_list[i]->getTextureRect().size.width*2;
							else if(list->data[i].kind!=list->data[i+1].kind &&(list->data[i+1].status!=c_FREE))
								x += p_list[i]->getTextureRect().size.width*1.5;
							else
								x += p_list[i]->getTextureRect().size.width*1.0;
						}
					}
					else if(list->data[i].status==c_AN_GANG)
					{
						if((list->data[i].kind==list->data[i+1].kind)&&(list->data[i].kind==list->data[i+2].kind)&&(list->data[i].kind==list->data[i+3].kind)&&(list->data[i].kind!=list->data[i+4].kind))//1
							x+=p_list[i]->getTextureRect().size.width*1.0;
						else if((list->data[i].kind==list->data[i+1].kind)&&(list->data[i].kind==list->data[i+2].kind)&&(list->data[i].kind!=list->data[i+3].kind))//2
							y+=17;
						else if(list->data[i].kind==list->data[i+1].kind&&list->data[i].kind!=list->data[i+2].kind)//3
						{
							auto s_card=Sprite::createWithTexture(g_mid_card_kind[list->data[i].kind]->getTexture());
							s_card->setAnchorPoint(Vec2(0.5,0.5));
							s_card->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height*0.6));
							p_list[i]->addChild(s_card,1);
							x+=p_list[i]->getTextureRect().size.width*1.0;
							y-=17;
						}
						else if(list->data[i].kind!=list->data[i+1].kind)//4
						{
							if(list->data[i+1].status==c_FREE)
								x += p_list[i]->getTextureRect().size.width*2;
							else if(list->data[i].kind!=list->data[i+1].kind &&(list->data[i+1].status!=c_FREE))
								x += p_list[i]->getTextureRect().size.width*1.5;
							else
								x += p_list[i]->getTextureRect().size.width*1.0;
						}
					}
				}
				else if(no==2)
				{
					if(list->data[i].status==c_AN_GANG)
					{
						if((list->data[i].kind==list->data[i+1].kind)&&(list->data[i].kind==list->data[i+2].kind)&&(list->data[i].kind!=list->data[i+3].kind))//2
						{
							p_list[i]=Sprite::createWithTexture(g_left_right_peng_out->getTexture());
							p_list[i]->setAnchorPoint(Vec2(0.3,0));
							p_list[i]->setPosition(Vec2(x-10,y-8));
							auto s_card=Sprite::createWithTexture(g_small_card_kind[list->data[i].kind]->getTexture());
							s_card->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height*0.65));
							s_card->setRotation(-90);
							s_card->setScale(0.9);
							p_list[i]->addChild(s_card);
						}
						else
						{
							p_list[i]=Sprite::createWithTexture(g_left_right_angang->getTexture());
							p_list[i]->setAnchorPoint(Vec2(0.3,0));
							p_list[i]->setPosition(Vec2(x-10,y-8));
						}
					}
					else
					{
						p_list[i]=Sprite::createWithTexture(g_left_right_peng_out->getTexture());
						p_list[i]->setAnchorPoint(Vec2(0.3,0));
						p_list[i]->setPosition(Vec2(x-10,y-8));
						auto s_card=Sprite::createWithTexture(g_small_card_kind[list->data[i].kind]->getTexture());
						s_card->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height*0.65));
						s_card->setRotation(-90);
						s_card->setScale(0.9);
						p_list[i]->addChild(s_card);
					}
					if(list->data[i].status==c_FREE||list->data[i].status==c_MING_KOU)
						y+=((p_list[i]->getTextureRect().size.height)*0.65);
					else if(list->data[i].status==c_PENG)
					{
						if((list->data[i+1].kind!=list->data[i].kind)||((list->data[i+1].kind==list->data[i].kind)&&(list->data[i+1].status!=list->data[i].status)))
							y+=((p_list[i]->getTextureRect().size.height)*0.65+5);
						else
							y+=((p_list[i]->getTextureRect().size.height)*0.65);
					}
					else if(list->data[i].status==c_MING_GANG||list->data[i].status==c_AN_GANG)
					{
						if((list->data[i].kind==list->data[i+1].kind)&&(list->data[i].kind==list->data[i+2].kind)&&(list->data[i].kind==list->data[i+3].kind)&&(list->data[i].kind!=list->data[i+4].kind))//1
							y+=(p_list[i]->getTextureRect().size.height*0.65+13);
						else if((list->data[i].kind==list->data[i+1].kind)&&(list->data[i].kind==list->data[i+2].kind)&&(list->data[i].kind!=list->data[i+3].kind))//2
							y-=13;
						else if(list->data[i].kind==list->data[i+1].kind&&list->data[i].kind!=list->data[i+2].kind)//3
							y+=((p_list[i]->getTextureRect().size.height)*0.65);
						else if(list->data[i].kind!=list->data[i+1].kind)//4
						{
							if(list->data[i+1].kind!=list->data[i].kind)
								y+=((p_list[i]->getTextureRect().size.height)*0.65+5);
							else
								y+=((p_list[i]->getTextureRect().size.height)*0.65);
						}
					}
				}
				if(no!=2)
					myframe->addChild(p_list[i],i+1,HAND_IN_CARDS_TAG_ID+no*20+i);
				else if(no==2)
					myframe->addChild(p_list[i],list->len-i,HAND_IN_CARDS_TAG_ID+no*20+i);
				p_list[i]->_ID=1;
			}
		}
	}
}

Spawn* NetRaceLayer::simple_tip_effect(Vec2 v,std::string act_name)
{
    LOGGER_WRITE("%s",__FUNCTION__);

	auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
	auto bgOfGang=Sprite::createWithSpriteFrameName("mojixx.png");
	bgOfGang->setPosition(v);
	myframe->addChild(bgOfGang,20,MOJI_EFFECT_TAG_ID);
	bgOfGang->setScale(0);
	auto tipOfPeng=Sprite::createWithSpriteFrameName(act_name);
	tipOfPeng->setPosition(Vec2(bgOfGang->getTextureRect().size.width/2,bgOfGang->getTextureRect().size.height/2));
	bgOfGang->addChild(tipOfPeng);
	tipOfPeng->setScale(0);
	//auto seqActionOfTip=Sequence::create(DelayTime::create(0.8),Spawn::create(ScaleTo::create(0.3,0.8),FadeOut::create(0.3),NULL),NULL);
	//auto seqActionOfTip1=Sequence::create(EaseElasticOut::create(ScaleTo::create(0.3,1)),DelayTime::create(0.5),Spawn::create(ScaleTo::create(0.3,0.8),FadeOut::create(0.3),NULL),NULL);
	//auto action1=TargetedAction::create(bgOfGang,seqActionOfTip);
	//auto action2=TargetedAction::create(tipOfPeng,seqActionOfTip1);
	auto seqActionOfTip=Sequence::create(ScaleTo::create(0,1),DelayTime::create(0.6),Spawn::create(ScaleTo::create(0.18,0.8),FadeOut::create(0.18),NULL),NULL);
	auto seqActionOfTip1=Sequence::create(ScaleTo::create(0,1.5),DelayTime::create(0.12),EaseElasticOut::create(ScaleTo::create(0.18,1)),DelayTime::create(0.3),Spawn::create(ScaleTo::create(0.18,0.8),FadeOut::create(0.18),NULL),NULL);
	auto action1=TargetedAction::create(bgOfGang,seqActionOfTip);
	auto action2=TargetedAction::create(tipOfPeng,seqActionOfTip1);

	auto bgOfGang1=Sprite::createWithSpriteFrameName("mojixx.png");
	bgOfGang1->setPosition(v);
	myframe->addChild(bgOfGang1,21,MOJI1_EFFECT_TAG_ID);
	auto tipOfPeng1=Sprite::createWithSpriteFrameName(act_name);
	tipOfPeng1->setPosition(Vec2(bgOfGang1->getTextureRect().size.width/2,bgOfGang1->getTextureRect().size.height/2));
	bgOfGang1->addChild(tipOfPeng1);
	bgOfGang1->setScale(0);
	//auto actionOfTip=Sequence::create(DelayTime::create(0.3),ScaleTo::create(0,1),Spawn::create(ScaleTo::create(0.3,1.5),FadeOut::create(0.3),NULL),NULL);
	//auto actionOfTip1=Sequence::create(DelayTime::create(0.3),ScaleTo::create(0,1),Spawn::create(ScaleTo::create(0.3,1.5),FadeOut::create(0.3),NULL),NULL);
	//auto action3=TargetedAction::create(bgOfGang1,actionOfTip);
	//auto action4=TargetedAction::create(tipOfPeng1,actionOfTip1);
	auto actionOfTip=Sequence::create(DelayTime::create(0.12),ScaleTo::create(0,1),Spawn::create(ScaleTo::create(0.18,1.5),FadeOut::create(0.18),NULL),NULL);
	auto actionOfTip1=Sequence::create(DelayTime::create(0.12),ScaleTo::create(0,1),Spawn::create(ScaleTo::create(0.18,1.5),FadeOut::create(0.18),NULL),NULL);
	auto action3=TargetedAction::create(bgOfGang1,actionOfTip);
	auto action4=TargetedAction::create(tipOfPeng1,actionOfTip1);
	
	auto spa=Spawn::create(action1,action2,action3,action4,NULL);

	return spa;

}

void NetRaceLayer::hu_effect_tip(int no)
{
    LOGGER_WRITE("%s(%d)",__FUNCTION__,0);

	((Button*)this->getChildByTag(MENU_BKG_TAG_ID)->getChildByTag(TUOGUAN_MENU_BUTTON))->setTouchEnabled(false);

    _Remove(this,ROBOT_TUO_GUAN);
    _Remove(this,TUOGUAN_CANCEL_BUTTON);
    
	auto myframe = this->getChildByTag(GAME_BKG_TAG_ID);
	scheduleOnce(schedule_selector(NetRaceLayer::raceAccount),3);

	if(no<3) {
        _roundManager->SetWin(SINGLE_WIN,no);
        
		auto callfunc=CallFunc::create(this,callfunc_selector(NetRaceLayer::showall));

        CallFunc*HuVoice = _SpeakAction(HU);

        if(no!=1)
		{
			Spawn *simple_seq=simple_tip_effect(getEffectVec(_roundManager->_curPlayer),"dahu.png");
			auto GoldAccount=CallFunc::create([=](){
				GoldNumInsert(no,3,_roundManager->_curPlayer);	
			});
			auto hu_seq=Sequence::create(Spawn::create(HuVoice,simple_seq,NULL),GoldAccount,callfunc,NULL);
			myframe->runAction(hu_seq);
		}
		else
		{
			Sequence* g_seq1;
			if(_roundManager->_players[1]->get_parter()->get_ting_status()!=1)
			{
				auto shade_act=(Sprite*)myframe->getChildByTag(HU_REMIND_ACT_TAG_ID);
				auto fadeOut=FadeOut::create(0.3);
				auto easeBounce=ScaleTo::create(0.3,1.5);
				auto spawn=Spawn::create(fadeOut,easeBounce,NULL);
				auto seq=Sequence::create(ScaleTo::create(0,1),spawn,NULL);
				auto shadeAction=TargetedAction::create(shade_act,seq);
				auto sim_act=Spawn::create(shadeAction,CCCallFunc::create([=]()
				{
					for(int i=0;i<5;i++)
					{
						if(myframe->getChildByTag(REMIND_ACT_TAG_ID+i))
							myframe->removeChildByTag(REMIND_ACT_TAG_ID+i,true);
					}
					for(int i=6;i<11;i++)
					{
						if(myframe->getChildByTag(REMIND_ACT_TAG_ID+i))
							myframe->removeChildByTag(REMIND_ACT_TAG_ID+i,true);
					}
				}),NULL);
				g_seq1=Sequence::create(sim_act,CCCallFunc::create([=]()
				{
					myframe->removeChildByTag(HU_REMIND_ACT_TAG_ID,true);
				}),NULL);
			}

			auto centerExplode=Sprite::createWithSpriteFrameName("101.png");
			centerExplode->setPosition(Vec2(visibleSize.width/2,visibleSize.height/3));
			myframe->addChild(centerExplode,30,IMG_101_EFFECT_TAG_ID);
			centerExplode->setOpacity(200);
			centerExplode->setScale(0);
			auto animation = Animation::create();
			animation->addSpriteFrameWithFile("101.png");
			animation->addSpriteFrameWithFile("103.png");
			animation->addSpriteFrameWithFile("105.png");
			animation->addSpriteFrameWithFile("106.png");
			animation->setDelayPerUnit(0.05f);
			animation->setRestoreOriginalFrame(true);
			auto action = Animate::create(animation);
			auto action1=Repeat::create(action,3);
			auto l_seq1=Sequence::create(DelayTime::create(0.3),ScaleTo::create(0,1),Spawn::create(callfunc,action1,NULL),FadeOut::create(0.1),NULL);
			//auto l_seq1=Sequence::create(DelayTime::create(0.3),ScaleTo::create(0,1),action1,FadeOut::create(0.1),NULL);
			auto ll_action1=TargetedAction::create(centerExplode,l_seq1);

			auto lightCircle=Sprite::createWithSpriteFrameName("4.png");
			lightCircle->setPosition(Vec2(visibleSize.width/2,visibleSize.height/3));
			myframe->addChild(lightCircle,29,IMG_4_EFFECT_TAG_ID);
			lightCircle->setRotation(90);
			lightCircle->setScaleX(0);
			lightCircle->setScaleY(0);
			lightCircle->setOpacity(150);
			BlendFunc tmp_oBlendFunc ={GL_SRC_ALPHA, GL_ONE};
			lightCircle->setBlendFunc(tmp_oBlendFunc);
			auto l_seq2=Sequence::create(DelayTime::create(0.3),ScaleTo::create(0,0.2,0.6),Spawn::create(EaseSineOut::create(ScaleTo::create(0.5,0.4,4.0)),EaseSineIn::create(FadeOut::create(0.5)),NULL),NULL);
			auto ll_action2=TargetedAction::create(lightCircle,l_seq2);

			auto lightCircle2=Sprite::createWithSpriteFrameName("Q13.png");
			lightCircle2->setAnchorPoint(Vec2(0.5,0));
			lightCircle2->setOpacity(180);
			lightCircle2->setPosition(Vec2(visibleSize.width/2,visibleSize.height/3-20));
			myframe->addChild(lightCircle2,30,IMG_Q13_EFFECT_TAG_ID);
			lightCircle2->setScale(0);
			auto l_seq3=Sequence::create(DelayTime::create(0.3),ScaleTo::create(0,0.3),Spawn::create(EaseSineOut::create(ScaleTo::create(0.5,1.5)),FadeOut::create(0.5),NULL),NULL);
			auto ll_action3=TargetedAction::create(lightCircle2,l_seq3);

			auto lightCircle3=Sprite::createWithSpriteFrameName("Q13.png");
			lightCircle3->setAnchorPoint(Vec2(0.5,0));
			lightCircle2->setOpacity(180);
			lightCircle3->setPosition(Vec2(visibleSize.width/2,visibleSize.height/3-20));
			myframe->addChild(lightCircle3,30,IMG_Q13_2_EFFECT_TAG_ID);
			lightCircle3->setScale(0);
			auto l_seq4=Sequence::create(DelayTime::create(0.5),ScaleTo::create(0,0.26),Spawn::create(EaseSineOut::create(ScaleTo::create(0.5,1.5)),FadeOut::create(0.5),NULL),NULL);
			auto ll_action4=TargetedAction::create(lightCircle3,l_seq4);
			
			auto lightCircle4=Sprite::createWithSpriteFrameName("888.png");
			lightCircle4->setPosition(Vec2(visibleSize.width/2,visibleSize.height/3));
			lightCircle4->setScale(0);
			myframe->addChild(lightCircle4,29,IMG_888_EFFECT_TAG_ID);
			lightCircle4->setBlendFunc(tmp_oBlendFunc);
			auto l_seq5=Sequence::create(DelayTime::create(0.3),ScaleTo::create(0,1),Spawn::create(EaseSineOut::create(ScaleTo::create(0.3,5.5)),FadeOut::create(0.3),NULL),NULL);
			auto ll_action5=TargetedAction::create(lightCircle4,l_seq5);

			auto star=Sprite::createWithSpriteFrameName("104.png");
			star->setAnchorPoint(Vec2(0.5,0));
			star->setScale(0);
			star->setPosition(Vec2(visibleSize.width/2,visibleSize.height/3));
			myframe->addChild(star,29,IMG_104_EFFECT_TAG_ID);
			auto l_seq6=Sequence::create(DelayTime::create(0.3),Spawn::create(EaseSineOut::create(ScaleTo::create(0.5,0.7)),FadeOut::create(0.5),NULL),NULL);
			auto ll_action6=TargetedAction::create(star,l_seq6);
			
			auto flame=Sprite::createWithSpriteFrameName("444.png");
			flame->setAnchorPoint(Vec2(0,0.5));
			flame->setRotation(-90);
			flame->setPosition(Vec2(visibleSize.width/2,visibleSize.height/3));
			myframe->addChild(flame,29,IMG_444_EFFECT_TAG_ID);
			flame->setBlendFunc(tmp_oBlendFunc);
			flame->setScale(0);
			auto l_seq7=Sequence::create(DelayTime::create(0.3),Spawn::create(EaseSineOut::create(ScaleTo::create(0.5,1.0)),FadeOut::create(0.5),NULL),NULL);
			auto ll_action7=TargetedAction::create(flame,l_seq7);

			auto firecircle=Sprite::createWithSpriteFrameName("666.png");
			firecircle->setPosition(Vec2(visibleSize.width/2,visibleSize.height/3));
			firecircle->setTag(4);
			firecircle->setScale(0);
			firecircle->setOpacity(200);
			myframe->addChild(firecircle,29,IMG_666_EFFECT_TAG_ID);
			auto actionScale=ScaleTo::create(0.5,1.0);
			auto c_fadeOut=FadeOut::create(0.5);
			auto c_seq1=Spawn::create(EaseSineOut::create(actionScale),c_fadeOut,NULL);
			firecircle->setOpacity(100);
			firecircle->setBlendFunc(tmp_oBlendFunc);
			auto l_seq8=Sequence::create(DelayTime::create(0.3),ScaleTo::create(0,0.1),c_seq1,NULL);
			auto ll_action8=TargetedAction::create(firecircle,l_seq8);
			
			auto ll_action9=Sequence::create(DelayTime::create(0.3),CCCallFunc::create([=]()
			{
				startParticleSystem(0.3);	
			}),NULL);

			auto sorPos=this->getPosition();
			auto pShake1 = MoveTo::create(0.1,Vec2(sorPos.x+10,sorPos.y+10));
			auto pShake2 = MoveTo::create(0.1,Vec2(sorPos.x,sorPos.y));
			auto pShake3 = MoveTo::create(0.1,Vec2(sorPos.x-10,sorPos.y-10));
			auto pShake4 = MoveTo::create(0.1,Vec2(sorPos.x,sorPos.y));
			auto pShake5 = MoveTo::create(0.1,Vec2(sorPos.x+10,sorPos.y));
			auto pShake6 = MoveTo::create(0.1,Vec2(sorPos.x,sorPos.y));
			auto pShake7 = MoveTo::create(0.1,Vec2(sorPos.x,sorPos.y+10));
			auto pShake8 = MoveTo::create(0.1,Vec2(sorPos.x,sorPos.y));
			auto l_seq10=Sequence::create(DelayTime::create(0.3),pShake1,pShake2,pShake3,pShake4,pShake5,pShake6,pShake7,pShake8,NULL);
			auto ll_action10=TargetedAction::create(this,l_seq10);
			auto l_spa=Spawn::create(ll_action1,ll_action2,ll_action3,ll_action4,ll_action5,ll_action6,ll_action7,ll_action8,ll_action9,ll_action10,NULL);
			
			Spawn *simple_seq=simple_tip_effect(getEffectVec(_roundManager->_curPlayer),"dahu.png");
			auto GoldAccount=CallFunc::create([=](){
				GoldNumInsert(no,3,_roundManager->_curPlayer);	
			});
			Spawn* hu_seq;
			if(_roundManager->_players[1]->get_parter()->get_ting_status()!=1)
				hu_seq=Spawn::create(simple_seq,HuVoice,GoldAccount,Sequence::create(g_seq1,l_spa,NULL),NULL);
			else
				hu_seq=Spawn::create(simple_seq,HuVoice,GoldAccount,l_spa,NULL);
			myframe->runAction(hu_seq);
		}
	}
	else if(no==3)//两个对家双响
	{
        _roundManager->SetWin(DOUBLE_WIN,_roundManager->_curPlayer);
        
		auto _doublehucallListener = EventListenerCustom::create(DOUBLE_HU_WITH_ME, [this](EventCustom * event){
			auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
			auto callfunc=CallFunc::create(this,callfunc_selector(NetRaceLayer::showall));
			auto DoubleFileFunc=CallFunc::create([=]{
				_eventDispatcher->removeCustomEventListeners(DOUBLE_HU_WITH_ME);
				auto yiPaoShuangXiang=Sprite::create("ypsx.png");
				yiPaoShuangXiang->setAnchorPoint(Vec2(0.5,0.5));
				yiPaoShuangXiang->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height-yiPaoShuangXiang->getTextureRect().size.height/2));
				yiPaoShuangXiang->setOpacity(0);
				myframe->addChild(yiPaoShuangXiang,30);//字

				auto move=MoveTo::create(0.2,Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
				yiPaoShuangXiang->runAction(Sequence::create(Spawn::create(FadeIn::create(0.1),EaseIn::create(move,2),NULL),DelayTime::create(1),FadeOut::create(0.8),NULL));

				auto xian=LayerColor::create();
				xian->ignoreAnchorPointForPosition(false);
				xian->setAnchorPoint(Vec2(0.5,0.5));
				xian->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2-84));
				xian->setContentSize(Size(600,8));
				xian->setColor(Color3B(193,63,30));
				xian->setOpacity(100);
				xian->setScale(0);
				myframe->addChild(xian,10);
				xian->runAction(Sequence::create(DelayTime::create(1.2),ScaleTo::create(0,0),NULL));
			});
			auto GoldAccount=CallFunc::create([=](){
				GoldNumInsert(3,3,_roundManager->_curPlayer);	
			});
			auto HuFontNo=simple_tip_effect( getEffectVec((_roundManager->_curPlayer+1)%3),"dahu.png");
			auto HuFontNo1=simple_tip_effect( getEffectVec((_roundManager->_curPlayer+2)%3),"dahu.png");
			myframe->runAction(Sequence::create(Spawn::create(DoubleFileFunc,callfunc,HuFontNo,HuFontNo1,NULL),GoldAccount,NULL));
		});
		_eventDispatcher->addEventListenerWithFixedPriority(_doublehucallListener,2);
	}
}

void NetRaceLayer::hu_tip_effect(Node *psender)
{
	int no = psender->_ID;
		hu_effect_tip(no);
}

//int NetRaceLayer::get_cur_player_no()
//{
//	return _roundManager->_curPlayer;
//}
//void NetRaceLayer::set_cur_player_no(int no)
//{
//	_roundManager->_curPlayer=no;
//}

void NetRaceLayer::set_aims_sequence(const int p_aim[])
{
    LOGGER_WRITE("%s",__FUNCTION__);
	for(int i=0;i<3;i++)
		aim[i]=p_aim[i];
}

void NetRaceLayer::distribute_card_event()
{
	if(_roundManager->_distributedNum<TOTAL_CARD_NUM) {
        DCI scard;
		scard.card = CARD_KIND(_roundManager->_unDistributedCards[_roundManager->_distributedNum++]/4);
		scard.num = _roundManager->_distributedNum;
		_eventDispatcher->dispatchCustomEvent(DISTRIBUTE_DONE_EVENT_TYPE,&scard);
        LOGGER_WRITE("%s : %s",__FUNCTION__,DISTRIBUTE_DONE_EVENT_TYPE);
	}
	else {
		_eventDispatcher->dispatchCustomEvent(NOONE_WIN_EVENT_TYPE,NULL);
        LOGGER_WRITE("%s : %s",__FUNCTION__,NOONE_WIN_EVENT_TYPE);
    }   
}

void NetRaceLayer::race_start_again()
{
    LOGGER_WRITE("%s",__FUNCTION__);

	auto _waitstartListener = EventListenerCustom::create(WAIT_START_CALLBACK_EVENT_TYPE, [this](EventCustom * event){
        LOGGER_WRITE("got %s",WAIT_START_CALLBACK_EVENT_TYPE);
        _roundManager->Shuffle();
		_roundManager->_distributedNum = 40;
	});

	_eventDispatcher->addEventListenerWithFixedPriority(_waitstartListener, 3);

	auto _calldistributeListener = EventListenerCustom::create(DISTRIBUTE_CALL_EVENT_TYPE, [this](EventCustom * event){
			distribute_card_event();
		});
	_eventDispatcher->addEventListenerWithFixedPriority(_calldistributeListener, 2);
}

/**************************************************
        voice
**************************************************/
const std::string BoysMusicPath[21]={"Music/1tiao.ogg","Music/2tiao.ogg","Music/3tiao.ogg","Music/4tiao.ogg","Music/5tiao.ogg",
	"Music/6tiao.ogg","Music/7tiao.ogg","Music/8tiao.ogg","Music/9tiao.ogg","Music/1tong.ogg","Music/2tong.ogg","Music/3tong.ogg",
	"Music/4tong.ogg","Music/5tong.ogg","Music/6tong.ogg","Music/7tong.ogg","Music/8tong.ogg","Music/9tong.ogg","Music/zhong.ogg",
	"Music/fa.ogg","Music/bai.ogg"};
const std::string GirlsMusicPath[21]={"Music/g_1tiao.ogg","Music/g_2tiao.ogg","Music/g_3tiao.ogg","Music/g_4tiao.ogg","Music/g_5tiao.ogg",
	"Music/g_6tiao.ogg","Music/g_7tiao.ogg","Music/g_8tiao.ogg","Music/g_9tiao.ogg","Music/g_1tong.ogg","Music/g_2tong.ogg","Music/g_3tong.ogg",
	"Music/g_4tong.ogg","Music/g_5tong.ogg","Music/g_6tong.ogg","Music/g_7tong.ogg","Music/g_8tong.ogg","Music/g_9tong.ogg","Music/g_zhong.ogg",
	"Music/g_fa.ogg","Music/g_bai.ogg"};
const std::string BoysActionPath[4]={"Music/peng.ogg","Music/gang.ogg","Music/ting2.ogg","Music/hu.ogg"};
const std::string GirlsActionPath[4]={"Music/g_peng.ogg","Music/g_gang.ogg","Music/g_ting1.ogg","Music/g_hu.ogg"};

CallFunc* NetRaceLayer::_SpeakCard() {
    std::string sex;
    _roundManager->_players[_roundManager->_curPlayer]->get_sex(sex);
    
    if( sex=="Boy" )
		return CallFunc::create([=](){
                    SimpleAudioEngine::sharedEngine()->playEffect(BoysMusicPath[_roundManager->_lastHandedOutCard].c_str());
                });
    else
		return CallFunc::create([=](){
                    SimpleAudioEngine::sharedEngine()->playEffect(GirlsMusicPath[_roundManager->_lastHandedOutCard].c_str());
                });
        
}

CallFunc* NetRaceLayer::_SpeakAction(ActionType_t id) {
    std::string sex;
    _roundManager->_players[_roundManager->_curPlayer]->get_sex(sex);
    
    if( sex=="Boy" )
		return CallFunc::create([=](){SimpleAudioEngine::sharedEngine()->playEffect(BoysActionPath[id].c_str());});
	else
		return CallFunc::create([=](){SimpleAudioEngine::sharedEngine()->playEffect(GirlsActionPath[id].c_str());});
}

CallFunc* NetRaceLayer::_Speak(char *file) {
	return CallFunc::create([=](){SimpleAudioEngine::sharedEngine()->playEffect(file);});
}

/***********************************************************
        button action
***********************************************************/
void NetRaceLayer::tuoGuanCancelPressed(Ref* pSender,ui::Widget::TouchEventType type)
{
	auto curButton=(Button*)pSender;
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
            LOGGER_WRITE("%s",__FUNCTION__);
            
			if(this->getChildByTag(ROBOT_TUO_GUAN))
				this->removeChildByTag(ROBOT_TUO_GUAN,true);//TUOGUAN_CANCEL_BUTTON
			auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
			auto TuoGuanButton=(Button*)this->getChildByTag(MENU_BKG_TAG_ID)->getChildByTag(TUOGUAN_MENU_BUTTON);
			TuoGuanButton->setTouchEnabled(true);
			TuoGuanButton->setHighlighted(false);
			curButton->setTouchEnabled(false);
			curButton->setScale(0);
			ifTuoGuan=false;
			auto StartSelfGame=CallFunc::create([=](){
				if( !_roundManager->IsTing(1) )
					waitfor_MyTouchShowCard();
				else
					ListenToTingButton();
			});
			myframe->runAction(Sequence::create(CallFunc::create([=](){this->removeChildByTag(TUOGUAN_CANCEL_BUTTON,true);}),StartSelfGame,NULL));
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void NetRaceLayer::tuoGuanPressed(Ref* pSender,ui::Widget::TouchEventType type)
{
    auto curButton=(Button*)pSender;
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
            LOGGER_WRITE("%s",__FUNCTION__);
            
			auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
			_eventDispatcher->removeEventListenersForTarget(myframe,true);//???????
			ifTuoGuan=true;
			curButton->setTouchEnabled(false);
			curButton->setHighlighted(true);

			auto TuoGuanCancel=Button::create("quxiaotuoguan2.png","quxiaotuoguan1.png","quxiaotuoguan2.png",UI_TEX_TYPE_PLIST);
			TuoGuanCancel->addTouchEventListener(CC_CALLBACK_2(NetRaceLayer::tuoGuanCancelPressed,this));
			TuoGuanCancel->setAnchorPoint(Vec2(0.5,0.5));
			TuoGuanCancel->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height*60/716));
			this->addChild(TuoGuanCancel,19,TUOGUAN_CANCEL_BUTTON);

			auto layer_color=LayerColor::create();
			layer_color->setColor(Color3B(0,0,0));
			layer_color->ignoreAnchorPointForPosition(false);
			layer_color->setAnchorPoint(Vec2(0,0));
			layer_color->setPosition(Vec2(origin.x,origin.y));
			layer_color->setContentSize(Size(visibleSize.width,visibleSize.height*120/716));
			layer_color->setOpacity(150);
			this->addChild(layer_color,10,ROBOT_TUO_GUAN);
            
			if(myframe->getChildByTag(QI_REMIND_ACT_TAG_ID) && _roundManager->_isWaitDecision)
			{
				delete_act_tip();
				_roundManager->_isWaitDecision=false;
				_roundManager->_tempActionToDo=a_JUMP;
                
				if(_roundManager->_isCardFromOthers)
				{
					if(_roundManager->_isQiangGangAsking)
					{
						_roundManager->_isQiangGangAsking=false;
                        
						auto GoldAccount=CallFunc::create([=](){
							GoldNumInsert(_roundManager->_qiangGangTargetNo,2,_roundManager->_curPlayer);
							_roundManager->_qiangGangTargetNo=-1;
						});
						auto curFunc=CCCallFunc::create(this,callfunc_selector(NetRaceLayer::call_distribute_card));
						auto curSeq=Sequence::create(GoldAccount,curFunc,NULL);
						myframe->runAction(curSeq);
					}
					else if(_roundManager->_isDoubleHuAsking)
					{
						_roundManager->_isDoubleHuAsking = false;
						auto huFunc=CallFunc::create([=](){hu_effect_tip(_roundManager->_otherOneForDouble);});
						myframe->runAction(huFunc);
					}
					else
					{
						_roundManager->_curPlayer=(_roundManager->_curPlayer+1)%3;
						auto curFunc=CCCallFunc::create(this,callfunc_selector(NetRaceLayer::call_distribute_card));
						myframe->runAction(curFunc);
					}
				}
				else
					waitfor_MyShowCardInstruct();
			}
			else
			{
				if(ifMyShowCardTime)
				{
					ifMyShowCardTime=false;
					_roundManager->_actionToDo=a_JUMP;
					if(_roundManager->_lastAction==a_JUMP)
						continue_gang_times=0;
					_roundManager->_lastAction=a_JUMP;
					waitfor_MyShowCardInstruct();
				}
			}
			//else if(ifMyTime)
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void NetRaceLayer::BackConfirmPressed(Ref* pSender,ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			auto curButton=(Button*)pSender;
			curButton->setTouchEnabled(false);
			auto scene = Scene::create();
			auto startLayer=HelloWorld::create();
			scene->addChild(startLayer,1);
			Director::sharedDirector()->replaceScene(scene);
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void NetRaceLayer::BackCancelPressed(Ref* pSender,ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
			auto backBUtton=(Button*)this->getChildByTag(MENU_BKG_TAG_ID)->getChildByTag(GAMEBACK_MENU_BUTTON);
			backBUtton->setTouchEnabled(true);
			auto curButton=(Button*)pSender;
			curButton->setTouchEnabled(false);
			(this->getChildByTag(GAME_BACK_BAR))->runAction(ScaleTo::create(0,0));
			//auto ifEndGameChooseFunc=CallFunc::create([=](){ifEndGameChoose=false;});
			auto StartSelfGame=CallFunc::create([=](){
				if(ifGameStart)
				{
					if(!_roundManager->IsTing(1))
						waitfor_MyTouchShowCard();
					else
						ListenToTingButton();
				}
			});
			if(this->getChildByTag(TUOGUAN_CANCEL_BUTTON))
				((Button*)this->getChildByTag(TUOGUAN_CANCEL_BUTTON))->setTouchEnabled(true);
			this->runAction(Sequence::create(CallFunc::create([=](){this->removeChildByTag(GAME_BACK_BAR,true);}),StartSelfGame,/*ifEndGameChooseFunc,*/NULL));
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void NetRaceLayer::backPressed(Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
{
	auto curButton=(Button*)pSender;
	auto VoiceEffect=CallFunc::create([=](){SimpleAudioEngine::sharedEngine()->playEffect("Music/ui_click.ogg");});
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			curButton->setTouchEnabled(false);
			Back();
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void NetRaceLayer::Back()
{
    LOGGER_WRITE("%s",__FUNCTION__);

	SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("GlobalUI.plist");

	if(this->getChildByTag(TUOGUAN_CANCEL_BUTTON))
		((Button*)this->getChildByTag(TUOGUAN_CANCEL_BUTTON))->setTouchEnabled(false);
	while(this->getChildByTag(GAME_BACK_BAR))
		this->removeChildByTag(GAME_BACK_BAR);

	auto backChooseBkg=LayerColor::create();
	backChooseBkg->setContentSize(Size(visibleSize.width,visibleSize.height));
	backChooseBkg->setColor(Color3B(28,120,135));
	//backChooseBkg->setColor(Color3B(114,83,52));
	backChooseBkg->setOpacity(50);
	backChooseBkg->setPosition(Vec2::ZERO);
	this->addChild(backChooseBkg,20,GAME_BACK_BAR);

	EventListenerTouchOneByOne* backChooseListener=EventListenerTouchOneByOne::create();
	backChooseListener->setSwallowTouches(true);
	backChooseListener->onTouchBegan=[=](Touch* touch, Event* event){return true;};
	backChooseListener->onTouchMoved=[=](Touch* touch, Event* event){};
	backChooseListener->onTouchEnded=[=](Touch* touch, Event* event){};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(backChooseListener,backChooseBkg);

	auto backChooseBar=Sprite::createWithSpriteFrameName("gameprompt.png");
	backChooseBar->ignoreAnchorPointForPosition(false);
	backChooseBar->setAnchorPoint(Vec2(0.5,0.5));
	backChooseBar->setPosition(Vec2(backChooseBkg->getContentSize().width/2,backChooseBkg->getContentSize().height/2));
	backChooseBkg->addChild(backChooseBar,0);

	auto strings = CCDictionary::createWithContentsOfFile("fonts/NewString.xml");
	auto nameofTishi = ((CCString*)strings->objectForKey("GameBackString"))->getCString();
	auto kindOfHu = LabelTTF::create(nameofTishi, "Arial", 40);
	kindOfHu->setColor(Color3B(114,83,52));
	kindOfHu->setAnchorPoint(Vec2(0.5,0.5));
	kindOfHu->ignoreAnchorPointForPosition(false);
	kindOfHu->setPosition(Vec2(backChooseBar->getContentSize().width/2,backChooseBar->getContentSize().height*0.6+20));
	backChooseBar->addChild(kindOfHu,2);

	auto ensureBut=Button::create("tuichu2.png","tuichu.png","tuichu.png",UI_TEX_TYPE_PLIST);//退出
	ensureBut->addTouchEventListener(CC_CALLBACK_2(NetRaceLayer::BackConfirmPressed,this));
	ensureBut->setAnchorPoint(Vec2(0.5,0.5));
	ensureBut->setPosition(Vec2(backChooseBar->getContentSize().width*0.75-15,backChooseBar->getContentSize().height*0.3));
	backChooseBar->addChild(ensureBut);

	auto BackBut=Button::create("jixuyouxi2.png","jixuyouxi.png","jixuyouxi.png",UI_TEX_TYPE_PLIST);//继续游戏
	BackBut->addTouchEventListener(CC_CALLBACK_2(NetRaceLayer::BackCancelPressed,this));
	BackBut->setAnchorPoint(Vec2(0.5,0.5));
	BackBut->setPosition(Vec2(backChooseBar->getContentSize().width*0.25+15,backChooseBar->getContentSize().height*0.3));
	backChooseBar->addChild(BackBut);
}

/***********************************************
            graphical residue cards
***********************************************/
void NetRaceLayer::create_residue_cards() {
	residue_card_bkg=Sprite::createWithSpriteFrameName("Tile_reservedBG.png");
	residue_card_bkg->setAnchorPoint(Vec2(0.0f,1.0f));
	residue_card_bkg->setPosition(Vec2(5 + origin.x, visibleSize.height-5+origin.y));
	this->addChild(residue_card_bkg,1,RESERVED_BKG_TAG_ID);		

	auto residue_card_warning=LayerColor::create(Color4B(250,47,47,250),residue_card_bkg->getContentSize().width,residue_card_bkg->getContentSize().height);
	residue_card_warning->setScale(0);
	residue_card_warning->setOpacity(50);
	residue_card_warning->ignoreAnchorPointForPosition(false);
	residue_card_warning->setAnchorPoint(Vec2(0,1));
	residue_card_warning->setPosition(Vec2(origin.x+5,origin.y+visibleSize.height-5));
	this->addChild(residue_card_warning,2,RESERVED_BKG_CHILD_TAG_ID+5);

	auto residue_card=Sprite::createWithSpriteFrameName("Tile_reserved.png");
	residue_card->setAnchorPoint(Vec2(0.0f,0.5f));
	residue_card->setScale(0.9);
	residue_card->setPosition(Vec2(residue_card_bkg->getTextureRect().size.width/10,
        residue_card_bkg->getTextureRect().size.height/2-3.5));
	residue_card_bkg->addChild(residue_card,0,RESERVED_BKG_CHILD_TAG_ID+4);

	auto residue_card2=Sprite::createWithSpriteFrameName("Tile_reserved.png");
	residue_card2->setAnchorPoint(Vec2(0.0f,0.5f));
	residue_card2->setScale(0.9);
	residue_card2->setPosition(Vec2(residue_card_bkg->getTextureRect().size.width/10,
        residue_card_bkg->getTextureRect().size.height/2+3.5));
	residue_card_bkg->addChild(residue_card2,0,RESERVED_BKG_CHILD_TAG_ID+3);

	auto residue_card1=Sprite::createWithSpriteFrameName("Tile_reserved.png");
	residue_card1->setAnchorPoint(Vec2(0.0f,0.5f));
	residue_card1->setScale(0.9);
	residue_card1->setPosition(Vec2(residue_card_bkg->getTextureRect().size.width/10+19.4,
        residue_card_bkg->getTextureRect().size.height/2-3.5));
	residue_card_bkg->addChild(residue_card1,0,RESERVED_BKG_CHILD_TAG_ID+2);
}

void NetRaceLayer::refresh_residue_cards() {
    if(!residue_card_bkg->getChildByTag(RESERVED_BKG_CHILD_TAG_ID+4))
    {
        auto residue_card=Sprite::createWithSpriteFrameName("Tile_reserved.png");
        residue_card->setAnchorPoint(Vec2(0.0f,0.5f));
        residue_card->setScale(0.9);
        residue_card->setPosition(Vec2(residue_card_bkg->getTextureRect().size.width/10,
            residue_card_bkg->getTextureRect().size.height/2-3.5));
        residue_card_bkg->addChild(residue_card,0,RESERVED_BKG_CHILD_TAG_ID+4);
    }
    if(!residue_card_bkg->getChildByTag(RESERVED_BKG_CHILD_TAG_ID+3))
    {
        auto residue_card2=Sprite::createWithSpriteFrameName("Tile_reserved.png");
        residue_card2->setAnchorPoint(Vec2(0.0f,0.5f));
        residue_card2->setScale(0.9);
        residue_card2->setPosition(Vec2(residue_card_bkg->getTextureRect().size.width/10,
            residue_card_bkg->getTextureRect().size.height/2+3.5));
        residue_card_bkg->addChild(residue_card2,0,RESERVED_BKG_CHILD_TAG_ID+3);
    }
    if(!residue_card_bkg->getChildByTag(RESERVED_BKG_CHILD_TAG_ID+2))
    {
        auto residue_card1=Sprite::createWithSpriteFrameName("Tile_reserved.png");
        residue_card1->setAnchorPoint(Vec2(0.0f,0.5f));
        residue_card1->setScale(0.9);
        residue_card1->setPosition(Vec2(residue_card_bkg->getTextureRect().size.width/10+19.4,
            residue_card_bkg->getTextureRect().size.height/2-3.5));
        residue_card_bkg->addChild(residue_card1,0,RESERVED_BKG_CHILD_TAG_ID+2);
    }
}

void NetRaceLayer::update_residue_cards(int no)
{
    LOGGER_WRITE("%s : %d",__FUNCTION__,no);

	if(residue_card_bkg->getChildByTag(RESERVED_BKG_CHILD_TAG_ID))
		residue_card_bkg->removeChildByTag(RESERVED_BKG_CHILD_TAG_ID,true);
    
	if(residue_card_bkg->getChildByTag(RESERVED_BKG_CHILD_TAG_ID+1))
		residue_card_bkg->removeChildByTag(RESERVED_BKG_CHILD_TAG_ID+1,true);

	if(residue_card_bkg->getChildByTag(RESERVED_BKG_CHILD_TAG_ID+2))
	{
		auto residue_card1 = (Sprite*)residue_card_bkg->getChildByTag(RESERVED_BKG_CHILD_TAG_ID+2);
		residue_no_x = residue_card1->getPosition().x+residue_card1->getContentSize().width;
		residue_no_y = residue_card_bkg->getTextureRect().size.height/2;
	}
	else if(residue_card_bkg->getChildByTag(RESERVED_BKG_CHILD_TAG_ID+4))
	{
		auto residue_card = (Sprite*)residue_card_bkg->getChildByTag(RESERVED_BKG_CHILD_TAG_ID+4);
		residue_no_x = residue_card->getPosition().x+residue_card->getContentSize().width;
		residue_no_y = residue_card_bkg->getTextureRect().size.height/2;
	}

	auto str_card_Num=String::createWithFormat("%d",no); 
	auto str_card_NUM=LabelTTF::create(str_card_Num->getCString(),"Arial",24);
	str_card_NUM->setColor(Color3B(0x00,0xCD,0x00));
	str_card_NUM->setAnchorPoint(Vec2(0.0f,0.5f));
	str_card_NUM->setPosition(Vec2(residue_no_x,residue_no_y));
	residue_card_bkg->addChild(str_card_NUM,1,RESERVED_BKG_CHILD_TAG_ID);

	auto str_card_no=Sprite::create("zhang.png");//error==ming里面
	str_card_no->setAnchorPoint(Vec2(0,0.5));
	str_card_no->setScale(0.9);
	str_card_no->setPosition(Vec2(residue_no_x+str_card_NUM->getContentSize().width*1.1,residue_no_y));
	residue_card_bkg->addChild(str_card_no,1,RESERVED_BKG_CHILD_TAG_ID+1);

	if(no<=6)
	{
		str_card_NUM->runAction(TintTo::create(0,255,47,47));
        
		if(no<=3&&no>=0)
		{
			(this->getChildByTag(RESERVED_BKG_CHILD_TAG_ID+5))->runAction(Sequence::create(ScaleTo::create(0,1),Blink::create(1,4),ScaleTo::create(0,0),NULL));
			switch (no)
			{
			case 3:				
				str_card_NUM->runAction(Blink::create(1,4));
				break;
			case 2:
				if(residue_card_bkg->getChildByTag(RESERVED_BKG_CHILD_TAG_ID+2))
					residue_card_bkg->removeChildByTag(RESERVED_BKG_CHILD_TAG_ID+2,true);
				str_card_NUM->runAction(Blink::create(1,4));
				break;
			case 1:
				if(residue_card_bkg->getChildByTag(RESERVED_BKG_CHILD_TAG_ID+3))
					residue_card_bkg->removeChildByTag(RESERVED_BKG_CHILD_TAG_ID+3,true);
				str_card_NUM->runAction(Blink::create(1,4));
				break;
			case 0:
				if(residue_card_bkg->getChildByTag(RESERVED_BKG_CHILD_TAG_ID+4))
				{
					residue_card_bkg->removeChildByTag(RESERVED_BKG_CHILD_TAG_ID+4,true);
					if(residue_card_bkg->getChildByTag(RESERVED_BKG_CHILD_TAG_ID+1))
					{
						residue_card_bkg->removeChildByTag(RESERVED_BKG_CHILD_TAG_ID+1);
						auto str_card_no=Sprite::create("zhang2.png");//error==ming里面
						str_card_no->setAnchorPoint(Vec2(0,0.5));
						str_card_no->setScale(0.9);
						str_card_no->setPosition(Vec2(residue_no_x+str_card_NUM->getContentSize().width*1.1,residue_no_y));
						residue_card_bkg->addChild(str_card_no,1,RESERVED_BKG_CHILD_TAG_ID+1);
					}
				}
				str_card_NUM->runAction(Blink::create(1,4));
				break;
			default:
				break;	
			}
		}
	}
}

/*****************************************************
        player information
*****************************************************/
std::string NetRaceLayer::_NumToString( int number ) {
    char buf[80] = {0};
	float tempProperty;
	int tempXiapShu;

	if(number<100000) {
		sprintf(buf,"%d",number);
	} else if( number>=100000 && number<10000000 ) {
		tempXiapShu = number%1000;
        
		if(tempXiapShu>=500)
			tempProperty=(float)(number-500)/10000;
		else
			tempProperty=(float)number/10000;
        
		tempXiapShu=number%10000;

        if(tempXiapShu>=1000)
			sprintf(buf,"%.1f",tempProperty);
		else
			sprintf(buf,"%.0f",tempProperty);
	} else if(number>=10000000&&number<100000000) {
		tempXiapShu=number%10000;
		if(tempXiapShu>=5000)
			tempProperty=(float)(number-500)/10000;
		else
			tempProperty=(float)number/10000;
		sprintf(buf,"%.0f",tempProperty);
	} else if(number>=100000000) {
		tempXiapShu=number%10000000;
		if(tempXiapShu>=5000000)
			tempProperty=(float)(number-5000000)/100000000;
		else
			tempProperty=(float)number/100000000;
        
		tempXiapShu=number%100000000;
		if(tempXiapShu>=10000000)
			sprintf(buf,"%.1f",tempProperty);
		else
			sprintf(buf,"%.0f",tempProperty);
	}

    return std::string(buf);
}

void NetRaceLayer::_GuiUpdateScore(LayerColor *layer,int score) {
	LabelAtlas* label = LabelAtlas::create(
        _NumToString(score),
        "fonts/Head_Beans_Number.png",14,20,'.');  
	Sprite* propertyUnit;

	if( score < 100000 )
	{
		label->setAnchorPoint(Vec2(0.5,0.5));
		label->setPosition(Vec2(layer->getContentSize().width/2,layer->getContentSize().height/2));
		layer->addChild(label,1);
	}
	if( score>=100000 && score<100000000 )
	{
		propertyUnit=Sprite::createWithSpriteFrameName("wan-hand.png");
		propertyUnit->setAnchorPoint(Vec2(1,0.5));
		float curMargins=(layer->getContentSize().width-propertyUnit->getTextureRect().size.width-label->getContentSize().width)/2;
		propertyUnit->setPosition(Vec2(layer->getContentSize().width-curMargins,layer->getContentSize().height/2));
		layer->addChild(propertyUnit,1);
        
		label->setAnchorPoint(Vec2(0,0.5));
		label->setPosition(Vec2(curMargins,layer->getContentSize().height/2));
		layer->addChild(label,1);
	}
	else if( score>=100000000 )
	{
		propertyUnit=Sprite::createWithSpriteFrameName("yi-hand.png");
		propertyUnit->setAnchorPoint(Vec2(1,0.5));
		float curMargins=(layer->getContentSize().width-propertyUnit->getTextureRect().size.width-label->getContentSize().width)/2;
		propertyUnit->setPosition(Vec2(layer->getContentSize().width-curMargins,layer->getContentSize().height/2));
		layer->addChild(propertyUnit,1);

        label->setAnchorPoint(Vec2(0,0.5));
		label->setPosition(Vec2(curMargins,layer->getContentSize().height/2));
		layer->addChild(label,1);
	}
}

void NetRaceLayer::GuiUpdateScore(int dir,int score)
{
    if( !_playerBkg[dir] ) {
        return;
    }

	LayerColor *propertyLayerBar = LayerColor::create();
	propertyLayerBar->ignoreAnchorPointForPosition(false);

    if(_playerBkg[dir]->getChildByTag(SCORE_TAG_ID))
        _playerBkg[dir]->removeChildByTag(SCORE_TAG_ID,true);

    propertyLayerBar->setAnchorPoint(Vec2(0.5,0.5));
    
	if(dir==0) {
		propertyLayerBar->setContentSize( Size(_playerBkg[0]->getTextureRect().size.width, 25) );
		propertyLayerBar->setPosition( Vec2(_playerBkg[0]->getTextureRect().size.width*0.5,
            _playerBkg[0]->getTextureRect().size.height * 0.1));
	} else if(dir==1) {
		propertyLayerBar->setContentSize( Size(140, 25) );
		propertyLayerBar->setPosition( Vec2(_playerBkg[1]->getTextureRect().size.width*0.6635,
            _playerBkg[1]->getTextureRect().size.height*0.3));
	} else if(dir==2) {
		propertyLayerBar->setContentSize( Size(_playerBkg[2]->getTextureRect().size.width, 25) );
		propertyLayerBar->setPosition( Vec2(_playerBkg[2]->getTextureRect().size.width*0.5,
            _playerBkg[2]->getTextureRect().size.height * 0.1));
	}
    
    _playerBkg[dir]->addChild(propertyLayerBar,1,SCORE_TAG_ID);
    
    _GuiUpdateScore(propertyLayerBar,score);
}

void NetRaceLayer::update_nickname(int direction,std::string str_Nick)
{		
	if(direction==0)
	{
		if(!_playerBkg[0])
			return;
		if(_playerBkg[0]->getChildByTag(NICK_NAME_TAG_ID))
			_playerBkg[0]->removeChildByTag(NICK_NAME_TAG_ID,true);

		auto left_Name=Label::create(str_Nick,"Arial",20);
		left_Name->setPosition(Vec2(_playerBkg[0]->getTextureRect().size.width*0.5,_playerBkg[0]->getTextureRect().size.height*0.265));
		_playerBkg[0]->addChild(left_Name,1,NICK_NAME_TAG_ID);
	}
	else if(direction==1)
	{
		if(!_playerBkg[1])
			return;
		if(_playerBkg[1]->getChildByTag(NICK_NAME_TAG_ID))
			_playerBkg[1]->removeChildByTag(NICK_NAME_TAG_ID,true);

		auto myName=Label::create(str_Nick,"Arial",20);
		myName->setPosition(Vec2(_playerBkg[1]->getTextureRect().size.width*0.65,_playerBkg[1]->getTextureRect().size.height*0.7));
		_playerBkg[1]->addChild(myName,1,NICK_NAME_TAG_ID);

	}
	else if(direction==2)
	{
		if(!_playerBkg[2])
			return;
		if(_playerBkg[2]->getChildByTag(NICK_NAME_TAG_ID))
			_playerBkg[2]->removeChildByTag(NICK_NAME_TAG_ID,true);

		auto Right_Name=Label::create(str_Nick,"Arial",20);
		Right_Name->setPosition(Vec2(_playerBkg[2]->getTextureRect().size.width*0.5,_playerBkg[2]->getTextureRect().size.height*0.265));
		_playerBkg[2]->addChild(Right_Name,1,NICK_NAME_TAG_ID);
	}

}

void NetRaceLayer::update_headimage(int direction,std::string head_photo)
{
	auto head_image=Sprite::createWithSpriteFrameName(head_photo);

	if(direction==0)
	{
		if(!_playerBkg[0])
			return;

		if(_playerBkg[0]->getChildByTag(HEAD_IMG_TAG_ID))
			_playerBkg[0]->removeChildByTag(HEAD_IMG_TAG_ID,true);

		head_image->setScaleX(_playerBkg[0]->getTextureRect().size.width*0.85/head_image->getTextureRect().size.width);
		head_image->setScaleY(_playerBkg[0]->getTextureRect().size.width*0.85/head_image->getTextureRect().size.width);
		head_image->setAnchorPoint(Vec2(0.5f,1.0f));
		head_image->setPosition(Vec2(_playerBkg[0]->getTextureRect().size.width*1/2,_playerBkg[0]->getTextureRect().size.height*23/24));
		_playerBkg[0]->addChild(head_image,1,HEAD_IMG_TAG_ID);
	}
	else if(direction==1)
	{
		if(!_playerBkg[1])
			return;
		if(_playerBkg[1]->getChildByTag(HEAD_IMG_TAG_ID))
			_playerBkg[1]->removeChildByTag(HEAD_IMG_TAG_ID,true);

		float head_scale;
		head_scale=(_playerBkg[1]->getTextureRect().size.height*5)/(6*head_image->getTextureRect().size.height);
		head_image->setScaleX(head_scale);
		head_image->setScaleY(head_scale);
		head_image->setAnchorPoint(Vec2(0.0f,0.5f));
		head_image->setPosition(Vec2(_playerBkg[1]->getTextureRect().size.width/39,_playerBkg[1]->getTextureRect().size.height*1/2));
		_playerBkg[1]->addChild(head_image,1,HEAD_IMG_TAG_ID);
	}
	else if(direction==2)
	{
		if(!_playerBkg[2])
			return;
		if(_playerBkg[2]->getChildByTag(HEAD_IMG_TAG_ID))
			_playerBkg[2]->removeChildByTag(HEAD_IMG_TAG_ID,true);

		head_image->setScaleX(_playerBkg[0]->getTextureRect().size.width*0.85/head_image->getTextureRect().size.width);
		head_image->setScaleY(_playerBkg[0]->getTextureRect().size.width*0.85/head_image->getTextureRect().size.width);
		head_image->setAnchorPoint(Vec2(0.5f,1.0f));
		head_image->setPosition(Vec2(_playerBkg[2]->getTextureRect().size.width*1/2,_playerBkg[2]->getTextureRect().size.height*23/24));
		_playerBkg[2]->addChild(head_image,1,HEAD_IMG_TAG_ID);
	}
}

typedef enum {
    AN_GANG = 1,
    MING_GANG,
    HU_WIN,
}GoldKind_t;

void NetRaceLayer::_CalcAnGangGold(int winner,int goldOfPlayer[3]) {
    goldOfPlayer[winner]       = 4*premiumLeast*(continue_gang_times);
    goldOfPlayer[(winner+1)%3] = -2*premiumLeast*(continue_gang_times);
    goldOfPlayer[(winner+2)%3] = -2*premiumLeast*(continue_gang_times);
}

void NetRaceLayer::_CalcMingGangGold(int winner,int loser,int goldOfPlayer[3]) {
    if (winner==loser) {
        goldOfPlayer[winner]       = 2*premiumLeast*(continue_gang_times);
        goldOfPlayer[(winner+1)%3] = -1*premiumLeast*(continue_gang_times);
        goldOfPlayer[(winner+2)%3] = -1*premiumLeast*(continue_gang_times);
    } else {
        goldOfPlayer[winner]       =2*premiumLeast*(continue_gang_times);
        goldOfPlayer[loser]        =-2*premiumLeast*(continue_gang_times);
    }
}

void NetRaceLayer::_CalcSingleWinGold(int goldOfPlayer[3], int winner) {
    auto score = _roundManager->_players[winner]->get_parter()->get_card_score();
    goldOfPlayer[winner] = score*premiumLeast;
    
    if(_roundManager->_curPlayer==winner) {
        goldOfPlayer[(winner+1)%3] = -(goldOfPlayer[winner]/2);
        goldOfPlayer[(winner+2)%3] = -(goldOfPlayer[winner]/2);
    } else {
        goldOfPlayer[_roundManager->_curPlayer] = -goldOfPlayer[winner];
    }
    
    goldOfPlayer[(winner+1)%3] = goldOfPlayer[(winner+1)%3] + goldOfPlayer[(winner+1)%3]*_roundManager->IsTing((winner+1)%3);
    goldOfPlayer[(winner+2)%3] = goldOfPlayer[(winner+2)%3] + goldOfPlayer[(winner+2)%3]*_roundManager->IsTing((winner+2)%3);
    goldOfPlayer[winner] = - (goldOfPlayer[(winner+1)%3] + goldOfPlayer[(winner+2)%3]);
}

void NetRaceLayer::_CalcDoubleWinGold(int goldOfPlayer[3], int loser) {
    for(int i=1;i<3;i++) {
        auto score = _roundManager->_players[(loser+i)%3]->get_parter()->get_card_score();
        int  ting  = _roundManager->IsTing((loser+i)%3);

        goldOfPlayer[(loser+i)%3] = score*premiumLeast + score*premiumLeast*ting;
    }

    goldOfPlayer[loser] = - ((goldOfPlayer[(loser+1)%3] + goldOfPlayer[(loser+2)%3]));
}

void NetRaceLayer::_CalcNoneWinGold(int goldOfPlayer[3], int loser) {
    GoldAccountImmediate[(loser+1)%3] = premiumLeast;
    GoldAccountImmediate[(loser+2)%3] = premiumLeast;
    GoldAccountImmediate[loser] = - ((goldOfPlayer[(loser+1)%3] + goldOfPlayer[(loser+2)%3]));
}

void NetRaceLayer::_CalcHuGold(int goldOfPlayer[3]) {
    WinInfo_t win;
    _roundManager->GetWin(win);
    
    switch(win.kind) {
        case SINGLE_WIN:
            _CalcSingleWinGold(goldOfPlayer,win.player);
            break;
        case DOUBLE_WIN:
            _CalcDoubleWinGold(goldOfPlayer,win.player);
            break;
        case NONE_WIN:
            _CalcNoneWinGold(goldOfPlayer,win.player);
            break;
    }
}

void NetRaceLayer::CalculateGoldNum(int goldOfPlayer[3],int GoldWinner,int Gold_kind,int who_give) {
	goldOfPlayer[0]=0;
	goldOfPlayer[1]=0;
	goldOfPlayer[2]=0;

	if(Gold_kind==AN_GANG) {
		_CalcAnGangGold(GoldWinner,goldOfPlayer);
	} else if(Gold_kind==MING_GANG) {
        _CalcMingGangGold(GoldWinner,who_give,goldOfPlayer);
	} else if(Gold_kind==HU_WIN) {
    	_CalcHuGold(goldOfPlayer);
	}
}

void NetRaceLayer::_UpdateGouldAccount(int id,int gold) {
    Database *database = Database::getInstance();

    int total;
    _roundManager->_players[id]->get_property(total);
    total += gold;
    _roundManager->_players[id]->set_property(total);
    
    int globalId = 0;
    _roundManager->_players[id]->get_player_id(globalId);
    database->SetProperty(globalId,total);
}

void NetRaceLayer::UpdateGoldAccounts(int goldOfPlayer[3]) {
    for(int i=0;i<3;i++) {
        _UpdateGouldAccount(i,goldOfPlayer[i]);
    }
}

Vec2 NetRaceLayer::_AnchorOfSign(int dir) {
    switch(dir) {
        case 0:   return Vec2(0.5,0.5);
        case 1:   return Vec2(0.5,0.5);
        case 2:   return Vec2(1,0.5);
    }
}

Vec2 NetRaceLayer::_PositionOfSign(int dir,Size size,Vec2 origin,int xOffset) {
    switch(dir) {
        case 0:   
            return Vec2(origin.x + size.width*271/1218,
                        origin.y + size.height*406/716);
        case 1:   
            return Vec2(origin.x + size.width*559/1218,
                        origin.y + size.height*154/716);
        case 2:   
            return Vec2(origin.x + size.width*996/1218 - xOffset,
                        origin.y + size.height*406/716);
    }
}

Vec2 NetRaceLayer::_DestOfSign(int dir,Size size,Vec2 origin,int xOffset) {
    switch(dir) {
        case 0:   
            return Vec2(origin.x + size.width*271/1218,
                        origin.y + size.height*480/716);
        case 1:   
            return Vec2(origin.x + size.width*559/1218,
                        origin.y + size.height*228/716);
        case 2:   
            return Vec2(origin.x + size.width*996/1218 - xOffset,
                        origin.y + size.height*480/716);
    }
}

Vec2 NetRaceLayer::_AnchorOfNumber(int dir) {
    switch(dir) {
        case 0:   return Vec2(0,0.5);
        case 1:   return Vec2(0,0.5);
        case 2:   return Vec2(1,0.5);
    }
}

Vec2 NetRaceLayer::_PositionOfNumber(int dir,Size size,Vec2 origin) {
    switch(dir) {
        case 0:   
            return Vec2(origin.x + size.width*291/1218,
                        origin.y + size.height*406/716);
        case 1:   
            return Vec2(origin.x + size.width*578/1218,
                        origin.y + size.height*154/716);
        case 2:   
            return Vec2(origin.x + size.width*996/1218,
                        origin.y + size.height*406/716);
    }
}

Vec2 NetRaceLayer::_DestOfNumber(int dir,Size size,Vec2 origin) {
    switch(dir) {
        case 0:   
            return Vec2(origin.x + size.width*291/1218,
                        origin.y + size.height*480/716);
        case 1:   
            return Vec2(origin.x + size.width*578/1218,
                        origin.y + size.height*228/716);
        case 2:   
            return Vec2(origin.x + size.width*996/1218,
                        origin.y + size.height*480/716);
    }
}

Vec2 NetRaceLayer::_AnchorOfGold(int dir) {
    switch(dir) {
        case 0:   return Vec2(0.5,0.5);
        case 1:   return Vec2(0.5,0.5);
        case 2:   return Vec2(0.5,0.5);
    }
}

Vec2 NetRaceLayer::_PositionOfGold(int dir,Size size,Vec2 origin,int xOffset) {
    switch(dir) {
        case 0:   
            return Vec2(origin.x + size.width*221/1218,
                        origin.y + size.height*406/716);
        case 1:   
            return Vec2(origin.x + size.width*509/1218,
                        origin.y + size.height*154/716);
        case 2:   
            return Vec2(origin.x + size.width*936/1218 - xOffset,
                        origin.y + size.height*406/716);
                
    }
}

Vec2 NetRaceLayer::_DestOfGold(int dir,Size size,Vec2 origin,int xOffset) {
    switch(dir) {
        case 0:   
            return Vec2(origin.x + size.width*221/1218,
                        origin.y + size.height*480/716);
        case 1:   
            return Vec2(origin.x + size.width*509/1218,
                        origin.y + size.height*228/716);
        case 2:   
            return Vec2(origin.x + size.width*936/1218 - xOffset,
                        origin.y + size.height*480/716);
    }
}

void NetRaceLayer::GuiJinBiShow(int dir, int gold) {
	Size size = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	char str[15];
	sprintf(str,"%d",abs(gold));
	auto number=LabelAtlas::create(std::string(str),"fonts/moneyMessage.png",28,39,'0');
	number->setAnchorPoint(_AnchorOfNumber(dir));
	number->setOpacity(0);
	number->setPosition(_PositionOfNumber(dir,size,origin));
	this->addChild(number,6,GOLD_NUM_INSERT_NUMBER);

    int xoffset = number->getContentSize().width;
    
	auto minus=Sprite::create("-.png");
	minus->setAnchorPoint(_AnchorOfSign(dir));
	minus->setPosition(_PositionOfSign(dir,size,origin,xoffset));
	minus->setOpacity(0);
	this->addChild(minus,6,GOLD_NUM_INSERT_JIANHAO);

	auto plus=Sprite::create("+.png");
	plus->setAnchorPoint(_AnchorOfSign(dir));
	plus->setPosition(_PositionOfSign(dir,size,origin,xoffset));
	plus->setOpacity(0);
	this->addChild(plus,6,GOLD_NUM_INSERT_JIAHAO);

	auto jinbi=Sprite::create("jinbi-game.png");
	jinbi->setAnchorPoint(_AnchorOfGold(dir));
	jinbi->setOpacity(0);
	jinbi->setPosition(_PositionOfGold(dir,size,origin,xoffset));
	this->addChild(jinbi,6,GOLD_NUM_INSERT_JINBI);

    Sprite *symbol;
    if( gold>0 ) {
        symbol = plus;
    } else if( gold<0 ) {
        symbol = minus;
    } else {// is the upper procedures necessary when gold==0 ???
        return ;
    }

	jinbi->runAction(Sequence::create(
        DelayTime::create(0),
        Spawn::create(FadeIn::create(1.5),
        MoveTo::create(1.5,_DestOfGold(dir,size,origin,xoffset)),NULL),
        ScaleTo::create(0,0),NULL));
    
	number->runAction(Sequence::create(
        DelayTime::create(0),
        Spawn::create(FadeIn::create(1.5),
        MoveTo::create(1.5,_DestOfNumber(dir,size,origin)),NULL),
        ScaleTo::create(0,0),NULL));

    symbol->runAction(Sequence::create(
        DelayTime::create(0),
        Spawn::create(FadeIn::create(1.5),
        MoveTo::create(1.5,_DestOfSign(dir,size,origin,xoffset)),NULL),
        ScaleTo::create(0,0),NULL));
}

void NetRaceLayer::GoldNumInsert(int GoldWinner,int Gold_kind,int who_give)
{
    CalculateGoldNum(GoldAccountImmediate,GoldWinner,Gold_kind,who_give);
    UpdateGoldAccounts(GoldAccountImmediate);

	for(int id=0;id<3;id++) {
        int score;
        _roundManager->_players[id]->get_property(score);
        GuiUpdateScore(id,score);
        GuiJinBiShow(id,GoldAccountImmediate[id]);        
	}
}



void NetRaceLayer::GuiShowReady(int dir)
{
	auto Ready = Sprite::createWithSpriteFrameName("zhunbei.png");
	Ready->setAnchorPoint(Vec2(0.5,1));
	Ready->setScale(0.8);

	if( dir==0 ) {
		Ready->setPosition(Vec2(
            _playerBkg[0]->getPosition().x + _playerBkg[0]->getContentSize().width/2,
            _playerBkg[0]->getPosition().y - _playerBkg[0]->getContentSize().height));
		this->addChild(Ready,2,READY_INDICATE_LEFT_TAG_ID);
	} else if( dir==1 ) {
		;
	} else if( dir==2 ) {
		Ready->setPosition(Vec2(
            _playerBkg[2]->getPosition().x - _playerBkg[2]->getContentSize().width/2,
            _playerBkg[2]->getPosition().y - _playerBkg[2]->getContentSize().height));
		this->addChild(Ready,2,READY_INDICATE_RIGHT_TAG_ID);
	}
}

/****************************************************
    clock
****************************************************/
void NetRaceLayer::_ClockAddTime( Sprite *clock, int time ) {
    Sprite *p = (Sprite *)clock;

	char strTime[16] = {0};
	sprintf(strTime,"%d",time);
    
	auto labelTime = LabelTTF::create( std::string(strTime), "Arial", 30 );
	labelTime->setAnchorPoint(Vec2(0.5,0.5));
	labelTime->setColor(Color3B(0,0,0));
	labelTime->setPosition(Vec2(p->getTextureRect().size.width/2,p->getTextureRect().size.height*0.55));

    clock->addChild(labelTime,0,ALARM_CLOCK_CHILD_NUM_TAG_ID);
}

void NetRaceLayer::_Remove(Node *parent, int childTag) {
    if(parent->getChildByTag(childTag)) {// this judgement is not necessary becaust the interface will do the same thing!!!
        parent->removeChildByTag(childTag,true);
    }
}

void NetRaceLayer::_Show(Node *parent, int childTag, bool flag) {
    if(parent->getChildByTag(childTag)) {
        parent->getChildByTag(childTag)->setVisible(flag);
    }
}

bool NetRaceLayer::_IsClickedOn(Node* button,Touch* touch) {
    float x      = button->getPosition().x;
    float y      = button->getPosition().y;
    Size size    = button->getContentSize();
    Rect rect    = Rect(x,y,size.width,size.height);
    
    if( rect.containsPoint( touch->getLocation() ) ) {
        return true;
    } else {
        return false;
    }
}

void NetRaceLayer::HideClock() {
    Node* indicator[4] = {0};
    
    indicator[0]  = this->getChildByTag(ALARM_CLOCK_INDICATE_LEFT_TAG_ID);
    indicator[1]  = this->getChildByTag(ALARM_CLOCK_INDICATE_DOWN_TAG_ID);
    indicator[2]  = this->getChildByTag(ALARM_CLOCK_INDICATE_RIGHT_TAG_ID);
    indicator[3]  = this->getChildByTag(ALARM_CLOCK_INDICATOR_TAG_ID);
    
    for(int i=0;i<4;i++) {
        if( indicator[i] ) {
            indicator[i]->setVisible(false);
        }
    }
}

void NetRaceLayer::UpdateClock(int time,int dir){		
    Node* indicator[4] = {0};
    
    indicator[0]  = this->getChildByTag(ALARM_CLOCK_INDICATE_LEFT_TAG_ID);
    indicator[1]  = this->getChildByTag(ALARM_CLOCK_INDICATE_DOWN_TAG_ID);
    indicator[2]  = this->getChildByTag(ALARM_CLOCK_INDICATE_RIGHT_TAG_ID);
    indicator[3]  = this->getChildByTag(ALARM_CLOCK_INDICATOR_TAG_ID);
    
    for(int i=0;i<4;i++) {
        if( !indicator[i] ) {
            return;
        }
    }

    indicator[3]->setVisible(true);
    _Remove(indicator[3],ALARM_CLOCK_CHILD_NUM_TAG_ID);
    _ClockAddTime((Sprite *)indicator[3],time);

    indicator[dir]->setVisible(true);
    indicator[(dir+1)%3]->setVisible(false);
    indicator[(dir+2)%3]->setVisible(false);
}

void NetRaceLayer::delete_ActionRemind()
{
    LOGGER_WRITE("%s",__FUNCTION__);

	auto myframe = this->getChildByTag(GAME_BKG_TAG_ID);

        
	for(int i=0;i<17;i++){
        _Remove(myframe,REMIND_ACT_TAG_ID+i);
	}
}

void NetRaceLayer::delete_ActionEffect()
{
    LOGGER_WRITE("%s",__FUNCTION__);

	auto myframe=this->getChildByTag(GAME_BKG_TAG_ID);
    
 	for(int i=0;i<31;i++) {
        _Remove(myframe,MOJI_EFFECT_TAG_ID+i);
	}
}

void NetRaceLayer::delete_act_tip()
{
    LOGGER_WRITE("%s",__FUNCTION__);

	delete_ActionRemind();
    delete_ActionEffect();    
}


/****************************************************
    effect position
****************************************************/
Vec2 NetRaceLayer::_OrigPositionOfGangCard(int i,Size size) {
    switch(i+1) {
        case 1: return Vec2(
            origin.x + visibleSize.width*0.5 + size.width*0.98,
            origin.y + visibleSize.height*0.255)
        case 2: return Vec2(
            origin.x + visibleSize.width*0.5 - size.width*1.96,
            origin.y + visibleSize.height*0.255)
        case 3: return Vec2(
            origin.x + visibleSize.width*0.5 - size.width*0.98,
            origin.y + visibleSize.height*0.255)
        case 4: return Vec2(
            origin.x + visibleSize.width*0.5,
            origin.y + visibleSize.height*0.255)
    }
}

Vec2 NetRaceLayer::_MiddlePositionOfGangCard(int i,Size size) {
    switch(i+1) {
        case 1: return Vec2(
                origin.x + visibleSize.width*0.5  +  size.width*0.98  +  150,
                origin.y + visibleSize.height*0.255)
        case 2: return Vec2(
			    origin.x + visibleSize.width*0.5 - size.width*1.96 - 150,
			    origin.y + visibleSize.height*0.255)
        case 3: return Vec2(
			    origin.x + visibleSize.width*0.5 - size.width*0.98 - 150,
			    origin.y + visibleSize.height*0.255)
        case 4: return Vec2(
			    origin.x + visibleSize.width*0.5 - 150,
			    origin.y + visibleSize.height*0.255)
    }
}

Vec2 NetRaceLayer::_DestPositionOfGangCard(int i,Size size) {
    switch(i+1) {
        case 1: return Vec2(
                origin.x + visibleSize.width*0.5  +  size.width*0.98,
                origin.y + visibleSize.height*0.255)
        case 2: return Vec2(
    			origin.x + visibleSize.width*0.5 - size.width*1.96,
    			origin.y + visibleSize.height*0.255)
        case 3: return Vec2(
			    origin.x + visibleSize.width*0.5 - size.width*0.98,
			    origin.y + visibleSize.height*0.255)
        case 4: return Vec2(
			    origin.x + visibleSize.width*0.5,
			    origin.y + visibleSize.height*0.255)
    }
}

void NetRaceLayer::_DestPositionOfGangCardInHand(int i,Size gangCard) {
    switch(i+1) {
        case 1: return Vec2(
            origin.x+visibleSize.width*0.5 - gangCard.width*1.96,
            origin.y+visibleSize.height*0.26);
        case 2: return Vec2(
            origin.x+visibleSize.width*0.5 - gangCard.width*0.98,
            origin.y+visibleSize.height*0.26);
        case 3: return Vec2(
            origin.x+visibleSize.width*0.5,
            origin.y+visibleSize.height*0.26);
        case 4: return Vec2(
            origin.x+visibleSize.width*0.5 + gangCard.width*0.98,
            origin.y+visibleSize.height*0.255);
    }
}

void NetRaceLayer::_CreateGangCardsMotion(TargetedAction *motions[4]) {
    for (int i=0; i<4; i++ ) {
        auto GangCard = Sprite::createWithTexture(g_my_angang->getTexture());
        Size GangCardSize = GangCard->getTextureRect().size;
        GangCard->setAnchorPoint(Vec2(0,0));       
        GangCard->setScale(0);
        
        GangCard->setPosition(_OrigPositionOfGangCard(i,GangCardSize));
        myframe->addChild(GangCard,30,SINGLE_ACTION_EFFECT_TAG_ID+i);

        motions[i] = TargetedAction::create(GangCard,Sequence::create(
            DelayTime::create(0.42),
            ScaleTo::create(0,1),
            MoveTo::create(0.12,_MiddlePositionOfGangCard(i,GangCardSize)),
            MoveTo::create(0.12,_DestPositionOfGangCard(i,GangCardSize)),NULL));
    }
}


void NetRaceLayer::_MiddlePositionOfGangCardInHand(int i,Vec2 origPos,Size freeCard) {
    switch(i+1) {
        case 1: return Vec2(origPos.x,origPos.y);
        case 2: return Vec2(origPos.x - freeCard.width*0.4, origPos.y);
        case 3: return Vec2(origPos.x - freeCard.width*0.8, origPos.y);
        case 4: return Vec2(origPos.x,origPos.y);
    }
}

void NetRaceLayer::_CreateGangCardInHandMotion(TargetedAction *motions[4],int cardInHand[4],CARD_KIND kind) {
    auto myframe = this->getChildByTag(GAME_BKG_TAG_ID);
    Size FreeCardSize = Sprite::createWithTexture(g_my_free->getTexture())->getTextureRect().size;
    auto GangCardSize = Sprite::createWithTexture(g_my_angang->getTexture())->getTextureRect().size;

    for (int i=0;i<4;i++ ) {
        auto OldCard = (Sprite*)myframe->getChildByTag(HAND_IN_CARDS_TAG_ID + cardInHand[i] + 1*20);//gang1
        auto OldPos  = OldCard->getPosition();
        auto OldSize = OldCard->getScale();
        
        auto GangCard=Sprite::createWithTexture(g_my_free->getTexture());//gang1
        GangCard->setScale(OldSize);
        GangCard->setAnchorPoint(Vec2(0,0));
        GangCard->setPosition(Vec2(OldPos.x,OldPos.y));
        
        auto GangKind=Sprite::createWithTexture(g_card_kind[kind]->getTexture());
        GangKind->setAnchorPoint(Vec2(0.5,0.5));
        /*!!! Gang1Kind.Positon use OldGang2Size, maybe mal-spell ??? */
        GangKind->setPosition(Vec2(OldSize.width/2,OldSize.height*0.4));
        GangCard->addChild(GangKind,1);
        myframe->addChild(GangCard,20,EFFET_NEWCATD1_TAG+i);

        motions[i] = TargetedAction::create(GangCard,Sequence::create(
            DelayTime::create(0.18),Spawn::create(
                ScaleTo::create(0,0.6),
                MoveTo::create(0,_MiddlePositionOfGangCardInHand(i,OldPos,FreeCardSize)),NULL),
            MoveTo::create(0.18,_DestPositionOfGangCardInHand(i,GangCardSize)),
            DelayTime::create(0.06),
            ScaleTo::create(0,0),NULL));
    }
}

/*local variable issue???*/
Vec2 NetRaceLayer::getEffectVec(int dir) {
    switch(dir) {
        case 0:
            return Vec2(origin.x+visibleSize.width*0.206,origin.y+visibleSize.height*0.6);
        case 1:
            return Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2);
        case 2:
            return Vec2(origin.x+visibleSize.width*0.79,origin.y+visibleSize.height*0.6);
        default:
            LOGGER_WRITE("%s Error ! invalid player");
    }
}

int  NetRaceLayer::_RotateAngleOfOutcard(int dir) {
    switch(dir) {
        case 0:
            return 90;
        case 1:
            return 0;
        case 2:
            return 270;
        default:
            LOGGER_WRITE("%s Error ! invalid player");
    }
}

Vec2 NetRaceLayer::_AnchorOfOutcard(int dir) {
    switch(dir) {
        case 0:
            return Vec2(0,0.5);
        case 2:
            return Vec2(1,0.5);
        default:
            LOGGER_WRITE("%s Error ! invalid player");
    }
}

Vec2 NetRaceLayer::_PositionOfOutcard(int dir,Size size,Vec2 origin) {
    switch(dir) {
        case 0:
            return Vec2(origin.x + size.width*0.18, origin.y + size.height*0.6);
        case 2:
            return Vec2(origin.x + size.width*0.82, origin.y + size.height*0.6);
        default:
            LOGGER_WRITE("%s Error ! invalid player");
    }
}














/****************************************************
    trash
****************************************************/
#ifdef TRASH
    void NetRaceLayer::_GuiJinBiShowMe(int gold) {
        Size size   = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
        char str[15];
        sprintf(str,"%d",abs(gold));
        auto number=LabelAtlas::create(std::string(str),"fonts/moneyMessage.png",28,39,'0');
        number->setAnchorPoint(_AnchorOfNumber(1));
        number->setOpacity(0);
        number->setPosition(_PositionOfNumber(1,size,origin));
        this->addChild(number,6,GOLD_NUM_INSERT_NUMBER);
    
        auto minus = Sprite::create("-.png");
        minus->setAnchorPoint(_AnchorOfSign(1));
        minus->setPosition(_PositionOfSign(1,size,origin));
        minus->setOpacity(0);
        this->addChild(minus,6,GOLD_NUM_INSERT_JIANHAO);
    
        auto plus=Sprite::create("+.png");
        plus->setAnchorPoint(_AnchorOfSign(1));
        plus->setPosition(_PositionOfSign(1,size,origin));
        plus->setOpacity(0);
        this->addChild(plus,6,GOLD_NUM_INSERT_JIAHAO);
        
        auto jinbi=Sprite::create("jinbi-game.png");
        jinbi->setAnchorPoint(_AnchorOfGold(1));
        jinbi->setOpacity(0);
        jinbi->setPosition(_PositionOfGold(1,size,origin));
        this->addChild(jinbi,6,GOLD_NUM_INSERT_JINBI);
    
        if( gold!=0 ) {
            jinbi->runAction(Sequence::create(
                DelayTime::create(0),
                Spawn::create(FadeIn::create(1.5),
                MoveTo::create(1.5,_DestOfGold(1,size,origin)),NULL),
                ScaleTo::create(0,0),NULL));
            number->runAction(Sequence::create(
                DelayTime::create(0),
                Spawn::create(FadeIn::create(1.5),
                MoveTo::create(1.5,_DestOfNumber(1,size,origin)),NULL),
                ScaleTo::create(0,0),NULL));
            if(gold>0) {
                plus->runAction(Sequence::create(
                    DelayTime::create(0),
                    Spawn::create(FadeIn::create(1.5),
                    MoveTo::create(1.5,_DestOfSign(1,size,origin)),NULL),
                    ScaleTo::create(0,0),NULL));
            } else {
                minus->runAction(Sequence::create(
                    DelayTime::create(0),
                    Spawn::create(FadeIn::create(1.5),
                    MoveTo::create(1.5,_DestOfSign(1,size,origin)),NULL),
                    ScaleTo::create(0,0),NULL));
            }   
        }
    }
    
    void NetRaceLayer::_GuiJinBiShowLeft(int gold) {
        Size size   = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
        char str[15];
        sprintf(str,"%d",abs(gold));
        auto number=LabelAtlas::create(std::string(str),"fonts/moneyMessage.png",28,39,'0');
        number->setAnchorPoint(_AnchorOfNumber(0));
        number->setOpacity(0);
        number->setPosition(_PositionOfNumber(0,size,origin));
        this->addChild(number,6,GOLD_NUM_INSERT_NUMBER);
    
        auto minus=Sprite::create("-.png");
        minus->setAnchorPoint(_AnchorOfSign(0));
        minus->setPosition(_PositionOfSign(0,size,origin));
        minus->setOpacity(0);
        this->addChild(minus,6,GOLD_NUM_INSERT_JIANHAO);
    
        auto plus=Sprite::create("+.png");
        plus->setAnchorPoint(_AnchorOfSign(0));
        plus->setPosition(_PositionOfSign(0,size,origin));
        plus->setOpacity(0);
        this->addChild(plus,6,GOLD_NUM_INSERT_JIAHAO);
    
        auto jinbi=Sprite::create("jinbi-game.png");
        jinbi->setAnchorPoint(_AnchorOfGold(0));
        jinbi->setOpacity(0);
        jinbi->setPosition(_PositionOfGold(0,size,origin));
        this->addChild(jinbi,6,GOLD_NUM_INSERT_JINBI);
    
        if( gold!=0 ) {
            jinbi->runAction(Sequence::create(
                DelayTime::create(0),
                Spawn::create(FadeIn::create(1.5),
                MoveTo::create(1.5,_DestOfGold(0,size,origin)),NULL),
                ScaleTo::create(0,0),NULL));
            number->runAction(Sequence::create(
                DelayTime::create(0),
                Spawn::create(FadeIn::create(1.5),
                MoveTo::create(1.5,_DestOfNumber(0,size,origin)),NULL),
                ScaleTo::create(0,0),NULL));
            if(gold>0) {
                plus->runAction(Sequence::create(
                    DelayTime::create(0),
                    Spawn::create(FadeIn::create(1.5),
                    MoveTo::create(1.5,_DestOfSign(0,size,origin)),NULL),
                    ScaleTo::create(0,0),NULL));
            } else {
                minus->runAction(Sequence::create(
                    DelayTime::create(0),
                    Spawn::create(FadeIn::create(1.5),
                    MoveTo::create(1.5,_DestOfSign(0,size,origin)),NULL),
                    ScaleTo::create(0,0),NULL));
            }
        }
    }
    
    void NetRaceLayer::_GuiJinBiShowRight(int gold) {
        Size size = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
        char str[15];
        sprintf(str,"%d",abs(gold));
        auto number=LabelAtlas::create(std::string(str),"fonts/moneyMessage.png",28,39,'0');
        number->setAnchorPoint(_AnchorOfNumber(2));
        number->setOpacity(0);
        number->setPosition(_PositionOfNumber(2,size,origin));
        this->addChild(number,6,GOLD_NUM_INSERT_NUMBER);
    
        int xoffset = number->getContentSize().width;
        
        auto minus=Sprite::create("-.png");
        minus->setAnchorPoint(_AnchorOfSign(2));
        minus->setPosition(_PositionOfSign(2,size,origin,xoffset));
        minus->setOpacity(0);
        this->addChild(minus,6,GOLD_NUM_INSERT_JIANHAO);
    
        auto plus=Sprite::create("+.png");
        plus->setAnchorPoint(_AnchorOfSign(2));
        plus->setPosition(_PositionOfSign(2,size,origin,xoffset));
        plus->setOpacity(0);
        this->addChild(plus,6,GOLD_NUM_INSERT_JIAHAO);
    
        auto jinbi=Sprite::create("jinbi-game.png");
        jinbi->setAnchorPoint(_AnchorOfGold(2));
        jinbi->setOpacity(0);
        jinbi->setPosition(_PositionOfGold(2,size,origin,xoffset));
        this->addChild(jinbi,6,GOLD_NUM_INSERT_JINBI);
    
        if( gold!=0 ) {
            jinbi->runAction(Sequence::create(
                DelayTime::create(0),
                Spawn::create(FadeIn::create(1.5),
                MoveTo::create(1.5,_DestOfGold(2,size,origin,xoffset)),NULL),
                ScaleTo::create(0,0),NULL));
            number->runAction(Sequence::create(
                DelayTime::create(0),
                Spawn::create(FadeIn::create(1.5),
                MoveTo::create(1.5,_DestOfNumber(2,size,origin)),NULL),
                ScaleTo::create(0,0),NULL));
            if(gold>0) {
                plus->runAction(Sequence::create(
                    DelayTime::create(0),
                    Spawn::create(FadeIn::create(1.5),
                    MoveTo::create(1.5,_DestOfSign(2,size,origin,xoffset)),NULL),
                    ScaleTo::create(0,0),NULL));
            } else {
                minus->runAction(Sequence::create(
                    DelayTime::create(0),
                    Spawn::create(FadeIn::create(1.5),
                    MoveTo::create(1.5,_DestOfSign(2,size,origin,xoffset)),NULL),
                    ScaleTo::create(0,0),NULL));
            }
        }
    }
#endif
