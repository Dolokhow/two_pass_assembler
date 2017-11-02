//
//  Section.cpp
//  sys_soft
//
//  Created by Djordje Bozic on 8/10/16.
//  Copyright © 2016 Djordje Bozic. All rights reserved.
//

#include "Section.h"


void Section::addWord(unsigned char var, var_type type){
    Elem* newElem = new Elem(var);
    if(cnt) {
        last->next = newElem;
        last = newElem;
    } else {
        first = newElem;
        last = newElem;
    }
    cnt++;
}
void Section::addWord(short var, var_type type){
    Elem* newElem = new Elem(var);
    if(cnt) {
        last->next = newElem;
        last = newElem;
    } else {
        first = newElem;
        last = newElem;
    }
    cnt++;
    
}
void Section::addWord(int var, bool little, var_type type){
    Elem* newElem = new Elem(var, little);
    if(cnt) {
        last->next = newElem;
        last = newElem;
    } else {
        first = newElem;
        last = newElem;
    }
    cnt++;
}

void Section::writeToFile(const char* filePath) {
    FileWriter::getInstance()->loadFile(filePath);
    FileWriter::getInstance()->writeLine('\n'+name);
    
    Elem* cur = first;
    int i = 0;
    while (cur!=0) {
        if (cur->type == CHR) {
            FileWriter::getInstance()->write(cur->sym);
            i++;
            if (i%12==0) FileWriter::getInstance()->newRow();
        }
        else if (cur->type == WRD) {
            unsigned short var1 = ByteManip::getInstance()->getSecondByte(cur->wrd);
            unsigned short var2 = ByteManip::getInstance()->getFirstByte(cur->wrd);
            FileWriter::getInstance()->writeByte(var1);
            i++;
            if (i%12==0) FileWriter::getInstance()->newRow();
            FileWriter::getInstance()->writeByte(var2);
            i++;
            if (i%12==0) FileWriter::getInstance()->newRow();
            }
        else if (cur->type == LNG) {
            unsigned short var1 = ByteManip::getInstance()->getFourthByte(cur->lng);
            unsigned short var2 = ByteManip::getInstance()->getThirdByte(cur->lng);
            unsigned short var3 = ByteManip::getInstance()->getSecondByte(cur->lng);
            unsigned short var4 = ByteManip::getInstance()->getFirstByte(cur->lng);
            
            (cur->little_endian)?FileWriter::getInstance()->writeByte(var1):FileWriter::getInstance()->writeByte(var4);
            i++;
            if (i%12==0) FileWriter::getInstance()->newRow();
            (cur->little_endian)?FileWriter::getInstance()->writeByte(var2):FileWriter::getInstance()->writeByte(var3);
            i++;
            if (i%12==0) FileWriter::getInstance()->newRow();
            (cur->little_endian)?FileWriter::getInstance()->writeByte(var3):FileWriter::getInstance()->writeByte(var2);
            i++;
            if (i%12==0) FileWriter::getInstance()->newRow();
            (cur->little_endian)?FileWriter::getInstance()->writeByte(var4):FileWriter::getInstance()->writeByte(var1);
            i++;
            if (i%12==0) FileWriter::getInstance()->newRow();
        
        }
        else {
            FileWriter::getInstance()->write((unsigned char)'E');
            i++;
            if (i%12==0) FileWriter::getInstance()->newRow();
        }
        cur = cur->next;
    }
    FileWriter::getInstance()->newRow();
    FileWriter::getInstance()->closeFile();
}


void SectionList::addEntry(Section* s) {
    Entry* newElem = new Entry(s);
    if(cnt) {
        last->next = newElem;
        last = newElem;
    } else {
        first = newElem;
        last = newElem;
    }
    cnt++;
}

Section* SectionList::getByName(string name) {
    Entry* cur = first;
    while (cur!=0) {
        if (cur->sect->name == name)
            return cur->sect;
        cur = cur->next;
    }
    return 0;
}

void SectionList::writeToFile(const char* filePath) {
    Entry* cur = first;
    while (cur!=0) {
        cur->sect->writeToFile(filePath);
        cur = cur->next;
    }
}

SectionList::~SectionList() {
    Entry* cur = first;
    while (first!=0) {
        cur = first;
        first = first->next;
        delete cur;
    }
}














