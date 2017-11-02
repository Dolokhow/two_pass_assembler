//
//  FileReader.cpp
//  sys_soft
//
//  Created by Djordje Bozic on 8/5/16.
//  Copyright © 2016 Djordje Bozic. All rights reserved.
//

#include "FileReader.h"

FileReader* FileReader::instance = 0;

FileReader* FileReader::getInstance() {
    if(instance == 0) {
        instance = new FileReader();
    }
    return instance;
}

void FileReader::loadFile(const char* filePath) {
    if(myFile.is_open()) myFile.close();
    myFile.open(filePath);
}

void FileReader:: closeFile() {
    if(myFile.is_open()) myFile.close();
}

string FileReader:: readLine() {
    string line;
    if(myFile.is_open()) {
        getline(myFile,line);
        if (myFile.eof()) {
            line=".end";
            myFile.close();
        }
        return line;
    }
    return "ERR";
}





