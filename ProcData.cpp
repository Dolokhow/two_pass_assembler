//
//  ProcData.cpp
//  sys_soft
//
//  Created by Djordje Bozic on 8/10/16.
//  Copyright © 2016 Djordje Bozic. All rights reserved.
//

#include "ProcData.h"

void ArgumentList::add(Argument* arg) {
    Elem* newElem = new Elem(arg);
    if(cnt) {
        last->next = newElem;
        last = newElem;
    } else {
        first = newElem;
        last = newElem;
    }
    cnt++;
}

Argument* ArgumentList::getElem(int index) {
    int ite = 0;
    Elem* cur = first;
    while (ite < index) {
        cur = cur->next;
        ite++;
    }
    if (cur!=0) return cur->arg;
    else return 0;
}

Argument* ArgumentList::getLast() {
    return last->arg;
}
ArgumentList::~ArgumentList() {
    Elem* to_del = first;
    while (first!=0) {
        to_del = first;
        first = first->next;
        delete to_del;
    }
}

