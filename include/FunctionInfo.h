#ifndef FUNCTIONINFO_H
#define FUNCTIONINFO_H

#include <list>
#include <string>
#include <memory>

using namespace std;

class FunctionInfo
{
    public:
        using Pos = pair<uint32_t, uint32_t>;

        FunctionInfo(const string& _name, Pos _line, Pos _pos) : functionName(_name), line(_line), pos(_pos) { }
        ~FunctionInfo();

        void setFunctionList(shared_ptr<list<FunctionInfo>> list)
        {
            functions = list;
        }

        string getName() const { return functionName; }
        Pos getLine() const { return line; }
        Pos getPos() const { return pos; }

    private:
        string functionName {};
        Pos line;
        Pos pos;
        shared_ptr<list<FunctionInfo>> functions;
};

#endif // FUNCTIONINFO_H
