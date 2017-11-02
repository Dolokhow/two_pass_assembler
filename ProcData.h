//
//  ProcData.h
//  sys_soft
//
//  Created by Djordje Bozic on 8/6/16.
//  Copyright © 2016 Djordje Bozic. All rights reserved.
//

#ifndef ProcData_h
#define ProcData_h
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

enum command_type {DIR,LAB,INS,END};
enum argument_type {INT,STR};

// ID: 1
enum directive_type {UNDEF,EXT,PUB,CHAR,WORD,LONG,ALIGN,SKIP,TEXT,DATA,BSS};

// ID: 2 nema tipova i podtipova

// ID: 3
enum instruction_type{ONE_OP, TWO_OP, THREE_OP, NONE};



struct Argument {
    argument_type type;
    int value;
    std::string name;
    Argument(argument_type t, int val, string nam) {
        type = t;
        value = val;
        name = nam;
    }
    
    unsigned char toChar() {
        if (type == INT) {
            stringstream ss;
            ss<<value;
            string s = ss.str();
            return s[0];
            
        }
        else if (type == STR) {
            if (name.length() == 4) return name[2];
            else if (name.length() == 3) return name[1];
            else if (name.length() == 1) return name[0];
            else return 'E';
        }
        else {
            return 'E';
        }
    }
};

struct ArgumentList {
    struct Elem{
        Argument* arg;
        Elem* next;
        Elem(Argument* a, Elem* n=0) {
            arg = a;
            next = n;
        }
    };
    Elem* first, *last;
    int cnt;
    ArgumentList(){first = 0; last = 0; cnt = 0;}
    void add(Argument* arg);
    Argument* getElem(int index);
    Argument* getLast();
    ~ArgumentList();
    
};


// ID: 1
struct Directive {
    directive_type type;
    string dir_name;
    int num_args;
    ArgumentList* args;
    Directive(directive_type t, int ar, string name) {
        type = t;
        num_args = ar;
        dir_name = name;
        args = new ArgumentList();
    }
    ~Directive() {
        delete args;
    }
};

// ID: 2
struct Label {
    std::string name;
    Label(std::string name) {
        this->name = name;
    }
};

//ID: 3
struct Instruction {
    instruction_type type;
    int cond;
    bool immediate;
    string instruction;
    ArgumentList* args;
    
    Instruction(string instr, int con) {
        type = NONE;
        args = new ArgumentList();
        instruction = instr;
        cond = con;
        immediate = false;
    }
};


struct ProcData {
    command_type c_type;
    bool is_end; // samo ako se radi o .end direktivi
    
    Directive* directive;
    Label* label;
    Instruction* instruction;
    ProcData() {is_end = true; directive = 0; label = 0; instruction = 0; c_type = END;}
    ProcData(Label* lab) {
        c_type = LAB;
        is_end = false;
        label = lab;
        directive = 0;
        instruction = 0;
    }
    ProcData(command_type c, Directive* dir,Label* lab, Instruction* instr) {
        c_type = c;
        directive = dir;
        label = lab;
        instruction = instr;
        is_end = false;
    }
    ~ProcData(){
        delete directive;
        delete label;
        delete instruction;
    }
};

// ovde ce sigurno biti potrebno jos funkcija-zavrsi kada zavrsis iterator!
struct ProcDataList{
    struct Entry {
        ProcData* proc;
        Entry* next;
        Entry(ProcData* p, Entry* n = 0){
            proc = p;
            next = n;
        }
    };
    Entry *first, *last;
    int cnt;
    ProcDataList() {
        first = last = 0;
        cnt = 0;
    }
    void addEntry(ProcData* e){
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
    
    ~ProcDataList() {
        Entry* cur = first;
        while (first!=0) {
            cur = first;
            first = first->next;
            delete cur;
        }
    }
};















#endif /* ProcData_h */
