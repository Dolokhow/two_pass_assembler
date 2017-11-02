//
//  RelocTable.cpp
//  sys_soft
//
//  Created by Djordje Bozic on 8/12/16.
//  Copyright © 2016 Djordje Bozic. All rights reserved.
//

#include "RelocTable.h"
#include "FileWriter.h"
#include <sstream>

void RelocTable:: addEntry(RelocTabEntry* e) {
    Entry* newEntry = new Entry(e);
    if(cnt) {
        last->next = newEntry;
        last = newEntry;
    } else {
        first = newEntry;
        last = newEntry;
    }
    cnt++;
}

void RelocTable:: writeToFile(const char* filePath) {
    FileWriter::getInstance()->loadFile(filePath);
    stringstream header;
    
    header<<"\n"<<name<<"\n";
    header<<std::left<<std::setfill(' ')<<std::setw(18)<<"SYM_TAB_INDEX";
    header<<std::left<<std::setfill(' ')<<std::setw(18)<<"OFFSET";
    header<<std::left<<std::setfill(' ')<<std::setw(18)<<"SHIFT";
    header<<std::left<<std::setfill(' ')<<std::setw(18)<<"RELOC_TYPE";
    header<<"\n";
    
    FileWriter::getInstance()->writeLine(header.str());
    Entry* cur = first;
    while (cur!=0) {
        string line = cur->entry->formatLine();
        FileWriter::getInstance()->writeLine(line);
        cur=cur->next;
    }
    FileWriter::getInstance()->closeFile();
}

RelocTable::~RelocTable(){
    Entry* cur = first;
    while (first!=0) {
        cur = first;
        first = first->next;
        delete cur;
    }
}

void RelocTableList::addTable(RelocTable* t) {
    Elem* newElem = new Elem(t);
    if(cnt) {
        last->next = newElem;
        last = newElem;
    } else {
        first = newElem;
        last = newElem;
    }
    cnt++;
}

void RelocTableList:: writeToFile(const char* filePath) {
    Elem* cur = first;
    while (cur!=0) {
        cur->reloc->writeToFile(filePath);
        cur = cur->next;
    }
}

RelocTable* RelocTableList::getByName(string name) {
    Elem* cur = first;
    while (cur!=0) {
        if (cur->reloc->getName() == name) return cur->reloc;
        cur=cur->next;
    }
    return 0;
}

RelocTableList::~RelocTableList() {
    Elem* cur = first;
    while (first!=0) {
        cur = first;
        first = first->next;
        delete cur;
    }
}


