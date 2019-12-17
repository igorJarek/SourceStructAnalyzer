#ifndef FUNCTIONINFO_H
#define FUNCTIONINFO_H

#include <Typedefs.h>

using namespace std;

class FunctionInfo
{
    public:
        FunctionInfo(const string& name, Pos _line, Pos _pos) : functionName(name), line(_line), pos(_pos) { }
        ~FunctionInfo();

        void setFunctionList(FunctionInfoListPtr list);
        FunctionInfoListPtr getFunctionList(void) const { return functions; }

        string getName(void) const { return functionName; }
        Pos getLine(void) const { return line; }
        Pos getPos(void) const { return pos; }

    private:
        string functionName {};
        Pos line;
        Pos pos;
        FunctionInfoListPtr functions;
};

#endif // FUNCTIONINFO_H
