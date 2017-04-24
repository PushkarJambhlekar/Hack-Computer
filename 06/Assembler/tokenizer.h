#include "parser.h"

#ifndef __tokenizer_h
#define __tokenizer_h

using namespace std;

#define TYPE_A           0
#define TYPE_C           1

#define DST_TOKEN       '='
#define JMP_TOKEN       ';'


struct instruction
{
    int type;
    union
    {
        struct
        {
            int destination;
            int expression;
            int jmp;
        } c;
        unsigned short  a;
    } i;
};

class Tokenizer
{
    vector <instruction> intn;
    vector <unsigned short> asmOut;
    map<string,unsigned short> iToc;

    string getDestination(string);
    void tokenize(const vector<string>&, Parser *);
    struct instruction getInstruction(string, Parser*);
    void tokenizeCInstruction(string, instruction *, Parser *p);
    
    public:

    void print();
    void build(Parser *);
    void convertToAsm();
    void writeToFile(string);
};

#endif
