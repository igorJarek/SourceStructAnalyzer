#ifndef TEXTEXT_H
#define TEXTEXT_H

#include <SFML/Graphics.hpp>
#include <Typedefs.h>

using namespace std;
using namespace sf;

class TextExt : public sf::Drawable, public sf::Transformable
{
    public:
        TextExt(const string& text, const Font& font, uint32_t characterSize);
        ~TextExt();

        FloatRect getLocalBounds() const;
        FloatRect getGlobalBounds() const;

        void changeCharacterColor(const sf::Color& newColor, uint64_t position);
        void changeCharactersColor(const sf::Color& newColor, Pos position);

    private:
        virtual void draw(RenderTarget& target, RenderStates states) const;

        String          m_text;
        const Font*     m_font = nullptr;
        unsigned int    m_characterSize;
        FloatRect       m_bounds;
        VertexArray     m_vertices;
};

#endif // TEXTEXT_H
