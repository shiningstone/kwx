
#include "RaceLayer.h"
#include "./../HelloWorldScene.h"
#include "RoundManager.h"
#include "NetRoundManager.h"
#include "StrategyRm.h"
#include "Player.h"

#include "./../utils/DebugCtrl.h"

using namespace cocos2d::ui;
USING_NS_CC;

/************************************************
                init 
************************************************/
RaceLayer::RaceLayer()
:PREMIUM_LEAST(200)
{
	s_scale = 1.189;
	s_no    = 1;

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

    _object = GObjectFactory::getInstance(origin,visibleSize);
    _voice = VoiceEffect::getInstance();
    _layout = GameLayout::getInstance(origin,visibleSize);
    _effect = GraphicEffect::getInstance(_voice,_object);
    
    _logger = LOGGER_REGISTER("RaceLayer");
}

RaceLayer::~RaceLayer()
{
	_eventDispatcher->removeAllEventListeners();
#if 0
    _voice->destroyInstance();
    _layout->destroyInstance();
    _object->destroyInstance();
    _effect->destroyInstance();
    _roundManager->destroyInstance();
#endif
    LOGGER_DEREGISTER(_logger);
}

bool RaceLayer::init() {
	if(!Layer::init()) {
		return false;
	}
	srand(time(0));
	
	return true;
}

/************************************************
            main process
************************************************/
void RaceLayer::CreateRace(GameMode_t mode)
{
    if(mode==LOCAL_GAME) {
        _roundManager = RoundManager::getInstance();
    } else {
        _roundManager = NetRoundManager::getInstance();
    }
    _roundManager->CreateRace(this);

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
		_UpdateHeadImage(dir,_roundManager->_players[dir]->_profile.photo);
		_UpdateNickName(dir,_roundManager->_players[dir]->_profile.name);
		GuiUpdateScore(dir,_roundManager->_players[dir]->_profile.property);
	}

    for(int i=0;i<4;i++) {
        auto mapai = _object->CreateMaPai(i);
        this->addChild(mapai,2,CARD_ARRAGE_LEFT_TAG_ID + i);
    }

    auto StartButton = _object->CreateButton(BTN_START);
    StartButton->addTouchEventListener(CC_CALLBACK_2(RaceLayer::BtnStartHandler,this));
    this->addChild(StartButton,2,START_GAME_TAG_ID);
}

void RaceLayer::StartGame()
{
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("BlockOtherImage.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("gametrayImage.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("MingTips.plist");
    /*!!! maybe lagging ???*/
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("race3.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("race4.plist");

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
    
	myframe = LayerColor::create(Color4B(0,0,0,0),visibleSize.width,visibleSize.height);
	myframe->setPosition(Vec2(origin.x, origin.y));
	this->addChild(myframe,3,GAME_BKG_TAG_ID);

	distributeCardPos=Vec2::ZERO;
	ifUpdateDuringEffect=false;
	_isCardInHandUpdated=false;
	ifInsertCardsTime=false;
	ifEffectTime=false;
    _myChosenCard = INVALID;
    _myTouchedCard = INVALID;
    
	ifPengAction=false;
	ifGangAction=false;

	_RaceBeginPrepare();

    _roundManager->StartGame();
}

void RaceLayer::WaitForFirstAction(PlayerDir_t zhuang)
{
    ListenToCardTouch();
    ((Button*)this->getChildByTag(MENU_BKG_TAG_ID)->getChildByTag(TUOGUAN_MENU_BUTTON))->setTouchEnabled(true);

    _roundManager->WaitForFirstAction(zhuang);
}

void RaceLayer::OthersHandoutEffect(PlayerDir_t dir,bool canKou) {
    myframe->_ID = dir;
    
	myframe->runAction(Sequence::create(
        _OthersShowCardEffect(dir,_roundManager->LastHandout(),canKou),CCCallFunc::create([=]() {
		_CardRiverUpdateEffect(dir);}), CCCallFunc::create([=]() {
        _roundManager->UpdateCards(dir,a_JUMP);}),CCCallFunc::create([=]() {
        _CardInHandUpdateEffect(dir);}), CCCallFunc::create([=]() {
        _roundManager->_isNewDistributed=false;
		_roundManager->WaitForResponse(dir);}),NULL));
}

/***********************************************
        effect entrances
***********************************************/
void RaceLayer::QiEffect() {
    _effect->Hide(myframe,QI_REMIND_ACT_TAG_ID);

    _QiEffect(MIDDLE);
}

void RaceLayer::PengEffect(PlayerDir_t dir, PlayerDir_t prevDir, Card_t card) {
    _effect->Hide(myframe,PENG_REMIND_ACT_TAG_ID);
    
    _PengEffect(dir,prevDir,card);
}

void RaceLayer::HuEffect(const WinInfo_t &win,bool qiangGang) {
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

void RaceLayer::GangEffect(PlayerDir_t winner,Card_t card, int gangCardIdx[], bool isAnGang, PlayerDir_t prevPlayer) {
    _effect->Hide(myframe,GANG_REMING_ACT_TAG_ID);
    
    if(isAnGang) {
        _AnGangEffect(winner,card,gangCardIdx);
    } else {
        _MingGangEffect(winner,prevPlayer,(Card_t)card,gangCardIdx);
    }
}

void RaceLayer::DoubleWin(const WinInfo_t &win) {
    HideClock();

    if(win.giver!=MIDDLE) {
        if(_roundManager->IsMing(MIDDLE)) {
            _HuEffect(win);
            _DistributeEvent(DOUBLE_HU_WITH_ME,NULL);
        } else {
            _roundManager->WaitForMyAction();
        }
    } else {
        _HuEffect(win);
        _DistributeEvent(DOUBLE_HU_WITH_ME,NULL);
    }
}

void RaceLayer::SingleWin(const WinInfo_t &win) {
    HideClock();

    if(win.winner==MIDDLE) {
        if(_roundManager->IsMing(MIDDLE)) {
            _HuEffect(win);
        } else {
            _roundManager->WaitForMyAction();
        }

    } else {
        _HuEffect(win);
    }
}

void RaceLayer::GangGoldEffect(int winner,int whoGive) {
    myframe->runAction(Sequence::create(CallFunc::create([=](){
        _roundManager->update_gold((PlayerDir_t)winner,MING_GANG,(PlayerDir_t)whoGive);}),CallFunc::create([=](){
        _roundManager->DistributeTo((PlayerDir_t)winner
            ,(Card_t)(_roundManager->_unDistributedCards[_roundManager->_distributedNum++]));}),NULL));
}

void RaceLayer::MyHandoutEffect(int chosenCard,Vec2 touch,int time,bool turnToMing)
{
    if(time==2) {
        if(myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+MIDDLE*20+chosenCard)) {
            myframe->removeChildByTag(HAND_IN_CARDS_TAG_ID+MIDDLE*20+chosenCard);
		}
	} else {
		//_roundManager->_players[MIDDLE]->_cards->add_effect_card();
		_ReOrderCardsInHand(chosenCard,_roundManager->_players[MIDDLE]->_cards);
        //_roundManager->_players[MIDDLE]->_cards->del_effect_card();
    }

    if(_myTouchedCard!=INVALID) {
        _roundManager->_players[MIDDLE]->hand_out(_myTouchedCard);
		_myTouchedCard = INVALID;
    } else {
        int last = _roundManager->_players[MIDDLE]->_cards->last();
        _roundManager->_players[MIDDLE]->hand_out(last);
    }
	_roundManager->UpdateCards(MIDDLE,a_JUMP);

    //CardNode_t *node = _roundManager->_players[MIDDLE]->_cards->back();
    //_roundManager->_players[MIDDLE]->_cards->insert_card(*node,1);
    //_roundManager->_players[MIDDLE]->_cards->pop_back();

	_MyHandoutEffect(_roundManager->LastHandout(),touch,time,turnToMing);
}

void RaceLayer::_CardInHandUpdateEffect(PlayerDir_t dir)
{
	if(_roundManager->IsMing(dir)) {
        _Show(this,MING_STATUS_PNG_0+dir,true);
	}
    
	for(int i=0;i<MAX_HANDIN_NUM;i++) {
        _Remove(myframe,HAND_IN_CARDS_TAG_ID+dir*20+i);
	}
	float x = _layout->_playerPosi[dir].basePoint.x+10; 
	float y = _layout->_playerPosi[dir].basePoint.y+10;

    CardInHand *cards = _roundManager->_players[dir]->_cards;

	if( _roundManager->IsMing(dir) && _roundManager->_firstMingNo==INVALID )
		_roundManager->_firstMingNo=dir;

	bool isMing = _roundManager->_players[dir]->_cards->IsMing;

	Sprite *p_list[MAX_HANDIN_NUM];
    
    for(int i=0;i<cards->size();i++) {
		//if(list->data[i].kind!=ck_NOT_DEFINED )
		{
			if(dir==LEFT || dir==RIGHT) {
				p_list[i] = _CreateCardInHand(dir,i,cards,isMing,Vec2(x,y));
                
				CartApperance_t apperance = cards->get_appearance(i,dir,_roundManager->IsMing(MIDDLE));
                if(apperance!=NORMAL_APPERANCE) {
                    _object->LayDownWithFace((PlayerDir_t)dir, p_list[i], cards->get_kind(i),apperance);
                }

                y += _YofNextCard(dir,i,cards,isMing,p_list[i]->getTextureRect().size.height);
			}
			else if(dir==MIDDLE)
			{
				if(cards->get_status(i)==c_FREE||cards->get_status(i)==c_MING_KOU)
				{
					if(isMing)
						p_list[i]=_object->Create(MING_CARD,(PlayerDir_t)dir,x,y);
					else
						p_list[i]=_object->Create(FREE_CARD,(PlayerDir_t)dir,x,y);
				}
				else if(cards->get_status(i)==c_PENG||cards->get_status(i)==c_MING_GANG)
				{
					p_list[i]=_object->Create(PENG_CARD,(PlayerDir_t)dir,x,y);
				}
				else if(cards->get_status(i)==c_AN_GANG)
				{
					if(cards->get_idx_in_group(i)==3)
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
				if(cards->is_wait_handout() && i==cards->last()-1)
					x+=30;

                /* the card showing somewhere else */
                if( _roundManager->IsCurEffectCard(cards->at(i)) )
					p_list[i]->setVisible(false);


                if(cards->get_status(i)==c_FREE) {
					if(isMing) {
					    _object->LayDownWithFace(p_list[i], cards->get_kind(i), 0.6);
					} else {
					    _object->LayDownWithFace(p_list[i], cards->get_kind(i), 0.4);
					}
                    
					if(isMing||(_roundManager->_actCtrl.decision==aMING&&!isMing))
					{
						if(cards->canPlay(i))
						{
							auto MingCps_yesMask=_object->Create(MING_BKG);
							MingCps_yesMask->setAnchorPoint(Vec2(0.5,0.5));
							MingCps_yesMask->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,
                                p_list[i]->getTextureRect().size.height/2));
							p_list[i]->addChild(MingCps_yesMask,2,MING_KOU_MASK);
						}
						else if(!cards->canPlay(i))
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
				else if(cards->get_status(i)==c_MING_KOU)
				{
					if(!isMing)
					{
						auto KouSign=_object->Create(MING_KOU_CARD);
						KouSign->setAnchorPoint(Vec2(0.5,0.5));
						KouSign->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,
                            p_list[i]->getTextureRect().size.height/2));
						p_list[i]->addChild(KouSign,2);

					    _object->LayDownWithFace(p_list[i], cards->get_kind(i), 0.4);
                        
						x += p_list[i]->getTextureRect().size.width*1;
					}
					else
					{
					    _object->LayDownWithFace(p_list[i], cards->get_kind(i), 0.6);

                        x += p_list[i]->getTextureRect().size.width*1;
					}
				}
				else if(cards->get_status(i)==c_PENG)
				{
					auto s_card=_object->CreateKind(cards->get_kind(i),MIDDLE_SIZE);
					s_card->setAnchorPoint(Vec2(0.5,0.5));
					s_card->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height*0.6));
					p_list[i]->addChild(s_card,1);
                    
					if(cards->get_status(i+1)==c_FREE)
						x += p_list[i]->getTextureRect().size.width*2;
					else if(cards->get_kind(i)!=cards->get_kind(i+1) &&(cards->get_status(i+1)))
						x += p_list[i]->getTextureRect().size.width*1.5;
					else
						x += p_list[i]->getTextureRect().size.width*1.0;
				}
				else if(cards->get_status(i)==c_MING_GANG)
				{
					auto s_card=_object->CreateKind((Card_t)cards->get_kind(i),MIDDLE_SIZE);
					s_card->setAnchorPoint(Vec2(0.5,0.5));
					s_card->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height*0.6));
					p_list[i]->addChild(s_card,1);

                    /*need confirm*/
                    int idxInGroup = cards->get_idx_in_group(i);
					if(idxInGroup==1)
						x+=p_list[i]->getTextureRect().size.width*1.0;
					else if(idxInGroup==2)
						y+=17;
					else if(idxInGroup==3) {
						x+=p_list[i]->getTextureRect().size.width*1.0;
						y-=17;
					}
					else {
						if(cards->get_status(i+1)==c_FREE)
							x += p_list[i]->getTextureRect().size.width*2;
						else if(cards->get_kind(i)!=cards->get_kind(i+1) &&(cards->get_status(i+1)!=c_FREE))
							x += p_list[i]->getTextureRect().size.width*1.5;
						else
							x += p_list[i]->getTextureRect().size.width*1.0;
					}
				}
				else if(cards->get_status(i)==c_AN_GANG)
				{
                    int idxInGroup = cards->get_idx_in_group(i);

					if(idxInGroup==1)
						x+=p_list[i]->getTextureRect().size.width*1.0;
					else if(idxInGroup==2)
						y+=17;
					else if(idxInGroup==3) {
						auto s_card=_object->CreateKind((Card_t)cards->get_kind(i),MIDDLE_SIZE);
						s_card->setAnchorPoint(Vec2(0.5,0.5));
						s_card->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height*0.6));
						p_list[i]->addChild(s_card,1);
						x+=p_list[i]->getTextureRect().size.width*1.0;
						y-=17;
					}
					else {
						if(cards->get_status(i+1)==c_FREE)
							x += p_list[i]->getTextureRect().size.width*2;
						else if(cards->get_kind(i)!=cards->get_kind(i+1) &&(cards->get_status(i+1)!=c_FREE))
							x += p_list[i]->getTextureRect().size.width*1.5;
						else
							x += p_list[i]->getTextureRect().size.width*1.0;
					}
				}
			}



			if(dir!=RIGHT) {//each one lay above the previous ones
                if(!_GetCardInHand((PlayerDir_t)dir,i)) {
                    _Add(myframe,p_list[i],HAND_IN_CARDS_TAG_ID+dir*20+i,i+1);
                }
            } else {//each one lay below the previous ones
                if(!_GetCardInHand((PlayerDir_t)dir,i)) {
                    _Add(myframe,p_list[i],HAND_IN_CARDS_TAG_ID+dir*20+i,cards->size()-i);
                }
            }
            
			if(dir==MIDDLE
                &&_roundManager->_actCtrl.decision==aMING
                &&!_roundManager->_players[MIDDLE]->_cards->IsMing
                &&cards->canPlay(i)) {
				p_list[i]->setZOrder(30);
            }
            
			p_list[i]->_ID=MIDDLE;

            if(dir==MIDDLE && _myChosenCard==i)
				p_list[i]->_ID=100;
		}//for each card
	}
    
	if(dir==MIDDLE&&isMing)
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
    
	if(dir==MIDDLE&&myframe->getChildByTag(TING_SING_BUTTON)) {
		_UpdateTingNum(MIDDLE);
	}

	//if(dir==MIDDLE)//测试_yusi
	//{
	//	auto cards=_roundManager->_players[MIDDLE]->_cards;
	//	const int  LAST = cards->real_last();
	//	for(int i=0; i<=LAST; i++) {
	//		auto card = _GetCardInHand(MIDDLE,i);
	//		auto x= card->getPosition().x;
	//		auto y= card->getPosition().y;
	//		CCLOG("card[%d].position(%f,%f)",i,x,y);
	//	}
	//	system("pause");
	//}
}

void RaceLayer::_CardRiverUpdateEffect(PlayerDir_t dir) {
	auto show_card_indicator=this->getChildByTag(SHOWCARD_INDICATOR_TAG_ID);
	show_card_indicator->setVisible(true);

    CardList *river = _roundManager->_players[dir]->_river;
    
    int i = 0;
    while( i<river->size() ) {
	    _Remove(myframe,HAND_OUT_CARDS_TAG_ID+dir*25+i);

        show_card_indicator->stopAllActions();
        
        show_card_indicator->setPosition(_layout->OrigPositionOfRiverCard((PlayerDir_t)dir,i));                      
        show_card_indicator->runAction(RepeatForever::create(Sequence::create(
            MoveTo::create(0.5,_layout->Middle1PositionOfRiverCard((PlayerDir_t)dir,i)),
            MoveTo::create(0.5,_layout->Middle2PositionOfRiverCard((PlayerDir_t)dir,i)),NULL)));

        auto show_card = _object->CreateRiverCard((PlayerDir_t)dir,river->get_kind(i));
        show_card->setPosition(_layout->DestPositionOfRiverCard((PlayerDir_t)dir,i));

        myframe->addChild(show_card, 
            _layout->ZorderOfRiverCard((PlayerDir_t)dir,i),
            HAND_OUT_CARDS_TAG_ID+dir*25+i);

        i++;
    }
}

/***************************************************
        distribute
***************************************************/
Vec2 RaceLayer::_GetLastCardPosition(PlayerDir_t dir,int cardLen) {
    float x = _layout->_playerPosi[dir].basePoint.x;
    float y = _layout->_playerPosi[dir].basePoint.y;

    switch(dir) {
        case MIDDLE:
            if(_roundManager->_actCtrl.decision==aMING_GANG || _roundManager->_actCtrl.decision==aSHOU_GANG
                || _roundManager->_actCtrl.decision==aAN_GANG) {
                x += distributeCardPos.x;
            } else {
                x += _GetCardInHand(MIDDLE,cardLen)->getPosition().x+30;
            }
            y += 60 + 13*(_roundManager->IsMing(dir));
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

void RaceLayer::_DistributeCard(const DistributeInfo_t &distribute) {
    PlayerDir_t     dir = distribute.target;
    int     lenOfInHand = distribute.cardsLen - 1;
    
    const Vec2 &lastCardPosition = _GetLastCardPosition(dir,lenOfInHand);

    Sprite *lastCard = _object->CreateDistributeCard(dir,distribute.newCard);

	if(1)
	{
		if(distribute.newCard<=8)//测试_yusi
			CCLOG("DistributeCard [%d] : %d Tiao",dir,distribute.newCard+1);
		else if(distribute.newCard<=17)
			CCLOG("DistributeCard [%d] : %d Bing",dir,distribute.newCard-8);
		else
			CCLOG("DistributeCard [%d] : %d Feng",dir,distribute.newCard-17);
	}

	lastCard->setPosition(lastCardPosition);

    
	_UpdateResidueCards(distribute.remain);

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

void RaceLayer::ListenToDistributeCard() {
	auto _distributedoneListener = EventListenerCustom::create(DISTRIBUTE_DONE_EVENT_TYPE, [this](EventCustom * event){
		DistributeInfo_t *userData = static_cast<DistributeInfo_t *>(event->getUserData());
        _DistributeCard(*userData);
	});
	_eventDispatcher->addEventListenerWithFixedPriority(_distributedoneListener,2);

	auto _noonewinListener = EventListenerCustom::create(NOONE_WIN_EVENT_TYPE, [this](EventCustom * event){

		((Button*)this->getChildByTag(MENU_BKG_TAG_ID)->getChildByTag(TUOGUAN_MENU_BUTTON))->setTouchEnabled(false);

        _Remove(this,ROBOT_TUO_GUAN);
        _Remove(this,TUOGUAN_CANCEL_BUTTON);
		
        _roundManager->SetWin(NONE_WIN,_roundManager->_curPlayer);

		scheduleOnce(schedule_selector(RaceLayer::raceAccount),3);

		this->runAction(
            Sequence::create(
                DelayTime::create(0.5),CallFunc::create([=](){
    			if(_roundManager->_firstMingNo!=-1)
    				_roundManager->update_gold(INVALID_DIR,HU_WIN,(PlayerDir_t)_roundManager->_firstMingNo);}),
    			DelayTime::create(1),
    			Spawn::create(CallFunc::create([=](){
        			this->addChild(_object->CreateHuangZhuangBkg(),10,HUANG_ZHUANG_LAYER);
        			auto HuangZhuang=_object->CreateHuangZhuang();;
        			this->addChild(HuangZhuang,10,HUANG_ZHUANG_FONT);
        			auto FontAction=TargetedAction::create(HuangZhuang,FadeIn::create(0.5));}),CallFunc::create(this,callfunc_selector(
    			    RaceLayer::showall)),NULL),NULL));
	});
	_eventDispatcher->addEventListenerWithFixedPriority(_noonewinListener,3);
}

void RaceLayer::_LeftBatchDistribute(int batchIdx, float delayRef, int cardLen) {//第一轮发牌
	auto cardHeight = _object->RectSize(L_IN_CARD).height;

    float x = _layout->_playerPosi[LEFT].basePoint.x+10;
    float y = _layout->_playerPosi[LEFT].basePoint.y+10;

    y -= (cardHeight*0.5)*4*(batchIdx);
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

void RaceLayer::_RightBatchDistribute(int batchIdx, float delayRef, int cardLen) {
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

void RaceLayer::FirstRoundDistributeEffect(PlayerDir_t zhuang) {
	auto VoiceEffect=_voice->Speak("sort.ogg");	

	float timeXH[PLAYER_NUM];
	timeXH[zhuang] = 0.7;
	timeXH[(zhuang+1)%PLAYER_NUM] = 0.9;
	timeXH[(zhuang+2)%PLAYER_NUM] = 1.1;

	int a,b;
	/************************************
	 自己 middle -> one batch from center to hand
	************************************/
	auto cards=_roundManager->_players[MIDDLE]->_cards;
	auto cardWidth = _object->RectSize(FREE_CARD).width;
	auto disCardWidth = _object->RectSize(IN_CARD).width;

	Sprite* meHandCard[DIST_BATCH_CARDS];
	Vec2 meHandPosition[DIST_BATCH_CARDS];

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
		changingPosition[1]+=(cardWidth*(a*2)+cardWidth);
		changingPosition[2]+=(cardWidth*(a*3)+cardWidth*2);
		changingPosition[3]+=(cardWidth*13+a*cardWidth+30);

		auto outTime1=Sequence::create(
			DelayTime::create(timeXH[1]-0.4),
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
	 自己 middle -> distribute to middle player
	************************************/
	Sprite* p_list[4];
	Sprite* lastTwo[2];
	float x,y;
	const Vec2 &REF = Vec2(_layout->_playerPosi[1].basePoint.x+10,
		_layout->_playerPosi[1].basePoint.y+10);

	for(a=0;a<DIST_BATCHES;a++)
	{
		if(a==0)//DealayTime::create(0.9)
		{
			for(int i=0;i<DIST_BATCH_CARDS;i++)
			{				
				auto s_card=_object->CreateKind(cards->get_kind(0+3*i),NORMAL);

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
				auto s_card=_object->CreateKind(cards->get_kind(1+3*i),NORMAL);
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
				auto s_card=_object->CreateKind(cards->get_kind(2+3*i),NORMAL);
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
			auto s_card=_object->CreateKind(cards->get_kind(12),NORMAL);
			lastTwo[0]=_object->Create(FREE_CARD,s_card);
			lastTwo[0]->setAnchorPoint(Vec2(0.0f,0.0f));
			lastTwo[0]->setPosition(Vec2(
				REF.x + cardWidth*1.0*12, 
				REF.y));
			lastTwo[0]->setScale(0);
			myframe->addChild(lastTwo[0],0+13,HAND_IN_CARDS_TAG_ID+1*20+12);

			lastTwo[0]->runAction(Sequence::create(DelayTime::create(timeXH[1]+2),ScaleTo::create(0,1),NULL));	

			if(zhuang==MIDDLE) {
				auto s_card=_object->CreateKind(cards->get_kind(13),NORMAL);
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
	ListenToDistributeCard();

	myframe->_ID = zhuang;
	myframe->runAction(Sequence::create(
		_FisrtRoundResidueUpdate(),Spawn::create(CallFunc::create([=](){
			_CardInHandUpdateEffect(LEFT);}),CallFunc::create([=](){
				_CardInHandUpdateEffect(MIDDLE);}),CallFunc::create([=](){
					_CardInHandUpdateEffect(RIGHT);}),NULL),CallFunc::create([=](){
						_DeleteStartDealCards();}),CCCallFunc::create([=](){
							WaitForFirstAction(zhuang);
						}),NULL));
}
Sequence *RaceLayer::_FisrtRoundResidueUpdate() {
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

void RaceLayer::ListenToDoubleHu() {
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
                RaceLayer::showall)),
                simple_tip_effect( _layout->PositionOfActSign((PlayerDir_t)((_roundManager->_curPlayer+1)%3)),"dahu.png"),
                simple_tip_effect( _layout->PositionOfActSign((PlayerDir_t)((_roundManager->_curPlayer+2)%3)),"dahu.png"),NULL),CallFunc::create([=](){
            _roundManager->update_gold(INVALID_DIR,HU_WIN,(PlayerDir_t)_roundManager->_curPlayer);}),NULL));
    });
    
    _eventDispatcher->addEventListenerWithFixedPriority(_doublehucallListener,2);
}

/***********************************************
            graphical residue cards
***********************************************/
void RaceLayer::_CreateResidueCards() {
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

void RaceLayer::_InitResidueCards() {
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

void RaceLayer::_UpdateResidueCards(int no)
{
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

void RaceLayer::GuiHideReady() {
    _Remove(this,READY_INDICATE_LEFT_TAG_ID);
    _Remove(this,READY_INDICATE_RIGHT_TAG_ID);
}

void RaceLayer::GuiShowReady(int dir)
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
void RaceLayer::_ClockAddTime( Sprite *clock, int time ) {
    Sprite *p = (Sprite *)clock;

	char strTime[16] = {0};
	sprintf(strTime,"%d",time);
    
	auto labelTime = LabelTTF::create( std::string(strTime), "Arial", 30 );
	labelTime->setAnchorPoint(Vec2(0.5,0.5));
	labelTime->setColor(Color3B(0,0,0));
	labelTime->setPosition(Vec2(p->getTextureRect().size.width/2,p->getTextureRect().size.height*0.55));

    clock->addChild(labelTime,0,ALARM_CLOCK_CHILD_NUM_TAG_ID);
}

void RaceLayer::HideClock() {
    Node* indicator[4] = {0};
    
    indicator[LEFT]   = this->getChildByTag(ALARM_CLOCK_INDICATE_LEFT_TAG_ID);
    indicator[MIDDLE] = this->getChildByTag(ALARM_CLOCK_INDICATE_DOWN_TAG_ID);
    indicator[RIGHT]  = this->getChildByTag(ALARM_CLOCK_INDICATE_RIGHT_TAG_ID);
    indicator[3]      = this->getChildByTag(ALARM_CLOCK_INDICATOR_TAG_ID);
    
    for(int i=0;i<4;i++) {
        if( indicator[i] ) {
            indicator[i]->setVisible(false);
        }
    }
}

void RaceLayer::UpdateClock(float dt) {
    Node* indicator[4] = {0};
    
    indicator[LEFT]   = this->getChildByTag(ALARM_CLOCK_INDICATE_LEFT_TAG_ID);
    indicator[MIDDLE] = this->getChildByTag(ALARM_CLOCK_INDICATE_DOWN_TAG_ID);
    indicator[RIGHT]  = this->getChildByTag(ALARM_CLOCK_INDICATE_RIGHT_TAG_ID);
    indicator[3]      = this->getChildByTag(ALARM_CLOCK_INDICATOR_TAG_ID);
    
    for(int i=0;i<4;i++) {
        if( !indicator[i] ) {
            return;
        }
    }

    indicator[3]->setVisible(true);
    _Remove(indicator[3],ALARM_CLOCK_CHILD_NUM_TAG_ID);
    _ClockAddTime((Sprite *)indicator[3],_timer.count--);

    indicator[_timer.curPlayer]->setVisible(true);
    indicator[(_timer.curPlayer+1)%3]->setVisible(false);
    indicator[(_timer.curPlayer+2)%3]->setVisible(false);

    if(_timer.count<0) {
        unschedule(schedule_selector(RaceLayer::UpdateClock));

        if(_timer.curPlayer==MIDDLE) {
            #ifndef TIMER_FREE
            _roundManager->ForceHandout();
            #endif
        }
    }
}

void RaceLayer::start_timer(int time,PlayerDir_t dir){
    _timer.curPlayer = dir;
    _timer.count     = time;

    unschedule(schedule_selector(RaceLayer::UpdateClock));

    UpdateClock(0);
    schedule(schedule_selector(RaceLayer::UpdateClock), 1.0f, kRepeatForever, 0);
}
/****************************************************
    effect position
****************************************************/

void RaceLayer::_CreateGangCardsMotion(TargetedAction *motions[4]) {
    
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

void RaceLayer::_CreateMingGangCardsMotion(TargetedAction *mostions[4],CARD_KIND kind) {
    
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


void RaceLayer::_CreateGangCardInHandMotion(TargetedAction *motions[4],int cardInHand[4],CARD_KIND kind) {
    
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

void RaceLayer::_AttachKindTexture(Sprite *parent,CARD_KIND kind) {
    auto kindTexture = _object->CreateKind((Card_t)kind,MIDDLE_SIZE);
    
    kindTexture->setAnchorPoint(Vec2(0.5,0.5));
    kindTexture->setPosition(Vec2(
        parent->getTextureRect().size.width/2,
        parent->getTextureRect().size.height*0.65));
    parent->addChild(kindTexture);
}


void RaceLayer::_AttachKindTextureToFreeCard(Sprite *parent,CARD_KIND kind) {
    auto kindTexture = _object->CreateKind((Card_t)kind,NORMAL);
    
    kindTexture->setAnchorPoint(Vec2(0.5,0.5));
    kindTexture->setPosition(Vec2(
        parent->getTextureRect().size.width/2,
        parent->getTextureRect().size.height*0.4));
    parent->addChild(kindTexture);
}
    
/* the first is non-peng card*/
void RaceLayer::_CreateMingGangCardInHandMotion(TargetedAction *motions[3], int idxInHand[3], CARD_KIND kind) {
    
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

void RaceLayer::_CreateFreeCard(Sprite *cards[3], int idxInHand[3], CARD_KIND kind) {
    

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

void RaceLayer::_CreateBackgroundElementMotion(TargetedAction *motions[5],int gangType) {
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

Sequence *RaceLayer::_HideReminder(int reminderTag, double lastingTime, double shadeScale) {
    if(myframe->getChildByTag(reminderTag)) {
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
    } else {
        return Sequence::create(CCCallFunc::create([=](){;}),NULL);
    }
}

Sequence *RaceLayer::_HideQiReminder() {
    auto shadeAction = TargetedAction::create((Sprite*)myframe->getChildByTag(QI_REMIND_ACT_BKG_TAG_ID),
        Sequence::create(
            ScaleTo::create(0,1), 
            Spawn::create(
                FadeOut::create(0.3),
                ScaleTo::create(0.3,1.3),NULL),NULL));

    return Sequence::create(
            shadeAction,CCCallFunc::create(this,callfunc_selector(
            RaceLayer::_DeleteActionReminder)),NULL);
}

Sprite *RaceLayer::_GetCardInHand(PlayerDir_t dir,int idx) {
    return (Sprite *)myframe->getChildByTag(HAND_IN_CARDS_TAG_ID + dir*20 + idx);
}

void RaceLayer::_ReOrderCardsInHand(int droppedCard,CardInHand *cards) {
    /*NOTE : this function is called after handout ,so the position is 1 bigger than length */
    const int  LAST      = cards->real_last();

    if(droppedCard==LAST) {
		_GetCardInHand(MIDDLE,droppedCard)->setScale(0);
        return;
    }
    
    const auto SIZE      = _GetCardInHand(MIDDLE,LAST)->getTextureRect().size;
    /* maybe need to be put into the circulation */
    const auto LAST_CARD = _GetCardInHand(MIDDLE,LAST);
    const auto LAST_POS  = LAST_CARD->getPosition();
    
    /* need to be optimized !!! */
    for(int i=cards->FreeStart; i<LAST; i++) {
        auto card = _GetCardInHand(MIDDLE,i);
        auto curPos   = card->getPosition();
        
        auto RightMove = MoveTo::create(0.3,Vec2(curPos.x+SIZE.width,curPos.y));
        auto LeftMove  = MoveTo::create(0.3,Vec2(curPos.x-SIZE.width*1.2,curPos.y));
                    
        if(i < droppedCard) {
            if(cards->get_kind(i) <= cards->get_kind(LAST))
                continue;
            else {
				if( i==cards->FreeStart || cards->get_kind(i-1)<=cards->get_kind(LAST) ) {
					float curScale=(float)(LAST-i)/13;
					float curDelayTime=curScale*0.35+0.15;
					auto LeftMoveAction1=MoveTo::create(0.15,Vec2(LAST_POS.x,LAST_POS.y+100));
					auto LeftMoveAction2=MoveTo::create(curDelayTime,Vec2(curPos.x,LAST_POS.y+100));
					auto LeftMoveAction3=MoveTo::create(0.15,Vec2(curPos.x,LAST_POS.y));
					auto LeftInsertSeq=Sequence::create(LeftMoveAction1,LeftMoveAction2,LeftMoveAction3,NULL);
                    LAST_CARD->runAction(LeftInsertSeq);
                }
                card->runAction(RightMove);
            }
        } else if(i==droppedCard) {
			if(i==cards->FreeStart) {
				if(i==LAST-1)
					LAST_CARD->runAction(MoveTo::create(0.3,Vec2(curPos.x,LAST_POS.y)));
				else if(cards->get_kind(i+1)>cards->get_kind(LAST))
				{
					float curScale=(float)(LAST-i)/13;
					float curDelayTime=curScale*0.35+0.15;
					auto LeftMoveAction1=MoveTo::create(0.15,Vec2(LAST_POS.x,LAST_POS.y+100));
					auto LeftMoveAction2=MoveTo::create(curDelayTime,Vec2(curPos.x,LAST_POS.y+100));
					auto LeftMoveAction3=MoveTo::create(0.15,Vec2(curPos.x,LAST_POS.y));
					auto LeftInsertSeq=Sequence::create(LeftMoveAction1,LeftMoveAction2,LeftMoveAction3,NULL);
					LAST_CARD->runAction(LeftInsertSeq);
				}
            } else if(i==LAST-1) {
                if(cards->get_kind(i-1)<=cards->get_kind(LAST))
                    LAST_CARD->runAction(MoveTo::create(0.3,Vec2(curPos.x,LAST_POS.y)));
			} else {
				if(cards->get_kind(i-1)<=cards->get_kind(LAST)&&cards->get_kind(i+1)>cards->get_kind(LAST))
				{
					float curScale=(float)(LAST-i)/13;
					float curDelayTime=curScale*0.35+0.15;
					auto LeftMoveAction1=MoveTo::create(0.15,Vec2(LAST_POS.x,LAST_POS.y+100));
					auto LeftMoveAction2=MoveTo::create(curDelayTime,Vec2(curPos.x,LAST_POS.y+100));
					auto LeftMoveAction3=MoveTo::create(0.15,Vec2(curPos.x,LAST_POS.y));
					auto LeftInsertSeq=Sequence::create(LeftMoveAction1,LeftMoveAction2,LeftMoveAction3,NULL);
					LAST_CARD->runAction(LeftInsertSeq);
				}
            }
            card->setScale(0);
        } else {/* i>chosenCard */
            if(cards->get_kind(i)>cards->get_kind(LAST))
                card->runAction(MoveTo::create(0.3,Vec2(curPos.x-SIZE.width*0.2,curPos.y)));
            else {
                if(i==LAST-1)
                    LAST_CARD->runAction(MoveTo::create(0.3,Vec2(curPos.x-SIZE.width*0.2,curPos.y)));
                else if(cards->get_kind(i+1)>cards->get_kind(LAST))
				{
					float curScale=(float)(LAST-i)/13;
					float curDelayTime=curScale*0.35+0.15;
					auto LeftMoveAction1=MoveTo::create(0.15,Vec2(LAST_POS.x,LAST_POS.y+100));
					auto LeftMoveAction2=MoveTo::create(curDelayTime,Vec2(curPos.x-SIZE.width*0.2,LAST_POS.y+100));
					auto LeftMoveAction3=MoveTo::create(0.15,Vec2(curPos.x-SIZE.width*0.2,curPos.y));
					auto LeftInsertSeq=Sequence::create(LeftMoveAction1,LeftMoveAction2,LeftMoveAction3,NULL);
					LAST_CARD->runAction(LeftInsertSeq);
				}
                card->runAction(LeftMove);
            }
        }
    }
}

TargetedAction *RaceLayer::_ShowBigMing(Node *myframe) {
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

TargetedAction* RaceLayer::_MingAnimation() {
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

TargetedAction *RaceLayer::_OthersShowCardEffect(PlayerDir_t dir,Card_t outCard,bool canKou) {
    _Remove(myframe,SHOW_CARD_INIDCATOR_TAD_ID);
    
	auto smallCard = _object->CreateKind(outCard,SMALL);
	smallCard->runAction(RotateTo::create(0,_layout->RotateAngleOfOutcard(dir)));

	Sprite   * cardOut = _object->Create(LR_OUT_CARD);
	smallCard->setPosition(Vec2(
        cardOut->getTextureRect().size.width/2,
        cardOut->getTextureRect().size.height*0.65));
	smallCard->setRotation(-90);/* !!! player0 is 90 in the old source, but it seems ok to set it as -90*/
	smallCard->setScale(0.9);
	cardOut->addChild(smallCard);
	cardOut->setAnchorPoint(Vec2(0.5,0.5));

	auto cardInHand = _roundManager->_players[dir]->_cards;

    auto curOutPosTemp = _GetCardInHand(dir,cardInHand->real_last())->getPosition();
    Vec2 curOutPos;/* here must be something I have not known */
    if(dir==RIGHT) {
		curOutPos = Vec2(
            curOutPosTemp.x,
            curOutPosTemp.y + cardOut->getTextureRect().size.height*1.5);
	}
	else {
        curOutPos = Vec2(
            _layout->_playerPosi[dir].basePoint.x + 10,
            curOutPosTemp.y - 20 - 35);
	}

    cardOut->setPosition(curOutPos);
    cardOut->setVisible(false);

    _Remove(myframe,OUT_CARD_FRAME_TAG_ID);
	if(dir==RIGHT)
		myframe->addChild(cardOut,0,OUT_CARD_FRAME_TAG_ID);/* !!! player2 is 20 in the old source, but it seems ok to set it as 0*/
	else{
		myframe->addChild(cardOut,20,OUT_CARD_FRAME_TAG_ID);
	}

	auto LastCard   = _GetCardInHand(dir,cardInHand->size());
    
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
        
		auto card = _object->CreateKind(outCard,NORMAL);
		card->setAnchorPoint(Vec2(0.5,0.5));
		card->setPosition(Vec2(cardBg->getTextureRect().size.width/2,cardBg->getTextureRect().size.height*0.4));
		cardBg->addChild(card);
        
		cardFrame->addChild(cardBg);
		cardFrame->runAction(Sequence::create(
            DelayTime::create(1.5),
            ScaleTo::create(0,0),NULL));
	});
    




	Sequence *result = Sequence::create(
                		hideLastInHand,CallFunc::create([=](){ 
                        _Show(myframe,OUT_CARD_FRAME_TAG_ID,true);} ),Spawn::create(
                    		showAndHideOutcardNotice,
                    		OthersBizerMove(dir,_roundManager->_players[dir]->_river),
                    		_voice->SpeakCard(outCard,
                                _roundManager->_players[dir]->GetSex()),NULL),
                		_voice->Speak("give"),NULL);;	

	if(canKou) {
		result = Sequence::create(
                        simple_tip_effect( _layout->PositionOfActSign(dir),"daming.png" ),
                        result,NULL);
	}

    return TargetedAction::create(cardOut,result);
}

void RaceLayer::_OthersMingGangEffect(PlayerDir_t dir,PlayerDir_t prevDir,bool isNewDistributed,Card_t card) {
    Sequence *gang_seq;
    
    auto hideOutCard = Sequence::create(NULL);
    auto goldEffect  = CallFunc::create([=](){NULL;});

    if(!isNewDistributed) {
        hideOutCard = Sequence::create(CCCallFunc::create([=](){
            _Show(this,SHOWCARD_INDICATOR_TAG_ID,false);}),TargetedAction::create(
            myframe->getChildByTag(OUT_CARD_FRAME_TAG_ID),Sequence::create(
                DelayTime::create(0.06),
                ScaleTo::create(0,0),NULL)),NULL);

        goldEffect = CallFunc::create([=](){
            _roundManager->update_gold(dir,MING_GANG,prevDir);});
    }
    
    myframe->runAction(Sequence::create(
        hideOutCard,
        Spawn::create(
            simple_tip_effect(_layout->PositionOfActSign(dir),"gang.png"),
            _voice->SpeakAction(aMING_GANG,_roundManager->_players[dir]->GetSex()),NULL),
        goldEffect,
        Sequence::create(CCCallFunc::create(this,callfunc_selector(
            RaceLayer::_DeleteActionTip)),CCCallFunc::create([=]() {
            _roundManager->UpdateCards(dir,a_MING_GANG,card);}),CCCallFunc::create([=]() {
            _CardInHandUpdateEffect(dir);}),CallFunc::create([=](){
            _roundManager->ActionAfterGang(dir);}),NULL),NULL));
}

void RaceLayer::_MyHandoutEffect(Card_t outCard,Vec2 touch,int time,bool turnToMing)
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
		bizerMotion = BizerMove1(_roundManager->_players[MIDDLE]->_river,
		                touch);
    }
	else {
		cardOut->setPosition(touch.x,100);
		bizerMotion = BizerMove2(_roundManager->_players[MIDDLE]->_river,
		                touch,time);
    }
    myframe->addChild(cardOut,30,OUT_CARD_FRAME_TAG_ID);

	CallFunc* speakCard  = _voice->SpeakCard(outCard,_roundManager->_players[MIDDLE]->GetSex());

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

	myframe->_ID = MIDDLE;
    
	myframe->runAction(Sequence::create(
        allEffect,
        DelayTime::create(0.5),CallFunc::create([=](){
        ifInsertCardsTime=false;}),Sequence::create(CCCallFunc::create([=]() {
    		_CardRiverUpdateEffect(MIDDLE);}),CCCallFunc::create([=]() {
            //_roundManager->UpdateCards(MIDDLE,a_JUMP);
            _Show(myframe,TING_SING_BUTTON,true);}),CallFunc::create([=](){
    		if(_isCardInHandUpdated)
    			_isCardInHandUpdated = false;
    		else {
                _Show(this,MING_STATUS_PNG_1,_roundManager->IsMing(MIDDLE));
    			_CardInHandUpdateEffect(MIDDLE);
    		}}),NULL),CCCallFunc::create([=]() {
		_roundManager->_isNewDistributed = false;
		_roundManager->WaitForResponse(MIDDLE);}),
        NULL));
}

/************************************************
                support
************************************************/
void RaceLayer::_CreateMeneButtons() {
    GMenu *gMenu = _object->CreateMenu();
    this->addChild(gMenu->_bkg,4,MENU_BKG_TAG_ID);

    auto buttonImg = Sprite::createWithSpriteFrameName("baomingbisai2.png");
    
    auto flagButton = _object->CreateButton(MENUBTN_BAOMING);
    gMenu->AddItem(flagButton,buttonImg);

    auto robotButton = _object->CreateButton(MENUBTN_TUOGUAN);
    robotButton->setTouchEnabled(false);
    robotButton->addTouchEventListener(CC_CALLBACK_2(RaceLayer::BtnTuoGuanHandler,this));
    gMenu->AddItem(robotButton);

    auto setButton = _object->CreateButton(MENUBTN_SHEZHI);
    gMenu->AddItem(setButton);

    auto mallButton = _object->CreateButton(MENUBTN_SHOP);
    gMenu->AddItem(mallButton);

    auto backButton = _object->CreateButton(MENUBTN_GAMEBACK);
    backButton->addTouchEventListener(CC_CALLBACK_2(RaceLayer::BtnBackHandler,this));
    gMenu->AddItem(backButton);
}

void RaceLayer::_CreateHeadImage() {
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

void RaceLayer::_RaceBeginPrepare() {
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

bool RaceLayer::_ResourcePrepare()
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

void RaceLayer::_StartParticleSystem(float delta)
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

void RaceLayer::_UpdateNonChosenCards(const CardInHand *cards, int chosen) {
    auto cardSize = _object->RectSize(FREE_CARD);

    float y1 = _layout->_playerPosi[MIDDLE].basePoint.y+10;
    auto startPos = _GetCardInHand(MIDDLE,cards->FreeStart)->getPosition();
    
    for(int i=cards->FreeStart; i<=cards->real_last(); i++) {
        auto loopCard=_GetCardInHand(MIDDLE,i);

        if(i==chosen) {
            continue;
        } else if(i<chosen) {
            loopCard->setPosition(Vec2(startPos.x + cardSize.width*(i-cards->FreeStart), y1));
            loopCard->setScale(1);
            loopCard->_ID = 1;
        } else if(i>chosen) {
            if( i==cards->last() && cards->is_wait_handout() ) {
                loopCard->setPosition(Vec2(startPos.x+cardSize.width*(i-cards->FreeStart+0.2)+30,y1));
            } else {
                loopCard->setPosition(Vec2(startPos.x+cardSize.width*(i-cards->FreeStart+0.2),y1));
            }
            
            loopCard->setScale(1);
            loopCard->_ID = 1;
        }
    }
}

void RaceLayer::_CancelChosenCardInHand() {
    if(_myChosenCard!=INVALID) {
        auto card = _GetCardInHand(MIDDLE,_myChosenCard);
        
        card->setPosition(card->getPosition() - Vec2(0,10));
        card->setScale(1);
        
        auto cardSize = _object->RectSize(FREE_CARD);
        auto cards    = _roundManager->_players[MIDDLE]->_cards;
        
        for(int i=_myChosenCard+1;i<=cards->real_last();i++) {
            auto behinds = _GetCardInHand(MIDDLE,i);
            behinds->setPosition(behinds->getPosition() - Vec2(cardSize.width*0.2,0));
        }
        
        _myChosenCard = INVALID;
    }
}

void RaceLayer::_UpdateCardsInHand(const CardInHand *cards, int chosen) {
    if(cards->canPlay(chosen)) {/* I think the cards behind start all can play ???*/                 

        _UpdateNonChosenCards(cards,chosen);

        float x1 = _layout->_playerPosi[MIDDLE].basePoint.x+10;
        float y1 = _layout->_playerPosi[MIDDLE].basePoint.y+10;
        auto cardSize = _object->RectSize(FREE_CARD);
        auto startPos = _GetCardInHand(MIDDLE,cards->FreeStart)->getPosition();
        auto loopCard = _GetCardInHand(MIDDLE,chosen);

        _myChosenCard = chosen;

        if( chosen==cards->last() && cards->is_wait_handout() ) {/* logic changed !!!*/
            loopCard->setPosition(Vec2(startPos.x+cardSize.width*(chosen-cards->FreeStart)+30,y1+10));
        } else {
            loopCard->setPosition(Vec2(startPos.x+cardSize.width*(chosen-cards->FreeStart),   y1+10));
        }

        /* provide hint bar when ming choose */
        if(_roundManager->_actCtrl.decision==aMING 
            && _roundManager->_isMingTime 
            && (!_roundManager->IsMing(MIDDLE))) {
            
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
        }
		else
		{
			if( loopCard->getScaleX()==1 ) {
				loopCard->setScale(1.2);
				loopCard->runAction(_voice->Speak("select"));
			}
		}
		ifChosed = true;
		_myTouchedCard  = chosen;
	}
}

Sprite *RaceLayer::_CreateEffectCard(int i,CARD_KIND kindId ) {
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

Sprite* RaceLayer::_GetCardOnTingSignBar(PlayerDir_t dir,int cardIdx) {
    switch (dir) {
        case LEFT:
            return (Sprite*)myframe->getChildByTag(TING_SING_LEFTBAR)->getChildByTag(cardIdx+3);
        case MIDDLE:
            return (Sprite*)myframe->getChildByTag(TING_SING_BAR)->getChildByTag(cardIdx+3);
        case RIGHT:
            return (Sprite*)myframe->getChildByTag(TING_SING_RIGHTBAR)->getChildByTag(cardIdx+3);
    }
}

LabelAtlas * RaceLayer::_CreateNumberSign(int number) {
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
void RaceLayer::HideActionButtons() {
    for(int i=0;i<17;i++){
        _Remove(myframe,REMIND_ACT_TAG_ID+i);
    }
}

void RaceLayer::ShowActionButtons(int actionsMask) {
    _Show(myframe,TING_SING_BAR,false);

	float y=origin.y+visibleSize.height*0.25;
	float x=origin.x+visibleSize.width*0.85;

	if(actionsMask!=a_JUMP) {
	    float width = _AddBtnQi(Vec2(x,y));
		x = x-width;
	}
    
	if(actionsMask&a_HU) {	
        float width = _AddBtnHu(Vec2(x,y));
		x = x-width;
	}
    
	if(actionsMask&a_MING) {
        float width = _AddBtnMing(Vec2(x,y));
		x = x-width;
	}
    
	if(actionsMask&a_AN_GANG || actionsMask&a_MING_GANG ||actionsMask&a_SHOU_GANG) {
        float width = _AddBtnGang(Vec2(x,y));
		x = x-width;
	}

	if(actionsMask&a_PENG) {		
        float width = _AddBtnPeng(Vec2(x,y));
		x = x-width;
	}
}

float RaceLayer::_AddBtnQi(const Vec2 &ref) {
    auto picWidth = _object->_image("qi.png")->getContentSize().width;

    auto myact_qi = _object->CreateButton(MIDDLE,BTN_QI,Vec2(ref.x,ref.y));
    myact_qi->_ID = MIDDLE;
    myact_qi->addTouchEventListener(CC_CALLBACK_2(RaceLayer::BtnQiHandler,this));
    myframe->addChild(myact_qi,35,QI_REMIND_ACT_TAG_ID);
    
    auto qi_act = _object->CreateBtnBkg(BTN_QI,Vec2(ref.x,ref.y));
    myframe->addChild(qi_act,34,QI_REMIND_ACT_BKG_TAG_ID);

    return picWidth/2+36;
}

float RaceLayer::_AddBtnHu(const Vec2 &ref) {
    auto picWidth = _object->_image("hu1.png")->getContentSize().width;
    float x = ref.x - picWidth/2;
    
    auto myact_hu = _object->CreateButton(MIDDLE,BTN_HU,Vec2(x,ref.y+6));
    myact_hu->_ID=MIDDLE;
    myact_hu->addTouchEventListener(CC_CALLBACK_2(RaceLayer::BtnHuHandler,this));
    myframe->addChild(myact_hu,35,HU_REMIND_ACT_TAG_ID);
    
    auto hu1_act = _object->CreateBtnBkg(BTN_HU,Vec2(x,ref.y+6));
    myframe->addChild(hu1_act,34,HU_REMIND_ACT_BKG_TAG_ID);
    
    return (picWidth+18);
}

float RaceLayer::_AddBtnMing(const Vec2 &ref) {
    auto picWidth = _object->_image("ming.png")->getContentSize().width;
    float x = ref.x - picWidth/2;
    
    auto myact_ming=_object->CreateButton(MIDDLE,BTN_MING,Vec2(x,ref.y+11));
    myact_ming->addTouchEventListener(CC_CALLBACK_2(RaceLayer::BtnMingHandler,this));
    myframe->addChild(myact_ming,35,MING_REMIND_ACT_TAG_ID);
    
    auto ming_act=_object->CreateBtnBkg(BTN_MING,Vec2(x,ref.y+11));
    myframe->addChild(ming_act,34,MING_REMIND_ACT_BKG_TAG_ID);
    
    return (picWidth+18);
}

float RaceLayer::_AddBtnGang(const Vec2 &ref) {
    auto picWidth = _object->_image("gang1.png")->getContentSize().width;
    float x = ref.x - picWidth/2;
    
    auto myact_gang=_object->CreateButton(MIDDLE,BTN_GANG,Vec2(x,ref.y));
    myact_gang->addTouchEventListener(CC_CALLBACK_2(RaceLayer::BtnGangHandler,this));
    myframe->addChild(myact_gang,35,GANG_REMING_ACT_TAG_ID);
    
    auto gang1_act=_object->CreateBtnBkg(BTN_GANG,Vec2(x,ref.y));
    myframe->addChild(gang1_act,34,GANG_REMING_ACT_BKG_TAG_ID);
    
    return (picWidth+18);
}

float RaceLayer::_AddBtnPeng(const Vec2 &ref) {
    auto picWidth = _object->_image("peng1.png")->getContentSize().width;
    float x = ref.x - picWidth/2;
    
    auto myact_peng=_object->CreateButton(MIDDLE,BTN_PENG,Vec2(x,ref.y+13));
    myact_peng->addTouchEventListener(CC_CALLBACK_2(RaceLayer::BtnPengHandler,this));
    myframe->addChild(myact_peng,35,PENG_REMIND_ACT_TAG_ID);
    
    auto peng1_act=_object->CreateBtnBkg(BTN_PENG,Vec2(x,ref.y+13));    
    myframe->addChild(peng1_act,34,PENG_REMIND_ACT_BKG_TAG_ID);

    return (picWidth+18);
}

/***********************************************
        action buttons
***********************************************/
void RaceLayer::BtnPengHandler(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
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

void RaceLayer::BtnHuHandler(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
{
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

void RaceLayer::BtnGangHandler(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
{
	auto curButton=(Button*)pSender;

	switch (type){
    	case cocos2d::ui::Widget::TouchEventType::BEGAN:
    		curButton->setScale(1.2);
    		break;
    	case cocos2d::ui::Widget::TouchEventType::MOVED:
    		break;
    	case cocos2d::ui::Widget::TouchEventType::ENDED:
            _CancelChosenCardInHand();
            _roundManager->RecvGang(MIDDLE);
    		break;
    	case cocos2d::ui::Widget::TouchEventType::CANCELED:
    		curButton->setScale(1);
    		break;
    	default:
    		break;
	}
}

void RaceLayer::BtnQiHandler(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
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

void RaceLayer::ListenToTingButton() {
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
void RaceLayer::ListenToCardTouch() {
    if( !_roundManager->IsMing(MIDDLE) && !_roundManager->_isTuoGuan ) {
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        
        listener->onTouchBegan = CC_CALLBACK_2(RaceLayer::_CardTouchBegan,this);
        listener->onTouchMoved = CC_CALLBACK_2(RaceLayer::_CardTouchMove,this);
        listener->onTouchEnded = CC_CALLBACK_2(RaceLayer::_CardTouchEnd,this);

        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,myframe);
   }
}

int RaceLayer::_FindCard(int start,int end,Touch *touch) {
    for(int i=start;i<=end;i++) {
        if( _IsClickedOn(_GetCardInHand(MIDDLE,i),touch) ) {
            return i;
        }
    }

    return INVALID;
}

bool RaceLayer::_CardTouchBegan(Touch* touch, Event* event) {
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
		/*当时写这里的时候不能确认TING_SING_BAR是否完全清除，明牌动作执行并不代表状态为明，明牌动作可取消*/
        while(myframe->getChildByTag(TING_SING_BAR) && (!_roundManager->IsMing(1)))
            myframe->removeChildByTag(TING_SING_BAR);
        
        float x1,y1;
        x1 = _layout->_playerPosi[MIDDLE].basePoint.x+10;
        y1 = _layout->_playerPosi[MIDDLE].basePoint.y+10;

        CardInHand *cards = _roundManager->_players[MIDDLE]->_cards;
        
        const auto startPos = _GetCardInHand(MIDDLE,cards->FreeStart)->getPosition();
        const auto cardSize = _object->RectSize(FREE_CARD);

        for(int i=cards->FreeStart; i<=cards->real_last(); i++) {
            auto card = _GetCardInHand(MIDDLE,i);
            card->_ID = 1;
            card->setScale(1);
            card->setAnchorPoint(Vec2(0,0));

            if(i==cards->last() && cards->is_wait_handout()) {
                card->setPosition(Vec2(startPos.x + cardSize.width*(i-cards->FreeStart)+30,y1));
            } else {
                card->setPosition(Vec2(startPos.x + cardSize.width*(i-cards->FreeStart),y1));
            }
        }
        
        _myChosenCard = INVALID;
    }
    
    return true;
}

void RaceLayer::_CardTouchMove(Touch* touch, Event* event) {
    CardInHand *cards = _roundManager->_players[MIDDLE]->_cards;
    
	if(myframe->getChildByTag(CHOOSE_CARD_TAG_ID)!=NULL && touch->getLocation().y>visibleSize.height*0.173) {
		myframe->getChildByTag(CHOOSE_CARD_TAG_ID)->setPosition(touch->getLocation());
		_myChosenCard = _myTouchedCard;
		return;
	} 
    
    for(int i=cards->FreeStart; i<=cards->real_last(); i++) {
        _GetCardInHand(MIDDLE,i)->setOpacity(255);
	}
    
	if(myframe->getChildByTag(CHOOSE_CARD_TAG_ID)!=NULL) {
		myframe->removeChildByTag(CHOOSE_CARD_TAG_ID);
    }
    
    int chosen = _FindCard(cards->FreeStart, cards->real_last(), touch);
    if(chosen!=INVALID) {
        _UpdateCardsInHand(cards,chosen);
    } else if(touch->getLocation().y>visibleSize.height*0.173) {
		if(myframe->getChildByTag(CHOOSE_CARD_TAG_ID)==NULL && ifChosed==true) {
			int  kind = _roundManager->_players[MIDDLE]->_cards->get_kind(_myTouchedCard);

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

			while(myframe->getChildByTag(TING_SING_BAR) && (!_roundManager->IsMing(MIDDLE)))
				myframe->removeChildByTag(TING_SING_BAR,true);
            
			_myChosenCard = _myTouchedCard;
		}
	}
}

void RaceLayer::_CardTouchEnd(Touch* touch, Event* event) {
    CardInHand *cards = _roundManager->_players[MIDDLE]->_cards;
    
	if(myframe->getChildByTag(CHOOSE_CARD_TAG_ID)!=NULL 
        && (touch->getLocation().y>visibleSize.height*0.2)) {
		if(_roundManager->_actCtrl.handoutAllow) {
			_myChosenCard = INVALID;
            _roundManager->RecvHandout(_myTouchedCard,touch->getLocation(),1);
		} else {
			_myChosenCard=_myTouchedCard;

            _GetCardInHand(MIDDLE,_myTouchedCard)->setOpacity(255);
            _Remove(myframe,CHOOSE_CARD_TAG_ID);
		}
		return;
	}
    
    for(int i=cards->FreeStart; i<=cards->real_last(); i++) {
		_GetCardInHand(MIDDLE,i)->setOpacity(255);
    }

    _Remove(myframe,CHOOSE_CARD_TAG_ID);
    
	int chosen = _FindCard(cards->FreeStart, cards->real_last(), touch);
    if(chosen!=INVALID && cards->canPlay(chosen)) {
        _UpdateNonChosenCards(cards,chosen);

		float x = _layout->_playerPosi[MIDDLE].basePoint.x+10;
		float y = _layout->_playerPosi[MIDDLE].basePoint.y+10;
		auto cardSize = _object->RectSize(FREE_CARD);
        auto startPos = _GetCardInHand(MIDDLE,cards->FreeStart)->getPosition();

        _myChosenCard = chosen;

        Sprite *loopCard = _GetCardInHand(MIDDLE,chosen);
		bool ChooseConfirm = false;

        if(chosen==cards->last()) {
            if(loopCard->_ID!=100) {
                loopCard->_ID=100;

                if(cards->is_wait_handout()) {
                    loopCard->setPosition(Vec2(startPos.x + cardSize.width*(chosen - cards->FreeStart)+30,y+10));
                } else {
                    loopCard->setPosition(Vec2(startPos.x + cardSize.width*(chosen - cards->FreeStart),y+10));
                }
                
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
                loopCard->setPosition(Vec2(startPos.x+cardSize.width*(chosen - cards->FreeStart),y+10));
                if(loopCard->getScale()==1)
                    loopCard->runAction(_voice->Speak("select"));
                loopCard->setScale(1.2);
            } else {
                ChooseConfirm=true;
            }
        }

        if(_roundManager->_actCtrl.decision==aMING && !_roundManager->IsMing(MIDDLE)
            && _roundManager->_isMingTime ) {
            while( myframe->getChildByTag(TING_SING_BAR) && (!_roundManager->IsMing(MIDDLE)) )
                myframe->removeChildByTag(TING_SING_BAR);
                
            Point curPos=loopCard->getPosition();
            _TingHintCreate(curPos,chosen);
        }
        
		if(ChooseConfirm && _roundManager->_actCtrl.handoutAllow) {
			_myChosenCard=INVALID;
			_myTouchedCard = chosen;
            _roundManager->RecvHandout(_myTouchedCard,touch->getLocation(),3);
		}
    }
}

/*************************************************************
        action effect
*************************************************************/
void RaceLayer::_PengEffect(PlayerDir_t dir, PlayerDir_t prevDir, Card_t card) {
	myframe->_ID = dir;
    
    _Remove(myframe,HAND_OUT_CARDS_TAG_ID+prevDir*25 + _roundManager->_players[prevDir]->_river->size());

	auto outCard = myframe->getChildByTag(OUT_CARD_FRAME_TAG_ID);
    auto hideOutcard = Spawn::create(CCCallFunc::create([=](){
			_Show(this,SHOWCARD_INDICATOR_TAG_ID,false);}),TargetedAction::create(
            outCard,Sequence::create(
                DelayTime::create(0.1),
                ScaleTo::create(0,0),NULL)),NULL);
		
	if(dir!=MIDDLE) {
		myframe->runAction(Sequence::create( 
                            Spawn::create(
                                _voice->SpeakAction(aPENG,_roundManager->_players[dir]->GetSex()),
                                simple_tip_effect( _layout->PositionOfActSign(dir),"peng.png" ),NULL), 
                            hideOutcard, 
                            Sequence::create(CCCallFunc::create(this,callfunc_selector(
                                RaceLayer::_DeleteActionTip)),   CallFunc::create([=](){
                                _roundManager->UpdateCards(dir,a_PENG,card);
                                _CardInHandUpdateEffect(dir);}), CCCallFunc::create([=](){
                    			_roundManager->_actCtrl.choices = _roundManager->_players[dir]->judge_action_again();
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
        CardInHand *cards = _roundManager->_players[MIDDLE]->_cards; 

        int pengCard[4] = {0};
        cards->find_free_cards(pengCard,card);

		int isChosenCanceled=0;
        
		if(_myChosenCard!=INVALID) {
            _myChosenCard = cards->rechoose_after_peng(_myChosenCard,pengCard);
            
            if(_myChosenCard==INVALID) {
                isChosenCanceled = 1;
            }
		}

		auto LeftPengCard = _CreateEffectCard(pengCard[0], (CARD_KIND)card);
		myframe->addChild(LeftPengCard,20,EFFET_NEWCATD1_TAG);
		auto moveLeftCardInHand = TargetedAction::create(LeftPengCard,Sequence::create(
            DelayTime::create(0.18),
            ScaleTo::create(0,0.6),
            MoveTo::create(0.18,Vec2(
                origin.x+visibleSize.width*0.45,
                origin.y+visibleSize.height*0.26)),
			DelayTime::create(0.12),
			ScaleTo::create(0,0),NULL));

		auto RightPengCard = _CreateEffectCard(pengCard[1], (CARD_KIND)card);
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
        
        const float GAP = (cards->FreeStart==0)? 0.5 : 0;
        
		Vector<FiniteTimeAction *> hide2CardsInhand;

		for(int i=cards->FreeStart;i<=cards->real_last();i++) {/* ??? why real_last not size*/
			auto s_card = (Sprite*)myframe->getChildByTag(HAND_IN_CARDS_TAG_ID + 1*20 + i);
            Sequence *seq;

            if(i==pengCard[0]||i==pengCard[1]) {
				seq = Sequence::create(
                    ScaleTo::create(0,0),NULL);
            } else {
				auto curPos   = s_card->getPosition();
				auto cardSize = s_card->getTextureRect().size;

                if (i<pengCard[0]) {
                    seq = Sequence::create(
                        DelayTime::create(0.18),
                        MoveTo::create(0.3,Vec2(
                            curPos.x + cardPengSize.width*(3.5+GAP),
                            curPos.y)),NULL);
                } else {
                    seq = Sequence::create(
                        DelayTime::create(0.18),
                        MoveTo::create(0.3,Vec2(
                            curPos.x + (cardPengSize.width*(3.5+GAP)) - cardSize.width*(2 + isChosenCanceled*0.2),
                            curPos.y)),NULL);
                }
            }
            
            hide2CardsInhand.insert(i-cards->FreeStart,TargetedAction::create(s_card,seq));
		}
        
        /****************
            tag update 
        ****************/
		for(int a=cards->real_last(); a>=cards->FreeStart; a--) {
			int curTag = HAND_IN_CARDS_TAG_ID + 1*20 + a;
            
			if(!myframe->getChildByTag(curTag))
				continue;
            
			auto curCard = (Sprite*)myframe->getChildByTag(curTag);
            
			if(a>pengCard[1])
				curCard->setTag(curTag+1);
			else if(a==pengCard[1])
				curCard->setTag(EFFECT_TEMP_CARD_TAG_ONE);
			else if(a==pengCard[0])
				curCard->setTag(EFFECT_TEMP_CARD_TAG_TWO);
			else if(a<pengCard[0] && a>cards->FreeStart)
				curCard->setTag(curTag+3);
            else if(a==cards->FreeStart) {
				curCard->setTag(curTag+3);
            }
		}
        /* why not set this value directly??? */
        ((Sprite*)myframe->getChildByTag(EFFECT_TEMP_CARD_TAG_ONE))->setTag(HAND_IN_CARDS_TAG_ID+1*20 + cards->FreeStart);
        ((Sprite*)myframe->getChildByTag(EFFECT_TEMP_CARD_TAG_TWO))->setTag(HAND_IN_CARDS_TAG_ID+1*20 + cards->FreeStart+1);

        auto EmptyCard=_object->Create(FREE_CARD);
        EmptyCard->setAnchorPoint(Vec2(1,1));
        EmptyCard->setScale(0);
        EmptyCard->setPosition(Vec2::ZERO);
        myframe->addChild(EmptyCard,1,HAND_IN_CARDS_TAG_ID + 1*20 + cards->FreeStart+2);

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

                    for(int a=cards->FreeStart-1;a>=0;a--) {
						if( _roundManager->IsCurEffectCard(cards->at(a)) ){
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
                _voice->SpeakAction(aPENG,
                    _roundManager->_players[dir]->GetSex()),Spawn::create(
                hide2CardsInhand),Spawn::create(
                moveRightCardInHand,
                moveLeftCardInHand,NULL),Sequence::create(
                    DelayTime::create(0.42),
                    hideOutcard,NULL),Spawn::create(
                backGoundEffect,
                move3PengCards,NULL),Sequence::create(
                DelayTime::create(0.84),
                _voice->Speak("paizhuangji"),NULL),NULL),Sequence::create(CCCallFunc::create(this,callfunc_selector(
            RaceLayer::_DeleteActionEffect)),
            callFunc_update_list,CallFunc::create([=](){
			if(myframe->getChildByTag(PENG_EFFECT_NODE_ID))
				myframe->removeChildByTag(PENG_EFFECT_NODE_ID,true);}),/*actcheckagain,*/NULL),NULL));
        
        /***************************************
            it is unneccessary for net-game???
        ***************************************/
		myframe->runAction( Sequence::create(CCCallFunc::create([=](){
        		_roundManager->UpdateCards(dir,a_PENG,card);
    			_roundManager->_actCtrl.choices = _roundManager->_players[dir]->judge_action_again();
    			_roundManager->WaitForMyAction();}),NULL));
	}
}

void RaceLayer::_AnGangEffect(PlayerDir_t dir,Card_t card,int gang[])
{
    if(dir!=MIDDLE) {
		myframe->runAction(Sequence::create(
            Spawn::create(
                _voice->SpeakAction(aAN_GANG,_roundManager->_players[dir]->GetSex()),
                simple_tip_effect(_layout->PositionOfActSign(dir),"gang.png"),NULL), CallFunc::create([=](){
			_roundManager->update_gold(dir,AN_GANG,dir);}), Sequence::create(CCCallFunc::create(this,callfunc_selector(
            RaceLayer::_DeleteActionTip)), CallFunc::create([=](){
            _roundManager->UpdateCards(dir,a_AN_GANG,card);}), CCCallFunc::create([=]() {
            _CardInHandUpdateEffect(dir);}),
            _voice->Speak("down"),CallFunc::create([=](){
            _roundManager->DistributeTo(dir
                ,(Card_t)(_roundManager->_unDistributedCards[_roundManager->_distributedNum++]));}),NULL),NULL));
	}
	else {
        int GangCardsPlace[4] = {0};//delete gang make GangCardsPlace zero
        if(gang!=NULL) {
			for(int num=0;num<4;num++)
			{
				int temp=gang[num];
				GangCardsPlace[num]=temp;
			}
            delete gang;
        }
        
		for(int NodeNum=0;NodeNum<3;NodeNum++) {
            _Remove(myframe,PENG_EFFECT_NODE_ID+NodeNum);
            
			if(ifEffectTime) {
				ifEffectTime=false;
				ifUpdateDuringEffect=false;
                //_roundManager->CancelEffectCard();
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
            move 4 cards
        **********************/
        TargetedAction *moveGangCardEffect[4];
        _CreateGangCardsMotion(moveGangCardEffect);

        /**********************
            move 4 cards in hand
        **********************/
        Size FreeCardSize = _object->RectSize(FREE_CARD);
        auto cards = _roundManager->_players[dir]->_cards; 
		auto curKindOfGang = cards->get_kind(GangCardsPlace[0]);
        
        /**********************
            restore position of new distributed card
        **********************/
        int isChosenCanceled = 0;
        int ifLeft           = 0;
        
		Sprite* baseCard;
		Vec2    basePos;
        
        if(_myChosenCard!=INVALID) {
            if(_myChosenCard<cards->size()-5)
                ifLeft=1;
        
            int oldChosen = _myChosenCard;
            _myChosenCard = cards->rechoose_after_gang(_myChosenCard,GangCardsPlace,false,GangCardsPlace[3]);
        
            if(_myChosenCard==INVALID && oldChosen!=cards->real_last()) {
                isChosenCanceled=1;
            }
        }
            
        auto Pengsize = _object->RectSize(PENG_CARD);
        
		baseCard = _GetCardInHand(MIDDLE,cards->last()-4-1);
		basePos  = baseCard->getPosition();

        if( cards->FreeStart>0 )
			distributeCardPos = Vec2(
			    basePos.x + Pengsize.width*3.5 + 30
			        + baseCard->getBoundingBox().size.width - FreeCardSize.width*isChosenCanceled*0.2*ifLeft,
			    basePos.y);
		else
			distributeCardPos = Vec2(
			    basePos.x + Pengsize.width*4 + 30
			        + baseCard->getBoundingBox().size.width - FreeCardSize.width*isChosenCanceled*0.2*ifLeft,
			    basePos.y);

        TargetedAction *moveGangCardInHand[4];
        _CreateGangCardInHandMotion(moveGangCardInHand, GangCardsPlace, (CARD_KIND)curKindOfGang);

		auto moveAngangCards = Spawn::create(
            moveGangCardEffect[0],moveGangCardEffect[1],moveGangCardEffect[2],moveGangCardEffect[3],
            moveGangCardInHand[0],moveGangCardInHand[1],moveGangCardInHand[2],moveGangCardInHand[3],NULL);

        /**********************
            move free cards in hand
        **********************/
        int actionStartPlace=0;
		for(int i=0;i<=cards->FreeStart;i++)
			if(cards->get_status(i)==c_MING_KOU || cards->get_status(i)==c_FREE) {
				actionStartPlace = i;
				break;
			}
        auto GangCardSize = _object->RectSize(AN_GANG_CARD);

        const float GAP = (cards->FreeStart==0)?(0.5):(0.0);
        
		Vector<FiniteTimeAction *>gang_list_seq;
		Spawn* moveFreeCards;

		if( card==cards->get_kind(cards->real_last()) ) {
            for(int i=actionStartPlace;i<cards->size();i++)
			{
                auto curPos = _GetCardInHand(dir,i)->getPosition();
                TargetedAction *motion=NULL;
                
				if(i<GangCardsPlace[0])
				{
					motion=TargetedAction::create( _GetCardInHand(dir,i),
                        Sequence::create(
                            DelayTime::create(0.18),
                            MoveTo::create(0.3,Vec2(
                                curPos.x+GangCardSize.width*(3.5+GAP),
                                curPos.y)),NULL));
				}
				else if(i==GangCardsPlace[0]||i==GangCardsPlace[1]||i==GangCardsPlace[2]||i==GangCardsPlace[3])
				{
					motion=TargetedAction::create(myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+dir*20+i),
                        Sequence::create(DelayTime::create(0.18),
                            ScaleTo::create(0,0),NULL));
				}
				else if(i>GangCardsPlace[2]&&i<GangCardsPlace[3])
				{
					motion = TargetedAction::create(myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+dir*20+i),
                        Sequence::create(
                            DelayTime::create(0.18),
                            MoveTo::create(0.3,Vec2(
                                curPos.x+(GangCardSize.width*(3.5+GAP)-FreeCardSize.width*(3+isChosenCanceled*0.2)),
                                curPos.y)),NULL));
				}

                gang_list_seq.insert(i-actionStartPlace,motion);
			}
		} else {
			int i=0;
			int insertPlace=0;
			auto lastCard=myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+dir*20+cards->size()-1);
			auto lastCardPos=lastCard->getPosition();
			auto lastCardSize=lastCard->getContentSize();
			for(int num=actionStartPlace;num<cards->size()-1;num++)
			{
				if(cards->get_kind(num)<=cards->get_kind(cards->size()-1)&&cards->get_kind(num+1)>cards->get_kind(cards->size()-1))
				{
					insertPlace=num+1;
					break;
				}
			}
			if(insertPlace==GangCardsPlace[3]+1)
				insertPlace=GangCardsPlace[0];
			else if(insertPlace==0)
				insertPlace=cards->size()-1;

			for(i=actionStartPlace;i<cards->size()-1;i++)
			{
                auto curPos=myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+dir*20+i)->getPosition();
				auto RightMoveWith=(i<insertPlace)?(0.0):(lastCardSize.width);
                TargetedAction *motion=NULL;
				if(i<GangCardsPlace[0]) {
					motion = TargetedAction::create(myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+dir*20+i),
                        Sequence::create(
                            DelayTime::create(0.18),
                            MoveTo::create(0.3,Vec2(
                                curPos.x + GangCardSize.width*(3.5+GAP)+RightMoveWith,
                                curPos.y)),NULL));
				} else if(i==GangCardsPlace[0]||i==GangCardsPlace[1]||i==GangCardsPlace[2]||i==GangCardsPlace[3]) {
					motion = TargetedAction::create(myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+dir*20+i),
                        Sequence::create(DelayTime::create(0.18),
                            ScaleTo::create(0,0),NULL));
				} else if(i>GangCardsPlace[3]) {
					motion = TargetedAction::create(myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+dir*20+i),
						Sequence::create(
						DelayTime::create(0.18),
						MoveTo::create(0.3,Vec2(
						curPos.x+(GangCardSize.width*(3.5+GAP)-FreeCardSize.width*(4+isChosenCanceled*0.2)+RightMoveWith),
						curPos.y)),NULL));
				}
                gang_list_seq.insert(i-actionStartPlace,motion);
			}
			auto curLeftPos = myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+dir*20+insertPlace-1)->getPosition();
			if(insertPlace==cards->size()-1)
			{
				int instanceBetween = lastCardPos.x-curLeftPos.x-FreeCardSize.width;
				auto actionMove = MoveTo::create(0.3,Vec2(
					lastCardPos.x+(GangCardSize.width*(3.5+GAP)-FreeCardSize.width*(4+isChosenCanceled*0.2))-instanceBetween,
					lastCardPos.y));
				TargetedAction *motion = TargetedAction::create(lastCard,
					Sequence::create(
					DelayTime::create(0.18),
					actionMove,NULL));
				gang_list_seq.insert(i-actionStartPlace,motion);
			}
			else
			{
				auto actionMoveUp=MoveTo::create(0.1,Vec2(lastCardPos.x,lastCardPos.y+100));
				MoveTo* actionMoveInsert=NULL;
				MoveTo* actionMoveDown=NULL;
				if(insertPlace>GangCardsPlace[0])
				{
					actionMoveInsert=MoveTo::create(0.2,Vec2(curLeftPos.x+GangCardSize.width*(3.5+GAP)-FreeCardSize.width*(3+isChosenCanceled*0.2),lastCardPos.y+100));
					actionMoveDown=MoveTo::create(0.1,Vec2(curLeftPos.x+GangCardSize.width*(3.5+GAP)-FreeCardSize.width*(3+isChosenCanceled*0.2),lastCardPos.y));
				}
				else
				{
					actionMoveInsert=MoveTo::create(0.2,Vec2(curLeftPos.x + GangCardSize.width*(3.5+GAP)+FreeCardSize.width,lastCardPos.y+100));
					actionMoveDown=MoveTo::create(0.1,Vec2(curLeftPos.x + GangCardSize.width*(3.5+GAP)+FreeCardSize.width,lastCardPos.y));
				}
				auto actionMove=Sequence::create(actionMoveUp,actionMoveInsert,actionMoveDown,NULL);
				TargetedAction *motion = TargetedAction::create(lastCard,
					Sequence::create(
					DelayTime::create(0.18),
					actionMove,NULL));
				gang_list_seq.insert(i-actionStartPlace,motion);
			}
		}
        
        moveFreeCards = Spawn::create(gang_list_seq);

        /**********************
            update tag
        **********************/
		for(int i=cards->size()-1;i>=0;i--) {
			int curTag=HAND_IN_CARDS_TAG_ID+1*20+i;
            
			if(!myframe->getChildByTag(curTag))
				continue;
            
			auto EveryCard=(Sprite*)myframe->getChildByTag(curTag);
            
			if( GangCardsPlace[0]<cards->FreeStart ) {/* peng cards transfer to gang cards */
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
				} else if(i>=cards->FreeStart && i<GangCardsPlace[0]) {
					EveryCard->setTag(curTag+4);      /* leave for 4 space for gang cards*/
                    
                    if(i==cards->FreeStart) {       /* insert 4 gang cards*/
                        for(int b=0;b<4;b++)
                            ((Sprite*)myframe->getChildByTag(EFFECT_TEMP_CARD_TAG_ONE+b))->setTag(HAND_IN_CARDS_TAG_ID+1*20+cards->FreeStart+b);
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
					for(int a=cards->FreeStart-1;a>=0;a--)
					{
						if( _roundManager->IsCurEffectCard(cards->at(a)) )
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
                    _voice->SpeakAction(aAN_GANG,
                        _roundManager->_players[dir]->GetSex()),
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
                    RaceLayer::_DeleteActionEffect)),
                    callFunc_update_list,
                    _voice->Speak("lanpai"),NULL),CallFunc::create([=](){
                _Remove(myframe,AN_GANG_EFFECT_NODE);}),NULL));

		myframe->_ID=MIDDLE;
		myframe->runAction(Sequence::create(CallFunc::create([=](){
            _roundManager->UpdateCards(MIDDLE,a_AN_GANG);}),DelayTime::create(0.48),CallFunc::create([=](){
			_roundManager->update_gold(dir,AN_GANG,dir);}),CallFunc::create([=](){
            _roundManager->DistributeTo(dir,(Card_t)(_roundManager->_unDistributedCards[_roundManager->_distributedNum++]));}),NULL));
		
			
			
			//myframe->runAction(Sequence::create(CallFunc::create([=](){
  //          _roundManager->UpdateCards(MIDDLE,a_AN_GANG);}),CCCallFunc::create([=]() {//ERROR_YUSI
  //          _ReOrderCardsInHand(0,_roundManager->_players[MIDDLE]->_cards);}),
  //          DelayTime::create(0.48), CallFunc::create([=](){
		//	_roundManager->update_gold(dir,AN_GANG,dir);}),CallFunc::create([=](){
  //          _roundManager->DistributeTo(dir
  //              ,(Card_t)(_roundManager->_unDistributedCards[_roundManager->_distributedNum++]));}),NULL));
	}
}

void RaceLayer::_MingGangEffect(PlayerDir_t dir,PlayerDir_t prevDir, Card_t card,int gang[])
{
    myframe->_ID = dir;

	if(!_roundManager->_isNewDistributed) {
        _Remove(myframe,HAND_OUT_CARDS_TAG_ID+prevDir*25 + _roundManager->_players[prevDir]->_river->size());
	}
    
    if(dir!=MIDDLE) {
        _OthersMingGangEffect(dir,prevDir,_roundManager->_isNewDistributed,card);
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

        CardInHand *cards = _roundManager->_players[MIDDLE]->_cards; 
        
		if( _roundManager->_isNewDistributed ) {
			auto lastInHand = (Sprite*)myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+dir*20 + cards->real_last());//gang1
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
            restore position of new distributed card
        **********************/
        int isChosenCanceled = 0;
        int ifLeft           = 0;
        
        if(_myChosenCard!=INVALID) {
            if(_myChosenCard < cards->size()-4) {
                ifLeft=1;
            }

            _myChosenCard = cards->rechoose_after_gang(_myChosenCard,gang,_roundManager->_isNewDistributed);
            if(_myChosenCard==INVALID) {
                isChosenCanceled=1;
            }
        }

		Sprite* baseCard;
		Vec2    basePos;
        
		if(_roundManager->_isNewDistributed) {
			baseCard = _GetCardInHand(MIDDLE,cards->last()-1);
			basePos  = baseCard->getPosition();
            
			distributeCardPos=Vec2(
                basePos.x + baseCard->getBoundingBox().size.width - FreeCardSize.width*isChosenCanceled*0.2,
                basePos.y);
		} else {
            auto Pengsize = _object->RectSize(PENG_CARD);
            
			baseCard = _GetCardInHand(MIDDLE,cards->last()-4-1);/*include others' card*/
			basePos  = baseCard->getPosition();

            if( cards->FreeStart>0 )
				distributeCardPos = Vec2(
				    basePos.x + Pengsize.width*3.5 + 30
				        + baseCard->getBoundingBox().size.width - FreeCardSize.width*isChosenCanceled*0.2*ifLeft,
				    basePos.y);
			else
				distributeCardPos = Vec2(
				    basePos.x + Pengsize.width*4 + 30
				        + baseCard->getBoundingBox().size.width - FreeCardSize.width*isChosenCanceled*0.2*ifLeft,
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
		if(_roundManager->_isNewDistributed) {
			show_card->setPosition(_layout->OrigPositionOfGangCard(0,cardPengSize));
		} else {
			show_card->setPosition(s_curOutCard->getPosition());
		}
		show_card->setScale(0);
		myframe->addChild(show_card,29,SINGLE_ACTION_EFFECT_TAG_ID);

		if(!_roundManager->_isNewDistributed) {
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
				    origin.y+visibleSize.height*0.255)),NULL)),NULL;/* the real_last NULL is non effect, BUG HERE???*/
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

        if(_roundManager->_isNewDistributed) {
            _CreateMingGangCardInHandMotion(gangCardInHandMotion,gang,(CARD_KIND)card);
		} else {
		    _CreateFreeCard(gangCard,gang,(CARD_KIND)card);

            if(cards->FreeStart>=0)/* FreeStart can less than 0 ???? */
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
		for(int a=0;a<=cards->FreeStart;a++)
			if(cards->get_status(a)==c_MING_KOU||cards->get_status(a)==c_FREE)
			{
				actionStartPlace=a;
				break;
			}
		actionStartPlace = (actionStartPlace>gang[0])?gang[0]:actionStartPlace;
            
		Vector<FiniteTimeAction *>gang_list_seq;
        
		if(_roundManager->_isNewDistributed) {
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
            const float GAP = (cards->FreeStart==0)?0.5:0.0;
            
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
            for(int i=gang[2]+1;i<cards->size()-1;i++) {/* right shift */
				auto curPos=myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+dir*20+i)->getPosition();
                
                gang_list_seq.insert(i-actionStartPlace,TargetedAction::create(
                    myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+dir*20+i),Sequence::create(
                        DelayTime::create(delayTime),
                        MoveTo::create(0.3,Vec2(
                            curPos.x+cardPengSize.width*(3.5+GAP)-FreeCardSize.width*(3+isChosenCanceled*0.2),
                            curPos.y)),NULL)));
			}
		}

        Spawn *moveFreeCards = Spawn::create(gang_list_seq);
        /**********************
            update tag
        **********************/
		for(int a=cards->real_last();a>=0;a--) {
			int curTag=HAND_IN_CARDS_TAG_ID+1*20+a;
			if(!myframe->getChildByTag(curTag))
				continue;
            
			auto EveryCard=(Sprite*)myframe->getChildByTag(curTag);
			if(_roundManager->_isNewDistributed) {
				if(a==cards->real_last())
					EveryCard->setTag(EFFECT_TEMP_CARD_TAG_FOUR);
				else if(a<cards->real_last()&&a>gang[2])
					EveryCard->setTag(curTag+1);
                
				if(a==(gang[2]+1)) {
					((Sprite*)myframe->getChildByTag(EFFECT_TEMP_CARD_TAG_FOUR))->setTag(HAND_IN_CARDS_TAG_ID+1*20+gang[2]+1);
				}
			} else {
				if(gang[0]<cards->FreeStart) {
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
					} else if(a<gang[0]&&a>=cards->FreeStart)
						EveryCard->setTag(curTag+4);
                    
					if(a==cards->FreeStart)
					{
						for(int b=0;b<3;b++)
							((Sprite*)myframe->getChildByTag(EFFECT_TEMP_CARD_TAG_ONE+b))->setTag(HAND_IN_CARDS_TAG_ID+1*20+cards->FreeStart+b);

						auto EmptyCard=_object->Create(FREE_CARD);
						EmptyCard->setAnchorPoint(Vec2(1,1));
						EmptyCard->setScale(0);
						EmptyCard->setPosition(Vec2::ZERO);
						myframe->addChild(EmptyCard,1,HAND_IN_CARDS_TAG_ID+1*20+cards->FreeStart+3);
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
					for(int a=cards->FreeStart-1;a>=0;a--)
					{
						if( _roundManager->IsCurEffectCard(cards->at(a)) )
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
                _voice->SpeakAction(aMING_GANG,_roundManager->_players[dir]->GetSex()),
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
                RaceLayer::_DeleteActionEffect)),
                callFunc_update_list,CallFunc::create([=](){
                _Remove(myframe,MING_GANG_EFFECT_NODE);}),NULL),NULL));

		myframe->runAction(Sequence::create(CCCallFunc::create([=]() {
            _roundManager->UpdateCards(dir,a_MING_GANG);}),
            DelayTime::create(0.48),CallFunc::create([=](){
			if(!_roundManager->_isNewDistributed)
				_roundManager->update_gold(dir,MING_GANG,prevDir);}),CallFunc::create([=](){
            _roundManager->ActionAfterGang(dir);}),NULL));
	}
}

/* why use different mechanism for single hu and double hu ??? */
void RaceLayer::_HuEffect(const WinInfo_t &win)
{
	((Button*)this->getChildByTag(MENU_BKG_TAG_ID)->getChildByTag(TUOGUAN_MENU_BUTTON))->setTouchEnabled(false);

    _Remove(this,ROBOT_TUO_GUAN);
    _Remove(this,TUOGUAN_CANCEL_BUTTON);
    
	scheduleOnce(schedule_selector(RaceLayer::raceAccount),3);

    PlayerDir_t winner = win.winner;
    PlayerDir_t giver  = win.giver;

	if(win.kind!=DOUBLE_WIN) {
        Sequence *backgroundEffect = (!_roundManager->IsMing(MIDDLE)&&(winner==MIDDLE))
            ? Sequence::create(
                    _HideReminder(HU_REMIND_ACT_TAG_ID, 0.3, 1.5),
                    _CreateHuBackgroundEffect(winner),NULL)
            : Sequence::create(
                    _CreateHuBackgroundEffect(winner),NULL);
        
		myframe->runAction(Spawn::create(
                simple_tip_effect(_layout->PositionOfActSign(winner),"dahu.png"),
                _voice->SpeakAction(aHU,_roundManager->_players[winner]->GetSex()),CallFunc::create([=](){
				_roundManager->update_gold(winner,HU_WIN,giver);}),CallFunc::create(this,callfunc_selector(
                RaceLayer::showall)),
                backgroundEffect,NULL));
	} else {
		ListenToDoubleHu();
	}
}

void RaceLayer::_QiEffect(PlayerDir_t dir) {
	myframe->_ID = MIDDLE;

    /********************************
        hide reminder
    ********************************/
	auto hideQiReminder =_HideQiReminder();
    
    /********************************
        
    ********************************/
	if(myframe->getChildByTag(QI_REMIND_ACT_BKG_TAG_ID)!=NULL && dir==MIDDLE) {/*??? is this judgement neccessary */
		if(_roundManager->_isNewDistributed ||
            _roundManager->_lastActionSource==MIDDLE) {
			if(_roundManager->player_can_gang()) {
				myframe->runAction(Sequence::create(
                    hideQiReminder,CallFunc::create([=](){
                        int real_last = _roundManager->_players[MIDDLE]->_cards->real_last();
                        Vec2 location = _GetCardInHand(MIDDLE,real_last)->getPosition();
                        _roundManager->RecvHandout(real_last,location,2);/*bug??? forced to handout real_last card*/
                    }),NULL));
			} else {
				myframe->runAction(hideQiReminder);
            }
		} else {
			if(_roundManager->_isQiangGangAsking) {
				_roundManager->_isQiangGangAsking=false;

				myframe->runAction(Sequence::create(
                    hideQiReminder,Spawn::create(CallFunc::create([=](){
    					_roundManager->update_gold((PlayerDir_t)_roundManager->_qiangGangTargetNo,MING_GANG,(PlayerDir_t)_roundManager->_curPlayer);
    					_roundManager->_qiangGangTargetNo = INVALID;/*!!! could this be called before runAction */}),CallFunc::create([=](){
                        _roundManager->DistributeTo((PlayerDir_t)_roundManager->_curPlayer
                            ,(Card_t)(_roundManager->_unDistributedCards[_roundManager->_distributedNum++]));}),NULL),NULL));
			} else if(_roundManager->_isDoubleHuAsking) {
				_roundManager->_isDoubleHuAsking=false;
                
				myframe->runAction(Sequence::create(
                    hideQiReminder,CallFunc::create([=](){
                    PlayerDir_t giver = (_roundManager->_otherOneForDouble==LEFT)?RIGHT:LEFT;
                    _roundManager->SetWin(DOUBLE_WIN,giver);

                    _HuEffect(_roundManager->GetWin());}),NULL));
			} else {
				myframe->runAction(Sequence::create(
                    hideQiReminder,CallFunc::create([=](){
                    _roundManager->DistributeTo(_roundManager->TurnTo(NEXT)
                        ,(Card_t)(_roundManager->_unDistributedCards[_roundManager->_distributedNum++]));}),NULL));
			}
		}
	}
}

/********************************************************
            cards
********************************************************/
/* this function is only for others */
Sprite *RaceLayer::_CreateCardInHand(PlayerDir_t dir,int idx,
                                        CardList *cards,bool isTing,const Vec2 &refer) {
    switch( cards->get_status(idx) ) {
        case c_FREE:
            {
                Sprite *card;
                
                if(isTing) {
                    card = _object->Create(LR_OUT_CARD,dir,refer.x,refer.y);
                } else if(!isTing&&_roundManager->IsMing(MIDDLE)) {
                    card = _object->Create(LR_AN_GANG_CARD,dir,refer.x,refer.y);
                } else {
                    TextureId_t texture = (dir==LEFT)?L_IN_CARD:R_IN_CARD;
                    card = _object->Create(texture,dir,refer.x,refer.y);
                    card->setAnchorPoint(_layout->AnchorOfFreeCard(dir));
                }

                return card;
            }
        case c_AN_GANG:
            if(cards->get_idx_in_group(idx)==3 && !isTing&&_roundManager->IsMing(MIDDLE))
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

float RaceLayer::_YofNextCard(PlayerDir_t dir,int idx,CardList *cards,bool isTing,float refY) {//更新手牌
    float up = (dir==LEFT)?(-1):1;
    float groupGap = (dir==LEFT)?0.4:0.8;
    
    switch(cards->get_status(idx)) {
        case c_FREE:
            if(isTing||_roundManager->IsMing(MIDDLE)) {
                return up*(refY*0.65);
            } else {
                return up*(refY*0.5);
            }
        case c_MING_KOU:
            return up*(refY*0.65);
        case c_PENG:
            if(isTing||_roundManager->IsMing(MIDDLE)) {
                if(cards->get_status(idx+1)==c_FREE||cards->get_status(idx+1)==c_MING_KOU)
                    return up*refY;
                else if(cards->get_kind(idx+1)!=cards->get_kind(idx))
                    return up*(refY*0.65+5);
                else 
                    return up*(refY*0.65);
            } else {
                if(cards->get_status(idx+1)==c_FREE)
                    return up*(refY*groupGap);
                else if(cards->get_kind(idx+1)!=cards->get_kind(idx))
                    return up*(refY*0.65+5);
                else
                    return up*(refY*0.65);
            }
        case c_MING_GANG:
        case c_AN_GANG:
            int groupIdx = cards->get_idx_in_group(idx);

            if( (dir==LEFT&&groupIdx==1) || (dir==RIGHT&&groupIdx==3)) {
                return up*(refY*0.65);
            } else if (groupIdx==2) {
                return -up*13;
            } else if( (dir==LEFT&&groupIdx==3) || (dir==RIGHT&&groupIdx==1)) {
                return up*(refY*0.65+13);
            } else {
                if(_roundManager->IsMing(MIDDLE)||isTing==1) {
                    if(cards->get_status(idx+1)==c_FREE||cards->get_status(idx+1)==c_MING_KOU)
                        return up*refY;
                    else
                        return up*(refY*0.65+5);
                } else {
                    if(cards->get_status(idx+1)==c_FREE)
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
void RaceLayer::_UpdateTingNum(PlayerDir_t dir) {
    TingInfo_t *ting = _roundManager->_players[dir]->_cards->_ting;

    if(ting!=NULL) {
        for(int i=0;i<ting->cardNum;i++) {
            Sprite* curCardBar = _GetCardOnTingSignBar(dir,i);
            curCardBar->removeChildByTag(2);
        
            auto residueNum = _CreateNumberSign((ting->cards+i)->remain);
            curCardBar->addChild(residueNum,1,2);
        }
    }
}

void RaceLayer::TingHintBarOfOthers(int curNo,int outCardIdx) {
    TingInfo_t *ting = _roundManager->_players[curNo]->_cards->_ting;
    if(ting!=NULL) {
        Card_t huCards[9];
        for(int i=0;i<ting->cardNum;i++) {
            huCards[i] = (ting->cards+i)->kind;
        }

        auto tingSignBar = _object->CreateTingSignBar((PlayerDir_t)curNo,huCards,ting->cardNum);
        myframe->addChild(tingSignBar,30,TING_SING_LEFTBAR+curNo/2);
    }
}

void RaceLayer::_TingHintCreate(Point curPos,int CardPlace)
{
    Hu_cardOut_place = CardPlace;

    CardInHand *cards = _roundManager->_players[MIDDLE]->_cards;
    
    int    choiceIdx = CardPlace - cards->FreeStart - cards->_alter->activated_cards_num();
    TingInfo_t *ting = cards->get_ting_info(CardPlace);

    if(ting!=NULL) {
        Card_t huCards[9];
        int    times[9];
        int    remains[9];
        for(int i=0;i<ting->cardNum;i++) {
            huCards[i] = (ting->cards+i)->kind;
            times[i]   = (ting->cards+i)->fan;
            remains[i] = (ting->cards+i)->remain;
        }
        
        auto tingSignBar = _object->CreateTingInfoBar(
            curPos,huCards,ting->cardNum,times,remains);
        myframe->addChild(tingSignBar, 30, TING_SING_BAR);
    }
}

void RaceLayer::BtnGangCancelHandler(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
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

            _roundManager->RecvGangCancel();
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}	
}

void RaceLayer::BtnGangConfirmHandler(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
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
			_Remove(myframe,GANG_CANCEL);
			curButton->setTouchEnabled(false);

            _roundManager->RecvGangConfirm();
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void RaceLayer::GangCancelEffect(ActionId_t action,CardInHand *cards) {
    for(int i=cards->FreeStart;i<cards->size();i++) {
        Sprite *card = _GetCardInHand(MIDDLE,i);
        _Remove(card,MING_KOU);
    }

    Node *button = myframe->getChildByTag(GANG_CANCEL);
    
    myframe->_ID = MIDDLE;
    myframe->runAction(
        Sequence::create(
            TargetedAction::create(button,ScaleTo::create(0,0)),CallFunc::create([=](){
            ListenToCardTouch();}),NULL));
}

void RaceLayer::GangConfirmEffect() {
    auto button = myframe->getChildByTag(MING_KOU_ENSURE);

    myframe->_ID = MIDDLE;
    myframe->runAction(Sequence::create(CCCallFunc::create(this,callfunc_selector(
        RaceLayer::_DeleteActionTip)),CallFunc::create([=](){
        _roundManager->_RecvGangConfirm();}),NULL));
}

/****************************************
        ming
****************************************/
void RaceLayer::KouCancelEffect(ActionId_t action,CardInHand *cards) {
    for(int i=cards->FreeStart;i<cards->size();i++) {
        Sprite *card = _GetCardInHand(MIDDLE,i);
        _Remove(card,MING_KOU);
    }

    Node *button = myframe->getChildByTag(MING_KOU_CANCEL);
    
    myframe->_ID = MIDDLE;
    myframe->runAction(
        Sequence::create(
            TargetedAction::create(button,ScaleTo::create(0,0)),CallFunc::create([=](){
            _SwitchCancelBtn(MING_CANCEL);
            _roundManager->RecvMing(true);}),NULL));
}

void RaceLayer::KouConfirmEffect() {
    auto button = myframe->getChildByTag(MING_KOU_ENSURE);

    myframe->_ID = MIDDLE;
    myframe->runAction(Sequence::create(CCCallFunc::create([=]() {
        _CardInHandUpdateEffect(MIDDLE);}),CCCallFunc::create(this,callfunc_selector(
        RaceLayer::_DeleteActionTip)),CallFunc::create([=](){
        _roundManager->RecvMing(true);}),NULL));
}

void RaceLayer::MingCancelEffect() {
    auto button = myframe->getChildByTag(MING_CANCEL);
    
    myframe->_ID = MIDDLE;
    myframe->runAction(Sequence::create(TargetedAction::create(
        button,ScaleTo::create(0,0)),CCCallFunc::create([=]() {
        _CardInHandUpdateEffect(MIDDLE);}),CCCallFunc::create(this,callfunc_selector(
        RaceLayer::_DeleteActionTip)),CallFunc::create([=](){
        _roundManager->_actCtrl.choices = _roundManager->_players[MIDDLE]->judge_action_again();
        _roundManager->WaitForMyAction();}),NULL));
}

void RaceLayer::BtnKouCancelHandler(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
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

void RaceLayer::BtnKouConfirmHandler(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
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

void RaceLayer::MingCancelHandler(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
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

bool RaceLayer::_KouTouchBegan(Touch* touch, Event* event) {
    CardInHand *cards=_roundManager->_players[MIDDLE]->_cards;

    Sprite *cardsInHand[MAX_HANDIN_NUM] = {0};
    int groupChosen=INVALID;
    
    for(int i=0;i<cards->size();i++) {
        cardsInHand[i]=_GetCardInHand(MIDDLE,i);
        cardsInHand[i]->_ID = MIDDLE;
    }
    
    for(int group=0; group<cards->_alter->group_num(); group++) {
        for(int i=0; i<cards->_alter->cards_num(group); i++) {
            if ( _IsClickedOn(cardsInHand[cards->_alter->get_card_idx(group,i)], touch) ) {
                groupChosen = group;
                break;
            }
        }
    }
    
    if(groupChosen!=INVALID) {
        for(int i=0; i<cards->_alter->cards_num(groupChosen); i++) {
            cardsInHand[cards->_alter->get_card_idx(groupChosen,i)]->_ID++;/*==2???*/
        }
    }
    
    return true;
}

int RaceLayer::_FindChosenGroup(Touch *touch,Sprite *cardsInHand[]) {
    CardInHand *cards = _roundManager->_players[MIDDLE]->_cards;

    for(int group=0; group<cards->_alter->group_num(); group++) {
        for(int i=0;i<cards->_alter->cards_num(group);i++) {
            if ( _IsClickedOn(cardsInHand[cards->_alter->get_card_idx(group,i)],touch) ) {
                int idx = cards->_alter->get_card_idx(group,i);
                if(cardsInHand[idx]->_ID!=1) {
                    return group;
                }
            }
        }
    }

    return INVALID;
}

void RaceLayer::_KouTouchEnded(Touch* touch, Event* event) {
    CardInHand *cards = _roundManager->_players[MIDDLE]->_cards;
    
    Sprite *cardsInHand[MAX_HANDIN_NUM];
    for(int i=0; i<cards->size(); i++) {
        cardsInHand[i]=_GetCardInHand(MIDDLE,i);
    }
    
    int groupChosen = _FindChosenGroup(touch,cardsInHand);
    
    if(groupChosen!=INVALID) {
        cards->_alter->switch_status(groupChosen);
    }

    cards->_alter->refresh();
    
    for(int group=0;group<cards->_alter->group_num();group++) {
        if(cards->_alter->is_activated(group)) {
            for(int i=0;i<cards->_alter->cards_num(group);i++) {
                _Remove(cardsInHand[cards->_alter->get_card_idx(group,i)],MING_KOU);
                _Remove(cardsInHand[cards->_alter->get_card_idx(group,i)],MING_KOU_MASK);
                
                auto MingKouMark=_object->Create(MING_KOU_CARD);
                MingKouMark->setAnchorPoint(Vec2(0.5,0.5));
                MingKouMark->setPosition(Vec2(cardsInHand[cards->_alter->get_card_idx(group,i)]->getTextureRect().size.width/2,cardsInHand[cards->_alter->get_card_idx(group,i)]->getTextureRect().size.height/2));
                cardsInHand[cards->_alter->get_card_idx(group,i)]->addChild(MingKouMark,2,MING_KOU);
            }
        } else if(cards->_alter->get_status(group)==sFREE) {
            for(int i=0;i<cards->_alter->cards_num(group);i++) {
                _Remove(cardsInHand[cards->_alter->get_card_idx(group,i)],MING_KOU);
                _Remove(cardsInHand[cards->_alter->get_card_idx(group,i)],MING_KOU_MASK);
                
                auto KouNo=_object->Create(MING_MASK_CARD);
                KouNo->setAnchorPoint(Vec2(0.5,0.5));
                KouNo->setPosition(Vec2(cardsInHand[cards->_alter->get_card_idx(group,i)]->getTextureRect().size.width/2,cardsInHand[cards->_alter->get_card_idx(group,i)]->getTextureRect().size.height/2));
                cardsInHand[cards->_alter->get_card_idx(group,i)]->addChild(KouNo,2,MING_KOU_MASK);
            }
        } else {
            for(int i=0;i<cards->_alter->cards_num(group);i++) {
                _Remove(cardsInHand[cards->_alter->get_card_idx(group,i)],MING_KOU);
                _Remove(cardsInHand[cards->_alter->get_card_idx(group,i)],MING_KOU_MASK);
            }
        }
    }
    
    bool ifEnsureVisible=false;
    for(int group=0;group<4;group++) {
        if(cards->_alter->is_activated(group)) {
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

void RaceLayer::ListenToKou(int no) {
    auto KouListener = EventListenerTouchOneByOne::create();
    KouListener->setSwallowTouches(true);
    
    KouListener->onTouchBegan = CC_CALLBACK_2(RaceLayer::_KouTouchBegan,this);
    KouListener->onTouchMoved = [=](Touch* touch, Event* event) {};
    KouListener->onTouchEnded = CC_CALLBACK_2(RaceLayer::_KouTouchEnded,this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(KouListener, myframe);
}

void RaceLayer::_SwitchCancelBtn(int tag) {
    _Remove(myframe,MING_KOU_CANCEL);
    _Remove(myframe,MING_CANCEL);
    _Remove(myframe,GANG_CANCEL);

    auto btn = _object->CreateButton(BTN_CANCEL);

    switch(tag) {
        case MING_KOU_CANCEL:
            btn->addTouchEventListener(CC_CALLBACK_2(RaceLayer::BtnKouCancelHandler,this));
            break;
        case MING_CANCEL:
            btn->addTouchEventListener(CC_CALLBACK_2(RaceLayer::MingCancelHandler,this));
            break;
        case GANG_CANCEL:
            btn->addTouchEventListener(CC_CALLBACK_2(RaceLayer::BtnGangCancelHandler,this));
            break;
    }
    
    myframe->addChild(btn,20,tag);
}

void RaceLayer::QueryGangCards() {
    _DeleteActionTip();
    _eventDispatcher->removeEventListenersForTarget(myframe,true);

    myframe->addChild(_object->CreateMingKouSign(),20,MING_KOU_SIGN);
    
    auto ChooseEnsure = _object->CreateButton(BTN_OK);
    ChooseEnsure->addTouchEventListener(CC_CALLBACK_2(RaceLayer::BtnGangConfirmHandler,this));
    ChooseEnsure->setVisible(false);
    myframe->addChild(ChooseEnsure,20,MING_KOU_ENSURE);
    
    _SwitchCancelBtn(GANG_CANCEL);
    MaskNon(sGANG_ENABLE);
    ListenToKou(MIDDLE);
}

void RaceLayer::QueryKouCards() {
    myframe->addChild(_object->CreateMingKouSign(),20,MING_KOU_SIGN);
    
    auto ChooseEnsure = _object->CreateButton(BTN_OK);
    ChooseEnsure->addTouchEventListener(CC_CALLBACK_2(RaceLayer::BtnKouConfirmHandler,this));
    ChooseEnsure->setVisible(false);
    myframe->addChild(ChooseEnsure,20,MING_KOU_ENSURE);
    
    _SwitchCancelBtn(MING_KOU_CANCEL);
    MaskNon(sKOU_ENABLE);
    ListenToKou(MIDDLE);
}

void RaceLayer::QueryMingOutCard() {
    _Remove(myframe,MING_KOU_ENSURE);
    _Remove(myframe,MING_KOU_SIGN);
    _Remove(myframe,MING_KOU_CANCEL);
    
    _SwitchCancelBtn(MING_CANCEL);
    
    myframe->_ID=MIDDLE;
    myframe->runAction(Sequence::create(CCCallFunc::create([=]() {
        _CardInHandUpdateEffect(MIDDLE);}),CCCallFunc::create(this,callfunc_selector(
        RaceLayer::ListenToCardTouch)),NULL));
}

void RaceLayer::BtnMingHandler(cocos2d::Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
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
			_roundManager->_actCtrl.decision = aMING;
            
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
                RaceLayer::_DeleteActionTip)),CCCallFunc::create([=](){
                _CancelChosenCardInHand();
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

void RaceLayer:: display_callback(cocos2d::Ref* pSender) {
    LOGGER_WRITE("%s",__FUNCTION__);
}

/*****************************************************
        player information
*****************************************************/
std::string RaceLayer::_NumToString( int number ) {
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

void RaceLayer::_GuiUpdateScore(LayerColor *layer,int score) {
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

void RaceLayer::GuiUpdateScore(int dir,int score)
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

void RaceLayer::_UpdateNickName(int direction,std::string str_Nick)
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

void RaceLayer::_UpdateHeadImage(int direction,std::string head_photo)
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

void RaceLayer::GuiJinBiShow(PlayerDir_t dir, int gold) {
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

/***********************************************************
        gold show
***********************************************************/
Label *RaceLayer::_CreateName(const char *name) {
    auto NickName=Label::create(name,"Arial",28);
    NickName->setAnchorPoint(Vec2(0.5,0.5));
    NickName->setPosition(Vec2(70,60));
    return NickName;
}

Sprite *RaceLayer::_CreateHeadImage(const char *file) {
    auto headPhoto=Sprite::createWithSpriteFrameName(file);//ͷ��4
    headPhoto->setScaleX(100/headPhoto->getTextureRect().size.width);
    headPhoto->setScaleY(100/headPhoto->getTextureRect().size.height);
    headPhoto->setAnchorPoint(Vec2(0,0));
    headPhoto->setPosition(Vec2(20,80));
    return headPhoto;
}

Sprite *RaceLayer::_CreateGoldImage() {
    auto gold = Sprite::createWithSpriteFrameName("result_money.png");//���1
    gold->setAnchorPoint(Vec2(0,0.5));
    gold->setPosition(Vec2(15,22));
    return gold;
}

LabelAtlas *RaceLayer::_CreatePropertyNumber(int number,Sprite *gold) {
    std::string strNumber = _NumToString(number);
    auto label = LabelAtlas::create(strNumber,"fonts/result_money_number.png",17,23,'.');
    label->setAnchorPoint(Vec2(0,0.5));
    label->setPosition(Vec2(gold->getPosition().x+gold->getTextureRect().size.width+5, 22));
    return label;
}

Sprite *RaceLayer::_CreatePropertyUnit(int number,LabelAtlas *label) {
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

Sprite *RaceLayer::_CreateHu() {
    auto Win=Sprite::createWithSpriteFrameName("jiesuanhu.png");//��5
    Win->setAnchorPoint(Vec2(0.5,0.5));
    Win->setPosition(Vec2(origin.x+visibleSize.width*0.9,origin.y+visibleSize.height*0.15363));
    Win->setVisible(false);
    return Win;
}

Sprite *RaceLayer::_CreateZiMo() {
    auto selfDrawn=Sprite::createWithSpriteFrameName("jiesuanzimo.png");//����6
    selfDrawn->setAnchorPoint(Vec2(0.5,0.5));
    selfDrawn->setPosition(Vec2(origin.x+visibleSize.width*0.9,origin.y+visibleSize.height*0.15363));
    selfDrawn->setVisible(false);
    return selfDrawn;
}

Sprite *RaceLayer::_CreateFangPao() {
    auto PointGun1=Sprite::createWithSpriteFrameName("jiesuanfangpao.png");//����7
    PointGun1->setAnchorPoint(Vec2(0.5,0.5));
    PointGun1->setPosition(Vec2(origin.x+visibleSize.width*0.9,origin.y+visibleSize.height*0.15363));
    PointGun1->setVisible(false);
    return PointGun1;
}

Sprite *RaceLayer::_CreateBaozhuang() {
	auto BaoZhuang=Sprite::createWithSpriteFrameName("jiesuanbaozhuang.png");//包庄
	BaoZhuang->setAnchorPoint(Vec2(0.5,0.5));
	BaoZhuang->setPosition(Vec2(origin.x+visibleSize.width*0.9,origin.y+visibleSize.height*0.15363));
	BaoZhuang->setVisible(false);
    return BaoZhuang;
}

void RaceLayer::_CreateAccountPanel(const UserProfile_t &profile, Node *parent) {
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

Sprite *RaceLayer::_CreateSymbol(PlayerDir_t dir,int gold,LayerColor *parent) {
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

LabelAtlas *RaceLayer::_CreatePropertyChange(PlayerDir_t dir,int gold,LayerColor *parent) {
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

void RaceLayer::_ExposeCards(PlayerDir_t dir,const WinInfo_t &win,LayerColor *parent) {
	auto posOfCards = Vec2(162,180);//牌0，1
	float x = posOfCards.x;
	float y = posOfCards.y - _object->RectSize(PENG_CARD).height/2;
    
	Sprite *show_card_list[MAX_HANDIN_NUM];

    CardInHand *cards = _roundManager->_players[dir]->_cards;
    
	for(int i=0;i<cards->size();i++)
	{
		show_card_list[i]=_object->Create(PENG_CARD);
		show_card_list[i]->setAnchorPoint(Vec2(0,0.5));
		show_card_list[i]->setPosition(Vec2(x,y));
		parent->addChild(show_card_list[i],2);
		auto s_card=_object->CreateKind((Card_t)cards->get_kind(i),MIDDLE_SIZE);
		s_card->setAnchorPoint(Vec2(0.5,0.5));
		s_card->setPosition(Vec2(show_card_list[i]->getTextureRect().size.width/2,show_card_list[i]->getTextureRect().size.height*0.6));
		show_card_list[i]->addChild(s_card,1);

        
		if(cards->get_status(i)==c_PENG||cards->get_status(i)==c_MING_KOU)
		{
			if(cards->get_kind(i)!=cards->get_kind(i+1))
			{
				x+=show_card_list[i]->getTextureRect().size.width*1.1;
			}
			else if(cards->get_kind(i)==cards->get_kind(i+1) && cards->get_status(i+1)==c_FREE)
			{
				x+=show_card_list[i]->getTextureRect().size.width*1.1;
			}
			else
			{
				x+=show_card_list[i]->getTextureRect().size.width*0.95;
			}
		}
		else if(cards->get_status(i)==c_MING_GANG||cards->get_status(i)==c_AN_GANG)
		{
			if(cards->get_kind(i)!=cards->get_kind(i+1))
			{
				x+=show_card_list[i]->getTextureRect().size.width*1.1;
			}
			else
			{
				x+=show_card_list[i]->getTextureRect().size.width*0.95;
			}
		}
		else if(cards->get_status(i)==c_FREE)
		{
			if( win.kind==SINGLE_WIN && win.winner==dir && win.giver==dir && i==(cards->size()-2) )
				x=x+show_card_list[i]->getTextureRect().size.width*0.95+30;
			else
				x+=show_card_list[i]->getTextureRect().size.width*0.95;
		}
	}

    if((win.kind==SINGLE_WIN && win.winner==dir) || (win.kind==DOUBLE_WIN && win.giver!=dir))
    {
        if(!_roundManager->_isNewDistributed)
        {
            auto winCard=_object->Create(PENG_CARD);
            winCard->setAnchorPoint(Vec2(0,0.5));
            winCard->setPosition(Vec2(x+30,y));
            auto s_card=_object->CreateKind(cards->get_kind(cards->real_last()),MIDDLE_SIZE);/*!!! the real_last card should be the one from others*/
            s_card->setAnchorPoint(Vec2(0.5,0.5));
            s_card->setPosition(Vec2(winCard->getTextureRect().size.width/2,winCard->getTextureRect().size.height*0.6));
            winCard->addChild(s_card,1);

            parent->addChild(winCard,2);
        }
    }
}

void RaceLayer::AccountShows(LayerColor* BarOfPlayer,int no) {
	_CreateAccountPanel(_roundManager->_players[no]->_profile,BarOfPlayer);

    const WinInfo_t &win = _roundManager->GetWin();

	if( !(win.kind==NONE_WIN && _roundManager->_firstMingNo==INVALID) ) {
        int gold[3] = {0};

        _roundManager->get_ending_gold(gold);
        
        auto sign = _CreateSymbol((PlayerDir_t)no,gold[no],BarOfPlayer);
        BarOfPlayer->addChild(sign,2,ACCOUNT_DIANPAO_FONT);

        auto goldChange = _CreatePropertyChange((PlayerDir_t)no,gold[no],BarOfPlayer);
        BarOfPlayer->addChild(goldChange,2,ACCOUNT_WINGOLD_NUM);
	}

    _ExposeCards((PlayerDir_t)no,win,BarOfPlayer);
    
	int tagNum = BarOfPlayer->getTag();
	if(_roundManager->IsMing(tagNum)&&
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

void RaceLayer::AccountHuKind(LayerColor* BarOfPlayer,int num)
{
	//float x=origin.x+visibleSize.width*0.17;
	float x = 162;
	float y = origin.y+visibleSize.height*0.1256-10;

	int tagNum    = BarOfPlayer->getTag();
	auto curScore = _roundManager->_players[tagNum]->get_score();

    const WinInfo_t &win = _roundManager->GetWin();

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
		if(_roundManager->_players[tagNum]->_cards->IsMing &&
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
void RaceLayer::raceAccount(float delta)
{
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
    
    const WinInfo_t &win = _roundManager->GetWin();

    switch(win.kind) {
        case SINGLE_WIN:
            {
                auto WinBar=(LayerColor*)raceAccoutLayer->getChildByTag(win.winner); 
                auto WinBarPlus=(LayerColor*)raceAccoutLayer->getChildByTag((win.winner+1)%3);
                auto WinBarMinus=(LayerColor*)raceAccoutLayer->getChildByTag((win.winner+2)%3);
            
                num = _roundManager->_players[win.winner]->get_hu_flag();
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
            
                num = _roundManager->_players[(win.giver+1)%3]->get_hu_flag();
                numDoubule = _roundManager->_players[(win.giver+2)%3]->get_hu_flag();
            
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
	ShowoutOnce->addTouchEventListener(CC_CALLBACK_1(RaceLayer::display_callback,this));
	ShowoutOnce->setAnchorPoint(Vec2(0.5,0.5));
	ShowoutOnce->setPosition(Vec2(origin.x+visibleSize.width*0.2996716,origin.y+visibleSize.height*0.075419));
	this->addChild(ShowoutOnce,11,SHINE_TAG_ID);

	auto PlayOnceAgin=Button::create("zailaiyiju1.png","zailaiyiju2.png","zailaiyiju2.png",UI_TEX_TYPE_PLIST);
	PlayOnceAgin->addTouchEventListener(CC_CALLBACK_2(RaceLayer::BtnRestartHandler,this));
	PlayOnceAgin->setAnchorPoint(Vec2(0.5,0.5));
	PlayOnceAgin->setPosition(Vec2(origin.x+visibleSize.width*0.6247948,origin.y+visibleSize.height*0.075419));
	this->addChild(PlayOnceAgin,11,RACE_RESTART_TAG_ID);
}

void  RaceLayer::showall()
{
    HideClock();
	
	float x,y;
	Sprite *p_list[MAX_HANDIN_NUM];

    int winner = _roundManager->GetLastWinner();
	for(int a=0;a<3;a++)
	{
		int no = (winner+a)%3;

        CardInHand *cards = _roundManager->_players[no]->_cards;
        
		for(int ik=0;ik<MAX_HANDIN_NUM;ik++)
		{
			if(myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+no*20+ik))
				myframe->removeChildByTag(HAND_IN_CARDS_TAG_ID+no*20+ik,true);
		}
		x=_layout->_playerPosi[no].basePoint.x+10; 
		y=_layout->_playerPosi[no].basePoint.y+10;
		for(int i=0;i<cards->size();i++)
		{
			if(cards->get_kind(i)!=ck_NOT_DEFINED )
			{
				if(no==0)
				{
					if(cards->get_status(i)==c_AN_GANG)
					{
						if(cards->get_kind(i)==cards->get_kind(i+1)&&cards->get_kind(i)!=cards->get_kind(i+2))
						{
							p_list[i]=_object->Create(LR_OUT_CARD);
							p_list[i]->setAnchorPoint(Vec2(0.3f,1.0f));
							p_list[i]->setPosition(Vec2(x-10,y-8));
							auto s_card=_object->CreateKind((Card_t)cards->get_kind(i),SMALL);
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
						auto s_card=_object->CreateKind((Card_t)cards->get_kind(i),SMALL);
						s_card->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height*0.65));
						s_card->setRotation(90);
						s_card->setScale(0.9);
						p_list[i]->addChild(s_card);
					}
					if(cards->get_status(i)==c_FREE||cards->get_status(i)==c_MING_KOU)
						y-=((p_list[i]->getTextureRect().size.height)*0.65);
					else if(cards->get_status(i)==c_PENG)
					{
						if((cards->get_kind(i+1)!=cards->get_kind(i))||((cards->get_kind(i+1)==cards->get_kind(i))&&(cards->get_status(i+1)!=cards->get_status(i))))
							y-=((p_list[i]->getTextureRect().size.height)*0.65+5);
						else
							y-=((p_list[i]->getTextureRect().size.height)*0.65);
					}
					else if(cards->get_status(i)==c_MING_GANG||cards->get_status(i)==c_AN_GANG)
					{
					    int idxInGroup = cards->get_idx_in_group(i);

                        if(idxInGroup==1) {
							y-=((p_list[i]->getTextureRect().size.height)*0.65);
                        } else if(idxInGroup==2) {
							y+=13;
                        } else if(idxInGroup==3) {
							y-=(((p_list[i]->getTextureRect().size.height)*0.65)+13);
                        } else {
							if(cards->get_kind(i+1)!=cards->get_kind(i)) {
								y-=((p_list[i]->getTextureRect().size.height)*0.65+5);
                            } else {/* ??? if this could happen */
								y-=((p_list[i]->getTextureRect().size.height)*0.65);
                            }
                        }
					}
				}
				else if(no==1)
				{
					if(cards->get_status(i)==c_FREE||cards->get_status(i)==c_MING_KOU)
						p_list[i]=_object->Create(MING_CARD);
					else if(cards->get_status(i)==c_PENG||cards->get_status(i)==c_MING_GANG)
						p_list[i]=_object->Create(PENG_CARD);
					else if(cards->get_status(i)==c_AN_GANG)
					{
						if(cards->get_kind(i)==cards->get_kind(i+1) && cards->get_kind(i)!=cards->get_kind(i+2))
							p_list[i]=_object->Create(PENG_CARD);
						else
							p_list[i]=_object->Create(AN_GANG_CARD);
					}
					p_list[i]->setAnchorPoint(Vec2(0,0));
					p_list[i]->setPosition(Vec2(x,y));
					if(cards->get_status(i)==c_FREE)
					{
						auto s_card=_object->CreateKind((Card_t)cards->get_kind(i),NORMAL);
						s_card->setAnchorPoint(Vec2(0.5,0.5));
						s_card->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height*0.6));
						p_list[i]->addChild(s_card,1);
						x+=p_list[i]->getTextureRect().size.width*1.0;
					}
					else if(cards->get_status(i)==c_MING_KOU)
					{
						auto s_card=_object->CreateKind((Card_t)cards->get_kind(i),NORMAL);
						s_card->setAnchorPoint(Vec2(0.5,0.5));
						s_card->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height*0.6));
						p_list[i]->addChild(s_card,1);
						x += p_list[i]->getTextureRect().size.width*1;
					}
					else if(cards->get_status(i)==c_PENG)
					{
						auto s_card=_object->CreateKind((Card_t)cards->get_kind(i),MIDDLE_SIZE);
						s_card->setAnchorPoint(Vec2(0.5,0.5));
						s_card->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height*0.6));
						p_list[i]->addChild(s_card,1);
						if(cards->get_status(i+1)==c_FREE)
							x += p_list[i]->getTextureRect().size.width*2;
						else if(cards->get_kind(i)!=cards->get_kind(i+1) &&(cards->get_status(i+1)!=c_FREE))
							x += p_list[i]->getTextureRect().size.width*1.5;
						else
							x += p_list[i]->getTextureRect().size.width*1.0;
					}
					else if(cards->get_status(i)==c_MING_GANG)
					{
						auto s_card=_object->CreateKind((Card_t)cards->get_kind(i),MIDDLE_SIZE);
						s_card->setAnchorPoint(Vec2(0.5,0.5));
						s_card->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height*0.6));
						p_list[i]->addChild(s_card,1);

					    int idxInGroup = cards->get_idx_in_group(i);

                        if(idxInGroup==1) {
							x+=p_list[i]->getTextureRect().size.width*1.0;
                        } else if(idxInGroup==2) {
							y+=17;
                        } else if(idxInGroup==3) {
							x+=p_list[i]->getTextureRect().size.width*1.0;
							y-=17;
                        } else {
							if(cards->get_status(i+1)==c_FREE)
								x += p_list[i]->getTextureRect().size.width*2;
							else if(cards->get_kind(i)!=cards->get_kind(i+1) &&(cards->get_status(i+1)!=c_FREE))
								x += p_list[i]->getTextureRect().size.width*1.5;
							else
								x += p_list[i]->getTextureRect().size.width*1.0;
                        }
					}
					else if(cards->get_status(i)==c_AN_GANG)
					{
					    int idxInGroup = cards->get_idx_in_group(i);

                        if(idxInGroup==1) {
							x+=p_list[i]->getTextureRect().size.width*1.0;
                        } else if(idxInGroup==2) {
							y+=17;
                        } else if(idxInGroup==3) {
							auto s_card=_object->CreateKind((Card_t)cards->get_kind(i),MIDDLE_SIZE);
							s_card->setAnchorPoint(Vec2(0.5,0.5));
							s_card->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height*0.6));
							p_list[i]->addChild(s_card,1);

							x+=p_list[i]->getTextureRect().size.width*1.0;
							y-=17;
                        } else {
							if(cards->get_status(i+1)==c_FREE)
								x += p_list[i]->getTextureRect().size.width*2;
							else if(cards->get_kind(i)!=cards->get_kind(i+1) &&(cards->get_status(i+1)!=c_FREE))
								x += p_list[i]->getTextureRect().size.width*1.5;
							else
								x += p_list[i]->getTextureRect().size.width*1.0;
                        }
					}
				}
				else if(no==2)
				{
					if(cards->get_status(i)==c_AN_GANG)
					{
						if((cards->get_kind(i)==cards->get_kind(i+1))&&(cards->get_kind(i)==cards->get_kind(i+2))&&(cards->get_kind(i)!=cards->get_kind(i+3)))//2
						{
							p_list[i]=_object->Create(LR_OUT_CARD);
							p_list[i]->setAnchorPoint(Vec2(0.3,0));
							p_list[i]->setPosition(Vec2(x-10,y-8));
							auto s_card=_object->CreateKind((Card_t)cards->get_kind(i),SMALL);
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
						auto s_card=_object->CreateKind((Card_t)cards->get_kind(i),SMALL);
						s_card->setPosition(Vec2(p_list[i]->getTextureRect().size.width/2,p_list[i]->getTextureRect().size.height*0.65));
						s_card->setRotation(-90);
						s_card->setScale(0.9);
						p_list[i]->addChild(s_card);
					}
					if(cards->get_status(i)==c_FREE||cards->get_status(i)==c_MING_KOU)
						y+=((p_list[i]->getTextureRect().size.height)*0.65);
					else if(cards->get_status(i)==c_PENG)
					{
						if((cards->get_kind(i+1)!=cards->get_kind(i))||((cards->get_kind(i+1)==cards->get_kind(i))&&(cards->get_status(i+1)!=cards->get_status(i))))
							y+=((p_list[i]->getTextureRect().size.height)*0.65+5);
						else
							y+=((p_list[i]->getTextureRect().size.height)*0.65);
					}
					else if(cards->get_status(i)==c_MING_GANG||cards->get_status(i)==c_AN_GANG)
					{
					    int idxInGroup = cards->get_idx_in_group(i);

                        if(idxInGroup==1) {
							y+=(p_list[i]->getTextureRect().size.height*0.65+13);
                        } else if(idxInGroup==2) {
							y-=13;
                        } else if(idxInGroup==3) {
							y+=((p_list[i]->getTextureRect().size.height)*0.65);
                        } else {
							if(cards->get_kind(i+1)!=cards->get_kind(i))
								y+=((p_list[i]->getTextureRect().size.height)*0.65+5);
							else
								y+=((p_list[i]->getTextureRect().size.height)*0.65);
                        }
					}
				}
				if(no!=2)
					myframe->addChild(p_list[i],i+1,HAND_IN_CARDS_TAG_ID+no*20+i);
				else if(no==2)
					myframe->addChild(p_list[i],cards->size()-i,HAND_IN_CARDS_TAG_ID+no*20+i);
                
				p_list[i]->_ID=1;
			}
		}
	}
}

/***********************************************************
        button accessosaries
***********************************************************/
void RaceLayer::BtnStartHandler(Ref* pSender,ui::Widget::TouchEventType type) {
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
                    RaceLayer::StartGame)),NULL));
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void RaceLayer::BtnRestartHandler(Ref* pSender,ui::Widget::TouchEventType type) {
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
                    RaceLayer::StartGame)),NULL));
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void RaceLayer::BtnTuoGuanCancelHandler(Ref* pSender,ui::Widget::TouchEventType type)
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
			if(this->getChildByTag(ROBOT_TUO_GUAN))
				this->removeChildByTag(ROBOT_TUO_GUAN,true);//TUOGUAN_CANCEL_BUTTON
			
			auto TuoGuanButton=(Button*)this->getChildByTag(MENU_BKG_TAG_ID)->getChildByTag(TUOGUAN_MENU_BUTTON);
			TuoGuanButton->setTouchEnabled(true);
			TuoGuanButton->setHighlighted(false);
			curButton->setTouchEnabled(false);
			curButton->setScale(0);
			_roundManager->_isTuoGuan=false;
			auto StartSelfGame=CallFunc::create([=](){
				if( !_roundManager->IsMing(1) )
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

void RaceLayer::BtnTuoGuanHandler(Ref* pSender,ui::Widget::TouchEventType type)
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
			TuoGuanCancel->addTouchEventListener(CC_CALLBACK_2(RaceLayer::BtnTuoGuanCancelHandler,this));
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
            
			if(myframe->getChildByTag(QI_REMIND_ACT_TAG_ID)) {
				_DeleteActionTip();
			}
            
            _roundManager->WaitForTuoGuanHandle();
		}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void RaceLayer::BtnBackConfirmHandler(Ref* pSender,ui::Widget::TouchEventType type)
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

void RaceLayer::BtnBackCancelHandler(Ref* pSender,ui::Widget::TouchEventType type)
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
					if(!_roundManager->IsMing(1))
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

void RaceLayer::BtnBackHandler(Ref* pSender,cocos2d::ui::Widget::TouchEventType type)
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

void RaceLayer::Back()
{
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
	ensureBut->addTouchEventListener(CC_CALLBACK_2(RaceLayer::BtnBackConfirmHandler,this));
	ensureBut->setAnchorPoint(Vec2(0.5,0.5));
	ensureBut->setPosition(Vec2(backChooseBar->getContentSize().width*0.75-15,backChooseBar->getContentSize().height*0.3));
	backChooseBar->addChild(ensureBut);

	auto BackBut=Button::create("jixuyouxi2.png","jixuyouxi.png","jixuyouxi.png",UI_TEX_TYPE_PLIST);//继续游戏
	BackBut->addTouchEventListener(CC_CALLBACK_2(RaceLayer::BtnBackCancelHandler,this));
	BackBut->setAnchorPoint(Vec2(0.5,0.5));
	BackBut->setPosition(Vec2(backChooseBar->getContentSize().width*0.25+15,backChooseBar->getContentSize().height*0.3));
	backChooseBar->addChild(BackBut);
}

/***********************************************
        general support
***********************************************/
Vec2 RaceLayer::GetCardPositionInHand(int idx) {
    return _GetCardInHand(MIDDLE,idx)->getPosition();
}

void RaceLayer::MaskNon(CardStatus_t status) {
    CardInHand *cards = _roundManager->_players[MIDDLE]->_cards;
    
    for(int i=0; i<cards->size(); i++) {
        if(cards->get_status(i)!=status) {
            _effect->Mask(_GetCardInHand(MIDDLE,i));
        }
    }
}

Spawn* RaceLayer::simple_tip_effect(Vec2 v,std::string act_name)
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


void RaceLayer::_DistributeEvent(const std::string event_type,void* val) {
	_eventDispatcher->dispatchCustomEvent(event_type,val);
}

void RaceLayer::_DeleteStartDealCards() {
	for(int i=0;i<3;i++) {
		for(int j=0;j<4;j++) {
            _Remove(myframe,START_CARDS_IN_TAG_ID+i*j);
		}
	}
}

void RaceLayer::_DeleteActionReminder()
{
	for(int i=0;i<17;i++){
        _Remove(myframe,REMIND_ACT_TAG_ID+i);
	}
}

void RaceLayer::_DeleteActionEffect()
{
 	for(int i=0;i<31;i++) {
        _Remove(myframe,MOJI_EFFECT_TAG_ID+i);
	}
}

void RaceLayer::_DeleteActionTip()
{
	_DeleteActionReminder();
    _DeleteActionEffect();    
}

void RaceLayer::hide_action_tip(ActionId_t action) {
    Sequence *hideReminder ;
    
    if(action==aPENG) {
        _effect->Hide(myframe,PENG_REMIND_ACT_TAG_ID);
        hideReminder = _HideReminder(PENG_REMIND_ACT_BKG_TAG_ID, 0.18, 1.2);
    } else if(action==aGANG) {
        _effect->Hide(myframe,GANG_REMING_ACT_TAG_ID);
        hideReminder = _HideReminder(GANG_REMING_ACT_BKG_TAG_ID, 0.18, 1.2);
    }

    myframe->runAction( Sequence::create(
        hideReminder,
        _voice->SpeakAction(action,_roundManager->_players[MIDDLE]->GetSex()),CCCallFunc::create(this,callfunc_selector(
        RaceLayer::_DeleteActionEffect)),NULL));
}

void RaceLayer::_Remove(Node *parent, int childTag) {
    if(parent->getChildByTag(childTag)) {// this judgement is not necessary becaust the interface will do the same thing!!!
        parent->removeChildByTag(childTag,true);
    }
}

void RaceLayer::_Add(Node *parent, Node *child, int childTag,int zOrder) {
    parent->addChild(child,zOrder,childTag);
}

void RaceLayer::_Show(Node *parent, int childTag, bool flag) {
    if(parent->getChildByTag(childTag)) {
        parent->getChildByTag(childTag)->setVisible(flag);
    }
}

bool RaceLayer::_IsClickedOn(Node* button,Touch* touch) {
    if(button==NULL) {
        return false;
    }
    
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

Spawn *RaceLayer::_CreateHuBackgroundEffect(PlayerDir_t winner) {
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
BezierTo* RaceLayer::BizerMove1(CardList* outCard,Vec2 location)
{
	ccBezierConfig config;

    _layout->GetBizerPoints(config, outCard->size()-1, location);

	BezierTo *action;
	if(ccpDistance(location,config.endPosition)<=200){
		action=BezierTo::create(0.18,config);
	} else {
		action=BezierTo::create(0.3,config);
	}
    
	return action;
}

BezierTo* RaceLayer::BizerMove2(CardList* outCard,Vec2 location,int time)
{
	ccBezierConfig config;
	if((outCard->size()-1)<6)
	{
		if(location.x<visibleSize.width*0.4)
		{
			config.endPosition=Vec2(_layout->_playerPosi[1].riverPoint.x+((outCard->size()-1)*36),_layout->_playerPosi[1].riverPoint.y);
			config.controlPoint_1=Vec2(location.x,200);
			config.controlPoint_2=Vec2(_layout->_playerPosi[1].riverPoint.x+((outCard->size()-1)*36)-100,_layout->_playerPosi[1].riverPoint.y-100);
		}
		else if(location.x>=visibleSize.width*0.6)
		{
			config.endPosition=Vec2(_layout->_playerPosi[1].riverPoint.x+((outCard->size()-1)*36),_layout->_playerPosi[1].riverPoint.y);
			config.controlPoint_1=Vec2(location.x,200);
			config.controlPoint_2=Vec2(_layout->_playerPosi[1].riverPoint.x+((outCard->size()-1)*36)+100,_layout->_playerPosi[1].riverPoint.y-100);
		}
		else if(location.x>visibleSize.width*0.4 && location.x<visibleSize.width*0.6)
		{
			config.endPosition=Vec2(_layout->_playerPosi[1].riverPoint.x+((outCard->size()-1)*36),_layout->_playerPosi[1].riverPoint.y);
			config.controlPoint_1=Vec2(location.x,200);
			config.controlPoint_2=Vec2(_layout->_playerPosi[1].riverPoint.x+((outCard->size()-1)*36),_layout->_playerPosi[1].riverPoint.y);
		}
	}
	else if((outCard->size()-1)<14)
	{
		if(location.x<visibleSize.width*0.4)
		{
			config.endPosition=Vec2(_layout->_playerPosi[1].riverPoint.x+((outCard->size()-8)*36),_layout->_playerPosi[1].riverPoint.y-41);
			if(time==2)
				config.controlPoint_1=Vec2(location.x,200);
			else
				config.controlPoint_1=Vec2(location.x+30,200);
			config.controlPoint_2=Vec2(_layout->_playerPosi[1].riverPoint.x+((outCard->size()-8)*36)-100,_layout->_playerPosi[1].riverPoint.y-141);
		}
		else if(location.x>=visibleSize.width*0.6)
		{
			config.endPosition=Vec2(_layout->_playerPosi[1].riverPoint.x+((outCard->size()-8)*36),_layout->_playerPosi[1].riverPoint.y-41);
			config.controlPoint_1=Vec2(location.x,200);
			config.controlPoint_2=Vec2(_layout->_playerPosi[1].riverPoint.x+((outCard->size()-8)*36)+100,_layout->_playerPosi[1].riverPoint.y-141);
		}
		else if(location.x>visibleSize.width*0.4 && location.x<visibleSize.width*0.6)
		{
			config.endPosition=Vec2(_layout->_playerPosi[1].riverPoint.x+((outCard->size()-8)*36),_layout->_playerPosi[1].riverPoint.y-41);
			config.controlPoint_1=Vec2(location.x,200);
			config.controlPoint_2=Vec2(_layout->_playerPosi[1].riverPoint.x+((outCard->size()-8)*36),_layout->_playerPosi[1].riverPoint.y-41);
		}
	}
	else
	{
		if(location.x<visibleSize.width*0.4)
		{
			config.endPosition=Vec2(_layout->_playerPosi[1].riverPoint.x+((outCard->size()-1)-16)*36,_layout->_playerPosi[1].riverPoint.y-82);
			if(time==2)
				config.controlPoint_1=Vec2(location.x,200);
			else
				config.controlPoint_1=Vec2(location.x+30,200);
			config.controlPoint_2=Vec2(_layout->_playerPosi[1].riverPoint.x+((outCard->size()-16)*36)-100,_layout->_playerPosi[1].riverPoint.y-182);
		}
		else if(location.x>=visibleSize.width*0.6)
		{
			config.endPosition=Vec2(_layout->_playerPosi[1].riverPoint.x+((outCard->size()-1)-16)*36,_layout->_playerPosi[1].riverPoint.y-82);
			config.controlPoint_1=Vec2(location.x,200);
			config.controlPoint_2=Vec2(_layout->_playerPosi[1].riverPoint.x+((outCard->size()-16)*36)+100,_layout->_playerPosi[1].riverPoint.y-182);
		}
		else if(location.x>visibleSize.width*0.4 && location.x<visibleSize.width*0.6)
		{
			config.endPosition=Vec2(_layout->_playerPosi[1].riverPoint.x+((outCard->size()-1)-16)*36,_layout->_playerPosi[1].riverPoint.y-82);
			config.controlPoint_1=Vec2(location.x,200);
			config.controlPoint_2=Vec2(_layout->_playerPosi[1].riverPoint.x+((outCard->size()-16)*36),_layout->_playerPosi[1].riverPoint.y-82);
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

BezierTo* RaceLayer::OthersBizerMove(int no,CardList* outCard)
{
	ccBezierConfig config;
    
	if(no==2)
	{
		int secondLast = _roundManager->_players[no]->_cards->real_last()-1;
		auto BizerPosForPlayerTwoTemp=myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+no*20+secondLast)->getPosition();
		auto BizerPosForPlayerTwo=Vec2(BizerPosForPlayerTwoTemp.x,BizerPosForPlayerTwoTemp.y);
		if((outCard->size()-1)<6)
		{
			config.controlPoint_1=Vec2(BizerPosForPlayerTwo.x-100,BizerPosForPlayerTwo.y);
			config.controlPoint_2=Vec2(_layout->_playerPosi[no].riverPoint.x,_layout->_playerPosi[no].riverPoint.y+30*(outCard->size()-1));
			config.endPosition=Vec2(_layout->_playerPosi[no].riverPoint.x,_layout->_playerPosi[no].riverPoint.y+30*(outCard->size()-1));
		}
		else if((outCard->size()-1)<14)
		{
			config.controlPoint_1=Vec2(BizerPosForPlayerTwo.x-100,BizerPosForPlayerTwo.y);
			config.controlPoint_2=Vec2(_layout->_playerPosi[no].riverPoint.x+48,_layout->_playerPosi[no].riverPoint.y+30*((outCard->size()-1)-7));
			config.endPosition=Vec2(_layout->_playerPosi[no].riverPoint.x+48,_layout->_playerPosi[no].riverPoint.y+30*((outCard->size()-1)-7));
		}
		else
		{
			config.controlPoint_1=Vec2(BizerPosForPlayerTwo.x-100,BizerPosForPlayerTwo.y);
			config.controlPoint_2=Vec2(_layout->_playerPosi[no].riverPoint.x+96,_layout->_playerPosi[no].riverPoint.y+30*((outCard->size()-1)-16));
			config.endPosition=Vec2(_layout->_playerPosi[no].riverPoint.x+96,_layout->_playerPosi[no].riverPoint.y+30*((outCard->size()-1)-16));
		}
	}
	else if(no==0)
	{
		int secondLast = _roundManager->_players[no]->_cards->real_last()-1;
		auto l_card=myframe->getChildByTag(HAND_IN_CARDS_TAG_ID+no*20+secondLast);
		if((outCard->size()-1)<6)
		{
			config.controlPoint_1=Vec2(_layout->_playerPosi[no].basePoint.x+110,l_card->getPosition().y-35);
			config.controlPoint_2=Vec2(_layout->_playerPosi[no].riverPoint.x,_layout->_playerPosi[no].riverPoint.y-30*(outCard->size()-1));
			config.endPosition=Vec2(_layout->_playerPosi[no].riverPoint.x,_layout->_playerPosi[no].riverPoint.y-30*(outCard->size()-1));
		}
		else if((outCard->size()-1)<14)
		{
			config.controlPoint_1=Vec2(_layout->_playerPosi[no].basePoint.x+110,l_card->getPosition().y-35);
			config.controlPoint_2=Vec2(_layout->_playerPosi[no].riverPoint.x-48,_layout->_playerPosi[no].riverPoint.y-30*((outCard->size()-1)-7));
			config.endPosition=Vec2(_layout->_playerPosi[no].riverPoint.x-48,_layout->_playerPosi[no].riverPoint.y-30*((outCard->size()-1)-7));
		}
		else
		{
			config.controlPoint_1=Vec2(_layout->_playerPosi[no].basePoint.x+110,l_card->getPosition().y-35);
			config.controlPoint_2=Vec2(_layout->_playerPosi[no].riverPoint.x-96,_layout->_playerPosi[no].riverPoint.y-30*((outCard->size()-1)-16));
			config.endPosition=Vec2(_layout->_playerPosi[no].riverPoint.x-96,_layout->_playerPosi[no].riverPoint.y-30*((outCard->size()-1)-16));
		}
	}
	auto action=BezierTo::create(0.3,config);
	return action;
}

