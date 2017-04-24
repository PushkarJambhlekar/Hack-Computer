#include <iostream>
#include <string>
#include <bitset>
#include "parser.h"
#include "bitwise.h"
#include "tokenizer.h"

void Tokenizer :: convertToAsm()
{
    for(auto it = intn.begin(); it!=intn.end(); it++)
    {
        instruction *i = &(*it);
        unsigned short out = 0;
        if (i->type == TYPE_A)
        {
            out = i->i.a;
        }
        else
        {
            out = SET_FIELD(out, i->i.c.destination, INSTRUCTION_DST);
            out = SET_FIELD(out, i->i.c.expression, INSTRUCTION_EXPR);
            out = SET_FIELD(out, i->i.c.jmp, INSTRUCTION_JMP);
            out = SET_FIELD(out, 0xf, INSTRUCTION_C_FIELD);
        }
        asmOut.push_back(out);
    }
}
void printInstruction(instruction *p)
{
    if (p->type == TYPE_A)
    {
        cout << "a = "<<p->i.a<<endl;
    }
    else
    {
        cout <<"Dst = "<<p->i.c.destination<<"\texpr = "<<p->i.c.expression<<"\tjmp = "<<p->i.c.jmp<<endl;
    }
}
void Tokenizer :: tokenizeCInstruction(string src, instruction *pDest, Parser *p)
{
    char *pch;
    string out;
    string jmpInstrunction;
    char *buf = strdup(src.c_str());
    //pch = strtok(buf, "=");
    int dst = 0;

    pDest->i.c.destination = 0;
    pDest->i.c.expression = 0;
    pDest->i.c.jmp = 0;

    pch = strchr(buf, DST_TOKEN);
    if (pch != NULL)
    {
        dst = pch-buf;
        out = src.substr(0,dst);
        pDest->i.c.destination = p->getAsm(DST_MAP, out);

    }

    pch = strchr(buf+dst+1, JMP_TOKEN);
    if(pch != NULL)
    {
        int e = pch - (buf+dst);
        out = src.substr(dst, e-dst);
        pDest->i.c.expression = p->getAsm(ALU_MAP, out);

        out = src.substr(e+1, 3);
        pDest->i.c.jmp = p->getAsm(JMP_MAP, out);
    }
    else
    {
        out = src.substr(dst+1, dst-strlen(buf));
        pDest->i.c.expression = p->getAsm(ALU_MAP, out);
    }
}
struct instruction Tokenizer :: getInstruction (string str, Parser *p)
{
    instruction i = {0};
    unsigned short val;
    if (str[0] == '@')
    {
        i.type = TYPE_A;
        i.i.a = VAL_MASK(GET_VAL(str.c_str()+1),INSTRUCTION_A_FIELD);
    }
    else
    {
        i.type = TYPE_C;
        tokenizeCInstruction(str, &i, p);
    }

    return i;
}

void Tokenizer :: tokenize (const vector <string>& vec, Parser *p)
{
    for (auto it=vec.begin(); it != vec.end(); it++)
    {
        string str = *it;
        intn.push_back(getInstruction(str, p));
    }
}

void Tokenizer :: build (Parser *p)
{
    const vector <string>& pass2 = p->getPass2();
    cout <<"in build"<<endl;
    tokenize(pass2, p);
}
void Tokenizer :: print()
{
    for (auto it = intn.begin(); it!=intn.end(); it++)
    {
        printInstruction(&(*it));
    }

     for (auto it = asmOut.begin(); it!=asmOut.end(); it++)
    {
        cout<<bitset<16>(*it)<<endl;
    }
}

void Tokenizer :: writeToFile(string path)
{
    ofstream file;
    string name = path.substr(0, path.rfind("."))+".hack";
    file.open (name);
    cout <<"Writing to file "<<name<<endl;
    for (auto it = asmOut.begin(); it!=asmOut.end(); it++)
    {
        file<<bitset<16>(*it)<<endl;
    }
    file.close();
}
