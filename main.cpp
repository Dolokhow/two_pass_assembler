//
//  main.cpp
//  sys_soft
//
//  Created by Djordje Bozic on 8/5/16.
//  Copyright © 2016 Djordje Bozic. All rights reserved.
//
#include "Iterater.h"

int main(int argc, const char * argv[]) {
   
   Iterater* iter = new Iterater(argv[1],argv[2]);
    iter->firstPass();
    iter->secondPass();
    delete iter;
    return 0;
    
}
