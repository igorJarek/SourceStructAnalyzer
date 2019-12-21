/*
    Based on Juan Arrieta's Simple C++ Lexer
    https://gist.github.com/arrieta/1a309138689e09375b90b3b1aa768e20
*/

#ifndef LEXER2_H
#define LEXER2_H

#include <RowedFile.h>
#include <Token.h>
#include <TokenList.h>
#include <Typedefs.h>

#include <set>

bool is_space(char c);
bool is_digit(char c);
bool is_identifier_char(char c);

class Lexer
{
    public:
        Lexer(RowedFilePtr _rowedFile);
        Token next(void) noexcept;
        TokenList parse(void);

        const static set<string> keywordsSet;
        const static set<string> preprocessorKeywordsSet;

    private:
        RowedFilePtr rowedFile;

        bool hashSymbol {false};

        Token name(CharInfo cInfo) noexcept;
        Token number(CharInfo cInfo) noexcept;
        Token slashOrComment(CharInfo cInfo) noexcept;
};

#endif // LEXER2_H

