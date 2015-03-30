
#include "NetRaceLayer.h"
#include "HelloWorldScene.h"
#include "RoundManager.h"

#include "NetPlayer.h"
#include "./../Me.h"

using namespace cocos2d::ui;
USING_NS_CC;

/**************************/
/* this is for test network communication*/
#include "./../utils/UtilBasic.h"
#include "./../protocol/MsgFormats.h"
#include "./../protocol/KwxMsg.h"
#include "./../protocol/EnvVariables.h"
/**************************/

/************************************************
                init 
************************************************/
NetRaceLayer::NetRaceLayer()
:PREMIUM_LEAST(200)
{
	s_scale=1.189;
	s_no=1;

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

    _object = GObjectFactory::getInstance(origin,visibleSize);
    _voice = VoiceEffect::getInstance();
    _layout = GameLayout::getInstance(origin,visibleSize);
    _effect = GraphicEffect::getInstance();
    
    _logger = LOGGER_REGISTER("NetRaceLayer");

	/**************************/
	/* this is just for test */
    INT8U buf[MSG_MAX_LEN] = {0};
    int   len = 0;
    
    SeatInfo *seat = SeatInfo::getInstance();
    seat->Set(0x00010203,0x04050607,0x08090a0b,1);
    
    KwxMsg aMsg(UP_STREAM);
    aMsg.StartReceiving();
    len = aMsg.SetAction(buf,(ActionId_t)PENG);
	/**************************/
}

void NetRaceLayer::Set(RoundManager *rm) {
    _roundManager = rm;
    _ai = Ai::getInstance(_roundManager);
}

NetRaceLayer::~NetRaceLayer()
{
	_eventDispatcher->removeAllEventListeners();

    delete _roundManager;
    _voice->destroyInstance();
    _layout->destroyInstance();
    _object->destroyInstance();
    _effect->destroyInstance();
    
    LOGGER_DEREGISTER(_logger);
}

bool NetRaceLayer::init() {
	if(!Layer::init()) {
		return false;
	}
	srand(time(0));
	
	return true;
}

/************************************************
            main process
************************************************/
void NetRaceLayer::CreateRace()
{
    LOGGER_WRITE("%s",__FUNCTION__);

	_isResoucePrepared=false;

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("gameprepareImage.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("tools.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("userhead.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("xzdd_prepare_pop_res.plist");

	this->addChild(_object->CreateTable(s_no), 0);
	this->addChild(_object->CreateBackground(),1,CENTER_BKG_TAG_ID);

    _CreateMeneButtons();
    _CreateResidueCards();
	_UpdateResidueCards(TOTAL_CARD_NUM - _roundManager->_distributedNum);
    _CreateHeadImage();

	this->addChild(_object->CreateMicIcon(),1,MIC_TAG_ID);
	this->addChild(_object->CreateModeFont(LOCAL_GAME),1,SINGLE_PLAY_TAG_ID);
    
	for(int dir=0;dir<3;dir++) {
		_UpdateHeadImage(dir,_roundManager->_cardHolders[dir]->_profile.photo);
		_UpdateNickName(dir,_roundManager->_cardHolders[dir]->_profile.name);
		GuiUpdateScore(dir,_roundManager->_cardHolders[dir]->_profile.property);
	}

    for(int i=0;i<4;i++) {
        auto mapai = _object->CreateMaPai(i);
        this->addChild(mapai,2,CARD_ARRAGE_LEFT_TAG_ID + i);
    }

    auto StartButton = _object->CreateButton(BTN_START);
    StartButton->addTouchEventListener(CC_CALLBACK_2(NetRaceLayer::BtnStartHandler,this));
    this->addChild(StartButton,2,START_GAME_TAG_ID);

    /* is this neccessary??? */
    _eventDispatcher->addEventListenerWithFixedPriority(EventListenerCustom::create(
        WAIT_START_CALLBACK_EVENT_TYPE, [this](EventCustom * event){
        _roundManager->Shuffle();}), 
        3);
}

void NetRaceLayer::StartGame()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("TimerImage.plist");

	if(!_isResoucePrepared) {
		_isResoucePrepared=true;
		_ResourcePrepare();
	}

    _Remove(this,GAME_BKG_TAG_ID);
    _Remove(this,START_GAME_TAG_ID);
    _Remove(this,START_GAME_TAG_ID+1);
    _Remove(this,RACE_RESTART_TAG_ID);
    _Remove(this,RACE_RESTART_TAG_ID+1);
    _Remove(this,SHINE_TAG_ID);
    _Remove(this,SHINE_TAG_ID+1);
    _Remove(this,RACE_ACCOUT_TAG_ID);
    _Remove(this,HUANG_ZHUANG_LAYER);
    _Remove(this,HUANG_ZHUANG_FONT);

    _Show(this,MING_STATUS_PNG_0,false);
    _Show(this,MING_STATUS_PNG_1,false);
    _Show(this,MING_STATUS_PNG_2,false);
    
	for(int i=0;i<4;i++) {
		while(this->getChildByTag(GOLD_NUM_INSERT_JINBI+i))
			this->removeChildByTag(GOLD_NUM_INSERT_JINBI+i,true);
	}
    
    _DistributeEvent(WAIT_START_CALLBACK_EVENT_TYPE,NULL);

	myframe = LayerColor::create(Color4B(0,0,0,0),visibleSize.width,visibleSize.height);
	myframe->setPosition(Vec2(origin.x, origin.y));
	this->addChild(myframe,3,GAME_BKG_TAG_ID);

	distributeCardPos=Vec2::ZERO;
	ifUpdateDuringEffect=false;
	_isCardInHandUpdated=false;
	ifInsertCardsTime=false;
	ifEffectTime=false;
    _myChosenCard = -1;
    
	_RaceBeginPrepare();                  //牌局开始效果

    _roundManager->StartGame();
}

void NetRaceLayer::WaitForFirstAction(PlayerDir_t zhuang)
{
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("BlockOtherImage.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("gametrayImage.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("MingTips.plist");
    /*!!! maybe lagging ???*/
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("race3.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("race4.plist");

    ListenToCardTouch();
    ((Button*)this->getChildByTag(MENU_BKG_TAG_ID)->getChildByTag(TUOGUAN_MENU_BUTTON))->setTouchEnabled(true);

    _roundManager->WaitForFirstAction(zhuang);
}

void NetRaceLayer::OthersHandoutEffect(PlayerDir_t dir,bool canKou) {
    myframe->_ID = dir;
    
	myframe->runAction(Sequence::create(
        _OthersShowCardEffect(dir,(Card_t)_roundManager->_lastHandedOutCard,canKou),CCCallFunc::create([=]() {
		_CardRiverUpdateEffect(dir);}), CCCallFunc::create([=]() {
        _roundManager->UpdateCards(dir,a_JUMP);}),CCCallFunc::create([=]() {
        _CardInHandUpdateEffect(dir);}), CCCallFunc::create([=]() {
		_roundManager->WaitForResponse(dir);}),NULL));
}

/***********************************************
        effect entrances
***********************************************/
void NetRaceLayer::QiEffect() {
    _effect->Hide(myframe,QI_REMIND_ACT_TAG_ID);

    _QiEffect(MIDDLE);
}

void NetRaceLayer::PengEffect(PlayerDir_t dir, PlayerDir_t prevDir, Card_t card) {
    _effect->Hide(myframe,PENG_REMIND_ACT_TAG_ID);
    
    _PengEffect(dir,prevDir,card);
}

void NetRaceLayer::HuEffect(const WinInfo_t &win,bool qiangGang) {
    _effect->Hide(myframe,HU_REMIND_ACT_TAG_ID);
    
    if(qiangGang) {
        _DeleteActionTip();
        _HuEffect(win);
    } else if(win.kind==DOUBLE_WIN) {
        _effect->Shade(myframe->getChildByTag(HU_REMIND_ACT_TAG_ID));
        _DeleteActionTip();
        _HuEffect(win);   /*why use event rather than call*/
        _DistributeEvent(DOUBLE_HU_WITH_ME,NULL);
    } else {
        _HuEffect(win);
    }
}

void NetRaceLayer::GangEffect(PlayerDir_t winner,Card_t card, int gangCardIdx[], bool isAnGang, PlayerDir_t prevPlayer) {
    _effect->Hide(myframe,GANG_REMING_ACT_TAG_ID);
    
    if(isAnGang) {
        _AnGangEffect(winner,card,gangCardIdx);
    } else {
        _MingGangEffect(winner,prevPlayer,(Card_t)card,gangCardIdx);
    }
}

void NetRaceLayer::DoubleWin(const WinInfo_t &win) {
    HideClock();

    if(win.winner!=MIDDLE) {
        if(_roundManager->IsTing(MIDDLE)) {
            _HuEffect(win);
            _DistributeEvent(DOUBLE_HU_WITH_ME,NULL);
        } else {
            _roundManager->WaitForMyAction();
        }
    }
}

void NetRaceLayer::SingleWin(const WinInfo_t &win) {
    HideClock();

    if(win.winner==MIDDLE) {
        if(_roundManager->IsTing(MIDDLE)) {
            _HuEffect(win);
        } else {
            _roundManager->WaitForMyAction();
        }

    } else {
        _HuEffect(win);
    }
}

void NetRaceLayer::GangGoldEffect(int winner,int whoGive) {
    myframe->runAction(Sequence::create(CallFunc::create([=](){
        GoldNumInsert(winner,2,whoGive);}),CallFunc::create([=](){
        _roundManager->DistributeTo((PlayerDir_t)winner);}),NULL));
}

void NetRaceLayer::MyHandoutEffect(int chosenCard,CARD_ARRAY *list,Vec2 touch,int time,bool turnToMing)
{
    if(time==2) {
        if(myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+MIDDLE*20+chosenCard)) {
            myframe->removeChildByTag(HAND_IN_CARDS_TAG_ID+MIDDLE*20+chosenCard);
        }
    } else {
        _ReOrderCardsInHand(chosenCard,list);
    }
    
	_MyHandoutEffect((Card_t)_roundManager->_lastHandedOutCard,touch,time,turnToMing);
}

void NetRaceLayer::_CardInHandUpdateEffect(PlayerDir_t dir)
{
	if(_roundManager->IsTing(dir)) {
        _Show(this,MING_STATUS_PNG_0+dir,true);
	}
    
	for(int i=0;i<MAX_HANDIN_NUM;i++) {
        _Remove(myframe,HAND_IN_CARDS_TAG_ID+dir*20+i);
	}

	float x = _layout->_playerPosi[dir].basePoint.x+10; 
	float y = _layout->_playerPosi[dir].basePoint.y+10;

	CARD_ARRAY *list=_roundManager->_players[dir]->get_parter()->get_card_list();

	if( _roundManager->IsTing(dir) && _roundManager->_firstMingNo==INVALID )
		_roundManager->_firstMingNo=dir;

	unsigned char ting_flag = _roundManager->_players[dir]->get_parter()->get_ting_status();

	Sprite *p_list[MAX_HANDIN_NUM];
    
    for(int i=0;i<list->len;i++)
	{
		if(list->data[i].kind!=ck_NOT_DEFINED )
		{
			if(dir==LEFT || dir==RIGHT) {
				p_list[i] = _CreateCardInHand(dir,i,list,(ting_flag==1),Vec2(x,y));
                
				CartApperance_t apperance = _roundManager->GetCardApperance(dir,i);
                if(apperance!=NORMAL_APPERANCE) {
                    _object->LayDownWithFace((PlayerDir_t)dir, p_list[i], (Card_t)list->data[i].kind,apperance);
                }

                y += _YofNextCard(dir,i,list,(ting_flag==1),p_list[i]->getTextureRect().size.height);
			}
			else if(dir==MIDDLE)
			{
				if(list->data[i].status==c_FREE||list->data[i].status==c_MING_KOU)
				{
					if(ting_flag==1)
						p_list[i]=_object->Create(MING_CARD,(PlayerDir_t)dir,x,y);
					else
						p_list[i]=_object->Create(FREE_CARD,(PlayerDir_t)dir,x,y);
				}
				else if(list->data[i].status==c_PENG||list->data[i].status==c_MING_GANG)
				{
					p_list[i]=_object->Create(PENG_CARD,(PlayerDir_t)dir,x,y);
				}
				else if(list->data[i].status==c_AN_GANG)
				{
					if(list->data[i].kind==list->data[i+1].kind && list->data[i].kind!=list->data[i+2].kind)
						p_list[i]=_object->Create(PENG_CARD,(PlayerDir_t)dir,x,y);
					else
						p_list[i]=_object->Create(AN_GANG_CARD,(PlayerDir_t)dir,x,y);
				}
                
                /* the card being chosen */
				if(_myChosenCard==i) {
					p_list[i]->setScale(1.2);
					p_list[i]->setPosition(Vec2(x,y+10));
					x+=p_list[i]->getTextureRect().size.width*0.2;
				}

                /* the second last card before handout*/
                int residualCardsNum = (list->len-list->atcvie_place)%3;
				if(residualCardsNum==2 && i==list->len-2)
					x+=30;

                /* the card showing somewhere else */
                if( _roundManager->IsCurEffectCard(list->data[i]) )
					p_list[i]->setVisible(false);


                if(list->data[i].status==c_FREE)
				{
					if(ting_flag==1)
					{
					    _object->LayDownWithFace(p_list[i], (Card_t)list->data[i].kind, 0.6);
					}
					else
					{
					    _object->LayDownWithFace(p_list[i], (Card_t)list->data[i].kind, 0.4);
					}
                    
					if(ting_flag==1||(_roundManager->_actionToDo==a_MING&&ting_flag==0))
					{
						if(list->data[i].can_play==cps_YES)
						{
							auto MingCps_yesMask=_object->Create(MING_BKG);
							MingCps_yesMask->setAnchorPoint(Vec2(0.5,0.5));
							MingCps_yesMask->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,
                                p_list[i]->getTextureRect().size.height/2));
							p_list[i]->addChild(MingCps_yesMask,2,MING_KOU_MASK);
						}
						else if(list->data[i].can_play==cps_NO)
						{
							Sprite* mask;
							mask=_object->Create(MING_MASK_CARD);
							mask->setAnchorPoint(Vec2(0.5,1));
							mask->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,
                                p_list[i]->getTextureRect().size.height));
							p_list[i]->addChild(mask,2,MING_KOU_MASK);
						}
					}
                    
					x+=p_list[i]->getTextureRect().size.width*1.0;
				}
				else if(list->data[i].status==c_MING_KOU)
				{
					if(ting_flag!=1)
					{
						auto KouSign=_object->Create(MING_KOU_CARD);
						KouSign->setAnchorPoint(Vec2(0.5,0.5));
						KouSign->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,
                            p_list[i]->getTextureRect().size.height/2));
						p_list[i]->addChild(KouSign,2);

					    _object->LayDownWithFace(p_list[i], (Card_t)list->data[i].kind, 0.4);
                        
						x += p_list[i]->getTextureRect().size.width*1;
					}
					else
					{
					    _object->LayDownWithFace(p_list[i], (Card_t)list->data[i].kind, 0.6);

                        x += p_list[i]->getTextureRect().size.width*1;
					}
				}
				else if(list->data[i].status==c_PENG)
				{
					auto s_card=_object->CreateKind((Card_t)list->data[i].kind,MIDDLE_SIZE);
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
					auto s_card=_object->CreateKind((Card_t)list->data[i].kind,MIDDLE_SIZE);
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
						auto s_card=_object->CreateKind((Card_t)list->data[i].kind,MIDDLE_SIZE);
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



			if(dir!=RIGHT) {//each one lay above the previous ones
                if(!_GetCardInHand((PlayerDir_t)dir,i)) {
                    myframe->addChild(p_list[i],i+1,        HAND_IN_CARDS_TAG_ID+dir*20+i);
                }
            }
			else {//each one lay below the previous ones
                if(!_GetCardInHand((PlayerDir_t)dir,i)) {
    				myframe->addChild(p_list[i],list->len-i,HAND_IN_CARDS_TAG_ID+dir*20+i);
                }
            }
            
			if(dir==MIDDLE
                &&_roundManager->_actionToDo==a_MING
                &&_roundManager->_players[MIDDLE]->get_parter()->get_ting_status()!=1
                &&list->data[i].can_play==cps_YES) {
				p_list[i]->setZOrder(30);
            }
            
			p_list[i]->_ID=MIDDLE;

            if(dir==MIDDLE && _myChosenCard==i)
				p_list[i]->_ID=100;
		}//for each card
	}
    
	if(dir==MIDDLE&&ting_flag!=0)
	{
		if(!myframe->getChildByTag(TING_SING_BUTTON))
		{
			Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(myframe,true);
            
			auto Ting_Sign = _object->CreateTingSign();
			myframe->addChild(Ting_Sign,1,TING_SING_BUTTON);

            while(myframe->getChildByTag(TING_SING_BAR))
				myframe->removeChildByTag(TING_SING_BAR);

            _TingHintCreate(Ting_Sign->getPosition(),Hu_cardOut_place);
			myframe->getChildByTag(TING_SING_BAR)->setVisible(false);
			ifTingSignBarVisible=false;
			ListenToTingButton();

            _CardInHandUpdateEffect(LEFT);
			_CardInHandUpdateEffect(RIGHT);
		}
	}
    
	if(dir==MIDDLE&&myframe->getChildByTag(TING_SING_BUTTON))
		_UpdateTingNum(MIDDLE);
}

void NetRaceLayer::_CardRiverUpdateEffect(PlayerDir_t dir) {
	outCardList* outCard = _roundManager->_players[dir]->get_parter()->getOutCardList();

	for(int i=0;i<outCard->length;i++) {
	    _Remove(myframe,HAND_OUT_CARDS_TAG_ID+dir*25+i);
	}
    
	auto show_card_indicator=this->getChildByTag(SHOWCARD_INDICATOR_TAG_ID);
	show_card_indicator->setVisible(true);

	for(int i=0;i<outCard->length;i++) {
        Card oCard;
        outCard->getCard(oCard,i+1);
        
        auto show_card = _object->CreateRiverCard((PlayerDir_t)dir,(Card_t)oCard.kind);
        show_card_indicator->stopAllActions();
        
        show_card_indicator->setPosition(_layout->OrigPositionOfRiverCard((PlayerDir_t)dir,i));                      
        show_card_indicator->runAction(RepeatForever::create(Sequence::create(
            MoveTo::create(0.5,_layout->Middle1PositionOfRiverCard((PlayerDir_t)dir,i)),
            MoveTo::create(0.5,_layout->Middle2PositionOfRiverCard((PlayerDir_t)dir,i)),NULL)));
        show_card->setPosition(_layout->DestPositionOfRiverCard((PlayerDir_t)dir,i));

        myframe->addChild(show_card, 
            _layout->ZorderOfRiverCard((PlayerDir_t)dir,i),
            HAND_OUT_CARDS_TAG_ID+dir*25+i);
	}
}

/***************************************************
        distribute
***************************************************/
Vec2 NetRaceLayer::_GetLastCardPosition(PlayerDir_t dir,int cardLen) {
    float x = _layout->_playerPosi[dir].basePoint.x;
    float y = _layout->_playerPosi[dir].basePoint.y;

    switch(dir) {
        case MIDDLE:
            if( _roundManager->_lastActionSource==MIDDLE
                && (_roundManager->_lastAction==a_AN_GANG||_roundManager->_lastAction==a_SHOU_GANG||_roundManager->_lastAction==a_MING_GANG) ) {
                x = distributeCardPos.x;
            } else {
                x += _GetCardInHand(MIDDLE,cardLen)->getPosition().x+30;
            }
            
            y += 60 + 13*(_roundManager->IsTing(dir));
            break;
        case LEFT:
            y = _GetCardInHand(LEFT,cardLen-1)->getPosition().y-20;//+5;
            break;
        case RIGHT:
            y = _GetCardInHand(RIGHT,cardLen)->getPosition().y+86;
            break;
    }
    
    return Vec2(x,y);
}

void NetRaceLayer::_DistributeCard(PlayerDir_t dir,int lenOfInHand) {
    const Vec2 &lastCardPosition = _GetLastCardPosition(dir,lenOfInHand);

    Sprite *lastCard = _object->CreateDistributeCard(dir,(Card_t)_roundManager->_lastHandedOutCard);
	lastCard->setPosition(lastCardPosition);
    
	_UpdateResidueCards(TOTAL_CARD_NUM - _roundManager->_distributedNum);

    switch(dir) {
        case LEFT:
        case MIDDLE:
            myframe->addChild(lastCard,30, HAND_IN_CARDS_TAG_ID+dir*20+(lenOfInHand+1));
            break;
        case RIGHT:
            myframe->addChild(lastCard,0,  HAND_IN_CARDS_TAG_ID+dir*20+(lenOfInHand+1));
            break;
    }

	auto last_one_action=TargetedAction::create(lastCard,Sequence::create(
        EaseElasticOut::create(
            MoveTo::create(0.3,Vec2(
                lastCard->getPositionX(),
                lastCard->getPositionY()-50))),NULL));

    DelayTime *delay = (dir!=MIDDLE)
        ? (DelayTime::create(0.5))
        : (DelayTime::create(0.0));
    
	myframe->_ID = dir;
	myframe->runAction(Sequence::create(
        last_one_action,
        delay,CCCallFunc::create([=]() {
		_roundManager->WaitForResponse(dir);}),NULL));
}

void NetRaceLayer::ListenToDistributeCard() {
    LOGGER_WRITE("%s",__FUNCTION__);

	auto _distributedoneListener = EventListenerCustom::create(DISTRIBUTE_DONE_EVENT_TYPE, [this](EventCustom * event){
		auto userData = static_cast<DistributeInfo_t *>(event->getUserData());
        
		_roundManager->_lastHandedOutCard = (CARD_KIND)userData->card;
        _roundManager->_isCardFromOthers  = false;

        auto target = userData->target;
        
        auto cards = _roundManager->_players[target]->get_parter()->get_card_list();
		_DistributeCard(target,cards->len-1);
	});
	_eventDispatcher->addEventListenerWithFixedPriority(_distributedoneListener,2);

	auto _noonewinListener = EventListenerCustom::create(NOONE_WIN_EVENT_TYPE, [this](EventCustom * event){

		((Button*)this->getChildByTag(MENU_BKG_TAG_ID)->getChildByTag(TUOGUAN_MENU_BUTTON))->setTouchEnabled(false);

        _Remove(this,ROBOT_TUO_GUAN);
        _Remove(this,TUOGUAN_CANCEL_BUTTON);
		
        _roundManager->SetWin(NONE_WIN,_roundManager->_curPlayer);

		scheduleOnce(schedule_selector(NetRaceLayer::raceAccount),3);

		this->runAction(
            Sequence::create(
                DelayTime::create(0.5),CallFunc::create([=](){
    			if(_roundManager->_firstMingNo!=-1)
    				GoldNumInsert(4,3,_roundManager->_firstMingNo);}),
    			DelayTime::create(1),
    			Spawn::create(CallFunc::create([=](){
        			this->addChild(_object->CreateHuangZhuangBkg(),10,HUANG_ZHUANG_LAYER);
        			auto HuangZhuang=_object->CreateHuangZhuang();;
        			this->addChild(HuangZhuang,10,HUANG_ZHUANG_FONT);
        			auto FontAction=TargetedAction::create(HuangZhuang,FadeIn::create(0.5));}),CallFunc::create(this,callfunc_selector(
    			    NetRaceLayer::showall)),NULL),NULL));
	});
	_eventDispatcher->addEventListenerWithFixedPriority(_noonewinListener,3);
}

void NetRaceLayer::_LeftBatchDistribute(int batchIdx, float delayRef, int cardLen) {
	auto cardHeight = _object->RectSize(L_IN_CARD).height;

    float x = _layout->_playerPosi[LEFT].basePoint.x+10;
    float y = _layout->_playerPosi[LEFT].basePoint.y+10;

    y -= (cardHeight*0.5)*(batchIdx);
    for(int i=0;i<DIST_BATCH_CARDS;i++) {
        int cardIdx = i+4*batchIdx;
        if(cardIdx==cardLen) {
            break;
        }
        
        Sprite *sprite = _object->Create(L_IN_CARD);
        sprite->setAnchorPoint(Vec2(0.0f,1.0f));
        sprite->setPosition(Vec2(x-10,y-8));
    
        y -= (cardHeight*0.5);
    
        myframe->addChild(sprite,cardIdx+1,HAND_IN_CARDS_TAG_ID + cardIdx);
        sprite->setScale(0);
        sprite->runAction(Sequence::create(
            DelayTime::create(delayRef + 0.2 + 0.6*batchIdx),
            ScaleTo::create(0,1),NULL));
    }
}

void NetRaceLayer::_RightBatchDistribute(int batchIdx, float delayRef, int cardLen) {
    auto cardHeight = _object->RectSize(R_IN_CARD).height;

    float x = _layout->_playerPosi[RIGHT].basePoint.x+10;
    float y = _layout->_playerPosi[RIGHT].basePoint.y+10;

    y += (cardHeight*0.5)*4*batchIdx;
    for(int i=0;i<DIST_BATCH_CARDS;i++) {
        int cardIdx = i+4*batchIdx;
        if(cardIdx==cardLen) {
            break;
        }
        
        Sprite *sprite = _object->Create(R_IN_CARD);
        sprite->setAnchorPoint(Vec2(0.0f,0.0f));
        sprite->setPosition(Vec2(x-10,y-8));
    
        y += (cardHeight*0.5);

        /* 15: assuming maximum length */
        myframe->addChild(sprite,15-(cardIdx+1),HAND_IN_CARDS_TAG_ID + 2*20 + cardIdx);
        sprite->setScale(0);
        sprite->runAction(Sequence::create(
            DelayTime::create(delayRef + 0.2 + 0.6*batchIdx),
            ScaleTo::create(0,1),NULL));
    }
}

void NetRaceLayer::FirstRoundDistributeEffect(PlayerDir_t zhuang) {
    LOGGER_WRITE("%s",__FUNCTION__);
	auto VoiceEffect=_voice->Speak("sort.ogg");	

	float timeXH[PLAYER_NUM];
    timeXH[zhuang] = 0.7;
    timeXH[(zhuang+1)%PLAYER_NUM] = 0.7+0.2;
    timeXH[(zhuang+2)%PLAYER_NUM] = 0.7+0.2+0.2;

	Sprite* lastTwo[2];
	
	auto ListOfNo1=_roundManager->_players[MIDDLE]->get_parter()->get_card_list();
    auto cardWidth = _object->RectSize(FREE_CARD).width;

	int a,b;
	//自己
    /************************************
        one batch from center to hand
    ************************************/
	Sprite* meHandCard[DIST_BATCH_CARDS];
	Vec2 meHandPosition[DIST_BATCH_CARDS];
	auto disCardWidth = _object->RectSize(IN_CARD).width;

	for (a=0;a<DIST_BATCH_CARDS;a++) {
		meHandPosition[a] = _layout->PositionOfNewDistributeCard(a,disCardWidth);

		meHandCard[a]=_object->Create(IN_CARD);
		meHandCard[a]->setAnchorPoint(Vec2(0,0.5));
		meHandCard[a]->setPosition(meHandPosition[a]);
		meHandCard[a]->setScale(0);
		myframe->addChild(meHandCard[a],3,START_CARDS_IN_TAG_ID+a);	
	}

	float changingPosition[DIST_BATCH_CARDS];
	Point HandoutEnd=Vec2(
        _layout->_playerPosi[MIDDLE].basePoint.x+10,
        _layout->_playerPosi[MIDDLE].basePoint.y+110);

	for(a=0;a<DIST_BATCHES;a++) {
		for (b=0;b<DIST_BATCH_CARDS;b++) {
			changingPosition[b]=HandoutEnd.x;
		}
		float y=HandoutEnd.y;

        changingPosition[0]+=cardWidth*a;
		changingPosition[1]+=cardWidth*(a*2)+cardWidth;
		changingPosition[2]+=cardWidth*(a*3)+cardWidth*2;
		changingPosition[3]+=(cardWidth*13+a*cardWidth+30);
        
		auto outTime1=Sequence::create(
            DelayTime::create(timeXH[1]-0.3),
            _effect->MoveDistributeCard(Vec2(changingPosition[0],y),
                Vec2(meHandPosition[a].x,meHandPosition[a].y)),NULL);

		auto outTime2 = _effect->MoveDistributeCard(Vec2(changingPosition[1],y),
            Vec2(meHandPosition[a].x,meHandPosition[a].y));
		auto outTime3 = _effect->MoveDistributeCard(Vec2(changingPosition[2],y),
            Vec2(meHandPosition[a].x,meHandPosition[a].y));

		Sequence* outTime4;

		if( (zhuang==1&&a<2)||(zhuang!=1&&a<1) )
			outTime4 = _effect->MoveDistributeCard(Vec2(changingPosition[3],y),
                Vec2(meHandPosition[a].x,meHandPosition[a].y));
		else
			outTime4=Sequence::create(
    			DelayTime::create(0.4),NULL);

		meHandCard[a]->runAction(Sequence::create(outTime1,outTime2,outTime3,outTime4,NULL));
	}


    /************************************
        distribute to middle player
    ************************************/
    Sprite* p_list[4];
    float x,y;
    const Vec2 &REF = Vec2(_layout->_playerPosi[1].basePoint.x+10,
                            _layout->_playerPosi[1].basePoint.y+10);

	for(a=0;a<DIST_BATCHES;a++)
	{
		if(a==0)//DealayTime::create(0.9)
		{
			for(int i=0;i<DIST_BATCH_CARDS;i++)
			{				
                auto s_card=_object->CreateKind((Card_t)ListOfNo1->data[0+3*i].kind,NORMAL);

				p_list[i]=_object->Create(FREE_CARD,s_card);
				p_list[i]->setAnchorPoint(Point(0.0f,0.0f));
                
				p_list[i]->setPosition(Vec2(REF.x+cardWidth*i, REF.y));
				p_list[i]->setScale(0);

                myframe->addChild(p_list[i],i+1,HAND_IN_CARDS_TAG_ID+1*20+i);

                auto list_seq0=Sequence::create(DelayTime::create(timeXH[1]+0.2),ScaleTo::create(0,1),NULL);
				auto mv1=MoveTo::create(0.2,Vec2(
                    REF.x+cardWidth*i*2, 
                    REF.y));
				auto delay1=DelayTime::create(0.4);
				auto mv2=MoveTo::create(0.2,Vec2(
                    REF.x+cardWidth*i*3, 
                    REF.y));
				auto delay2=DelayTime::create(0.4);
				auto list_seq1=Sequence::create(DelayTime::create(timeXH[1]+0.2),delay1,mv1,delay2,mv2,NULL);
				auto list_spa=Spawn::create(list_seq0,list_seq1,NULL);
				p_list[i]->runAction(list_spa);
			}
		}
		else if(a==1)//DelayTime::create(1.5)
		{
			for(int i=0;i<DIST_BATCH_CARDS;i++)
			{
				auto s_card=_object->CreateKind((Card_t)ListOfNo1->data[1+3*i].kind,NORMAL);
                p_list[i]=_object->Create(FREE_CARD,s_card);
				p_list[i]->setAnchorPoint(Point(0.0f,0.0f));
				p_list[i]->setPosition(Vec2(REF.x+cardWidth*1.0*i*2+cardWidth, REF.y));
				p_list[i]->setScale(0);

				myframe->addChild(p_list[i],i+5,HAND_IN_CARDS_TAG_ID+1*20+4+i);

				auto list_seq0=Sequence::create(DelayTime::create(timeXH[1]+0.8),ScaleTo::create(0,1),NULL);
				auto mv1=MoveTo::create(0.2,Vec2(
                    REF.x+cardWidth*i*3+cardWidth, 
                    REF.y));
				auto delay1=DelayTime::create(0.4);
				auto list_seq1=Sequence::create(DelayTime::create(timeXH[1]+0.8),delay1,mv1,NULL);
				auto list_spa=Spawn::create(list_seq0,list_seq1,NULL);
				p_list[i]->runAction(list_spa);
			}
		}
		else if(a==2)//DelayTime::create(2.1)
		{
			for(int i=0;i<DIST_BATCH_CARDS;i++)
			{
				auto s_card=_object->CreateKind((Card_t)ListOfNo1->data[2+3*i].kind,NORMAL);

				p_list[i]=_object->Create(FREE_CARD,s_card);
				p_list[i]->setAnchorPoint(Point(0.0f,0.0f));
				p_list[i]->setPosition(Vec2(
                    REF.x + cardWidth*(i*3) + cardWidth*1.0*2, 
                    REF.y));
				p_list[i]->setScale(0);

				myframe->addChild(p_list[i],i+9,HAND_IN_CARDS_TAG_ID+1*20+8+i);

				p_list[i]->runAction(Sequence::create(DelayTime::create(timeXH[1]+1.4),ScaleTo::create(0,1),NULL));
			}

		}
		else if(a==3)//DelayTime(2.7)
		{
			auto s_card=_object->CreateKind((Card_t)ListOfNo1->data[12].kind,NORMAL);
			lastTwo[0]=_object->Create(FREE_CARD,s_card);
			lastTwo[0]->setAnchorPoint(Vec2(0.0f,0.0f));
			lastTwo[0]->setPosition(Vec2(
                REF.x + cardWidth*1.0*12, 
                REF.y));
			lastTwo[0]->setScale(0);
			myframe->addChild(lastTwo[0],0+13,HAND_IN_CARDS_TAG_ID+1*20+12);

			lastTwo[0]->runAction(Sequence::create(DelayTime::create(timeXH[1]+2),ScaleTo::create(0,1),NULL));	
            
			if(zhuang==MIDDLE) {
				auto s_card=_object->CreateKind((Card_t)ListOfNo1->data[13].kind,NORMAL);
				lastTwo[1]=_object->Create(FREE_CARD,s_card);
				lastTwo[1]->setAnchorPoint(Vec2(0.0f,0.0f));
				lastTwo[1]->setPosition(Vec2(
                    REF.x + cardWidth*1.0*13 + 30,
                    REF.y));
				lastTwo[1]->setScale(0);
				myframe->addChild(lastTwo[1],0+18,HAND_IN_CARDS_TAG_ID+1*20+13);
                
				lastTwo[1]->runAction(Sequence::create(DelayTime::create(timeXH[1]+2),ScaleTo::create(0,1),NULL));
			}
		}
	}
    
	/**********************************************************
        distribute to right player
    **********************************************************/
    auto cardHeight=_object->RectSize(LR_IN_CARD).height;

	auto rHandinHeight=_object->RectSize(R_IN_CARD).height;

	Point RinghtHandoutEnd=Vec2(
        _layout->_playerPosi[2].basePoint.x+10,
        _layout->_playerPosi[2].basePoint.y+10);

	Sprite* RobotHandCard[4];
	Vec2 RightHandPosition[4];
	for (a=0;a<4;a++)
	{
		RobotHandCard[a]=_object->Create(LR_IN_CARD);
		RobotHandCard[a]->setAnchorPoint(Vec2(0.5,0));
		RobotHandCard[a]->setPosition(Vec2(
            origin.x+visibleSize.width*0.5,
            origin.y+visibleSize.height*0.5-cardHeight*1.65+cardHeight*a*0.65));
		RobotHandCard[a]->setScale(0);
		myframe->addChild(RobotHandCard[a],6-a,START_CARDS_IN_TAG_ID+2*a);
		RightHandPosition[a]=RobotHandCard[a]->getPosition();
	}
    
    for(a=0;a<4;a++)
	{
		for (b=0;b<4;b++)
		{
			changingPosition[b]=RinghtHandoutEnd.y;
		}
		x=RinghtHandoutEnd.x;
		changingPosition[0]+=rHandinHeight*(a*0.4+0.6);				
		changingPosition[1]+=rHandinHeight*((a+4)*0.4+0.6);			
		changingPosition[2]+=rHandinHeight*((a+8)*0.4+0.6);	
		changingPosition[3]+=rHandinHeight*((13+a)*0.4+0.6);
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

    for(a=0;a<4;a++) {
        _RightBatchDistribute(a,timeXH[2],(zhuang==2)?14:13);
    }
    
	/**********************************************************
        distribute to left player
    **********************************************************/
    auto lHandinHeight=_object->RectSize(L_IN_CARD).height;

	Point LeftHandoutEnd=Vec2(
        _layout->_playerPosi[0].basePoint.x+10,
        _layout->_playerPosi[0].basePoint.y+10);
    
	Sprite* LeftRobotHandCard[4];
	Vec2 LeftHandPosition[4];
	for (a=0;a<4;a++)
	{
		LeftRobotHandCard[a]=_object->Create(LR_IN_CARD);
		LeftRobotHandCard[a]->setAnchorPoint(Vec2(0.5f,0.5f));
		LeftRobotHandCard[a]->setPosition(Vec2(
            origin.x+visibleSize.width*0.5,
            origin.y+visibleSize.height*0.5+cardHeight*1.2-cardHeight*a*0.6));
		LeftRobotHandCard[a]->setScale(0);
		myframe->addChild(LeftRobotHandCard[a],3,START_CARDS_IN_TAG_ID+3*a);
		LeftHandPosition[a]=LeftRobotHandCard[a]->getPosition();
	}

	for(a=0;a<4;a++)
	{
		for (b = 0; b < 4; b++)
		{
			changingPosition[b]=LeftHandoutEnd.y;
		}
		x=LeftHandoutEnd.x;
		changingPosition[0]-=lHandinHeight*a*0.6;				
		changingPosition[1]-=lHandinHeight*(a+4)*0.6;			
		changingPosition[2]-=lHandinHeight*(a+8)*0.6;	
		changingPosition[3]-=((lHandinHeight*13+a*lHandinHeight)*0.6+13.6);
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

	for(a=0;a<4;a++) {
	    _LeftBatchDistribute(a,timeXH[0],(zhuang==0)?14:13);
    }
	/**********************************************************
        
    **********************************************************/
	_roundManager->_isCardFromOthers=false;
	_roundManager->_distributedNum=0;	
	_roundManager->_actionToDo=a_JUMP;
    
	ListenToDistributeCard();

	myframe->_ID = zhuang;
	myframe->runAction(Sequence::create(
        _FisrtRoundResidueUpdate(),Spawn::create(CallFunc::create([=](){
		_CardInHandUpdateEffect(LEFT);}),CallFunc::create([=](){
		_CardInHandUpdateEffect(MIDDLE);}),CallFunc::create([=](){
		_CardInHandUpdateEffect(RIGHT);}),NULL),CallFunc::create([=](){
		UpdateClock(0,zhuang);}),CallFunc::create(this,callfunc_selector(
        NetRaceLayer::_DeleteStartDealCards)),CCCallFunc::create([=](){
		WaitForFirstAction(zhuang);
	}),NULL));
}

Sequence *NetRaceLayer::_FisrtRoundResidueUpdate() {
	auto updateFourCards=CallFunc::create([=](){
		_roundManager->_distributedNum += 4;
		_UpdateResidueCards(TOTAL_CARD_NUM - _roundManager->_distributedNum);		
	});
	auto updateTwoCards=CallFunc::create([=](){
		_roundManager->_distributedNum +=2;
		_UpdateResidueCards(TOTAL_CARD_NUM - _roundManager->_distributedNum);		
	});
	auto updateOneCards=CallFunc::create([=](){
		_roundManager->_distributedNum +=1;
		_UpdateResidueCards(TOTAL_CARD_NUM - _roundManager->_distributedNum);		
	});

	auto HandDelay   = DelayTime::create(0.2);
	auto HandFourSeq = Sequence::create(updateFourCards,HandDelay,NULL);
	auto HandOneSeq  = Sequence::create(updateOneCards,HandDelay,NULL);
	auto HandTwoSeq  = Sequence::create(updateTwoCards,HandDelay,NULL);

	return Sequence::create(
        DelayTime::create(0.7),
        HandFourSeq,HandFourSeq,HandFourSeq,
        HandFourSeq,HandFourSeq,HandFourSeq,
        HandFourSeq,HandFourSeq,HandFourSeq,

        HandTwoSeq, HandOneSeq, HandOneSeq,NULL);
}

void NetRaceLayer::ListenToDoubleHu() {
    auto _doublehucallListener = EventListenerCustom::create(DOUBLE_HU_WITH_ME, [this](EventCustom * event){
        
        auto DoubleFileFunc = CallFunc::create([=]{
            _eventDispatcher->removeCustomEventListeners(DOUBLE_HU_WITH_ME);
            
            auto yiPaoShuangXiang = Sprite::create("ypsx.png");
            yiPaoShuangXiang->setAnchorPoint(Vec2(0.5,0.5));
            yiPaoShuangXiang->setPosition(Vec2(
                origin.x+visibleSize.width/2,
                origin.y+visibleSize.height - yiPaoShuangXiang->getTextureRect().size.height/2));
            yiPaoShuangXiang->setOpacity(0);
            myframe->addChild(yiPaoShuangXiang,30);//字
    
            yiPaoShuangXiang->runAction(
                Sequence::create(
                    Spawn::create(
                        FadeIn::create(0.1),
                        EaseIn::create(
                            MoveTo::create(0.2,Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2)),2),NULL),
                    DelayTime::create(1),
                    FadeOut::create(0.8),NULL));
    
            auto xian = LayerColor::create();
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
        
        myframe->runAction(Sequence::create(
            Spawn::create(
                DoubleFileFunc,CallFunc::create(this,callfunc_selector(
                NetRaceLayer::showall)),
                simple_tip_effect( _layout->PositionOfActSign((PlayerDir_t)((_roundManager->_curPlayer+1)%3)),"dahu.png"),
                simple_tip_effect( _layout->PositionOfActSign((PlayerDir_t)((_roundManager->_curPlayer+2)%3)),"dahu.png"),NULL),CallFunc::create([=](){
            GoldNumInsert(3,3,_roundManager->_curPlayer);}),NULL));
    });
    
    _eventDispatcher->addEventListenerWithFixedPriority(_doublehucallListener,2);
}

/***********************************************
            graphical residue cards
***********************************************/
void NetRaceLayer::_CreateResidueCards() {
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

	_InitResidueCards();
}

void NetRaceLayer::_InitResidueCards() {
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

void NetRaceLayer::_UpdateResidueCards(int no)
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

void NetRaceLayer::GuiShowReady(int dir)
{
	auto Ready = Sprite::createWithSpriteFrameName("zhunbei.png");
	Ready->setAnchorPoint(Vec2(0.5,1));
	Ready->setScale(0.8);

	if( dir==0 ) {
		Ready->setPosition(Vec2(
            _layout->_playerBkg[0]->getPosition().x + _layout->_playerBkg[0]->getContentSize().width/2,
            _layout->_playerBkg[0]->getPosition().y - _layout->_playerBkg[0]->getContentSize().height));
		this->addChild(Ready,2,READY_INDICATE_LEFT_TAG_ID);
	} else if( dir==1 ) {
		;
	} else if( dir==2 ) {
		Ready->setPosition(Vec2(
            _layout->_playerBkg[2]->getPosition().x - _layout->_playerBkg[2]->getContentSize().width/2,
            _layout->_playerBkg[2]->getPosition().y - _layout->_playerBkg[2]->getContentSize().height));
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

/****************************************************
    effect position
****************************************************/

void NetRaceLayer::_CreateGangCardsMotion(TargetedAction *motions[4]) {
    
    for (int i=0; i<4; i++ ) {
        auto GangCard = _object->Create(AN_GANG_CARD);
        Size GangCardSize = GangCard->getTextureRect().size;
        GangCard->setAnchorPoint(Vec2(0,0));       
        GangCard->setScale(0);
        
        GangCard->setPosition(_layout->OrigPositionOfGangCard(i,GangCardSize));
        myframe->addChild(GangCard,30,SINGLE_ACTION_EFFECT_TAG_ID+i);

        motions[i] = TargetedAction::create(GangCard,Sequence::create(
            DelayTime::create(0.42),
            ScaleTo::create(0,1),
            MoveTo::create(0.12,_layout->MiddlePositionOfGangCard(i,GangCardSize)),
            MoveTo::create(0.12,_layout->DestPositionOfGangCard(i,GangCardSize)),NULL));
    }
}

void NetRaceLayer::_CreateMingGangCardsMotion(TargetedAction *mostions[4],CARD_KIND kind) {
    
    for (int i=1;i<4;i++) {
        auto card = _object->Create(PENG_CARD);
        Size size = card->getTextureRect().size;
        _AttachKindTexture(card,kind);

        card->setAnchorPoint(Vec2(0,0));
        card->setScale(0);
        card->setPosition(_layout->OrigPositionOfGangCard(i,size));
        myframe->addChild(card,29,SINGLE_ACTION_EFFECT_TAG_ID+i);
        
        mostions[i] = TargetedAction::create(card,Sequence::create(
            DelayTime::create(0.42),
            ScaleTo::create(0,1),
            DelayTime::create(0.12),
            MoveTo::create(0.12,_layout->MiddlePositionOfGangCard(i,size)),
            MoveTo::create(0.12,_layout->DestPositionOfGangCard(i,size)),NULL));
    }
}


void NetRaceLayer::_CreateGangCardInHandMotion(TargetedAction *motions[4],int cardInHand[4],CARD_KIND kind) {
    
    Size FreeCardSize = _object->RectSize(FREE_CARD);
    auto GangCardSize = _object->RectSize(AN_GANG_CARD);

    for (int i=0;i<4;i++ ) {
        auto OldCard = (Sprite*)myframe->getChildByTag(HAND_IN_CARDS_TAG_ID + cardInHand[i] + 1*20);//gang1
        auto OldPos  = OldCard->getPosition();
        auto OldScale = OldCard->getScale();
        auto OldSize = OldCard->getTextureRect().size;
        
        auto GangCard=_object->Create(FREE_CARD);//gang1
        GangCard->setScale(OldScale);
        GangCard->setAnchorPoint(Vec2(0,0));
        GangCard->setPosition(Vec2(OldPos.x,OldPos.y));
        
        auto GangKind=_object->CreateKind((Card_t)kind,NORMAL);
        GangKind->setAnchorPoint(Vec2(0.5,0.5));
        /*!!! Gang1Kind.Positon use OldGang2Size, maybe mal-spell ??? */
        GangKind->setPosition(Vec2(OldSize.width/2,OldSize.height*0.4));
        GangCard->addChild(GangKind,1);
        myframe->addChild(GangCard,20,EFFET_NEWCATD1_TAG+i);

        motions[i] = TargetedAction::create(GangCard,Sequence::create(
            DelayTime::create(0.18),Spawn::create(
                ScaleTo::create(0,0.6),
                MoveTo::create(0,_layout->MiddlePositionOfGangCardInHand(i,OldPos,FreeCardSize)),NULL),
            MoveTo::create(0.18,_layout->DestPositionOfGangCardInHand(i,GangCardSize)),
            DelayTime::create(0.06),
            ScaleTo::create(0,0),NULL));
    }
}

void NetRaceLayer::_AttachKindTexture(Sprite *parent,CARD_KIND kind) {
    auto kindTexture = _object->CreateKind((Card_t)kind,MIDDLE_SIZE);
    
    kindTexture->setAnchorPoint(Vec2(0.5,0.5));
    kindTexture->setPosition(Vec2(
        parent->getTextureRect().size.width/2,
        parent->getTextureRect().size.height*0.65));
    parent->addChild(kindTexture);
}


void NetRaceLayer::_AttachKindTextureToFreeCard(Sprite *parent,CARD_KIND kind) {
    auto kindTexture = _object->CreateKind((Card_t)kind,NORMAL);
    
    kindTexture->setAnchorPoint(Vec2(0.5,0.5));
    kindTexture->setPosition(Vec2(
        parent->getTextureRect().size.width/2,
        parent->getTextureRect().size.height*0.4));
    parent->addChild(kindTexture);
}
    
/* the first is non-peng card*/
void NetRaceLayer::_CreateMingGangCardInHandMotion(TargetedAction *motions[3], int idxInHand[3], CARD_KIND kind) {
    
	float delayTime=0.18;
    
    for (int i=0;i<3;i++) {
		auto OldCard = (Sprite*)myframe->getChildByTag(HAND_IN_CARDS_TAG_ID + 1*20 + idxInHand[i]);
		auto OldPos  = OldCard->getPosition();
		auto OldSize = OldCard->getTextureRect().size;
        
        Sprite* card = _object->Create(PENG_CARD);
        card->setAnchorPoint(Vec2(0,0));
        card->setPosition(Vec2(OldPos.x,OldPos.y));

        _AttachKindTexture(card,kind);
        
        myframe->addChild(card,20,(EFFET_NEWCATD1_TAG+1)+i);
        motions[i] = TargetedAction::create(card,Sequence::create(
            DelayTime::create(delayTime),
            MoveTo::create(0.18,_layout->PositionOfMingGangCard((i+1),card->getTextureRect().size)),
            DelayTime::create(0.06),
            ScaleTo::create(0,0),NULL));
    }
}

void NetRaceLayer::_CreateFreeCard(Sprite *cards[3], int idxInHand[3], CARD_KIND kind) {
    

    for(int i=0;i<3;i++) {
 		auto OldCard = (Sprite*)myframe->getChildByTag(HAND_IN_CARDS_TAG_ID + 1*20 + idxInHand[i]);
		auto OldPos  = OldCard->getPosition();
		auto OldSize = OldCard->getTextureRect().size;
        
        cards[i]=_object->Create(FREE_CARD);
        cards[i]->setScale(OldCard->getScale());
        cards[i]->setAnchorPoint(Vec2(0,0));
        cards[i]->setPosition(Vec2(OldPos.x,OldPos.y));
        
        _AttachKindTextureToFreeCard(cards[i],kind);

        myframe->addChild(cards[i],20,EFFET_NEWCATD1_TAG+1+i);
    }
}

void NetRaceLayer::_CreateBackgroundElementMotion(TargetedAction *motions[5],int gangType) {
    Size GangCardSize = _object->RectSize(AN_GANG_CARD);

    const double delays[2][5] = {
        {0.66,   0.9,   1.14,   0.66,   0.66},/* an gang */
        {0.78,  1.02,   1.26,   0.78,   0.78},/* ming gang */
    };
    
    auto lightCircle = Sprite::createWithSpriteFrameName("4.png");
    lightCircle->setPosition(Vec2(
        origin.x+visibleSize.width*0.5+GangCardSize.width*0.98,
        origin.y+visibleSize.height*0.315));
    myframe->addChild(lightCircle,32,IMG_4_EFFECT_TAG_ID);
    lightCircle->setScale(0);
    BlendFunc tmp_oBlendFunc ={GL_SRC_ALPHA, GL_ONE};
    lightCircle->setBlendFunc(tmp_oBlendFunc);
    motions[0] = TargetedAction::create(lightCircle, 
        Sequence::create(
            DelayTime::create(delays[gangType][0]),
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
    motions[1]=TargetedAction::create(lightCircle1,
        Sequence::create(
            DelayTime::create(delays[gangType][1]),
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
    motions[2] = TargetedAction::create(lightCircle2,
        Sequence::create(
            DelayTime::create(delays[gangType][2]),
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
    motions[3]=TargetedAction::create(yellowlight,
        Sequence::create(
            DelayTime::create(delays[gangType][3]),
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
    motions[4]=TargetedAction::create(light,Sequence::create(
            DelayTime::create(delays[gangType][4]),
            ScaleTo::create(0.12,0.5),
        Spawn::create(
            ScaleTo::create(0.18,0),
            FadeOut::create(0.18),NULL),NULL));
}

Sequence *NetRaceLayer::_HideReminder(int reminderTag, double lastingTime, double shadeScale) {
    auto shadeAction = TargetedAction::create((Sprite*)myframe->getChildByTag(reminderTag),
        Sequence::create(
            ScaleTo::create(0,1), 
            Spawn::create(
                FadeOut::create(lastingTime),
                ScaleTo::create(lastingTime,shadeScale),NULL),NULL));
    
    return Sequence::create( 
        Spawn::create(
            shadeAction, CCCallFunc::create([=]() {/* why devide into 2 parts??? is it ok to execute as _ShadeQiReminder*/
            for(int i=0; i<11; i++) {
                if(i==reminderTag) {
                    continue;
                } else {
                    _Remove(myframe,REMIND_ACT_TAG_ID+i);
                }
            }}),NULL),CCCallFunc::create([=](){
        _Remove(myframe,reminderTag);}),NULL);
}

Sequence *NetRaceLayer::_HideQiReminder() {
    auto shadeAction = TargetedAction::create((Sprite*)myframe->getChildByTag(QI_REMIND_ACT_BKG_TAG_ID),
        Sequence::create(
            ScaleTo::create(0,1), 
            Spawn::create(
                FadeOut::create(0.3),
                ScaleTo::create(0.3,1.3),NULL),NULL));

    return Sequence::create(
            shadeAction,CCCallFunc::create(this,callfunc_selector(
            NetRaceLayer::_DeleteActionReminder)),NULL);
}

Sprite *NetRaceLayer::_GetCardInHand(PlayerDir_t dir,int idx) {
    return (Sprite *)myframe->getChildByTag(HAND_IN_CARDS_TAG_ID + dir*20 + idx);
}

void NetRaceLayer::_ReOrderCardsInHand(int droppedCard,CARD_ARRAY *cards) {
    /*NOTE : this function is called after handout ,so the position is 1 bigger than length */
    const int  LAST      = (cards->len-1)+1;

    if(droppedCard==LAST) {
		_GetCardInHand(MIDDLE,droppedCard)->setScale(0);
        return;
    }
    
    const auto SIZE      = _GetCardInHand(MIDDLE,0)->getTextureRect().size;
    /* maybe need to be put into the circulation */
    const auto LAST_CARD = _GetCardInHand(MIDDLE,LAST);
    const auto LAST_POS  = LAST_CARD->getPosition();
    
    /* need to be optimized !!! */
    for(int i=cards->atcvie_place; i<LAST; i++) {
        auto card = _GetCardInHand(MIDDLE,i);
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
                    
        if(i < droppedCard) {
            if(cards->data[i].kind <= cards->data[LAST].kind)
                continue;
            else {
                if( i==cards->atcvie_place || cards->data[i-1].kind<=cards->data[LAST].kind )
                    LAST_CARD->runAction(RightInsertSeq);
                
                card->runAction(RightMove);
            }
        } else if(i==droppedCard) {
            if(i==cards->atcvie_place) {
                if(cards->data[i+1].kind>cards->data[LAST].kind)
                    LAST_CARD->runAction(RightInsertSeq);
            } else if(i==LAST-1) {
                if(cards->data[i-1].kind<=cards->data[LAST].kind)
                    LAST_CARD->runAction(MoveTo::create(0.3,Vec2(curPos.x,LAST_POS.y)));
            } else {
                if(cards->data[i-1].kind<=cards->data[LAST].kind&&cards->data[i+1].kind>cards->data[LAST].kind)
                    LAST_CARD->runAction(RightInsertSeq);
            }
            
            card->setScale(0);
        } else {/* i>chosenCard */
            if(cards->data[i].kind>cards->data[LAST].kind)
                card->runAction(MoveTo::create(0.3,Vec2(curPos.x-SIZE.width*0.2,curPos.y)));
            else {
                if(i==LAST-1)
                    LAST_CARD->runAction(MoveTo::create(0.3,Vec2(curPos.x-SIZE.width*0.2,curPos.y)));
                else if(cards->data[i+1].kind>cards->data[LAST].kind)
                    LAST_CARD->runAction(LeftInsertSeq);                    
                card->runAction(LeftMove);
            }
        }
    }
}

TargetedAction *NetRaceLayer::_ShowBigMing(Node *myframe) {
    auto damingFont = Sprite::createWithSpriteFrameName("daming.png");
    
    damingFont->setAnchorPoint(Vec2(0.5,0.5));
    damingFont->setOpacity(0);
    damingFont->setPosition(_layout->PositionOfMingSign());

    myframe->addChild(damingFont,31,MING_EFFECT_DAMING);

    return TargetedAction::create(damingFont,Sequence::create(
        FadeIn::create(0.18),
        DelayTime::create(0.36),Spawn::create(
        FadeOut::create(0.18),
        ScaleTo::create(0.18,1.2),NULL),NULL));
}

TargetedAction* NetRaceLayer::_MingAnimation() {
    _eventDispatcher->removeEventListenersForTarget(myframe,true);
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ming-tx.plist");
    
    while(myframe->getChildByTag(MING_EFFECT_DAMING))
        myframe->removeChildByTag(MING_EFFECT_DAMING);
    
    while(myframe->getChildByTag(MING_EFFECT_ANIMATE))
        myframe->removeChildByTag(MING_EFFECT_ANIMATE);
    
    auto mingSign=Sprite::createWithSpriteFrameName("ming_1.png");
        mingSign->setAnchorPoint(Vec2(0.5,0.5));
        mingSign->setScale(0);
        mingSign->setPosition(_layout->Center());

    myframe->addChild(mingSign,30,MING_EFFECT_ANIMATE);
    
    auto MingAnimate = Animation::create();
        MingAnimate->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ming_1.png"));
        MingAnimate->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ming_2.png"));
        MingAnimate->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ming_3.png"));
        MingAnimate->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ming_4.png"));
        MingAnimate->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ming_5.png"));
        MingAnimate->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ming_6.png"));
        MingAnimate->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ming_7.png"));
        MingAnimate->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ming_8.png"));
        MingAnimate->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ming_9.png"));
        MingAnimate->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ming_10.png"));
        MingAnimate->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ming_11.png"));
        MingAnimate->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ming_12.png"));
        MingAnimate->setDelayPerUnit(0.1f);
        MingAnimate->setRestoreOriginalFrame(true);
    
    return TargetedAction::create(
            mingSign,Sequence::create(
                ScaleTo::create(0,2),
                Animate::create(MingAnimate),
                ScaleTo::create(0,0),NULL));
}

TargetedAction *NetRaceLayer::_OthersShowCardEffect(PlayerDir_t dir,Card_t outCard,bool canKou) {
	auto smallCard = _object->CreateKind(outCard,SMALL);
	smallCard->runAction(RotateTo::create(0,_layout->RotateAngleOfOutcard(dir)));
    
	if(myframe->getChildByTag(SHOW_CARD_INIDCATOR_TAD_ID)) {
		myframe->removeChildByTag(SHOW_CARD_INIDCATOR_TAD_ID);
	}
    
	auto cardInHand = _roundManager->_players[dir]->get_parter()->get_card_list();
	auto LastCard   = _GetCardInHand(dir,cardInHand->len);
	auto hideLastInHand = CallFunc::create([=](){
		if(LastCard)
			LastCard->setScale(0);
	});

	CallFunc* showAndHideOutcardNotice = CallFunc::create([=](){
		auto cardFrame = Sprite::create("tileImage/tile_lastTileBG.png");
		cardFrame->setAnchorPoint(_layout->AnchorOfOutcard(dir));
		cardFrame->setPosition(_layout->PositionOfOutcard(dir));
		myframe->addChild(cardFrame,35,SHOW_CARD_INIDCATOR_TAD_ID);
        
		auto cardBg = _object->Create(FREE_CARD);
		cardBg->setAnchorPoint(Vec2(0.5,0.5));
		cardBg->setPosition( Vec2(cardFrame->getTextureRect().size.width*0.515,cardFrame->getTextureRect().size.height*0.515) );
		cardFrame->addChild(cardBg);
        
		auto card = _object->CreateKind(outCard,NORMAL);
		card->setAnchorPoint(Vec2(0.5,0.5));
		card->setPosition(Vec2(cardBg->getTextureRect().size.width/2,cardBg->getTextureRect().size.height*0.4));
		cardBg->addChild(card);
        
		cardFrame->runAction(Sequence::create(
            DelayTime::create(1.5),
            ScaleTo::create(0,0),NULL));
	});
    
	if(myframe->getChildByTag(OUT_CARD_FRAME_TAG_ID))
		myframe->removeChildByTag(OUT_CARD_FRAME_TAG_ID);

	Sprite   * cardOut = _object->Create(LR_OUT_CARD);
	smallCard->setPosition(Vec2(
        cardOut->getTextureRect().size.width/2,
        cardOut->getTextureRect().size.height*0.65));
	smallCard->setRotation(-90);/* !!! player0 is 90 in the old source, but it seems ok to set it as -90*/
	smallCard->setScale(0.9);
	cardOut->addChild(smallCard);
	cardOut->setAnchorPoint(Vec2(0.5,0.5));

    Vec2 curOutPos;/* here must be something I have not known */
    if(dir==RIGHT) {
		auto curOutPosTemp = _GetCardInHand(dir,cardInHand->len-1)->getPosition();
		curOutPos = Vec2(
            curOutPosTemp.x,
            curOutPosTemp.y + cardOut->getTextureRect().size.height*1.5);
	}
	else {
		auto cardOut_Yposition = _GetCardInHand(dir,cardInHand->len-1)->getPosition().y;
        curOutPos = Vec2(
            _layout->_playerPosi[dir].basePoint.x + 10,
            cardOut_Yposition - 20 - 35);
	}

    cardOut->setPosition(curOutPos);
    cardOut->setVisible(false);
    myframe->addChild(cardOut,0,OUT_CARD_FRAME_TAG_ID);/* !!! player2 is 20 in the old source, but it seems ok to set it as 0*/

    BezierTo *inHandMoveToOutHand = OthersBizerMove(dir,
                                        _roundManager->_players[dir]->get_parter()->getOutCardList());
    
	Sequence *result = Sequence::create(
                		hideLastInHand,CallFunc::create([=](){ 
                        _Show(myframe,OUT_CARD_FRAME_TAG_ID,true);} ),Spawn::create(
                    		showAndHideOutcardNotice,
                    		inHandMoveToOutHand,
                    		_voice->SpeakCard(outCard,
                                _roundManager->_cardHolders[dir]->GetSex()),NULL),
                		_voice->Speak("give"),NULL);;	

	if(canKou) {
		result = Sequence::create(
                        simple_tip_effect( _layout->PositionOfActSign(dir),"daming.png" ),
                        result,NULL);
	}

    return TargetedAction::create(cardOut,result);
}

void NetRaceLayer::_OthersMingGangEffect(PlayerDir_t dir,PlayerDir_t prevDir,bool isCardFromOthers) {
    Sequence *gang_seq;
    
    auto hideOutCard = Sequence::create(NULL);
    auto goldEffect  = CallFunc::create([=](){NULL;});

    if(isCardFromOthers) {
        hideOutCard = Sequence::create(CCCallFunc::create([=](){
            _Show(this,SHOWCARD_INDICATOR_TAG_ID,false);}),TargetedAction::create(
            myframe->getChildByTag(OUT_CARD_FRAME_TAG_ID),Sequence::create(
                DelayTime::create(0.06),
                ScaleTo::create(0,0),NULL)),NULL);

        goldEffect = CallFunc::create([=](){
            GoldNumInsert(dir,2,prevDir);});
    }
    
    myframe->runAction(Sequence::create(
        hideOutCard,
        Spawn::create(
            simple_tip_effect(_layout->PositionOfActSign(dir),"gang.png"),
            _voice->SpeakAction(GANG,_roundManager->_cardHolders[dir]->GetSex()),NULL),
        goldEffect,
        Sequence::create(CCCallFunc::create(this,callfunc_selector(
            NetRaceLayer::_DeleteActionTip)),CCCallFunc::create([=]() {
            _roundManager->UpdateCards(dir,a_MING_GANG);}),CCCallFunc::create([=]() {
            _CardInHandUpdateEffect(dir);}),CallFunc::create([=](){
            _roundManager->ActionAfterGang(dir);}),NULL),NULL));
}

void NetRaceLayer::_MyHandoutEffect(Card_t outCard,Vec2 touch,int time,bool turnToMing)
{
	if(time==1) {
		if(myframe->getChildByTag(CHOOSE_CARD_TAG_ID))
			myframe->removeChildByTag(CHOOSE_CARD_TAG_ID);
	}

    if(myframe->getChildByTag(OUT_CARD_FRAME_TAG_ID)!=NULL)
		myframe->removeChildByTag(OUT_CARD_FRAME_TAG_ID);// why without true/false

    _Show(myframe, TING_SING_BAR, false);
    _Remove(myframe, MING_CANCEL);// why called twice ???

	BezierTo *bizerMotion;
    auto cardOut = _object->CreateRiverCard(MIDDLE,outCard);
    
	if(time==1) {
		cardOut->setPosition(touch);
		bizerMotion = BizerMove1(_roundManager->_players[MIDDLE]->get_parter()->getOutCardList(),
		                touch);
    }
	else {
		cardOut->setPosition(touch.x,100);
		bizerMotion = BizerMove2(_roundManager->_players[MIDDLE]->get_parter()->getOutCardList(),
		                touch,time);
    }
    myframe->addChild(cardOut,30,OUT_CARD_FRAME_TAG_ID);

	CallFunc* speakCard  = _voice->SpeakCard(outCard,_roundManager->_cardHolders[MIDDLE]->GetSex());

    Spawn* mingEffect = turnToMing?
                (Spawn::create(
                    _ShowBigMing(myframe),
                    _MingAnimation(),NULL))
                :(Spawn::create(CallFunc::create([=](){;}),NULL));
        
	Spawn* allEffect = Spawn::create(
            mingEffect,TargetedAction::create(
            cardOut,Sequence::create(Spawn::create(
                bizerMotion,
                speakCard,NULL),
                _voice->Speak("give"),NULL)),CallFunc::create([=](){
            ifInsertCardsTime=true;}),CallFunc::create([=](){
            _DeleteActionReminder();}),NULL);

	myframe->_ID=1;
	_roundManager->_isCardFromOthers = true;
    
	myframe->runAction(Sequence::create(
        allEffect,
        DelayTime::create(0.12),CallFunc::create([=](){
        ifInsertCardsTime=false;}),Sequence::create(CCCallFunc::create([=]() {
    		_CardRiverUpdateEffect(MIDDLE);}),CCCallFunc::create([=]() {
            _roundManager->UpdateCards(MIDDLE,a_JUMP);
            _Show(myframe,TING_SING_BUTTON,true);}),CallFunc::create([=](){
    		if(_isCardInHandUpdated)
    			_isCardInHandUpdated = false;
    		else {
                _Show(this,MING_STATUS_PNG_1,_roundManager->IsTing(1));
    			_CardInHandUpdateEffect(MIDDLE);
    		}}),NULL),CCCallFunc::create([=]() {
		_roundManager->WaitForResponse(MIDDLE);}),
        NULL));
}

/************************************************
                support
************************************************/
void NetRaceLayer::_CreateMeneButtons() {
    GMenu *gMenu = _object->CreateMenu();
    this->addChild(gMenu->_bkg,4,MENU_BKG_TAG_ID);

    auto buttonImg = Sprite::createWithSpriteFrameName("baomingbisai2.png");
    
    auto flagButton = _object->CreateButton(MENUBTN_BAOMING);
    gMenu->AddItem(flagButton,buttonImg);

    auto robotButton = _object->CreateButton(MENUBTN_TUOGUAN);
    robotButton->setTouchEnabled(false);
    robotButton->addTouchEventListener(CC_CALLBACK_2(NetRaceLayer::BtnTuoGuanHandler,this));
    gMenu->AddItem(robotButton);

    auto setButton = _object->CreateButton(MENUBTN_SHEZHI);
    gMenu->AddItem(setButton);

    auto mallButton = _object->CreateButton(MENUBTN_SHOP);
    gMenu->AddItem(mallButton);

    auto backButton = _object->CreateButton(MENUBTN_GAMEBACK);
    backButton->addTouchEventListener(CC_CALLBACK_2(NetRaceLayer::BtnBackHandler,this));
    gMenu->AddItem(backButton);
}

void NetRaceLayer::_CreateHeadImage() {
    Sprite *bkg = _object->CreateHeadBkg(LEFT);
    _layout->SetPlayerBkg(LEFT,bkg);
    this->addChild(_layout->_playerBkg[0],1,LEFT_IMG_BKG_TAG_ID);

    bkg = _object->CreateHeadBkg(RIGHT);
    _layout->SetPlayerBkg(RIGHT,bkg);
    this->addChild(_layout->_playerBkg[2],1,RIGHT_IMG_BKG_TAG_ID);

    bkg = _object->CreateHeadBkg(MIDDLE);
    _layout->SetPlayerBkg(MIDDLE,bkg);
    this->addChild(_layout->_playerBkg[1],1,MID_IMG_BKG_TAG_ID);
}

void NetRaceLayer::_RaceBeginPrepare() {
    _Remove(this,READY_INDICATE_LEFT_TAG_ID);
    _Remove(this,READY_INDICATE_RIGHT_TAG_ID);
    _Remove(this,READY_INDICATE_MID_TAG_ID);

	(Sprite*)this->getChildByTag(LEFT_IMG_BKG_TAG_ID)->runAction(
        EaseIn::create(MoveTo::create(0.6,Vec2(29,536)),1.5));
	(Sprite*)this->getChildByTag(RIGHT_IMG_BKG_TAG_ID)->runAction(
        EaseIn::create(MoveTo::create(0.6,Vec2(1190,536)),1.5));
	(Sprite*)this->getChildByTag(MID_IMG_BKG_TAG_ID)->runAction(
        EaseIn::create(MoveTo::create(0.6,Vec2(49,129)),1.5));
    
	(Sprite*)this->getChildByTag(MIC_TAG_ID)->runAction(Sequence::create(
        DelayTime::create(0.65),
        EaseBounceOut::create(ScaleTo::create(0.3,1)),NULL));
    
	for(int i=CARD_ARRAGE_LEFT_TAG_ID;i<=CARD_ARRAGE_TOP_TAG_ID;i++) {
		(Sprite*)this->getChildByTag(i)->runAction(
		    EaseIn::create(FadeOut::create(0.2),2));
    }

	auto colorLayer=LayerColor::create(Color4B(0,0,0,150),visibleSize.width,visibleSize.height*200/716);
	colorLayer->setAnchorPoint(Vec2(0.5,0.5));
	colorLayer->ignoreAnchorPointForPosition(false);
	colorLayer->setScale(0);
	colorLayer->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
	this->addChild(colorLayer,4,COLOR_LAYER_TAG_ID);

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

	paijuStart->runAction(
        Sequence::create(
            DelayTime::create(0.7),
            ScaleTo::create(0,4),
            Spawn::create(
                FadeIn::create(0.2),
                ScaleTo::create(0.2,1),NULL),
            Spawn::create(
                _voice->Speak("duijukaishi.ogg"),
                Sequence::create(
                    DelayTime::create(0.5),
                    ScaleTo::create(0.15,0.5),
                    Spawn::create(
                        ScaleTo::create(0.3,4),
                        FadeOut::create(0.3),NULL),NULL),NULL),NULL));
    
	colorLayer->runAction(
        Sequence::create(
            DelayTime::create(0.9),
            ScaleTo::create(0,1),
            DelayTime::create(0.9),
            ScaleTo::create(0.4,1,0),NULL));
    
	paijuStart_guang->runAction(
        Sequence::create(
            DelayTime::create(1.7),
            Spawn::create(
                ScaleTo::create(0.3,8,3),
                FadeIn::create(0.3),NULL),
                Spawn::create(
                    ScaleTo::create(0.5,8,0),
                    FadeOut::create(0.5),NULL),NULL));

    _Show(this,SHOWCARD_INDICATOR_TAG_ID,false);

    int lastWinner = _roundManager->GetLastWinner();
    _zhuangSign->setAnchorPoint(_layout->AnchorOfZhuang((PlayerDir_t)lastWinner));
    _zhuangSign->setPosition(_layout->_playerPosi[lastWinner].zhuangPoint);
	_zhuangSign->runAction(Sequence::create(
        DelayTime::create(0.65),
        EaseBounceOut::create(ScaleTo::create(0.3,1)),NULL));

	auto MingSignForMe=(Sprite*)this->getChildByTag(MING_STATUS_PNG_1);
	if(lastWinner!=1) {
        MingSignForMe->setPosition(_layout->PositionOfMingSignForMe());
    } else {
        MingSignForMe->setPosition(_layout->PositionOfMingSignForMe(_zhuangSign->getTextureRect().size.width));
    }
}

bool NetRaceLayer::_ResourcePrepare()
{
	this->addChild(_object->CreateMingSign(LEFT),2,MING_STATUS_PNG_0);
	this->addChild(_object->CreateMingSign(MIDDLE),2,MING_STATUS_PNG_1);
	this->addChild(_object->CreateMingSign(RIGHT),2,MING_STATUS_PNG_2);

    _zhuangSign = _object->CreateZhuangSign();
	this->addChild(_zhuangSign,1,LAST_WINNER_TAG_ID);
	this->addChild(_object->CreateClock(),3,ALARM_CLOCK_INDICATOR_TAG_ID);

	this->addChild(_object->CreateClockIndicator(MIDDLE),3,ALARM_CLOCK_INDICATE_DOWN_TAG_ID);
	this->addChild(_object->CreateClockIndicator(RIGHT),4,ALARM_CLOCK_INDICATE_RIGHT_TAG_ID);
	this->addChild(_object->CreateClockIndicator(LEFT),4,ALARM_CLOCK_INDICATE_LEFT_TAG_ID);

	this->addChild(_object->CreatePlayerPointer(),5,SHOWCARD_INDICATOR_TAG_ID);

	return true;
}

void NetRaceLayer::_StartParticleSystem(float delta)
{
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

void NetRaceLayer::_UpdateNonChosenCards(const CardsInfo_t &cards, int chosen) {
    auto cardSize = _object->RectSize(FREE_CARD);

    float y1 = _layout->_playerPosi[MIDDLE].basePoint.y+10;
    auto startPos = _GetCardInHand(MIDDLE,cards.start)->getPosition();
    
    for(int i=cards.start; i<=cards.last; i++) {
        auto loopCard=_GetCardInHand(MIDDLE,i);

        if(i==chosen) {
            continue;
        } else if(i<chosen) {
            loopCard->setPosition(Vec2(startPos.x + cardSize.width*(i-cards.start), y1));
            loopCard->setScale(1);
            loopCard->_ID = 1;
        } else if(i>chosen) {
            if( i==cards.last && cards.residual==2 ) {
                loopCard->setPosition(Vec2(startPos.x+cardSize.width*(i-cards.start)+30+14,y1));
            } else {
                loopCard->setPosition(Vec2(startPos.x+cardSize.width*(i-cards.start)+14,y1));
            }
            
            loopCard->setScale(1);
            loopCard->_ID = 1;
        }
    }
}

void NetRaceLayer::_UpdateCardsInHand(const CardsInfo_t &cards, int chosen) {
    if(cards.list->data[chosen].can_play==cps_YES) {/* I think the cards behind start all can play ???*/                 

        _UpdateNonChosenCards(cards,chosen);

        float x1 = _layout->_playerPosi[MIDDLE].basePoint.x+10;
        float y1 = _layout->_playerPosi[MIDDLE].basePoint.y+10;
        auto cardSize = _object->RectSize(FREE_CARD);
        auto startPos = _GetCardInHand(MIDDLE,cards.start)->getPosition();
        auto loopCard = _GetCardInHand(MIDDLE,chosen);

        _myChosenCard = chosen;

        if( chosen==cards.last && cards.residual==2 ) {/* logic changed !!!*/
            loopCard->setPosition(Vec2(startPos.x+cardSize.width*(chosen-cards.start)+30,y1+10));
        } else {
            loopCard->setPosition(Vec2(startPos.x+cardSize.width*(chosen-cards.start),   y1+10));
        }

        /* provide hint bar when ming choose */
        if(_roundManager->_actionToDo==a_MING && _roundManager->_isMingTime && (!_roundManager->IsTing(MIDDLE))) {
            if( loopCard->getScaleX()==1 ) {
                loopCard->setScale(1.2);
                loopCard->runAction(_voice->Speak("select"));

                while(myframe->getChildByTag(TING_SING_BAR))
                myframe->removeChildByTag(TING_SING_BAR);

                auto appearPoint=loopCard->getPosition();
                _TingHintCreate(appearPoint,chosen);
            } else if (!myframe->getChildByTag(TING_SING_BAR)) {
                auto appearPoint=loopCard->getPosition();
                _TingHintCreate(appearPoint,chosen);
            }
             
            ifChosed = true;
            _myTouchedCard  = chosen;
        } 
    }
}

Sprite *NetRaceLayer::_CreateEffectCard(int i,CARD_KIND kindId ) {
    auto card = _GetCardInHand(MIDDLE,i);
    auto pos  = card->getPosition();
    
    auto effectCard = _object->Create(FREE_CARD);
    effectCard->setAnchorPoint( Vec2(0,0) );
    effectCard->setScale( card->getScale() );
    effectCard->setPosition( Vec2(pos.x,pos.y) );
    
    auto kind = _object->CreateKind((Card_t)kindId,NORMAL);
    kind->setAnchorPoint(Vec2(0.5,0.5));
    kind->setPosition(Vec2(
        effectCard->getTextureRect().size.width/2,
        effectCard->getTextureRect().size.height*0.4));

    effectCard->addChild(kind,1);

    return effectCard;
}

Sprite* NetRaceLayer::_GetCardOnTingSignBar(PlayerDir_t dir,int cardIdx) {
    switch (dir) {
        case LEFT:
            return (Sprite*)myframe->getChildByTag(TING_SING_LEFTBAR)->getChildByTag(cardIdx+3);
        case MIDDLE:
            return (Sprite*)myframe->getChildByTag(TING_SING_BAR)->getChildByTag(cardIdx+3);
        case RIGHT:
            return (Sprite*)myframe->getChildByTag(TING_SING_RIGHTBAR)->getChildByTag(cardIdx+3);
    }
}

LabelAtlas * NetRaceLayer::_CreateNumberSign(int number) {
    char strNumber[10];
    sprintf(strNumber,"%d",number);

    LabelAtlas *residueNum = (number==0)
        ? LabelAtlas::create(strNumber,"fonts/NumMingNotice_grey.png",17, 20, '0')
        : LabelAtlas::create(strNumber,"fonts/NumMingNotice.png",17, 20, '0');

    residueNum->setAnchorPoint(Vec2(0.5,0.5));
    residueNum->setPosition(Vec2(78,24));

    return residueNum;
}


/***********************************************
        action button show
***********************************************/
void NetRaceLayer::ShowActionButtons() {
    _Show(myframe,TING_SING_BAR,false);

	float y=origin.y+visibleSize.height*0.25;
	float x=origin.x+visibleSize.width*0.85;

	if(_roundManager->_actionToDo!=a_JUMP) {
	    float width = _AddBtnQi(Vec2(x,y));
		x = x-width;
	}
    
	if(_roundManager->_actionToDo&a_HU) {	
        float width = _AddBtnHu(Vec2(x,y));
		x = x-width;
	}
    
	if(_roundManager->_actionToDo&a_MING) {
        float width = _AddBtnMing(Vec2(x,y));
		x = x-width;
	}
    
	if(_roundManager->_actionToDo&a_AN_GANG 
        || _roundManager->_actionToDo&a_MING_GANG
        ||_roundManager->_actionToDo&a_SHOU_GANG) {
        float width = _AddBtnGang(Vec2(x,y));
		x = x-width;
	}

	if(_roundManager->_actionToDo&a_PENG) {		
        float width = _AddBtnPeng(Vec2(x,y));
		x = x-width;
	}
}

float NetRaceLayer::_AddBtnQi(const Vec2 &ref) {
    auto picWidth = _object->_image("qi.png")->getContentSize().width;

    auto myact_qi = _object->CreateButton(MIDDLE,BTN_QI,Vec2(ref.x,ref.y));
    myact_qi->_ID = MIDDLE;
    myact_qi->addTouchEventListener(CC_CALLBACK_2(NetRaceLayer::BtnQiHandler,this));
    myframe->addChild(myact_qi,35,QI_REMIND_ACT_TAG_ID);
    
    auto qi_act = _object->CreateBtnBkg(BTN_QI,Vec2(ref.x,ref.y));
    myframe->addChild(qi_act,34,QI_REMIND_ACT_BKG_TAG_ID);

    return picWidth/2+36;
}

float NetRaceLayer::_AddBtnHu(const Vec2 &ref) {
    auto picWidth = _object->_image("hu1.png")->getContentSize().width;
    float x = ref.x - picWidth/2;
    
    auto myact_hu = _object->CreateButton(MIDDLE,BTN_HU,Vec2(x,ref.y+6));
    myact_hu->_ID=MIDDLE;
    myact_hu->addTouchEventListener(CC_CALLBACK_2(NetRaceLayer::BtnHuHandler,this));
    myframe->addChild(myact_hu,35,HU_REMIND_ACT_TAG_ID);
    
    auto hu1_act = _object->CreateBtnBkg(BTN_HU,Vec2(x,ref.y+6));
    myframe->addChild(hu1_act,34,HU_REMIND_ACT_BKG_TAG_ID);
    
    return (picWidth+18);
}

float NetRaceLayer::_AddBtnMing(const Vec2 &ref) {
    auto picWidth = _object->_image("ming.png")->getContentSize().width;
    float x = ref.x - picWidth/2;
    
    auto myact_ming=_object->CreateButton(MIDDLE,BTN_MING,Vec2(x,ref.y+11));
    myact_ming->addTouchEventListener(CC_CALLBACK_2(NetRaceLayer::BtnMingHandler,this));
    myframe->addChild(myact_ming,35,MING_REMIND_ACT_TAG_ID);
    
    auto ming_act=_object->CreateBtnBkg(BTN_MING,Vec2(x,ref.y+11));
    myframe->addChild(ming_act,34,MING_REMIND_ACT_BKG_TAG_ID);
    
    return (picWidth+18);
}

float NetRaceLayer::_AddBtnGang(const Vec2 &ref) {
    auto picWidth = _object->_image("gang1.png")->getContentSize().width;
    float x = ref.x - picWidth/2;
    
    auto myact_gang=_object->CreateButton(MIDDLE,BTN_GANG,Vec2(x,ref.y));
    myact_gang->addTouchEventListener(CC_CALLBACK_2(NetRaceLayer::BtnGangHandler,this));
    myframe->addChild(myact_gang,35,GANG_REMING_ACT_TAG_ID);
    
    auto gang1_act=_object->CreateBtnBkg(BTN_GANG,Vec2(x,ref.y));
    myframe->addChild(gang1_act,34,GANG_REMING_ACT_BKG_TAG_ID);
    
    return (picWidth+18);
}

float NetRaceLayer::_AddBtnPeng(const Vec2 &ref) {
    auto picWidth = _object->_image("peng1.png")->getContentSize().width;
    float x = ref.x - picWidth/2;
    
    auto myact_peng=_object->CreateButton(MIDDLE,BTN_PENG,Vec2(x,ref.y+13));
    myact_peng->addTouchEventListener(CC_CALLBACK_2(NetRaceLayer::BtnPengHandler,this));
    myframe->addChild(myact_peng,35,PENG_REMIND_ACT_TAG_ID);
    
    auto peng1_act=_object->CreateBtnBkg(BTN_PENG,Vec2(x,ref.y+13));    
    myframe->addChild(peng1_act,34,PENG_REMIND_ACT_BKG_TAG_ID);

    return (picWidth+18);
}

/***********************************************
        action buttons
***********************************************/
void NetRaceLayer::BtnPengHandler(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
{
	auto curButton=(Button*)pSender;
    
	switch (type) {
    	case cocos2d::ui::Widget::TouchEventType::BEGAN:
    		curButton->setScale(1.2);
    		break;
    	case cocos2d::ui::Widget::TouchEventType::MOVED:
    		break;
    	case cocos2d::ui::Widget::TouchEventType::ENDED:
            _roundManager->RecvPeng(MIDDLE);
    		break;
    	case cocos2d::ui::Widget::TouchEventType::CANCELED:
    		curButton->setScale(1);
    		break;
    	default:
    		break;
	}
}

void NetRaceLayer::BtnHuHandler(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
{
	_roundManager->_actionToDo = a_HU;
    
	auto curButton=(Button*)pSender;
    curButton->_ID = pSender->_ID;
	int no = pSender->_ID;

    switch(type) {
    	case cocos2d::ui::Widget::TouchEventType::BEGAN:
    		curButton->setScale(1.2);
    		break;
    	case cocos2d::ui::Widget::TouchEventType::MOVED:
    		break;
    	case cocos2d::ui::Widget::TouchEventType::ENDED:
            _roundManager->RecvHu(MIDDLE);
			break;
    	case cocos2d::ui::Widget::TouchEventType::CANCELED:
    		curButton->setScale(1);
    		break;
	}
}

void NetRaceLayer::BtnGangHandler(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
{
	auto curButton=(Button*)pSender;

	switch (type){
    	case cocos2d::ui::Widget::TouchEventType::BEGAN:
    		curButton->setScale(1.2);
    		break;
    	case cocos2d::ui::Widget::TouchEventType::MOVED:
    		break;
    	case cocos2d::ui::Widget::TouchEventType::ENDED:
            _roundManager->RecvGang(MIDDLE);
    		break;
    	case cocos2d::ui::Widget::TouchEventType::CANCELED:
    		curButton->setScale(1);
    		break;
    	default:
    		break;
	}
}

void NetRaceLayer::BtnQiHandler(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
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
        _roundManager->RecvQi();
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		curButton->setScale(1);
		break;
	default:
		break;
	}
}

void NetRaceLayer::ListenToTingButton() {
	auto TingListener = EventListenerTouchOneByOne::create();
	TingListener->setSwallowTouches(false);
    
	TingListener->onTouchBegan=[=](Touch* touch, Event* event) {
		return true;
	};
	TingListener->onTouchMoved=[=](Touch* touch, Event* event) {};
	TingListener->onTouchEnded=[=](Touch* touch, Event* event) {
		auto button = (Sprite*)myframe->getChildByTag(TING_SING_BUTTON);

        if( !_IsClickedOn(button,touch) ) {
            ifTingSignBarVisible=false;
		} else {
			ifTingSignBarVisible = !ifTingSignBarVisible;
		}

        _Show(myframe,TING_SING_BAR,ifTingSignBarVisible);
	};
    
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(TingListener,myframe);
}

/*********************************************
        choose card
*********************************************/
void NetRaceLayer::ListenToCardTouch() {
    if( !_roundManager->IsTing(MIDDLE) && !_roundManager->_isTuoGuan ) {
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        
        listener->onTouchBegan = CC_CALLBACK_2(NetRaceLayer::_CardTouchBegan,this);
        listener->onTouchMoved = CC_CALLBACK_2(NetRaceLayer::_CardTouchMove,this);
        listener->onTouchEnded = CC_CALLBACK_2(NetRaceLayer::_CardTouchEnd,this);

        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,myframe);
   }
}

/*****************************/
/* only used when select card */
void NetRaceLayer::_GetCardsInfo(CardsInfo_t *info) {
    info->list  = _roundManager->_players[MIDDLE]->get_parter()->get_card_list();
    info->start = info->list->atcvie_place;
    info->last   = info->list->len - 1;
    info->residual = (info->list->len - info->list->atcvie_place)%3;
}
/*****************************/
int NetRaceLayer::_FindCard(int start,int end,Touch *touch) {
    for(int i=start;i<=end;i++) {
        if( _IsClickedOn(_GetCardInHand(MIDDLE,i),touch) ) {
            return i;
        }
    }

    return INVALID;
}

bool NetRaceLayer::_CardTouchBegan(Touch* touch, Event* event) {
    if( ifInsertCardsTime ) {
        ifInsertCardsTime=false;
        
        _CardInHandUpdateEffect(MIDDLE);
        _isCardInHandUpdated=true;
    }
    
    if( ifEffectTime && ifUpdateDuringEffect ) {
        ifUpdateDuringEffect=false;
        _CardInHandUpdateEffect(MIDDLE);
    }

    ifChosed=false;
    
    if( touch->getLocation().y > visibleSize.height*0.173 ) {
        /* why is there TING_SING_BAR if non-ting??? from others??? */
        while(myframe->getChildByTag(TING_SING_BAR) && (!_roundManager->IsTing(1)))
            myframe->removeChildByTag(TING_SING_BAR);
        
        float x1,y1;
        x1 = _layout->_playerPosi[MIDDLE].basePoint.x+10;
        y1 = _layout->_playerPosi[MIDDLE].basePoint.y+10;

        CardsInfo_t cards = {0};
        _GetCardsInfo(&cards);
        
        const auto startPos = _GetCardInHand(MIDDLE,cards.start)->getPosition();
        const auto cardSize = _object->RectSize(FREE_CARD);

        for(int i=cards.start; i<=cards.last; i++) {
            auto card = _GetCardInHand(MIDDLE,i);
            card->_ID = 1;
            card->setScale(1);
            card->setAnchorPoint(Vec2(0,0));

            if(i==cards.last && cards.residual==2) {
                card->setPosition(Vec2(startPos.x + cardSize.width*(i-cards.start)+30,y1));
            } else {
                card->setPosition(Vec2(startPos.x + cardSize.width*(i-cards.start),y1));
            }
        }
        
        _myChosenCard = INVALID;
    }
    
    return true;
}

void NetRaceLayer::_CardTouchMove(Touch* touch, Event* event) {
    CardsInfo_t cards = {0};
    _GetCardsInfo(&cards);
    
	if(myframe->getChildByTag(CHOOSE_CARD_TAG_ID)!=NULL && touch->getLocation().y>visibleSize.height*0.173) {
		myframe->getChildByTag(CHOOSE_CARD_TAG_ID)->setPosition(touch->getLocation());
		_myChosenCard = _myTouchedCard;
		return;
	} 
    
    for(int i=cards.start; i<=cards.last; i++) {
        _GetCardInHand(MIDDLE,i)->setOpacity(255);
	}
    
	if(myframe->getChildByTag(CHOOSE_CARD_TAG_ID)!=NULL) {
		myframe->removeChildByTag(CHOOSE_CARD_TAG_ID);
    }
    
    int chosen = _FindCard(cards.start, cards.last, touch);
    if(chosen!=INVALID) {
        _UpdateCardsInHand(cards,chosen);
    } else if(touch->getLocation().y>visibleSize.height*0.173) {
		if(myframe->getChildByTag(CHOOSE_CARD_TAG_ID)==NULL && ifChosed==true) {
			int  kind = _roundManager->_players[MIDDLE]->get_parter()->get_card_list()->data[_myTouchedCard].kind;

			auto freeCard = _object->Create(FREE_CARD);
			auto card = _object->CreateKind((Card_t)kind,NORMAL);
            
			freeCard->addChild(card);
			card->setPosition(Vec2(
                freeCard->getTextureRect().size.width/2,
                freeCard->getTextureRect().size.height*0.4));
			freeCard->setScale(1.2);
			freeCard->setPosition(touch->getLocation());

			myframe->addChild(freeCard,35,CHOOSE_CARD_TAG_ID);
			_GetCardInHand(MIDDLE,_myTouchedCard)->setOpacity(150);

			while(myframe->getChildByTag(TING_SING_BAR) && (!_roundManager->IsTing(MIDDLE)))
				myframe->removeChildByTag(TING_SING_BAR,true);
            
			_myChosenCard = _myTouchedCard;
		}
	}
}

void NetRaceLayer::_CardTouchEnd(Touch* touch, Event* event) {
    CardsInfo_t cards = {0};
    _GetCardsInfo(&cards);
    
	if(myframe->getChildByTag(CHOOSE_CARD_TAG_ID)!=NULL 
        && (touch->getLocation().y>visibleSize.height*0.2)) {
		if(_roundManager->_isMyShowTime) {
			_roundManager->_isMyShowTime=false;
            
			_myChosenCard=INVALID;

            _roundManager->RecvHandout(_myTouchedCard,touch->getLocation(),1);
		} else {
			_myChosenCard=_myTouchedCard;

            _GetCardInHand(MIDDLE,_myTouchedCard)->setOpacity(255);
            _Remove(myframe,CHOOSE_CARD_TAG_ID);
		}
		return;
	}
    
    for(int i=cards.start; i<=cards.last; i++) {
		_GetCardInHand(MIDDLE,i)->setOpacity(255);
    }

    _Remove(myframe,CHOOSE_CARD_TAG_ID);
    
	int chosen = _FindCard(cards.start, cards.last, touch);
    if(chosen!=INVALID) {
        _UpdateNonChosenCards(cards,chosen);

		float x = _layout->_playerPosi[MIDDLE].basePoint.x+10;
		float y = _layout->_playerPosi[MIDDLE].basePoint.y+10;
		auto cardSize = _object->RectSize(FREE_CARD);
        auto startPos = _GetCardInHand(MIDDLE,cards.start)->getPosition();

        _myChosenCard = chosen;

        Sprite *loopCard = _GetCardInHand(MIDDLE,chosen);
		bool ChooseConfirm = false;

        if(chosen==cards.last) {
            if(loopCard->_ID!=100) {
                loopCard->_ID=100;
                
                if(cards.residual==1)
                    loopCard->setPosition(Vec2(startPos.x + cardSize.width*(chosen-cards.start),y+10));
                else if(cards.residual==2)
                    loopCard->setPosition(Vec2(startPos.x + cardSize.width*(chosen-cards.start)+30,y+10));
                
                if(loopCard->getScale()==1) {
                    loopCard->runAction(_voice->Speak("select"));
                }
                
                loopCard->setScale(1.2);
            } else {
                ChooseConfirm=true;
            }
        } else {
            if(loopCard->_ID!=100) {
                loopCard->_ID=100;
                loopCard->setPosition(Vec2(startPos.x+cardSize.width*(chosen-cards.start),y+10));
                if(loopCard->getScale()==1)
                    loopCard->runAction(_voice->Speak("select"));
                loopCard->setScale(1.2);
            } else {
                ChooseConfirm=true;
            }
        }

        if(chosen!=cards.last) {/* why??? */
            if(_roundManager->_actionToDo==a_MING && !_roundManager->IsTing(MIDDLE)
                && _roundManager->_isMingTime ) {
                while( myframe->getChildByTag(TING_SING_BAR) && (!_roundManager->IsTing(MIDDLE)) )
                    myframe->removeChildByTag(TING_SING_BAR);
                
                Point curPos=loopCard->getPosition();
                _TingHintCreate(curPos,chosen);
            }
        }
        
		if(ChooseConfirm && _roundManager->_isMyShowTime) {
			_roundManager->_isMyShowTime=false;
            
			_myChosenCard=INVALID;
			_myTouchedCard = chosen;

            _roundManager->RecvHandout(_myTouchedCard,touch->getLocation(),3);
		}
    }
}

/*************************************************************
        action effect
*************************************************************/
void NetRaceLayer::_PengEffect(PlayerDir_t dir, PlayerDir_t prevDir, Card_t card) {
	myframe->_ID = dir;
    
	const int riverLast =_roundManager->_players[prevDir]->get_parter()->getOutCardList()->length;
    _Remove(myframe,HAND_OUT_CARDS_TAG_ID+prevDir*25+riverLast);

	auto outCard = myframe->getChildByTag(OUT_CARD_FRAME_TAG_ID);
    auto hideOutcard = Spawn::create(CCCallFunc::create([=](){
			_Show(this,SHOWCARD_INDICATOR_TAG_ID,false);}),TargetedAction::create(
            outCard,Sequence::create(
                DelayTime::create(0.1),
                ScaleTo::create(0,0),NULL)),NULL);
		
	if(dir!=MIDDLE) {
		myframe->runAction(Sequence::create( 
                            Spawn::create(
                                _voice->SpeakAction(PENG,_roundManager->_cardHolders[dir]->GetSex()),
                                simple_tip_effect( _layout->PositionOfActSign(dir),"peng.png" ),NULL), 
                            hideOutcard, 
                            Sequence::create(CCCallFunc::create(this,callfunc_selector(
                                NetRaceLayer::_DeleteActionTip)),   CallFunc::create([=](){
                                _roundManager->UpdateCards(dir,a_PENG);}),    CCCallFunc::create([=]() {
                                _CardInHandUpdateEffect(dir);}), CCCallFunc::create([=](){
                    			_roundManager->_actionToDo = _roundManager->_players[dir]->get_parter()->ActiontodoCheckAgain();
                				_roundManager->WaitForOthersAction(dir);}),NULL),NULL));
	} else {
		if(myframe->getChildByTag(PENG_EFFECT_NODE_ID)) {
			myframe->removeChildByTag(PENG_EFFECT_NODE_ID,true);
			if(ifEffectTime) {
				ifEffectTime=false;
				ifUpdateDuringEffect=false;
                
				_roundManager->CancelEffectCard();
                
				_DeleteActionEffect();
				_CardInHandUpdateEffect(dir);
			}
		}
        
        _roundManager->SetEffectCard(card,c_PENG);
        
		ifEffectTime=true;
		ifUpdateDuringEffect=true;

        /****************
            hide reminder
        ****************/
        auto hideReminder = _HideReminder(PENG_REMIND_ACT_BKG_TAG_ID, 0.18, 1.2);
	
        /****************
            move 3 cards
        ****************/
        Sprite *showCards[3] = {0};
        for(int i=0;i<3;i++) {
            showCards[i] = _object->Create(PENG_CARD);
            auto kind = _object->CreateKind(card,MIDDLE_SIZE);
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
			if(card==list->data[i].kind) {
				if(i==0) {
					firstMatch = 0;
				} else if(card!=list->data[i-1].kind) {
					firstMatch = i;
				} else if(card==list->data[i-1].kind) {
					secondMatch = i;
					break;
				}
			}
		}

		int ifZeroPointTwo=0;
		if(_myChosenCard!=-1) {
			if(_myChosenCard > secondMatch) {
				_myChosenCard += 1;
            }
			else if(_myChosenCard== secondMatch || _myChosenCard==firstMatch) {
				ifZeroPointTwo = 1;
				_myChosenCard = -1;
			}
			else if(_myChosenCard<firstMatch)
				_myChosenCard += 3;
		}

		auto LeftPengCard = _CreateEffectCard(firstMatch, (CARD_KIND)card);
		myframe->addChild(LeftPengCard,20,EFFET_NEWCATD1_TAG);
		auto moveLeftCardInHand = TargetedAction::create(LeftPengCard,Sequence::create(
            DelayTime::create(0.18),
            ScaleTo::create(0,0.6),
            MoveTo::create(0.18,Vec2(
                origin.x+visibleSize.width*0.45,
                origin.y+visibleSize.height*0.26)),
			DelayTime::create(0.12),
			ScaleTo::create(0,0),NULL));

		auto RightPengCard = _CreateEffectCard(secondMatch, (CARD_KIND)card);
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
		auto cardPengSize = _object->RectSize(PENG_CARD);
        
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

        auto EmptyCard=_object->Create(FREE_CARD);
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


		auto PengEffectNode = Node::create();
		PengEffectNode->_ID = dir;
		myframe->addChild(PengEffectNode,1,PENG_EFFECT_NODE_ID);
        
		auto callFunc_update_list = CCCallFunc::create([=](){
			if(ifEffectTime) {

                ifEffectTime=false;
                
				if(ifUpdateDuringEffect) {
					ifUpdateDuringEffect=false;
					_roundManager->CancelEffectCard();
					_CardInHandUpdateEffect(dir);
				} else  {
                    int sameCardNum = 0;

                    for(int a=list->atcvie_place-1;a>=0;a--) {
						if( _roundManager->IsCurEffectCard(list->data[a]) ){
							sameCardNum++;
							((Sprite*)myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+1*20+a))->setVisible(true);
                            
							if(sameCardNum==3){
								_roundManager->CancelEffectCard();
								break;
							}
						}
					}
				}
			}
		});

		PengEffectNode->runAction(Sequence::create(
            hideReminder,Spawn::create(
                simple_tip_effect(_layout->PositionOfActSign(dir),"peng.png" ),
                _voice->SpeakAction(PENG,
                    _roundManager->_cardHolders[dir]->GetSex()),Spawn::create(
                hide2CardsInhand),Spawn::create(
                moveRightCardInHand,
                moveLeftCardInHand,NULL),Sequence::create(
                    DelayTime::create(0.42),
                    hideOutcard,NULL),Spawn::create(
                backGoundEffect,
                move3PengCards,NULL),Sequence::create(
                DelayTime::create(0.84),
                _voice->Speak("paizhuangji"),NULL),NULL),Sequence::create(CCCallFunc::create(this,callfunc_selector(
            NetRaceLayer::_DeleteActionEffect)),
            callFunc_update_list,CallFunc::create([=](){
			if(myframe->getChildByTag(PENG_EFFECT_NODE_ID))
				myframe->removeChildByTag(PENG_EFFECT_NODE_ID,true);}),/*actcheckagain,*/NULL),NULL));
        
        /***************************************
            it is unneccessary for net-game???
        ***************************************/
		myframe->runAction( Sequence::create(CallFunc::create([=](){
                _roundManager->UpdateCards(dir,a_PENG);}),CCCallFunc::create([=](){
    			_roundManager->_actionToDo = _roundManager->_players[dir]->get_parter()->ActiontodoCheckAgain();
    			_roundManager->WaitForMyAction();}),CallFunc::create([=](){
                _roundManager->_isMyShowTime=true;}),NULL));
	}
}

void NetRaceLayer::_AnGangEffect(PlayerDir_t dir,Card_t card,int gang[])
{
	int GangCardsPlace[4]={gang[0],gang[1],gang[2],gang[3]};
	delete gang;

    if(dir!=MIDDLE) {
		myframe->runAction(Sequence::create(
            Spawn::create(
                _voice->SpeakAction(GANG,_roundManager->_cardHolders[dir]->GetSex()),
                simple_tip_effect(_layout->PositionOfActSign(dir),"gang.png"),NULL), CallFunc::create([=](){
			GoldNumInsert(dir,1,dir);}), Sequence::create(CCCallFunc::create(this,callfunc_selector(
            NetRaceLayer::_DeleteActionTip)), CallFunc::create([=](){
            _roundManager->UpdateCards(dir,a_AN_GANG);}), CCCallFunc::create([=]() {
            _CardInHandUpdateEffect(dir);}),
            _voice->Speak("down"),CallFunc::create([=](){
            _roundManager->DistributeTo(dir);}),NULL),NULL));
	}
	else {
		for(int NodeNum=0;NodeNum<3;NodeNum++) {
            _Remove(myframe,PENG_EFFECT_NODE_ID+NodeNum);
            
			if(ifEffectTime) {
				ifEffectTime=false;
				ifUpdateDuringEffect=false;
                _roundManager->CancelEffectCard();
				_DeleteActionEffect();
				_CardInHandUpdateEffect(dir);
			}
		}

		ifEffectTime=true;
		ifUpdateDuringEffect=true;
        /**********************
            clear screen
        **********************/
        auto hideReminder = _HideReminder(GANG_REMING_ACT_BKG_TAG_ID, 0.18, 1.3);

        /**********************
            logical
        **********************/
		auto list = _roundManager->_players[dir]->get_parter()->get_card_list();
        Card outCard;

		int ifZeroPointTwo=0;
		int ifLeft=0;
		if(_myChosenCard!=-1) {
			if(_myChosenCard<list->len-5)
				ifLeft=1;
			if(_myChosenCard==GangCardsPlace[3]
                ||_myChosenCard==GangCardsPlace[2]
                ||_myChosenCard==GangCardsPlace[1]
                ||_myChosenCard==GangCardsPlace[0]) {
				if(_myChosenCard!=(list->len-1))
					ifZeroPointTwo=1;
				_myChosenCard=-1;
			} else if(_myChosenCard<GangCardsPlace[3]
			            &&_myChosenCard>GangCardsPlace[2])
				_myChosenCard+=1;
			else if(_myChosenCard<GangCardsPlace[0])
				_myChosenCard+=4;
		}

        /**********************
            move 4 cards
        **********************/
        TargetedAction *moveGangCardEffect[4];
        _CreateGangCardsMotion(moveGangCardEffect);

        /**********************
            move 4 cards in hand
        **********************/
        Size FreeCardSize = _object->RectSize(FREE_CARD);
		auto curKindOfGang = list->data[GangCardsPlace[0]].kind;

        TargetedAction *moveGangCardInHand[4];
        _CreateGangCardInHandMotion(moveGangCardInHand, GangCardsPlace, curKindOfGang);

		auto moveAngangCards = Spawn::create(
            moveGangCardEffect[0],moveGangCardEffect[1],moveGangCardEffect[2],moveGangCardEffect[3],
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
        auto GangCardSize = _object->RectSize(AN_GANG_CARD);

        const float GAP = (list->atcvie_place==0)?(0.5):(0.0);
        
		Vector<FiniteTimeAction *>gang_list_seq;
		Spawn* moveFreeCards;
        
		if( card==list->data[list->len-1].kind ) {
            int i;
            for(i=actionStartPlace;i<list->len-1;i++)
			{
                auto curPos = _GetCardInHand(dir,i)->getPosition();
                TargetedAction *motion;
                
				if(i<GangCardsPlace[0]&&(list->data[i].status==c_MING_KOU||list->data[i].status==c_FREE))
				{
					motion=TargetedAction::create( _GetCardInHand(dir,i),
                        Sequence::create(
                            DelayTime::create(0.18),
                            MoveTo::create(0.3,Vec2(
                                curPos.x+GangCardSize.width*(3.5+GAP),
                                curPos.y)),NULL));
				}
				else if(i==GangCardsPlace[0]||i==GangCardsPlace[1]||i==GangCardsPlace[2])
				{
					motion=TargetedAction::create(myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+dir*20+i),
                        Sequence::create(
                            ScaleTo::create(0,0),NULL));
				}
				else if(i>GangCardsPlace[2])
				{
					motion = TargetedAction::create(myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+dir*20+i),
                        Sequence::create(
                            DelayTime::create(0.18),
                            MoveTo::create(0.3,Vec2(
                                curPos.x+(GangCardSize.width*(3.5+GAP)-FreeCardSize.width*(3+ifZeroPointTwo*0.2)),
                                curPos.y)),NULL));
				}

                gang_list_seq.insert(i-actionStartPlace,motion);
			}
            
			TargetedAction *motion = TargetedAction::create( myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+dir*20+list->len-1),
                Sequence::create(
                    DelayTime::create(0.18),
                    ScaleTo::create(0,0),NULL));
            
            gang_list_seq.insert(i-actionStartPlace,motion);
		} else {
			for(int i=actionStartPlace;i<=list->len-1;i++)
			{
                auto curPos=myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+dir*20+i)->getPosition();
                TargetedAction *motion;
                
				if(i<GangCardsPlace[0]&&(list->data[i].status==c_MING_KOU||list->data[i].status==c_FREE)) {
					motion = TargetedAction::create(myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+dir*20+i),
                        Sequence::create(
                            DelayTime::create(0.18),
                            MoveTo::create(0.3,Vec2(
                                curPos.x + GangCardSize.width*(3.5+GAP),
                                curPos.y)),NULL));
				} else if(i==GangCardsPlace[0]||i==GangCardsPlace[1]||i==GangCardsPlace[2]) {
					motion = TargetedAction::create(myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+dir*20+i),
                        Sequence::create(
                            ScaleTo::create(0,0),NULL));
				} else if(i==GangCardsPlace[3]) {
					motion = TargetedAction::create(myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+dir*20+i),
                        Sequence::create(
                            ScaleTo::create(0,0),NULL));
                } else if(i>GangCardsPlace[3]) {
                    MoveTo* actionMove;
                    if( (i==list->len-1) && (card!=list->data[list->len-1].kind)) {
						auto curLeftPos = myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+dir*20+i-1)->getPosition();
						int instanceBetween = curPos.x-curLeftPos.x-FreeCardSize.width;
                        
						actionMove = MoveTo::create(0.3,Vec2(
                            curPos.x+(GangCardSize.width*(3.5+GAP)-FreeCardSize.width*(4+ifZeroPointTwo*0.2))-instanceBetween,
                            curPos.y));
					}
					else
						actionMove = MoveTo::create(0.3,Vec2(
						    curPos.x+(GangCardSize.width*(3.5+GAP)-FreeCardSize.width*(4+ifZeroPointTwo*0.2)),
						    curPos.y));
                    
					motion = TargetedAction::create(myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+dir*20+i),
                        Sequence::create(
                            DelayTime::create(0.18),
                            actionMove,NULL));
				}

                gang_list_seq.insert(i-actionStartPlace,motion);
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
            
			if( GangCardsPlace[0]<list->atcvie_place ) {/* peng cards transfer to gang cards */
				if(i==GangCardsPlace[3])
					EveryCard->setTag(EFFECT_TEMP_CARD_TAG_FOUR);
				else if(i>GangCardsPlace[2] && i<GangCardsPlace[3]) {
                    EveryCard->setTag(curTag+1);
                    
                    if(i==GangCardsPlace[2]+1) {       /* insert 1 addition gang card*/
                        ((Sprite*)myframe->getChildByTag(EFFECT_TEMP_CARD_TAG_FOUR))->setTag(HAND_IN_CARDS_TAG_ID+1*20+GangCardsPlace[2]+1);
                        break;
                    }
                }
			} else {/* gang directly */
				if(i>GangCardsPlace[2] && i<GangCardsPlace[3]) {
					EveryCard->setTag(curTag+1);      /*right shift*/
                } else if(i==GangCardsPlace[3]||i==GangCardsPlace[2]||i==GangCardsPlace[1]||i==GangCardsPlace[0]) {
					if(i==GangCardsPlace[3])
						EveryCard->setTag(EFFECT_TEMP_CARD_TAG_FOUR);
					if(i==GangCardsPlace[2])
						EveryCard->setTag(EFFECT_TEMP_CARD_TAG_THREE);
					if(i==GangCardsPlace[1])
						EveryCard->setTag(EFFECT_TEMP_CARD_TAG_TWO);
					if(i==GangCardsPlace[0])
						EveryCard->setTag(EFFECT_TEMP_CARD_TAG_ONE);
				} else if(i>=list->atcvie_place && i<GangCardsPlace[0]) {
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
        TargetedAction *bgElementsMotions[5];
        _CreateBackgroundElementMotion(bgElementsMotions,0);

        /* final effect */
		auto callFunc_update_list=CCCallFunc::create([=](){
			if(ifEffectTime) {
				ifEffectTime=false;
				if(ifUpdateDuringEffect) {
					ifUpdateDuringEffect=false;
					_roundManager->CancelEffectCard();
					_CardInHandUpdateEffect(dir);
				} else {
					int sameCardNum=0;
					for(int a=list->atcvie_place-1;a>=0;a--)
					{
						if( _roundManager->IsCurEffectCard(list->data[a]) )
						{
							sameCardNum++;
							((Sprite*)myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+1*20+a))->setVisible(true);
							if(sameCardNum==4)
							{
								_roundManager->CancelEffectCard();
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
                    simple_tip_effect(_layout->PositionOfActSign(dir),"gang.png"),
                    _voice->SpeakAction(GANG,
                        _roundManager->_cardHolders[dir]->GetSex()),
                    moveFreeCards,
                    Spawn::create(
            		    moveAngangCards,
            		    Sequence::create(
            		        DelayTime::create(0.66),CCCallFunc::create([=]() {
                    		_StartParticleSystem(0.3);
                    		}),NULL),
                        bgElementsMotions[0],
                        bgElementsMotions[1],
                        bgElementsMotions[2],
                        bgElementsMotions[3],
                        bgElementsMotions[4],NULL),
            		Sequence::create(
                        DelayTime::create(0.66),CallFunc::create([=](){
                        _voice->Speak("paizhuangji");}),NULL),NULL),
                Sequence::create(CCCallFunc::create(this,callfunc_selector(
                    NetRaceLayer::_DeleteActionEffect)),
                    callFunc_update_list,
                    _voice->Speak("lanpai"),NULL),CallFunc::create([=](){
                _Remove(myframe,AN_GANG_EFFECT_NODE);}),NULL));
        
		myframe->_ID=MIDDLE;
		myframe->runAction(Sequence::create(CallFunc::create([=](){
            _roundManager->UpdateCards(MIDDLE,a_AN_GANG);}),
            DelayTime::create(0.48), CallFunc::create([=](){
			GoldNumInsert(dir,1,dir);}),CallFunc::create([=](){
            _roundManager->DistributeTo(dir);}),NULL));
	}
}

void NetRaceLayer::_MingGangEffect(PlayerDir_t dir,PlayerDir_t prevDir, Card_t card,int gang[])
{
    myframe->_ID = dir;

	auto list = _roundManager->_players[dir]->get_parter()->get_card_list();

	if(_roundManager->_isCardFromOthers) {
        int riverLast = _roundManager->_players[prevDir]->get_parter()->getOutCardList()->length;
        _Remove(myframe,HAND_OUT_CARDS_TAG_ID+prevDir*25+riverLast);
	}
    
    if(dir!=MIDDLE) {
        _OthersMingGangEffect(dir,prevDir,_roundManager->_isCardFromOthers);
	} else {
		for(int node=0;node<3;node++) {
		    _Remove(myframe,PENG_EFFECT_NODE_ID+node);

			if(ifEffectTime) {
				ifEffectTime=false;
				ifUpdateDuringEffect=false;
                
                _roundManager->CancelEffectCard();
				_DeleteActionEffect();
				_CardInHandUpdateEffect((PlayerDir_t)dir);
			}
		}

        _roundManager->SetEffectCard(card,c_MING_GANG);
		ifEffectTime=true;
		ifUpdateDuringEffect=true;

        /**********************
            clear screen
        **********************/
        auto hideReminder = _HideReminder(GANG_REMING_ACT_BKG_TAG_ID, 0.18, 1.3);

        /**********************
            hide out card
        **********************/
		Size FreeCardSize = _object->RectSize(FREE_CARD);

		Sprite   *s_curOutCard;
		Sequence *hideOutcard;

		if( !_roundManager->_isCardFromOthers ) {
			auto lastInHand = (Sprite*)myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+dir*20 + list->len-1);//gang1
			lastInHand->runAction(ScaleTo::create(0,0));
			
			s_curOutCard=_object->Create(FREE_CARD);
			s_curOutCard->setAnchorPoint(Vec2(0,0));
			s_curOutCard->setPosition(Vec2(
                lastInHand->getPosition().x,
                lastInHand->getPosition().y));
			auto kind = _object->CreateKind((Card_t)card,NORMAL);
			kind->setAnchorPoint(Vec2(0.5,0.5));
			kind->setPosition(Vec2(
                s_curOutCard->getTextureRect().size.width/2,
                s_curOutCard->getTextureRect().size.height*0.4));
			s_curOutCard->addChild(kind,1);
            
			myframe->addChild(s_curOutCard,29,EFFET_NEWCATD1_TAG);
			lastInHand->runAction(ScaleTo::create(0,0));
            
			if(s_curOutCard) {
				hideOutcard = Sequence::create(DelayTime::create(0.06),NULL);
			}
		}
		else {
			s_curOutCard = (Sprite*)myframe->getChildByTag(OUT_CARD_FRAME_TAG_ID);
            
			hideOutcard = Sequence::create(Spawn::create(CCCallFunc::create([=]() {
			    _Show(this,SHOWCARD_INDICATOR_TAG_ID,false);}),TargetedAction::create(
                s_curOutCard,Sequence::create(
                    DelayTime::create(0.06),
                    ScaleTo::create(0,0),NULL)),NULL),NULL);
		}
        
        /**********************
            
        **********************/
        int ifZeroPointTwo=0;
        int ifLeft=0;
        if(_myChosenCard!=-1) {
            if(_myChosenCard<list->len-4)
                ifLeft=1;
            
            if(!_roundManager->_isCardFromOthers) {
                if(_myChosenCard>gang[2])
                    _myChosenCard+=1;
            } else {
                if(_myChosenCard>gang[2])
                    _myChosenCard+=1;
                else if(_myChosenCard==gang[0]||_myChosenCard==gang[1]||_myChosenCard==gang[2]) {
                    ifZeroPointTwo=1;
                    _myChosenCard=-1;
                }
                else if(_myChosenCard<gang[0])
                    _myChosenCard+=4;
            }
        }
            
		auto Pengsize = _object->RectSize(PENG_CARD);
        
		Sprite* baseCard;
		Vec2    basePos;
        
		if(!_roundManager->_isCardFromOthers) {
			baseCard = _GetCardInHand(MIDDLE,list->len-2);
			basePos  = baseCard->getPosition();
            
			distributeCardPos=Vec2(
                basePos.x + baseCard->getBoundingBox().size.width + 30 - FreeCardSize.width*ifZeroPointTwo*0.2,
                basePos.y);
		} else {
			baseCard = _GetCardInHand(MIDDLE,list->len-4);
			basePos  = baseCard->getPosition();

            if( list->atcvie_place>0 )
				distributeCardPos = Vec2(
				    basePos.x + Pengsize.width*3.5 + 30 
				        + baseCard->getBoundingBox().size.width - FreeCardSize.width*ifZeroPointTwo*0.2*ifLeft,
				    basePos.y);
			else
				distributeCardPos = Vec2(
				    basePos.x + Pengsize.width*4 + 30 
				        + baseCard->getBoundingBox().size.width - FreeCardSize.width*ifZeroPointTwo*0.2*ifLeft,
				    basePos.y);
		}

        /**********************
            gang cards motions
        **********************/
        TargetedAction *gangCardsMotion[4];
        Sequence *gangCard0Motion;

		auto show_card = _object->Create(PENG_CARD);
		auto cardPengSize = show_card->getTextureRect().size;

        _AttachKindTexture(show_card,(CARD_KIND)card);
        
		show_card->setAnchorPoint(Vec2(0,0));
		if(!_roundManager->_isCardFromOthers) {
			show_card->setPosition(_layout->OrigPositionOfGangCard(0,cardPengSize));
		} else {
			show_card->setPosition(s_curOutCard->getPosition());
		}
		show_card->setScale(0);
		myframe->addChild(show_card,29,SINGLE_ACTION_EFFECT_TAG_ID);

		if(_roundManager->_isCardFromOthers) {
			gangCardsMotion[0] = TargetedAction::create(show_card,Sequence::create(
                DelayTime::create(0.42),
                ScaleTo::create(0,1),
                MoveTo::create(0.12,Vec2(
                    origin.x+visibleSize.width*0.5+cardPengSize.width*0.98,
                    origin.y+visibleSize.height*0.255)),
				MoveTo::create(0.12,Vec2(
				    origin.x+visibleSize.width*0.5+cardPengSize.width*0.98+150,
				    origin.y+visibleSize.height*0.255)),
				MoveTo::create(0.12,Vec2(
				    origin.x+visibleSize.width*0.5+cardPengSize.width*0.98,
				    origin.y+visibleSize.height*0.255)),NULL)),NULL;
            gangCard0Motion = Sequence::create(gangCardsMotion[0],NULL);
		} else {
			gangCardsMotion[0] = TargetedAction::create(s_curOutCard,Sequence::create(
                    DelayTime::create(0.42),
                    ScaleTo::create(0,0.6),
                    MoveTo::create(0.12,_layout->OrigPositionOfGangCard(0,cardPengSize)),
                    ScaleTo::create(0,0),NULL));
            gangCard0Motion = Sequence::create(
                gangCardsMotion[0],
                TargetedAction::create(show_card,Sequence::create(
                    ScaleTo::create(0,1),
                    MoveTo::create(0.12,_layout->MiddlePositionOfGangCard(0,cardPengSize)),
    				MoveTo::create(0.12,_layout->DestPositionOfGangCard(0,cardPengSize)),NULL)),NULL);
		}

        _CreateMingGangCardsMotion(gangCardsMotion,(CARD_KIND)card);

        /**********************
            move cards in hand
        **********************/
		Sequence*Gang2Card_seq;
		Sequence* Gang3Card_seq;
		Sequence* Gang4Card_seq;
        float delayTime=0.18;

        TargetedAction *gangCardInHandMotion[3];
        Sprite *gangCard[3];

        if(!_roundManager->_isCardFromOthers) {
            _CreateMingGangCardInHandMotion(gangCardInHandMotion,gang,(CARD_KIND)card);
		} else {
		    _CreateFreeCard(gangCard,gang,(CARD_KIND)card);

            if(list->atcvie_place>=0)/* active_place can less than 0 ???? */
			{
                auto OldGang2Pos = gangCard[0]->getPosition();
                auto OldGang3Pos = gangCard[1]->getPosition();
                auto OldGang4Pos = gangCard[2]->getPosition();
                
				gangCardInHandMotion[0]=TargetedAction::create(gangCard[0],Sequence::create(
                    DelayTime::create(delayTime),
                    ScaleTo::create(0,0.6),MoveTo::create(0.18,Vec2(
        				origin.x+visibleSize.width*0.5-gangCard[0]->getTextureRect().size.width*0.6*1.96,
        				origin.y+visibleSize.height*0.26)),
                    DelayTime::create(0.06),
                    ScaleTo::create(0,0),NULL));

				gangCardInHandMotion[1]=TargetedAction::create(gangCard[1],Sequence::create(
                    DelayTime::create(delayTime),Spawn::create(
                        ScaleTo::create(0,0.6),
                        MoveTo::create(0,Vec2(OldGang3Pos.x-FreeCardSize.width*0.4,OldGang3Pos.y)),NULL),
                    MoveTo::create(0.18,Vec2(
        				origin.x+visibleSize.width*0.5-gangCard[1]->getTextureRect().size.width*0.6*0.98,
        				origin.y+visibleSize.height*0.26)),
                    DelayTime::create(0.06),
                    ScaleTo::create(0,0),NULL));

				gangCardInHandMotion[2]=TargetedAction::create(gangCard[2],Sequence::create(
                    DelayTime::create(delayTime),Spawn::create(
                        ScaleTo::create(0,0.6),
                        MoveTo::create(0,Vec2(OldGang4Pos.x-FreeCardSize.width*0.8,OldGang4Pos.y)),NULL),
                    MoveTo::create(0.18,Vec2(
        				origin.x+visibleSize.width*0.5,
        				origin.y+visibleSize.height*0.26)),
                    DelayTime::create(0.06),
                    ScaleTo::create(0,0),NULL));
			}
		}

        /**********************
            move free cards in hand
        **********************/
		int actionStartPlace=0;
		for(int a=0;a<=list->atcvie_place;a++)
			if(list->data[a].status==c_MING_KOU||list->data[a].status==c_FREE)
			{
				actionStartPlace=a;
				break;
			}
		actionStartPlace = (actionStartPlace>gang[0])?gang[0]:actionStartPlace;
            
		Vector<FiniteTimeAction *>gang_list_seq;
        
		if(!_roundManager->_isCardFromOthers) {
			for(int i=actionStartPlace; i<=gang[0]; i++) {/* right shift */
				auto curPos=myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+dir*20+i)->getPosition();

				auto action=TargetedAction::create(myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+dir*20+i),
                    Sequence::create(
                    DelayTime::create(delayTime),
                    MoveTo::create(0.3,Vec2(
                        curPos.x+cardPengSize.width*3.5,
                        curPos.y)),NULL));
				gang_list_seq.insert(i-actionStartPlace,action);
             }
			for(int i=gang[0]; i<=gang[2]; i++) {/*hide gang cards*/
				gang_list_seq.insert(i-actionStartPlace,TargetedAction::create(
                    myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+dir*20+i),Sequence::create(
                        ScaleTo::create(0,0),NULL)));
			}
		}
		else {
            const float GAP = (list->atcvie_place==0)?0.5:0.0;
            
			for(int i=actionStartPlace;i<gang[0];i++) {/* right shift */
                auto curPos=myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+dir*20+i)->getPosition();

                gang_list_seq.insert(i-actionStartPlace,TargetedAction::create(
                    myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+dir*20+i),Sequence::create(
                        DelayTime::create(delayTime),
                        MoveTo::create(0.3,Vec2(
                            curPos.x+cardPengSize.width*(3.5+GAP),
                            curPos.y)),NULL)));
			}
            for(int i=gang[0];i<=gang[2];i++) {/* hide */
				gang_list_seq.insert(i-actionStartPlace,TargetedAction::create(
                    myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+dir*20+i),Sequence::create(
                        ScaleTo::create(0,0),NULL)));
			} 
            for(int i=gang[2]+1;i<list->len;i++) {/* right shift */
				auto curPos=myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+dir*20+i)->getPosition();
                
                gang_list_seq.insert(i-actionStartPlace,TargetedAction::create(
                    myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+dir*20+i),Sequence::create(
                        DelayTime::create(delayTime),
                        MoveTo::create(0.3,Vec2(
                            curPos.x+cardPengSize.width*(3.5+GAP)-FreeCardSize.width*(3+ifZeroPointTwo*0.2),
                            curPos.y)),NULL)));
			}
		}

        Spawn *moveFreeCards = Spawn::create(gang_list_seq);
        /**********************
            update tag
        **********************/
		for(int a=list->len-1;a>=0;a--) {
			int curTag=HAND_IN_CARDS_TAG_ID+1*20+a;
			if(!myframe->getChildByTag(curTag))
				continue;
            
			auto EveryCard=(Sprite*)myframe->getChildByTag(curTag);
			if(!_roundManager->_isCardFromOthers) {
				if(a==list->len-1)
					EveryCard->setTag(EFFECT_TEMP_CARD_TAG_FOUR);
				else if(a<list->len-1&&a>gang[2])
					EveryCard->setTag(curTag+1);
				if(a==(gang[2]+1)) {
					((Sprite*)myframe->getChildByTag(EFFECT_TEMP_CARD_TAG_FOUR))->setTag(HAND_IN_CARDS_TAG_ID+1*20+gang[2]+1);
				}
			} else {
				if(gang[0]<list->atcvie_place) {
					if(a>gang[2])
						EveryCard->setTag(curTag+1);
                    
					if(a==gang[2]+1) {
						auto EmptyCard=_object->Create(FREE_CARD);
						EmptyCard->setAnchorPoint(Vec2(1,1));
						EmptyCard->setScale(0);
						EmptyCard->setPosition(Vec2::ZERO);
						myframe->addChild(EmptyCard,1,HAND_IN_CARDS_TAG_ID+1*20+gang[2]+1);
					}
				} else {
					if(a>gang[2])
						EveryCard->setTag(curTag+1);
					else if(a<=gang[2]&&a>=gang[0]) {
						if(a==gang[2])
							EveryCard->setTag(EFFECT_TEMP_CARD_TAG_THREE);
						if(a==gang[1])
							EveryCard->setTag(EFFECT_TEMP_CARD_TAG_TWO);
						if(a==gang[0])
							EveryCard->setTag(EFFECT_TEMP_CARD_TAG_ONE);
					} else if(a<gang[0]&&a>=list->atcvie_place)
						EveryCard->setTag(curTag+4);
                    
					if(a==list->atcvie_place)
					{
						for(int b=0;b<3;b++)
							((Sprite*)myframe->getChildByTag(EFFECT_TEMP_CARD_TAG_ONE+b))->setTag(HAND_IN_CARDS_TAG_ID+1*20+list->atcvie_place+b);

						auto EmptyCard=_object->Create(FREE_CARD);
						EmptyCard->setAnchorPoint(Vec2(1,1));
						EmptyCard->setScale(0);
						EmptyCard->setPosition(Vec2::ZERO);
						myframe->addChild(EmptyCard,1,HAND_IN_CARDS_TAG_ID+1*20+list->atcvie_place+3);
					}
				}
			}
		}

        /**********************
            background effect
        **********************/
        TargetedAction *bgElementsMotions[5];
        _CreateBackgroundElementMotion(bgElementsMotions, 1);

        /* final effect */
		auto callFunc_update_list = CCCallFunc::create([=](){
			if(ifEffectTime)
			{
				ifEffectTime=false;
				if(ifUpdateDuringEffect)
				{
					ifUpdateDuringEffect=false;
                    _roundManager->CancelEffectCard();
					_CardInHandUpdateEffect((PlayerDir_t)dir);
				}
				else 
				{
					int sameCardNum=0;
					for(int a=list->atcvie_place-1;a>=0;a--)
					{
						if( _roundManager->IsCurEffectCard(list->data[a]) )
						{
							sameCardNum++;
							((Sprite*)myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+1*20+a))->setVisible(true);
							if(sameCardNum==4)
							{
								_roundManager->CancelEffectCard();
								break;
							}
						}
					}
				}
			}
		});

		auto MingGangEffectNode = Node::create();
		myframe->addChild(MingGangEffectNode,1,MING_GANG_EFFECT_NODE);

		MingGangEffectNode->_ID = dir;
		MingGangEffectNode->runAction(
            Sequence::create(
                hideReminder,
            Spawn::create(
                simple_tip_effect(_layout->PositionOfActSign(dir),"gang.png"),
                _voice->SpeakAction(GANG,_roundManager->_cardHolders[dir]->GetSex()),
                hideOutcard,
                moveFreeCards,
                Spawn::create(
                    Spawn::create(
                        gangCard0Motion,gangCardsMotion[1],gangCardsMotion[2],gangCardsMotion[3],
                        gangCardInHandMotion[0],gangCardInHandMotion[1],gangCardInHandMotion[2],NULL),
                    Sequence::create(
                        DelayTime::create(0.78), CCCallFunc::create([=]() {
            			_StartParticleSystem(0.3); }),NULL),
                    bgElementsMotions[0],
                    bgElementsMotions[1],
                    bgElementsMotions[2],
                    bgElementsMotions[3],
                    bgElementsMotions[4],NULL),
                Sequence::create(
                    DelayTime::create(0.78),CallFunc::create([=](){
                    _voice->Speak("paizhuangji");}),NULL),NULL),
            Sequence::create(CCCallFunc::create(this,callfunc_selector(
                NetRaceLayer::_DeleteActionEffect)),
                callFunc_update_list,CallFunc::create([=](){
                _Remove(myframe,MING_GANG_EFFECT_NODE);}),NULL),NULL));

		myframe->runAction(Sequence::create(CCCallFunc::create([=]() {
            _roundManager->UpdateCards(dir,a_MING_GANG);}),
            DelayTime::create(0.48),CallFunc::create([=](){
			if(_roundManager->_isCardFromOthers)
				GoldNumInsert(dir,2,prevDir);}),CallFunc::create([=](){
            _roundManager->ActionAfterGang(dir);}),NULL));
	}
}

/* why use different mechanism for single hu and double hu ??? */
void NetRaceLayer::_HuEffect(const WinInfo_t &win)
{
    LOGGER_WRITE("%s(%d-%d)",__FUNCTION__,win.winner,win.giver);

	((Button*)this->getChildByTag(MENU_BKG_TAG_ID)->getChildByTag(TUOGUAN_MENU_BUTTON))->setTouchEnabled(false);

    _Remove(this,ROBOT_TUO_GUAN);
    _Remove(this,TUOGUAN_CANCEL_BUTTON);
    
	scheduleOnce(schedule_selector(NetRaceLayer::raceAccount),3);

    PlayerDir_t winner = win.winner;
    PlayerDir_t giver  = win.giver;

	if(win.kind!=DOUBLE_WIN) {
        if(winner!=MIDDLE) {
			myframe->runAction(Sequence::create(
                Spawn::create(
                    _voice->SpeakAction(HU,_roundManager->_cardHolders[winner]->GetSex()),
                    simple_tip_effect(_layout->PositionOfActSign(winner),"dahu.png"),NULL),CallFunc::create([=](){
				GoldNumInsert(winner,3,giver);}),CallFunc::create(this,callfunc_selector(
                NetRaceLayer::showall)),NULL));
		} else {
            Sequence *backgroundEffect = _roundManager->IsTing(MIDDLE) 
                ? Sequence::create(
                        _HideReminder(HU_REMIND_ACT_TAG_ID, 0.3, 1.5),
                        _CreateHuBackgroundEffect(winner),NULL)
                : Sequence::create(
                        _CreateHuBackgroundEffect(winner),NULL);
            
			myframe->runAction(Spawn::create(
                    simple_tip_effect(_layout->PositionOfActSign(winner),"dahu.png"),
                    _voice->SpeakAction(HU,_roundManager->_cardHolders[winner]->GetSex()),CallFunc::create([=](){
    				GoldNumInsert(winner,3,giver);}),
                    backgroundEffect,NULL));
		}
	} else {
		ListenToDoubleHu();
	}
}

void NetRaceLayer::_QiEffect(PlayerDir_t dir) {
	myframe->_ID = MIDDLE;

    /********************************
        hide reminder
    ********************************/
	auto hideQiReminder =_HideQiReminder();
    
    /********************************
        
    ********************************/
	if(myframe->getChildByTag(QI_REMIND_ACT_BKG_TAG_ID)!=NULL && dir==MIDDLE) {/*??? is this judgement neccessary */
		if(!_roundManager->_isCardFromOthers) {
			if(_roundManager->_isGangAsking) {
				myframe->runAction(Sequence::create(
                    hideQiReminder,CallFunc::create([=](){
    					_roundManager->_isGangAsking = false;
                        
                        CardsInfo_t cards;
                        _GetCardsInfo(&cards);
                        
                        Vec2 location = _GetCardInHand(MIDDLE,cards.last)->getPosition();

                        _roundManager->RecvHandout(cards.last,location,2);/*bug??? forced to handout last card*/
                    }),NULL));
			} else
				myframe->runAction(hideQiReminder);
		} else {
			if(_roundManager->_isQiangGangAsking) {
				_roundManager->_isQiangGangAsking=false;

				myframe->runAction(Sequence::create(
                    hideQiReminder,Spawn::create(CallFunc::create([=](){
    					GoldNumInsert(_roundManager->_qiangGangTargetNo,2,_roundManager->_curPlayer);
    					_roundManager->_qiangGangTargetNo = INVALID;/*!!! could this be called before runAction */}),CallFunc::create([=](){
                        _roundManager->DistributeTo((PlayerDir_t)_roundManager->_curPlayer);}),NULL),NULL));
			} else if(_roundManager->_isDoubleHuAsking) {
				_roundManager->_isDoubleHuAsking=false;
                
				myframe->runAction(Sequence::create(
                    hideQiReminder,CallFunc::create([=](){
                    PlayerDir_t giver = (_roundManager->_otherOneForDouble==LEFT)?RIGHT:LEFT;
                    _roundManager->SetWin(DOUBLE_WIN,giver);
                    
                    WinInfo_t win;
                    _roundManager->GetWin(win);
                    _HuEffect(win);}),NULL));
			} else {
				myframe->runAction(Sequence::create(
                    hideQiReminder,CallFunc::create([=](){
                    _roundManager->DistributeTo(_roundManager->TurnToNext());}),NULL));
			}
		}
	}
}

/********************************************************
            cards
********************************************************/
/* this function is only for others */
Sprite *NetRaceLayer::_CreateCardInHand(PlayerDir_t dir,int idx,
                                        CARD_ARRAY *cards,bool isTing,const Vec2 &refer) {
    switch( cards->data[idx].status ) {
        case c_FREE:
            {
                Sprite *card;
                
                if(isTing) {
                    card = _object->Create(LR_OUT_CARD,dir,refer.x,refer.y);
                } else if(!isTing&&_roundManager->IsTing(MIDDLE)) {
                    card = _object->Create(LR_AN_GANG_CARD,dir,refer.x,refer.y);
                } else {
                    TextureId_t texture = (dir==LEFT)?L_IN_CARD:R_IN_CARD;
                    card = _object->Create(texture,dir,refer.x,refer.y);
                    card->setAnchorPoint(_layout->AnchorOfFreeCard(dir));
                }

                return card;
            }
        case c_AN_GANG:
            if(cards->data[idx].kind==cards->data[idx+1].kind
                &&cards->data[idx].kind!=cards->data[idx+2].kind
                &&!isTing&&_roundManager->IsTing(MIDDLE))
                return _object->Create(LR_OUT_CARD,dir,refer.x,refer.y);
            else
                return _object->Create(LR_AN_GANG_CARD,dir,refer.x,refer.y);
        case c_MING_KOU:
            return _object->Create(LR_AN_GANG_CARD,dir,refer.x,refer.y);
        case c_PENG:
        case c_MING_GANG:
            return _object->Create(LR_OUT_CARD,dir,refer.x,refer.y);
    }
}

float NetRaceLayer::_YofNextCard(PlayerDir_t dir,int idx,CARD_ARRAY *cards,bool isTing,float refY) {
    float up = (dir==LEFT)?(-1):1;
    float groupGap = (dir==LEFT)?0.4:0.8;
    
    switch(cards->data[idx].status) {
        case c_FREE:
            if(isTing||_roundManager->IsTing(MIDDLE)) {
                return up*(refY*0.65);
            } else {
                return up*(refY*0.5);
            }
        case c_MING_KOU:
            return up*(refY*0.65);
        case c_PENG:
            if(isTing||_roundManager->IsTing(MIDDLE)) {
                if(cards->data[idx+1].status==c_FREE||cards->data[idx+1].status==c_MING_KOU)
                    return up*refY;
                else if(cards->data[idx+1].kind!=cards->data[idx].kind)
                    return up*(refY*0.65+5);
                else 
                    return up*(refY*0.65);
            } else {
                if(cards->data[idx+1].status==c_FREE)
                    return up*(refY*groupGap);
                else if(cards->data[idx+1].kind!=cards->data[idx].kind)
                    return up*(refY*0.65+5);
                else
                    return up*(refY*0.65);
            }
        case c_MING_GANG:
        case c_AN_GANG:
            int groupIdx = _roundManager->_GroupIdx(idx,cards);
            if( (dir==LEFT&&groupIdx==1) || (dir==RIGHT&&groupIdx==3)) {
                return up*(refY*0.65);
            } else if (groupIdx==2) {
                return -up*13;
            } else if( (dir==LEFT&&groupIdx==3) || (dir==RIGHT&&groupIdx==1)) {
                return up*(refY*0.65+13);
            } else {
                if(_roundManager->IsTing(MIDDLE)||isTing==1) {
                    if(cards->data[idx+1].status==c_FREE||cards->data[idx+1].status==c_MING_KOU)
                        return up*refY;
                    else
                        return up*(refY*0.65+5);
                } else {
                    if(cards->data[idx+1].status==c_FREE)
                        return up*(refY*groupGap);
                    else
                        return up*(refY*0.65+5);
                }
            }
    }
}

/******************************************************
        ting hint bar
******************************************************/
void NetRaceLayer::_UpdateTingNum(PlayerDir_t dir) {
    _roundManager->_players[dir]->get_parter()->get_ming_reserved_cards_num(_roundManager->_river);
    
	int hu_residueForEvery[MAX_HANDIN_NUM][9];//剩余牌数
	int hu_NumForEveryCard[MAX_HANDIN_NUM];//胡张数

	_roundManager->_players[dir]->get_parter()->get_hu_NumForEveryCard(hu_NumForEveryCard);//张数
	_roundManager->_players[dir]->get_parter()->get_hu_residueForEvery(hu_residueForEvery);//剩余牌数

	int cardNum = hu_NumForEveryCard[Hu_cardOut_place];

	for(int i=0;i<cardNum;i++) {
		Sprite* curCardBar = _GetCardOnTingSignBar(dir,i);
		curCardBar->removeChildByTag(2);

        auto residueNum = _CreateNumberSign(hu_residueForEvery[Hu_cardOut_place][i]);
        curCardBar->addChild(residueNum,1,2);
	}
}

void NetRaceLayer::TingHintBarOfOthers(int curNo,int outCardIdx) {
	Hu_cardOut_place = outCardIdx;
    
	int hu_NumForEveryCard[MAX_HANDIN_NUM];
	CARD_KIND hu_cards[MAX_HANDIN_NUM][9];
	_roundManager->_players[curNo]->get_parter()->get_hu_NumForEveryCard(hu_NumForEveryCard);
	_roundManager->_players[curNo]->get_parter()->get_hu_cards(hu_cards);

    auto tingSignBar = _object->CreateTingSignBar((PlayerDir_t)curNo,(Card_t *)hu_cards[outCardIdx],hu_NumForEveryCard[outCardIdx]);
    myframe->addChild(tingSignBar,30,TING_SING_LEFTBAR+curNo/2);
}

void NetRaceLayer::_TingHintCreate(Point curPos,int CardPlace)
{
	_roundManager->_players[MIDDLE]->get_parter()->get_ming_reserved_cards_num(_roundManager->_river);
    
	Hu_cardOut_place=CardPlace;
	int huTiemsForEveryOne[MAX_HANDIN_NUM][9];//番型--
	int hu_residueForEvery[MAX_HANDIN_NUM][9];//剩余牌数
	int hu_NumForEveryCard[MAX_HANDIN_NUM];//胡张数
	CARD_KIND hu_cards[MAX_HANDIN_NUM][9];//胡哪几张牌
	_roundManager->_players[MIDDLE]->get_parter()->get_hu_NumForEveryCard(hu_NumForEveryCard);//张数
	_roundManager->_players[MIDDLE]->get_parter()->get_hu_residueForEvery(hu_residueForEvery);//剩余牌数
	_roundManager->_players[MIDDLE]->get_parter()->get_huTiemsForEveryOne(huTiemsForEveryOne);//番型
	_roundManager->_players[MIDDLE]->get_parter()->get_hu_cards(hu_cards);//胡哪几张牌

	int cardNum=hu_NumForEveryCard[CardPlace];

    auto tingSignBar = _object->CreateTingInfoBar(
        curPos,(Card_t *)hu_cards[CardPlace],cardNum,huTiemsForEveryOne[CardPlace],hu_residueForEvery[CardPlace]);
	myframe->addChild(tingSignBar, 30, TING_SING_BAR);
}

/****************************************
        ming
****************************************/
void NetRaceLayer::KouCancelEffect(CARD_ARRAY *cards) {
    for(int i=cards->atcvie_place;i<cards->len;i++) {
        Sprite *card = _GetCardInHand(MIDDLE,i);
        _Remove(card,MING_KOU);
    }

    auto button = myframe->getChildByTag(MING_KOU_CANCEL);
    
    myframe->_ID = MIDDLE;
    myframe->runAction(
        Sequence::create(
            TargetedAction::create(button,
                ScaleTo::create(0,0)),CallFunc::create([=](){
                _roundManager->RecvMing();}),NULL));
}

void NetRaceLayer::KouConfirmEffect() {
    auto button = myframe->getChildByTag(MING_KOU_ENSURE);

    myframe->_ID = MIDDLE;
    myframe->runAction(Sequence::create(TargetedAction::create(
        button,ScaleTo::create(0,0)),CCCallFunc::create([=]() {
        _CardInHandUpdateEffect(MIDDLE);}),CCCallFunc::create(this,callfunc_selector(
        NetRaceLayer::_DeleteActionTip)),CallFunc::create([=](){
        _roundManager->RecvMing();}),NULL));
}

void NetRaceLayer::MingCancelEffect() {
    auto button = myframe->getChildByTag(MING_CANCEL);
    
    myframe->_ID = MIDDLE;
    myframe->runAction(Sequence::create(TargetedAction::create(
        button,ScaleTo::create(0,0)),CCCallFunc::create([=]() {
        _CardInHandUpdateEffect(MIDDLE);}),CCCallFunc::create(this,callfunc_selector(
        NetRaceLayer::_DeleteActionTip)),CallFunc::create([=](){
        _roundManager->_actionToDo=_roundManager->_players[MIDDLE]->get_parter()->ActiontodoCheckAgain();
        _roundManager->WaitForMyAction();}),NULL));
}

void NetRaceLayer::BtnKouCancelHandler(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
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
            Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(myframe,true);

			_Remove(myframe,MING_KOU_ENSURE);			
			_Remove(myframe,MING_KOU_SIGN);
			curButton->setTouchEnabled(false);

            _roundManager->RecvKouCancel();
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}	
}

void NetRaceLayer::BtnKouConfirmHandler(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
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
			Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(myframe,true);
            
            _Remove(myframe,MING_KOU_SIGN);
			curButton->setTouchEnabled(false);

            _roundManager->RecvKouConfirm();
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void NetRaceLayer::MingCancelHandler(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
            _roundManager->RecvMingCancel();
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

Node *NetRaceLayer::_NonKouMask(Sprite *card) {
    auto mask = _object->Create(MING_MASK_CARD);
    mask->setAnchorPoint(Vec2(0.5,0.5));
    mask->setPosition(card->getTextureRect().size.width/2,card->getTextureRect().size.height/2);
    card->addChild(mask,2,MING_KOU_MASK);

    return mask;
}

void NetRaceLayer::_MaskNonKouCards(CARD_ARRAY *cards) {
    for(int i=cards->atcvie_place; i<cards->len; i++) {
        if(cards->data[i].status!=c_KOU_ENABLE) {
            Sprite *card = (Sprite*)myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+20+i);
            _NonKouMask(card);
        }
    }
}

bool NetRaceLayer::_KouTouchBegan(Touch* touch, Event* event) {
    auto cards=_roundManager->_players[MIDDLE]->get_parter()->get_card_list();

    Sprite *cardsInHand[MAX_HANDIN_NUM] = {0};
    int groupChosen=-1;
    
    for(int i=0;i<cards->len;i++) {
        cardsInHand[i]=_GetCardInHand(MIDDLE,i);
        cardsInHand[i]->_ID = MIDDLE;
    }
    
    for(int group=0; group<_ai->KouCardGroupNum(); group++) {
        for(int i=0; i<3; i++) {
            if ( _IsClickedOn(cardsInHand[_ai->KouCardIndex(group,i)], touch) ) {
                groupChosen = group;
                break;/* BUG : only 1 group can be chosen??? */
            }
        }
    }
    
    if(groupChosen!=-1) {
        for(int i=0; i<3; i++) {
            cardsInHand[_ai->KouCardIndex(groupChosen,i)]->_ID++;/*==2???*/
        }
    }
    
    return true;
}

/* !!! only one group one time */
int NetRaceLayer::_FindChosenGroup(Touch *touch,Sprite *cardsInHand[]) {
    for(int group=0; group<_ai->KouCardGroupNum(); group++) {
        for(int i=0;i<3;i++) {
            if ( _IsClickedOn(cardsInHand[_ai->KouCardIndex(group,i)],touch) ) {
                int idx = _ai->KouCardIndex(group,i);
                if(cardsInHand[idx]->_ID!=1) {
                    return group;
                }
            }
        }
    }

    return INVALID;
}

void NetRaceLayer::_KouTouchEnded(Touch* touch, Event* event) {
    auto cards=_roundManager->_players[MIDDLE]->get_parter()->get_card_list();

    Sprite *cardsInHand[MAX_HANDIN_NUM];
    for(int i=0; i<cards->len; i++) {
        cardsInHand[i]=_GetCardInHand(MIDDLE,i);
    }
    
    int groupChosen = _FindChosenGroup(touch,cardsInHand);
    
    if(groupChosen!=INVALID) {
        _ai->SwitchGroupStatus(groupChosen,cards);
    }

    _ai->Refresh(cards);
    
    for(int group=0;group<_ai->KouCardGroupNum();group++) {
        for(int i=0;i<3;i++) {
            _Remove(cardsInHand[_ai->KouCardIndex(group,i)],MING_KOU);
            _Remove(cardsInHand[_ai->KouCardIndex(group,i)],MING_KOU_MASK);

            if(cards->data[_ai->KouCardIndex(group,i)].status==c_MING_KOU) {
                auto MingKouMark=_object->Create(MING_KOU_CARD);
                MingKouMark->setAnchorPoint(Vec2(0.5,0.5));
                MingKouMark->setPosition(Vec2(cardsInHand[_ai->KouCardIndex(group,i)]->getTextureRect().size.width/2,cardsInHand[_ai->KouCardIndex(group,i)]->getTextureRect().size.height/2));
                cardsInHand[_ai->KouCardIndex(group,i)]->addChild(MingKouMark,2,MING_KOU);
            } else if(cards->data[_ai->KouCardIndex(group,i)].status!=c_KOU_ENABLE) {
                auto KouNo=_object->Create(MING_MASK_CARD);
                KouNo->setAnchorPoint(Vec2(0.5,0.5));
                KouNo->setPosition(Vec2(cardsInHand[_ai->KouCardIndex(group,i)]->getTextureRect().size.width/2,cardsInHand[_ai->KouCardIndex(group,i)]->getTextureRect().size.height/2));
                cardsInHand[_ai->KouCardIndex(group,i)]->addChild(KouNo,2,MING_KOU_MASK);
            }
        }
    }
    
    bool ifEnsureVisible=false;
    for(int group=0;group<4;group++) {
        if(_ai->KouCardStatus(group)==c_MING_KOU) {
            ifEnsureVisible=true;
            break;
        }
    }

    Node *node = myframe->getChildByTag(MING_KOU_ENSURE);
    if(ifEnsureVisible)
        myframe->getChildByTag(MING_KOU_ENSURE)->setVisible(true);
    else
        myframe->getChildByTag(MING_KOU_ENSURE)->setVisible(false);
}

void NetRaceLayer::ListenToKou(int no) {
    auto cards=_roundManager->_players[no]->get_parter()->get_card_list();
    auto KouListener = EventListenerTouchOneByOne::create();
    KouListener->setSwallowTouches(true);
    
    KouListener->onTouchBegan = CC_CALLBACK_2(NetRaceLayer::_KouTouchBegan,this);
    KouListener->onTouchMoved = [=](Touch* touch, Event* event) {};
    KouListener->onTouchEnded = CC_CALLBACK_2(NetRaceLayer::_KouTouchEnded,this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(KouListener, myframe);
}

void NetRaceLayer::QueryKouCards() {
    auto kouCancel = _object->CreateButton(BTN_CANCEL);
    kouCancel->addTouchEventListener(CC_CALLBACK_2(NetRaceLayer::BtnKouCancelHandler,this));
    myframe->addChild(kouCancel,20,MING_KOU_CANCEL);
    
    myframe->addChild(_object->CreateMingKouSign(),20,MING_KOU_SIGN);
    
    auto ChooseEnsure = _object->CreateButton(BTN_OK);
    ChooseEnsure->addTouchEventListener(CC_CALLBACK_2(NetRaceLayer::BtnKouConfirmHandler,this));
    ChooseEnsure->setVisible(false);
    myframe->addChild(ChooseEnsure,20,MING_KOU_ENSURE);
    
    CARD_ARRAY *cards = _roundManager->_players[MIDDLE]->get_parter()->get_card_list();
    _MaskNonKouCards(cards);
    ListenToKou(MIDDLE);
}

void NetRaceLayer::QueryMingOutCard() {
    _Remove(myframe,MING_KOU_ENSURE);
    _Remove(myframe,MING_KOU_SIGN);
    _Remove(myframe,MING_KOU_CANCEL);
    
    auto MingCancel = _object->CreateButton(BTN_CANCEL);
    MingCancel->addTouchEventListener(CC_CALLBACK_2(NetRaceLayer::MingCancelHandler,this));
    myframe->addChild(MingCancel,20,MING_CANCEL);
    
    myframe->_ID=MIDDLE;
    myframe->runAction(Sequence::create(CCCallFunc::create([=]() {
        _CardInHandUpdateEffect(MIDDLE);}),CCCallFunc::create(this,callfunc_selector(
        NetRaceLayer::ListenToCardTouch)),NULL));
}

void NetRaceLayer::BtnMingHandler(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
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
            if(_roundManager->_isWaitDecision) {
				_roundManager->_isWaitDecision = false;
				_roundManager->_actionToDo =_roundManager->_tempActionToDo;
				_roundManager->_tempActionToDo = a_JUMP;
			}
            
			for(int dir=0;dir<3;dir++) {//only once is enough?
			    _Remove(myframe,PENG_EFFECT_NODE_ID+dir);
                
				if(ifEffectTime) {
					ifEffectTime=false;
					ifUpdateDuringEffect=false;
                    
                    _roundManager->CancelEffectCard();
					_DeleteActionEffect();
                    
					_CardInHandUpdateEffect(MIDDLE);
				}
			}

			_eventDispatcher->removeEventListenersForTarget(myframe,true);

            Button *button = (Button*)pSender;
			auto icon = (Sprite*)myframe->getChildByTag(MING_REMIND_ACT_BKG_TAG_ID);

            button->_ID = MIDDLE;
			button->setTouchEnabled(false);
            
			myframe->_ID = MIDDLE;
            myframe->runAction(Sequence::create(
                Spawn::create(
                    TargetedAction::create(button,
                        ScaleTo::create(0,0)),
                    TargetedAction::create(icon,Sequence::create(
                        ScaleTo::create(0,1),Spawn::create(
                        FadeOut::create(0.3),
                        ScaleTo::create(0.3,1.3),NULL),NULL)),NULL),CCCallFunc::create(this,callfunc_selector(
                NetRaceLayer::_DeleteActionTip)),CCCallFunc::create([=](){
                _roundManager->RecvMing();}),NULL));
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		((Button*)pSender)->setScale(1);
		break;
	default:
		break;
	}
}

void NetRaceLayer:: display_callback(cocos2d::Ref* pSender) {
    LOGGER_WRITE("%s",__FUNCTION__);
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
    if( !_layout->_playerBkg[dir] ) {
        return;
    }

	LayerColor *propertyLayerBar = LayerColor::create();
	propertyLayerBar->ignoreAnchorPointForPosition(false);

    if(_layout->_playerBkg[dir]->getChildByTag(SCORE_TAG_ID))
        _layout->_playerBkg[dir]->removeChildByTag(SCORE_TAG_ID,true);

    propertyLayerBar->setAnchorPoint(Vec2(0.5,0.5));
    
	if(dir==0) {
		propertyLayerBar->setContentSize( Size(_layout->_playerBkg[0]->getTextureRect().size.width, 25) );
		propertyLayerBar->setPosition( Vec2(_layout->_playerBkg[0]->getTextureRect().size.width*0.5,
            _layout->_playerBkg[0]->getTextureRect().size.height * 0.1));
	} else if(dir==1) {
		propertyLayerBar->setContentSize( Size(140, 25) );
		propertyLayerBar->setPosition( Vec2(_layout->_playerBkg[1]->getTextureRect().size.width*0.6635,
            _layout->_playerBkg[1]->getTextureRect().size.height*0.3));
	} else if(dir==2) {
		propertyLayerBar->setContentSize( Size(_layout->_playerBkg[2]->getTextureRect().size.width, 25) );
		propertyLayerBar->setPosition( Vec2(_layout->_playerBkg[2]->getTextureRect().size.width*0.5,
            _layout->_playerBkg[2]->getTextureRect().size.height * 0.1));
	}
    
    _layout->_playerBkg[dir]->addChild(propertyLayerBar,1,SCORE_TAG_ID);
    
    _GuiUpdateScore(propertyLayerBar,score);
}

void NetRaceLayer::_UpdateNickName(int direction,std::string str_Nick)
{		
	if(direction==0)
	{
		if(!_layout->_playerBkg[0])
			return;
		if(_layout->_playerBkg[0]->getChildByTag(NICK_NAME_TAG_ID))
			_layout->_playerBkg[0]->removeChildByTag(NICK_NAME_TAG_ID,true);

		auto left_Name=Label::create(str_Nick,"Arial",20);
		left_Name->setPosition(Vec2(_layout->_playerBkg[0]->getTextureRect().size.width*0.5,_layout->_playerBkg[0]->getTextureRect().size.height*0.265));
		_layout->_playerBkg[0]->addChild(left_Name,1,NICK_NAME_TAG_ID);
	}
	else if(direction==1)
	{
		if(!_layout->_playerBkg[1])
			return;
		if(_layout->_playerBkg[1]->getChildByTag(NICK_NAME_TAG_ID))
			_layout->_playerBkg[1]->removeChildByTag(NICK_NAME_TAG_ID,true);

		auto myName=Label::create(str_Nick,"Arial",20);
		myName->setPosition(Vec2(_layout->_playerBkg[1]->getTextureRect().size.width*0.65,_layout->_playerBkg[1]->getTextureRect().size.height*0.7));
		_layout->_playerBkg[1]->addChild(myName,1,NICK_NAME_TAG_ID);

	}
	else if(direction==2)
	{
		if(!_layout->_playerBkg[2])
			return;
		if(_layout->_playerBkg[2]->getChildByTag(NICK_NAME_TAG_ID))
			_layout->_playerBkg[2]->removeChildByTag(NICK_NAME_TAG_ID,true);

		auto Right_Name=Label::create(str_Nick,"Arial",20);
		Right_Name->setPosition(Vec2(_layout->_playerBkg[2]->getTextureRect().size.width*0.5,_layout->_playerBkg[2]->getTextureRect().size.height*0.265));
		_layout->_playerBkg[2]->addChild(Right_Name,1,NICK_NAME_TAG_ID);
	}

}

void NetRaceLayer::_UpdateHeadImage(int direction,std::string head_photo)
{
	auto head_image=Sprite::createWithSpriteFrameName(head_photo);

	if(direction==0)
	{
		if(!_layout->_playerBkg[0])
			return;

		if(_layout->_playerBkg[0]->getChildByTag(HEAD_IMG_TAG_ID))
			_layout->_playerBkg[0]->removeChildByTag(HEAD_IMG_TAG_ID,true);

		head_image->setScaleX(_layout->_playerBkg[0]->getTextureRect().size.width*0.85/head_image->getTextureRect().size.width);
		head_image->setScaleY(_layout->_playerBkg[0]->getTextureRect().size.width*0.85/head_image->getTextureRect().size.width);
		head_image->setAnchorPoint(Vec2(0.5f,1.0f));
		head_image->setPosition(Vec2(_layout->_playerBkg[0]->getTextureRect().size.width*1/2,_layout->_playerBkg[0]->getTextureRect().size.height*23/24));
		_layout->_playerBkg[0]->addChild(head_image,1,HEAD_IMG_TAG_ID);
	}
	else if(direction==1)
	{
		if(!_layout->_playerBkg[1])
			return;
		if(_layout->_playerBkg[1]->getChildByTag(HEAD_IMG_TAG_ID))
			_layout->_playerBkg[1]->removeChildByTag(HEAD_IMG_TAG_ID,true);

		float head_scale;
		head_scale=(_layout->_playerBkg[1]->getTextureRect().size.height*5)/(6*head_image->getTextureRect().size.height);
		head_image->setScaleX(head_scale);
		head_image->setScaleY(head_scale);
		head_image->setAnchorPoint(Vec2(0.0f,0.5f));
		head_image->setPosition(Vec2(_layout->_playerBkg[1]->getTextureRect().size.width/39,_layout->_playerBkg[1]->getTextureRect().size.height*1/2));
		_layout->_playerBkg[1]->addChild(head_image,1,HEAD_IMG_TAG_ID);
	}
	else if(direction==2)
	{
		if(!_layout->_playerBkg[2])
			return;
		if(_layout->_playerBkg[2]->getChildByTag(HEAD_IMG_TAG_ID))
			_layout->_playerBkg[2]->removeChildByTag(HEAD_IMG_TAG_ID,true);

		head_image->setScaleX(_layout->_playerBkg[0]->getTextureRect().size.width*0.85/head_image->getTextureRect().size.width);
		head_image->setScaleY(_layout->_playerBkg[0]->getTextureRect().size.width*0.85/head_image->getTextureRect().size.width);
		head_image->setAnchorPoint(Vec2(0.5f,1.0f));
		head_image->setPosition(Vec2(_layout->_playerBkg[2]->getTextureRect().size.width*1/2,_layout->_playerBkg[2]->getTextureRect().size.height*23/24));
		_layout->_playerBkg[2]->addChild(head_image,1,HEAD_IMG_TAG_ID);
	}
}

void NetRaceLayer::_CalcAnGangGold(int winner,int goldOfPlayer[3]) {
    goldOfPlayer[winner]       = 4*PREMIUM_LEAST*(_roundManager->_continue_gang_times);
    goldOfPlayer[(winner+1)%3] = -2*PREMIUM_LEAST*(_roundManager->_continue_gang_times);
    goldOfPlayer[(winner+2)%3] = -2*PREMIUM_LEAST*(_roundManager->_continue_gang_times);
}

void NetRaceLayer::_CalcMingGangGold(int winner,int giver,int goldOfPlayer[3]) {
    if (winner==giver) {
        goldOfPlayer[winner]       = 2*PREMIUM_LEAST*(_roundManager->_continue_gang_times);
        goldOfPlayer[(winner+1)%3] = -1*PREMIUM_LEAST*(_roundManager->_continue_gang_times);
        goldOfPlayer[(winner+2)%3] = -1*PREMIUM_LEAST*(_roundManager->_continue_gang_times);
    } else {
        goldOfPlayer[winner]       =2*PREMIUM_LEAST*(_roundManager->_continue_gang_times);
        goldOfPlayer[giver]        =-2*PREMIUM_LEAST*(_roundManager->_continue_gang_times);
    }
}

void NetRaceLayer::_CalcSingleWinGold(int goldOfPlayer[3], int winner,int whoGive) {
    auto score = _roundManager->_players[winner]->get_parter()->get_card_score();
    goldOfPlayer[winner] = score*PREMIUM_LEAST;
    
    if(whoGive==winner) {
        goldOfPlayer[(winner+1)%3] = -(goldOfPlayer[winner]/2);
        goldOfPlayer[(winner+2)%3] = -(goldOfPlayer[winner]/2);
    } else {
        goldOfPlayer[whoGive] = -goldOfPlayer[winner];
    }
    
    goldOfPlayer[(winner+1)%3] = goldOfPlayer[(winner+1)%3] + goldOfPlayer[(winner+1)%3]*_roundManager->IsTing((winner+1)%3);
    goldOfPlayer[(winner+2)%3] = goldOfPlayer[(winner+2)%3] + goldOfPlayer[(winner+2)%3]*_roundManager->IsTing((winner+2)%3);
    goldOfPlayer[winner] = - (goldOfPlayer[(winner+1)%3] + goldOfPlayer[(winner+2)%3]);
}

void NetRaceLayer::_CalcDoubleWinGold(int goldOfPlayer[3], int giver) {
    for(int i=1;i<3;i++) {
        auto score = _roundManager->_players[(giver+i)%3]->get_parter()->get_card_score();
        int  ting  = _roundManager->IsTing((giver+i)%3);

        goldOfPlayer[(giver+i)%3] = score*PREMIUM_LEAST + score*PREMIUM_LEAST*ting;
    }

    goldOfPlayer[giver] = - ((goldOfPlayer[(giver+1)%3] + goldOfPlayer[(giver+2)%3]));
}

void NetRaceLayer::_CalcNoneWinGold(int goldOfPlayer[3], int giver) {
    GoldAccountImmediate[(giver+1)%3] = PREMIUM_LEAST;
    GoldAccountImmediate[(giver+2)%3] = PREMIUM_LEAST;
    GoldAccountImmediate[giver] = - ((goldOfPlayer[(giver+1)%3] + goldOfPlayer[(giver+2)%3]));
}

void NetRaceLayer::_CalcHuGold(int goldOfPlayer[3]) {
    WinInfo_t win;
    _roundManager->GetWin(win);
    
    switch(win.kind) {
        case SINGLE_WIN:
            _CalcSingleWinGold(goldOfPlayer,win.winner,win.giver);
            break;
        case DOUBLE_WIN:
            _CalcDoubleWinGold(goldOfPlayer,win.giver);
            break;
        case NONE_WIN:
            _CalcNoneWinGold(goldOfPlayer,win.winner);
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

void NetRaceLayer::GuiJinBiShow(PlayerDir_t dir, int gold) {
	Size size = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	char str[15];
	sprintf(str,"%d",abs(gold));
	auto number=LabelAtlas::create(std::string(str),"fonts/moneyMessage.png",28,39,'0');
	number->setAnchorPoint(_layout->AnchorOfNumber(dir));
	number->setOpacity(0);
	number->setPosition(_layout->PositionOfNumber(dir));
	this->addChild(number,6,GOLD_NUM_INSERT_NUMBER);

    int xoffset = number->getContentSize().width;
    
	auto minus=Sprite::create("-.png");
	minus->setAnchorPoint(_layout->AnchorOfSign(dir));
	minus->setPosition(_layout->PositionOfSign(dir,xoffset));
	minus->setOpacity(0);
	this->addChild(minus,6,GOLD_NUM_INSERT_JIANHAO);

	auto plus=Sprite::create("+.png");
	plus->setAnchorPoint(_layout->AnchorOfSign(dir));
	plus->setPosition(_layout->PositionOfSign(dir,xoffset));
	plus->setOpacity(0);
	this->addChild(plus,6,GOLD_NUM_INSERT_JIAHAO);

	auto jinbi=Sprite::create("jinbi-game.png");
	jinbi->setAnchorPoint(_layout->AnchorOfGold(dir));
	jinbi->setOpacity(0);
	jinbi->setPosition(_layout->PositionOfGold(dir,xoffset));
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
        MoveTo::create(1.5,_layout->DestOfGold(dir,xoffset)),NULL),
        ScaleTo::create(0,0),NULL));
    
	number->runAction(Sequence::create(
        DelayTime::create(0),
        Spawn::create(FadeIn::create(1.5),
        MoveTo::create(1.5,_layout->DestOfNumber(dir)),NULL),
        ScaleTo::create(0,0),NULL));

    symbol->runAction(Sequence::create(
        DelayTime::create(0),
        Spawn::create(FadeIn::create(1.5),
        MoveTo::create(1.5,_layout->DestOfSign(dir,xoffset)),NULL),
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
        GuiJinBiShow((PlayerDir_t)id,GoldAccountImmediate[id]);        
	}
}


/***********************************************************
        gold show
***********************************************************/
Label *NetRaceLayer::_CreateName(const char *name) {
    auto NickName=Label::create(name,"Arial",28);
    NickName->setAnchorPoint(Vec2(0.5,0.5));
    NickName->setPosition(Vec2(70,60));
    return NickName;
}

Sprite *NetRaceLayer::_CreateHeadImage(const char *file) {
    auto headPhoto=Sprite::createWithSpriteFrameName(file);//ͷ��4
    headPhoto->setScaleX(100/headPhoto->getTextureRect().size.width);
    headPhoto->setScaleY(100/headPhoto->getTextureRect().size.height);
    headPhoto->setAnchorPoint(Vec2(0,0));
    headPhoto->setPosition(Vec2(20,80));
    return headPhoto;
}

Sprite *NetRaceLayer::_CreateGoldImage() {
    auto gold = Sprite::createWithSpriteFrameName("result_money.png");//���1
    gold->setAnchorPoint(Vec2(0,0.5));
    gold->setPosition(Vec2(15,22));
    return gold;
}

LabelAtlas *NetRaceLayer::_CreatePropertyNumber(int number,Sprite *gold) {
    std::string strNumber = _NumToString(number);
    auto label = LabelAtlas::create(strNumber,"fonts/result_money_number.png",17,23,'.');
    label->setAnchorPoint(Vec2(0,0.5));
    label->setPosition(Vec2(gold->getPosition().x+gold->getTextureRect().size.width+5, 22));
    return label;
}

Sprite *NetRaceLayer::_CreatePropertyUnit(int number,LabelAtlas *label) {
    Sprite* unit = NULL;
    if(number>=100000&&number<100000000) {
        unit=Sprite::createWithSpriteFrameName("wan-hand.png");
        unit->setAnchorPoint(Vec2(0,0.5));
        unit->setPosition(Vec2(label->getPosition().x+label->getContentSize().width+5,22));
    }
    else if(number>=100000000) {
        unit=Sprite::createWithSpriteFrameName("yi-hand.png");
        unit->setAnchorPoint(Vec2(0,0.5));
        unit->setPosition(Vec2(label->getPosition().x+label->getContentSize().width+5,22));
    }
    return unit;
}

Sprite *NetRaceLayer::_CreateHu() {
    auto Win=Sprite::createWithSpriteFrameName("jiesuanhu.png");//��5
    Win->setAnchorPoint(Vec2(0.5,0.5));
    Win->setPosition(Vec2(origin.x+visibleSize.width*0.9,origin.y+visibleSize.height*0.15363));
    Win->setVisible(false);
    return Win;
}

Sprite *NetRaceLayer::_CreateZiMo() {
    auto selfDrawn=Sprite::createWithSpriteFrameName("jiesuanzimo.png");//����6
    selfDrawn->setAnchorPoint(Vec2(0.5,0.5));
    selfDrawn->setPosition(Vec2(origin.x+visibleSize.width*0.9,origin.y+visibleSize.height*0.15363));
    selfDrawn->setVisible(false);
    return selfDrawn;
}

Sprite *NetRaceLayer::_CreateFangPao() {
    auto PointGun1=Sprite::createWithSpriteFrameName("jiesuanfangpao.png");//����7
    PointGun1->setAnchorPoint(Vec2(0.5,0.5));
    PointGun1->setPosition(Vec2(origin.x+visibleSize.width*0.9,origin.y+visibleSize.height*0.15363));
    PointGun1->setVisible(false);
    return PointGun1;
}

Sprite *NetRaceLayer::_CreateBaozhuang() {
	auto BaoZhuang=Sprite::createWithSpriteFrameName("jiesuanbaozhuang.png");//包庄
	BaoZhuang->setAnchorPoint(Vec2(0.5,0.5));
	BaoZhuang->setPosition(Vec2(origin.x+visibleSize.width*0.9,origin.y+visibleSize.height*0.15363));
	BaoZhuang->setVisible(false);
    return BaoZhuang;
}

void NetRaceLayer::_CreateAccountPanel(const UserProfile_t &profile, Node *parent) {
    parent->addChild(_CreateName(profile.name),2,ACCOUNT_NIKENAME);
    parent->addChild(_CreateHeadImage(profile.photo),2,ACCOUNT_IMAGE);
    auto gold = _CreateGoldImage();
    parent->addChild(gold,2,ACCOUNT_JINBI);
    auto label = _CreatePropertyNumber(profile.property,gold);
    parent->addChild(label,2,ACCOUNT_PROPRETY);
    auto unit = _CreatePropertyUnit(profile.property,label);
    if(unit!=NULL) {
        parent->addChild(unit,ACCOUNT_PROPRETY_UNIT);
    }
    parent->addChild(_CreateHu(),2,ACCOUNT_HU_FONT);
    parent->addChild(_CreateZiMo(),2,ACCOUNT_ZIMO_FONT);
    parent->addChild(_CreateFangPao(),2,ACCOUNT_DIANPAO_FONT);
    parent->addChild(_CreateBaozhuang(),2,ACCOUNT_BAOZHUANG_FONT);
}

Sprite *NetRaceLayer::_CreateSymbol(PlayerDir_t dir,int gold,LayerColor *parent) {
    Sprite *sign = NULL;
    
    if(gold>0) {
        sign = Sprite::createWithSpriteFrameName("fen_add.png");
    } else {
        sign = Sprite::createWithSpriteFrameName("fen_sub.png");
    }
    sign->setAnchorPoint(Vec2(0,0.5));
    
    if( _roundManager->IsWinner(dir) )
        sign->setPosition(Vec2(origin.x+visibleSize.width*0.816+20,parent->getContentSize().height/2));
    else
        sign->setPosition(Vec2(origin.x+visibleSize.width*0.816+20,origin.y+30));

    if(gold!=0) {
        sign->setVisible(true);
    } else {
        sign->setVisible(false);
    }

    return sign;
}

LabelAtlas *NetRaceLayer::_CreatePropertyChange(PlayerDir_t dir,int gold,LayerColor *parent) {
    char char_AccoutnGold[80];
    std::string str_AccountGold;
    sprintf(char_AccoutnGold,"%d",abs(gold));
    str_AccountGold = char_AccoutnGold;
    LabelAtlas* propertyOfIncrease = NULL;

    if(gold>0) {
        propertyOfIncrease = LabelAtlas::create(str_AccountGold,"fonts/Score_add_number.png",26,32,'0');//加
    } else {
        propertyOfIncrease = LabelAtlas::create(str_AccountGold,"fonts/Score_sub_number.png",26,32,'0');//减
    }

    propertyOfIncrease->setAnchorPoint(Vec2(0,0.5));
    if( _roundManager->IsWinner(dir)  )
        propertyOfIncrease->setPosition(Vec2(origin.x+visibleSize.width*0.816+50+20,parent->getContentSize().height/2));
    else
        propertyOfIncrease->setPosition(Vec2(origin.x+visibleSize.width*0.816+50+20,origin.y+30));

    if(gold!=0) {
        propertyOfIncrease->setVisible(true);
    } else {
        propertyOfIncrease->setVisible(false);
    }

    return propertyOfIncrease;
}

void NetRaceLayer::_ExposeCards(PlayerDir_t dir,const WinInfo_t &win,LayerColor *parent) {
	auto posOfCards = Vec2(162,180);//牌0，1
	float x = posOfCards.x;
	float y = posOfCards.y - _object->RectSize(PENG_CARD).height/2;
    
	Sprite *show_card_list[MAX_HANDIN_NUM];
	CARD_ARRAY *list=_roundManager->_players[dir]->get_parter()->get_card_list();
    
	for(int i=0;i<list->len;i++)
	{
		show_card_list[i]=_object->Create(PENG_CARD);
		show_card_list[i]->setAnchorPoint(Vec2(0,0.5));
		show_card_list[i]->setPosition(Vec2(x,y));
		parent->addChild(show_card_list[i],2);
		auto s_card=_object->CreateKind((Card_t)list->data[i].kind,MIDDLE_SIZE);
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
			if( win.kind==SINGLE_WIN && win.winner==dir && win.giver==dir && i==(list->len-2) )
				x=x+show_card_list[i]->getTextureRect().size.width*0.95+30;
			else
				x+=show_card_list[i]->getTextureRect().size.width*0.95;
		}
	}

    if((win.kind==SINGLE_WIN && win.winner==dir) || (win.kind==DOUBLE_WIN && win.giver!=dir))
    {
        if(_roundManager->_isCardFromOthers)
        {
            auto winCard=_object->Create(PENG_CARD);
            winCard->setAnchorPoint(Vec2(0,0.5));
            winCard->setPosition(Vec2(x+30,y));
            auto s_card=_object->CreateKind((Card_t)list->data[list->len].kind,MIDDLE_SIZE);
            s_card->setAnchorPoint(Vec2(0.5,0.5));
            s_card->setPosition(Vec2(winCard->getTextureRect().size.width/2,winCard->getTextureRect().size.height*0.6));
            winCard->addChild(s_card,1);

            parent->addChild(winCard,2);
        }
    }
}

void NetRaceLayer::AccountShows(LayerColor* BarOfPlayer,int no) {
	_CreateAccountPanel(_roundManager->_cardHolders[no]->_profile,BarOfPlayer);

    WinInfo_t win;
    _roundManager->GetWin(win);

	if( !(win.kind==NONE_WIN && _roundManager->_firstMingNo==INVALID) ) {
        auto sign = _CreateSymbol((PlayerDir_t)no,GoldAccountImmediate[no],BarOfPlayer);
        BarOfPlayer->addChild(sign,2,ACCOUNT_DIANPAO_FONT);

        auto goldChange = _CreatePropertyChange((PlayerDir_t)no,GoldAccountImmediate[no],BarOfPlayer);
        BarOfPlayer->addChild(goldChange,2,ACCOUNT_WINGOLD_NUM);
	}

    _ExposeCards((PlayerDir_t)no,win,BarOfPlayer);
    
	int tagNum = BarOfPlayer->getTag();
	if(_roundManager->IsTing(tagNum)&&
            ((win.kind==SINGLE_WIN && 
                ((win.winner==win.giver && tagNum!=win.winner)  /* others zimo */
                ||(win.winner!=win.giver && tagNum==win.giver)))/* tagNum dianpao */
            || (win.kind==DOUBLE_WIN && tagNum==win.giver))) {
        float Extra_x=162;
        float Extra_y=origin.y+visibleSize.height*0.1256-10;

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
	float x = 162;
	float y = origin.y+visibleSize.height*0.1256-10;

	int tagNum=BarOfPlayer->getTag();
	unsigned char tingStatus=_roundManager->_players[tagNum]->get_parter()->get_ting_status();
	auto curScore=_roundManager->_players[tagNum]->get_parter()->get_card_score();

    WinInfo_t win;
    _roundManager->GetWin(win);
    
	if((win.kind==SINGLE_WIN
            &&(win.winner==win.giver && curScore==2)
            ||(win.winner!=win.giver && tagNum==win.winner && curScore==1))
        ||(win.kind==DOUBLE_WIN && tagNum!=win.giver && curScore==1))
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
		if(tingStatus==1 &&
                ((win.kind==SINGLE_WIN && tagNum==win.winner)
                ||(win.kind==DOUBLE_WIN && (tagNum!=win.giver)))) {
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
	//
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

    switch(win.kind) {
        case SINGLE_WIN:
            {
                auto WinBar=(LayerColor*)raceAccoutLayer->getChildByTag(win.winner); 
                auto WinBarPlus=(LayerColor*)raceAccoutLayer->getChildByTag((win.winner+1)%3);
                auto WinBarMinus=(LayerColor*)raceAccoutLayer->getChildByTag((win.winner+2)%3);
            
                _roundManager->_players[win.winner]->get_parter()->get_Hu_Flag(&num);
                AccountShows(WinBar,win.winner);
                AccountShows(WinBarPlus,(win.winner+1)%3);
                AccountShows(WinBarMinus,(win.winner+2)%3);
            
                if(win.winner==win.giver) {
                    WinBar->getChildByTag(ACCOUNT_ZIMO_FONT)->setVisible(true);
                } else {
                    WinBar->getChildByTag(ACCOUNT_HU_FONT)->setVisible(true);
                    raceAccoutLayer->getChildByTag(win.giver)->getChildByTag(ACCOUNT_DIANPAO_FONT)->setVisible(true);
                }
            
                AccountHuKind(WinBar,num);
                break;
            }
        case DOUBLE_WIN:
            {
                auto WinBar=(LayerColor*)raceAccoutLayer->getChildByTag(win.giver); 
                auto WinBarPlus=(LayerColor*)raceAccoutLayer->getChildByTag((win.giver+1)%3);
                auto WinBarMinus=(LayerColor*)raceAccoutLayer->getChildByTag((win.giver+2)%3);
            
                _roundManager->_players[(win.giver+1)%3]->get_parter()->get_Hu_Flag(&num);
                _roundManager->_players[(win.giver+2)%3]->get_parter()->get_Hu_Flag(&numDoubule);
            
                AccountShows(WinBar,win.giver);
                AccountShows(WinBarPlus,(win.giver+1)%3);
                AccountShows(WinBarMinus,(win.giver+2)%3);
            
                WinBar->getChildByTag(ACCOUNT_DIANPAO_FONT)->setVisible(true);
                WinBarPlus->getChildByTag(ACCOUNT_HU_FONT)->setVisible(true);
                WinBarMinus->getChildByTag(ACCOUNT_HU_FONT)->setVisible(true);
            
                AccountHuKind(WinBarPlus,num);
                AccountHuKind(WinBarMinus,numDoubule);
                break;
            }
        case NONE_WIN:
            if(_roundManager->_firstMingNo!=INVALID) {
                int fakeWinner = _roundManager->_firstMingNo;
                
                auto WinBar=(LayerColor*)raceAccoutLayer->getChildByTag(fakeWinner); 
                auto WinBarPlus=(LayerColor*)raceAccoutLayer->getChildByTag((fakeWinner+1)%3);
                auto WinBarMinus=(LayerColor*)raceAccoutLayer->getChildByTag((fakeWinner+2)%3);
                
                AccountShows(WinBar,fakeWinner);
                AccountShows(WinBarPlus,(fakeWinner+1)%3);
                AccountShows(WinBarMinus,(fakeWinner+2)%3);
            
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
                auto WinBar=(LayerColor*)raceAccoutLayer->getChildByTag(_roundManager->_curPlayer); 
                auto WinBarPlus=(LayerColor*)raceAccoutLayer->getChildByTag((_roundManager->_curPlayer+1)%3);
                auto WinBarMinus=(LayerColor*)raceAccoutLayer->getChildByTag((_roundManager->_curPlayer+2)%3);
                
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
	PlayOnceAgin->addTouchEventListener(CC_CALLBACK_2(NetRaceLayer::BtnRestartHandler,this));
	PlayOnceAgin->setAnchorPoint(Vec2(0.5,0.5));
	PlayOnceAgin->setPosition(Vec2(origin.x+visibleSize.width*0.6247948,origin.y+visibleSize.height*0.075419));
	this->addChild(PlayOnceAgin,11,RACE_RESTART_TAG_ID);
}

void  NetRaceLayer::showall()
{
    LOGGER_WRITE("%s",__FUNCTION__);

    HideClock();
	
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
		x=_layout->_playerPosi[no].basePoint.x+10; 
		y=_layout->_playerPosi[no].basePoint.y+10;
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
							p_list[i]=_object->Create(LR_OUT_CARD);
							p_list[i]->setAnchorPoint(Vec2(0.3f,1.0f));
							p_list[i]->setPosition(Vec2(x-10,y-8));
							auto s_card=_object->CreateKind((Card_t)list->data[i].kind,SMALL);
							s_card->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height*0.65));
							s_card->setRotation(90);
							s_card->setScale(0.9);
							p_list[i]->addChild(s_card);
						}
						else
						{
							p_list[i]=_object->Create(LR_AN_GANG_CARD);
							p_list[i]->setAnchorPoint(Vec2(0.3f,1.0f));
							p_list[i]->setPosition(Vec2(x-10,y-8));
						}
					}
					else
					{
						p_list[i]=_object->Create(LR_OUT_CARD);
						p_list[i]->setAnchorPoint(Vec2(0.3f,1.0f));
						p_list[i]->setPosition(Vec2(x-10,y-8));
						auto s_card=_object->CreateKind((Card_t)list->data[i].kind,SMALL);
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
						p_list[i]=_object->Create(MING_CARD);
					else if(list->data[i].status==c_PENG||list->data[i].status==c_MING_GANG)
						p_list[i]=_object->Create(PENG_CARD);
					else if(list->data[i].status==c_AN_GANG)
					{
						if(list->data[i].kind==list->data[i+1].kind && list->data[i].kind!=list->data[i+2].kind)
							p_list[i]=_object->Create(PENG_CARD);
						else
							p_list[i]=_object->Create(AN_GANG_CARD);
					}
					p_list[i]->setAnchorPoint(Vec2(0,0));
					p_list[i]->setPosition(Vec2(x,y));
					if(list->data[i].status==c_FREE)
					{
						auto s_card=_object->CreateKind((Card_t)list->data[i].kind,NORMAL);
						s_card->setAnchorPoint(Vec2(0.5,0.5));
						s_card->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height*0.6));
						p_list[i]->addChild(s_card,1);
						x+=p_list[i]->getTextureRect().size.width*1.0;
					}
					else if(list->data[i].status==c_MING_KOU)
					{
						auto s_card=_object->CreateKind((Card_t)list->data[i].kind,NORMAL);
						s_card->setAnchorPoint(Vec2(0.5,0.5));
						s_card->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height*0.6));
						p_list[i]->addChild(s_card,1);
						x += p_list[i]->getTextureRect().size.width*1;
					}
					else if(list->data[i].status==c_PENG)
					{
						auto s_card=_object->CreateKind((Card_t)list->data[i].kind,MIDDLE_SIZE);
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
						auto s_card=_object->CreateKind((Card_t)list->data[i].kind,MIDDLE_SIZE);
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
							auto s_card=_object->CreateKind((Card_t)list->data[i].kind,MIDDLE_SIZE);
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
							p_list[i]=_object->Create(LR_OUT_CARD);
							p_list[i]->setAnchorPoint(Vec2(0.3,0));
							p_list[i]->setPosition(Vec2(x-10,y-8));
							auto s_card=_object->CreateKind((Card_t)list->data[i].kind,SMALL);
							s_card->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height*0.65));
							s_card->setRotation(-90);
							s_card->setScale(0.9);
							p_list[i]->addChild(s_card);
						}
						else
						{
							p_list[i]=_object->Create(LR_AN_GANG_CARD);
							p_list[i]->setAnchorPoint(Vec2(0.3,0));
							p_list[i]->setPosition(Vec2(x-10,y-8));
						}
					}
					else
					{
						p_list[i]=_object->Create(LR_OUT_CARD);
						p_list[i]->setAnchorPoint(Vec2(0.3,0));
						p_list[i]->setPosition(Vec2(x-10,y-8));
						auto s_card=_object->CreateKind((Card_t)list->data[i].kind,SMALL);
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

/***********************************************************
        button accessosaries
***********************************************************/
void NetRaceLayer::BtnStartHandler(Ref* pSender,ui::Widget::TouchEventType type) {
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

			auto startIcon = _object->CreateStartGame();
			this->addChild(startIcon,2,START_GAME_TAG_ID+1);

			this->runAction(
                Sequence::create(
                    Spawn::create(
                        TargetedAction::create(curButton,Sequence::create(
                            DelayTime::create(0.1),
                            FadeOut::create(0.2),NULL)),
                        TargetedAction::create(startIcon,Spawn::create(
                            ScaleTo::create(0.3,1.3),
                            FadeOut::create(0.3),NULL)),
                        _voice->Speak("anniu.ogg"),NULL),
                    DelayTime::create(0.2),CallFunc::create(this,callfunc_selector(
                    NetRaceLayer::StartGame)),NULL));
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void NetRaceLayer::BtnRestartHandler(Ref* pSender,ui::Widget::TouchEventType type) {
    LOGGER_WRITE("%s",__FUNCTION__);

	auto curButton=(Button*)pSender;

	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		{
			((Button*)this->getChildByTag(MENU_BKG_TAG_ID)->getChildByTag(GAMEBACK_MENU_BUTTON))->setTouchEnabled(true);
			((Button*)this->getChildByTag(MENU_BKG_TAG_ID)->getChildByTag(TUOGUAN_MENU_BUTTON))->setHighlighted(false);
            
            _InitResidueCards();
			_UpdateResidueCards(TOTAL_CARD_NUM - _roundManager->_distributedNum);
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			curButton->setTouchEnabled(false);
            
			auto restartBtn = _object->CreateRestartGame();
			this->addChild(restartBtn,2,RACE_RESTART_TAG_ID+1);

			this->runAction(
                Sequence::create(
                    Spawn::create(TargetedAction::create(
                        restartBtn,Spawn::create(
                            ScaleTo::create(0.3,1.3),
                            FadeOut::create(0.3),NULL)),
                        _voice->Speak("anniu.ogg"),NULL),
                    DelayTime::create(0.2),CallFunc::create(this,callfunc_selector(
                    NetRaceLayer::StartGame)),NULL));
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void NetRaceLayer::BtnTuoGuanCancelHandler(Ref* pSender,ui::Widget::TouchEventType type)
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
			
			auto TuoGuanButton=(Button*)this->getChildByTag(MENU_BKG_TAG_ID)->getChildByTag(TUOGUAN_MENU_BUTTON);
			TuoGuanButton->setTouchEnabled(true);
			TuoGuanButton->setHighlighted(false);
			curButton->setTouchEnabled(false);
			curButton->setScale(0);
			_roundManager->_isTuoGuan=false;
			auto StartSelfGame=CallFunc::create([=](){
				if( !_roundManager->IsTing(1) )
					ListenToCardTouch();
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

void NetRaceLayer::BtnTuoGuanHandler(Ref* pSender,ui::Widget::TouchEventType type)
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
			_eventDispatcher->removeEventListenersForTarget(myframe,true);//???????

			_roundManager->_isTuoGuan=true;
			curButton->setTouchEnabled(false);
			curButton->setHighlighted(true);

			auto TuoGuanCancel=Button::create("quxiaotuoguan2.png","quxiaotuoguan1.png","quxiaotuoguan2.png",UI_TEX_TYPE_PLIST);
			TuoGuanCancel->addTouchEventListener(CC_CALLBACK_2(NetRaceLayer::BtnTuoGuanCancelHandler,this));
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
            
			if(myframe->getChildByTag(QI_REMIND_ACT_TAG_ID) && _roundManager->_isWaitDecision) {
				_DeleteActionTip();
				_roundManager->_isWaitDecision=false;
				_roundManager->_tempActionToDo=a_JUMP;
                
				if(_roundManager->_isCardFromOthers) {
					if(_roundManager->_isQiangGangAsking) {
						_roundManager->_isQiangGangAsking=false;
                        
						myframe->runAction(Sequence::create(CallFunc::create([=](){
							GoldNumInsert(_roundManager->_qiangGangTargetNo,2,_roundManager->_curPlayer);
							_roundManager->_qiangGangTargetNo=INVALID;}),CallFunc::create([=](){
                            _roundManager->DistributeTo((PlayerDir_t)_roundManager->_curPlayer);}),NULL));
					} else if(_roundManager->_isDoubleHuAsking) {
						_roundManager->_isDoubleHuAsking = false;

						myframe->runAction(CallFunc::create([=](){
                            PlayerDir_t giver = (_roundManager->_otherOneForDouble==LEFT)?RIGHT:LEFT;
                            _roundManager->SetWin(DOUBLE_WIN,giver);
                            
                            WinInfo_t win;
                            _roundManager->GetWin(win);
                            _HuEffect(win);}));
					} else {
						myframe->runAction(CallFunc::create([=](){
                            _roundManager->DistributeTo(_roundManager->TurnToNext());}));
					}
				} else {
					_roundManager->WaitForMyChoose();
                }
			} else {
				if(_roundManager->_isMyShowTime) {
					_roundManager->_isMyShowTime=false;
					_roundManager->_actionToDo=a_JUMP;

					if(_roundManager->_lastAction==a_JUMP)
						_roundManager->_continue_gang_times=0;

					_roundManager->_lastAction=a_JUMP;

					_roundManager->WaitForMyChoose();
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

void NetRaceLayer::BtnBackConfirmHandler(Ref* pSender,ui::Widget::TouchEventType type)
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

void NetRaceLayer::BtnBackCancelHandler(Ref* pSender,ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			
			auto backBUtton=(Button*)this->getChildByTag(MENU_BKG_TAG_ID)->getChildByTag(GAMEBACK_MENU_BUTTON);
			backBUtton->setTouchEnabled(true);
			auto curButton=(Button*)pSender;
			curButton->setTouchEnabled(false);
			(this->getChildByTag(GAME_BACK_BAR))->runAction(ScaleTo::create(0,0));

			auto StartSelfGame=CallFunc::create([=](){
				if(_roundManager->_isGameStart)
				{
					if(!_roundManager->IsTing(1))
						ListenToCardTouch();
					else
						ListenToTingButton();
				}
			});
			if(this->getChildByTag(TUOGUAN_CANCEL_BUTTON))
				((Button*)this->getChildByTag(TUOGUAN_CANCEL_BUTTON))->setTouchEnabled(true);
			this->runAction(Sequence::create(
                CallFunc::create([=](){this->removeChildByTag(GAME_BACK_BAR,true);}),
                StartSelfGame,NULL));
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void NetRaceLayer::BtnBackHandler(Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
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
	ensureBut->addTouchEventListener(CC_CALLBACK_2(NetRaceLayer::BtnBackConfirmHandler,this));
	ensureBut->setAnchorPoint(Vec2(0.5,0.5));
	ensureBut->setPosition(Vec2(backChooseBar->getContentSize().width*0.75-15,backChooseBar->getContentSize().height*0.3));
	backChooseBar->addChild(ensureBut);

	auto BackBut=Button::create("jixuyouxi2.png","jixuyouxi.png","jixuyouxi.png",UI_TEX_TYPE_PLIST);//继续游戏
	BackBut->addTouchEventListener(CC_CALLBACK_2(NetRaceLayer::BtnBackCancelHandler,this));
	BackBut->setAnchorPoint(Vec2(0.5,0.5));
	BackBut->setPosition(Vec2(backChooseBar->getContentSize().width*0.25+15,backChooseBar->getContentSize().height*0.3));
	backChooseBar->addChild(BackBut);
}

/***********************************************
        general support
***********************************************/
Vec2 NetRaceLayer::GetCardPositionInHand(int idx) {
    return _GetCardInHand(MIDDLE,idx)->getPosition();
}

Spawn* NetRaceLayer::simple_tip_effect(Vec2 v,std::string act_name)
{
    auto bgOfGang=Sprite::createWithSpriteFrameName("mojixx.png");
    bgOfGang->setPosition(v);
    myframe->addChild(bgOfGang,20,MOJI_EFFECT_TAG_ID);
    bgOfGang->setScale(0);

    auto tipOfPeng=Sprite::createWithSpriteFrameName(act_name);
    tipOfPeng->setPosition(Vec2(bgOfGang->getTextureRect().size.width/2,bgOfGang->getTextureRect().size.height/2));
    bgOfGang->addChild(tipOfPeng);
    tipOfPeng->setScale(0);
    
    auto action1 = TargetedAction::create(
        bgOfGang,Sequence::create(
            ScaleTo::create(0,1),
            DelayTime::create(0.6),
            Spawn::create(
                ScaleTo::create(0.18,0.8),
                FadeOut::create(0.18),NULL),NULL));
    auto action2 = TargetedAction::create(
        tipOfPeng,Sequence::create(
            ScaleTo::create(0,1.5),
            DelayTime::create(0.12),
            EaseElasticOut::create(ScaleTo::create(0.18,1)),
            DelayTime::create(0.3),
            Spawn::create(
                ScaleTo::create(0.18,0.8),
                FadeOut::create(0.18),NULL),NULL));

    auto bgOfGang1=Sprite::createWithSpriteFrameName("mojixx.png");
    bgOfGang1->setPosition(v);
    myframe->addChild(bgOfGang1,21,MOJI1_EFFECT_TAG_ID);
    
    auto tipOfPeng1=Sprite::createWithSpriteFrameName(act_name);
    tipOfPeng1->setPosition(Vec2(bgOfGang1->getTextureRect().size.width/2,bgOfGang1->getTextureRect().size.height/2));
    bgOfGang1->addChild(tipOfPeng1);
    bgOfGang1->setScale(0);
    
    auto action3 = TargetedAction::create(
        bgOfGang1,Sequence::create(
            DelayTime::create(0.12),
            ScaleTo::create(0,1),
            Spawn::create(
                ScaleTo::create(0.18,1.5),
                FadeOut::create(0.18),NULL),NULL));
    auto action4 = TargetedAction::create(
        tipOfPeng1,Sequence::create(
            DelayTime::create(0.12),
            ScaleTo::create(0,1),
            Spawn::create(
                ScaleTo::create(0.18,1.5),
                FadeOut::create(0.18),NULL),NULL));
    
    return Spawn::create(action1,action2,action3,action4,NULL);
}


void NetRaceLayer::_DistributeEvent(const std::string event_type,void* val) {
    LOGGER_WRITE("%s : %s",__FUNCTION__,event_type.c_str());
	_eventDispatcher->dispatchCustomEvent(event_type,val);
}

void NetRaceLayer::_DeleteStartDealCards() {
	for(int i=0;i<3;i++) {
		for(int j=0;j<4;j++) {
            _Remove(myframe,START_CARDS_IN_TAG_ID+i*j);
		}
	}
}

void NetRaceLayer::_DeleteActionReminder()
{
    LOGGER_WRITE("%s",__FUNCTION__);
	for(int i=0;i<17;i++){
        _Remove(myframe,REMIND_ACT_TAG_ID+i);
	}
}

void NetRaceLayer::_DeleteActionEffect()
{
    LOGGER_WRITE("%s",__FUNCTION__);
 	for(int i=0;i<31;i++) {
        _Remove(myframe,MOJI_EFFECT_TAG_ID+i);
	}
}

void NetRaceLayer::_DeleteActionTip()
{
	_DeleteActionReminder();
    _DeleteActionEffect();    
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

Spawn *NetRaceLayer::_CreateHuBackgroundEffect(PlayerDir_t winner) {
    auto centerImage = Sprite::createWithSpriteFrameName("101.png");
    centerImage->setPosition(Vec2(visibleSize.width/2,visibleSize.height/3));
    myframe->addChild(centerImage,30,IMG_101_EFFECT_TAG_ID);
    centerImage->setOpacity(200);
    centerImage->setScale(0);
    auto animation = Animation::create();
    animation->addSpriteFrameWithFile("101.png");
    animation->addSpriteFrameWithFile("103.png");
    animation->addSpriteFrameWithFile("105.png");
    animation->addSpriteFrameWithFile("106.png");
    animation->setDelayPerUnit(0.05f);
    animation->setRestoreOriginalFrame(true);
    auto ll_action1 = TargetedAction::create(centerImage,Sequence::create(
        DelayTime::create(0.3),
        ScaleTo::create(0,1),
        Spawn::create(
            simple_tip_effect(_layout->PositionOfActSign(winner),"dahu.png"),
            Repeat::create(Animate::create(
                animation),3),NULL),
        FadeOut::create(0.1),NULL));
    
    auto lightCircle=Sprite::createWithSpriteFrameName("4.png");
    lightCircle->setPosition(Vec2(visibleSize.width/2,visibleSize.height/3));
    myframe->addChild(lightCircle,29,IMG_4_EFFECT_TAG_ID);
    lightCircle->setRotation(90);
    lightCircle->setScaleX(0);
    lightCircle->setScaleY(0);
    lightCircle->setOpacity(150);
    BlendFunc tmp_oBlendFunc ={GL_SRC_ALPHA, GL_ONE};
    lightCircle->setBlendFunc(tmp_oBlendFunc);
    auto ll_action2 = TargetedAction::create(
        lightCircle,Sequence::create(
            DelayTime::create(0.3),
            ScaleTo::create(0,0.2,0.6),
            Spawn::create(
                EaseSineOut::create(
                    ScaleTo::create(0.5,0.4,4.0)),
                EaseSineIn::create(
                    FadeOut::create(0.5)),NULL),NULL));
    
    auto lightCircle2=Sprite::createWithSpriteFrameName("Q13.png");
    lightCircle2->setAnchorPoint(Vec2(0.5,0));
    lightCircle2->setOpacity(180);
    lightCircle2->setPosition(Vec2(visibleSize.width/2,visibleSize.height/3-20));
    myframe->addChild(lightCircle2,30,IMG_Q13_EFFECT_TAG_ID);
    lightCircle2->setScale(0);
    auto ll_action3=TargetedAction::create(
        lightCircle2,Sequence::create(
            DelayTime::create(0.3),
            ScaleTo::create(0,0.3),
            Spawn::create(
                EaseSineOut::create(
                    ScaleTo::create(0.5,1.5)),
                FadeOut::create(0.5),NULL),NULL));
    
    auto lightCircle3=Sprite::createWithSpriteFrameName("Q13.png");
    lightCircle3->setAnchorPoint(Vec2(0.5,0));
    lightCircle2->setOpacity(180);
    lightCircle3->setPosition(Vec2(visibleSize.width/2,visibleSize.height/3-20));
    myframe->addChild(lightCircle3,30,IMG_Q13_2_EFFECT_TAG_ID);
    lightCircle3->setScale(0);
    auto ll_action4=TargetedAction::create(
        lightCircle3,Sequence::create(
            DelayTime::create(0.5),
            ScaleTo::create(0,0.26),
            Spawn::create(
                EaseSineOut::create(
                    ScaleTo::create(0.5,1.5)),
                FadeOut::create(0.5),NULL),NULL));
    
    auto lightCircle4=Sprite::createWithSpriteFrameName("888.png");
    lightCircle4->setPosition(Vec2(visibleSize.width/2,visibleSize.height/3));
    lightCircle4->setScale(0);
    myframe->addChild(lightCircle4,29,IMG_888_EFFECT_TAG_ID);
    lightCircle4->setBlendFunc(tmp_oBlendFunc);
    auto ll_action5=TargetedAction::create(
        lightCircle4,Sequence::create(
            DelayTime::create(0.3),
            ScaleTo::create(0,1),
            Spawn::create(
                EaseSineOut::create(
                    ScaleTo::create(0.3,5.5)),
                FadeOut::create(0.3),NULL),NULL));
    
    auto star=Sprite::createWithSpriteFrameName("104.png");
    star->setAnchorPoint(Vec2(0.5,0));
    star->setScale(0);
    star->setPosition(Vec2(visibleSize.width/2,visibleSize.height/3));
    myframe->addChild(star,29,IMG_104_EFFECT_TAG_ID);
    auto ll_action6=TargetedAction::create(
        star,Sequence::create(
            DelayTime::create(0.3),
            Spawn::create(
                EaseSineOut::create(
                    ScaleTo::create(0.5,0.7)),
                FadeOut::create(0.5),NULL),NULL));
    
    auto flame=Sprite::createWithSpriteFrameName("444.png");
    flame->setAnchorPoint(Vec2(0,0.5));
    flame->setRotation(-90);
    flame->setPosition(Vec2(visibleSize.width/2,visibleSize.height/3));
    myframe->addChild(flame,29,IMG_444_EFFECT_TAG_ID);
    flame->setBlendFunc(tmp_oBlendFunc);
    flame->setScale(0);
    auto ll_action7=TargetedAction::create(
        flame,Sequence::create(
            DelayTime::create(0.3),
            Spawn::create(
                EaseSineOut::create(
                    ScaleTo::create(0.5,1.0)),
                FadeOut::create(0.5),NULL),NULL));
    
    auto firecircle=Sprite::createWithSpriteFrameName("666.png");
    firecircle->setPosition(Vec2(visibleSize.width/2,visibleSize.height/3));
    firecircle->setTag(4);
    firecircle->setScale(0);
    firecircle->setOpacity(200);
    myframe->addChild(firecircle,29,IMG_666_EFFECT_TAG_ID);
    firecircle->setOpacity(100);
    firecircle->setBlendFunc(tmp_oBlendFunc);
    auto ll_action8=TargetedAction::create(
        firecircle,Sequence::create(
            DelayTime::create(0.3),
            ScaleTo::create(0,0.1),
            Spawn::create(
                EaseSineOut::create(
                    ScaleTo::create(0.5,1.0)),
                FadeOut::create(0.5),NULL),NULL));
    
    auto ll_action9=Sequence::create(DelayTime::create(0.3),CCCallFunc::create([=](){
        _StartParticleSystem(0.3);  
    }),NULL);
    
    auto sorPos=this->getPosition();
    auto ll_action10=TargetedAction::create(
        this,Sequence::create(
            DelayTime::create(0.3),
            MoveTo::create(0.1,Vec2(sorPos.x+10,sorPos.y+10)),
            MoveTo::create(0.1,Vec2(sorPos.x,sorPos.y)),
            MoveTo::create(0.1,Vec2(sorPos.x-10,sorPos.y-10)),
            MoveTo::create(0.1,Vec2(sorPos.x,sorPos.y)),
            MoveTo::create(0.1,Vec2(sorPos.x+10,sorPos.y)),
            MoveTo::create(0.1,Vec2(sorPos.x,sorPos.y)),
            MoveTo::create(0.1,Vec2(sorPos.x,sorPos.y+10)),
            MoveTo::create(0.1,Vec2(sorPos.x,sorPos.y)),NULL));
    
    return Spawn::create(
        ll_action1,ll_action2,ll_action3,ll_action4,ll_action5,
        ll_action6,ll_action7,ll_action8,ll_action9,ll_action10,NULL);
}


/***********************************************
        
***********************************************/
BezierTo* NetRaceLayer::BizerMove1(outCardList* outCard,Vec2 location)
{
	ccBezierConfig config;

    _layout->GetBizerPoints(config, outCard->length-1, location);

	BezierTo *action;
	if(ccpDistance(location,config.endPosition)<=200){
		action=BezierTo::create(0.18,config);
	} else {
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
			config.endPosition=Vec2(_layout->_playerPosi[1].riverPoint.x+((outCard->length-1)*36),_layout->_playerPosi[1].riverPoint.y);
			config.controlPoint_1=Vec2(location.x,200);
			config.controlPoint_2=Vec2(_layout->_playerPosi[1].riverPoint.x+((outCard->length-1)*36)-100,_layout->_playerPosi[1].riverPoint.y-100);
		}
		else if(location.x>=visibleSize.width*0.6)
		{
			config.endPosition=Vec2(_layout->_playerPosi[1].riverPoint.x+((outCard->length-1)*36),_layout->_playerPosi[1].riverPoint.y);
			config.controlPoint_1=Vec2(location.x,200);
			config.controlPoint_2=Vec2(_layout->_playerPosi[1].riverPoint.x+((outCard->length-1)*36)+100,_layout->_playerPosi[1].riverPoint.y-100);
		}
		else if(location.x>visibleSize.width*0.4 && location.x<visibleSize.width*0.6)
		{
			config.endPosition=Vec2(_layout->_playerPosi[1].riverPoint.x+((outCard->length-1)*36),_layout->_playerPosi[1].riverPoint.y);
			config.controlPoint_1=Vec2(location.x,200);
			config.controlPoint_2=Vec2(_layout->_playerPosi[1].riverPoint.x+((outCard->length-1)*36),_layout->_playerPosi[1].riverPoint.y);
		}
	}
	else if((outCard->length-1)<14)
	{
		if(location.x<visibleSize.width*0.4)
		{
			config.endPosition=Vec2(_layout->_playerPosi[1].riverPoint.x+((outCard->length-8)*36),_layout->_playerPosi[1].riverPoint.y-41);
			if(time==2)
				config.controlPoint_1=Vec2(location.x,200);
			else
				config.controlPoint_1=Vec2(location.x+30,200);
			config.controlPoint_2=Vec2(_layout->_playerPosi[1].riverPoint.x+((outCard->length-8)*36)-100,_layout->_playerPosi[1].riverPoint.y-141);
		}
		else if(location.x>=visibleSize.width*0.6)
		{
			config.endPosition=Vec2(_layout->_playerPosi[1].riverPoint.x+((outCard->length-8)*36),_layout->_playerPosi[1].riverPoint.y-41);
			config.controlPoint_1=Vec2(location.x,200);
			config.controlPoint_2=Vec2(_layout->_playerPosi[1].riverPoint.x+((outCard->length-8)*36)+100,_layout->_playerPosi[1].riverPoint.y-141);
		}
		else if(location.x>visibleSize.width*0.4 && location.x<visibleSize.width*0.6)
		{
			config.endPosition=Vec2(_layout->_playerPosi[1].riverPoint.x+((outCard->length-8)*36),_layout->_playerPosi[1].riverPoint.y-41);
			config.controlPoint_1=Vec2(location.x,200);
			config.controlPoint_2=Vec2(_layout->_playerPosi[1].riverPoint.x+((outCard->length-8)*36),_layout->_playerPosi[1].riverPoint.y-41);
		}
	}
	else
	{
		if(location.x<visibleSize.width*0.4)
		{
			config.endPosition=Vec2(_layout->_playerPosi[1].riverPoint.x+((outCard->length-1)-16)*36,_layout->_playerPosi[1].riverPoint.y-82);
			if(time==2)
				config.controlPoint_1=Vec2(location.x,200);
			else
				config.controlPoint_1=Vec2(location.x+30,200);
			config.controlPoint_2=Vec2(_layout->_playerPosi[1].riverPoint.x+((outCard->length-16)*36)-100,_layout->_playerPosi[1].riverPoint.y-182);
		}
		else if(location.x>=visibleSize.width*0.6)
		{
			config.endPosition=Vec2(_layout->_playerPosi[1].riverPoint.x+((outCard->length-1)-16)*36,_layout->_playerPosi[1].riverPoint.y-82);
			config.controlPoint_1=Vec2(location.x,200);
			config.controlPoint_2=Vec2(_layout->_playerPosi[1].riverPoint.x+((outCard->length-16)*36)+100,_layout->_playerPosi[1].riverPoint.y-182);
		}
		else if(location.x>visibleSize.width*0.4 && location.x<visibleSize.width*0.6)
		{
			config.endPosition=Vec2(_layout->_playerPosi[1].riverPoint.x+((outCard->length-1)-16)*36,_layout->_playerPosi[1].riverPoint.y-82);
			config.controlPoint_1=Vec2(location.x,200);
			config.controlPoint_2=Vec2(_layout->_playerPosi[1].riverPoint.x+((outCard->length-16)*36),_layout->_playerPosi[1].riverPoint.y-82);
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
    
	if(no==2)
	{
		int lenForPlayerTwo=_roundManager->_players[2]->get_parter()->get_card_list()->len;
		auto BizerPosForPlayerTwoTemp=myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+no*20+lenForPlayerTwo-1)->getPosition();
		auto BizerPosForPlayerTwo=Vec2(BizerPosForPlayerTwoTemp.x,BizerPosForPlayerTwoTemp.y);
		if((outCard->length-1)<6)
		{
			config.controlPoint_1=Vec2(BizerPosForPlayerTwo.x-100,BizerPosForPlayerTwo.y);
			config.controlPoint_2=Vec2(_layout->_playerPosi[no].riverPoint.x,_layout->_playerPosi[no].riverPoint.y+30*(outCard->length-1));
			config.endPosition=Vec2(_layout->_playerPosi[no].riverPoint.x,_layout->_playerPosi[no].riverPoint.y+30*(outCard->length-1));
		}
		else if((outCard->length-1)<14)
		{
			config.controlPoint_1=Vec2(BizerPosForPlayerTwo.x-100,BizerPosForPlayerTwo.y);
			config.controlPoint_2=Vec2(_layout->_playerPosi[no].riverPoint.x+48,_layout->_playerPosi[no].riverPoint.y+30*((outCard->length-1)-7));
			config.endPosition=Vec2(_layout->_playerPosi[no].riverPoint.x+48,_layout->_playerPosi[no].riverPoint.y+30*((outCard->length-1)-7));
		}
		else
		{
			config.controlPoint_1=Vec2(BizerPosForPlayerTwo.x-100,BizerPosForPlayerTwo.y);
			config.controlPoint_2=Vec2(_layout->_playerPosi[no].riverPoint.x+96,_layout->_playerPosi[no].riverPoint.y+30*((outCard->length-1)-16));
			config.endPosition=Vec2(_layout->_playerPosi[no].riverPoint.x+96,_layout->_playerPosi[no].riverPoint.y+30*((outCard->length-1)-16));
		}
	}
	else if(no==0)
	{
		auto l_list_len=_roundManager->_players[no]->get_parter()->get_card_list()->len-1;
		auto l_card=myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+no*20+l_list_len);
		if((outCard->length-1)<6)
		{
			config.controlPoint_1=Vec2(_layout->_playerPosi[no].basePoint.x+110,l_card->getPosition().y-35);
			config.controlPoint_2=Vec2(_layout->_playerPosi[no].riverPoint.x,_layout->_playerPosi[no].riverPoint.y-30*(outCard->length-1));
			config.endPosition=Vec2(_layout->_playerPosi[no].riverPoint.x,_layout->_playerPosi[no].riverPoint.y-30*(outCard->length-1));
		}
		else if((outCard->length-1)<14)
		{
			config.controlPoint_1=Vec2(_layout->_playerPosi[no].basePoint.x+110,l_card->getPosition().y-35);
			config.controlPoint_2=Vec2(_layout->_playerPosi[no].riverPoint.x-48,_layout->_playerPosi[no].riverPoint.y-30*((outCard->length-1)-7));
			config.endPosition=Vec2(_layout->_playerPosi[no].riverPoint.x-48,_layout->_playerPosi[no].riverPoint.y-30*((outCard->length-1)-7));
		}
		else
		{
			config.controlPoint_1=Vec2(_layout->_playerPosi[no].basePoint.x+110,l_card->getPosition().y-35);
			config.controlPoint_2=Vec2(_layout->_playerPosi[no].riverPoint.x-96,_layout->_playerPosi[no].riverPoint.y-30*((outCard->length-1)-16));
			config.endPosition=Vec2(_layout->_playerPosi[no].riverPoint.x-96,_layout->_playerPosi[no].riverPoint.y-30*((outCard->length-1)-16));
		}
	}
	auto action=BezierTo::create(0.3,config);
	return action;
}

