#ifndef TOKENLIST_H
#define TOKENLIST_H

#include "Token.h"
#include <Typedefs.h>

class TokenListException
{
    public:
        TokenListException() noexcept = default;
        virtual ~TokenListException() noexcept = default;
        virtual const char* what() noexcept = 0;
};

class TokenListEndToken : public TokenListException
{
    public:
        TokenListEndToken() noexcept = default;
        virtual ~TokenListEndToken() noexcept = default;
        virtual const char* what() noexcept { return "Token List : End Token"; };
};

class TokenList
{
    public:
        TokenList(TokenListPtr tokenList);
        ~TokenList();

        Token get();
        Token peek(uint64_t offset = 0);
        void seek(uint64_t offset);

    private:
        TokenListPtr tokenListPtr;
        TokenListIter iterator;
};

#endif // TOKENLIST_H
