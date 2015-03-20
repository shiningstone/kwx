
#include <stdlib.h>

#include "RaceType.h"
#include "Raction.h"

#include "NetRole.h"
#include "RoundManager.h"

RoundManager::RoundManager(NetRaceLayer *uiManager) {
    _uiManager = uiManager;

    _lastWin.player = INVALID;

    _river = NULL;
	for(int i=0;i<TOTAL_CARD_NUM;i++) {
		_unDistributedCards[i]=i;
	}
    _distributedNum = 0;

    for(int i=0;i<PLAYER_NUMBER;i++) {
        _players[i] = NULL;
    }
    
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
    if( _lastWin.player==INVALID ) {
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
            ((_lastWin.player==curPlayer && _lastWin.player!=no)||
            (_lastWin.player!=curPlayer && no!=_lastWin.player && no!=curPlayer)))
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


    LOGGER_WRITE("NETWORK: Shuffle should be executed at the server, and it is more reasonable to hide the card sequence to clients");
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

    return 0;
}

int RoundManager::NotifyStart() {
    LOGGER_WRITE("NETWORK %s : send notification to server",__FUNCTION__);
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

int RoundManager::AllowMovement() {
    LOGGER_WRITE("enable reaction to player's movement",__FUNCTION__);
    return 0;
}

bool RoundManager::WaitForDistribute() {
    LOGGER_WRITE("NETWORK : %s",__FUNCTION__);
    return true;
}

bool RoundManager::WaitForAction() {
    LOGGER_WRITE("NETWORK : %s",__FUNCTION__);
    return true;
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

void RoundManager::RecvPeng(Button *curButton) {
    Card        card;
    PlayerDir_t prevPlayer;
    
    if(_isWaitDecision) {
        _isWaitDecision = false;
        _actionToDo = _tempActionToDo;
        _tempActionToDo = a_JUMP;
    
        _continue_gang_times = 0;
        _lastAction=a_PENG;
        
        const int riverLast =_players[_curPlayer]->get_parter()->getOutCardList()->length;
        
        _players[_curPlayer]->get_parter()->getOutCardList()->getCard(card,riverLast);
        _players[_curPlayer]->get_parter()->getOutCardList()->deleteItem();
        
        RecordOutCard(card);
        RecordOutCard(card);
    
        prevPlayer = (PlayerDir_t)_curPlayer;
        _curPlayer=1;
    }

    _uiManager->PengPressed(curButton,prevPlayer,(Card_t)card.kind);
}

void RoundManager::RecvHu(Button *curButton) {
    if(_isWaitDecision) {
        _isWaitDecision = false;
        _actionToDo = _tempActionToDo;
        _tempActionToDo = a_JUMP;
    }

    if(_isQiangGangAsking) {
        _lastActionWithGold = a_QIANG_GANG;
    }

    _uiManager->HuPressed(curButton, _isQiangGangAsking, _isDoubleHuAsking);
}

void RoundManager::RecvGang(Button *curButton) {
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
    
	auto list=_players[1]->get_parter()->get_card_list();
	if( _actionToDo & a_AN_GANG || _actionToDo & a_SHOU_GANG ) {
		_lastActionSource = 1;
        
		if(_actionToDo&a_AN_GANG) {
			_actionToDo=a_AN_GANG;
			_lastAction=a_AN_GANG;
			_lastActionWithGold=a_AN_GANG;
		} else if(_actionToDo&a_SHOU_GANG) {
			_actionToDo=a_SHOU_GANG;
			_lastAction=a_SHOU_GANG;
			_lastActionWithGold=a_SHOU_GANG;
		}
        
        FindGangCards(1,gangCardIdx);
        card =(Card_t)list->data[gangCardIdx[0]].kind;
        
		if( !IsTing(_curPlayer) ) {
			SetEffectCard(card,c_AN_GANG);
		}

        _uiManager->GangPressed(curButton,card,gangCardIdx);
	}
	else if( _actionToDo & a_MING_GANG ) {
		_lastActionSource=1;
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
            
			_curPlayer=1;
		}else {
			GangCard=list->data[list->len-1];
			RecordOutCard(GangCard);
		}

        FindGangCards(1,gangCardIdx,(Card_t)GangCard.kind);

        _uiManager->GangPressed(curButton,(Card_t)GangCard.kind,gangCardIdx,false,prevPlayer);
	}
}

void RoundManager::StartGame() {
    _uiManager = NetRaceLayer::create();
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

