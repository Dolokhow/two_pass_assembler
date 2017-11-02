//
//  RelocTabEntry.h
//  sys_soft
//
//  Created by Djordje Bozic on 8/12/16.
//  Copyright © 2016 Djordje Bozic. All rights reserved.
//

#ifndef RelocTabEntry_h
#define RelocTabEntry_h

#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

enum reloc_type{R_386_32, R_386_PC32, R_386_32NEG, R_386_PC32NEG, R_386_PC10, R_386_10, R_386_19, R_386_PC19, R_386_16H, R_386_16L}; // proveri da li jos tipova postoje

struct RelocTabEntry{
    int sym_tab_index; //definise specifican element ili sekciju
    int offset; // offset u okviru sekcije u bajtovima
    int shift; // offset u odnosu na bajt u bitovima
    reloc_type type;
    
    RelocTabEntry(int index, int off, int shf, reloc_type t){
        sym_tab_index = index;
        offset = off;
        shift = shf;
        type = t;
    }
    
    string formatLine(){
        stringstream line_stream;
        line_stream<<std::left<<std::setfill(' ')<<std::setw(18)<<std::hex<<std::uppercase<<sym_tab_index;
        line_stream<<std::left<<std::setfill(' ')<<std::setw(18)<<std::hex<<std::uppercase<<offset;
        line_stream<<std::left<<std::setfill(' ')<<std::setw(18)<<std::hex<<std::uppercase<<shift;
        string typeName = "";
        switch(type){
            case R_386_32:
                typeName = "R_386_32";
                break;
            case R_386_PC32:
                typeName = "R_386_PC32";
                break;
            case R_386_32NEG:
                typeName = "R_386_32NEG";
                break;
            case R_386_PC32NEG:
                typeName = "R_386_PC32NEG";
                break;
            case R_386_10:
                typeName = "R_386_10";
                break;
            case R_386_PC10:
                typeName = "R_386_PC10";
                break;
            case R_386_19:
                typeName = "R_386_19";
                break;
            case R_386_PC19:
                typeName = "R_386_PC19";
                break;
            case R_386_16H:
                typeName = "R_386_16H";
                break;
            case R_386_16L:
                typeName = "R_386_16L";
                break;
            }
        line_stream<<std::left<<std::setfill(' ')<<std::setw(18) << typeName;
        return line_stream.str();
    }
};




#endif /* RelocTabEntry_h */

