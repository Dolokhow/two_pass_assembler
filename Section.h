//
//  Section.h
//  sys_soft
//
//  Created by Djordje Bozic on 8/9/16.
//  Copyright © 2016 Djordje Bozic. All rights reserved.
//

#ifndef Section_h
#define Section_h
#include <iostream>
#include "FileWriter.h"

#include "ByteManip.h"
using namespace std;
enum var_type {CHR,WRD,LNG,UND};
struct Section{
    
    struct Elem{
        var_type type;
        bool little_endian;
        unsigned char sym;
        short wrd;
        int lng;
        Elem* next;
        
        Elem(unsigned char chr, Elem* next = 0) {
            type = CHR;
            sym = chr;
            wrd = 0;
            lng = 0;
            little_endian = true;
            this->next = next;
        }
        Elem(short wrd, Elem* next = 0) {
            type = WRD;
            sym = '0';
            this->wrd = wrd;
            lng = 0;
            little_endian = true;
            this->next = next;
        }
        Elem(int lng, bool little, Elem* next = 0) {
            type = LNG;
            sym = '0';
            wrd = 0;
            little_endian = little;
            this->lng = lng;
            this->next = next;
        }
    };
    
    string name;
    Elem *first, *last;
    int cnt;
    Section(string name) {
        this->name = name;
        first = last = 0;
        cnt = 0;
    }
    void addWord(unsigned char var, var_type type);
    void addWord(short var, var_type type);
    void addWord(int var, bool little, var_type type);
    void writeToFile(const char* filePath);
};

struct SectionList{
    struct Entry{
        Section* sect;
        Entry* next;
        Entry(Section* s, Entry* n = 0) {
            sect = s;
            next = n;
        }
    };
    Entry *first, *last;
    int cnt;
    
    SectionList() {
        first = last = 0;
        cnt = 0;
    }
    
    void addEntry(Section* s);
    Section* getByName(string name);
    void writeToFile(const char* filePath);
    ~SectionList();
};















#endif /* Section_h */
