
#include "NetGameScene.h"
#include "NetMe.h"
#include "NetPlayer.h"
#include "MeRaceRound.h"
#include <stdio.h>
using namespace std;

NetGameScene::NetGameScene():NetRaceLayer()
{
    _logger = LOGGER_REGISTER("NetRaceLayer");
    _roundManager = new RoundManager();
}

NetGameScene::~NetGameScene()
{
    delete _roundManager;
    LOGGER_DEREGISTER(_logger);
}

bool NetGameScene::init()
{
    LOGGER_WRITE("%s",__FUNCTION__);
	if(!NetRaceLayer::init())
	{
		return false;
	}
	init_race_sequence();
	return true;
}

void NetGameScene::distribute_card_event()
{
	DCI scard;
	if(dist_card_no<TOTAL_CARD_NUM)
	{
		scard.card=CARD_KIND(card_seq[dist_card_no++]/4);
		scard.num=dist_card_no;
		_eventDispatcher->dispatchCustomEvent(DISTRIBUTE_DONE_EVENT_TYPE,&scard);
        LOGGER_WRITE("%s : %s",__FUNCTION__,DISTRIBUTE_DONE_EVENT_TYPE);
	}
	else
    {
		_eventDispatcher->dispatchCustomEvent(NOONE_WIN_EVENT_TYPE,NULL);
        LOGGER_WRITE("%s : %s",__FUNCTION__,NOONE_WIN_EVENT_TYPE);
    }   
}

#include "./../utils/UtilBasic.h"
#include "./../protocol/MsgFormats.h"
#include "./../protocol/KwxMsg.h"
#include "./../protocol/EnvVariables.h"
void NetGameScene::race_start_again()
{
    LOGGER_WRITE("%s",__FUNCTION__);

	/**************************/
	/* this is just for test */
    INT8U buf[MSG_MAX_LEN] = {0};
    int   len = 0;
    
    SeatInfo *seat = SeatInfo::getInstance();
    seat->Set(0x00010203,0x04050607,0x08090a0b,1);
    
    KwxMsg aMsg(UP_STREAM);
    aMsg.StartReceiving();
    len = aMsg.SetAction(buf,PENG);
	/**************************/

	auto _waitstartListener = EventListenerCustom::create(WAIT_START_CALLBACK_EVENT_TYPE, [this](EventCustom * event){
        LOGGER_WRITE("got %s",WAIT_START_CALLBACK_EVENT_TYPE);

        _roundManager->Shuffle(card_seq);
        set_cards_sequence(card_seq);
		set_aims_sequence(aim);
		set_winner_no( _roundManager->GetLastWinner() );

		dist_card_no=40;
	});

	_eventDispatcher->addEventListenerWithFixedPriority(_waitstartListener, 3);

	auto _calldistributeListener = EventListenerCustom::create(DISTRIBUTE_CALL_EVENT_TYPE, [this](EventCustom * event){
			distribute_card_event();
		});
	_eventDispatcher->addEventListenerWithFixedPriority(_calldistributeListener, 2);

}

void NetGameScene::init_race_sequence()
{
    LOGGER_WRITE("%s",__FUNCTION__);

	memset(aim,0,sizeof(int)*3);

	for(int i=0;i<TOTAL_CARD_NUM;i++)
	{
		card_seq[i]=i;
	}

	race_role[0]=new NetPlayer();
	race_role[1]=new NetMe();
	race_role[2]=new NetPlayer();

	race_action[0]=new NetRRound();
	race_action[1]=new MeRRound();
	race_action[2]=new NetRRound();

	init_role(race_role,race_action);
    _roundManager->SetPlayers(race_role);

	race_role[0]->get_parter()->set_role_type(INTERNET_PLAYER);
	race_role[1]->get_parter()->set_role_type(SINGLE_BOADR_ME);
	race_role[2]->get_parter()->set_role_type(INTERNET_PLAYER);

	create_race();

	race_start_again();
	
}

