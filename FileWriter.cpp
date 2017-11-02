//
//  FileWriter.cpp
//  sys_soft
//
//  Created by Djordje Bozic on 8/5/16.
//  Copyright © 2016 Djordje Bozic. All rights reserved.
//

#include <stdio.h>
#include <iomanip>
#include "FileWriter.h"

FileWriter* FileWriter::instance = 0;

FileWriter* FileWriter::getInstance() {
    if(instance == 0) {
        instance = new FileWriter();
    }
    return instance;
}

void FileWriter::loadFile(const char* filePath) {
    if(myFile.is_open()) myFile.close();
    myFile.open(filePath, std::ios::app);
}

void FileWriter:: closeFile() {
    if(myFile.is_open()) myFile.close();
}

void FileWriter:: writeLine(string line) {
    if(myFile.is_open()) {
        myFile << line << '\n';
    }
}

void FileWriter:: write(unsigned char var) {
    if (myFile.is_open()) {
        myFile << std::setw(2) <<std::setfill('0')<<std::hex<<std::uppercase<<(unsigned int)var <<' ';
    }
}

void FileWriter:: write(unsigned int var) {
    if (myFile.is_open()) {
        myFile << std::setw(8) <<std::setfill('0')<<std::hex<<std::uppercase<<(unsigned int)var <<' ';
    }
}

void FileWriter:: write(unsigned short var) {
    if (myFile.is_open()) {
        myFile << std::setw(4) <<std::setfill('0')<<std::hex<<std::uppercase<<(unsigned short)var <<' ';
    }
}

void FileWriter::writeByte(unsigned short var) {
    if (myFile.is_open()) {
        myFile << std::setw(2) <<std::setfill('0')<<std::hex<<std::uppercase<<(unsigned short)var <<' ';
    }
}

void FileWriter:: newRow() {
    if(myFile.is_open()) {
        myFile << '\n';
    }
}

