#include "TextExt.h"

TextExt::TextExt(const string& text, const Font& font, unsigned int characterSize) :
    m_text(text), m_font(&font), m_characterSize(characterSize), m_vertices(Quads)
{
    float whitespaceWidth   = m_font->getGlyph(L' ', m_characterSize, false /* isBold */).advance;
    float x                 = 0.f;
    float y                 = static_cast<float>(m_characterSize);
    uint32_t prevChar       = 0;

    float minX = static_cast<float>(m_characterSize);
    float minY = static_cast<float>(m_characterSize);
    float maxX = 0.f;
    float maxY = 0.f;

    for(uint32_t curChar : m_text)
    {
        if(curChar == L'\r')
            continue;

        if(curChar == L'\n')
            break;

        x += m_font->getKerning(prevChar, curChar, m_characterSize);

        prevChar = curChar;

        if ((curChar == L' ') || (curChar == L'\t'))
        {
            minX = min(minX, x);
            minY = min(minY, y);

            switch (curChar)
            {
                case L' ':  x += whitespaceWidth;     break;
                case L'\t': x += whitespaceWidth * 4; break;
            }

            maxX = max(maxX, x);
            maxY = max(maxY, y);

            continue;
        }

        const Glyph& glyph = m_font->getGlyph(curChar, m_characterSize, false /* isBold */);

        float left   = glyph.bounds.left;
        float top    = glyph.bounds.top;
        float right  = glyph.bounds.left + glyph.bounds.width;
        float bottom = glyph.bounds.top  + glyph.bounds.height;

        float u1 = static_cast<float>(glyph.textureRect.left);
        float v1 = static_cast<float>(glyph.textureRect.top);
        float u2 = static_cast<float>(glyph.textureRect.left + glyph.textureRect.width);
        float v2 = static_cast<float>(glyph.textureRect.top  + glyph.textureRect.height);

        m_vertices.append(sf::Vertex(
                                     sf::Vector2f(x + left, y + top),
                                     sf::Color::White,
                                     sf::Vector2f(u1, v1)
                                     ));
        m_vertices.append(sf::Vertex(
                                     sf::Vector2f(x + right, y + top),
                                     sf::Color::White,
                                     sf::Vector2f(u2, v1)
                                     ));
        m_vertices.append(sf::Vertex(
                                     sf::Vector2f(x + right, y + bottom),
                                     sf::Color::White,
                                     sf::Vector2f(u2, v2)
                                     ));
        m_vertices.append(sf::Vertex(
                                     sf::Vector2f(x + left, y + bottom),
                                     sf::Color::White,
                                     sf::Vector2f(u1, v2)
                                     ));

        minX = std::min(minX, x + left);
        maxX = std::max(maxX, x + right);
        minY = std::min(minY, y + top);
        maxY = std::max(maxY, y + bottom);

        x += glyph.advance;
    }

    m_bounds.left = minX;
    m_bounds.top = minY;
    m_bounds.width = maxX - minX;
    m_bounds.height = maxY - minY;
}

TextExt::~TextExt()
{
    //dtor
}

void TextExt::changeCharacterColor(const sf::Color& newColor, uint64_t position)
{
    uint64_t startIndex {position * 4};
    if(startIndex >= m_vertices.getVertexCount())
        return;

    m_vertices[startIndex+0].color = newColor;
    m_vertices[startIndex+1].color = newColor;
    m_vertices[startIndex+2].color = newColor;
    m_vertices[startIndex+3].color = newColor;
}

void TextExt::changeCharactersColor(const sf::Color& newColor, Pos position)
{
    for(uint64_t startIndex {position.first}; startIndex <= position.second; ++startIndex)
    {
        uint64_t index = startIndex * 4;
        if(index >= m_vertices.getVertexCount())
            return;

        m_vertices[index+0].color = newColor;
        m_vertices[index+1].color = newColor;
        m_vertices[index+2].color = newColor;
        m_vertices[index+3].color = newColor;
    }
}

FloatRect TextExt::getLocalBounds() const
{
    return m_bounds;
}

FloatRect TextExt::getGlobalBounds() const
{
    return getTransform().transformRect(getLocalBounds());
}

void TextExt::draw(RenderTarget& target, RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = &m_font->getTexture(m_characterSize);

    target.draw(m_vertices, states);
}
