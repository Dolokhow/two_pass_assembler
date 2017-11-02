//
//  RelocTable.h
//  sys_soft
//
//  Created by Djordje Bozic on 8/12/16.
//  Copyright © 2016 Djordje Bozic. All rights reserved.
//

#ifndef RelocTable_h
#define RelocTable_h

#include "RelocTabEntry.h"

class RelocTable{
    struct Entry {
        RelocTabEntry* entry;
        Entry* next;
        
        Entry(RelocTabEntry* r, Entry* n = 0) {
            entry = r;
            next = n;
        }
    };
    string name;
    int sym_index;
    Entry *first, *last;
    int cnt;
public:
    
    RelocTable(string n, int sym){
        name = n;
        sym_index = sym;
        first = last = 0;
        cnt = 0;
    }
    void addEntry(RelocTabEntry*);
    void writeToFile(const char* filePath);
    string getName() {return name;}
    int getIndex(){return sym_index;}
    ~RelocTable();
};

class RelocTableList{
    struct Elem {
        RelocTable* reloc;
        Elem* next;
        
        Elem(RelocTable* r, Elem* n = 0) {
            reloc = r;
            next = n;
        }
    };
    
    Elem *first, *last;
    int cnt;
    
public:
    RelocTableList(){
        first = last = 0;
        cnt = 0;
    }
    void addTable(RelocTable*);
    RelocTable* getByName(string name);
    void writeToFile(const char*);
    ~RelocTableList();
};


#endif /* RelocTable_h */
