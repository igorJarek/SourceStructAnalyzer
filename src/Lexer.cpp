#include "Lexer.h"

namespace {
    set<string> createKeywordsSet()
    {
        set<string> keywordsList;
        keywordsList.insert("alignas");     keywordsList.insert("dynamic_cast");    keywordsList.insert("reinterpret_cast");
        keywordsList.insert("alignof");     keywordsList.insert("else");            keywordsList.insert("requires");
        keywordsList.insert("and");         keywordsList.insert("enum");            keywordsList.insert("return");
        keywordsList.insert("and_eq");      keywordsList.insert("explicit");        keywordsList.insert("short");
        keywordsList.insert("asm");         keywordsList.insert("export");          keywordsList.insert("signed");
        keywordsList.insert("auto");        keywordsList.insert("extern");          keywordsList.insert("sizeof");
        keywordsList.insert("bitand");      keywordsList.insert("false");           keywordsList.insert("static");
        keywordsList.insert("bitor");       keywordsList.insert("float");           keywordsList.insert("static_assert");
        keywordsList.insert("bool");        keywordsList.insert("for");             keywordsList.insert("static_cast");
        keywordsList.insert("break");       keywordsList.insert("friend");          keywordsList.insert("struct");
        keywordsList.insert("case");        keywordsList.insert("goto");            keywordsList.insert("switch");
        keywordsList.insert("catch");       keywordsList.insert("if");              keywordsList.insert("template");
        keywordsList.insert("char");        keywordsList.insert("inline");          keywordsList.insert("this");
        keywordsList.insert("char8_t");     keywordsList.insert("int");             keywordsList.insert("thread_local");
        keywordsList.insert("char16_t");    keywordsList.insert("long");            keywordsList.insert("throw");
        keywordsList.insert("char32_t");    keywordsList.insert("mutable");         keywordsList.insert("true");
        keywordsList.insert("class");       keywordsList.insert("namespace");       keywordsList.insert("try");
        keywordsList.insert("compl");       keywordsList.insert("new");             keywordsList.insert("typedef");
        keywordsList.insert("concept");     keywordsList.insert("noexcept");        keywordsList.insert("typeid");
        keywordsList.insert("const");       keywordsList.insert("not");             keywordsList.insert("typename");
        keywordsList.insert("consteval");   keywordsList.insert("not_eq");          keywordsList.insert("union");
        keywordsList.insert("constexpr");   keywordsList.insert("nullptr");         keywordsList.insert("unsigned");
        keywordsList.insert("const_cast");  keywordsList.insert("operator");        keywordsList.insert("using");
        keywordsList.insert("continue");    keywordsList.insert("or");              keywordsList.insert("virtual");
        keywordsList.insert("decltype");    keywordsList.insert("or_eq");           keywordsList.insert("void");
        keywordsList.insert("default");     keywordsList.insert("private");         keywordsList.insert("volatile");
        keywordsList.insert("delete");      keywordsList.insert("protected");       keywordsList.insert("wchar_t");
        keywordsList.insert("do");          keywordsList.insert("public");          keywordsList.insert("while");
        keywordsList.insert("double");      keywordsList.insert("register");        keywordsList.insert("xor");
        keywordsList.insert("xor_eq");

        return keywordsList;
    }

    set<string> createPreprocessorKeywordsSet()
    {
        set<string> preprocessorKeywordsList;
        preprocessorKeywordsList.insert("if");
        preprocessorKeywordsList.insert("elif");
        preprocessorKeywordsList.insert("else");
        preprocessorKeywordsList.insert("endif");
        preprocessorKeywordsList.insert("ifdef");
        preprocessorKeywordsList.insert("ifndef");
        preprocessorKeywordsList.insert("define");
        preprocessorKeywordsList.insert("undef");
        preprocessorKeywordsList.insert("include");
        preprocessorKeywordsList.insert("line");
        preprocessorKeywordsList.insert("error");
        preprocessorKeywordsList.insert("pragma");
        preprocessorKeywordsList.insert("defined");

        return preprocessorKeywordsList;
    }
}

const set<string> Lexer::keywordsSet = createKeywordsSet();
const set<string> Lexer::preprocessorKeywordsSet = createPreprocessorKeywordsSet();

bool is_space(char c)
{
    switch (c)
    {
        case ' ':
        case '\t':
        case '\r':
        case '\n':
        case '\v':
        case '\f':
            return true;
        default:
            return false;
    }
}

bool is_digit(char c)
{
    uint32_t number = static_cast<uint32_t>(c);
    if(number >= '0' && number <= '9')
        return true;
    else
        return false;
}

bool is_identifier_char(char c)
{
    uint32_t number = static_cast<uint32_t>(c);
    if((number >= 'A' && number <= 'Z') ||
       (number >= 'a' && number <= 'z') ||
       is_digit(c) || c == '_')
        return true;
    else
        return false;
}

Lexer::Lexer(shared_ptr<RowedFile> _rowedFile)
{
    rowedFilePtr = _rowedFile;
    rowedFilePtr->resetStringIndexPtr();
}

std::shared_ptr<list<Token>> Lexer::parse()
{
    std::shared_ptr<list<Token>> tokenList = make_shared<list<Token>>();

    for(Token token = next(); !token.is(Token::Kind::End); token = next())
        tokenList->push_back(token);

    return tokenList;
}

Token Lexer::next() noexcept
{
    while(is_space(rowedFilePtr->peek().c))
    {
        char c = rowedFilePtr->get().c;
        (void)c;
    }

    CharInfo cInfo = rowedFilePtr->get();
    switch (cInfo.c)
    {
        case '\0':
            return Token(Token::Kind::End, cInfo);
        default:
            return Token(Token::Kind::Unexpected, cInfo);
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
        case 'g':
        case 'h':
        case 'i':
        case 'j':
        case 'k':
        case 'l':
        case 'm':
        case 'n':
        case 'o':
        case 'p':
        case 'q':
        case 'r':
        case 's':
        case 't':
        case 'u':
        case 'v':
        case 'w':
        case 'x':
        case 'y':
        case 'z':
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
        case 'G':
        case 'H':
        case 'I':
        case 'J':
        case 'K':
        case 'L':
        case 'M':
        case 'N':
        case 'O':
        case 'P':
        case 'Q':
        case 'R':
        case 'S':
        case 'T':
        case 'U':
        case 'V':
        case 'W':
        case 'X':
        case 'Y':
        case 'Z':
        case '_':
            return name(cInfo);
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return number(cInfo);
        case '(':
            return Token(Token::Kind::LeftParen, cInfo);
        case ')':
            return Token(Token::Kind::RightParen, cInfo);
        case '[':
            return Token(Token::Kind::LeftSquare, cInfo);
        case ']':
            return Token(Token::Kind::RightSquare, cInfo);
        case '{':
            return Token(Token::Kind::LeftCurly, cInfo);
        case '}':
            return Token(Token::Kind::RightCurly, cInfo);
        case '<':
            return Token(Token::Kind::LessThan, cInfo);
        case '>':
            return Token(Token::Kind::GreaterThan, cInfo);
        case '=':
            return Token(Token::Kind::Equal, cInfo);
        case '+':
            return Token(Token::Kind::Plus, cInfo);
        case '-':
            return Token(Token::Kind::Minus, cInfo);
        case '*':
            return Token(Token::Kind::Asterisk, cInfo);
        case '/':
            return slashOrComment(cInfo);
        case '#':
            hashSymbol = true;
            return Token(Token::Kind::Hash, cInfo);
        case '.':
            return Token(Token::Kind::Dot, cInfo);
        case ',':
            return Token(Token::Kind::Comma, cInfo);
        case ':':
            return Token(Token::Kind::Colon, cInfo);
        case ';':
            return Token(Token::Kind::Semicolon, cInfo);
        case '\'':
            return Token(Token::Kind::SingleQuote, cInfo);
        case '"':
            return Token(Token::Kind::DoubleQuote, cInfo);
        case '|':
            return Token(Token::Kind::Pipe, cInfo);
        case '!':
            return Token(Token::Kind::Exclamation, cInfo);
        case '&':
            return Token(Token::Kind::Ampersand, cInfo);
        case '%':
            return Token(Token::Kind::Percent, cInfo);
        case '~':
            return Token(Token::Kind::Tylda, cInfo);
        case '?':
            return Token(Token::Kind::Question, cInfo);
        case '^':
            return Token(Token::Kind::Power, cInfo);
    }
}

Token Lexer::name(CharInfo cInfo) noexcept
{
    string name         {cInfo.c};
    uint32_t startLine  {cInfo.line};
    uint32_t startPos   {cInfo.pos};
    uint32_t endPos     {0};
    CharInfo newCInfo;

    while(is_identifier_char(rowedFilePtr->peek().c))
    {
        newCInfo = rowedFilePtr->get();
        name += newCInfo.c;
        endPos = newCInfo.pos;
    }

    if(hashSymbol)
    {
        hashSymbol = false;
        set<string>::iterator result = preprocessorKeywordsSet.find(name);
        if(result != preprocessorKeywordsSet.end())
            return Token(Token::Kind::PreprocessorKeyword, name, startLine, Token::Pos(startPos, endPos));
    }
    else
    {
        set<string>::iterator result = keywordsSet.find(name);
        if(result != keywordsSet.end())
            return Token(Token::Kind::Keyword, name, startLine, Token::Pos(startPos, endPos));
    }

    return Token(Token::Kind::Identifier, name, startLine, Token::Pos(startPos, endPos));
}

Token Lexer::number(CharInfo cInfo) noexcept
{
    string number       {cInfo.c};
    uint32_t startLine  {cInfo.line};
    uint32_t startPos   {cInfo.pos};
    uint32_t endPos     {0};
    CharInfo newCInfo;

    while(is_digit(rowedFilePtr->peek().c))
    {
        newCInfo = rowedFilePtr->get();
        number += newCInfo.c;
        endPos = newCInfo.pos;
    }

    return Token(Token::Kind::Number, number, startLine, Token::Pos(startPos, endPos));
}

Token Lexer::slashOrComment(CharInfo cInfo) noexcept
{
    char peek {rowedFilePtr->peek().c};
    if(peek == '/')
    {
        char c = rowedFilePtr->get().c;
        (void)c;
        while(rowedFilePtr->get().c != '\n');
        return next();
    }
    else if(peek == '*')
    {
        char c = rowedFilePtr->get().c;
        (void)c;
        bool loopBreaker {false};
        while(!loopBreaker)
        {
            while(rowedFilePtr->get().c != '*');
            c = rowedFilePtr->get().c;
            if(c == '/')
               loopBreaker = true;
        }

        return next();
    }
    else
        return Token(Token::Kind::Slash, cInfo);
}
