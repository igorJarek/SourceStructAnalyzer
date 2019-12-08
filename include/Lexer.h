#ifndef LEXER_H
#define LEXER_H

#include <RowedFile.h>
#include <Token.h>
#include <set>
#include <memory>

bool is_space(char c);
bool is_digit(char c);
bool is_identifier_char(char c);

class Lexer
{
    public:
        Lexer(RowedFile& _rowedFile);
        Token next() noexcept;
        std::shared_ptr<list<Token>> parse();

        const static set<string> keywordsSet;
        const static set<string> preprocessorKeywordsSet;

    private:
        RowedFile rowedFile;

        bool hashSymbol {false};

        Token name(CharInfo cInfo) noexcept;
        Token number(CharInfo cInfo) noexcept;
        Token slashOrComment(CharInfo cInfo) noexcept;
};

#endif // LEXER_H
