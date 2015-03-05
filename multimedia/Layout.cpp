
#include "Layout.h"

/*************************************
    gold count
*************************************/
Vec2 Layout::_AnchorOfSign(int dir) {
    switch(dir) {
        case 0:   return Vec2(0.5,0.5);
        case 1:   return Vec2(0.5,0.5);
        case 2:   return Vec2(1,0.5);
    }
}

Vec2 Layout::_PositionOfSign(int dir,Size size,Vec2 origin,int xOffset) {
    switch(dir) {
        case 0:   
            return Vec2(origin.x + size.width*271/1218,
                        origin.y + size.height*406/716);
        case 1:   
            return Vec2(origin.x + size.width*559/1218,
                        origin.y + size.height*154/716);
        case 2:   
            return Vec2(origin.x + size.width*996/1218 - xOffset,
                        origin.y + size.height*406/716);
    }
}

Vec2 Layout::_DestOfSign(int dir,Size size,Vec2 origin,int xOffset) {
    switch(dir) {
        case 0:   
            return Vec2(origin.x + size.width*271/1218,
                        origin.y + size.height*480/716);
        case 1:   
            return Vec2(origin.x + size.width*559/1218,
                        origin.y + size.height*228/716);
        case 2:   
            return Vec2(origin.x + size.width*996/1218 - xOffset,
                        origin.y + size.height*480/716);
    }
}

Vec2 Layout::_AnchorOfNumber(int dir) {
    switch(dir) {
        case 0:   return Vec2(0,0.5);
        case 1:   return Vec2(0,0.5);
        case 2:   return Vec2(1,0.5);
    }
}

Vec2 Layout::_PositionOfNumber(int dir,Size size,Vec2 origin) {
    switch(dir) {
        case 0:   
            return Vec2(origin.x + size.width*291/1218,
                        origin.y + size.height*406/716);
        case 1:   
            return Vec2(origin.x + size.width*578/1218,
                        origin.y + size.height*154/716);
        case 2:   
            return Vec2(origin.x + size.width*996/1218,
                        origin.y + size.height*406/716);
    }
}

Vec2 Layout::_DestOfNumber(int dir,Size size,Vec2 origin) {
    switch(dir) {
        case 0:   
            return Vec2(origin.x + size.width*291/1218,
                        origin.y + size.height*480/716);
        case 1:   
            return Vec2(origin.x + size.width*578/1218,
                        origin.y + size.height*228/716);
        case 2:   
            return Vec2(origin.x + size.width*996/1218,
                        origin.y + size.height*480/716);
    }
}

Vec2 Layout::_AnchorOfGold(int dir) {
    switch(dir) {
        case 0:   return Vec2(0.5,0.5);
        case 1:   return Vec2(0.5,0.5);
        case 2:   return Vec2(0.5,0.5);
    }
}

Vec2 Layout::_PositionOfGold(int dir,Size size,Vec2 origin,int xOffset) {
    switch(dir) {
        case 0:   
            return Vec2(origin.x + size.width*221/1218,
                        origin.y + size.height*406/716);
        case 1:   
            return Vec2(origin.x + size.width*509/1218,
                        origin.y + size.height*154/716);
        case 2:   
            return Vec2(origin.x + size.width*936/1218 - xOffset,
                        origin.y + size.height*406/716);
                
    }
}

Vec2 Layout::_DestOfGold(int dir,Size size,Vec2 origin,int xOffset) {
    switch(dir) {
        case 0:   
            return Vec2(origin.x + size.width*221/1218,
                        origin.y + size.height*480/716);
        case 1:   
            return Vec2(origin.x + size.width*509/1218,
                        origin.y + size.height*228/716);
        case 2:   
            return Vec2(origin.x + size.width*936/1218 - xOffset,
                        origin.y + size.height*480/716);
    }
}

/*************************************
    out card
*************************************/
int  NetRaceLayer::_RotateAngleOfOutcard(int dir) {
    switch(dir) {
        case 0:
            return 90;
        case 1:
            return 0;
        case 2:
            return 270;
        default:
            LOGGER_WRITE("%s Error ! invalid player");
    }
}

Vec2 NetRaceLayer::_AnchorOfOutcard(int dir) {
    switch(dir) {
        case 0:
            return Vec2(0,0.5);
        case 2:
            return Vec2(1,0.5);
        default:
            LOGGER_WRITE("%s Error ! invalid player");
    }
}

Vec2 NetRaceLayer::_PositionOfOutcard(int dir,Size size,Vec2 origin) {
    switch(dir) {
        case 0:
            return Vec2(origin.x + size.width*0.18, origin.y + size.height*0.6);
        case 2:
            return Vec2(origin.x + size.width*0.82, origin.y + size.height*0.6);
        default:
            LOGGER_WRITE("%s Error ! invalid player");
    }
}

/*************************************
    action
*************************************/
Vec2 NetRaceLayer::_PostionOfActSign(int dir) {
    switch(dir) {
        case 0:
            return Vec2(origin.x+visibleSize.width*0.206,origin.y+visibleSize.height*0.6);
        case 1:
            return Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2);
        case 2:
            return Vec2(origin.x+visibleSize.width*0.79,origin.y+visibleSize.height*0.6);
        default:
            LOGGER_WRITE("%s Error ! invalid player");
    }
}

Vec2 NetRaceLayer::_OrigPositionOfGangCard(int i,Size size) {
    switch(i+1) {
        case 1: return Vec2(
            origin.x + visibleSize.width*0.5 + size.width*0.98,
            origin.y + visibleSize.height*0.255);
        case 2: return Vec2(
            origin.x + visibleSize.width*0.5 - size.width*1.96,
            origin.y + visibleSize.height*0.255);
        case 3: return Vec2(
            origin.x + visibleSize.width*0.5 - size.width*0.98,
            origin.y + visibleSize.height*0.255);
        case 4: return Vec2(
            origin.x + visibleSize.width*0.5,
            origin.y + visibleSize.height*0.255);
    }
}

Vec2 NetRaceLayer::_MiddlePositionOfGangCard(int i,Size size) {
    switch(i+1) {
        case 1: return Vec2(
                origin.x + visibleSize.width*0.5  +  size.width*0.98  +  150,
                origin.y + visibleSize.height*0.255);
        case 2: return Vec2(
			    origin.x + visibleSize.width*0.5 - size.width*1.96 - 150,
			    origin.y + visibleSize.height*0.255);
        case 3: return Vec2(
			    origin.x + visibleSize.width*0.5 - size.width*0.98 - 150,
			    origin.y + visibleSize.height*0.255);
        case 4: return Vec2(
			    origin.x + visibleSize.width*0.5 - 150,
			    origin.y + visibleSize.height*0.255);
    }
}

Vec2 NetRaceLayer::_DestPositionOfGangCard(int i,Size size) {
    switch(i+1) {
        case 1: return Vec2(
                origin.x + visibleSize.width*0.5  +  size.width*0.98,
                origin.y + visibleSize.height*0.255);
        case 2: return Vec2(
    			origin.x + visibleSize.width*0.5 - size.width*1.96,
    			origin.y + visibleSize.height*0.255);
        case 3: return Vec2(
			    origin.x + visibleSize.width*0.5 - size.width*0.98,
			    origin.y + visibleSize.height*0.255);
        case 4: return Vec2(
			    origin.x + visibleSize.width*0.5,
			    origin.y + visibleSize.height*0.255);
    }
}

Vec2 NetRaceLayer::_DestPositionOfGangCardInHand(int i,Size gangCard) {
    switch(i+1) {
        case 1: return Vec2(
            origin.x+visibleSize.width*0.5 - gangCard.width*1.96,
            origin.y+visibleSize.height*0.26);
        case 2: return Vec2(
            origin.x+visibleSize.width*0.5 - gangCard.width*0.98,
            origin.y+visibleSize.height*0.26);
        case 3: return Vec2(
            origin.x+visibleSize.width*0.5,
            origin.y+visibleSize.height*0.26);
        case 4: return Vec2(
            origin.x+visibleSize.width*0.5 + gangCard.width*0.98,
            origin.y+visibleSize.height*0.255);
    }
}

Vec2 NetRaceLayer::_PositionOfMingGangCard(int i,Size size) {
    switch(i+1) {
        case 1: return Vec2(
            origin.x + visibleSize.width*0.5 + size.width*0.98,
            origin.y + visibleSize.height*0.26);
        case 2: return Vec2(
            origin.x + visibleSize.width*0.5 - size.width*1.96,
            origin.y + visibleSize.height*0.26);
        case 3: return Vec2(
            origin.x + visibleSize.width*0.5 - size.width*0.98,
            origin.y + visibleSize.height*0.26);
        case 4: return Vec2(
            origin.x + visibleSize.width*0.5,
            origin.y + visibleSize.height*0.26);
    }
}

Vec2 NetRaceLayer::_MiddlePositionOfGangCardInHand(int i,Vec2 origPos,Size freeCard) {
    switch(i+1) {
        case 1: return Vec2(origPos.x,origPos.y);
        case 2: return Vec2(origPos.x - freeCard.width*0.4, origPos.y);
        case 3: return Vec2(origPos.x - freeCard.width*0.8, origPos.y);
        case 4: return Vec2(origPos.x,origPos.y);
    }
}

/*************************************
    singleton
*************************************/
Layout *Layout::getInstance() {
    if (_instance==NULL) {
        _instance = new Layout();
    }

    return _instance;
}

void Layout::destroyInstance() {
    delete _instance;
    _instance = NULL;
}


