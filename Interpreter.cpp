//
//  Interpreter.cpp
//  sys_soft
//
//  Created by Djordje Bozic on 8/6/16.
//  Copyright © 2016 Djordje Bozic. All rights reserved.
//

#include "Interpreter.h"
#include <string>
#include "stdlib.h"

ProcData* Interpreter:: interpret(string line) {
    this->line = line;
    Label* label = prepareLabel();
    if (this->line.length() > 0 && this->line[0]!='\r') {
        if (isEnd()) return new ProcData();
        else if (checkDir()) { //ako je direktiva
            Directive* dir = createDirect();
            return new ProcData(DIR,dir,label,0);
        }
        else {
            Instruction* instr = createInstr();
            return new ProcData(INS,0,label,instr);
        }
    }
    else return new ProcData(label); // ako je samo labela u pitanju!
}


int Interpreter::findLabel() { //vraca poziciju kraja labele ako je ima u redu, u suprotnom vraca neg!
    int pos = (int)line.find(':');
    if (pos!=string::npos) return pos;
    return -1;
}

string Interpreter::readLabel(int pos_end) { //vrati ime labele sa pocetka reda
    string labName="";
    int iterator = 0;
    while (iterator < pos_end) {
        labName+=line[iterator];
        iterator++;
    }
    line.erase(line.begin(), line.begin()+pos_end+1); // skloni labelu sa dvotackom iz instrukcije (u lokalnom zapisu)
    if(line.length()!=0 && line[0]==' ') line.erase(line.begin(),line.begin()+1); // skloni eventualni space posle dvotacke
    return labName;
}

Label* Interpreter::prepareLabel() {
    int lab = findLabel();
    string label;
    if (lab != -1) {
        label = readLabel(lab);
        return new Label(label);
    }
    return 0;
}

bool Interpreter:: checkDir() {
    if (line[0]=='.') return true;
    else return false;
}

bool Interpreter:: isEnd() {
    if (!(line.find(".end")==string::npos)) return true;
    else return false;
}

directive_type Interpreter:: checkSubDir() {
    switch (line[1]) {
        case 'p':
            return PUB;
        case 'e':
            return EXT;
        case 'c':
            return CHAR;
        case 'w':
            return WORD;
        case 'l':
            return LONG;
        case 'a':
            return ALIGN;
        case 's':
            return SKIP;
        case 't':
            return TEXT;
        case 'd':
            return DATA;
        case 'b':
            return BSS;
    }
    return UNDEF;
}

int Interpreter::countArguments(int arg_start) {
    int i = arg_start;
    int count = 0;
    
    if (line[arg_start]!='\r' && line.length()>arg_start) count = 1;
    while (i < line.length()) {
        if (line[i]==',') count++;
        i++;
    }
    return count;
}


//ovo je menjano, pazi!
bool Interpreter::findChar(string arg){
    int pos = 0;
    while (arg[pos]!=',' && pos < arg.length() && arg[pos]!='\r') {
        if (std::isalpha(arg[pos])||std::ispunct(arg[pos])) return true;
        pos++;
    }
    return false;
}

Argument* Interpreter:: handleArgument(string arg) {
    if (!findChar(arg))
        return new Argument(INT,atoi(arg.c_str()),"");
    else return new Argument(STR,-1,arg);
}

Directive* Interpreter::createDirect() {
    directive_type type = checkSubDir();
    string direct = getName();
    if (line.length() > 0) {
        int arg_cnt = countArguments(0);
        Directive* dir = new Directive(type,arg_cnt,direct);
        for (int i = 0; i < arg_cnt; i++) dir->args->add(handleArgument(getArg()));
        return dir;
    }
    else
        return new Directive(type,0,direct); // u slucaju da se dogodio word, long ili sekcija
}

int Interpreter:: getCondition() {
    if (!(line.find('_')==string::npos)) { //dohvata uslov
        string condition = "";
        int i = 0;
        for (i = 0; i < 2; i++) condition+=line[i];
        line.erase(line.begin(), line.begin()+i+1);
        if (condition == "eq") return 0;
        else if (condition == "ne") return 1;
        else if (condition == "gt") return 2;
        else if (condition == "ge") return 3;
        else if (condition == "lt") return 4;
        else if (condition == "le") return 5;
        else if (condition == "al") return 7;
        else return 6;
    } else return 7;
}

string Interpreter:: getName() {
    string instr = "";
    int i = 0;
    for (i = 0; i < line.length() && line[i]!=' ' && line[i]!='\r'; i++) instr+=line[i];
    if (line[i] == '\r') line="";
    else if (i!=line.length()) {
        line.erase(line.begin(), line.begin()+i+1);
    }
    else line = "";
    return instr;
}

string Interpreter:: getArg() {
    int i = 0;
    string str = "";
    for (i = 0; i < line.length() && line[i]!=',' && line[i]!='\r'; i++) str+=line[i];
    if (i!=line.length() && line[i]!='\r') {
        if (line[i+1] == ' ') line.erase(line.begin(), line.begin()+i+2);
        else line.erase(line.begin(), line.begin()+i+1);
    }
    return str;
}

Argument* Interpreter::handleArgument(string arg, Instruction* instr) {
    if (std::isalpha(arg[0])) {
        int value = -1;
        if (arg == "r0") value = 0;
        else if (arg == "r1") value = 1;
        else if (arg == "r2") value = 2;
        else if (arg == "r3") value = 3;
        else if (arg == "r4") value = 4;
        else if (arg == "r5") value = 5;
        else if (arg == "r6") value = 6;
        else if (arg == "r7") value = 7;
        else if (arg == "r8") value = 8;
        else if (arg == "r9") value = 9;
        else if (arg == "r10") value = 10;
        else if (arg == "r11") value = 11;
        else if (arg == "r12") value = 12;
        else if (arg == "r13") value = 13;
        else if (arg == "r14") value = 14;
        else if (arg == "r15") value = 15;
        else if (arg == "r16") value = 16;
        else if (arg == "r17") value = 17;
        else if (arg == "r18") value = 18;
        else if (arg == "r19") value = 19;
        if (value == -1) {
            if (arg[0] == '-') {
                instr->immediate = true;
                arg.erase(arg.begin(), arg.begin()+1);
                return new Argument(INT, 0-atoi(arg.c_str()),"");
            } // mozda je negativan broj
            
            else return new Argument(STR,-1,arg); // vraca labelu
        }
        else return new Argument(INT,value,""); // vraca registar
        
    }
    else {
        instr->immediate = true; // fleg koji nam sluzi da razlikujemo broj registra od broja!
        return new Argument(INT,atoi(arg.c_str()),"");
    }
}


Instruction* Interpreter:: createInstr() {
    int cond = getCondition();
    string instr = getName();
    Instruction* instruction = new Instruction(instr, cond);
    int i = 0;
    int end = countArguments(0);
    for (i = 0; i < end; i++) instruction->args->add(handleArgument(getArg(), instruction));
    if (i == 1) instruction->type = ONE_OP;
    else if (i == 2) instruction->type = TWO_OP;
    else instruction->type = THREE_OP;
    return instruction;
}

























