#ifndef WORLDOFFERSBUILDER_H
#define WORLDOFFERSBUILDER_H
#include <string>

class WorldOffersMenu
{
public:
    WorldOffersMenu() = default;
    ~WorldOffersMenu() = default;

    // get
    std::string Build() const { return m_menu; }

    // set
    void Kill()
    {
        m_menu = "";
    }

    // fn
    WorldOffersMenu* AddFloater(const std::string& text, int player_count, double scale, unsigned int color)
    {
        m_menu.append("add_floater|" + text + "|" + std::to_string(player_count) + "|" + std::to_string(scale) + "|" + std::to_string(color) + "\n");
        return this;
    }

    WorldOffersMenu* AddButton(const std::string& text, const std::string& name, double scale, unsigned int color)
    {
        m_menu.append("add_button|" + text + "|" + name + "|" + std::to_string(scale) + "|" + std::to_string(color) + "\n");
        return this;
    }

    WorldOffersMenu* AddSpacer()
    {
        m_menu.append("add_spacer|\n");
        return this;
    }

    WorldOffersMenu* SetDefault(const std::string& worldName)
    {
        m_menu.append("default|" + worldName + "\n");
        return this;
    }

    WorldOffersMenu* AddHeading(const std::string& text)
    {
        m_menu.append("add_heading|" + text + "\n");
        return this;
    }

    WorldOffersMenu* AddFilter()
    {
        m_menu.append("add_filter\n");
        return this;
    }

    WorldOffersMenu* SetMaxRows(const int& maxRows)
    {
        m_menu.append("set_max_rows|" + std::to_string(maxRows) + "\n");
        return this;
    }

    WorldOffersMenu* SetupSimpleMenu()
    {
        m_menu.append("setup_simple_menu\n");
        return this;
    }

private:
    std::string m_menu = "";

};

#endif WORLDOFFERSBUILDER_H