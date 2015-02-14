
#include <string.h>

#include "MsgFormats.h"
#include "KwxMsg.h"

KwxMsg::KwxMsg(int dir) {
    if (dir==UP_STREAM) {
        _header = new UpHeader();
    } else {
        _header = new DnHeader();
    }

    _body = new MsgBody();
}

KwxMsg::~KwxMsg() {
    delete _header;
    delete _body;
}

int KwxMsg::Serialize(INT8U *outMsg) {
    int len = 0;

    len += _header->Serialize(outMsg);
    len += _body->Serialize(outMsg+len);

    return len;
}

int KwxMsg::Deserialize(const INT8U *inMsg) {
    int len = 0;

    if (memcmp(inMsg,Header::PCHC,3)) {
        return KWX_INVALID_PCHC;
    }

    len += _header->Deserialize(inMsg);
    len += _body->Deserialize(inMsg+len);

    return len;
}

int KwxMsg::SetRequestCode(RequestId_t code) {
    return 0;
}

int KwxMsg::SetRoomPath(RoomPath_t code) {
    return 0;
}

int KwxMsg::SetRoomId(RoomId_t code) {
    return 0;
}

int KwxMsg::SetTableId(TableId_t code) {
    return 0;
}

int KwxMsg::SetSeatId(INT32U code) {
    return 0;
}

int KwxMsg::SetAction(ActionId_t code) {
    return 0;
}

