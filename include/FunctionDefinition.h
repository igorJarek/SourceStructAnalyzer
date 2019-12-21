#ifndef FUNCTIONINFO_H
#define FUNCTIONINFO_H

#include <Typedefs.h>

using namespace std;

class FunctionDefinition
{
    public:
        FunctionDefinition(const string& functionName, Pos line, FunctionCallListPtr functionCalls)
            : m_functionName(functionName), m_line(line), m_functionCalls(functionCalls)
        {

        }
        ~FunctionDefinition()
        {

        }

        FunctionCallListPtr getFunctionCallList(void) const { return m_functionCalls; }

        string getName(void) const { return m_functionName; }
        Pos getLine(void) const { return m_line; }

    private:
        string m_functionName {};
        Pos m_line;
        FunctionCallListPtr m_functionCalls;
};

#endif // FUNCTIONINFO_H
