//
//  SymbolTable.cpp
//  sys_soft
//
//  Created by Djordje Bozic on 8/5/16.
//  Copyright © 2016 Djordje Bozic. All rights reserved.
//

#include "SymbolTable.h"
#include "FileWriter.h"
#include <sstream>

SymbolTable::SymbolTable() {
    //postavlja undefined sekciju po defaultu, ostalo dodaje interpretator
    first = last = new Entry(new SymTabEntry(0,"und",0,0,false,0));
    last_section = first;
    
}

SymbolTable::~SymbolTable() {
    Entry* cur = first;
    while (first!=0) {
        cur = first;
        first = first->next;
        delete cur;
    }
}

void SymbolTable:: addEntry(SymTabEntry* entry) {
    Entry* newEntry = new Entry(entry);
    last->next = newEntry;
    last = newEntry;
}

void SymbolTable:: insertEntry(SymTabEntry* entry) {
    Entry* newEntry = new Entry(entry,last_section->next);
    last_section->next = newEntry;
    last_section = newEntry;
    
}

//testiraj jos naredne dve funkcije!!!

SymTabEntry* SymbolTable::findValue(string name) {
    Entry* cur = first;
    int index = 0;
    while (cur) {
        if (cur->entry->name == name) {
            return cur->entry; //vraca poziciju elementa u listi
        }
        index++;
        cur = cur->next;
    }
    return 0; //za slucaj da ne pronadje
}

void SymbolTable::modifyEntry(SymTabEntry* entry, mod_type mod, int value, int section) {
    if(mod == SIZE) { //ako se modifikuje velicina sekcije
        entry->size = value;
    }
    else if (mod == OFFSET) { //ako se modifikuje offset promenjive
        entry->offset = value;
    }
    else if (mod==OFFASECT) { //kada mora i sekcija da se menja
        entry->offset = value;
        entry->section = section;
    }
}

void SymbolTable::manageIndexes(){
    Entry* cur = first;
    int new_index = 0;
    while (cur) {
        Entry* to_chng = first;
        int old_index = cur->entry->index;
        cur->entry->index = new_index;
        while (to_chng) {
            if (to_chng->entry->section == old_index && !to_chng->index_changed){
                to_chng->entry->section = new_index;
                to_chng->index_changed = true;
            }
            to_chng = to_chng->next;
        }
        new_index++;
        cur = cur->next;
    }
}


void SymbolTable::writeToFile(const char* filePath) {
    Entry* cur = first;
    FileWriter::getInstance()->loadFile(filePath);
    
    stringstream header;
    header<<std::left<<std::setfill(' ')<<std::setw(8)<<"INDEX";
    header<<std::left<<std::setfill(' ')<<std::setw(35)<<"NAME";
    header<<std::left<<std::setfill(' ')<<std::setw(8)<<"SECTION";
    header<<std::left<<std::setfill(' ')<<std::setw(20)<<"OFFSET";
    header<<std::left<<std::setfill(' ')<<std::setw(4)<<"G/L";
    header<<std::left<<std::setfill(' ')<<std::setw(20)<<"SIZE";
    header<<"\n";
    
    manageIndexes();
    FileWriter::getInstance()->writeLine(header.str());
     

    while (cur) {
        string line = cur->entry->formatLine();
        FileWriter::getInstance()->writeLine(line);
        cur=cur->next;
    }
    FileWriter::getInstance()->closeFile();
}
