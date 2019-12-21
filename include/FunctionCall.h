#ifndef FUNCTIONCALL_H
#define FUNCTIONCALL_H

#include <Typedefs.h>
#include <Token.h>
#include <string>

class FunctionCall
{
    public:
        FunctionCall(Token functionNameToken, Token openParenthesisToken, Token closeParenthesisToken) :
            m_functionNameToken(functionNameToken),
            m_openParenthesisToken(openParenthesisToken),
            m_closeParenthesisToken(closeParenthesisToken)
        {

        }

        ~FunctionCall() = default;

        std::string getFunctionName()           const { return m_functionNameToken.lexeme(); }
        uint32_t    getFunctionNameLine()       const { return m_functionNameToken.line(); }
        Pos         getFunctionNamePos()        const { return m_functionNameToken.pos(); }

        uint32_t    getOpenParenthesisLine()    const { return m_openParenthesisToken.line(); }
        Pos         getOpenParenthesisPos()     const { return m_openParenthesisToken.pos(); }

        uint32_t    getCloseParenthesisLine()   const { return m_closeParenthesisToken.line(); }
        Pos         getCloseParenthesisPos()    const { return m_closeParenthesisToken.pos(); }

    private:
        Token m_functionNameToken;
        Token m_openParenthesisToken;
        Token m_closeParenthesisToken;
};

#endif // FUNCTIONCALL_H
