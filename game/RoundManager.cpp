
#include <stdlib.h>

#include "RaceType.h"
#include "Raction.h"

#include "NetRole.h"
#include "NetRaceLayer.h"
#include "RoundManager.h"

RoundManager::RoundManager(NetRaceLayer *uiManager) {
    _uiManager = uiManager;

    _lastWin.player = INVALID_DIR;

    _river = NULL;
	for(int i=0;i<TOTAL_CARD_NUM;i++) {
		_unDistributedCards[i]=i;
	}
    _distributedNum = 0;

    for(int i=0;i<PLAYER_NUMBER;i++) {
        _players[i] = NULL;
    }

    _ai = Ai::getInstance(this);
    _logger = LOGGER_REGISTER("RoundManager");
}

RoundManager::~RoundManager() {
    delete _river;
    for(int i=0;i<PLAYER_NUMBER;i++) {
        delete _players[i];
    }
    
    LOGGER_DEREGISTER(_logger);
}

/***********************************************
        winner information
***********************************************/
PlayerDir_t RoundManager::GetLastWinner() {
    if( _lastWin.player==INVALID_DIR ) {
        LOGGER_WRITE("NETWORK: Request(last winner) not defined");
        _lastWin.player = MIDDLE;
    }
    return _lastWin.player;
}

void RoundManager::SetWin(WinKind_t kind,int player) {
    _lastWin.kind       = kind;
    _lastWin.player     = (PlayerDir_t)player;
}

void RoundManager::GetWin(WinInfo_t &info) {
    info.kind   = _lastWin.kind;
    info.player = _lastWin.player;
}

bool RoundManager::IsWinner(int no, int curPlayer, int FirstMingPlayer) {
	if((_lastWin.kind==SINGLE_WIN&&
                ((_lastWin.player==curPlayer && _lastWin.player!=no)
                ||(_lastWin.player!=curPlayer && no!=_lastWin.player && no!=curPlayer)))
        ||(_lastWin.kind==NONE_WIN && FirstMingPlayer!=-1 && no!=FirstMingPlayer)) {
        return true;
    } else {
        return false;
    }
}

/***********************************************
        river information
***********************************************/
void RoundManager::RecordHandOut(int cardIdx) {
    RecordOutCard(_players[_curPlayer]->get_parter()->get_card_list()->data[cardIdx]);
    _lastHandedOutCard = _players[_curPlayer]->get_parter()->hand_out(cardIdx);
}

void RoundManager::RecordOutCard( Card card ) {
    _river->insertItem(card);

    LOGGER_WRITE("RIVER : ");
    char cards[84] = {0};
    int  i = 0;
    outCardNode *p = _river->head;
    while(p->pNext) {
        cards[i++] = p->pNext->data.kind;
        p = p->pNext;
    }
    LOGGER_WRITE_ARRAY(cards,i);
}

void RoundManager::RenewOutCard() {
    delete _river;
	_river = new outCardList;
}

/***********************************************
        player information
***********************************************/
#include "NetMe.h"
#include "NetPlayer.h"
#include "NetRaceRound.h"

void RoundManager::Init() {
    InitPlayers();
}

void RoundManager::InitPlayers() {
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

void RoundManager::_GenerateIds(int ids[]) {
    ids[0]=rand()%16;
    ids[1]=17;
    
    do {
        ids[2]=rand()%16;
    } while( ids[2]==ids[0] );
}

void RoundManager::LoadPlayerInfo() {
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

bool RoundManager::IsTing(int id) {
    return _players[id]->get_parter()->get_ting_status();
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
    _otherHandedOut = CARD_UNKNONW;
    
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

bool RoundManager::GetReadyStatus(int seatId) {
    LOGGER_WRITE("NETWORK : %s %d",__FUNCTION__,seatId);
    return true;
}

bool RoundManager::WaitUntilAllReady() {
    LOGGER_WRITE("NETWORK : %s",__FUNCTION__);
    while( !GetReadyStatus(0) || !GetReadyStatus(2) ) {
        //delay
    }

    return true;
}


void RoundManager::set_aims_sequence(const int p_aim[])
{
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

bool RoundManager::IsCurEffectCard(Card card) {
    if(card.kind==_curEffectCard.kind && card.status==_curEffectCard.status) {
        return true;
    } else {
        return false;
    }
}
/****************************************
        card handler
****************************************/
int RoundManager::FindGangCards(int dir,int cards[4],Card_t target) {
    auto list = _players[dir]->get_parter()->get_card_list();

    if( _actionToDo & a_AN_GANG || _actionToDo & a_SHOU_GANG ) {
        if(!IsTing(dir)) {
            int matchCardNum = 0;
            int firstMatchCard = 0;
            
            for(int i=list->atcvie_place; i<list->len; i++) {
                for(int j=i+1; j<list->len; j++) {
                    if(list->data[i].kind==list->data[j].kind) {
                        matchCardNum++;
                        if(matchCardNum==3) {
                            firstMatchCard = i;
                            break;
                        }
                    }
                }
            }
            
            int idx = 0;
            for(int i=firstMatchCard; i<list->len; i++) {
                if(list->data[firstMatchCard].kind==list->data[i].kind) {
                    cards[idx++] = i;
                }
            }
        } else {
            cards[3] = list->len-1;
        
            int p = 0;
            for(int i=0; i<list->atcvie_place; i++){
                if(list->data[i].kind==list->data[cards[3]].kind) {
                    cards[p++]=i;
                    if(p==3) {
                        break;
                    }
                }
            }
        }
    } else {
		int l_len = _isCardFromOthers ? (list->len) : (list->len-1);
        
		for(int i=0;i<l_len;i++) {
			if(target==list->data[i].kind) {
				if(i==0) {
					cards[0]=i;
				} else if(i>0 && target!=list->data[i-1].kind) {
					cards[0]=i;
				} else if(i==1 && target==list->data[i-1].kind) {
					cards[1]=i;
				} else if(i>1 && target==list->data[i-1].kind && target!=list->data[i-2].kind) {
					cards[1]=i;
				} else if(i>1 && target==list->data[i-1].kind && target==list->data[i-2].kind) {
					cards[2]=i;
				}
			}
		}
    }

    return 0;
}

void RoundManager::RecvPeng(PlayerDir_t dir) {
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
    _curPlayer=dir;

    _uiManager->PengEffect((PlayerDir_t)_curPlayer,prevPlayer,(Card_t)card.kind);
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

    /*!!!just for compile reason for now, the value should be set before play effect*/
    WinInfo_t win;
    win.player = (PlayerDir_t)dir;
    if(_isDoubleHuAsking) {
        win.kind = DOUBLE_WIN;
    }

    _uiManager->HuEffect(win, _isQiangGangAsking);
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
        
        FindGangCards(dir,gangCardIdx);
        card =(Card_t)list->data[gangCardIdx[0]].kind;
        
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

        FindGangCards(dir,gangCardIdx,(Card_t)GangCard.kind);

        _uiManager->GangEffect(dir,(Card_t)GangCard.kind,gangCardIdx,false,prevPlayer);
	}
}

void RoundManager::RecvQi() {
    _uiManager->QiEffect();
}

void RoundManager::CreateRace(Scene *scene) {
    _uiManager = NetRaceLayer::create();
    scene->addChild(_uiManager);

    _uiManager->Set(this);

    InitPlayers();
    LoadPlayerInfo();
	_isGameStart=false;
    
    _uiManager->CreateRace();
}

void RoundManager::StartGame() {
	_isGameStart=false;
    _cardHolders[MIDDLE]->_isReady = true;
    WaitUntilAllReady();

    RenewOutCard();

    int lastWinner = GetLastWinner();
    _actionToDo = _players[(lastWinner)%3]->init(&(_unDistributedCards[0]),14,aim[lastWinner]);//鐜╁鎵嬬墝鍒濆鍖?
	if(_actionToDo!=a_TIMEOUT) {
		_players[(lastWinner+1)%3]->init(&(_unDistributedCards[14]),13,aim[(lastWinner+1)%3]);
		_players[(lastWinner+2)%3]->init(&(_unDistributedCards[27]),13,aim[(lastWinner+2)%3]);
		_uiManager->FirstRoundDistributeEffect(lastWinner);//鐗屽眬寮�濮嬪彂鐗屾晥鏋溿�?
	}
}

void RoundManager::RecvHandout(int idx,Vec2 touch,int mode) {
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

    bool turnToMing = false;
	if(_actionToDo==a_MING && 
        !IsTing(_curPlayer) ) {
		_players[_curPlayer]->get_parter()->LockAllCards();
		_players[_curPlayer]->get_parter()->set_ting_status(1);

        turnToMing = true;
    }

    _uiManager->MyHandoutEffect(idx,cardsInHand,touch,mode,turnToMing);
}

void RoundManager::QiangGangHuJudge() {
    LOGGER_WRITE("%s",__FUNCTION__);

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
        win.kind = DOUBLE_WIN;
        win.player = (PlayerDir_t)_curPlayer;
        
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
        win.player = (PlayerDir_t)((action1&a_HU) ? no1 : no2);

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

int RoundManager::_GroupIdx(int idx,CARD_ARRAY *cards) {
    if((cards->data[idx].kind==cards->data[idx+1].kind)&&(cards->data[idx].kind==cards->data[idx+2].kind)&&(cards->data[idx].kind==cards->data[idx+3].kind)&&(cards->data[idx].kind!=cards->data[idx+4].kind))
        return 1;
    else if((cards->data[idx].kind==cards->data[idx+1].kind)&&(cards->data[idx].kind==cards->data[idx+2].kind)&&(cards->data[idx].kind!=cards->data[idx+3].kind))
        return 2;
    else if(cards->data[idx].kind==cards->data[idx+1].kind&&cards->data[idx].kind!=cards->data[idx+2].kind)
        return 3;
    else if(cards->data[idx].kind!=cards->data[idx+1].kind)
        return 4;
}

CartApperance_t RoundManager::GetCardApperance(PlayerDir_t dir,int idx) {
	CARD_ARRAY *cards = _players[dir]->get_parter()->get_card_list();
    CARD_STATUS status = cards->data[idx].status;

    bool isTing       = IsTing(dir);
    bool isMiddleTing = IsTing(MIDDLE);
    
    if(status==c_FREE) {
        if(isTing) {
            return LAYDOWN_SHOW ;
        } else if (isMiddleTing) {
            return LAYDOWN_HIDE ;
        }
    } else if(status==c_PENG || status==c_MING_GANG) {
        return LAYDOWN_SHOW ;
    } else if(status==c_AN_GANG) {
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

TingInfo_t RoundManager::GetTingInfo(PlayerDir_t dir,int outCarIdx) {
    TingInfo_t TingInfo;
    return TingInfo;
}

void RoundManager::RecvKouCancel() {
    auto cards = _players[MIDDLE]->get_parter()->get_card_list();
    for(int i=cards->atcvie_place;i<cards->len;i++) {
        cards->data[i].status=c_FREE;
    }

    _uiManager->KouCancelEffect(cards);
}

void RoundManager::RecvKouConfirm() {
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
            _uiManager->QueryMingOutCard();
        }
    } else {
        WaitForOthersChoose();
    }
}

void RoundManager::WaitForMyAction() {
    _uiManager->ShowActionButtons();

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
                (IsTing(_curPlayer) 
                && !_isGangAsking) ) {
            auto cards = _players[MIDDLE]->get_parter()->get_card_list();
            Vec2 location = _uiManager->GetCardPositionInHand(cards->len-1);
            RecvHandout(cards->len-1,location,2);
            
		} else {
			_isMyShowTime = true;
        }
	}
}

void RoundManager::WaitForOthersAction(PlayerDir_t dir) {
    LOGGER_WRITE("%s (%d) perform action %d",__FUNCTION__,dir,_actionToDo);
    auto list=_players[dir]->get_parter()->get_card_list();

    if(_actionToDo&a_HU) {
        _uiManager->_HuEffect(dir);
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

        Card_t card;
        int* Angang=new int[4];

        FindGangCards(dir,Angang);
        card =(Card_t)list->data[Angang[0]].kind;

        if( !IsTing(dir) ) {/* is dir equals dir ??? */
            SetEffectCard(card,c_AN_GANG);
        }

        _uiManager->_AnGangEffect(dir,card,Angang);
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

        int l_len;
        if(!_isCardFromOthers)
            l_len=list->len-1;
        else
            l_len=list->len;

        int* Angang=new int[4];
        for(int i=0;i<l_len;i++) {/* is this logic neccessary ??? --- gang[1]=gang[0]+1;gang[2]=gang[1]+1*/
            if(GangCard.kind==list->data[i].kind) {
                if(i==0) {
                    Angang[0]=i;
                } else if(i>0 && GangCard.kind!=list->data[i-1].kind) {
                    Angang[0]=i;
                } else if(i==1 && GangCard.kind==list->data[i-1].kind) {
                    Angang[1]=i;
                } else if(i>1 && GangCard.kind==list->data[i-1].kind && GangCard.kind!=list->data[i-2].kind) {
                    Angang[1]=i;
                } else if(i>1 && GangCard.kind==list->data[i-1].kind && GangCard.kind==list->data[i-2].kind) {
                    Angang[2]=i;
                }
            }
        }

        _uiManager->_MingGangEffect(dir,prevPlayer,(Card_t)GangCard.kind,Angang);
    }
    else if(_actionToDo&a_MING) {
        RecvMing();
    } else if(_actionToDo&a_PENG) {
        Card card;

        _continue_gang_times = 0;
        _lastAction=a_PENG;
        
        const int riverLast =_players[dir]->get_parter()->getOutCardList()->length;
        
        _players[dir]->get_parter()->getOutCardList()->getCard(card,riverLast);
        _players[dir]->get_parter()->getOutCardList()->deleteItem();
        
        RecordOutCard(card);
        RecordOutCard(card);
        
        PlayerDir_t prevPlayer = (PlayerDir_t)dir;
        _curPlayer = dir;

        _uiManager->PengEffect((PlayerDir_t)dir,prevPlayer,(Card_t)card.kind);
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

void RoundManager::WaitForResponse(PlayerDir_t dir) {
    unsigned char curTingStatus=_players[dir]->get_parter()->get_ting_status();
    
    if(!_isCardFromOthers)
    {
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
        
        if(_players[dir]->get_parter()->get_role_type()==SINGLE_BOARD_ROBOT)
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
        
        if(_players[dir]->get_parter()->get_role_type()==INTERNET_PLAYER)
        {
            LOGGER_WRITE("NETWORK : NetPlayer action here, %s %d",__FILE__,__LINE__);
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
        
        if((PlayerDir_t)dir==MIDDLE)
        {
            if(_players[MIDDLE]->get_parter()->get_ting_status()==1&&(_actionToDo&a_HU)){
                _uiManager->_HuEffect(MIDDLE);
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
        
        if(no==1&&_isTuoGuan)
        {
            if(_players[1]->get_parter()->get_ting_status()==1&&(action1&a_HU))
                action1=a_HU;
            else
                action1=a_JUMP;
        }
        
        if(_players[no]->get_parter()->get_role_type()==SINGLE_BOARD_ROBOT)
        {
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
        
        if(_players[no]->get_parter()->get_role_type()==INTERNET_PLAYER)
        {
            LOGGER_WRITE("NETWORK : NetPlayer action here, %s %d",__FILE__,__LINE__);
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
        if(_players[no1]->get_parter()->get_role_type()==SINGLE_BOARD_ROBOT)
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
        
        if(_players[no1]->get_parter()->get_role_type()==INTERNET_PLAYER)
        {
            LOGGER_WRITE("NETWORK : NetPlayer action here, %s %d",__FILE__,__LINE__);
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
            if((no!=1&&no1!=1)||(no==1||no1==1&&_players[1]->get_parter()->get_ting_status()==1))
            {
                _uiManager->_HuEffect(3);
                _uiManager->_DistributeEvent(DOUBLE_HU_WITH_ME,NULL);
            }
            else if((no==1||no1==1)&&_players[1]->get_parter()->get_ting_status()==0)
            {
                _isDoubleHuAsking = true;
                if(no==1)
                {
                    _otherOneForDouble = no1;
                    _actionToDo=action1;
                }
                else
                {
                    _otherOneForDouble = no;
                    _actionToDo=action2;
                }                   
                WaitForMyAction();
                return;
            }
        }
        else if(action1&a_HU||action2&a_HU)//鐐圭偖
        {
            _uiManager->HideClock();
            if((no==1&&(action1&a_HU))||(no1==1&&(action2&a_HU)))
            {
                if(_players[1]->get_parter()->get_ting_status()==1)//&&_actionToDo&a_HU)
                {
                    _uiManager->_HuEffect(1);
                }
                else
                {
                    if(no==1)
                        _actionToDo=action1;
                    else
                        _actionToDo=action2;
                    WaitForMyAction();
                    return;
                }
            }
            else if(no!=1&&(action1&a_HU))
                _uiManager->_HuEffect(no);
            else if(no1!=1&&(action2&a_HU))
                _uiManager->_HuEffect(no1);
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
            DistributeTo((PlayerDir_t)_curPlayer);
        }
    }
}

void RoundManager::DistributeTo(PlayerDir_t dir) {
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

void RoundManager::UpdateCards(PlayerDir_t dir,ARRAY_ACTION action) {
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

