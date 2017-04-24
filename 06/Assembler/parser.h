#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <map>

#ifndef __parser_h_
#define __parser_h_

using namespace std;

#define SYM_SEPERATOR       "="
#define LABLES              0
#define SYMBOLS             1
#define VARIABLES           2
#define ALU_MAP             3
#define DST_MAP             4
#define JMP_MAP             5
class Parser
{
    map <string, int> lables;
    map <string, int> symbols;
    map <string, int> variables;
    map <string, int> aluMap;
    map <string, int> dstMap;
    map <string, int> jmpMap;
    vector<string> pass2;
    int varAddress;


    void constructAluMap();
    void constructJmpMap();
    void constructDstMap();
    void buildDefsymMap(string);
    void replaceLables(string);
    bool isLable(string);
    int getLableValue(string);
    void replaceSymbolsAndVariables(string);
    map<string,int>::iterator isPresent(int,string,bool*);
    void generateCleanFile(string src, string dst);
    vector<string> openFile(string file);
    void buildSymtable(string);


    public:
    Parser()
    {
        varAddress = 16;
        constructAluMap();
        constructJmpMap();
        constructDstMap();
        buildDefsymMap("SYMBOLS");
        cout << "Initialized"<<endl;
    }

    Parser (string src, string dst)
    {
        varAddress = 16;
        constructAluMap();
        constructJmpMap();
        constructDstMap();
        buildDefsymMap("SYMBOLS");
        
        generateCleanFile(src, dst);
        buildSymtable(dst);
        replaceLables(dst);
        replaceSymbolsAndVariables(dst+".pass1");
    }

    void printMap(int i);

    const vector<string> & getPass2();
    int getAsm(int type, string str);
};

#endif