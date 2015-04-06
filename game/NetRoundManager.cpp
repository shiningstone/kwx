
#include <stdlib.h>

#include "cocos2d.h"
USING_NS_CC;

#include "./../protocol/CommonMsg.h"
#include "./../protocol/KwxMessenger.h"

#include "RaceType.h"
#include "Raction.h"

#include "NetRole.h"
#include "NetRaceLayer.h"
#include "RoundManager.h"
#include "NetRoundManager.h"

NetRoundManager::NetRoundManager(NetRaceLayer *uiManager)
:RoundManager(uiManager) {
    _MODE = NETWORK_GAME;

    _uiManager = uiManager;

    _lastWin.winner = INVALID_DIR;
    _lastWin.giver  = INVALID_DIR;

    _river = NULL;
	for(int i=0;i<TOTAL_CARD_NUM;i++) {
		_unDistributedCards[i]=i;
	}
    _distributedNum = 0;

    for(int i=0;i<PLAYER_NUM;i++) {
        _players[i] = NULL;
    }

    _ai = Ai::getInstance(this);
    _messenger = new KwxMessenger();
    _logger = LOGGER_REGISTER("NetRoundManager");
}

NetRoundManager::~NetRoundManager() {
    delete _river;
    for(int i=0;i<PLAYER_NUM;i++) {
        delete _players[i];
    }

    delete _messenger;
    LOGGER_DEREGISTER(_logger);
}

/***********************************************
        player information
***********************************************/
#include "NetMe.h"
#include "NetPlayer.h"
#include "NetRaceRound.h"

void NetRoundManager::Init() {
    InitPlayers();
}

void NetRoundManager::InitPlayers() {
    _cardHolders[0] = new CardHolder();
    _cardHolders[1] = new CardHolder();
    _cardHolders[2] = new CardHolder();

	_players[0] = new NetPlayer();
	_players[1] = new NetMe();
	_players[2] = new NetPlayer();

	_players[0]->set_parter( new NetRRound() );
	_players[1]->set_parter( new NetRRound() );
	_players[2]->set_parter( new NetRRound() );

	_players[0]->get_parter()->set_role_type( INTERNET_PLAYER );
	_players[1]->get_parter()->set_role_type( SINGLE_BOADR_ME );
	_players[2]->get_parter()->set_role_type( INTERNET_PLAYER );
}

void NetRoundManager::LoadPlayerInfo() {
    Database *database = Database::getInstance();
    
    int  ids[3] = {0};
    _GenerateIds(ids);
    
	for(int dir=0;dir<3;dir++)
	{	
        UserProfile_t profile = {0};
        database->GetUserProfile(ids[dir],profile);
        _cardHolders[dir]->Set(&profile);
	}
}

/****************************************
       networks
****************************************/
void NetRoundManager::ListenToMessenger() {
    _msgQueue = MsgQueue::getInstance(this);
}

void NetRoundManager::RecvMsg(void* val) {
    _msgQueue->push(val);
}

void NetRoundManager::HandleMsg(void * aMsg) {
    auto di = static_cast<DsInstruction *>(aMsg);
    LOGGER_WRITE("get ds instruction %d\n",di->request);

    switch(di->request) {
        case REQ_GAME_SEND_START:
            _DiRecv((GameStartResponse *)di);
            break;
        case REQ_GAME_RECV_START:
            _DiRecv((GameStartNotif *)di);
            break;
        case REQ_GAME_DIST_BEGINCARDS:
            _DiRecv((FirstDistZhuang *)di);
        default:
            LOGGER_WRITE("%s undefined request code %d\n",__FUNCTION__,di->request);
            break;
    }
}

void NetRoundManager::_DiRecv(GameStartResponse *info) {
    _cardHolders[MIDDLE]->_isReady = true;
    _uiManager->GuiShowReady(MIDDLE);
    LOGGER_WRITE("NOTE: Player%d's score should set to %d\n",MIDDLE,info->score);
    delete info;
}

void NetRoundManager::_DiRecv(GameStartNotif *info) {
    _cardHolders[info->seat]->_isReady = true;
    _uiManager->GuiShowReady(info->seat);
    LOGGER_WRITE("NOTE: Player%d's score should set to %d\n",info->seat,info->score);
    delete info;
}

void NetRoundManager::_DiRecv(FirstDistZhuang *info) {
    _uiManager->GuiHideReady();

    RenewOutCard();
    Shuffle();

    int cards[14];
    for(int i=0;i<14;i++) {
        cards[i] = info->cards[i]*4;
    }
    _actionToDo = info->GetAvailActions(info->remind);
    delete info;
    
    _players[_curPlayer]->init(cards,14,aim[MIDDLE]);//玩家手牌初始�?
    _players[(_curPlayer+1)%3]->init(&(_unDistributedCards[14]),13,aim[(MIDDLE+1)%3]);
    _players[(_curPlayer+2)%3]->init(&(_unDistributedCards[27]),13,aim[(MIDDLE+2)%3]);

	_uiManager->FirstRoundDistributeEffect(MIDDLE);//牌局开始发牌效果�?
}

/****************************************
       main interface
****************************************/
void NetRoundManager::CreateRace(Scene *scene) {
    _uiManager = NetRaceLayer::create();
    scene->addChild(_uiManager);

    _uiManager->Set(this);

    InitPlayers();
    LoadPlayerInfo();
	_isGameStart=false;

    LOGGER_WRITE("NOTE: SeatInfo should be set when get response from server rather than here\n");
    SeatInfo *seat = SeatInfo::getInstance();
    seat->Set(0x00010203,0x04050607,0x08090a0b,1);

    ListenToMessenger();
    _messenger->StartReceiving();
    
    _uiManager->CreateRace();
}

void NetRoundManager::StartGame() {
	_isGameStart=false;
    
    RequestGameStart aReq;
    aReq.Set();
    _messenger->Send(aReq);
}

void NetRoundManager::RecvPeng(PlayerDir_t dir) {
    Card        card;
    PlayerDir_t prevPlayer;
    
    if(_isWaitDecision) {
        _isWaitDecision = false;
        _actionToDo = _tempActionToDo;
        _tempActionToDo = a_JUMP;
    }
    
    _continue_gang_times = 0;
    _lastAction=a_PENG;
    
    const int riverLast =_players[_curPlayer]->get_parter()->getOutCardList()->length;
    
    _players[_curPlayer]->get_parter()->getOutCardList()->getCard(card,riverLast);
    _players[_curPlayer]->get_parter()->getOutCardList()->deleteItem();
    
    RecordOutCard(card);
    RecordOutCard(card);

    prevPlayer = (PlayerDir_t)_curPlayer;
    _curPlayer = dir;

    _uiManager->PengEffect(dir,prevPlayer,(Card_t)card.kind);
}

void NetRoundManager::RecvHu(PlayerDir_t dir) {
    if(_isWaitDecision) {
        _isWaitDecision = false;
        _actionToDo = _tempActionToDo;
        _tempActionToDo = a_JUMP;
    }

    if(_isQiangGangAsking) {
        _lastActionWithGold = a_QIANG_GANG;
    }

    if(_isDoubleHuAsking) {
        SetWin(DOUBLE_WIN,_curPlayer);
    } else {
        SetWin(SINGLE_WIN,dir);
    }

    _uiManager->HuEffect(_lastWin, _isQiangGangAsking);
}

void NetRoundManager::RecvGang(PlayerDir_t dir) {
    if(_isGangAsking)//is this judgement neccessary?
        _isGangAsking = false;
    
    if(_isWaitDecision) {
        _isWaitDecision=false;
        _actionToDo = _tempActionToDo;
        _tempActionToDo = a_JUMP;
    }

	_continue_gang_times++;

    int* gangCardIdx=new int[4];
    Card_t card;
    
	auto list=_players[dir]->get_parter()->get_card_list();
	if( _actionToDo & a_AN_GANG || _actionToDo & a_SHOU_GANG ) {
		_lastActionSource = dir;
        
		if(_actionToDo&a_AN_GANG) {
			_actionToDo=a_AN_GANG;
			_lastAction=a_AN_GANG;
			_lastActionWithGold=a_AN_GANG;
		} else if(_actionToDo&a_SHOU_GANG) {
			_actionToDo=a_SHOU_GANG;
			_lastAction=a_SHOU_GANG;
			_lastActionWithGold=a_SHOU_GANG;
		}
        
        card = _ai->FindGangCards(gangCardIdx,list,CARD_UNKNOWN,_actionToDo,IsTing(dir),_isCardFromOthers);
        
		if( !IsTing(_curPlayer) ) {
			SetEffectCard(card,c_AN_GANG);
		}

        _uiManager->GangEffect(dir,card,gangCardIdx);
	}
	else if( _actionToDo & a_MING_GANG ) {
		_lastActionSource=dir;
		_actionToDo=a_MING_GANG;
		_lastAction=a_MING_GANG;
		_lastActionWithGold=a_MING_GANG;

		Card GangCard;
		PlayerDir_t prevPlayer = (PlayerDir_t)_curPlayer;
        
		if(_isCardFromOthers) {
			int riverLast = _players[_curPlayer]->get_parter()->getOutCardList()->length;
			_players[_curPlayer]->get_parter()->getOutCardList()->getCard(GangCard,riverLast);
			_players[_curPlayer]->get_parter()->getOutCardList()->deleteItem();

			RecordOutCard(GangCard);
			RecordOutCard(GangCard);
			RecordOutCard(GangCard);
            
			_curPlayer=dir;
		}else {
			GangCard=list->data[list->len-1];
			RecordOutCard(GangCard);
		}

        card = _ai->FindGangCards(gangCardIdx,list,(Card_t)GangCard.kind,_actionToDo,IsTing(dir),_isCardFromOthers);
        _uiManager->GangEffect(dir,(Card_t)GangCard.kind,gangCardIdx,false,prevPlayer);
	}
}

void NetRoundManager::RecvQi() {
	if(_lastAction==a_JUMP) {
		_continue_gang_times=0;
    }

	_lastAction=a_JUMP;
	_actionToDo=a_JUMP;

	if(_isWaitDecision) {
		_isWaitDecision=false;
		_tempActionToDo=a_JUMP;
	}

    _uiManager->QiEffect();
}

void NetRoundManager::RecvHandout(int idx,Vec2 touch,int mode) {
    auto cardsInHand = _players[MIDDLE]->get_parter()->get_card_list();

    if(_isGangAsking) {
        _isGangAsking = false;
    }
    
	if(_isMingTime) {
		_isMingTime=false;
	} else {
        if(_actionToDo&a_MING) {
            _actionToDo = a_JUMP;
        }
    }
    
	if(_isWaitDecision) {
		_isWaitDecision=false;
		_tempActionToDo=a_JUMP;
	}

    RecordHandOut(idx);

    Card_t card = (Card_t)_lastHandedOutCard;
    RequestHandout aReq;
    aReq.Set(card);
    _messenger->Send(aReq);

    bool turnToMing = false;
	if(_actionToDo==a_MING && 
        !IsTing(_curPlayer) ) {
		_players[_curPlayer]->get_parter()->LockAllCards();
		_players[_curPlayer]->get_parter()->set_ting_status(1);

        turnToMing = true;
    }

    _uiManager->MyHandoutEffect(idx,cardsInHand,touch,mode,turnToMing);
}

void NetRoundManager::RecvKouCancel() {
    auto cards = _players[MIDDLE]->get_parter()->get_card_list();
    for(int i=cards->atcvie_place;i<cards->len;i++) {
        cards->data[i].status=c_FREE;
    }

    _uiManager->KouCancelEffect(cards);
}

void NetRoundManager::RecvKouConfirm() {
    auto cards = _players[MIDDLE]->get_parter()->get_card_list();
    for(int i=cards->atcvie_place;i<cards->len;i++) {
        if(cards->data[i].status==c_KOU_ENABLE)
            cards->data[i].status=c_FREE;
    }   
    
    UpdateCards(MIDDLE,a_KOU);
    
    auto ming_indexesCur=_players[MIDDLE]->get_parter()->ming_check();
    _players[MIDDLE]->get_parter()->set_ming_indexes(ming_indexesCur);

    _uiManager->KouConfirmEffect();
}

void NetRoundManager::RecvMingCancel() {
    _isMingTime=false;
    
    UpdateCards(MIDDLE,a_KOU_CANCEL);
    _actionToDo=a_JUMP;
    
    _players[1]->get_parter()->MingCancel();
    _players[1]->get_parter()->set_ming_indexes(0);
    _players[1]->get_parter()->set_ting_status(0);

    _uiManager->MingCancelEffect();
}

void NetRoundManager::RecvMing() {
	_actionToDo=a_MING;

    _ai->KouCardCheck((PlayerDir_t)_curPlayer);

    if(_curPlayer==MIDDLE) {
        if(_ai->KouCardGroupNum()>0) {
            _uiManager->QueryKouCards();
        } else {
            _isMingTime=true;
            UpdateCards(MIDDLE,a_MING);
            
            _uiManager->QueryMingOutCard();
        }
    } else {
        WaitForOthersChoose();
    }
}

void NetRoundManager::WaitForMyAction() {
    _uiManager->ShowActionButtons(_actionToDo);

	if(_actionToDo!=a_JUMP) {
		_isWaitDecision = true;
		_tempActionToDo=_actionToDo;
		_actionToDo=a_JUMP;
	}

	if(_actionToDo&a_AN_GANG 
        || _actionToDo&a_MING_GANG
        ||_actionToDo&a_SHOU_GANG) {
        _isGangAsking = true;
	}

	if(!_isCardFromOthers) {
		if(_lastAction==a_JUMP&&!(_lastActionSource==1&&_continue_gang_times!=0)) {
			_continue_gang_times=0;
        }

		_lastAction=a_JUMP;
		WaitForMyChoose();
	}
}

void NetRoundManager::WaitForMyChoose() {
	if(!_isCardFromOthers) {/* is this judgement neccessary??? */
		if( _isTuoGuan ||
                (IsTing(_curPlayer) && !_isGangAsking) ) {
            auto cards = _players[MIDDLE]->get_parter()->get_card_list();
            Vec2 location = _uiManager->GetCardPositionInHand(cards->len-1);
            RecvHandout(cards->len-1,location,2);
            
		} else {
			_isMyShowTime = true;
        }
	}
}

void NetRoundManager::WaitForOthersAction(PlayerDir_t dir) {
    LOGGER_WRITE("%s (%d) perform action %d",__FUNCTION__,dir,_actionToDo);
    auto list=_players[dir]->get_parter()->get_card_list();

    if(_actionToDo&a_HU) {
        RecvHu(dir);
    } else if(_actionToDo&a_AN_GANG||_actionToDo&a_SHOU_GANG) {
        _continue_gang_times++;
        _lastActionSource = dir;
        
        if(_actionToDo&a_AN_GANG) {
            _actionToDo=a_AN_GANG;
            _lastAction=a_AN_GANG;
            _lastActionWithGold=a_AN_GANG;
        } else if(_actionToDo&a_SHOU_GANG) {
            _actionToDo=a_SHOU_GANG;
            _lastAction=a_SHOU_GANG;
            _lastActionWithGold=a_SHOU_GANG;
        }

        int* gangIdx=new int[4];
        Card_t card = _ai->FindGangCards(gangIdx,list,CARD_UNKNOWN,_actionToDo,IsTing(dir),_isCardFromOthers);

        if( !IsTing(dir) ) {
            SetEffectCard(card,c_AN_GANG);
        }

        _uiManager->_AnGangEffect(dir,card,gangIdx);
    } else if(_actionToDo&a_MING_GANG) {
        _lastActionSource=dir;
        _actionToDo=a_MING_GANG;
        _lastAction=a_MING_GANG;
        _lastActionWithGold=a_MING_GANG;

        Card GangCard;
        PlayerDir_t prevPlayer = (PlayerDir_t)dir;
        if(_isCardFromOthers) {
            int riverLast = _players[dir]->get_parter()->getOutCardList()->length;
            _players[dir]->get_parter()->getOutCardList()->getCard(GangCard,riverLast);
            _players[dir]->get_parter()->getOutCardList()->deleteItem();

            RecordOutCard(GangCard);
            RecordOutCard(GangCard);
            RecordOutCard(GangCard);
            _curPlayer=dir;
        }else {
            GangCard=list->data[list->len-1];
            RecordOutCard(GangCard);
        }

        int* gangIdx=new int[4];
        Card_t card = _ai->FindGangCards(gangIdx,list,(Card_t)GangCard.kind,_actionToDo,IsTing(dir),_isCardFromOthers);
        _uiManager->_MingGangEffect(dir,prevPlayer,card,gangIdx);
    }
    else if(_actionToDo&a_MING) {
        RecvMing();
    } else if(_actionToDo&a_PENG) {
        RecvPeng(dir);
    } else if(_actionToDo==a_JUMP) {
        if(_lastAction==a_JUMP) {
            _continue_gang_times=0;
        }
        _lastAction=a_JUMP;
        
        WaitForOthersChoose();
    }
}

void NetRoundManager::WaitForOthersChoose() {
    if ( _curPlayer==1 ) {/* this should never happen */
        return;
    }

    bool canKou = false;
	int index = _ai->ChooseWorstCard(canKou);
    
    if ( canKou ) {
        _otherHandedOut = (Card_t)_players[_curPlayer]->get_parter()->get_card_list()->data[index].kind;
        
        _ai->KouCardCheck((PlayerDir_t)_curPlayer);
        if(_ai->KouCardGroupNum()>0) {
            _ai->MingKouChoose((PlayerDir_t)_curPlayer);
        }
    }

    RecordOutCard(_players[_curPlayer]->get_parter()->get_card_list()->data[index]);
	_lastHandedOutCard=_players[_curPlayer]->get_parter()->hand_out(index);

    if(canKou) {
        _uiManager->TingHintBarOfOthers(_curPlayer,index);

        /* it is dangerous to raise these lines to upper, since the following will change the card list*/
        if(_ai->KouCardGroupNum()>0)
            UpdateCards((PlayerDir_t)_curPlayer,a_KOU);

        UpdateCards((PlayerDir_t)_curPlayer,a_MING);

        _players[_curPlayer]->get_parter()->LockAllCards();
        _players[_curPlayer]->get_parter()->set_ting_status(1);
    }

	_isCardFromOthers=true;

    _uiManager->OthersHandoutEffect((PlayerDir_t)_curPlayer,canKou);
}

void NetRoundManager::WaitForResponse(PlayerDir_t dir) {
    LOGGER_WRITE("%s (%d)\n",__FUNCTION__,dir);
}

void NetRoundManager::DistributeTo(PlayerDir_t dir) {
    if(_distributedNum<TOTAL_CARD_NUM) {
        DistributeInfo_t distInfo;
        
        distInfo.target = dir;
        distInfo.card   = (Card_t)(_unDistributedCards[_distributedNum++]/4);
        distInfo.distNum = _distributedNum;
        
        _uiManager->_DistributeEvent(DISTRIBUTE_DONE_EVENT_TYPE,&distInfo);
    } else {
		_uiManager->_DistributeEvent(NOONE_WIN_EVENT_TYPE,NULL);
    }
}

void NetRoundManager::ActionAfterGang(PlayerDir_t dir) {
    if(!_isCardFromOthers) {
        QiangGangHuJudge(dir);
    } else {
        DistributeTo(dir);
    }
}

/*************************************
        singleton
*************************************/
NetRoundManager* NetRoundManager::_instance = NULL;

NetRoundManager *NetRoundManager::getInstance() {
    if (_instance==NULL) {
        _instance = new NetRoundManager(NULL);
    }

    return _instance;
}

void NetRoundManager::destroyInstance() {
    delete _instance;
    _instance = NULL;
}

