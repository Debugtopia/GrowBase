/*
 FText class by MrOnlineCoder
 
 Original code from SFML sources
*/

#ifndef FTEXT_H
#define FTEXT_H
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <string>
#include <vector>

using namespace sf;

////////////////////////////////////////////////////////////
/// \brief Graphical text that can be drawn to a render target
///
////////////////////////////////////////////////////////////
class FText : public Drawable, public Transformable
{
public:

    ////////////////////////////////////////////////////////////
    /// \brief Enumeration of the string drawing styles
    ///
    ////////////////////////////////////////////////////////////
    enum Style
    {
        Regular       = 0,      ///< Regular characters, no style
        Bold          = 1 << 0, ///< Bold characters
        Italic        = 1 << 1, ///< Italic characters
        Underlined    = 1 << 2, ///< Underlined characters
        StrikeThrough = 1 << 3  ///< Strike through characters
    };

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Creates an empty text.
    ///
    ////////////////////////////////////////////////////////////
    FText();

    ////////////////////////////////////////////////////////////
    /// \brief Construct the text from a string, font and size
    ///
    /// Note that if the used font is a bitmap font, it is not
    /// scalable, thus not all requested sizes will be available
    /// to use. This needs to be taken into consideration when
    /// setting the character size. If you need to display text
    /// of a certain size, make sure the corresponding bitmap
    /// font that supports that size is used.
    ///
    /// \param string         Text assigned to the string
    /// \param font           Font used to draw the string
    /// \param characterSize  Base size of characters, in pixels
    ///
    ////////////////////////////////////////////////////////////
    FText(const String& string, const Font& font, unsigned int characterSize = 30);

    ////////////////////////////////////////////////////////////
    /// \brief Set the text's string
    ///
    /// The \a string argument is a sf::String, which can
    /// automatically be constructed from standard string types.
    /// So, the following calls are all valid:
    /// \code
    /// text.setString("hello");
    /// text.setString(L"hello");
    /// text.setString(std::string("hello"));
    /// text.setString(std::wstring(L"hello"));
    /// \endcode
    /// A text's string is empty by default.
    ///
    /// \param string New string
    ///
    /// \see getString
    ///
    ////////////////////////////////////////////////////////////
    void setString(const String& string);

    ////////////////////////////////////////////////////////////
    /// \brief Set the text's font
    ///
    /// The \a font argument refers to a font that must
    /// exist as long as the text uses it. Indeed, the text
    /// doesn't store its own copy of the font, but rather keeps
    /// a pointer to the one that you passed to this function.
    /// If the font is destroyed and the text tries to
    /// use it, the behavior is undefined.
    ///
    /// \param font New font
    ///
    /// \see getFont
    ///
    ////////////////////////////////////////////////////////////
    void setFont(const Font& font);

    ////////////////////////////////////////////////////////////
    /// \brief Set the character size
    ///
    /// The default size is 30.
    ///
    /// Note that if the used font is a bitmap font, it is not
    /// scalable, thus not all requested sizes will be available
    /// to use. This needs to be taken into consideration when
    /// setting the character size. If you need to display text
    /// of a certain size, make sure the corresponding bitmap
    /// font that supports that size is used.
    ///
    /// \param size New character size, in pixels
    ///
    /// \see getCharacterSize
    ///
    ////////////////////////////////////////////////////////////
    void setCharacterSize(unsigned int size);

    ////////////////////////////////////////////////////////////
    /// \brief Set the text's style
    ///
    /// You can pass a combination of one or more styles, for
    /// example sf::Text::Bold | sf::Text::Italic.
    /// The default style is sf::Text::Regular.
    ///
    /// \param style New style
    ///
    /// \see getStyle
    ///
    ////////////////////////////////////////////////////////////
    void setStyle(Uint32 style);

    ////////////////////////////////////////////////////////////
    /// \brief Set the fill color of the text
    ///
    /// By default, the text's fill color is opaque white.
    /// Setting the fill color to a transparent color with an outline
    /// will cause the outline to be displayed in the fill area of the text.
    ///
    /// \param color New fill color of the text
    ///
    /// \see getFillColor
    ///
    /// \deprecated There is now fill and outline colors instead
    /// of a single global color.
    /// Use setFillColor(); or setOutlineColor(); instead.
    ///
    ////////////////////////////////////////////////////////////
    SFML_DEPRECATED void setColor(const sf::Color& color);

    ////////////////////////////////////////////////////////////
    /// \brief Set the fill color of the text
    ///
    /// By default, the text's fill color is opaque white.
    /// Setting the fill color to a transparent color with an outline
    /// will cause the outline to be displayed in the fill area of the text.
    ///
    /// \param color New fill color of the text
    ///
    /// \see getFillColor
    ///
    ////////////////////////////////////////////////////////////
    void setFillColor(const sf::Color& color);

    ////////////////////////////////////////////////////////////
    /// \brief Set the outline color of the text
    ///
    /// By default, the text's outline color is opaque black.
    ///
    /// \param color New outline color of the text
    ///
    /// \see getOutlineColor
    ///
    ////////////////////////////////////////////////////////////
    void setOutlineColor(const sf::Color& color);

    ////////////////////////////////////////////////////////////
    /// \brief Set the thickness of the text's outline
    ///
    /// By default, the outline thickness is 0.
    ///
    /// Be aware that using a negative value for the outline
    /// thickness will cause distorted rendering.
    ///
    /// \param thickness New outline thickness, in pixels
    ///
    /// \see getOutlineThickness
    ///
    ////////////////////////////////////////////////////////////
    void setOutlineThickness(float thickness);

    ////////////////////////////////////////////////////////////
    /// \brief Get the text's string
    ///
    /// The returned string is a sf::String, which can automatically
    /// be converted to standard string types. So, the following
    /// lines of code are all valid:
    /// \code
    /// sf::String   s1 = text.getString();;
    /// std::string  s2 = text.getString();;
    /// std::wstring s3 = text.getString();;
    /// \endcode
    ///
    /// \return Text's string
    ///
    /// \see setString
    ///
    ////////////////////////////////////////////////////////////
    const String& getString(); 

    ////////////////////////////////////////////////////////////
    /// \brief Get the text's font
    ///
    /// If the text has no font attached, a NULL pointer is returned.
    /// The returned pointer is const, which means that you
    /// cannot modify the font when you get it from this function.
    ///
    /// \return Pointer to the text's font
    ///
    /// \see setFont
    ///
    ////////////////////////////////////////////////////////////
    const Font* getFont(); 

    ////////////////////////////////////////////////////////////
    /// \brief Get the character size
    ///
    /// \return Size of the characters, in pixels
    ///
    /// \see setCharacterSize
    ///
    ////////////////////////////////////////////////////////////
    unsigned int getCharacterSize(); 

    ////////////////////////////////////////////////////////////
    /// \brief Get the text's style
    ///
    /// \return Text's style
    ///
    /// \see setStyle
    ///
    ////////////////////////////////////////////////////////////
    Uint32 getStyle(); 

    ////////////////////////////////////////////////////////////
    /// \brief Get the fill color of the text
    ///
    /// \return Fill color of the text
    ///
    /// \see setFillColor
    ///
    /// \deprecated There is now fill and outline colors instead
    /// of a single global color.
    /// Use getFillColor(); or getOutlineColor(); instead.
    ///
    ////////////////////////////////////////////////////////////
    SFML_DEPRECATED const sf::Color& getColor(); 

    ////////////////////////////////////////////////////////////
    /// \brief Get the fill color of the text
    ///
    /// \return Fill color of the text
    ///
    /// \see setFillColor
    ///
    ////////////////////////////////////////////////////////////
    const sf::Color& getFillColor(); 

    ////////////////////////////////////////////////////////////
    /// \brief Get the outline color of the text
    ///
    /// \return Outline color of the text
    ///
    /// \see setOutlineColor
    ///
    ////////////////////////////////////////////////////////////
    const sf::Color& getOutlineColor(); 

    ////////////////////////////////////////////////////////////
    /// \brief Get the outline thickness of the text
    ///
    /// \return Outline thickness of the text, in pixels
    ///
    /// \see setOutlineThickness
    ///
    ////////////////////////////////////////////////////////////
    float getOutlineThickness(); 

    ////////////////////////////////////////////////////////////
    /// \brief Return the position of the \a index-th character
    ///
    /// This function computes the visual position of a character
    /// from its index in the string. The returned position is
    /// in global coordinates (translation, rotation, scale and
    /// origin are applied).
    /// If \a index is out of range, the position of the end of
    /// the string is returned.
    ///
    /// \param index Index of the character
    ///
    /// \return Position of the character
    ///
    ////////////////////////////////////////////////////////////
    Vector2f findCharacterPos(std::size_t index);

    ////////////////////////////////////////////////////////////
    /// \brief Get the local bounding rectangle of the entity
    ///
    /// The returned rectangle is in local coordinates, which means
    /// that it ignores the transformations (translation, rotation,
    /// scale, ...) that are applied to the entity.
    /// In other words, this function returns the bounds of the
    /// entity in the entity's coordinate system.
    ///
    /// \return Local bounding rectangle of the entity
    ///
    ////////////////////////////////////////////////////////////
    FloatRect getLocalBounds(); 

    ////////////////////////////////////////////////////////////
    /// \brief Get the global bounding rectangle of the entity
    ///
    /// The returned rectangle is in global coordinates, which means
    /// that it takes into account the transformations (translation,
    /// rotation, scale, ...) that are applied to the entity.
    /// In other words, this function returns the bounds of the
    /// text in the global 2D world's coordinate system.
    ///
    /// \return Global bounding rectangle of the entity
    ///
    ////////////////////////////////////////////////////////////
    FloatRect getGlobalBounds(); 

  // get growtopia color for special color macro characters
  static sf::Color getColorForChar(char c);

private:

    ////////////////////////////////////////////////////////////
    /// \brief Draw the text to a render target
    ///
    /// \param target Render target to draw to
    /// \param states Current render states
    ///
    ////////////////////////////////////////////////////////////
    virtual void draw(RenderTarget& target, RenderStates states) const; 

    ////////////////////////////////////////////////////////////
    /// \brief Make sure the text's geometry is updated
    ///
    /// All the attributes related to rendering are cached, such
    /// that the geometry is only updated when necessary.
    ///
    ////////////////////////////////////////////////////////////
    void ensureGeometryUpdate() const;

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    String              m_string;             ///< String to display
    const Font*         m_font;               ///< Font used to display the string
    unsigned int        m_characterSize;      ///< Base size of characters, in pixels
    Uint32              m_style;              ///< Text style (see Style enum)
    sf::Color               m_fillColor;          ///< Text fill color
    sf::Color               m_outlineColor;       ///< Text outline color
    float               m_outlineThickness;   ///< Thickness of the text's outline
    mutable VertexArray m_vertices;           ///< Vertex array containing the fill geometry
    mutable VertexArray m_outlineVertices;    ///< Vertex array containing the outline geometry
    mutable FloatRect   m_bounds;             ///< Bounding rectangle of the text (in local coordinates)
    mutable bool        m_geometryNeedUpdate; ///< Does the geometry need to be recomputed?
};



#endif