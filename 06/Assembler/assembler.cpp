#include "parser.h"
#include "tokenizer.h"

int main(int argc, char**argv)
{
    string src(argv[1]);
    string dst = "." + src.substr(src.rfind("\\")+1,src.size()-1);

    Parser p(src,dst);
    Tokenizer t;

    t.build(&p);
    t.convertToAsm();
    t.writeToFile(src);
    return 0;
}