//
//  SymbolTable.h
//  sys_soft
//
//  Created by Djordje Bozic on 8/5/16.
//  Copyright © 2016 Djordje Bozic. All rights reserved.
//

#ifndef SymbolTable_h
#define SymbolTable_h

#include <iostream>
#include "SymTabEntry.h"
using namespace std;
enum mod_type {SIZE,OFFSET,OFFASECT};
class SymbolTable {
    struct Entry {
        SymTabEntry* entry;
        Entry* next;
        bool index_changed;
        
        Entry(SymTabEntry* e, Entry* n=0) {
            entry = e;
            next = n;
            index_changed = false;
        }
    };
    
    Entry *first, *last, *last_section;
    void manageIndexes();
public:
    SymbolTable();
    void writeToFile(const char* filaPath);
    void addEntry(SymTabEntry*);
    void insertEntry(SymTabEntry*);
    SymTabEntry* findValue(string);
    void modifyEntry(SymTabEntry*,mod_type,int, int);
    
    ~SymbolTable();
    
};


#endif /* SymbolTable_h */
