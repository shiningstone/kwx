
#include "Layout.h"

void Layout::SetScope(cocos2d::Point p,cocos2d::Size s) {
    ORIGIN = p;
    SIZE   = s;
}

/*************************************
    gold count
*************************************/
Vec2 Layout::AnchorOfSign(int dir) {
    switch(dir) {
        case 0:   return Vec2(0.5,0.5);
        case 1:   return Vec2(0.5,0.5);
        case 2:   return Vec2(1,0.5);
    }
}

Vec2 Layout::PositionOfSign(int dir,int xOffset) {
    switch(dir) {
        case 0:   
            return Vec2(ORIGIN.x + SIZE.width*271/1218,
                        ORIGIN.y + SIZE.height*406/716);
        case 1:   
            return Vec2(ORIGIN.x + SIZE.width*559/1218,
                        ORIGIN.y + SIZE.height*154/716);
        case 2:   
            return Vec2(ORIGIN.x + SIZE.width*996/1218 - xOffset,
                        ORIGIN.y + SIZE.height*406/716);
    }
}

Vec2 Layout::DestOfSign(int dir,int xOffset) {
    switch(dir) {
        case 0:   
            return Vec2(ORIGIN.x + SIZE.width*271/1218,
                        ORIGIN.y + SIZE.height*480/716);
        case 1:   
            return Vec2(ORIGIN.x + SIZE.width*559/1218,
                        ORIGIN.y + SIZE.height*228/716);
        case 2:   
            return Vec2(ORIGIN.x + SIZE.width*996/1218 - xOffset,
                        ORIGIN.y + SIZE.height*480/716);
    }
}

Vec2 Layout::AnchorOfNumber(int dir) {
    switch(dir) {
        case 0:   return Vec2(0,0.5);
        case 1:   return Vec2(0,0.5);
        case 2:   return Vec2(1,0.5);
    }
}

Vec2 Layout::PositionOfNumber(int dir) {
    switch(dir) {
        case 0:   
            return Vec2(ORIGIN.x + SIZE.width*291/1218,
                        ORIGIN.y + SIZE.height*406/716);
        case 1:   
            return Vec2(ORIGIN.x + SIZE.width*578/1218,
                        ORIGIN.y + SIZE.height*154/716);
        case 2:   
            return Vec2(ORIGIN.x + SIZE.width*996/1218,
                        ORIGIN.y + SIZE.height*406/716);
    }
}

Vec2 Layout::DestOfNumber(int dir) {
    switch(dir) {
        case 0:   
            return Vec2(ORIGIN.x + SIZE.width*291/1218,
                        ORIGIN.y + SIZE.height*480/716);
        case 1:   
            return Vec2(ORIGIN.x + SIZE.width*578/1218,
                        ORIGIN.y + SIZE.height*228/716);
        case 2:   
            return Vec2(ORIGIN.x + SIZE.width*996/1218,
                        ORIGIN.y + SIZE.height*480/716);
    }
}

Vec2 Layout::AnchorOfGold(int dir) {
    switch(dir) {
        case 0:   return Vec2(0.5,0.5);
        case 1:   return Vec2(0.5,0.5);
        case 2:   return Vec2(0.5,0.5);
    }
}

Vec2 Layout::PositionOfGold(int dir,int xOffset) {
    switch(dir) {
        case 0:   
            return Vec2(ORIGIN.x + SIZE.width*221/1218,
                        ORIGIN.y + SIZE.height*406/716);
        case 1:   
            return Vec2(ORIGIN.x + SIZE.width*509/1218,
                        ORIGIN.y + SIZE.height*154/716);
        case 2:   
            return Vec2(ORIGIN.x + SIZE.width*936/1218 - xOffset,
                        ORIGIN.y + SIZE.height*406/716);
                
    }
}

Vec2 Layout::DestOfGold(int dir,int xOffset) {
    switch(dir) {
        case 0:   
            return Vec2(ORIGIN.x + SIZE.width*221/1218,
                        ORIGIN.y + SIZE.height*480/716);
        case 1:   
            return Vec2(ORIGIN.x + SIZE.width*509/1218,
                        ORIGIN.y + SIZE.height*228/716);
        case 2:   
            return Vec2(ORIGIN.x + SIZE.width*936/1218 - xOffset,
                        ORIGIN.y + SIZE.height*480/716);
    }
}

/*************************************
    out card
*************************************/
int  Layout::RotateAngleOfOutcard(int dir) {
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

Vec2 Layout::AnchorOfOutcard(int dir) {
    switch(dir) {
        case 0:
            return Vec2(0,0.5);
        case 2:
            return Vec2(1,0.5);
        default:
            LOGGER_WRITE("%s Error ! invalid player");
    }
}

Vec2 Layout::PositionOfOutcard(int dir) {
    switch(dir) {
        case 0:
            return Vec2(ORIGIN.x + SIZE.width*0.18, ORIGIN.y + SIZE.height*0.6);
        case 2:
            return Vec2(ORIGIN.x + SIZE.width*0.82, ORIGIN.y + SIZE.height*0.6);
        default:
            LOGGER_WRITE("%s Error ! invalid player");
    }
}

/*************************************
    action
*************************************/
Vec2 Layout::Center() {
    return Vec2(ORIGIN.x+SIZE.width/2, ORIGIN.y+SIZE.height/2);
}

Vec2 Layout::PositionOfMingSign() {
    return Vec2(ORIGIN.x+SIZE.width/2, ORIGIN.y+SIZE.height/2+50);
}

Vec2 Layout::PositionOfActSign(int dir) {
    switch(dir) {
        case 0:
            return Vec2(ORIGIN.x+SIZE.width*0.206,ORIGIN.y+SIZE.height*0.6);
        case 1:
            return Vec2(ORIGIN.x+SIZE.width/2,ORIGIN.y+SIZE.height/2);
        case 2:
            return Vec2(ORIGIN.x+SIZE.width*0.79,ORIGIN.y+SIZE.height*0.6);
        default:
            LOGGER_WRITE("%s Error ! invalid player");
    }
}

Vec2 Layout::OrigPositionOfGangCard(int i,Size cardSize) {
    switch(i+1) {
        case 1: return Vec2(
            ORIGIN.x + SIZE.width*0.5 + cardSize.width*0.98,
            ORIGIN.y + SIZE.height*0.255);
        case 2: return Vec2(
            ORIGIN.x + SIZE.width*0.5 - cardSize.width*1.96,
            ORIGIN.y + SIZE.height*0.255);
        case 3: return Vec2(
            ORIGIN.x + SIZE.width*0.5 - cardSize.width*0.98,
            ORIGIN.y + SIZE.height*0.255);
        case 4: return Vec2(
            ORIGIN.x + SIZE.width*0.5,
            ORIGIN.y + SIZE.height*0.255);
    }
}

Vec2 Layout::MiddlePositionOfGangCard(int i,Size cardSize) {
    switch(i+1) {
        case 1: return Vec2(
                ORIGIN.x + SIZE.width*0.5  +  cardSize.width*0.98  +  150,
                ORIGIN.y + SIZE.height*0.255);
        case 2: return Vec2(
			    ORIGIN.x + SIZE.width*0.5 - cardSize.width*1.96 - 150,
			    ORIGIN.y + SIZE.height*0.255);
        case 3: return Vec2(
			    ORIGIN.x + SIZE.width*0.5 - cardSize.width*0.98 - 150,
			    ORIGIN.y + SIZE.height*0.255);
        case 4: return Vec2(
			    ORIGIN.x + SIZE.width*0.5 - 150,
			    ORIGIN.y + SIZE.height*0.255);
    }
}

Vec2 Layout::DestPositionOfGangCard(int i,Size cardSize) {
    switch(i+1) {
        case 1: return Vec2(
                ORIGIN.x + SIZE.width*0.5  +  cardSize.width*0.98,
                ORIGIN.y + SIZE.height*0.255);
        case 2: return Vec2(
    			ORIGIN.x + SIZE.width*0.5 - cardSize.width*1.96,
    			ORIGIN.y + SIZE.height*0.255);
        case 3: return Vec2(
			    ORIGIN.x + SIZE.width*0.5 - cardSize.width*0.98,
			    ORIGIN.y + SIZE.height*0.255);
        case 4: return Vec2(
			    ORIGIN.x + SIZE.width*0.5,
			    ORIGIN.y + SIZE.height*0.255);
    }
}

Vec2 Layout::DestPositionOfGangCardInHand(int i,Size gangCard) {
    switch(i+1) {
        case 1: return Vec2(
            ORIGIN.x+SIZE.width*0.5 - gangCard.width*1.96,
            ORIGIN.y+SIZE.height*0.26);
        case 2: return Vec2(
            ORIGIN.x+SIZE.width*0.5 - gangCard.width*0.98,
            ORIGIN.y+SIZE.height*0.26);
        case 3: return Vec2(
            ORIGIN.x+SIZE.width*0.5,
            ORIGIN.y+SIZE.height*0.26);
        case 4: return Vec2(
            ORIGIN.x+SIZE.width*0.5 + gangCard.width*0.98,
            ORIGIN.y+SIZE.height*0.255);
    }
}

Vec2 Layout::PositionOfMingGangCard(int i,Size cardSize) {
    switch(i+1) {
        case 1: return Vec2(
            ORIGIN.x + SIZE.width*0.5 + cardSize.width*0.98,
            ORIGIN.y + SIZE.height*0.26);
        case 2: return Vec2(
            ORIGIN.x + SIZE.width*0.5 - cardSize.width*1.96,
            ORIGIN.y + SIZE.height*0.26);
        case 3: return Vec2(
            ORIGIN.x + SIZE.width*0.5 - cardSize.width*0.98,
            ORIGIN.y + SIZE.height*0.26);
        case 4: return Vec2(
            ORIGIN.x + SIZE.width*0.5,
            ORIGIN.y + SIZE.height*0.26);
    }
}

Vec2 Layout::MiddlePositionOfGangCardInHand(int i,Vec2 origPos,Size freeCard) {
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


