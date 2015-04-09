
#include <stdlib.h>

#include "RaceType.h"
#include "Raction.h"

#include "NetRole.h"
#include "NetRaceLayer.h"
#include "RoundManager.h"

RoundManager::RoundManager(NetRaceLayer *uiManager) {
    _MODE = LOCAL_GAME;

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
    _logger = LOGGER_REGISTER("RoundManager");
}

RoundManager::~RoundManager() {
    delete _gRiver;
    for(int i=0;i<PLAYER_NUM;i++) {
        delete _players[i];
    }
    
    LOGGER_DEREGISTER(_logger);
}

/***********************************************
        winner information
***********************************************/
PlayerDir_t RoundManager::GetLastWinner() {
    if( _lastWin.winner==INVALID_DIR ) {
        LOGGER_WRITE("NETWORK: Request(last winner) not defined");
        _lastWin.winner = MIDDLE;
    }
    return _lastWin.winner;
}

void RoundManager::SetWin(WinKind_t kind,int player) {
    _lastWin.kind       = kind;

    if(kind==DOUBLE_WIN) {
        _lastWin.winner = INVALID_DIR;
        _lastWin.giver  = (PlayerDir_t)player;
    } else {
        _lastWin.winner = (PlayerDir_t)player;
        _lastWin.giver  = (PlayerDir_t)_curPlayer;
    }
}

void RoundManager::GetWin(WinInfo_t &info) {
    info.kind   = _lastWin.kind;
    info.winner = _lastWin.winner;
    info.giver  = _lastWin.giver;
}

bool RoundManager::IsWinner(int no) {
	if((_lastWin.kind==SINGLE_WIN&&
                ((_lastWin.winner==_curPlayer && _lastWin.winner!=no)
                ||(_lastWin.winner!=_curPlayer && no!=_lastWin.winner && no!=_curPlayer)))
        ||(_lastWin.kind==NONE_WIN && _firstMingNo!=INVALID && no!=_firstMingNo)) {
        return true;
    } else {
        return false;
    }
}

/***********************************************
        general operations
***********************************************/
bool RoundManager::IsTing(int id) {
    return _players[id]->get_parter()->get_ting_status();
}

PlayerDir_t RoundManager::TurnToNext() {
    _curPlayer = (_curPlayer+1)%PLAYER_NUM;
    return (PlayerDir_t)_curPlayer;
}

/***********************************************
        river information
***********************************************/
void RoundManager::RecordOutCard( CardNode_t *card ) {
    _gRiver->push_back(card->kind);
    _gRiver->show();
}

void RoundManager::RenewOutCard() {
    delete _gRiver;
	_gRiver = new CardList;
}

/***********************************************
        player information
***********************************************/
#include "NetRole.h"
#include "NetPlayer.h"
#include "NetRaceRound.h"

void RoundManager::InitPlayers() {
	_players[0] = new NetPlayer();
	_players[1] = new NetRole();
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

void RoundManager::_GenerateIds(int ids[]) {
    ids[0]=rand()%16;
    ids[1]=17;
    
    do {
        ids[2]=rand()%16;
    } while( ids[2]==ids[0] );
}

int RoundManager::Shuffle() {
	for(int j=0;j<2;j++) {//伪随机数列生成
		for(int i=0;i<TOTAL_CARD_NUM;i++) {
			int tmp = _unDistributedCards[i];
			int cur = rand()%TOTAL_CARD_NUM;
			_unDistributedCards[i] = _unDistributedCards[cur];
			_unDistributedCards[cur] = tmp;
		}
	}
	////////////////////测试/////////////////////////
	//[0,3]一条 [4,7]二条 [8,11]三条 [12,15]四条 [16,19]五条 [20,23]六条 [24,27]七条 [28,31]八条 [32,35]九条 
	//[36,39]一筒 [40,43]二筒 [44,47]三筒 [48,51]四筒 [52,55]五筒 [56,59]六筒 [60,63]七筒 [64,67]八筒 [68,71]九筒
	//[72,75]红中 [76,79]发财 [80,83]白板
	int oneSeq[14]={0,0,0,8,9,10,12,16,20,24,28,32,36,39};
	int twoSeq[13]={36,36,36,40,40,44,44,48,56,60,64,68,68};
	int threeSeq[13]={36,36,36,40,40,44,44,48,56,60,64,68,68};
	_unDistributedCards[40]=0;
	_unDistributedCards[41]=4;
	_unDistributedCards[42]=8;
	_unDistributedCards[43]=12;
	_unDistributedCards[44]=16;
	_unDistributedCards[45]=20;
	_unDistributedCards[46]=24;
	_unDistributedCards[47]=80;
	_unDistributedCards[48]=80;
	_unDistributedCards[49]=80;
	_unDistributedCards[50]=80;
	_unDistributedCards[51]=80;
	int c=0;
	for(int a=0;a<14;a++)
	{
		if(oneSeq[a]<0||oneSeq[a]>83)
		{
			CCLOG("ERROR CARDS");
			return 0;
		}
		_unDistributedCards[c++]=oneSeq[a];
	}
	for(int a=0;a<13;a++)
	{
		if(twoSeq[a]<0||twoSeq[a]>83)
		{
			CCLOG("ERROR CARDS");
			return 0;
		}
		_unDistributedCards[c++]=twoSeq[a];
	}
	for(int a=0;a<13;a++)
	{
		if(threeSeq[a]<0||threeSeq[a]>83)
		{
			CCLOG("ERROR CARDS");
			return 0;
		}
		_unDistributedCards[c++]=threeSeq[a];
	}


    char p[TOTAL_CARD_NUM] = {0};
    for(int i=0;i<TOTAL_CARD_NUM;i++) {
        p[i] = (_unDistributedCards[i])/4;
    }
    LOGGER_WRITE_ARRAY(p,TOTAL_CARD_NUM);

    _isGangAsking = false;
    _isQiangGangAsking = false;
    _isDoubleHuAsking = false;
    _isCardFromOthers = false;
    _firstMingNo = INVALID;
    _qiangGangTargetNo = INVALID;
    _otherOneForDouble = INVALID;
    _isWaitDecision = false;
    _isGangHua = false;
    _isMyShowTime = false;
    _isTuoGuan = false;
    _otherHandedOut = CARD_UNKNOWN;
    
    CancelEffectCard();
    _isMingTime = false;

	_actionToDo=a_JUMP;
	_continue_gang_times=0;
    _lastAction = INVALID;
    _lastActionWithGold = INVALID;
    _lastActionSource = INVALID;

    _curPlayer = GetLastWinner();

    _distributedNum = 40;

    return 0;
}


/****************************************
        before start
****************************************/
bool RoundManager::GetReadyStatus(PlayerDir_t dir) {
    LOGGER_WRITE("NETWORK : %s %d",__FUNCTION__,dir);
    return true;
}

bool RoundManager::WaitUntilAllReady() {
    LOGGER_WRITE("NETWORK : %s",__FUNCTION__);
    while( !GetReadyStatus(LEFT) || !GetReadyStatus(RIGHT) ) {
        //delay
    }

    return true;
}

void RoundManager::set_aims_sequence(const int p_aim[]) {
    LOGGER_WRITE("%s",__FUNCTION__);
	for(int i=0;i<3;i++)
		aim[i]=p_aim[i];
}

/****************************************
        effect card
****************************************/
void RoundManager::CancelEffectCard() {
    _curEffectCard.kind = ck_NOT_DEFINED;
    _curEffectCard.status = c_NOT_DEFINDED;
}

void RoundManager::SetEffectCard(int kind,int status) {
    _curEffectCard.kind = (CARD_KIND)kind;
    _curEffectCard.status = (CARD_STATUS)status;
}

bool RoundManager::IsCurEffectCard(const CardNode_t *card) {
    if(card->kind==_curEffectCard.kind && card->status==_curEffectCard.status) {
        return true;
    } else {
        return false;
    }
}

/****************************************
       main interface
****************************************/
void RoundManager::CreateRace(Scene *scene) {
    _uiManager = NetRaceLayer::create();
    scene->addChild(_uiManager);

    _uiManager->Set(this);

    InitPlayers();
	_isGameStart=false;
    
    _uiManager->CreateRace();
}

void RoundManager::StartGame() {
	_isGameStart=false;
    
    _players[MIDDLE]->_isReady = true;
	_uiManager->GuiShowReady(MIDDLE);
    WaitUntilAllReady();

    RenewOutCard();
    Shuffle();

    int lastWinner = GetLastWinner();
    _actionToDo = _players[(lastWinner)%3]->init(&(_unDistributedCards[0]),14,aim[lastWinner]);//鐜╁鎵嬬墝鍒濆鍖?
	if(_actionToDo!=a_TIMEOUT) {
		_players[(lastWinner+1)%3]->init(&(_unDistributedCards[14]),13,aim[(lastWinner+1)%3]);
		_players[(lastWinner+2)%3]->init(&(_unDistributedCards[27]),13,aim[(lastWinner+2)%3]);
		_uiManager->FirstRoundDistributeEffect((PlayerDir_t)lastWinner);//鐗屽眬寮�濮嬪彂鐗屾晥鏋溿�?
	}
}

void RoundManager::RecvPeng(PlayerDir_t dir) {
    PlayerDir_t prevPlayer;
    
    if(_isWaitDecision) {
        _isWaitDecision = false;
        _actionToDo = _tempActionToDo;
        _tempActionToDo = a_JUMP;
    }
    
    _continue_gang_times = 0;
    _lastAction=a_PENG;
    
    CardNode_t * last = _players[_curPlayer]->_river->back();
    Card_t card = last->kind;    
    
    RecordOutCard(last);
    RecordOutCard(last);

    _players[_curPlayer]->_river->pop_back();

    prevPlayer = (PlayerDir_t)_curPlayer;
    _curPlayer = dir;

    _uiManager->PengEffect(dir,prevPlayer,card);
}

void RoundManager::RecvHu(PlayerDir_t dir) {
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

void RoundManager::RecvGang(PlayerDir_t dir) {
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
    
	auto list=_players[dir]->_cardsInHand;
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

		Card_t GangCard;
		PlayerDir_t prevPlayer = (PlayerDir_t)_curPlayer;
        
		if(_isCardFromOthers) {
            CardNode_t * last = _players[_curPlayer]->_river->back();
            GangCard = last->kind;
    
			RecordOutCard(last);
			RecordOutCard(last);
			RecordOutCard(last);
            
            _players[_curPlayer]->_river->pop_back();

            _curPlayer=dir;
		}else {
            CardNode_t * last = _players[_curPlayer]->_river->back();
            GangCard = last->kind;
    
			RecordOutCard(last);
		}

        card = _ai->FindGangCards(gangCardIdx,list,GangCard,_actionToDo,IsTing(dir),_isCardFromOthers);
        _uiManager->GangEffect(dir,GangCard,gangCardIdx,false,prevPlayer);
	}
}

void RoundManager::RecvQi() {
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

void RoundManager::RecvHandout(int idx,Vec2 touch,int mode) {
    auto cardsInHand = _players[MIDDLE]->_cardsInHand;

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

    RecordOutCard(_players[_curPlayer]->_cardsInHand->at(idx));
    _lastHandedOutCard = _players[_curPlayer]->get_parter()->hand_out(idx);
    _players[_curPlayer]->_river->push_back((Card_t)_lastHandedOutCard);

    bool turnToMing = false;
	if(_actionToDo==a_MING && 
        !IsTing(_curPlayer) ) {
		_players[_curPlayer]->get_parter()->LockAllCards();
		_players[_curPlayer]->get_parter()->set_ting_status(1);

        turnToMing = true;
    }

    _uiManager->MyHandoutEffect(idx,cardsInHand,touch,mode,turnToMing);
}

void RoundManager::QiangGangHuJudge(PlayerDir_t dir) {
    LOGGER_WRITE("%s",__FUNCTION__);

    _qiangGangTargetNo = dir;

	_isCardFromOthers=true;
	unsigned char curTingStatus=_players[_curPlayer]->get_parter()->get_ting_status();
    
	int no1=(_curPlayer+1)%3;
    unsigned char action1=_players[no1]->get_parter()->hand_in(
        _lastHandedOutCard,
        _isCardFromOthers,
        curTingStatus,
        false,
        a_QIANG_GANG,
        _continue_gang_times,
        _isGangHua
    );

	int no2=(_curPlayer+2)%3;
	unsigned char action2=_players[no2]->get_parter()->hand_in(
        _lastHandedOutCard,
        _isCardFromOthers,
        curTingStatus,
        false,
        a_QIANG_GANG,
        _continue_gang_times,
        _isGangHua
    );

	if((action1&a_HU)&&(action2&a_HU)) {
        WinInfo_t win;
        win.kind  = DOUBLE_WIN;
        win.giver = (PlayerDir_t)_curPlayer;
        
        if(no1==1) {
            _actionToDo=action1;
            _otherOneForDouble = no2;
        } else {
            _actionToDo=action2;
            _otherOneForDouble = no1;
        }

        _isDoubleHuAsking = true;
        _lastActionWithGold=a_QIANG_GANG;

        _uiManager->DoubleWin(win);
	} else if(action1&a_HU||action2&a_HU) {
        WinInfo_t win;
        win.kind = SINGLE_WIN;
        win.winner = (PlayerDir_t)((action1&a_HU) ? no1 : no2);
        win.giver = (PlayerDir_t)_curPlayer;

        if(no1==1)
            _actionToDo=action1;
        else
            _actionToDo=action2;

        _isQiangGangAsking=true;
        _lastActionWithGold=a_QIANG_GANG;
        
        _uiManager->SingleWin(win);
	} else {
		_isCardFromOthers=false;

        _uiManager->GangGoldEffect(_qiangGangTargetNo,_curPlayer);
	}
}

int RoundManager::_GroupIdx(int idx,CardList *cards) {
    if((cards->get_kind(idx)==cards->get_kind(idx+1))&&(cards->get_kind(idx)==cards->get_kind(idx+2))&&(cards->get_kind(idx)==cards->get_kind(idx+3))&&(cards->get_kind(idx)!=cards->get_kind(idx+4)))
        return 1;
    else if((cards->get_kind(idx)==cards->get_kind(idx+1))&&(cards->get_kind(idx)==cards->get_kind(idx+2))&&(cards->get_kind(idx)!=cards->get_kind(idx+3)))
        return 2;
    else if(cards->get_kind(idx)==cards->get_kind(idx+1)&&cards->get_kind(idx)!=cards->get_kind(idx+2))
        return 3;
    else if(cards->get_kind(idx)!=cards->get_kind(idx+1))
        return 4;
}

CartApperance_t RoundManager::GetCardApperance(PlayerDir_t dir,int idx) {
	auto cards = _players[dir]->_cardsInHand;
    CardStatus_t status = cards->get_status(idx);

    bool isTing       = IsTing(dir);
    bool isMiddleTing = IsTing(MIDDLE);
    
    if(status==sFREE) {
        if(isTing) {
            return LAYDOWN_SHOW ;
        } else if (isMiddleTing) {
            return LAYDOWN_HIDE ;
        }
    } else if(status==sPENG || status==sMING_GANG) {
        return LAYDOWN_SHOW ;
    } else if(status==sAN_GANG) {
        int groupIdx = _GroupIdx(idx,cards);
        
        if((dir==LEFT&&groupIdx==3) || (dir==RIGHT&&groupIdx==2)) {
            if(!isTing && isMiddleTing) {/* here must be a bug */
                if(isTing) {
                    return LAYDOWN_SHOW;
                } else if(!isTing&&isMiddleTing) {
                    return LAYDOWN_HIDE;
                }
            }
        }
    }

    return NORMAL_APPERANCE;
}

void RoundManager::RecvKouCancel() {
    auto cards = _players[MIDDLE]->_cardsInHand;

    for(int i=cards->FreeStart;i<cards->size();i++) {
        cards->set_status(i,sFREE);
    }

    _uiManager->KouCancelEffect(cards);
}

void RoundManager::RecvKouConfirm() {
    auto cards = _players[MIDDLE]->_cardsInHand;

    for(int i=cards->FreeStart;i<cards->size();i++) {
        if(cards->get_status(i)==sKOU_ENABLE)
            cards->set_status(i,sFREE);
    }   
    
    UpdateCards(MIDDLE,a_KOU);
    
    auto ming_indexesCur=_players[MIDDLE]->get_parter()->ming_check();
    _players[MIDDLE]->get_parter()->set_ming_indexes(ming_indexesCur);

    _uiManager->KouConfirmEffect();
}

void RoundManager::RecvMingCancel() {
    _isMingTime=false;
    
    UpdateCards(MIDDLE,a_KOU_CANCEL);
    _actionToDo=a_JUMP;
    
    _players[1]->get_parter()->MingCancel();
    _players[1]->get_parter()->set_ming_indexes(0);
    _players[1]->get_parter()->set_ting_status(0);

    _uiManager->MingCancelEffect();
}

void RoundManager::RecvMing() {
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

void RoundManager::WaitForFirstAction(PlayerDir_t zhuang) {
    _isGameStart = true;

    _curPlayer = zhuang;
    _ai->UpdateAtFirstRound(_actionToDo);

    if(zhuang==MIDDLE) {
        WaitForMyAction();
    } else {
        WaitForOthersAction((PlayerDir_t)zhuang);
    }
}

void RoundManager::WaitForMyAction() {
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

void RoundManager::WaitForMyChoose() {
	if(!_isCardFromOthers) {/* is this judgement neccessary??? */
		if( _isTuoGuan ||
                (IsTing(_curPlayer) && !_isGangAsking) ) {
            auto cards = _players[MIDDLE]->_cardsInHand;
            Vec2 location = _uiManager->GetCardPositionInHand(cards->size()-1);
            RecvHandout(cards->size()-1,location,2);
            
		} else {
			_isMyShowTime = true;
        }
	}
}

void RoundManager::WaitForOthersAction(PlayerDir_t dir) {
    LOGGER_WRITE("%s (%d) perform action %d",__FUNCTION__,dir,_actionToDo);
    auto list=_players[dir]->_cardsInHand;

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

        Card_t GangCard;
        PlayerDir_t prevPlayer = (PlayerDir_t)dir;
        if(_isCardFromOthers) {
            CardNode_t * last = _players[_curPlayer]->_river->back();
            GangCard = last->kind;

            RecordOutCard(last);
            RecordOutCard(last);
            RecordOutCard(last);
            
            _players[_curPlayer]->_river->pop_back();

            _curPlayer=dir;
        }else {
            CardNode_t * last = _players[_curPlayer]->_river->back();
            GangCard = last->kind;

            RecordOutCard(last);

            _players[_curPlayer]->_river->pop_back();
        }

        int* gangIdx=new int[4];
        Card_t card = _ai->FindGangCards(gangIdx,list,GangCard,_actionToDo,IsTing(dir),_isCardFromOthers);
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

void RoundManager::WaitForOthersChoose() {
    if ( _curPlayer==1 ) {/* this should never happen */
        return;
    }

    bool canKou = false;
	int index = _ai->ChooseWorstCard(canKou);
    
    if ( canKou ) {
        _otherHandedOut = _players[_curPlayer]->_cardsInHand->get_kind(index);
        
        _ai->KouCardCheck((PlayerDir_t)_curPlayer);
        if(_ai->KouCardGroupNum()>0) {
            _ai->MingKouChoose((PlayerDir_t)_curPlayer);
        }
    }

    RecordOutCard(_players[_curPlayer]->_cardsInHand->at(index));
	_lastHandedOutCard=_players[_curPlayer]->get_parter()->hand_out(index);
    _players[_curPlayer]->_river->push_back((Card_t)_lastHandedOutCard);

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

void RoundManager::WaitForResponse(PlayerDir_t dir) {
    unsigned char curTingStatus=_players[dir]->get_parter()->get_ting_status();
    
    if(!_isCardFromOthers) {
        _isGangHua=false;
        if(_lastActionSource==dir&&_continue_gang_times!=0)
            _isGangHua=true;
        else
            _continue_gang_times=0;
        
        _actionToDo = 
            _players[dir]->get_parter()->hand_in(
                _lastHandedOutCard,
                _isCardFromOthers,
                curTingStatus,
                (_distributedNum==TOTAL_CARD_NUM),
                _lastActionWithGold,
                _continue_gang_times,
                _isGangHua
            );
        
        if(dir!=MIDDLE)
        {
            if(_players[dir]->get_robot_hu_target()==SAME_TIAO_TARGET)
            {
                if(_lastHandedOutCard/9!=0&&!(_actionToDo&a_HU)&&!(_actionToDo&a_AN_GANG)&&!(_actionToDo&a_SHOU_GANG)&&!(_actionToDo&a_MING_GANG))
                    _actionToDo=a_JUMP;
            }
            else if(_players[dir]->get_robot_hu_target()==SAME_TONG_TARGET)
            {
                if(_lastHandedOutCard/9!=1&&!(_actionToDo&a_HU)&&!(_actionToDo&a_AN_GANG)&&!(_actionToDo&a_SHOU_GANG)&&!(_actionToDo&a_MING_GANG))
                    _actionToDo=a_JUMP;
            }
            else if(_players[dir]->get_robot_hu_target()==SEVEN_COUPLES_TARGET)
                if(!(_actionToDo&a_HU)&&!(_actionToDo&a_AN_GANG)&&!(_actionToDo&a_SHOU_GANG)&&!(_actionToDo&a_MING_GANG))
                    _actionToDo=a_JUMP;
        }
        
        if((PlayerDir_t)dir==MIDDLE) {
            if(IsTing(MIDDLE)&&(_actionToDo&a_HU)){
                RecvHu(MIDDLE);
            }else{
                if(_isTuoGuan)
                    _actionToDo=a_JUMP;
                WaitForMyAction();
            }
        }else{
            WaitForOthersAction((PlayerDir_t)dir);
        }
    }else{
        int no=((PlayerDir_t)dir+1)%3;
        
        unsigned char action1 = 
            _players[no]->get_parter()->hand_in(
                _lastHandedOutCard,
                _isCardFromOthers,
                curTingStatus,
                (_distributedNum==TOTAL_CARD_NUM),
                _lastActionWithGold,
                _continue_gang_times,
                _isGangHua
            );
        
        if(no==MIDDLE&&_isTuoGuan) {
            if(IsTing(MIDDLE)&&(action1&a_HU))
                action1=a_HU;
            else
                action1=a_JUMP;
        } else if(no!=MIDDLE) {
            if(_players[no]->get_robot_hu_target()==SAME_TIAO_TARGET)
            {
                if(_lastHandedOutCard/9!=0&&!(action1&a_HU)&&!(action1&a_AN_GANG)&&!(action1&a_SHOU_GANG)&&!(action1&a_MING_GANG))
                    action1 = a_JUMP;
            }
            else if(_players[no]->get_robot_hu_target()==SAME_TONG_TARGET)
            {
                if(_lastHandedOutCard/9!=1&&!(action1&a_HU)&&!(action1&a_AN_GANG)&&!(action1&a_SHOU_GANG)&&!(action1&a_MING_GANG))
                    action1 = a_JUMP;
            }
            else if(_players[no]->get_robot_hu_target()==SEVEN_COUPLES_TARGET)
                if(!(action1&a_HU)&&!(action1&a_AN_GANG)&&!(action1&a_SHOU_GANG)&&!(action1&a_MING_GANG))
                    action1=a_JUMP;
        }
        
        int no1=((PlayerDir_t)dir+2)%3;
        
        unsigned char action2=
            _players[no1]->get_parter()->hand_in(
                _lastHandedOutCard,
                _isCardFromOthers,
                curTingStatus,
                (_distributedNum==TOTAL_CARD_NUM),
                _lastActionWithGold,
                _continue_gang_times,
                _isGangHua
            );
        if(no1==1&&_isTuoGuan)
        {
            if(_players[1]->get_parter()->get_ting_status()==1&&(action1&a_HU))
                action2=a_HU;
            else
                action2=a_JUMP;
        }
        if(no1!=MIDDLE)
        {
            if(_players[no1]->get_robot_hu_target()==SAME_TIAO_TARGET)
            {
                if(_lastHandedOutCard/9!=0&&!(action2&a_HU)&&!(action2&a_AN_GANG)&&!(action2&a_SHOU_GANG)&&!(action2&a_MING_GANG))
                    action2 = a_JUMP;
            }
            else if(_players[no1]->get_robot_hu_target()==SAME_TONG_TARGET)
            {
                if(_lastHandedOutCard/9!=1&&!(action2&a_HU)&&!(action2&a_AN_GANG)&&!(action2&a_SHOU_GANG)&&!(action2&a_MING_GANG))
                    action2 = a_JUMP;
            }
            else if(_players[no1]->get_robot_hu_target()==SEVEN_COUPLES_TARGET)
                if(!(action2&a_HU)&&!(action2&a_AN_GANG)&&!(action2&a_SHOU_GANG)&&!(action2&a_MING_GANG))
                    action2=a_JUMP;
        }
        
        if((action1&a_HU)&&(action2&a_HU))
        {
            _uiManager->HideClock();
            
            if((no!=MIDDLE&&no1!=MIDDLE) || (no==MIDDLE||no1==MIDDLE&&IsTing(MIDDLE))){ 
                SetWin(DOUBLE_WIN,(PlayerDir_t)_curPlayer);
                _uiManager->_HuEffect(_lastWin);
                _uiManager->_DistributeEvent(DOUBLE_HU_WITH_ME,NULL);
            } else if((no==MIDDLE||no1==MIDDLE) && !IsTing(MIDDLE)) {
                _isDoubleHuAsking = true;
                if(no==1) {
                    _otherOneForDouble = no1;
                    _actionToDo=action1;
                } else {
                    _otherOneForDouble = no;
                    _actionToDo=action2;
                }                   
                WaitForMyAction();
                return;
            }
        } else if(action1&a_HU||action2&a_HU)//鐐圭偖
        {
            _uiManager->HideClock();
            
            if((no==1&&(action1&a_HU))||(no1==1&&(action2&a_HU))) {
                if(_players[1]->get_parter()->get_ting_status()==1) {
                    RecvHu(MIDDLE);
                } else {
                    if(no==1)
                        _actionToDo=action1;
                    else
                        _actionToDo=action2;
                    WaitForMyAction();
                    return;
                }
            }
            else if(no!=1&&(action1&a_HU)) {
                RecvHu((PlayerDir_t)no);
            }
            else if(no1!=1&&(action2&a_HU)) {
                RecvHu((PlayerDir_t)no1);
            }
        }
        else if(action1!=a_JUMP)//涓嬪
        {
            _actionToDo=action1;
            if(no==1)
            {
                _uiManager->UpdateClock(0,no);
                WaitForMyAction();
                return;
            }
            else
            {
                _uiManager->UpdateClock(0,no);
                WaitForOthersAction((PlayerDir_t)no);
                return;
            }
        }
        else if(action2!=a_JUMP)//涓婂
        {
            _actionToDo=action2;
            if(no1==1)
            {
                _uiManager->UpdateClock(0,no1);
                WaitForMyAction();
                return;
            }
            else
            {
                _uiManager->UpdateClock(0,no1);
                WaitForOthersAction((PlayerDir_t)no1);
                return;
            }
        }
        else if(action1==action2&&action1==a_JUMP)
        {
            _actionToDo=action1;
            _curPlayer=(_curPlayer+1)%3;
            _uiManager->UpdateClock(0,_curPlayer);
            DistributeTo((PlayerDir_t)_curPlayer,(Card_t)(_unDistributedCards[_distributedNum++]/4));
        }
    }
}

void RoundManager::DistributeTo(PlayerDir_t dir,Card_t card) {
    if(_distributedNum<TOTAL_CARD_NUM+1) {
        DistributeInfo_t distInfo;
        
        distInfo.target = dir;
        distInfo.card   = card;
        distInfo.distNum = _distributedNum;
        
        _uiManager->_DistributeEvent(DISTRIBUTE_DONE_EVENT_TYPE,&distInfo);
    } else {
		_uiManager->_DistributeEvent(NOONE_WIN_EVENT_TYPE,NULL);
    }
}

void RoundManager::ActionAfterGang(PlayerDir_t dir) {
    if(!_isCardFromOthers) {
        QiangGangHuJudge(dir);
    } else {
        DistributeTo(dir,(Card_t)(_unDistributedCards[_distributedNum++]/4));
    }
}

void RoundManager::UpdateCards(PlayerDir_t dir,ARRAY_ACTION action,Card_t actKind) {
    if(action==a_PENG) {
        _isCardFromOthers = true;
    }

    if(_actionToDo&a_AN_GANG) {
        _players[dir]->get_parter()->action(_isCardFromOthers,a_AN_GANG);
    } else if(_actionToDo&a_SHOU_GANG) {
        _players[dir]->get_parter()->action(_isCardFromOthers,a_SHOU_GANG);
    } else {
        _players[dir]->get_parter()->action(_isCardFromOthers,action);
    }
}

/*************************************
        singleton
*************************************/
RoundManager* RoundManager::_instance = NULL;

RoundManager *RoundManager::getInstance() {
    if (_instance==NULL) {
        _instance = new RoundManager(NULL);
    }

    return _instance;
}

void RoundManager::destroyInstance() {
    delete _instance;
    _instance = NULL;
}

