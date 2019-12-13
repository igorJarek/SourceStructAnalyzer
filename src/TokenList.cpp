#include "TokenList.h"

TokenList::TokenList(shared_ptr<list<Token>> tokenList)
{
    tokenListPtr = tokenList;
    iterator = tokenListPtr->begin();
}

TokenList::~TokenList()
{
    //dtor
}

Token TokenList::get()
{
    Token token = *iterator;
    ++iterator;
    return token;
}

Token TokenList::peek(uint64_t offset)
{
    ListIterator begin = iterator;
    for(uint64_t index = 0; index < offset; ++index)
    {
        ++begin;
        if(begin == tokenListPtr->end())
            throw TokenListEndToken();
    }

    return *begin;
}

void TokenList::seek(uint64_t offset)
{
    for(uint64_t index = 0; index < offset; ++index)
    {
        ++iterator;
        if(iterator == tokenListPtr->end())
            throw TokenListEndToken();
    }
}
