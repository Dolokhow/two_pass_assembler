//
//  Interpreter.h
//  sys_soft
//
//  Created by Djordje Bozic on 8/6/16.
//  Copyright © 2016 Djordje Bozic. All rights reserved.
//

#ifndef Interpreter_h
#define Interpreter_h
#include <iostream>
#include "ProcData.h"
using namespace std;


class Interpreter {
    string line;
    // kreira labele, direktive i instrukcije
    Label* prepareLabel();
    Directive* createDirect();
    Instruction* createInstr();
    
    //utility
    int findLabel();
    string readLabel(int pos_end);
    bool checkDir();
    directive_type checkSubDir();
    int countArguments(int);
    string getName();
    bool isEnd();
    bool findChar(string);
    int getCondition();
    string getArg();
    Argument* handleArgument(string str);
    Argument* handleArgument(string, Instruction* instr);
    
    
    
public:
    ProcData* interpret(string);
    
    
};

#endif /* Interpreter_h */
