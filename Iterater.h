//
//  Iterater.h
//  sys_soft
//
//  Created by Djordje Bozic on 8/6/16.
//  Copyright © 2016 Djordje Bozic. All rights reserved.
//

#ifndef Iterater_h
#define Iterater_h

#include "Interpreter.h"
#include "SymTabEntry.h"
#include "SymbolTable.h"
#include "Section.h"
#include "RelocTable.h"
#include <iostream>
using namespace std;

class Iterater {
    Interpreter* interpreter;
    ProcDataList* procData;
    SymbolTable* symTable;
    SectionList* sections;
    RelocTableList* relocs;
    string sourceFile,destinationFile;
    
    SymTabEntry* curSection;
    int loc_cnt;
    int index;
    
    SymTabEntry* findEntry(string name);
    void addEntry(SymTabEntry*);
    void insertEntry(SymTabEntry*);
    void modifyEntry(SymTabEntry*,mod_type,int,int);
    
    //prvi prolaz
    void directiveHandlerFirstPass(ProcData*);
    void instructionHandlerFirstPass(ProcData*);
    void labelhandler(ProcData*);
    
    //drugi prolaz
    void directiveHandlerSecondPass(ProcData*);
    void instructionHandlerSecondPass(ProcData*);
    int handleFirstByte(int, int, int);
    void addToSection(int);
    void interruptHandler(ProcData*);
    void arithmeticHandler(ProcData*);
    void logicalHandler(ProcData*);
    void ioHandler(ProcData*);
    void movshHandler(ProcData*);
    void ldstHandler(ProcData*);
    void callHandler(ProcData*);
    void ldcHandler(ProcData*);
    bool checkForExpr(Argument*);
    void manageExpr(Argument*);
    
    
    
    
    
public:
    Iterater(string src, string dst) {
        interpreter = new Interpreter();
        procData = new ProcDataList();
        symTable = new SymbolTable();
        sections = new SectionList();
        relocs = new RelocTableList();
        sourceFile = src;
        destinationFile = dst;
        loc_cnt = 0;
        index = 1; //zato sto je undef na 0
    }
    void firstPass();
    void secondPass();
    
    ~Iterater() {
        delete interpreter;
        delete procData;
        delete symTable;
        delete sections;
    }
    
};


#endif /* Iterater_h */
