#ifndef RENDERCACHE_H
#define RENDERCACHE_H
#include <string>
#include <unordered_map>
#include <SFML/Graphics.hpp>



// ok, some kid will be on my dick for this, but shut the fuck up, it's not bad naming you're just retarded faggot that gives out hypocritism - hexago
using RTFont = sf::Font;
using RTTEX = sf::Texture;


enum class eFontTypes
{
    SMALL_FONT, // Century Gothic
    BIG_FONT // Century Gothic Bold
};


class GrowRenderCache
{
public:
    GrowRenderCache() = default;
    ~GrowRenderCache() = default;


    // get
    std::unordered_map<std::string, RTTEX>       GetTextures() const { return m_textures; }
    RTFont                                       GetFont(eFontTypes font);          

    // set


    // fn
    void                                         SetActiveFile(RTTEX& tex, const std::string& fName, const bool& bUseIconTex = false);

private:
    RTFont                                       m_smallFont;
    RTFont                                       m_bigFont;

    std::unordered_map<std::string, RTTEX>       m_textures;
};

GrowRenderCache * GetGrowCache();

#endif RENDERCACHE_H