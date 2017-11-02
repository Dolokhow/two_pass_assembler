//
//  SymTabEntry.h
//  sys_soft
//
//  Created by Djordje Bozic on 8/5/16.
//  Copyright © 2016 Djordje Bozic. All rights reserved.
//

#ifndef SymTabEntry_h
#define SymTabEntry_h
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

struct SymTabEntry {
    // offset i size u bajtovima
    int index;
    string name;
    int section;
    int offset;
    bool global;
    int size;
    
    SymTabEntry(int ind, string na, int sec, int off, bool glo, int siz) {
        index = ind;
        name = na;
        section = sec;
        offset = off;
        global = glo;
        size = siz;
    }
    string formatLine(){
        stringstream line_stream;
        line_stream<<std::left<<std::setfill(' ')<<std::setw(8)<<std::hex<<std::uppercase<<index;
        line_stream<<std::left<<std::setfill(' ')<<std::setw(35)<<name;
        line_stream<<std::left<<std::setfill(' ')<<std::setw(8)<<std::hex<<std::uppercase<<section;
        line_stream<<std::left<<std::setfill(' ')<<std::setw(20)<<std::hex<<std::uppercase<<offset;
        line_stream<<std::left<<std::setfill(' ')<<std::setw(4)<<((global)?"g":"l");
        line_stream<<std::left<<std::setfill(' ')<<std::setw(20)<<std::hex<<std::uppercase<<size;
        return line_stream.str();
    }
};

#endif /* SymTabEntry_h */
