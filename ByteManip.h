//
//  ByteManip.h
//  sys_soft
//
//  Created by Djordje Bozic on 8/10/16.
//  Copyright © 2016 Djordje Bozic. All rights reserved.
//

#ifndef ByteManip_h
#define ByteManip_h

class ByteManip{
    static ByteManip* instance;
    ByteManip(){};
    ByteManip(const ByteManip& bm);
    
public:
    static ByteManip* getInstance();
    
    short getFirstByte(short);
    short getSecondByte(short);
    short getFirstByte(int);
    short getSecondByte(int);
    short getThirdByte(int);
    short getFourthByte(int);
    
    int modify(int source, int to_insert, int poss, int len);
};


#endif /* ByteManip_h */
