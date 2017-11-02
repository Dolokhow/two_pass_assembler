//
//  Iterater.cpp
//  sys_soft
//
//  Created by Djordje Bozic on 8/6/16.
//  Copyright © 2016 Djordje Bozic. All rights reserved.
//

#include "Iterater.h"
#include "FileReader.h"
#include <stdlib.h>

void Iterater:: addEntry(SymTabEntry* entry) {
    symTable->addEntry(entry);
    index++;
}

void Iterater:: insertEntry(SymTabEntry* entry) {
    symTable->insertEntry(entry);
    index++;
}

SymTabEntry* Iterater::findEntry(string name) {
    return symTable->findValue(name);
    
}

void Iterater::modifyEntry(SymTabEntry* loc, mod_type mod, int val, int sec) {
    symTable->modifyEntry(loc, mod, val, sec);
}

void Iterater:: firstPass() {
    FileReader::getInstance()->loadFile(sourceFile.c_str());
    string line="";
    curSection = symTable->findValue("und");
    while (line!="ERR") {
        line = FileReader::getInstance()->readLine();
        ProcData* proc = interpreter->interpret(line); //dohvatim pripremljene podatke
        procData->addEntry(proc); //keshiram ih
        if (proc->is_end == true) { //treba da se regulise velicina prethodne sekcije
            modifyEntry(curSection, SIZE, loc_cnt, 0);
            break;
        }
        // modifikacija tabele simbola
        if (proc->c_type == DIR) { //ako je u pitanju direktiva - ne iskljucuje postojanje labele na pocetku!
            directiveHandlerFirstPass(proc);
            
        }
        else if (proc->c_type == LAB) {
            labelhandler(proc);
            
        }
        else {
            instructionHandlerFirstPass(proc);
        }
    }
    symTable->writeToFile(destinationFile.c_str());
    FileReader::getInstance()->closeFile();
}

void Iterater::directiveHandlerFirstPass(ProcData* proc) {
    labelhandler(proc); //ako ima labele odradi posao, ako nema, ne radi nista
    
    if (proc->directive->type == TEXT || proc->directive->type == BSS || proc->directive->type == DATA) {
        
        modifyEntry(curSection, SIZE, loc_cnt, 0); // prethodnoj modifikujemo velicinu
        loc_cnt = 0;
        SymTabEntry* newEntry = new SymTabEntry(index,proc->directive->dir_name,index,loc_cnt,false,0);
        curSection = newEntry;
        insertEntry(newEntry);
        
        //pravljenje sekcije - ne popunjava se jos
        //pravljenje relokacione tabele za sekciju - ne popunjava se jos
        Section* newSection = new Section(proc->directive->dir_name);
        sections->addEntry(newSection);
        RelocTable* newReloc = new RelocTable(proc->directive->dir_name, curSection->index);
        relocs->addTable(newReloc);
    }
    else if (proc->directive->type == PUB || proc->directive->type == EXT){
        int i = 0;
        while (i < proc->directive->num_args) {
            string value = proc->directive->args->getElem(i)->name;
            SymTabEntry* newEntry = new SymTabEntry(index,value,0,0,true,0);
            addEntry(newEntry);
            i++;
        }
    }
    else if (proc->directive->type == CHAR){
        if (proc->directive->num_args == 0) loc_cnt++;
        else loc_cnt+=proc->directive->num_args;
    }
    else if (proc->directive->type == WORD){
        if (proc->directive->num_args == 0) loc_cnt+=2;
        else {
            for (int i = 0; i < proc->directive->num_args; i++) {
                if (proc->directive->args->getElem(i)->type == STR) loc_cnt+=4;
                else loc_cnt+=2;
            }
        }
    }
    else if (proc->directive->type == LONG){
        if (proc->directive->num_args == 0) loc_cnt+=4;
        else loc_cnt+=4*proc->directive->num_args;
    }
    else if (proc->directive->type == ALIGN){
        if (proc->directive->num_args == 3) {
            int num_pad = 0;
            while ((loc_cnt+num_pad)% proc->directive->args->getElem(0)->value != 0)
                num_pad++;
            if (num_pad < proc->directive->args->getElem(2)->value) loc_cnt+=num_pad;
        }
        else {
            while (loc_cnt%proc->directive->args->getElem(0)->value != 0) loc_cnt++;
        }
    }
    else if (proc->directive->type == SKIP){
        int value = proc->directive->args->getElem(0)->value;
        loc_cnt+=value;
    }
}

void Iterater::instructionHandlerFirstPass(ProcData* proc) {
    labelhandler(proc);
    loc_cnt+=4;
    if (proc->instruction->instruction == "ldc") loc_cnt+=4;
}

void Iterater::labelhandler(ProcData* proc) {
    if (proc->label!=0) {
        SymTabEntry* entry = findEntry(proc->label->name);
        if (entry) modifyEntry(entry, OFFASECT, loc_cnt-curSection->offset,curSection->section);
        else {
            SymTabEntry* newEntry = new SymTabEntry(index,proc->label->name,curSection->index,loc_cnt,false,0);
            addEntry(newEntry);
        }
    }
}


void Iterater::secondPass(){
    ProcDataList::Entry* iterator = procData->first;
    curSection = 0;
    loc_cnt = 0;
    while (!iterator->proc->is_end) {
        if (iterator->proc->c_type == DIR) {
            directiveHandlerSecondPass(iterator->proc);
        
        }
        else if (iterator->proc->c_type == INS) {
            instructionHandlerSecondPass(iterator->proc);
        }
        iterator = iterator->next;
    }
    sections->writeToFile(destinationFile.c_str());
    relocs->writeToFile(destinationFile.c_str());
    }

void Iterater:: directiveHandlerSecondPass(ProcData* proc) {
    if (proc->directive->type == TEXT || proc->directive->type == DATA || proc->directive->type == BSS) {
        curSection = symTable->findValue(proc->directive->dir_name); //OVO JE KLJUCNO DA RADI!
        loc_cnt = 0;
        
    }
    else if (proc->directive->type == CHAR) {
        Section* sect = sections->getByName(curSection->name);
        if (proc->directive->num_args == 0) {
            sect->addWord((unsigned char)NULL, CHR);
            loc_cnt++;
        }
        else {
            int i = 0;
            while (i < proc->directive->num_args) {
                sect->addWord((unsigned char)(proc->directive->args->getElem(i)->toChar()), CHR);
                i++;
            }
            loc_cnt+=proc->directive->num_args;
        }
    }
    else if (proc->directive->type == WORD) {
        Section* sect = sections->getByName(curSection->name);
        if (proc->directive->num_args == 0) {
            sect->addWord((short)0, WRD);
            loc_cnt+=2;
        } else {
            int i = 0;
            while (i < proc->directive->num_args) {
                if(proc->directive->args->getElem(i)->type == INT) {
                    sect->addWord((short)(proc->directive->args->getElem(i)->value), WRD);
                    loc_cnt+=2;
                }
                else { //kada se naidje na labelu
                    string lab_name = proc->directive->args->getElem(i)->name;
                    SymTabEntry* e = symTable->findValue(lab_name);
                    RelocTable* r = relocs->getByName(curSection->name);
                    if (r == 0) return;
                    if (e->global == false) {
                        sect->addWord(e->offset,true, LNG); // doda ofset labele jer je lokalna!
                        r->addEntry(new RelocTabEntry(e->section,loc_cnt,0,R_386_32)); //index sekcije jer je lokalna!
                    } else {
                        sect->addWord(0,true, LNG); // ne znam ofset jer je globalna!
                        r->addEntry(new RelocTabEntry(e->index,loc_cnt,0,R_386_32)); // referisem samu varijablu jer je globl!
                    }
                    loc_cnt+=4;
                }
                i++;
            }
        }
    }
    else if (proc->directive->type == LONG) {
        Section* sect = sections->getByName(curSection->name);
        if (proc->directive->num_args == 0) {
            sect->addWord((int)0,true, LNG);
            loc_cnt+=4;
        }
        else {
            int i = 0;
            while (i < proc->directive->num_args) {
                if (proc->directive->args->getElem(i)->type == INT){
                    sect->addWord((int)(proc->directive->args->getElem(i)->value),true, LNG);
                }
                else {
                    if (checkForExpr(proc->directive->args->getElem(i))) {//za slucaj da smo naleteli na izraz
                        manageExpr(proc->directive->args->getElem(i));
                    }
                    else { //slucaj kada je u pitanju samo labela je isti kao u .word-u!
                        string lab_name = proc->directive->args->getElem(i)->name;
                        SymTabEntry* e = symTable->findValue(lab_name);
                        RelocTable* r = relocs->getByName(curSection->name);
                        if (r == 0) return;
                        if (e->global == false) {
                            sect->addWord(e->offset, true, LNG); // doda ofset labele jer je lokalna!
                            r->addEntry(new RelocTabEntry(e->section,loc_cnt,0,R_386_32)); //index sekcije jer je lokalna!
                        } else {
                            sect->addWord(0, true, LNG); // ne znam ofset jer je globalna!
                            r->addEntry(new RelocTabEntry(e->index,loc_cnt,0,R_386_32)); // referisem samu varijablu jer je globl!
                        }
                    }
                }
                loc_cnt+=4;
                i++;
            }
        }
    }
    else if (proc->directive->type == ALIGN) {
        if (proc->directive->num_args == 3) {
            int num_pad = 0;
            while ((loc_cnt+num_pad)% proc->directive->args->getElem(0)->value != 0)
                num_pad++;
            if (num_pad < proc->directive->args->getElem(2)->value) {
                loc_cnt+=num_pad;
                int i = 0;
                Section* sect = sections->getByName(curSection->name);
                while (i < num_pad) {
                    sect->addWord((unsigned char)proc->directive->args->getElem(1)->toChar(), CHR);
                    i++;
                }
            }
        }
        else  if (proc->directive->num_args == 2){
            while (loc_cnt%proc->directive->args->getElem(0)->value != 0) {
                loc_cnt++;
                Section* sect = sections->getByName(curSection->name);
                sect->addWord((unsigned char)proc->directive->args->getElem(1)->toChar(), CHR);
            }
        }
        else if (proc->directive->num_args == 1) {
            while (loc_cnt%proc->directive->args->getElem(0)->value != 0) {
                loc_cnt++;
                Section* sect = sections->getByName(curSection->name);
                sect->addWord((unsigned char)NULL, CHR);
            }
            
        }
    }
    else if (proc->directive->type == SKIP) {
        if (proc->directive->num_args == 2) {
            int i = 0;
            Section* sect = sections->getByName(curSection->name);
            while (i < proc->directive->args->getElem(0)->value) {
                sect->addWord((unsigned char)proc->directive->args->getElem(1)->toChar(), CHR);
                i++;
            }
            loc_cnt+=i;
        }
        else if (proc->directive->num_args == 1) {
            int i = 0;
            Section* sect = sections->getByName(curSection->name);
            while (i < proc->directive->args->getElem(0)->value) {
                sect->addWord((unsigned char)NULL, CHR);
                i++;
            }
            loc_cnt+=i;
        }
    }
    
}

bool Iterater:: checkForExpr(Argument* a) {
    string arg = a->name;
    if (arg.find("+")!= string::npos || arg.find("-")!= string::npos || arg.find("*")!= string::npos || arg.find("/")!= string::npos || arg.find("%")!= string::npos) return true;
    else return false;
}


void Iterater:: manageExpr(Argument* a) {
    string arg1 = "";
    string arg2 = "";
    char op1 = 'n';
    char op2 = 'n';
    int i = 0;
    bool args2 = false;
    //dohvatanje prvog operatora - ako ga ima
    if (a->name[i] == '+' || a->name[i] == '-') op1 = a->name[i++];
    
    //dohvatanje prvog argumenta
    while (a->name[i]!= '+' && a->name[i]!= '-' && a->name[i]!= '*' && a->name[i]!= '/' && a->name[i]!= '%' && a->name[i]!= ' ' && a->name[i]!= '\r' && i < a->name.length())
        arg1+=a->name[i++];
    if (i < a->name.length() && a->name[i]!='\r') {
        args2 = true;
        //dohvatanje drugog operatora - u zavisnosti kako je pisan izraz
        if (a->name[i] == ' ' && a->name[i+2] == ' ') {
            op2 = a->name[i+1];
            i+=3;
        }
        else if(a->name[i] == ' ' && a->name[i+2]!=' ') {
            op2 = a->name[i+1];
            i+=2;
        }
        else if (a->name[i]!=' ' && a->name[i+1] == ' ') {
            op2 = a->name[i];
            i+=2;
        } else {
            op2 = a->name[i];
            i++;
        }
        //dohvatanje drugog argumenta
        while (a->name[i]!= ',' && a->name[i]!= '\r' && i < a->name.length())
            arg2+=a->name[i++];
    }
    
    //obrada prvog argumenta
    bool arg1_lab = false;
    int arg1_value = -1;
    for (int j = 0; j < arg1.length(); j++) {
        if (std::isalpha(arg1[j])) arg1_lab = true;
    }
    if (!arg1_lab) {
        arg1_value = atoi(arg1.c_str());
        if (op1 == '-') arg1_value = 0-arg1_value;
    }
    else {
        //ide do tabele simbola pa trazi ofset!
        SymTabEntry* e = symTable->findValue(arg1);
        if (e->global == false) {
            arg1_value = e->offset;
            if (op1 == '-') arg1_value = 0-arg1_value;
        } else {
            arg1_value = 0;
        }
    }
    
    int arg2_value = -1;
    bool arg2_lab = false;
    if (args2) {
        //obrada drugog elementa
        for (int j = 0; j < arg2.length(); j++) {
            if (std::isalpha(arg2[j])) arg2_lab = true;
        }
        if (!arg2_lab) arg2_value = atoi(arg2.c_str());
        else {
            //ide do tabele simbola pa trazi ofset!
            SymTabEntry* e = symTable->findValue(arg2);
            if (e->global == false) {
                arg2_value = e->offset;
            } else {
                arg2_value = 0;
            }
        }
    }
    
    int result = 0;
    if (!args2) result = arg1_value;
    else {
        switch (op2) {
            case '-':
                result = arg1_value-arg2_value;
                break;
            case '+':
                result = arg1_value+arg2_value;
                break;
            case '*':
                result = arg1_value*arg2_value;
                break;
            case '/':
                result = arg1_value/arg2_value;
                break;
            case '%':
                result = arg1_value*arg2_value;
                break;
        }
    }

    // zapisujemo rezultat u tekucu sekciju i nalazimo adekvatnu tabelu relokacije
    Section* sect = sections->getByName(curSection->name);
    sect->addWord(result, true, LNG);
    RelocTable* r = relocs->getByName(curSection->name);
    reloc_type t1 = (op1 == '-')?R_386_32NEG:R_386_32;
    reloc_type t2 = (op2 == '-')?R_386_32NEG:R_386_32;
    
    //pravi zapise o relokaciji za simbole u izrazu
    if (arg1_lab && arg2_lab) {
        if (!(!symTable->findValue(arg1)->global && !symTable->findValue(arg2)->global && symTable->findValue(arg1)->section == symTable->findValue(arg2)->section && op2 == '-' && op1 != '-')){ // ako nisu oba lokalna i iz iste sekcije
            
            if (symTable->findValue(arg1)->global) {
                r->addEntry(new RelocTabEntry(symTable->findValue(arg1)->index,loc_cnt,0,t1));
            }
            else {
                r->addEntry(new RelocTabEntry(symTable->findValue(arg1)->section,loc_cnt,0,t1));
            }
            if (symTable->findValue(arg2)->global) {
                r->addEntry(new RelocTabEntry(symTable->findValue(arg2)->index,loc_cnt,0,t2));
            }
            else {
                r->addEntry(new RelocTabEntry(symTable->findValue(arg2)->section,loc_cnt,0,t2));
            }
        }
    }
    else if(arg1_lab) {
        if (symTable->findValue(arg1)->global) {
            r->addEntry(new RelocTabEntry(symTable->findValue(arg1)->index,loc_cnt,0,t1));
        }
        else {
            r->addEntry(new RelocTabEntry(symTable->findValue(arg1)->section,loc_cnt,0,t1));
        }
    }
    else if(arg2_lab) {
        if (symTable->findValue(arg2)->global) {
            r->addEntry(new RelocTabEntry(symTable->findValue(arg2)->index,loc_cnt,0,t2));
        }
        else {
            r->addEntry(new RelocTabEntry(symTable->findValue(arg2)->section,loc_cnt,0,t2));
        }
    }
}



int Iterater:: handleFirstByte(int cond, int flag, int type) {
    int instr = 0;
    ByteManip* manip = ByteManip::getInstance();
    instr = manip->modify(instr, cond, 0, 3);
    instr = manip->modify(instr, flag, 3, 1);
    instr = manip->modify(instr, type, 4, 4);
    return instr;
}

void Iterater:: addToSection(int instr) {
    Section* sect = sections->getByName(curSection->name);
    sect->addWord(instr, false, LNG);
    loc_cnt+=4;
}
void Iterater:: interruptHandler(ProcData* proc) {
    int instr = 0;
    ByteManip* manip = ByteManip::getInstance();
    instr = handleFirstByte(proc->instruction->cond, 0, 0);
    instr = manip->modify(instr, proc->instruction->args->getElem(0)->value, 8, 4);
    addToSection(instr);
}

void Iterater:: arithmeticHandler(ProcData* proc) {
    int instr = 0;
    ByteManip* manip = ByteManip::getInstance();
    int op_code = 1;
    switch (proc->instruction->instruction[0]) {
        case 'a':
            op_code = 1;
            break;
        case 's':
            op_code = 2;
            break;
        case 'm':
            op_code = 3;
            break;
        case 'd':
            op_code = 4;
            break;
        case 'c':
            op_code = 5;
            break;
    }
    instr = handleFirstByte(proc->instruction->cond, 1, op_code);
    instr = manip->modify(instr, proc->instruction->args->getElem(0)->value, 8, 5);
    if (proc->instruction->immediate) {
        instr = manip->modify(instr, 1, 13, 1);
        instr = manip->modify(instr, proc->instruction->args->getElem(1)->value, 14, 18);
    }
    else {
        instr = manip->modify(instr, 0, 13, 1);
        instr = manip->modify(instr, proc->instruction->args->getElem(1)->value, 14, 5);
    }
    addToSection(instr);
}

void Iterater:: logicalHandler(ProcData* proc) {
    int instr = 0;
    ByteManip* manip = ByteManip::getInstance();
    int op_code = 1;
    switch (proc->instruction->instruction[0]) {
        case 'a':
            op_code = 6;
            break;
        case 'o':
            op_code = 7;
            break;
        case 'n':
            op_code = 8;
            break;
        case 't':
            op_code = 9;
            break;
    }
    instr = handleFirstByte(proc->instruction->cond, 1, op_code);
    instr = manip->modify(instr, proc->instruction->args->getElem(0)->value, 8, 5);
    instr = manip->modify(instr, proc->instruction->args->getElem(1)->value, 13, 5);
    addToSection(instr);
}

void Iterater:: ioHandler(ProcData* proc) {
    int instr = 0;
    ByteManip* manip = ByteManip::getInstance();
    instr = handleFirstByte(proc->instruction->cond, 0, 13);
    int io = (proc->instruction->instruction == "in")?1:0;
    instr = manip->modify(instr, proc->instruction->args->getElem(0)->value, 8, 4);
    instr = manip->modify(instr, proc->instruction->args->getElem(1)->value, 12, 4);
    instr = manip->modify(instr, io, 16, 1);
    addToSection(instr);
}

void Iterater:: movshHandler(ProcData* proc) {
    int instr = 0;
    ByteManip* manip = ByteManip::getInstance();
    instr = handleFirstByte(proc->instruction->cond, 1, 14);
    instr = manip->modify(instr, proc->instruction->args->getElem(0)->value, 8, 5);
    instr = manip->modify(instr, proc->instruction->args->getElem(1)->value, 13, 5);
    if (proc->instruction->instruction == "shl") {
        instr = manip->modify(instr, proc->instruction->args->getElem(2)->value, 18, 5);
        instr = manip->modify(instr, 1, 23, 1);
    }
    else if (proc->instruction->instruction=="shr") {
        instr = manip->modify(instr, proc->instruction->args->getElem(2)->value, 18, 5);
        instr = manip->modify(instr, 0, 23, 1);
    }
    addToSection(instr);
}

void Iterater:: ldstHandler(ProcData* proc) {
    int postfix = 1;
    if (proc->instruction->instruction.length() == 5) {
        string postfx = "";
        postfx+=proc->instruction->instruction[3];
        postfx+=proc->instruction->instruction[4];
        if (postfx == "ia") postfix = 2;
        else if (postfx == "da") postfix = 3;
        else if (postfx == "ib") postfix = 4;
        else if (postfx == "db") postfix = 5;
    }
    if (proc->instruction->args->getElem(0)->value == 16) postfix = 0; // za slucaj da je registar pc
    int type = 0;
    if (proc->instruction->instruction.find("ldr")!=string::npos) type = 1;
    
    int instr = 0;
    ByteManip* manip = ByteManip::getInstance();
    instr = handleFirstByte(proc->instruction->cond, 0, 10);
    instr = manip->modify(instr, proc->instruction->args->getElem(0)->value, 8, 5);
    instr = manip->modify(instr, proc->instruction->args->getElem(1)->value, 13, 5);
    instr = manip->modify(instr, postfix, 18, 3);
    instr = manip->modify(instr, type, 21, 1);
    if (proc->instruction->args->getElem(2)->type == INT)
        instr = manip->modify(instr, proc->instruction->args->getElem(2)->value, 22, 10);
    else {
        SymTabEntry* e = symTable->findValue(proc->instruction->args->getElem(2)->name);
        RelocTable* r = relocs->getByName(curSection->name);
        if (e->global) {
            instr = manip->modify(instr, 0, 22, 10);
            (postfix!=0)?r->addEntry(new RelocTabEntry(e->index,loc_cnt+2,6,R_386_10)):r->addEntry(new RelocTabEntry(e->index,loc_cnt+2,6,R_386_PC10));
        }
        else {
            instr = manip->modify(instr, e->offset, 22, 10);
            (postfix!=0)?r->addEntry(new RelocTabEntry(e->section,loc_cnt+2,6,R_386_10)):r->addEntry(new RelocTabEntry(e->section,loc_cnt+2,6,R_386_PC10));
        }
    }
    addToSection(instr);
}


void Iterater:: callHandler(ProcData* proc) {
    int instr = 0;
    bool pc = false;
    ByteManip* manip = ByteManip::getInstance();
    instr = handleFirstByte(proc->instruction->cond, 0, 12);
    if (proc->instruction->type == TWO_OP) {
        instr = manip->modify(instr, proc->instruction->args->getElem(0)->value, 8, 5);
        if (proc->instruction->args->getElem(0)->value == 16) pc = true;
    }
    else  {// ako ima jedan operand - podrazumeva se da je reg = pc
        instr = manip->modify(instr, 16, 8, 5);
        pc = true;
    }
    if (proc->instruction->args->getLast()->type == INT) {// ako je immediate
        (pc)?instr = manip->modify(instr, proc->instruction->args->getLast()->value-loc_cnt-4, 13, 19):instr = manip->modify(instr, proc->instruction->args->getLast()->value, 13, 19);
    }
    else {
        SymTabEntry* e = symTable->findValue(proc->instruction->args->getLast()->name);
        RelocTable* r = relocs->getByName(curSection->name);
        if (e->global) {
            (pc)?instr = manip->modify(instr, 0-loc_cnt-4, 13, 19):instr = manip->modify(instr, 0, 13, 19);
            (pc)?r->addEntry(new RelocTabEntry(e->index,loc_cnt+1,5,R_386_PC19)):r->addEntry(new RelocTabEntry(e->index,loc_cnt+1,5,R_386_19));
        }
        else {
            (pc)?instr = manip->modify(instr, e->offset-loc_cnt-4, 13, 19):instr = manip->modify(instr, e->offset, 13, 19);
            if (e->section != curSection->section || pc == false)
                (pc)?r->addEntry(new RelocTabEntry(e->section,loc_cnt+1,5,R_386_PC19)):r->addEntry(new RelocTabEntry(e->section,loc_cnt+1,5,R_386_19));
        }
    }
    addToSection(instr);
}

void Iterater:: ldcHandler(ProcData* proc) {
    ByteManip* manip = ByteManip::getInstance();
    SymTabEntry* e = symTable->findValue(proc->instruction->args->getElem(1)->name);
    RelocTable* r = relocs->getByName(curSection->name);
    if (proc->instruction->instruction == "ldcl" || proc->instruction->instruction == "ldc") {
        int instr = 0;
        instr = handleFirstByte(proc->instruction->cond, 0, 15);
        instr = manip->modify(instr, proc->instruction->args->getElem(0)->value, 8, 4);
        instr = manip->modify(instr, 0, 12, 1);
        if (e->global) {
            instr = manip->modify(instr, 0, 16, 16);
            r->addEntry(new RelocTabEntry(e->index,loc_cnt+2,0,R_386_16L));
        }
        else {
            instr = manip->modify(instr, e->offset, 16, 16);
            r->addEntry(new RelocTabEntry(e->section,loc_cnt+2,0,R_386_16L));
        }
        addToSection(instr);
    }
    if (proc->instruction->instruction == "ldch" || proc->instruction->instruction == "ldc") {
        int instr = 0;
        instr = handleFirstByte(proc->instruction->cond, 0, 15);
        instr = manip->modify(instr, proc->instruction->args->getElem(0)->value, 8, 4);
        instr = manip->modify(instr, 1, 12, 1);
        if (e->global) {
            instr = manip->modify(instr, 0, 16, 16);
            r->addEntry(new RelocTabEntry(e->index,loc_cnt+2,0,R_386_16H));
        }
        else {
            int mod_entry = e->offset;
            mod_entry = mod_entry&0xffff0000;
            mod_entry = mod_entry>>16;
            instr = manip->modify(instr, mod_entry, 16, 16);
            r->addEntry(new RelocTabEntry(e->section,loc_cnt+2,0,R_386_16H));
        }
        addToSection(instr);
    }
}

void Iterater::instructionHandlerSecondPass(ProcData* proc) {
    if (proc->instruction->instruction == "int")
        interruptHandler(proc);
    else if (proc->instruction->instruction == "add" ||proc->instruction->instruction == "sub" ||proc->instruction->instruction == "mul" ||proc->instruction->instruction == "div" ||proc->instruction->instruction == "cmp")
        arithmeticHandler(proc);
    else if (proc->instruction->instruction == "and" || proc->instruction->instruction == "or" || proc->instruction->instruction == "not" || proc->instruction->instruction == "test")
        logicalHandler(proc);
    else if (proc->instruction->instruction.find("ldr")!=string::npos || proc->instruction->instruction.find("str")!=string::npos)
        ldstHandler(proc);
    else if (proc->instruction->instruction == "call")
        callHandler(proc);
    else if (proc->instruction->instruction == "in" || proc->instruction->instruction == "out")
        ioHandler(proc);
    else if (proc->instruction->instruction == "mov" || proc->instruction->instruction == "shl" || proc->instruction->instruction == "shr")
        movshHandler(proc);
    else if(proc->instruction->instruction.find("ldc")!=string::npos)
        ldcHandler(proc);
}


















