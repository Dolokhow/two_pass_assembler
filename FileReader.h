//
//  FileReader.h
//  sys_soft
//
//  Created by Djordje Bozic on 8/5/16.
//  Copyright © 2016 Djordje Bozic. All rights reserved.
//

#ifndef FileReader_h
#define FileReader_h

#include <fstream>
using namespace std;


class FileReader {
private:
    ifstream myFile;
    static FileReader* instance;
    FileReader(){};
    FileReader(const FileReader& fr);
    
public:
    static FileReader* getInstance();
    void loadFile(const char* filePath);
    void closeFile();
    string readLine();
};


#endif /* FileReader_h */
