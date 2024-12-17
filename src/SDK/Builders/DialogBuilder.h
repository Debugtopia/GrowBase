#ifndef DIALOGBUILDER_H
#define DIALOGBUILDER_H
#include <string>

enum class eDialogElementSizes
{
    EXTRASMALL,
    SMALL,
    MEDIUM,
    LARGE,
    BIG
};

enum class eDialogElementDirections
{
    LEFT,
    RIGHT,
    CENTER
};

class GameDialog
{
public:
    GameDialog() = default;
    ~GameDialog() = default;

    // get
    std::string Build() const { return m_menu; }

    std::string GetSizeAsString(eDialogElementSizes size)
    {
        switch (size)
        {
            case eDialogElementSizes::EXTRASMALL:
                return "extra_small";
            case eDialogElementSizes::SMALL:
                return "small";
            case eDialogElementSizes::MEDIUM:
                return "medium";
            case eDialogElementSizes::LARGE:
                return "large";
            case eDialogElementSizes::BIG:
                return "big";
        }

        return "small";
    }

    std::string GetDirectionAsString(eDialogElementDirections dir)
    {
        switch (dir)
        {
            case eDialogElementDirections::LEFT:
                return "left";
            case eDialogElementDirections::RIGHT:
                return "right";
            case eDialogElementDirections::CENTER:
                return "center";
        }

        return "left";
    }

    // set
    void Kill()
    {
        m_menu = "";
    }

    // fn
    GameDialog* SetDefaultColor(char c)
    {
        // set_default_color|`c

        m_menu.append("set_default_color|`" + std::string(1, c) + "\n");
        return this;
    }

    GameDialog* AddSpacer(const eDialogElementSizes& size)
    {
        // add_spacer|small
        // add_spacer|big

        m_menu.append("add_spacer|" + GetSizeAsString(size) + "|\n");
        return this;
    }

    GameDialog* AddLabelWithIcon(const eDialogElementSizes& size, const std::string& text, const eDialogElementDirections& direction, const int& iconItemID)
    {
        // add_label_with_icon|big|`wBig Text``|left|242
        // add_label_with_icon|small|`wSmall Text``|left|242

        m_menu.append("add_label_with_icon|" + GetSizeAsString(size) + "|" + text + "|" + GetDirectionAsString(direction) + "|" + std::to_string(iconItemID) + "\n");
        return this;
    }

    GameDialog* AddTextbox(const std::string& text)
    {
        // add_textbox|text|
        // ises FONT_NORMAL on the client side
        m_menu.append("add_textbox|" + text + "|left|\n");
        return this;
    }

    GameDialog* AddSmallText(const std::string& smallText)
    {
        // add_smalltext|small_text|
        // uses FONT_SMALL on the client side

        m_menu.append("add_smalltext|" + smallText + "|left|\n");
        return this;
    }

    GameDialog* AddButton(const std::string& buttonName, const std::string& buttonText)
    {
        // add_button|button_name|button_text|noflags|0|0|

        m_menu.append("add_button|" + buttonName + "|" + buttonText + "|noflags|0|0|\n");
        return this;
    }

    GameDialog* AddURLButton(const std::string& buttonName, const std::string& buttonText, const std::string& url, const std::string& description)
    {
        // add_url_button|button_name|button_text|noflags|url|description|0|0|

        m_menu.append("add_url_button|" + buttonName + "|" + buttonText + "|noflags|" + url + "|" + description + "|0|0|\n");
        return this;
    }

    GameDialog* AddCheckbox(const std::string& checkName, const std::string& checkText, const bool& bEnabled)
    {
        // add_checkbox|check_name|check_text|0 or 1

        m_menu.append("add_checkbox|" + checkName + "|" + checkText + "|" + (bEnabled ? "1" : "0") + "\n");
        return this;
    }

    GameDialog* EndDialog(const std::string& dialogName, const std::string& yellowButtonText, const std::string& whiteButtonText)
    {
        // end_dialog|dialog_name|yellow_button_text|white_button_text

        m_menu.append("end_dialog|" + dialogName + "|" + yellowButtonText + "|" + whiteButtonText + "|\n");
        return this;
    }

    GameDialog* EmbedData(const std::string& embedTitle, const std::string& embedValue)
    {
        // embed_data|embed_title|embed_value
        // embed_data|tileX|100
        // embed_data|tileY|60
        // not visible on the client side, used in server side as a verify elements

        m_menu.append("embed_data|" + embedTitle + "|" + embedValue + "\n");
        return this;
    }

    GameDialog* AddItemPicker(const std::string& pickerName, const std::string& buttonText, const std::string& extraText)
    {
        // add_item_picker|picker_name|button_text|extra_text|
        // used for stuff that allows displaying items inside(donation boxes, art canvases, item suckers, ...)

        m_menu.append("add_item_picker|" + pickerName + "|" + buttonText + "|" + extraText + "|\n");
        return this;
    }

    GameDialog* AddTextInput(const std::string& inputName, const std::string& inputText, const std::string& inputInsideText, const int& inputLength)
    {
        // add_text_input|input_name|input_text|input_inside_text|input_length
        // used for labels, notes, ...

        m_menu.append("add_text_input|" + inputName + "|" + inputText + "|" + inputInsideText + "|" + std::to_string(inputLength) + "|\n");
        return this;
    }

private:
    std::string m_menu = "";

};

#endif DIALOGBUILDER_H