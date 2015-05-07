
#include <stdlib.h>

#include "cocos2d.h"
USING_NS_CC;

#include "./../protocol/CommonMsg.h"
#include "./../protocol/KwxMessenger.h"

#include "RaceType.h"

#include "CardCollection.h"
#include "Player.h"
#include "RaceLayer.h"
#include "RoundManager.h"
#include "NetRoundManager.h"

NetRoundManager::NetRoundManager(RaceLayer *uiManager)
:RoundManager(uiManager) {
    _MODE = NETWORK_GAME;

    _uiManager = uiManager;

    _lastWin.winner = INVALID_DIR;
    _lastWin.giver  = INVALID_DIR;

	_gRiver = new CardList;
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
    delete _gRiver;
    for(int i=0;i<PLAYER_NUM;i++) {
        delete _players[i];
    }

    delete _messenger;
    LOGGER_DEREGISTER(_logger);
}

/***********************************************
        player information
***********************************************/
#include "Player.h"
#include "NetPlayer.h"

void NetRoundManager::InitPlayers() {
	_players[0] = new NetPlayer();
	_players[1] = new Player();
	_players[2] = new NetPlayer();

    Database *database = Database::getInstance();

    int  ids[3] = {0};
    _GenerateIds(ids);

    for(int dir=0;dir<3;dir++)
    {   
        UserProfile_t profile = {0};
        database->GetUserProfile(ids[dir],profile);
        _players[dir]->Set(&profile);
    }
}

/****************************************
       networks
****************************************/
/*this could be settle via a more effective way*/
int NetRoundManager::GetAvailActions(int actNum,const ActionId_t actions[]) {
    int actionToDo = 0;
    
    for (int i=0;i<actNum; i++) {
        switch(actions[i]) {
            case aQi:
                actionToDo |= a_JUMP;
                break;
            case aPENG:
                actionToDo |= a_PENG;
                break;
            case aMING_GANG:
                actionToDo |= a_MING_GANG;
                break;
            case aAN_GANG:
                actionToDo |= a_AN_GANG;
                break;
            case aMING:
                actionToDo |= a_MING;
                break;
            case aHU:
                actionToDo |= a_HU;
                break;
            case aSHOU_GANG:
                actionToDo |= a_SHOU_GANG;
                break;
            case aKOU:
                actionToDo = a_KOU;
                break;
            case aKOU_CANCEL:
                actionToDo = a_KOU_CANCEL;
                break;
        }
    }

    return actionToDo;
}

void NetRoundManager::ListenToMessenger() {
    _msgQueue = MsgQueue::getInstance(this);
}

void NetRoundManager::RecvMsg(void* val) {
    _msgQueue->push(val);
}

void NetRoundManager::HandleMsg(void * aMsg) {
    auto di = static_cast<DsInstruction *>(aMsg);

    #ifdef WIN32
    LOGGER_WRITE("get ds instruction %s\n",di->Desc(di->request));
    #else
    LOGGER_WRITE("get ds instruction %d\n",di->request);
    #endif
    
    switch(di->request) {
        case REQ_GAME_SEND_START:
            _DiRecv((GameStartResponse *)di);
            break;
        case REQ_GAME_RECV_START:
            _DiRecv((GameStartNotif *)di);
            break;
        case REQ_GAME_DIST_BEGINCARDS:
            _DiRecv((FirstDistZhuang *)di);
            break;
        case REQ_GAME_DIST_BEGINCARDS_OTHER:
            _DiRecv((FirstDistNonZhuang *)di);
            break;
        case REQ_GAME_DIST_CARD_TOOTHER:
            _DiRecv((DistCardNotif *)di);
            break;
        case REQ_GAME_DIST_CARD:
            _DiRecv((DistCardInfo *)di);
            break;
        case REQ_GAME_SEND_SHOWCARD:
            _DiRecv((ShowCardResponse *)di);
            break;
        case REQ_GAME_RECV_SHOWCARD:
            _DiRecv((ShowCardNotif *)di);
            break;
        case REQ_GAME_DIST_REMIND:
            _DiRecv((RemindInfo *)di);
            break;
        case REQ_GAME_SEND_ACTION:
            _DiRecv((ActionResponse *)di);
            break;
        case REQ_GAME_RECV_ACTION:
            _DiRecv((ActionNotif *)di);
            break;
        case REQ_GAME_DIST_HU_CALCULATE:
            _DiRecv((HuInfoNotif *)di);
            break;

        /*********************************************
            �ƾ��޹�REQUEST
        *********************************************/
        case REQ_GAME_SEND_ENTER:
            _DiRecv((EnterRoomResponse *)di);
            break;
        case REQ_GAME_RECV_ENTER:
            _DiRecv((EnterRoomNotif *)di);
            break;
        default:
            LOGGER_WRITE("%s undefined request code %d\n",__FUNCTION__,di->request);
            break;
    }
}

void NetRoundManager::_DiRecv(GameStartResponse *info) {
    _players[MIDDLE]->_isReady = true;
    _uiManager->GuiShowReady(MIDDLE);
    LOGGER_WRITE("NOTE: Player%d's score should set to %d\n",MIDDLE,info->score);
    delete info;
}

void NetRoundManager::_DiRecv(GameStartNotif *info) {
    _players[info->seat]->_isReady = true;
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
        cards[i] = info->cards[i];
    }
    PlayerDir_t dir = (PlayerDir_t)info->seat;
    INT8U timer     = info->timer;
    _actionToDo     = GetAvailActions(info->remind.actionNum, info->remind.actions);
    delete info;

    _players[_curPlayer]->init(cards,14,aim[MIDDLE]);//玩家手牌初始�?
    _players[(_curPlayer+1)%3]->init(&(_unDistributedCards[14]),13,aim[(MIDDLE+1)%3]);
    _players[(_curPlayer+2)%3]->init(&(_unDistributedCards[27]),13,aim[(MIDDLE+2)%3]);

	_uiManager->FirstRoundDistributeEffect(MIDDLE);//牌局开始发牌效果�?
    _uiManager->UpdateClock(timer,MIDDLE);
}

void NetRoundManager::_DiRecv(FirstDistNonZhuang *info) {
    _uiManager->GuiHideReady();

    int cards[13];
    for(int i=0;i<13;i++) {
        cards[i] = info->cards[i];
    }
    PlayerDir_t me     = (PlayerDir_t)info->seat;
    PlayerDir_t zhuang = (PlayerDir_t)info->zhuang;
    INT8U timer        = info->timer;
    delete info;

    _curPlayer = zhuang;
    PlayerDir_t other  = (zhuang==LEFT)?RIGHT:LEFT;
    
    _players[zhuang]->init(&(_unDistributedCards[0]),14,aim[zhuang]);//玩家手牌初始�?
    _players[MIDDLE]->init(cards,13,aim[MIDDLE]);
    _players[other]->init(&(_unDistributedCards[27]),13,aim[other]);

	_uiManager->FirstRoundDistributeEffect(zhuang);//牌局开始发牌效果�?
    _uiManager->UpdateClock(timer,_curPlayer);
}

void NetRoundManager::_DiRecv(DistCardNotif *info) {
    PlayerDir_t target = (PlayerDir_t)info->seat;
    Card_t card        = (Card_t)info->kind;
    INT8U timer        = info->timer;
    _distributedNum    = info->remain;
    delete info;

    ServerDistributeTo(target,card);
    _players[target]->hand_in(
        card,
        false,
        IsTing(target),
        (_distributedNum==TOTAL_CARD_NUM),
        _lastActionWithGold,
        _continue_gang_times,
        _isGangHua
    );
    _uiManager->UpdateClock(timer,target);
}

void NetRoundManager::_DiRecv(DistCardInfo *info) {
    PlayerDir_t target = (PlayerDir_t)info->seat;
    Card_t card        = (Card_t)info->kind;
    INT8U timer        = info->timer;
    _distributedNum    = info->remain;

    _curPlayer        = MIDDLE;
    _isNewDistributed = true;
    _actionToDo       = GetAvailActions(info->remind.actionNum,info->remind.actions);
    delete info;

    ServerDistributeTo(target,card);
    _players[MIDDLE]->hand_in(
        card,
        false,
        IsTing(MIDDLE),
        (_distributedNum==TOTAL_CARD_NUM),
        _lastActionWithGold,
        _continue_gang_times,
        _isGangHua
    );
    ServerWaitForMyAction();

    _uiManager->UpdateClock(timer,target);
    _uiManager->ListenToCardTouch();
}

void NetRoundManager::_DiRecv(ShowCardResponse *info) {
    LOGGER_WRITE("%s handout ret = %d",__FUNCTION__,info->status);
    delete info;
}

void NetRoundManager::_DiRecv(ShowCardNotif *info) {
    PlayerDir_t dir    = (PlayerDir_t)info->seat;
    Card_t card        = (Card_t)info->kind;
    delete info;

    RecordOutCard(card);
	_lastHandedOutCard = card;
    _actionToDo = a_JUMP;
    
    _curPlayer = dir;

    _players[dir]->_cards->pop_back();
    _players[dir]->_river->push_back(card);
        
	_isNewDistributed = false;

    _uiManager->OthersHandoutEffect(dir,false);
}

void NetRoundManager::_DiRecv(RemindInfo *info) {
    PlayerDir_t dir = (PlayerDir_t)info->seat;
    PlayerDir_t whoGive = (PlayerDir_t)info->whoGive;
    INT8U timer     = info->timer;
    Card_t kind     = info->kind;
    _actionToDo     = GetAvailActions(info->remind.actionNum,info->remind.actions);
    delete info;

    if(dir!=whoGive) {
        _isNewDistributed = false;
    } else {
        _isNewDistributed = true;
    }

    _isMyShowTime = true;
    ServerWaitForMyAction();
}

void NetRoundManager::_DiRecv(ActionResponse *info) {
    PlayerDir_t dir  = (PlayerDir_t)info->seat;
    PlayerDir_t wait = (PlayerDir_t)info->waitSeat;
    delete info;

    LOGGER_WRITE("NOTE: something should happen here\n");
    //_uiManager->PengEffect(dir,prevPlayer,(Card_t)card.kind);
}

void NetRoundManager::_DiRecv(ActionNotif *info) {
    PlayerDir_t dir     = (PlayerDir_t)info->seat;
    PlayerDir_t whoGive = (PlayerDir_t)info->whoGive;
    _actionToDo     = GetAvailActions(info->actionNum, info->actions);
    Card_t card     = info->card[0];
    delete info;

    _curPlayer = whoGive;
    switch(_actionToDo) {
        case aPENG:
            RecvPeng(dir);
            break;
        case aMING_GANG:
            {
                _lastActionSource=dir;
                _actionToDo=a_MING_GANG;
                _lastAction=a_MING_GANG;
                _lastActionWithGold=aMING_GANG;
                
                if(!_isNewDistributed) {
                    _players[_curPlayer]->_river->pop_back();
    
			        RecordOutCard(card);
			        RecordOutCard(card);
			        RecordOutCard(card);
                    
                    _curPlayer = dir;
                    
                    _players[dir]->hand_in(
                        card,
                        _isNewDistributed,
                        false,
                        (_distributedNum==TOTAL_CARD_NUM),
                        _lastActionWithGold,
                        _continue_gang_times,
                        _isGangHua
                    );
                }else {
                    LOGGER_WRITE("NOTE: this is ming gang by himself");
                }

                _uiManager->_MingGangEffect(dir,whoGive,card,NULL);
            }
            break;
        case aAN_GANG:
            {
                _lastActionSource = dir;
                
                if(_actionToDo&a_AN_GANG) {
                    _actionToDo=a_AN_GANG;
                    _lastAction=a_AN_GANG;
                    _lastActionWithGold=aAN_GANG;
                } else if(_actionToDo&a_SHOU_GANG) {
                    _actionToDo=a_SHOU_GANG;
                    _lastAction=a_SHOU_GANG;
                    _lastActionWithGold = aSHOU_GANG;
                }
                
                int *gangCardIdx = new int(4);
                _uiManager->GangEffect(dir,CARD_UNKNOWN,gangCardIdx);
            }
            break;
    }
}

void NetRoundManager::_UpdateWin(HuInfo_t *player) {
    int winnerNum = 0;
    int winner = 0;
    int loser = 0;
    
    for(int i=0;i<PLAYER_NUM;i++) {
        if( player[i].winType==ZI_MO ) {
            SetWin(SINGLE_WIN,i);
            break;
        } else if( player[i].winType==HU_PAI ) {
            winner = i;
            winnerNum++;
        } else if( player[i].winType==DIAN_PAO ) {
            loser = i;
        }
    }

    if(winnerNum==1) {
        SetWin(SINGLE_WIN,winner);
    } else {
        SetWin(DOUBLE_WIN,loser);
    }
}

void NetRoundManager::_DiRecv(HuInfoNotif *info) {
    _UpdateWin(info->hu);

    for(int i=0;i<PLAYER_NUM;i++) {
        _players[i]->init(info->hu[i].card, info->hu[i].cardNum, aim[i]);
    }

    delete info;
    
    if(_isWaitDecision) {
        _isWaitDecision = false;
        _actionToDo = _tempActionToDo;
        _tempActionToDo = a_JUMP;
    }

    if(_isQiangGangAsking) {
        _lastActionWithGold = aQIANG_GANG;
    }

    _uiManager->HuEffect(_lastWin, _isQiangGangAsking);
}

void NetRoundManager::_DiRecv(EnterRoomResponse *info) {
    LOGGER_WRITE("NOTE: profile of players should be updated here\n");
    LOGGER_WRITE("Start heartbeats\n");
    KwxHeart::getInstance();
}

void NetRoundManager::_DiRecv(EnterRoomNotif *info) {
    LOGGER_WRITE("NOTE: profile of players should be updated here\n");
}

void NetRoundManager::UpdateCards(PlayerDir_t dir,ARRAY_ACTION action,Card_t actKind) {
    if(dir==MIDDLE) {
        RoundManager::UpdateCards(dir,action);
    } else {
        if(action==aPENG) {
            _isNewDistributed = false;
        }
        
        if(_actionToDo&aAN_GANG) {
            _players[dir]->others_action(_isNewDistributed,aAN_GANG,actKind);
        } else if(_actionToDo&aSHOU_GANG) {
            _players[dir]->others_action(_isNewDistributed,aSHOU_GANG,actKind);
        } else {
            _players[dir]->others_action(_isNewDistributed,(ActionId_t)action,actKind);
        }
    }
}

void NetRoundManager::ServerWaitForMyAction() {
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

	if(_isNewDistributed) {
		if(_lastAction==a_JUMP&&!(_lastActionSource==1&&_continue_gang_times!=0)) {
			_continue_gang_times=0;
        }

		_lastAction=a_JUMP;
		WaitForMyChoose();
	}
}

void NetRoundManager::ServerDistributeTo(PlayerDir_t dir,Card_t card) {
    if(_distributedNum<TOTAL_CARD_NUM+1) {
		_lastHandedOutCard = card;
        _isNewDistributed  = true;

        DistributeInfo_t distInfo;
        
        distInfo.target = dir;
        distInfo.card   = card;
        distInfo.distNum = _distributedNum;
        
        _uiManager->_DistributeEvent(DISTRIBUTE_DONE_EVENT_TYPE,&distInfo);
    } else {
		_uiManager->_DistributeEvent(NOONE_WIN_EVENT_TYPE,NULL);
    }
}

/****************************************
       main interface
****************************************/
void NetRoundManager::CreateRace(Scene *scene) {
    _uiManager = RaceLayer::create();
    scene->addChild(_uiManager);

    _uiManager->Assign(this);

    InitPlayers();
	_isGameStart=false;

    SeatInfo *seat = SeatInfo::getInstance();
    seat->Set(1,0x10010000,1,1);

    ListenToMessenger();
    _messenger->StartReceiving();
    
    /**********************************************/
    /* enter room should be called somewhere else */
    RequestEnterRoom aReq;
    aReq.Set();
    _messenger->Send(aReq);
    /**********************************************/
    
    _uiManager->CreateRace();
}

void NetRoundManager::StartGame() {
	_isGameStart=false;
    
    RequestGameStart aReq;
    aReq.Set();
    _messenger->Send(aReq);
}

void NetRoundManager::RecvPeng(PlayerDir_t dir) {
    PlayerDir_t prevPlayer;
    
    if(_isWaitDecision) {
        _isWaitDecision = false;
        _actionToDo = _tempActionToDo;
        _tempActionToDo = a_JUMP;
    }
    
    _continue_gang_times = 0;
    _lastAction=a_PENG;


    Card_t pengCard = _players[_curPlayer]->_river->get_kind(_players[_curPlayer]->_river->last());
    _players[_curPlayer]->_river->pop_back();
    
    RecordOutCard(pengCard);
    RecordOutCard(pengCard);

    prevPlayer = (PlayerDir_t)_curPlayer;
    _curPlayer = dir;

    _players[dir]->hand_in(
        _lastHandedOutCard,
        _isNewDistributed,
        false,
        (_distributedNum==TOTAL_CARD_NUM),
        _lastActionWithGold,
        _continue_gang_times,
        _isGangHua
    );

    if(dir==MIDDLE) {
        RequestSendAction aReq;
        aReq.Set(aPENG,pengCard);
        _messenger->Send(aReq);
    }

    LOGGER_WRITE("NOTE: maybe this action should be taken later\n");
    _uiManager->PengEffect(dir,prevPlayer,pengCard);
}

void NetRoundManager::RecvHu(PlayerDir_t dir) {
    if(dir==MIDDLE) {
        RequestSendAction aReq;
        aReq.Set(aHU);
        _messenger->Send(aReq);
    }
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
    Card_t gangCard;

    CardInHand *cards = _players[dir]->_cards;

	if( _actionToDo&a_AN_GANG || _actionToDo&a_SHOU_GANG ) {
		_lastActionSource = dir;
        
		if(_actionToDo&a_AN_GANG) {
			_actionToDo=a_AN_GANG;
			_lastAction=a_AN_GANG;
			_lastActionWithGold = aAN_GANG;
		} else if(_actionToDo&a_SHOU_GANG) {
			_actionToDo=a_SHOU_GANG;
			_lastAction=a_SHOU_GANG;
			_lastActionWithGold = aSHOU_GANG;
		}
        
        gangCard = cards->find_an_gang_cards(gangCardIdx);
        
		if( !IsTing(_curPlayer) ) {
			SetEffectCard(gangCard,c_AN_GANG);
		}

        _uiManager->GangEffect(dir,gangCard,gangCardIdx);
	}
	else if( _actionToDo & a_MING_GANG ) {
		_lastActionSource=dir;
		_actionToDo=a_MING_GANG;
		_lastAction=a_MING_GANG;
		_lastActionWithGold = aMING_GANG;

		PlayerDir_t prevPlayer = (PlayerDir_t)_curPlayer;
        
		if(!_isNewDistributed) {
            CardNode_t * last = _players[_curPlayer]->_river->back();
            gangCard = last->kind;
            _players[_curPlayer]->_river->pop_back();
    
			RecordOutCard(gangCard);
			RecordOutCard(gangCard);
			RecordOutCard(gangCard);
            
            _players[dir]->hand_in(
                _lastHandedOutCard,
                true,
                false,
                (_distributedNum==TOTAL_CARD_NUM),
                _lastActionWithGold,
                _continue_gang_times,
                _isGangHua
            );
            
			_curPlayer=dir;
		}else {
		    gangCard = cards->get_kind(cards->last());
			RecordOutCard( gangCard );
		}

        gangCard = cards->find_ming_gang_cards(gangCardIdx,gangCard);
        _uiManager->GangEffect(dir,gangCard,gangCardIdx,false,prevPlayer);
	}
    
    if(dir==MIDDLE) {
        RequestSendAction aReq;
        aReq.Set(aAN_GANG,gangCard);
        _messenger->Send(aReq);
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

void NetRoundManager::RecvHandout(int chosen,Vec2 touch,int mode) {

    if(_isGangAsking) {
        _isGangAsking = false;
    }
    
	if(_isMingTime) {
		_isMingTime=false;
        
        RequestSendAction aReq;
        aReq.Set(aMING);
        _messenger->Send(aReq);
	} else {
        if(_actionToDo&a_MING) {
            _actionToDo = a_JUMP;
        }
    }
    
	if(_isWaitDecision) {
		_isWaitDecision=false;
		_tempActionToDo=a_JUMP;
	}

    RecordOutCard(_players[MIDDLE]->_cards->get_kind(chosen));
    _lastHandedOutCard = _players[MIDDLE]->hand_out(chosen);
    _players[MIDDLE]->_river->push_back(_lastHandedOutCard);

    RequestShowCard aReq;
    aReq.Set(_lastHandedOutCard);
    _messenger->Send(aReq);

    bool turnToMing = false;
	if(_actionToDo==a_MING && !IsTing(_curPlayer) ) {
        _players[_curPlayer]->_cards->set_ming(chosen);

        turnToMing = true;
    }

    _uiManager->MyHandoutEffect(chosen,touch,mode,turnToMing);
}

void NetRoundManager::RecvKouCancel() {
    _players[MIDDLE]->_cards->clear_kou_choices();
    _uiManager->KouCancelEffect(_players[MIDDLE]->_cards);
}

void NetRoundManager::RecvKouConfirm() {
    CardInHand *cards = _players[MIDDLE]->_cards;

    for(int i=cards->FreeStart;i<cards->size();i++) {
        if(cards->get_status(i)==c_KOU_ENABLE)
            cards->set_status(i,sFREE);
    }   
    
    UpdateCards(MIDDLE,a_KOU);
    cards->collect_ming_info(_gRiver);

    Card_t kinds[4];
    int    kindNum = _players[MIDDLE]->_cards->get_kou_kinds(kinds);
    
    RequestSendAction aReq;
    aReq.Set(aKOU,kindNum,kinds);
    _messenger->Send(aReq);

    _uiManager->KouConfirmEffect();
}

void NetRoundManager::RecvMingCancel() {
    _isMingTime=false;
    
    UpdateCards(MIDDLE,a_KOU_CANCEL);
    _actionToDo=a_JUMP;
    
    _players[MIDDLE]->_cards->cancel_ming();
    /*!!!BUG MAYBE HERE : should clear MingInfo_t of cardsInHand*/

    _uiManager->MingCancelEffect();
}

void NetRoundManager::RecvMing(bool isFromKouStatus) {
	_actionToDo=a_MING;

    if(!isFromKouStatus) {
        _players[MIDDLE]->_cards->scan_kou_cards();
        if(_players[MIDDLE]->_cards->kou_group_num()>0) {
            _uiManager->QueryKouCards();
        } else {
            _uiManager->QueryMingOutCard();
        }
    } else {
        _isMingTime=true;
        
        UpdateCards(MIDDLE,a_MING);
        _uiManager->QueryMingOutCard();
    }
}


void NetRoundManager::WaitForMyAction() {
    LOGGER_WRITE("%s (%d) wait",__FUNCTION__,1);
}

void NetRoundManager::WaitForMyChoose() {
	if(_isNewDistributed) {/* is this judgement neccessary??? */
		if( _isTuoGuan ||
                (IsTing(_curPlayer) && !_isGangAsking) ) {
            int last = _players[MIDDLE]->_cards->last();
            
            Vec2 location = _uiManager->GetCardPositionInHand(last);
            RecvHandout(last,location,2);
            
		} else {
			_isMyShowTime = true;
        }
	}
}

void NetRoundManager::WaitForOthersAction(PlayerDir_t dir) {
    LOGGER_WRITE("%s (%d) wait",__FUNCTION__,dir);
}

void NetRoundManager::WaitForOthersChoose() {
    LOGGER_WRITE("%s (%d) waiting\n",__FUNCTION__,_curPlayer);
}

void NetRoundManager::WaitForResponse(PlayerDir_t dir) {
    LOGGER_WRITE("%s (%d) waiting\n",__FUNCTION__,dir);
}

void NetRoundManager::DistributeTo(PlayerDir_t dir,Card_t card) {
}

void NetRoundManager::ActionAfterGang(PlayerDir_t dir) {
}

void NetRoundManager::WaitForFirstAction(PlayerDir_t zhuang) {
    _isGameStart = true;

    _curPlayer = zhuang;
    _ai->UpdateAtFirstRound(_actionToDo);

    if(zhuang==MIDDLE) {
        ServerWaitForMyAction();
    } else {
        WaitForOthersAction((PlayerDir_t)zhuang);
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

