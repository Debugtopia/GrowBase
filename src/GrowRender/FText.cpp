#include <BaseApp.h> // precompiled
#include <GrowRender/FText.h>

#include <cmath>


    // Add an underline or strikethrough line to the vertex array
    void addLine(sf::VertexArray& vertices, float lineLength, float lineTop, const sf::Color& color, float offset, float thickness, float outlineThickness = 0)
    {
        float top = std::floor(lineTop + offset - (thickness / 2) + 0.5f);
        float bottom = top + std::floor(thickness + 0.5f);

        vertices.append(sf::Vertex(sf::Vector2f(-outlineThickness,             top    - outlineThickness), color, sf::Vector2f(1, 1)));
        vertices.append(sf::Vertex(sf::Vector2f(lineLength + outlineThickness, top    - outlineThickness), color, sf::Vector2f(1, 1)));
        vertices.append(sf::Vertex(sf::Vector2f(-outlineThickness,             bottom + outlineThickness), color, sf::Vector2f(1, 1)));
        vertices.append(sf::Vertex(sf::Vector2f(-outlineThickness,             bottom + outlineThickness), color, sf::Vector2f(1, 1)));
        vertices.append(sf::Vertex(sf::Vector2f(lineLength + outlineThickness, top    - outlineThickness), color, sf::Vector2f(1, 1)));
        vertices.append(sf::Vertex(sf::Vector2f(lineLength + outlineThickness, bottom + outlineThickness), color, sf::Vector2f(1, 1)));
    }

    // Add a glyph quad to the vertex array
    void addGlyphQuad(sf::VertexArray& vertices, sf::Vector2f position, const sf::Color& color, const sf::Glyph& glyph, float italic, float outlineThickness = 0)
    {
        float left   = glyph.bounds.left;
        float top    = glyph.bounds.top;
        float right  = glyph.bounds.left + glyph.bounds.width;
        float bottom = glyph.bounds.top  + glyph.bounds.height;

        float u1 = static_cast<float>(glyph.textureRect.left);
        float v1 = static_cast<float>(glyph.textureRect.top);
        float u2 = static_cast<float>(glyph.textureRect.left + glyph.textureRect.width);
        float v2 = static_cast<float>(glyph.textureRect.top  + glyph.textureRect.height);

        vertices.append(sf::Vertex(sf::Vector2f(position.x + left  - italic * top    - outlineThickness, position.y + top    - outlineThickness), color, sf::Vector2f(u1, v1)));
        vertices.append(sf::Vertex(sf::Vector2f(position.x + right - italic * top    - outlineThickness, position.y + top    - outlineThickness), color, sf::Vector2f(u2, v1)));
        vertices.append(sf::Vertex(sf::Vector2f(position.x + left  - italic * bottom - outlineThickness, position.y + bottom - outlineThickness), color, sf::Vector2f(u1, v2)));
        vertices.append(sf::Vertex(sf::Vector2f(position.x + left  - italic * bottom - outlineThickness, position.y + bottom - outlineThickness), color, sf::Vector2f(u1, v2)));
        vertices.append(sf::Vertex(sf::Vector2f(position.x + right - italic * top    - outlineThickness, position.y + top    - outlineThickness), color, sf::Vector2f(u2, v1)));
        vertices.append(sf::Vertex(sf::Vector2f(position.x + right - italic * bottom - outlineThickness, position.y + bottom - outlineThickness), color, sf::Vector2f(u2, v2)));
    }



////////////////////////////////////////////////////////////
FText::FText() :
m_string            (),
m_font              (NULL),
m_characterSize     (30),
m_style             (Regular),
m_fillColor         (255, 255, 255),
m_outlineColor      (0, 0, 0),
m_outlineThickness  (0),
m_vertices          (Triangles),
m_outlineVertices   (Triangles),
m_bounds            (),
m_geometryNeedUpdate(false)
{

}


////////////////////////////////////////////////////////////
FText::FText(const String& string, const Font& font, unsigned int characterSize) :
m_string            (string),
m_font              (&font),
m_characterSize     (characterSize),
m_style             (Regular),
m_fillColor         (255, 255, 255),
m_outlineColor      (0, 0, 0),
m_outlineThickness  (0),
m_vertices          (Triangles),
m_outlineVertices   (Triangles),
m_bounds            (),
m_geometryNeedUpdate(true)
{

}


////////////////////////////////////////////////////////////
void FText::setString(const String& string)
{
    if (m_string != string)
    {
        m_string = string;
        m_geometryNeedUpdate = true;
    }
}


////////////////////////////////////////////////////////////
void FText::setFont(const Font& font)
{
    if (m_font != &font)
    {
        m_font = &font;
        m_geometryNeedUpdate = true;
    }
}


////////////////////////////////////////////////////////////
void FText::setCharacterSize(unsigned int size)
{
    if (m_characterSize != size)
    {
        m_characterSize = size;
        m_geometryNeedUpdate = true;
    }
}


////////////////////////////////////////////////////////////
void FText::setStyle(Uint32 style)
{
    if (m_style != style)
    {
        m_style = style;
        m_geometryNeedUpdate = true;
    }
}


////////////////////////////////////////////////////////////
void FText::setColor(const sf::Color& color)
{
    setFillColor(color);
}


////////////////////////////////////////////////////////////
void FText::setFillColor(const sf::Color& color)
{
    if (color != m_fillColor)
    {
        m_fillColor = color;

        // Change vertex colors directly, no need to update whole geometry
        // (if geometry is updated anyway, we can skip this step)
        if (!m_geometryNeedUpdate)
        {
            for (std::size_t i = 0; i < m_vertices.getVertexCount(); ++i)
                m_vertices[i].color = m_fillColor;
        }
    }
}


////////////////////////////////////////////////////////////
void FText::setOutlineColor(const sf::Color& color)
{
    if (color != m_outlineColor)
    {
        m_outlineColor = color;

        // Change vertex colors directly, no need to update whole geometry
        // (if geometry is updated anyway, we can skip this step)
        if (!m_geometryNeedUpdate)
        {
            for (std::size_t i = 0; i < m_outlineVertices.getVertexCount(); ++i)
                m_outlineVertices[i].color = m_outlineColor;
        }
    }
}


////////////////////////////////////////////////////////////
void FText::setOutlineThickness(float thickness)
{
    if (thickness != m_outlineThickness)
    {
        m_outlineThickness = thickness;
        m_geometryNeedUpdate = true;
    }
}


////////////////////////////////////////////////////////////
const String& FText::getString()
{
    return m_string;
}


////////////////////////////////////////////////////////////
const Font* FText::getFont()
{
    return m_font;
}


////////////////////////////////////////////////////////////
unsigned int FText::getCharacterSize()
{
    return m_characterSize;
}


////////////////////////////////////////////////////////////
Uint32 FText::getStyle()
{
    return m_style;
}


////////////////////////////////////////////////////////////
const sf::Color& FText::getColor()
{
    return getFillColor();
}


////////////////////////////////////////////////////////////
const sf::Color& FText::getFillColor()
{
    return m_fillColor;
}


////////////////////////////////////////////////////////////
const sf::Color& FText::getOutlineColor()
{
    return m_outlineColor;
}


////////////////////////////////////////////////////////////
float FText::getOutlineThickness()
{
    return m_outlineThickness;
}


////////////////////////////////////////////////////////////
Vector2f FText::findCharacterPos(std::size_t index)
{
    // Make sure that we have a valid font
    if (!m_font)
        return Vector2f();

    // Adjust the index if it's out of range
    if (index > m_string.getSize())
        index = m_string.getSize();

    // Precompute the variables needed by the algorithm
    bool  bold   = (m_style & Bold) != 0;
    float hspace = static_cast<float>(m_font->getGlyph(L' ', m_characterSize, bold).advance);
    float vspace = static_cast<float>(m_font->getLineSpacing(m_characterSize));

    // Compute the position
    Vector2f position;
    Uint32 prevChar = 0;
    for (std::size_t i = 0; i < index; ++i)
    {
        Uint32 curChar = m_string[i];

        // Apply the kerning offset
        position.x += static_cast<float>(m_font->getKerning(prevChar, curChar, m_characterSize));
        prevChar = curChar;

        // Handle special characters
        switch (curChar)
        {
            case ' ':  position.x += hspace;                 continue;
            case '\t': position.x += hspace * 4;             continue;
            case '\n': position.y += vspace; position.x = 0; continue;
        }

        // For regular characters, add the advance offset of the glyph
        position.x += static_cast<float>(m_font->getGlyph(curChar, m_characterSize, bold).advance);
    }

    // Transform the position to global coordinates
    position = getTransform().transformPoint(position);

    return position;
}


////////////////////////////////////////////////////////////
FloatRect FText::getLocalBounds()
{
    ensureGeometryUpdate();

    return m_bounds;
}


////////////////////////////////////////////////////////////
FloatRect FText::getGlobalBounds()
{
    return getTransform().transformRect(getLocalBounds());
}


////////////////////////////////////////////////////////////
void FText::draw(RenderTarget& target, RenderStates states) const
{
    if (m_font)
    {
        ensureGeometryUpdate();

        states.transform *= getTransform();
        states.texture = &m_font->getTexture(m_characterSize);

        // Only draw the outline if there is something to draw
        if (m_outlineThickness != 0)
            target.draw(m_outlineVertices, states);

        target.draw(m_vertices, states);
    }
}


void FText::ensureGeometryUpdate() const
{
    // Do nothing, if geometry has not changed
    if (!m_geometryNeedUpdate)
        return;

    // Mark geometry as updated
    m_geometryNeedUpdate = false;

    // Clear the previous geometry
    m_vertices.clear();
    m_outlineVertices.clear();
    m_bounds = FloatRect();


    // No font or FText: nothing to draw
    if (!m_font || m_string.isEmpty())
        return;

    // Compute values related to the FText style
    bool  bold               = (m_style & Bold) != 0;
    bool  underlined         = (m_style & Underlined) != 0;
    bool  strikeThrough      = (m_style & StrikeThrough) != 0;
    float italic             = (m_style & Italic) ? 0.208f : 0.f; // 12 degrees
    float underlineOffset    = m_font->getUnderlinePosition(m_characterSize);
    float underlineThickness = m_font->getUnderlineThickness(m_characterSize);

    // Compute the location of the strike through dynamically
    // We use the center point of the lowercase 'x' glyph as the reference
    // We reuse the underline thickness as the thickness of the strike through as well
    FloatRect xBounds = m_font->getGlyph(L'x', m_characterSize, bold).bounds;
    float strikeThroughOffset = xBounds.top + xBounds.height / 2.f;

    // Precompute the variables needed by the algorithm
    float hspace = static_cast<float>(m_font->getGlyph(L' ', m_characterSize, bold).advance);
    float vspace = static_cast<float>(m_font->getLineSpacing(m_characterSize));
    float x      = 0.f;
    float y      = static_cast<float>(m_characterSize);

    // Create one quad for each character
    float minX = static_cast<float>(m_characterSize);
    float minY = static_cast<float>(m_characterSize);
    float maxX = 0.f;
    float maxY = 0.f;
    Uint32 prevChar = 0;

	sf::Color fcolor = m_fillColor;

    for (std::size_t i = 0; i < m_string.getSize(); ++i)
    {
        Uint32 curChar = m_string[i];

        // Apply the kerning offset
        x += m_font->getKerning(prevChar, curChar, m_characterSize);
        prevChar = curChar;

        // If we're using the underlined style and there's a new line, draw a line
        if (underlined && (curChar == L'\n'))
        {
            addLine(m_vertices, x, y, m_fillColor, underlineOffset, underlineThickness);

            if (m_outlineThickness != 0)
                addLine(m_outlineVertices, x, y, m_outlineColor, underlineOffset, underlineThickness, m_outlineThickness);
        }

        // If we're using the strike through style and there's a new line, draw a line across all characters
        if (strikeThrough && (curChar == L'\n'))
        {
            addLine(m_vertices, x, y, m_fillColor, strikeThroughOffset, underlineThickness);

            if (m_outlineThickness != 0)
                addLine(m_outlineVertices, x, y, m_outlineColor, strikeThroughOffset, underlineThickness, m_outlineThickness);
        }

        // Handle special characters
        if ((curChar == ' ') || (curChar == '\t') || (curChar == '\n'))
        {
            // Update the current bounds (min coordinates)
            minX = std::min(minX, x);
            minY = std::min(minY, y);

            switch (curChar)
            {
                case ' ':  x += hspace;        break;
                case '\t': x += hspace * 4;    break;
                case '\n': y += vspace; x = 0; break;
            }

            // Update the current bounds (max coordinates)
            maxX = std::max(maxX, x);
            maxY = std::max(maxY, y);

            // Next glyph, no need to create a quad for whitespace
            continue;
        }

		/*
			CUSTOM CODE FROM MrOnlineCoder
		*/

		if (curChar == '`') 
        {
			i++;

			char colorChar = m_string[i];

			if (colorChar != '`') 
            {
				fcolor = FText::getColorForChar(colorChar);
				continue;
			}
		}

        
        // Apply the outline
        if (m_outlineThickness != 0)
        {
            const Glyph& glyph = m_font->getGlyph(curChar, m_characterSize, bold, m_outlineThickness);

            float left   = glyph.bounds.left;
            float top    = glyph.bounds.top;
            float right  = glyph.bounds.left + glyph.bounds.width;
            float bottom = glyph.bounds.top  + glyph.bounds.height;

            // Add the outline glyph to the vertices
            addGlyphQuad(m_outlineVertices, Vector2f(x, y), m_outlineColor, glyph, italic, m_outlineThickness);

            // Update the current bounds with the outlined glyph bounds
            minX = std::min(minX, x + left   - italic * bottom - m_outlineThickness);
            maxX = std::max(maxX, x + right  - italic * top    - m_outlineThickness);
            minY = std::min(minY, y + top    - m_outlineThickness);
            maxY = std::max(maxY, y + bottom - m_outlineThickness);
        }

        // Extract the current glyph's description
        const Glyph& glyph = m_font->getGlyph(curChar, m_characterSize, bold);

        // Add the glyph to the vertices
        addGlyphQuad(m_vertices, Vector2f(x, y), fcolor, glyph, italic);

        // Update the current bounds with the non outlined glyph bounds
        if (m_outlineThickness == 0)
        {
            float left   = glyph.bounds.left;
            float top    = glyph.bounds.top;
            float right  = glyph.bounds.left + glyph.bounds.width;
            float bottom = glyph.bounds.top  + glyph.bounds.height;

            minX = std::min(minX, x + left  - italic * bottom);
            maxX = std::max(maxX, x + right - italic * top);
            minY = std::min(minY, y + top);
            maxY = std::max(maxY, y + bottom);
        }

        // Advance to the next character
        x += glyph.advance;
    }

    // If we're using the underlined style, add the last line
    if (underlined && (x > 0))
    {
        addLine(m_vertices, x, y, fcolor, underlineOffset, underlineThickness);

        if (m_outlineThickness != 0)
            addLine(m_outlineVertices, x, y, m_outlineColor, underlineOffset, underlineThickness, m_outlineThickness);
    }

    // If we're using the strike through style, add the last line across all characters
    if (strikeThrough && (x > 0))
    {
        addLine(m_vertices, x, y, fcolor, strikeThroughOffset, underlineThickness);

        if (m_outlineThickness != 0)
            addLine(m_outlineVertices, x, y, m_outlineColor, strikeThroughOffset, underlineThickness, m_outlineThickness);
    }

    // Update the bounding rectangle
    m_bounds.left = minX;
    m_bounds.top = minY;
    m_bounds.width = maxX - minX;
    m_bounds.height = maxY - minY;
}

// this is for growtopia color characters
sf::Color FText::getColorForChar(char c) {
	switch (c)
	{
        case '`':
            return sf::Color(255,255,255);
		case '0':
			return sf::Color(255, 255, 255);
		case '1':
			return sf::Color(173, 244, 255);
		case '2':
			return sf::Color(73, 252, 0);
		case '3':
			return sf::Color(191,218,255);
		case '4':
			return sf::Color(255,39,29);
		case '5':
			return sf::Color(235,183,255);
		case '6':
			return sf::Color(255,202,111);
		case '7':
			return sf::Color(230,230,230);
        case '8':
			return sf::Color(255,148,69);
        case '9':
			return sf::Color(255,238,125);
        case '!':
			return sf::Color(209,255,249);
        case '@':
			return sf::Color(255,205,201);
        case '#':
			return sf::Color(255,143,243);
        case '$':
			return sf::Color(255,252,197);
        case '^':
			return sf::Color(181,255,151);
        case '&':
			return sf::Color(254,235,255);
        case 'w':
			return sf::Color(255, 255,255);
        case 'o':
			return sf::Color(252,230,186);
        case 'p':
			return sf::Color(255,223,241);
        case 'b':
			return sf::Color(0, 0,0);
        case 'q':
			return sf::Color(12,96,164);
        case 'e':
			return sf::Color(25,185,255);
        case 'r':
			return sf::Color(111,211,87);
        case 't':
			return sf::Color(47,131,13);
        case 'a':
			return sf::Color(81,81,81);
        case 's':
			return sf::Color(158,158,158);
        case 'c':
			return sf::Color(80,255,255);
		default:
			return sf::Color(255,255,255);
	}
}