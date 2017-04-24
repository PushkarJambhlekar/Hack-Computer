#include "parser.h"

//typedef string string;

using namespace std;


pair <string, int> getSymPair(string str,int base)
{
    char *pch;
    char *buffer = strdup(str.c_str());
    string val;
    pch = strtok (buffer, SYM_SEPERATOR);
    val = pch;
    pch = strtok (NULL, SYM_SEPERATOR);
    int i = stoi ((string)pch, NULL, base);
    return pair<string, int>(val,i);
}

const vector<string> & Parser :: getPass2()
{
    return pass2;
}
bool isNumber (string str)
{
    char *p;
    strtol(str.c_str(), &p, 10);
    return *p == 0;
}

map<string,int>::iterator Parser:: isPresent(int val, string key, bool *bValid)
{
    map <string, int> m;

    switch (val)
    {
        case LABLES:
            m = lables;
        break;

        case SYMBOLS:
            m = symbols;
        break;

        case VARIABLES:
            m = variables;
        break;
    }

    map<string,int>::iterator it;

    it = m.find(key);
    *bValid = (it != m.end());
    return it;
}
void Parser :: replaceSymbolsAndVariables (string path)
{
    vector<string> vec = openFile(path);

    pass2.empty();
    for (auto it = vec.begin(); it!=vec.end(); it++)
    {
        string str = *it;
        string sym = str.substr(1,str.size()-1);
        if (str[0] == '@' && !isNumber(sym))
        {
            bool found;
            auto loc = isPresent(SYMBOLS, sym, &found);
            int var = 0;
            if (!found)
            {
                loc = isPresent(VARIABLES, sym, &found);
                if (!found)
                {
                    var = varAddress;
                    variables.insert(pair<string,int>(sym,varAddress++));
                }
                else
                {
                    var = loc->second;
                }
            }
            else
            {
                var = loc->second;
            }
            str = "@"+to_string(var);
        }
        //cout<<str<<endl;
        pass2.push_back(str);
    }
}

int Parser :: getLableValue(string str)
{
    auto it = lables.find(str);

    if (it != lables.end())
    {
        return it->second;
    }
    return -1;
}
bool Parser :: isLable(string str)
{
    return (str[0] == '(' && str[str.size()-1] == ')');
}
void Parser::replaceLables(string path)
{
    vector<string> vec = openFile(path);

    ofstream out;
    out.open(path+".pass1");
    for(auto it=vec.begin(); it!=vec.end();it++)
    {
        string str = *it;

        if (isLable(str))
            continue;
        if (str[0]=='@')
        {
            int val;
            if ((val = getLableValue(str.substr (1,str.size()-1))) != -1)
            {
                str = "@"+to_string(val);
            }
        }
        out <<str<<endl;
    }
    out.close();
}

void Parser :: buildDefsymMap(string path)
{
    vector<string> vec = openFile(path);

    for(auto it=vec.begin(); it!=vec.end();it++)
    {
        symbols.insert(getSymPair(*it,10));
    }
}
int Parser :: getAsm(int type, string str)
{
    map<string, int> m;

    switch(type)
    {
        case ALU_MAP:
            m = aluMap;
            break;
        case DST_MAP:
            m = dstMap;
            break;
        case JMP_MAP:
            m = jmpMap;
            break;
    }

    auto it = m.find(str);
    if (it == m.end())
    {
        cout <<"Failed to find out "<<str<<endl;
        exit(0);
    }
    return it->second;
}
void Parser :: printMap(int which)
{
    map<string, int> m;

    switch (which)
    {
        case 0:
            m = lables;
            break;
        case 1:
            m = symbols;
            break;
        case 2:
            m = variables;
            break;
        case 3: 
            m = aluMap;
            break;
        case 4: 
            m = dstMap;
            break;
        case 5: 
            m = jmpMap;
            break;
    }

    for (auto it = m.begin(); it != m.end(); it++)
    {
        cout <<it->first<<":"<<it->second<<endl;
    }
}

void Parser::buildSymtable(string path)
{
    vector<string> vec = openFile(path);
    string symFile=path+".sym";
    ofstream out;
    out.open(symFile);

    int i = 0;
    for (auto it=vec.begin(); it!=vec.end();it++)
    {
        string str = *it;
        if(str[0] == '(')
        {
            string outStr = str.substr(1,str.size()-2)+SYM_SEPERATOR+to_string(i);
            out<<outStr<<endl;
            i--;
            lables.insert(getSymPair(outStr,10));
        }
        i++;
    }
}

void Parser :: constructAluMap()
{
    vector<string> vec = openFile("ALU_MAP");

    for (auto it = vec.begin(); it != vec.end(); it++)
    {
        aluMap.insert(getSymPair(*it, 2));
    }

}
void Parser :: constructDstMap()
{
    vector<string> vec = openFile("DST_MAP");

    for (auto it = vec.begin(); it != vec.end(); it++)
    {
        dstMap.insert(getSymPair(*it, 2));
    }

}
void Parser :: constructJmpMap()
{
    vector<string> vec = openFile("Jmp_MAP");

    for (auto it = vec.begin(); it != vec.end(); it++)
    {
        jmpMap.insert(getSymPair(*it, 2));
    }

}
vector<string> Parser :: openFile(string file)
{
    vector<string> vec;
    ifstream asmFile (file);
    if (!asmFile)
    {
        cout <<file<<" File does not exists\n";
        return vec;
    }

    for (string line; getline(asmFile, line); )
    {
        vec.push_back(line);
    }
    asmFile.close();
    return vec;
}
void Parser :: generateCleanFile(string src, string dst)
{
    vector<string> vec = openFile(src);
    ofstream dstFile;
    char tempStr[10000];

    dstFile.open(dst);

    int i = 0;
    for (vector<string>::iterator it=vec.begin(); it!=vec.end(); it++)
    {
        string str = *it;
        auto ind = remove (str.begin(), str.end(), ' ');
        str.erase (ind, str.end());

        if (str.c_str()[0]!='/'&& !str.empty())
        {
            size_t found = str.find("//");
            if (found != string::npos)
            {
                str = str.substr(0,found);
            }
            dstFile<<str<<endl;
        }
    }
    dstFile.close();
}
