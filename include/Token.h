/*
    Based on Juan Arrieta's Simple C++ Lexer
    https://gist.github.com/arrieta/1a309138689e09375b90b3b1aa768e20
*/

#ifndef TOKEN_H
#define TOKEN_H

#include <RowedFile.h> // CharInfo structure
#include <Typedefs.h>

class Token
{
    public:
        enum class Kind
        {
            Number,
            Identifier,
            LeftParen,
            RightParen,
            LeftSquare,
            RightSquare,
            LeftCurly,
            RightCurly,
            LessThan,
            GreaterThan,
            Equal,
            Plus,
            Minus,
            Asterisk,
            Slash,
            Hash,
            Dot,
            Comma,
            Colon,
            Semicolon,
            SingleQuote,
            DoubleQuote,
            Comment,
            Pipe,
            Exclamation,
            Ampersand,
            Percent,
            Keyword,
            PreprocessorKeyword,
            Tylda,
            Question,
            Power,
            Backslash,
            Unexpected,
            End
        };

        Token() = default;
        Token(Kind k, CharInfo cInfo) noexcept : m_kind{k}
        {
            m_lexeme += cInfo.c;
            m_line = cInfo.line;
            m_pos = Pos(cInfo.pos, cInfo.pos);
        }

        Token(Kind k, string& lex, uint32_t line, Pos pos) noexcept : m_kind{k}, m_line(line), m_pos(pos) { m_lexeme = move(lex); }

        Kind kind() const noexcept { return m_kind; }
        void kind(Kind k) noexcept { m_kind = k; }

        bool is(Kind k) const noexcept { return m_kind == k; }
        bool is_not(Kind k) const noexcept { return m_kind != k; }
        bool is_one_of(Kind k1, Kind k2) const noexcept { return is(k1) || is(k2); }

        template <typename... Ts>
        bool is_one_of(Kind k1, Kind k2, Ts... ks) const noexcept
        {
            return is(k1) || is_one_of(k2, ks...);
        }

        std::string lexeme() const noexcept { return m_lexeme; }

        uint32_t line() const { return m_line; }
        Pos pos() const { return m_pos; }

    private:
        Kind         m_kind{};
        std::string  m_lexeme{};
        uint32_t     m_line{};
        Pos          m_pos;
};

#endif // TOKEN_H
