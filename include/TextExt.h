#ifndef TEXTEXT_H
#define TEXTEXT_H

#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/String.hpp>
#include <string>
#include <vector>
#include <Typedefs.h>

using namespace sf;

class TextExt : public Drawable, public Transformable
{
    public:
        enum Style
        {
            Regular       = 0,      ///< Regular characters, no style
            Bold          = 1 << 0, ///< Bold characters
            Italic        = 1 << 1, ///< Italic characters
            Underlined    = 1 << 2, ///< Underlined characters
            StrikeThrough = 1 << 3  ///< Strike through characters
        };

        TextExt();
        TextExt(const String& string, const Font& font, unsigned int characterSize = 30);

        void setString(const String& string);
        void setFont(const Font& font);
        void setCharacterSize(unsigned int size);
        void setLineSpacing(float spacingFactor);
        void setLetterSpacing(float spacingFactor);
        void setStyle(Uint32 style);
        void setFillColor(const Color& color);
        void setOutlineColor(const Color& color);
        void setOutlineThickness(float thickness);

        void changeCharacterColor(const sf::Color& newColor, uint64_t position);
        void changeCharactersColor(const sf::Color& newColor, Pos position);

        const String& getString() const;
        const Font* getFont() const;
        unsigned int getCharacterSize() const;
        float getLetterSpacing() const;
        float getLineSpacing() const;
        Uint32 getStyle() const;
        const Color& getFillColor() const;
        const Color& getOutlineColor() const;
        float getOutlineThickness() const;
        Vector2f findCharacterPos(std::size_t index) const;
        FloatRect getLocalBounds() const;
        FloatRect getGlobalBounds() const;

    private:
        virtual void draw(RenderTarget& target, RenderStates states) const;
        void ensureGeometryUpdate() const;

        String              m_string;              ///< String to display
        const Font*         m_font;                ///< Font used to display the string
        unsigned int        m_characterSize;       ///< Base size of characters, in pixels
        float               m_letterSpacingFactor; ///< Spacing factor between letters
        float               m_lineSpacingFactor;   ///< Spacing factor between lines
        Uint32              m_style;               ///< Text style (see Style enum)
        Color               m_fillColor;           ///< Text fill color
        Color               m_outlineColor;        ///< Text outline color
        float               m_outlineThickness;    ///< Thickness of the text's outline
        mutable VertexArray m_vertices;            ///< Vertex array containing the fill geometry
        mutable VertexArray m_outlineVertices;     ///< Vertex array containing the outline geometry
        mutable FloatRect   m_bounds;              ///< Bounding rectangle of the text (in local coordinates)
        mutable bool        m_geometryNeedUpdate;  ///< Does the geometry need to be recomputed?
    };

#endif // TEXTEXT_H
