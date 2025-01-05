#include <BaseApp.h> // precompiled

#include <GrowRender/RenderCache.h>



GrowRenderCache g_renderCache;
GrowRenderCache * GetGrowCache() { return &g_renderCache; }


RTFont GrowRenderCache::GetFont(eFontTypes font)
{
    switch (font)
    {
        case eFontTypes::SMALL_FONT:
            return m_smallFont;

        case eFontTypes::BIG_FONT:
            return m_bigFont;
    }
}

void GrowRenderCache::SetActiveFile(RTTEX& tex, const std::string& fName, const bool& bUseIconTex)
{
    nova_str texture_name = fName;
    bool bDirectoryFound = fName.find('/') != std::string::npos;
    if (bDirectoryFound == false)
    {
        // like client does when reading textures, we add game/ to texture name to seperate textures with their proper directories
        texture_name = "game/" + texture_name + "";
    }

    // setting the texture
    auto it = m_textures.find(texture_name);
    if (it == m_textures.end())
    {
        // texture not found
        tex = RTTEX();
        return;
    }

    tex = it->second;
}