//
//  ByteManip.cpp
//  sys_soft
//
//  Created by Djordje Bozic on 8/10/16.
//  Copyright © 2016 Djordje Bozic. All rights reserved.
//

#include <iostream>
#include "ByteManip.h"

ByteManip* ByteManip::instance = 0;

ByteManip* ByteManip::getInstance() {
    if(instance == 0) {
        instance = new ByteManip();
    }
    return instance;
}


short ByteManip::getFirstByte(short var){
    return (var>>8) & 0x00ff;
}

short ByteManip::getSecondByte(short var){
    return var&0x00ff;
}

short ByteManip::getFirstByte(int var){
    return (var>>24)& 0x000000ff;
}

short ByteManip::getSecondByte(int var){
    return (var>>16) & 0x000000ff;
}

short ByteManip::getThirdByte(int var){
    return (var>>8) & 0x000000ff;
 }

short ByteManip::getFourthByte(int var){
    return var&0x000000ff;
}

int ByteManip:: modify(int source, int to_insert, int poss, int len) {
    int mod = source;
    // secenje inserta na tacan broj bita
    int mask = 1;
    for (int i = 0; i < len; i++) mask*=2;
    mask-=1; // 2^n -1
    int masked = to_insert&mask;
    masked<<=(32-poss-len); // mora da figurise i poss i len!
    int ret = mod|masked;
    return ret;
}

