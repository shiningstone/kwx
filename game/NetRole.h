#ifndef __NET_ROLE_H__
#define __NET_ROLE_H__

#include "./../OutCardList.h"
#include "./../RaceType.h"
#include "./../NetRaceRound.h"

#include "./../utils/BasicType.h"
#include "./../utils/LogManager.h"

class NetRole {
public:
	NetRole();
    NetRole(int id);

	virtual void set_robot_hu_target(ROBOT_TARGET par_target);
	virtual ROBOT_TARGET get_robot_hu_target();

	virtual unsigned char init(int card_array[],int len,int aim);
	virtual NetRRound* get_parter();
	virtual void set_parter(NetRRound* p_parter);
    
    bool          _isReady;

    /**************************************************
            user's profile
    **************************************************/
    UserProfile_t _profile;    
    Sex_t GetSex();
    int   UpdateProperty(int change);

    void Set(const UserProfile_t *profile);
protected:
    Logger    *_logger;
	NetRRound *parter;
};

#endif
