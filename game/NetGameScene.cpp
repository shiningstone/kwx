
#include "NetGameScene.h"
#include <stdio.h>
using namespace std;

NetGameScene::NetGameScene():NetRaceLayer()
{
    _logger = LOGGER_REGISTER("NetRaceLayer");
}

NetGameScene::~NetGameScene()
{
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

void NetGameScene::race_start_again()
{
    LOGGER_WRITE("%s",__FUNCTION__);

	auto _waitstartListener = EventListenerCustom::create(WAIT_START_CALLBACK_EVENT_TYPE, [this](EventCustom * event){

		for(int j=0;j<2;j++)//伪随机数列生成
		{
			for(int i=0;i<TOTAL_CARD_NUM;i++)
			{
				int tmp=card_seq[i];
				int cur=rand()%TOTAL_CARD_NUM;
				card_seq[i]=card_seq[cur];
				card_seq[cur]=tmp;
			}
		}
        
        LOGGER_WRITE("got %s",WAIT_START_CALLBACK_EVENT_TYPE);
        char buf[1024] = {0};
        int usedBytes  = 0;
	    sprintf(buf+usedBytes, "\t");
        usedBytes += 1;
	    for(int i=0; i<TOTAL_CARD_NUM; i++) {
		    sprintf(buf+usedBytes, "%02d ",(card_seq[i])%TOTAL_CARD_KIND);
            usedBytes += 3;

		    if( (i+1)%16==0 ) {
			    sprintf(buf+usedBytes, "\n\t");
                usedBytes += 2;
		    }
	    }
        sprintf(buf+usedBytes, "\n");
        log(buf);

        set_cards_sequence(card_seq);
		set_aims_sequence(aim);
		set_winner_no(get_last_winner_no());

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

    LOGGER_WRITE("NETWORK: Request(last winner) not defined");
	set_winner_no( _roundManager->GetLastWinner() );

	race_role[0]=new NetPlayer();
	race_role[1]=new Me();
	race_role[2]=new NetPlayer();

	race_action[0]=new NetRRound();
	race_action[1]=new NetRRound();
	race_action[2]=new NetRRound();

	init_role(race_role,race_action);

	race_role[0]->get_parter()->set_role_type(INTERNET_PLAYER);
	race_role[1]->get_parter()->set_role_type(SINGLE_BOADR_ME);
	race_role[2]->get_parter()->set_role_type(INTERNET_PLAYER);

	create_race();

	race_start_again();
	
}

