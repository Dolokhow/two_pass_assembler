//
//  FileWriter.h
//  sys_soft
//
//  Created by Djordje Bozic on 8/5/16.
//  Copyright © 2016 Djordje Bozic. All rights reserved.
//

#ifndef FileWriter_h
#define FileWriter_h

#include <fstream>
using namespace std;

class FileWriter {
private:
    ofstream myFile;
    static FileWriter* instance;
    FileWriter(){};
    FileWriter(const FileWriter& fr);
    
public:
    static FileWriter* getInstance();
    void loadFile(const char* filePath);
    void closeFile();
    void writeLine(string);
    void write(unsigned char);
    void write(unsigned short);
    void write(unsigned int);
    void writeByte(unsigned short);
    void newRow();

};



#endif /* FileWriter_h */
