#include <BaseApp.h> // precompiled
#include <map>
#include <format>

#include <SDK/Proton/TextScanner.h>
#include <SDK/Proton/MiscUtils.h>
#include <SDK/Proton/FileSystem/FileManager.h>

ItemInfoManager g_itemsManager;
ItemInfoManager* GetItemInfoManager() { return &g_itemsManager; }


ItemInfoManager::~ItemInfoManager()
{
    for (int i = 0; i < m_items.size(); i++)
    {
        ItemInfo* pItem = m_items[i];
        if (pItem == NULL)
        {
            continue;
        }

        // de-allocating the item
        delete pItem;
    }

    m_items.clear();
}

ItemInfo* ItemInfoManager::GetItemByID(const uint16_t& ID)
{
    for (int i = 0; i < m_items.size(); i++)
    {
        ItemInfo* pItem = m_items[i];
        if (pItem == NULL || pItem->ID != ID)
        {
            continue;
        }

        return pItem;
    }

    return NULL;
}

ItemInfo* ItemInfoManager::GetItemByName(std::string fName)
{
    for (int i = 0; i < m_items.size(); i++)
    {
        ItemInfo* pItem = m_items[i];
        if (pItem == NULL || pItem->name != fName)
        {
            continue;
        }

        return pItem;
    }

    return NULL;
}

ItemInfo* ItemInfoManager::CreateSeedVersionOfLastAddedItem(const uint16_t& tileID)
{
    ItemInfo* pFruitItem = GetItemByID(tileID);
    if (pFruitItem == NULL)
    {
        return NULL;
    }

    ItemInfo* pTargetItem = GetItemByID(tileID + 1);
    if (pTargetItem != NULL)
    {
        return pTargetItem;
    }

    nova_str name = pFruitItem->name + " Seed";
    if (tileID == ITEM_ID_MAGIC_EGG)
    {
        name = "Magic Egg";
    }

    if (tileID == ITEM_ID_STARSEED)
    {
        name = "Starseed";
    }

    if (tileID == ITEM_ID_GALACTIC_STARSEED)
    {
        name = "Galactic Starseed";
    }

    if (tileID == ITEM_ID_MUTATED_SEED)
    {
        name = "Mutated Seed";
    }

    ItemInfo* pSeed = new ItemInfo();
    pSeed->ID = tileID + 1;
    pSeed->name = name;
    pSeed->type = TYPE_SEED;
    pSeed->soundType = 0;
    pSeed->visualType = VISUAL_EFFECT_NORMAL;
    pSeed->tileStorage = STORAGE_SINGLE_FRAME;
    pSeed->hardness = 120;
    pSeed->layer = 0;
    pSeed->regenTime = 2;
    pSeed->bodyPart = 0;
    pSeed->rarity = pFruitItem->rarity;
    pSeed->editableTypes = pFruitItem->editableTypes;
    pSeed->texture = pFruitItem->texture;
    pSeed->textureX = pFruitItem->textureX;
    pSeed->textureY = pFruitItem->textureY;
    pSeed->textureHash = pFruitItem->textureHash;
    pSeed->seedColor = MAKE_RGBA(255, 255, 255, 255); // set later by defs
    pSeed->treeColor = MAKE_RGBA(255, 255, 255, 255); // set later by defs
    pSeed->bloomTime = 31;
    pSeed->desc = "Plant it to grow a `w" + pFruitItem->name + "``.";

    return pSeed;
}

int ItemInfoManager::StringToItemType(const std::string& str)
{
    std::map<nova_str, int> eItemTypeMap = {
    {"TYPE_FIST", TYPE_FIST},
    {"TYPE_WRENCH", TYPE_WRENCH},
    {"TYPE_DOOR", TYPE_DOOR},
    {"TYPE_LOCK", TYPE_LOCK},
    {"TYPE_GEMS", TYPE_GEMS},
    {"TYPE_TREASURE", TYPE_TREASURE},
    {"TYPE_DEADLY_BLOCK", TYPE_DEADLY_BLOCK},
    {"TYPE_TRAMPOLINE", TYPE_TRAMPOLINE},
    {"TYPE_CONSUMABLE", TYPE_CONSUMABLE},
    {"TYPE_GATEWAY", TYPE_GATEWAY},
    {"TYPE_SIGN", TYPE_SIGN},
    {"TYPE_SFX_WITH_EXTRA_FRAME", TYPE_SFX_WITH_EXTRA_FRAME},
    {"TYPE_BOOMBOX", TYPE_BOOMBOX},
    {"TYPE_MAIN_DOOR", TYPE_MAIN_DOOR},
    {"TYPE_PLATFORM", TYPE_PLATFORM},
    {"TYPE_BEDROCK", TYPE_BEDROCK},
    {"TYPE_LAVA", TYPE_LAVA},
    {"TYPE_NORMAL", TYPE_NORMAL},
    {"TYPE_BACKGROUND", TYPE_BACKGROUND},
    {"TYPE_SEED", TYPE_SEED},
    {"TYPE_CLOTHES", TYPE_CLOTHES},
    {"TYPE_NORMAL_WITH_EXTRA_FRAME", TYPE_NORMAL_WITH_EXTRA_FRAME},
    {"TYPE_BACKGD_SFX_EXTRA_FRAME", TYPE_BACKGD_SFX_EXTRA_FRAME},
    {"TYPE_BACK_BOOMBOX", TYPE_BACK_BOOMBOX},
    {"TYPE_BOUNCY", TYPE_BOUNCY},
    {"TYPE_POINTY", TYPE_POINTY},
    {"TYPE_PORTAL", TYPE_PORTAL},
    {"TYPE_CHECKPOINT", TYPE_CHECKPOINT},
    {"TYPE_MUSIC_NOTE", TYPE_MUSIC_NOTE},
    {"TYPE_ICE", TYPE_ICE},
    {"TYPE_SWITCHEROO", TYPE_SWITCHEROO},
    {"TYPE_CHEST", TYPE_CHEST},
    {"TYPE_MAILBOX", TYPE_MAILBOX},
    {"TYPE_BULLETIN", TYPE_BULLETIN},
    {"TYPE_PINATA", TYPE_PINATA},
    {"TYPE_DICE", TYPE_DICE},
    {"TYPE_CHEMICAL", TYPE_CHEMICAL},
    {"TYPE_PROVIDER", TYPE_PROVIDER},
    {"TYPE_LAB", TYPE_LAB},
    {"TYPE_ACHIEVEMENT", TYPE_ACHIEVEMENT},
    {"TYPE_WEATHER_MACHINE", TYPE_WEATHER_MACHINE},
    {"TYPE_SCORE_BOARD", TYPE_SCORE_BOARD},
    {"TYPE_SUNGATE", TYPE_SUNGATE},
    {"TYPE_PROFILE", TYPE_PROFILE},
    {"TYPE_DEADLY_IF_ON", TYPE_DEADLY_IF_ON},
    {"TYPE_HEART_MONITOR", TYPE_HEART_MONITOR},
    {"TYPE_DONATION_BOX", TYPE_DONATION_BOX},
    {"TYPE_TOYBOX", TYPE_TOYBOX},
    {"TYPE_MANNEQUIN", TYPE_MANNEQUIN},
    {"TYPE_SECURITY_CAMERA", TYPE_SECURITY_CAMERA},
    {"TYPE_MAGICEGG", TYPE_MAGICEGG},
    {"TYPE_GAME", TYPE_GAME},
    {"TYPE_GAME_GEN", TYPE_GAME_GEN},
    {"TYPE_XENONITE", TYPE_XENONITE},
    {"TYPE_DRESSUP", TYPE_DRESSUP},
    {"TYPE_CRYSTAL", TYPE_CRYSTAL},
    {"TYPE_BURGLAR", TYPE_BURGLAR},
    {"TYPE_COMPACTOR", TYPE_COMPACTOR},
    {"TYPE_SPOTLIGHT", TYPE_SPOTLIGHT},
    {"TYPE_WIND", TYPE_WIND},
    {"TYPE_DISPLAY_BLOCK", TYPE_DISPLAY_BLOCK},
    {"TYPE_VENDING", TYPE_VENDING},
    {"TYPE_FISHTANK", TYPE_FISHTANK},
    {"TYPE_PETFISH", TYPE_PETFISH},
    {"TYPE_SOLAR", TYPE_SOLAR},
    {"TYPE_FORGE", TYPE_FORGE},
    {"TYPE_GIVING_TREE", TYPE_GIVING_TREE},
    {"TYPE_GIVING_TREE_STUMP", TYPE_GIVING_TREE_STUMP},
    {"TYPE_STEAMPUNK", TYPE_STEAMPUNK},
    {"TYPE_STEAM_LAVA_IF_ON", TYPE_STEAM_LAVA_IF_ON},
    {"TYPE_STEAM_ORGAN", TYPE_STEAM_ORGAN},
    {"TYPE_TAMAGOTCHI", TYPE_TAMAGOTCHI},
    {"TYPE_SWING", TYPE_SWING},
    {"TYPE_FLAG", TYPE_FLAG},
    {"TYPE_LOBSTER_TRAP", TYPE_LOBSTER_TRAP},
    {"TYPE_ART_CANVAS", TYPE_ART_CANVAS},
    {"TYPE_BATTLE_CAGE", TYPE_BATTLE_CAGE},
    {"TYPE_PET_TRAINER", TYPE_PET_TRAINER},
    {"TYPE_STEAM_ENGINE", TYPE_STEAM_ENGINE},
    {"TYPE_LOCKBOT", TYPE_LOCKBOT},
    {"TYPE_WEATHER_SPECIAL", TYPE_WEATHER_SPECIAL},
    {"TYPE_SPIRIT_STORAGE", TYPE_SPIRIT_STORAGE},
    {"TYPE_DISPLAY_SHELF", TYPE_DISPLAY_SHELF},
    {"TYPE_VIP_ENTRANCE", TYPE_VIP_ENTRANCE},
    {"TYPE_CHALLENGE_TIMER", TYPE_CHALLENGE_TIMER},
    {"TYPE_CHALLENGE_FLAG", TYPE_CHALLENGE_FLAG},
    {"TYPE_FISH_MOUNT", TYPE_FISH_MOUNT},
    {"TYPE_PORTRAIT", TYPE_PORTRAIT},
    {"TYPE_WEATHER_SPECIAL2", TYPE_WEATHER_SPECIAL2},
    {"TYPE_FOSSIL", TYPE_FOSSIL},
    {"TYPE_FOSSIL_PREP", TYPE_FOSSIL_PREP},
    {"TYPE_DNA_MACHINE", TYPE_DNA_MACHINE},
    {"TYPE_BLASTER", TYPE_BLASTER},
    {"TYPE_VALHOWLA", TYPE_VALHOWLA},
    {"TYPE_CHEMSYNTH", TYPE_CHEMSYNTH},
    {"TYPE_CHEMTANK", TYPE_CHEMTANK},
    {"TYPE_STORAGE", TYPE_STORAGE},
    {"TYPE_OVEN", TYPE_OVEN},
    {"TYPE_SUPER_MUSIC", TYPE_SUPER_MUSIC},
    {"TYPE_GEIGER_CHARGER", TYPE_GEIGER_CHARGER},
    {"TYPE_ADVENTURE_RESET", TYPE_ADVENTURE_RESET},
    {"TYPE_TOMB_ROBBER", TYPE_TOMB_ROBBER},
    {"TYPE_FACTION", TYPE_FACTION},
    {"TYPE_RED_FACTION", TYPE_RED_FACTION},
    {"TYPE_GREEN_FACTION", TYPE_GREEN_FACTION},
    {"TYPE_BLUE_FACTION", TYPE_BLUE_FACTION},
    {"TYPE_ANCES", TYPE_ANCES},
    {"TYPE_UNKNOWN_108", TYPE_UNKNOWN_108},
    {"TYPE_FISHGOTCHI_TANK", TYPE_FISHGOTCHI_TANK},
    {"TYPE_FISHING_BLOCK", TYPE_FISHING_BLOCK},
    {"TYPE_ITEM_SUCKER", TYPE_ITEM_SUCKER},
    {"TYPE_ITEM_PLANTER", TYPE_ITEM_PLANTER},
    {"TYPE_ROBOT", TYPE_ROBOT},
    {"TYPE_COMMAND", TYPE_COMMAND},
    {"TYPE_TICKET", TYPE_TICKET},
    {"TYPE_STATS_BLOCK", TYPE_STATS_BLOCK},
    {"TYPE_FIELD_NODE", TYPE_FIELD_NODE},
    {"TYPE_OUIJA_BOARD", TYPE_OUIJA_BOARD},
    {"TYPE_ARCHITECT_MACHINE", TYPE_ARCHITECT_MACHINE},
    {"TYPE_STARSHIP", TYPE_STARSHIP},
    {"TYPE_AUTODELETE", TYPE_AUTODELETE},
    {"TYPE_GREEN_FOUNTAIN", TYPE_GREEN_FOUNTAIN},
    {"TYPE_AUTO_ACTION_BREAK", TYPE_AUTO_ACTION_BREAK},
    {"TYPE_AUTO_ACTION_HARVEST", TYPE_AUTO_ACTION_HARVEST},
    {"TYPE_AUTO_ACTION_HARVEST_SUCK", TYPE_AUTO_ACTION_HARVEST_SUCK},
    {"TYPE_LIGHTNING_IF_ON", TYPE_LIGHTNING_IF_ON},
    {"TYPE_PHASED_BLOCK", TYPE_PHASED_BLOCK},
    {"TYPE_MUD", TYPE_MUD},
    {"TYPE_ROOT_CUTTING", TYPE_ROOT_CUTTING},
    {"TYPE_PASSWORD_STORAGE", TYPE_PASSWORD_STORAGE},
    {"TYPE_PHASED_BLOCK_2", TYPE_PHASED_BLOCK_2},
    {"TYPE_BOMB", TYPE_BOMB},
    {"TYPE_WEATHER_INFINITY", TYPE_WEATHER_INFINITY},
    {"TYPE_SLIME", TYPE_SLIME},
    {"TYPE_COMPLETIONIST", TYPE_COMPLETIONIST},
    {"TYPE_SWITCHEROO2", TYPE_SWITCHEROO2},
    {"TYPE_FEEDING_BLOCK", TYPE_FEEDING_BLOCK},
    {"TYPE_KRANKENS_BLOCK", TYPE_KRANKENS_BLOCK},
    {"TYPE_FRIENDS_ENTRANCE", TYPE_FRIENDS_ENTRANCE}
    };

    auto it = eItemTypeMap.find(str);
    if (it == eItemTypeMap.end())
    {
        return TYPE_FIST;
    }

    return eItemTypeMap[str];
}

std::string ItemInfoManager::ItemTypeToString(const int& itemType)
{
    std::map<int, nova_str> eItemTypeMap = {
    {TYPE_FIST, "TYPE_FIST"},
    {TYPE_WRENCH, "TYPE_WRENCH"},
    {TYPE_DOOR, "TYPE_DOOR"},
    {TYPE_LOCK, "TYPE_LOCK"},
    {TYPE_GEMS, "TYPE_GEMS"},
    {TYPE_TREASURE, "TYPE_TREASURE"},
    {TYPE_DEADLY_BLOCK, "TYPE_DEADLY_BLOCK"},
    {TYPE_TRAMPOLINE, "TYPE_TRAMPOLINE"},
    {TYPE_CONSUMABLE, "TYPE_CONSUMABLE"},
    {TYPE_GATEWAY, "TYPE_GATEWAY"},
    {TYPE_SIGN, "TYPE_SIGN"},
    {TYPE_SFX_WITH_EXTRA_FRAME, "TYPE_SFX_WITH_EXTRA_FRAME"},
    {TYPE_BOOMBOX, "TYPE_BOOMBOX"},
    {TYPE_MAIN_DOOR, "TYPE_MAIN_DOOR"},
    {TYPE_PLATFORM, "TYPE_PLATFORM"},
    {TYPE_BEDROCK, "TYPE_BEDROCK"},
    {TYPE_LAVA, "TYPE_LAVA"},
    {TYPE_NORMAL, "TYPE_NORMAL"},
    {TYPE_BACKGROUND, "TYPE_BACKGROUND"},
    {TYPE_SEED, "TYPE_SEED"},
    {TYPE_CLOTHES, "TYPE_CLOTHES"},
    {TYPE_NORMAL_WITH_EXTRA_FRAME, "TYPE_NORMAL_WITH_EXTRA_FRAME"},
    {TYPE_BACKGD_SFX_EXTRA_FRAME, "TYPE_BACKGD_SFX_EXTRA_FRAME"},
    {TYPE_BACK_BOOMBOX, "TYPE_BACK_BOOMBOX"},
    {TYPE_BOUNCY, "TYPE_BOUNCY"},
    {TYPE_POINTY, "TYPE_POINTY"},
    {TYPE_PORTAL, "TYPE_PORTAL"},
    {TYPE_CHECKPOINT, "TYPE_CHECKPOINT"},
    {TYPE_MUSIC_NOTE, "TYPE_MUSIC_NOTE"},
    {TYPE_ICE, "TYPE_ICE"},
    {TYPE_SWITCHEROO, "TYPE_SWITCHEROO"},
    {TYPE_CHEST, "TYPE_CHEST"},
    {TYPE_MAILBOX, "TYPE_MAILBOX"},
    {TYPE_BULLETIN, "TYPE_BULLETIN"},
    {TYPE_PINATA, "TYPE_PINATA"},
    {TYPE_DICE, "TYPE_DICE"},
    {TYPE_CHEMICAL, "TYPE_CHEMICAL"},
    {TYPE_PROVIDER, "TYPE_PROVIDER"},
    {TYPE_LAB, "TYPE_LAB"},
    {TYPE_ACHIEVEMENT, "TYPE_ACHIEVEMENT"},
    {TYPE_WEATHER_MACHINE, "TYPE_WEATHER_MACHINE"},
    {TYPE_SCORE_BOARD, "TYPE_SCORE_BOARD"},
    {TYPE_SUNGATE, "TYPE_SUNGATE"},
    {TYPE_PROFILE, "TYPE_PROFILE"},
    {TYPE_DEADLY_IF_ON, "TYPE_DEADLY_IF_ON"},
    {TYPE_HEART_MONITOR, "TYPE_HEART_MONITOR"},
    {TYPE_DONATION_BOX, "TYPE_DONATION_BOX"},
    {TYPE_TOYBOX, "TYPE_TOYBOX"},
    {TYPE_MANNEQUIN, "TYPE_MANNEQUIN"},
    {TYPE_SECURITY_CAMERA, "TYPE_SECURITY_CAMERA"},
    {TYPE_MAGICEGG, "TYPE_MAGICEGG"},
    {TYPE_GAME, "TYPE_GAME"},
    {TYPE_GAME_GEN, "TYPE_GAME_GEN"},
    {TYPE_XENONITE, "TYPE_XENONITE"},
    {TYPE_DRESSUP, "TYPE_DRESSUP"},
    {TYPE_CRYSTAL, "TYPE_CRYSTAL"},
    {TYPE_BURGLAR, "TYPE_BURGLAR"},
    {TYPE_COMPACTOR, "TYPE_COMPACTOR"},
    {TYPE_SPOTLIGHT, "TYPE_SPOTLIGHT"},
    {TYPE_WIND, "TYPE_WIND"},
    {TYPE_DISPLAY_BLOCK, "TYPE_DISPLAY_BLOCK"},
    {TYPE_VENDING, "TYPE_VENDING"},
    {TYPE_FISHTANK, "TYPE_FISHTANK"},
    {TYPE_PETFISH, "TYPE_PETFISH"},
    {TYPE_SOLAR, "TYPE_SOLAR"},
    {TYPE_FORGE, "TYPE_FORGE"},
    {TYPE_GIVING_TREE, "TYPE_GIVING_TREE"},
    {TYPE_GIVING_TREE_STUMP, "TYPE_GIVING_TREE_STUMP"},
    {TYPE_STEAMPUNK, "TYPE_STEAMPUNK"},
    {TYPE_STEAM_LAVA_IF_ON, "TYPE_STEAM_LAVA_IF_ON"},
    {TYPE_STEAM_ORGAN, "TYPE_STEAM_ORGAN"},
    {TYPE_TAMAGOTCHI, "TYPE_TAMAGOTCHI"},
    {TYPE_SWING, "TYPE_SWING"},
    {TYPE_FLAG, "TYPE_FLAG"},
    {TYPE_LOBSTER_TRAP, "TYPE_LOBSTER_TRAP"},
    {TYPE_ART_CANVAS, "TYPE_ART_CANVAS"},
    {TYPE_BATTLE_CAGE, "TYPE_BATTLE_CAGE"},
    {TYPE_PET_TRAINER, "TYPE_PET_TRAINER"},
    {TYPE_STEAM_ENGINE, "TYPE_STEAM_ENGINE"},
    {TYPE_LOCKBOT, "TYPE_LOCKBOT"},
    {TYPE_WEATHER_SPECIAL, "TYPE_WEATHER_SPECIAL"},
    {TYPE_SPIRIT_STORAGE, "TYPE_SPIRIT_STORAGE"},
    {TYPE_DISPLAY_SHELF, "TYPE_DISPLAY_SHELF"},
    {TYPE_VIP_ENTRANCE, "TYPE_VIP_ENTRANCE"},
    {TYPE_CHALLENGE_TIMER, "TYPE_CHALLENGE_TIMER"},
    {TYPE_CHALLENGE_FLAG, "TYPE_CHALLENGE_FLAG"},
    {TYPE_FISH_MOUNT, "TYPE_FISH_MOUNT"},
    {TYPE_PORTRAIT, "TYPE_PORTRAIT"},
    {TYPE_WEATHER_SPECIAL2, "TYPE_WEATHER_SPECIAL2"},
    {TYPE_FOSSIL, "TYPE_FOSSIL"},
    {TYPE_FOSSIL_PREP, "TYPE_FOSSIL_PREP"},
    {TYPE_DNA_MACHINE, "TYPE_DNA_MACHINE"},
    {TYPE_BLASTER, "TYPE_BLASTER"},
    {TYPE_VALHOWLA, "TYPE_VALHOWLA"},
    {TYPE_CHEMSYNTH, "TYPE_CHEMSYNTH"},
    {TYPE_CHEMTANK, "TYPE_CHEMTANK"},
    {TYPE_STORAGE, "TYPE_STORAGE"},
    {TYPE_OVEN, "TYPE_OVEN"},
    {TYPE_SUPER_MUSIC, "TYPE_SUPER_MUSIC"},
    {TYPE_GEIGER_CHARGER, "TYPE_GEIGER_CHARGER"},
    {TYPE_ADVENTURE_RESET, "TYPE_ADVENTURE_RESET"},
    {TYPE_TOMB_ROBBER, "TYPE_TOMB_ROBBER"},
    {TYPE_FACTION, "TYPE_FACTION"},
    {TYPE_RED_FACTION, "TYPE_RED_FACTION"},
    {TYPE_GREEN_FACTION, "TYPE_GREEN_FACTION"},
    {TYPE_BLUE_FACTION, "TYPE_BLUE_FACTION"},
    {TYPE_ANCES, "TYPE_ANCES"},
    {TYPE_UNKNOWN_108, "TYPE_UNKNOWN_108"},
    {TYPE_FISHGOTCHI_TANK, "TYPE_FISHGOTCHI_TANK"},
    {TYPE_FISHING_BLOCK, "TYPE_FISHING_BLOCK"},
    {TYPE_ITEM_SUCKER, "TYPE_ITEM_SUCKER"},
    {TYPE_ITEM_PLANTER, "TYPE_ITEM_PLANTER"},
    {TYPE_ROBOT, "TYPE_ROBOT"},
    {TYPE_COMMAND, "TYPE_COMMAND"},
    {TYPE_TICKET, "TYPE_TICKET"},
    {TYPE_STATS_BLOCK, "TYPE_STATS_BLOCK"},
    {TYPE_FIELD_NODE, "TYPE_FIELD_NODE"},
    {TYPE_OUIJA_BOARD, "TYPE_OUIJA_BOARD"},
    {TYPE_ARCHITECT_MACHINE, "TYPE_ARCHITECT_MACHINE"},
    {TYPE_STARSHIP, "TYPE_STARSHIP"},
    {TYPE_AUTODELETE, "TYPE_AUTODELETE"},
    {TYPE_GREEN_FOUNTAIN, "TYPE_GREEN_FOUNTAIN"},
    {TYPE_AUTO_ACTION_BREAK, "TYPE_AUTO_ACTION_BREAK"},
    {TYPE_AUTO_ACTION_HARVEST, "TYPE_AUTO_ACTION_HARVEST"},
    {TYPE_AUTO_ACTION_HARVEST_SUCK, "TYPE_AUTO_ACTION_HARVEST_SUCK"},
    {TYPE_LIGHTNING_IF_ON, "TYPE_LIGHTNING_IF_ON"},
    {TYPE_PHASED_BLOCK, "TYPE_PHASED_BLOCK"},
    {TYPE_MUD, "TYPE_MUD"},
    {TYPE_ROOT_CUTTING, "TYPE_ROOT_CUTTING"},
    {TYPE_PASSWORD_STORAGE, "TYPE_PASSWORD_STORAGE"},
    {TYPE_PHASED_BLOCK_2, "TYPE_PHASED_BLOCK_2"},
    {TYPE_BOMB, "TYPE_BOMB"},
    {TYPE_WEATHER_INFINITY, "TYPE_WEATHER_INFINITY"},
    {TYPE_SLIME, "TYPE_SLIME"},
    {TYPE_COMPLETIONIST, "TYPE_COMPLETIONIST"},
    {TYPE_SWITCHEROO2, "TYPE_SWITCHEROO2"},
    {TYPE_FEEDING_BLOCK, "TYPE_FEEDING_BLOCK"},
    {TYPE_KRANKENS_BLOCK, "TYPE_KRANKENS_BLOCK"},
    {TYPE_FRIENDS_ENTRANCE, "TYPE_FRIENDS_ENTRANCE"}
    };

    auto it = eItemTypeMap.find(itemType);
    if (it == eItemTypeMap.end())
    {
        return "TYPE_FIST";
    }

    return eItemTypeMap[itemType];
}

int ItemInfoManager::StringToTileVisualEffect(const std::string& str)
{
    if (str == "VISUAL_EFFECT_NORMAL") return VISUAL_EFFECT_NORMAL;
    else if (str == "VISUAL_EFFECT_FLAME_LICK") return VISUAL_EFFECT_FLAME_LICK;
    else if (str == "VISUAL_EFFECT_SMOKING") return VISUAL_EFFECT_SMOKING;
    else if (str == "VISUAL_EFFECT_GLOW_TINT1") return VISUAL_EFFECT_GLOW_TINT1;
    else if (str == "VISUAL_EFFECT_ANIM") return VISUAL_EFFECT_ANIM;
    else if (str == "VISUAL_EFFECT_BUBBLES") return VISUAL_EFFECT_BUBBLES;
    else if (str == "VISUAL_EFFECT_PET") return VISUAL_EFFECT_PET;
    else if (str == "VISUAL_EFFECT_PET_ANIM") return VISUAL_EFFECT_PET_ANIM;
    else if (str == "VISUAL_EFFECT_NO_ARMS") return VISUAL_EFFECT_NO_ARMS;
    else if (str == "VISUAL_EFFECT_WAVEY") return VISUAL_EFFECT_WAVEY;
    else if (str == "VISUAL_EFFECT_WAVEY_ANIM") return VISUAL_EFFECT_WAVEY_ANIM;
    else if (str == "VISUAL_EFFECT_BOTHARMS") return VISUAL_EFFECT_BOTHARMS;
    else if (str == "VISUAL_EFFECT_LOWHAIR") return VISUAL_EFFECT_LOWHAIR;
    else if (str == "VISUAL_EFFECT_UNDERFACE") return VISUAL_EFFECT_UNDERFACE;
    else if (str == "VISUAL_EFFECT_SKINTINT") return VISUAL_EFFECT_SKINTINT;
    else if (str == "VISUAL_EFFECT_MASK") return VISUAL_EFFECT_MASK;
    else if (str == "VISUAL_EFFECT_ANIM_MASK") return VISUAL_EFFECT_ANIM_MASK;
    else if (str == "VISUAL_EFFECT_LOWHAIR_MASK") return VISUAL_EFFECT_LOWHAIR_MASK;
    else if (str == "VISUAL_EFFECT_GHOST") return VISUAL_EFFECT_GHOST;
    else if (str == "VISUAL_EFFECT_PULSE") return VISUAL_EFFECT_PULSE;
    else if (str == "VISUAL_EFFECT_COLORIZE") return VISUAL_EFFECT_COLORIZE;
    else if (str == "VISUAL_EFFECT_COLORIZE_TO_SHIRT") return VISUAL_EFFECT_COLORIZE_TO_SHIRT;
    else if (str == "VISUAL_EFFECT_COLORIZE_ANIM") return VISUAL_EFFECT_COLORIZE_ANIM;
    else if (str == "VISUAL_EFFECT_HIGHFACE") return VISUAL_EFFECT_HIGHFACE;
    else if (str == "VISUAL_EFFECT_HIGHFACE_ANIM") return VISUAL_EFFECT_HIGHFACE_ANIM;
    else if (str == "VISUAL_EFFECT_RAINBOW_SHIFT") return VISUAL_EFFECT_RAINBOW_SHIFT;
    else if (str == "VISUAL_EFFECT_BACKFORE") return VISUAL_EFFECT_BACKFORE;
    else if (str == "VISUAL_EFFECT_COLORIZE_WITH_SKIN") return VISUAL_EFFECT_COLORIZE_WITH_SKIN;
    else if (str == "VISUAL_EFFECT_NO_RENDER") return VISUAL_EFFECT_NO_RENDER;
    else if (str == "VISUAL_EFFECT_SPIN") return VISUAL_EFFECT_SPIN;
    else if (str == "VISUAL_EFFECT_OFFHAND") return VISUAL_EFFECT_OFFHAND;
    else if (str == "VISUAL_EFFECT_WINGED") return VISUAL_EFFECT_WINGED;
    else if (str == "VISUAL_EFFECT_SINK") return VISUAL_EFFECT_SINK;
    else if (str == "VISUAL_EFFECT_DARKNESS") return VISUAL_EFFECT_DARKNESS;
    else if (str == "VISUAL_EFFECT_LIGHTSOURCE") return VISUAL_EFFECT_LIGHTSOURCE;
    else if (str == "VISUAL_EFFECT_LIGHT_IF_ON") return VISUAL_EFFECT_LIGHT_IF_ON;
    else if (str == "VISUAL_EFFECT_DISCOLOR") return VISUAL_EFFECT_DISCOLOR;
    else if (str == "VISUAL_EFFECT_STEP_SPIN") return VISUAL_EFFECT_STEP_SPIN;
    else if (str == "VISUAL_EFFECT_PETCOLORED") return VISUAL_EFFECT_PETCOLORED;
    else if (str == "VISUAL_EFFECT_SILKFOOT") return VISUAL_EFFECT_SILKFOOT;
    else if (str == "VISUAL_EFFECT_TILTY") return VISUAL_EFFECT_TILTY;
    else if (str == "VISUAL_EFFECT_TILTY_DARK") return VISUAL_EFFECT_TILTY_DARK;
    else if (str == "VISUAL_EFFECT_NEXT_FRAME_IF_ON") return VISUAL_EFFECT_NEXT_FRAME_IF_ON;
    else if (str == "VISUAL_EFFECT_WOBBLE") return VISUAL_EFFECT_WOBBLE;
    else if (str == "VISUAL_EFFECT_SCROLL") return VISUAL_EFFECT_SCROLL;
    else if (str == "VISUAL_EFFECT_LIGHTSOURCE_PULSE") return VISUAL_EFFECT_LIGHTSOURCE_PULSE;
    else if (str == "VISUAL_EFFECT_BUBBLE_MACHINE") return VISUAL_EFFECT_BUBBLE_MACHINE;
    else if (str == "VISUAL_EFFECT_VERYLOWHAIR") return VISUAL_EFFECT_VERYLOWHAIR;
    else if (str == "VISUAL_EFFECT_VERYLOWHAIR_MASK") return VISUAL_EFFECT_VERYLOWHAIR_MASK;

    return VISUAL_EFFECT_NORMAL;
}

std::string ItemInfoManager::TileVisualEffectToString(const int& visualType)
{
    switch (visualType)
    {
        case VISUAL_EFFECT_NORMAL: return "VISUAL_EFFECT_NORMAL";
        case VISUAL_EFFECT_FLAME_LICK: return "VISUAL_EFFECT_FLAME_LICK";
        case VISUAL_EFFECT_SMOKING: return "VISUAL_EFFECT_SMOKING";
        case VISUAL_EFFECT_GLOW_TINT1: return "VISUAL_EFFECT_GLOW_TINT1";
        case VISUAL_EFFECT_ANIM: return "VISUAL_EFFECT_ANIM";
        case VISUAL_EFFECT_BUBBLES: return "VISUAL_EFFECT_BUBBLES";
        case VISUAL_EFFECT_PET: return "VISUAL_EFFECT_PET";
        case VISUAL_EFFECT_PET_ANIM: return "VISUAL_EFFECT_PET_ANIM";
        case VISUAL_EFFECT_NO_ARMS: return "VISUAL_EFFECT_NO_ARMS";
        case VISUAL_EFFECT_WAVEY: return "VISUAL_EFFECT_WAVEY";
        case VISUAL_EFFECT_WAVEY_ANIM: return "VISUAL_EFFECT_WAVEY_ANIM";
        case VISUAL_EFFECT_BOTHARMS: return "VISUAL_EFFECT_BOTHARMS";
        case VISUAL_EFFECT_LOWHAIR: return "VISUAL_EFFECT_LOWHAIR";
        case VISUAL_EFFECT_UNDERFACE: return "VISUAL_EFFECT_UNDERFACE";
        case VISUAL_EFFECT_SKINTINT: return "VISUAL_EFFECT_SKINTINT";
        case VISUAL_EFFECT_MASK: return "VISUAL_EFFECT_MASK";
        case VISUAL_EFFECT_ANIM_MASK: return "VISUAL_EFFECT_ANIM_MASK";
        case VISUAL_EFFECT_LOWHAIR_MASK: return "VISUAL_EFFECT_LOWHAIR_MASK";
        case VISUAL_EFFECT_GHOST: return "VISUAL_EFFECT_GHOST";
        case VISUAL_EFFECT_PULSE: return "VISUAL_EFFECT_PULSE";
        case VISUAL_EFFECT_COLORIZE: return "VISUAL_EFFECT_COLORIZE";
        case VISUAL_EFFECT_COLORIZE_TO_SHIRT: return "VISUAL_EFFECT_COLORIZE_TO_SHIRT";
        case VISUAL_EFFECT_COLORIZE_ANIM: return "VISUAL_EFFECT_COLORIZE_ANIM";
        case VISUAL_EFFECT_HIGHFACE: return "VISUAL_EFFECT_HIGHFACE";
        case VISUAL_EFFECT_HIGHFACE_ANIM: return "VISUAL_EFFECT_HIGHFACE_ANIM";
        case VISUAL_EFFECT_RAINBOW_SHIFT: return "VISUAL_EFFECT_RAINBOW_SHIFT";
        case VISUAL_EFFECT_BACKFORE: return "VISUAL_EFFECT_BACKFORE";
        case VISUAL_EFFECT_COLORIZE_WITH_SKIN: return "VISUAL_EFFECT_COLORIZE_WITH_SKIN";
        case VISUAL_EFFECT_NO_RENDER: return "VISUAL_EFFECT_NO_RENDER";
        case VISUAL_EFFECT_SPIN: return "VISUAL_EFFECT_SPIN";
        case VISUAL_EFFECT_OFFHAND: return "VISUAL_EFFECT_OFFHAND";
        case VISUAL_EFFECT_WINGED: return "VISUAL_EFFECT_WINGED";
        case VISUAL_EFFECT_SINK: return "VISUAL_EFFECT_SINK";
        case VISUAL_EFFECT_DARKNESS: return "VISUAL_EFFECT_DARKNESS";
        case VISUAL_EFFECT_LIGHTSOURCE: return "VISUAL_EFFECT_LIGHTSOURCE";
        case VISUAL_EFFECT_LIGHT_IF_ON: return "VISUAL_EFFECT_LIGHT_IF_ON";
        case VISUAL_EFFECT_DISCOLOR: return "VISUAL_EFFECT_DISCOLOR";
        case VISUAL_EFFECT_STEP_SPIN: return "VISUAL_EFFECT_STEP_SPIN";
        case VISUAL_EFFECT_PETCOLORED: return "VISUAL_EFFECT_PETCOLORED";
        case VISUAL_EFFECT_SILKFOOT: return "VISUAL_EFFECT_SILKFOOT";
        case VISUAL_EFFECT_TILTY: return "VISUAL_EFFECT_TILTY";
        case VISUAL_EFFECT_TILTY_DARK: return "VISUAL_EFFECT_TILTY_DARK";
        case VISUAL_EFFECT_NEXT_FRAME_IF_ON: return "VISUAL_EFFECT_NEXT_FRAME_IF_ON";
        case VISUAL_EFFECT_WOBBLE: return "VISUAL_EFFECT_WOBBLE";
        case VISUAL_EFFECT_SCROLL: return "VISUAL_EFFECT_SCROLL";
        case VISUAL_EFFECT_LIGHTSOURCE_PULSE: return "VISUAL_EFFECT_LIGHTSOURCE_PULSE";
        case VISUAL_EFFECT_BUBBLE_MACHINE: return "VISUAL_EFFECT_BUBBLE_MACHINE";
        case VISUAL_EFFECT_VERYLOWHAIR: return "VISUAL_EFFECT_VERYLOWHAIR";
        case VISUAL_EFFECT_VERYLOWHAIR_MASK: return "VISUAL_EFFECT_VERYLOWHAIR_MASK";
    }

    return "VISUAL_EFFECT_NORMAL";
}

int ItemInfoManager::StringToItemFlag(const std::string& str)
{
    if (str == "NONE") return ITEMFLAG_NONE;
    else if (str == "FLIPPED") return FLIPPED;
    else if (str == "EDITABLE") return EDITABLE;
    else if (str == "SEEDLESS") return SEEDLESS;
    else if (str == "PERMANENT") return PERMANENT;
    else if (str == "DROPLESS") return DROPLESS;
    else if (str == "NOSELF") return NOSELF;
    else if (str == "NOSHADOW") return NOSHADOW;
    else if (str == "WORLD_LOCK") return WORLD_LOCK;
    else if (str == "BETA") return BETA;
    else if (str == "AUTOPICKUP") return AUTOPICKUP;
    else if (str == "MOD") return MOD;
    else if (str == "RANDGROW") return RANDGROW;
    else if (str == "PUBLIC") return PUBLIC;
    else if (str == "FOREGROUND") return FOREGROUND;
    else if (str == "HOLIDAY") return HOLIDAY;
    else if (str == "UNTRADEABLE") return UNTRADEABLE;

    return ITEMFLAG_NONE;
}

std::string ItemInfoManager::ItemFlagToString(const int& itemFlag)
{
    switch (itemFlag)
    {
        case ITEMFLAG_NONE: return "NONE";
        case FLIPPED: return "FLIPPED";
        case EDITABLE: return "EDITABLE";
        case SEEDLESS: return "SEEDLESS";
        case PERMANENT: return "PERMANENT";
        case DROPLESS: return "DROPLESS";
        case NOSELF: return "NOSELF";
        case NOSHADOW: return "NOSHADOW";
        case WORLD_LOCK: return "WORLD_LOCK";
        case BETA: return "BETA";
        case AUTOPICKUP: return "AUTOPICKUP";
        case MOD: return "MOD";
        case RANDGROW: return "RANDGROW";
        case PUBLIC: return "PUBLIC";
        case FOREGROUND: return "FOREGROUND";
        case HOLIDAY: return "HOLIDAY";
        case UNTRADEABLE: return "UNTRADEABLE";
        default: return "NONE";
    }

    return "NONE";
}

int ItemInfoManager::StringToItemFlag2(const std::string& str)
{
    if (str == "NONE") return ITEMFLAG2_NONE;
    else if (str == "ROBOT_DEADLY") return ROBOT_DEADLY;
    else if (str == "ROBOT_SHOOT_LEFT") return ROBOT_SHOOT_LEFT;
    else if (str == "ROBOT_SHOOT_RIGHT") return ROBOT_SHOOT_RIGHT;
    else if (str == "ROBOT_SHOOT_DOWN") return ROBOT_SHOOT_DOWN;
    else if (str == "ROBOT_SHOOT_UP") return ROBOT_SHOOT_UP;
    else if (str == "ROBOT_CAN_SHOOT") return ROBOT_CAN_SHOOT;
    else if (str == "ROBOT_LAVA") return ROBOT_LAVA;
    else if (str == "ROBOT_POINTY") return ROBOT_POINTY;
    else if (str == "ROBOT_SHOOT_DEADLY") return ROBOT_SHOOT_DEADLY;
    else if (str == "GUILD_ITEM") return GUILD_ITEM;
    else if (str == "GUILD_FLAG") return GUILD_FLAG;
    else if (str == "STARSHIP_HELP") return STARSHIP_HELP;
    else if (str == "STARSHIP_REACTOR") return STARSHIP_REACTOR;
    else if (str == "STARSHIP_VIEWSCREEN") return STARSHIP_VIEWSCREEN;
    else if (str == "SUPERMOD") return SUPERMOD;
    else if (str == "TILE_DEADLY_IF_ON") return TILE_DEADLY_IF_ON;
    else if (str == "LONG_HAND_ITEM") return LONG_HAND_ITEM;
    else if (str == "GEMLESS") return GEMLESS;
    else if (str == "TRANSMUTABLE") return TRANSMUTABLE;
    else if (str == "DUNGEON_ITEM") return DUNGEON_ITEM;
    else if (str == "PVE_MELEE") return PVE_MELEE;
    else if (str == "PVE_RANGED") return PVE_RANGED;
    else if (str == "PVE_AUTO_AIM") return PVE_AUTO_AIM;
    else if (str == "ONE_IN_WORLD") return ONE_IN_WORLD;
    else if (str == "ONLY_FOR_WORLD_OWNER") return ONLY_FOR_WORLD_OWNER;

    return ITEMFLAG2_NONE;
}

int ItemInfoManager::StringToTileStorage(const std::string& str)
{
    if (str == "STORAGE_SINGLE_FRAME_ALONE") return STORAGE_SINGLE_FRAME_ALONE;
    else if (str == "STORAGE_SINGLE_FRAME") return STORAGE_SINGLE_FRAME;
    else if (str == "STORAGE_SMART_EDGE") return STORAGE_SMART_EDGE;
    else if (str == "STORAGE_SMART_EDGE_HORIZ") return STORAGE_SMART_EDGE_HORIZ;
    else if (str == "STORAGE_SMART_CLING") return STORAGE_SMART_CLING;
    else if (str == "STORAGE_SMART_OUTER") return STORAGE_SMART_OUTER;
    else if (str == "STORAGE_RANDOM") return STORAGE_RANDOM;
    else if (str == "STORAGE_SMART_EDGE_VERT") return STORAGE_SMART_EDGE_VERT;
    else if (str == "STORAGE_SMART_EDGE_HORIZ_CAVE") return STORAGE_SMART_EDGE_HORIZ_CAVE;
    else if (str == "STORAGE_SMART_CLING2") return STORAGE_SMART_CLING2;
    else if (str == "STORAGE_SMART_EDGE_HORIZ_FLIPPABLE") return STORAGE_SMART_EDGE_HORIZ_FLIPPABLE;

    return STORAGE_SINGLE_FRAME_ALONE;
}

std::string ItemInfoManager::TileStorageToString(const int& tileStorage)
{
    switch (tileStorage)
    {
        case STORAGE_SINGLE_FRAME_ALONE: return "STORAGE_SINGLE_FRAME_ALONE";
        case STORAGE_SINGLE_FRAME: return "STORAGE_SINGLE_FRAME";
        case STORAGE_SMART_EDGE: return "STORAGE_SMART_EDGE";
        case STORAGE_SMART_EDGE_HORIZ: return "STORAGE_SMART_EDGE_HORIZ";
        case STORAGE_SMART_CLING: return "STORAGE_SMART_CLING";
        case STORAGE_SMART_OUTER: return "STORAGE_SMART_OUTER";
        case STORAGE_RANDOM: return "STORAGE_RANDOM";
        case STORAGE_SMART_EDGE_VERT: return "STORAGE_SMART_EDGE_VERT";
        case STORAGE_SMART_EDGE_HORIZ_CAVE: return "STORAGE_SMART_EDGE_HORIZ_CAVE";
        case STORAGE_SMART_CLING2: return "STORAGE_SMART_CLING2";
        case STORAGE_SMART_EDGE_HORIZ_FLIPPABLE: return "STORAGE_SMART_EDGE_HORIZ_FLIPPABLE";
    }

    return "STORAGE_SINGLE_FRAME_ALONE";
}

int ItemInfoManager::StringToTileCollision(const std::string& str)
{
    if (str == "TILE_COLLISION_NONE") return TILE_COLLISION_NONE;
    else if (str == "TILE_COLLISION_SOLID") return TILE_COLLISION_SOLID;
    else if (str == "TILE_COLLISION_JUMP_THROUGH") return TILE_COLLISION_JUMP_THROUGH;
    else if (str == "TILE_COLLISION_GATEWAY") return TILE_COLLISION_GATEWAY;
    else if (str == "TILE_COLLISION_IF_OFF") return TILE_COLLISION_IF_OFF;
    else if (str == "TILE_COLLISION_ONE_WAY") return TILE_COLLISION_ONE_WAY;
    else if (str == "TILE_COLLISION_VIP") return TILE_COLLISION_VIP;
    else if (str == "TILE_COLLISION_JUMP_DOWN") return TILE_COLLISION_JUMP_DOWN;
    else if (str == "TILE_COLLISION_ADVENTURE") return TILE_COLLISION_ADVENTURE;
    else if (str == "TILE_COLLISION_IF_ON") return TILE_COLLISION_IF_ON;
    else if (str == "TILE_COLLISION_FACTION") return TILE_COLLISION_FACTION;
    else if (str == "TILE_COLLISION_GUILD") return TILE_COLLISION_GUILD;
    else if (str == "TILE_COLLISION_CLOUD") return TILE_COLLISION_CLOUD;

    return TILE_COLLISION_NONE;
}

std::string ItemInfoManager::TileCollisionToString(const int& tileCollision)
{
    switch (tileCollision)
    {
        case TILE_COLLISION_NONE: return "TILE_COLLISION_NONE";
        case TILE_COLLISION_SOLID: return "TILE_COLLISION_SOLID";
        case TILE_COLLISION_JUMP_THROUGH: return "TILE_COLLISION_JUMP_THROUGH";
        case TILE_COLLISION_GATEWAY: return "TILE_COLLISION_GATEWAY";
        case TILE_COLLISION_IF_OFF: return "TILE_COLLISION_IF_OFF";
        case TILE_COLLISION_ONE_WAY: return "TILE_COLLISION_ONE_WAY";
        case TILE_COLLISION_VIP: return "TILE_COLLISION_VIP";
        case TILE_COLLISION_JUMP_DOWN: return "TILE_COLLISION_JUMP_DOWN";
        case TILE_COLLISION_ADVENTURE: return "TILE_COLLISION_ADVENTURE";
        case TILE_COLLISION_IF_ON: return "TILE_COLLISION_IF_ON";
        case TILE_COLLISION_FACTION: return "TILE_COLLISION_FACTION";
        case TILE_COLLISION_GUILD: return "TILE_COLLISION_GUILD";
        case TILE_COLLISION_CLOUD: return "TILE_COLLISION_CLOUD";
    }

    return "TILE_COLLISION_NONE";
}

std::string ItemInfoManager::ItemFlag2ToString(const int& itemFlag)
{
    switch (itemFlag)
    {
        case ITEMFLAG2_NONE: return "NONE";
        case ROBOT_DEADLY: return "ROBOT_DEADLY";
        case ROBOT_SHOOT_LEFT: return "ROBOT_SHOOT_LEFT";
        case ROBOT_SHOOT_RIGHT: return "ROBOT_SHOOT_RIGHT";
        case ROBOT_SHOOT_DOWN: return "ROBOT_SHOOT_DOWN";
        case ROBOT_SHOOT_UP: return "ROBOT_SHOOT_UP";
        case ROBOT_CAN_SHOOT: return "ROBOT_CAN_SHOOT";
        case ROBOT_LAVA: return "ROBOT_LAVA";
        case ROBOT_POINTY: return "ROBOT_POINTY";
        case ROBOT_SHOOT_DEADLY: return "ROBOT_SHOOT_DEADLY";
        case GUILD_ITEM: return "GUILD_ITEM";
        case GUILD_FLAG: return "GUILD_FLAG";
        case STARSHIP_HELP: return "STARSHIP_HELP";
        case STARSHIP_REACTOR: return "STARSHIP_REACTOR";
        case STARSHIP_VIEWSCREEN: return "STARSHIP_VIEWSCREEN";
        case SUPERMOD: return "SUPERMOD";
        case TILE_DEADLY_IF_ON: return "TILE_DEADLY_IF_ON";
        case LONG_HAND_ITEM: return "LONG_HAND_ITEM";
        case GEMLESS: return "GEMLESS";
        case TRANSMUTABLE: return "TRANSMUTABLE";
        case DUNGEON_ITEM: return "DUNGEON_ITEM";
        case PVE_MELEE: return "PVE_MELEE";
        case PVE_RANGED: return "PVE_RANGED";
        case PVE_AUTO_AIM: return "PVE_AUTO_AIM";
        case ONE_IN_WORLD: return "ONE_IN_WORLD";
        case ONLY_FOR_WORLD_OWNER: return "ONLY_FOR_WORLD_OWNER";
        default: return "NONE";
    }

    return "NONE";
}

int ItemInfoManager::StringToItemFxFlag(const std::string& str)
{
    if (str == "NONE") return ITEMFXFLAG_NONE;
    else if (str == "MULTI_ANIM_START") return MULTI_ANIM_START;
    else if (str == "PING_PONG_ANIM") return PING_PONG_ANIM;
    else if (str == "OVERLAY_OBJECT") return OVERLAY_OBJECT;
    else if (str == "OFFSET_UP") return OFFSET_UP;
    else if (str == "DUAL_LAYER") return DUAL_LAYER;
    else if (str == "MULTI_ANIM2_START") return MULTI_ANIM2_START;
    else if (str == "UNK_0x40") return UNK_0x40;
    else if (str == "USE_SKIN_TINT") return USE_SKIN_TINT;
    else if (str == "SEED_TINT_LAYER1") return SEED_TINT_LAYER1;
    else if (str == "SEED_TINT_LAYER2") return SEED_TINT_LAYER2;
    else if (str == "RAINBOW_TINT_LAYER1") return RAINBOW_TINT_LAYER1;
    else if (str == "RAINBOW_TINT_LAYER2") return RAINBOW_TINT_LAYER2;
    else if (str == "GLOW") return GLOW;
    else if (str == "NO_ARMS") return NO_ARMS;
    else if (str == "FRONT_ARM_PUNCH") return FRONT_ARM_PUNCH;
    else if (str == "RENDER_OFFHAND") return RENDER_OFFHAND;
    else if (str == "SLOWFALL_OBJECT") return SLOWFALL_OBJECT;
    else if (str == "REPLACEMENT_SPRITE") return REPLACEMENT_SPRITE;
    else if (str == "ORB_FLOAT") return ORB_FLOAT;
    else if (str == "UNK_0x80000") return UNK_0x80000;
    else if (str == "RENDER_FX_VARIANT_VERSION") return RENDER_FX_VARIANT_VERSION;

    return ITEMFXFLAG_NONE;
}

std::string ItemInfoManager::ItemFxFlagToString(const int& fxFlag)
{
    switch (fxFlag)
    {
        case ITEMFXFLAG_NONE: return "NONE";
        case MULTI_ANIM_START: return "MULTI_ANIM_START";
        case PING_PONG_ANIM: return "PING_PONG_ANIM";
        case OVERLAY_OBJECT: return "OVERLAY_OBJECT";
        case OFFSET_UP: return "OFFSET_UP";
        case DUAL_LAYER: return "DUAL_LAYER";
        case MULTI_ANIM2_START: return "MULTI_ANIM2_START";
        case UNK_0x40: return "UNK_0x40";
        case USE_SKIN_TINT: return "USE_SKIN_TINT";
        case SEED_TINT_LAYER1: return "SEED_TINT_LAYER1";
        case SEED_TINT_LAYER2: return "SEED_TINT_LAYER2";
        case RAINBOW_TINT_LAYER1: return "RAINBOW_TINT_LAYER1";
        case RAINBOW_TINT_LAYER2: return "RAINBOW_TINT_LAYER2";
        case GLOW: return "GLOW";
        case NO_ARMS: return "NO_ARMS";
        case FRONT_ARM_PUNCH: return "FRONT_ARM_PUNCH";
        case RENDER_OFFHAND: return "RENDER_OFFHAND";
        case SLOWFALL_OBJECT: return "SLOWFALL_OBJECT";
        case REPLACEMENT_SPRITE: return "REPLACEMENT_SPRITE";
        case ORB_FLOAT: return "ORB_FLOAT";
        case UNK_0x80000: return "UNK_0x80000";
        case RENDER_FX_VARIANT_VERSION: return "RENDER_FX_VARIANT_VERSION";
        default: return "NONE";
    }

    return "NONE";
}

bool ItemInfoManager::Load()
{
    TextScanner t;
    if (!t.LoadFile("item_definitions.txt"))
    {
        return false;
    }

    int lastID = 0;
    std::vector<nova_str> lines = t.GetLines();
    for (int i = 0; i < lines.size(); i++)
    {
        const nova_str& line = lines[i];
        if (line.starts_with('#') || line.empty())
        {
            continue;
        }

        std::vector<nova_str> tokens = Utils::StringTokenize(line, "|");
        if (tokens[0] == "add_item")
        {
            if (tokens.size() < 15)
            {
                // failed to parse item
                return false;
            }

            ItemInfo* pItem = new ItemInfo();
            pItem->ID = std::atoi(tokens[1].c_str());
            pItem->name = tokens[2];
            pItem->type = StringToItemType(tokens[3].c_str());
            pItem->soundType = std::atoi(tokens[4].c_str());
            pItem->visualType = StringToTileVisualEffect(tokens[5]);
            pItem->tileStorage = StringToTileStorage(tokens[6]);
            pItem->textureX = std::atoi(tokens[7].c_str());
            pItem->textureY = std::atoi(tokens[8].c_str());
            pItem->texture = tokens[9];
            pItem->textureHash = std::atoi(tokens[10].c_str());
            pItem->layer = std::atoi(tokens[11].c_str());
            pItem->tileCollision = StringToTileCollision(tokens[12]);
            pItem->hardness = std::atoi(tokens[13].c_str());
            pItem->regenTime = std::atoi(tokens[14].c_str());

            lastID = pItem->ID;
            m_items.push_back(pItem);
        }

        if (tokens[0] == "setup_seed")
        {
            if (tokens.size() < 13)
            {
                // failed to parse item
                return false;
            }

            ItemInfo* pItem = CreateSeedVersionOfLastAddedItem(std::atoi(tokens[1].c_str()) - 1);
            pItem->seedBase = std::atoi(tokens[2].c_str());
            pItem->seedOver = std::atoi(tokens[3].c_str());
            pItem->treeBase = std::atoi(tokens[4].c_str());
            pItem->treeOver = std::atoi(tokens[5].c_str());
            std::vector<nova_str> seedColor = Utils::StringTokenize(tokens[7], ",");
            std::vector<nova_str> treeColor = Utils::StringTokenize(tokens[9], ",");
            if (seedColor.size() < 4 || treeColor.size() < 4)
            {
                // failed to parse seed/tree color
                delete pItem;
                return false;
            }

            pItem->seedColor = MAKE_RGBA(std::atoi(seedColor[0].c_str()), std::atoi(seedColor[1].c_str()), std::atoi(seedColor[2].c_str()), std::atoi(seedColor[3].c_str()));
            pItem->treeColor = MAKE_RGBA(std::atoi(seedColor[0].c_str()), std::atoi(seedColor[1].c_str()), std::atoi(seedColor[2].c_str()), std::atoi(seedColor[3].c_str()));
            pItem->seed1 = std::atoi(tokens[10].c_str());
            pItem->seed2 = std::atoi(tokens[11].c_str());
            pItem->bloomTime = std::atoi(tokens[12].c_str());

            lastID = pItem->ID;
            m_items.push_back(pItem);
        }

        if (tokens[0] == "add_clothes")
        {
            if (tokens.size() < 12)
            {
                // failed to parse item
                return false;
            }

            ItemInfo* pItem = new ItemInfo();
            pItem->ID = std::atoi(tokens[1].c_str());
            pItem->name = tokens[2];
            pItem->type = StringToItemType(tokens[3].c_str());
            pItem->soundType = std::atoi(tokens[4].c_str()); //sleeves
            pItem->visualType = StringToTileVisualEffect(tokens[5]);
            pItem->tileStorage = StringToTileStorage(tokens[6]);
            pItem->textureX = std::atoi(tokens[7].c_str());
            pItem->textureY = std::atoi(tokens[8].c_str());
            pItem->texture = tokens[9];
            pItem->textureHash = std::atoi(tokens[10].c_str());
            pItem->layer = 0;
            pItem->tileCollision = TILE_COLLISION_SOLID;
            pItem->hardness = 0;
            pItem->regenTime = 0;
            pItem->bodyPart = std::atoi(tokens[11].c_str());

            lastID = pItem->ID;
            m_items.push_back(pItem);
        }

        if (tokens[0] == "set_extra_string")
        {
            if (tokens.size() < 5)
            {
                return false;
            }

            ItemInfo* pItem = GetItemByID(std::atoi(tokens[1].c_str()));
            if (pItem == NULL)
            {
                return false;
            }

            pItem->textureExtra = tokens[2];
            pItem->animMS = std::atoi(tokens[4].c_str());
        }

        if (tokens[0] == "set_flags")
        {
            ItemInfo* pItem = GetItemByID(lastID);
            if (pItem == NULL)
            {
                return false;
            }

            for (size_t i = 1; i < tokens.size(); i++)
            {
                pItem->editableTypes |= StringToItemFlag(tokens[i].c_str());
            }
        }

        if (tokens[0] == "set_cook")
        {
            if (tokens.size() < 1)
            {
                return false;
            }

            ItemInfo* pItem = GetItemByID(lastID);
            if (pItem == NULL)
            {
                return false;
            }

            pItem->cookingTime = std::atoi(tokens[1].c_str());
        }

        if (tokens[0] == "force_rarity")
        {
            if (tokens.size() < 1)
            {
                return false;
            }

            ItemInfo* pItem = GetItemByID(lastID);
            if (pItem == NULL)
            {
                return false;
            }

            pItem->rarity = std::atoi(tokens[1].c_str());
        }

        if (tokens[0] == "set_max_count")
        {
            if (tokens.size() < 1)
            {
                return false;
            }

            ItemInfo* pItem = GetItemByID(lastID);
            if (pItem == NULL)
            {
                return false;
            }

            pItem->maxCount = std::atoi(tokens[1].c_str());
        }

        if (tokens[0] == "set_pet_info")
        {
            if (tokens.size() < 5)
            {
                return false;
            }

            ItemInfo* pItem = GetItemByID(lastID);
            if (pItem == NULL)
            {
                return false;
            }

            pItem->petName = tokens[1];
            pItem->petSuffix = tokens[2];
            pItem->petPrefix = tokens[3];
            pItem->petAbility = tokens[4];
        }

        if (tokens[0] == "set_anims")
        {
            if (tokens.size() < 5)
            {
                return false;
            }

            ItemInfo* pItem = GetItemByID(lastID);
            if (pItem == NULL)
            {
                return false;
            }

            pItem->animationType = std::atoi(tokens[1].c_str());
            pItem->animString = tokens[2];
            pItem->animTexture = tokens[3];
            pItem->animString2 = tokens[4];
        }

        if (tokens[0] == "set_layers")
        {
            if (tokens.size() < 3)
            {
                return false;
            }

            ItemInfo* pItem = GetItemByID(lastID);
            if (pItem == NULL)
            {
                return false;
            }

            pItem->DLayer1 = std::atoi(tokens[1].c_str());
            pItem->DLayer2 = std::atoi(tokens[2].c_str());
        }

        if (tokens[0] == "set_flags2")
        {
            ItemInfo* pItem = GetItemByID(lastID);
            if (pItem == NULL)
            {
                return false;
            }

            for (size_t i = 1; i < tokens.size(); i++)
            {
                pItem->flags |= StringToItemFlag2(tokens[i].c_str());
            }
        }

        if (tokens[0] == "set_fx_flags")
        {
            ItemInfo* pItem = GetItemByID(lastID);
            if (pItem == NULL)
            {
                return false;
            }

            for (size_t i = 1; i < tokens.size(); i++)
            {
                pItem->flags |= StringToItemFxFlag(tokens[i].c_str());
            }
        }

        if (tokens[0] == "set_client_data")
        {
            ItemInfo* pItem = GetItemByID(lastID);
            if (pItem == NULL)
            {
                return false;
            }

            std::vector<nova_str> data = Utils::StringTokenize(tokens[1], ",");
            if (data.size() != 60)
            {
                return false;
            }

            for (int i = 0; i < 9; i++)
            {
                pItem->clientData[i] = std::atoi(data[i].c_str());
            }
        }

        if (tokens[0] == "body_parts")
        {
            ItemInfo* pItem = GetItemByID(lastID);
            if (pItem == NULL)
            {
                return false;
            }

            std::vector<nova_str> parts = Utils::StringTokenize(tokens[1], ",");
            if (parts.size() != 9)
            {
                return false;
            }

            for (int i = 0; i < 9; i++)
            {
                pItem->bodyParts[i] = std::atoi(parts[i].c_str());
            }
        }

        if (tokens[0] == "set_clock")
        {
            if (tokens.size() < 1)
            {
                return false;
            }

            ItemInfo* pItem = GetItemByID(lastID);
            if (pItem == NULL)
            {
                return false;
            }

            pItem->clockDivider = std::atoi(tokens[1].c_str());
        }

        if (tokens[0] == "set_parent")
        {
            if (tokens.size() < 1)
            {
                return false;
            }

            ItemInfo* pItem = GetItemByID(lastID);
            if (pItem == NULL)
            {
                return false;
            }

            pItem->parentID = std::atoi(tokens[1].c_str());
        }

        if (tokens[0] == "tile_range")
        {
            if (tokens.size() < 1)
            {
                return false;
            }

            ItemInfo* pItem = GetItemByID(lastID);
            if (pItem == NULL)
            {
                return false;
            }

            pItem->tileRange = std::atoi(tokens[1].c_str());
        }

        if (tokens[0] == "pile_range")
        {
            if (tokens.size() < 1)
            {
                return false;
            }

            ItemInfo* pItem = GetItemByID(lastID);
            if (pItem == NULL)
            {
                return false;
            }

            pItem->pileRange = std::atoi(tokens[1].c_str());
        }

        if (tokens[0] == "custom_punch")
        {
            if (tokens.size() < 1)
            {
                return false;
            }

            ItemInfo* pItem = GetItemByID(lastID);
            if (pItem == NULL)
            {
                return false;
            }

            pItem->customPunch = tokens[1];
        }

        if (tokens[0] == "set_sit_info")
        {
            if (tokens.size() < 9)
            {
                return false;
            }

            ItemInfo* pItem = GetItemByID(lastID);
            if (pItem == NULL)
            {
                return false;
            }

            pItem->sitable = std::atoi(tokens[1].c_str());
            pItem->sitOffsetX = std::atoi(tokens[2].c_str());
            pItem->sitOffsetY = std::atoi(tokens[3].c_str());
            pItem->sitOverlayX = std::atoi(tokens[4].c_str());
            pItem->sitOverlayY = std::atoi(tokens[5].c_str());
            pItem->sitOverlayOffsetX = std::atoi(tokens[6].c_str());
            pItem->sitOverlayOffsetY = std::atoi(tokens[7].c_str());
            pItem->sitTexture = tokens[8];
        }

        if (tokens[0] == "set_render_info")
        {
            if (tokens.size() < 4)
            {
                return false;
            }

            ItemInfo* pItem = GetItemByID(lastID);
            if (pItem == NULL)
            {
                return false;
            }

            pItem->rendererFile = tokens[1];
            pItem->unwantedV17 = std::atoi(tokens[2].c_str());
            pItem->rendererHash = (uint32_t)std::atoi(tokens[3].c_str());
        }

        if (tokens[0] == "desc")
        {
            if (tokens.size() < 1)
            {
                return false;
            }

            ItemInfo* pItem = GetItemByID(lastID);
            if (pItem == NULL)
            {
                return false;
            }

            pItem->desc = tokens[1];
        }

        if (tokens[0] == "lock_power")
        {
            if (tokens.size() < 1)
            {
                return false;
            }

            ItemInfo* pItem = GetItemByID(lastID);
            if (pItem == NULL)
            {
                return false;
            }

            pItem->lockPower = std::atoi(tokens[1].c_str());
        }

        if (tokens[0] == "is_locked")
        {
            ItemInfo* pItem = GetItemByID(lastID);
            if (pItem == NULL)
            {
                return false;
            }

            pItem->bLocked = true;
        }

        if (tokens[0] == "set_element")
        {
            if (tokens.size() < 1)
            {
                return false;
            }

            ItemInfo* pItem = GetItemByID(lastID);
            if (pItem == NULL)
            {
                return false;
            }

            pItem->chi = std::atoi(tokens[1].c_str());
        }

        if (tokens[0] == "link_playmod")
        {
            if (tokens.size() < 1)
            {
                return false;
            }

            ItemInfo* pItem = GetItemByID(lastID);
            if (pItem == NULL)
            {
                return false;
            }

            pItem->playmodID = std::atoi(tokens[1].c_str());
        }
    }

    t.Kill();
    LogMsg("loaded %d items from item_definitions.txt", (int)m_items.size());
    return true;
}

bool ItemInfoManager::LoadFile()
{
    FileInstance f("items.dat");
    if (!f.IsLoaded())
    {
        LogError("failed to load items from file.");
        return false;
    }

    char *pCharData = f.GetAsChars();
    int size = f.GetSize();
    int offset = 0;

    m_data = std::vector<char>(pCharData, pCharData + size);
    m_hash = Utils::HashString(pCharData, size);
    u8 *pMem = reinterpret_cast<u8*>(m_data.data());

    MemorySerializeRaw(m_version, pMem, offset, false);
    MemorySerializeRaw(m_itemCount, pMem, offset, false);

    std::ofstream o("enum.txt");
    for (int i = 0; i < m_itemCount; i++)
    {
        ItemInfo *pItem = new ItemInfo();
        pItem->SerializeFromMem(m_version, pMem, offset);
        if (i != pItem->ID)
        {
            return false;
        }

        std::string name = pItem->name;
        std::transform(name.begin(), name.end(), name.begin(), ::toupper);
        Utils::StringReplace(" ", "_", name);
        Utils::StringReplace("_-_", "_", name);
        Utils::StringReplace("-", "_", name);
        Utils::StringReplace(":", "_", name);
        Utils::StringReplace("'", "_", name);
        Utils::StringReplace("!", "_", name);
        Utils::StringReplace("#", "_", name);
        Utils::StringReplace(".", "_", name);
        Utils::StringReplace("(", "_", name);
        Utils::StringReplace(")", "_", name);

        o << std::format("    ITEM_ID_{} = {},\n", name, pItem->ID);
        m_items.push_back(pItem);
    }

    o.close();
    m_pUpdatePacket = (GameUpdatePacket*)std::malloc(sizeof(GameUpdatePacket) + size);
    if (m_pUpdatePacket == NULL)
    {
        return false;
    }

    std::memset(m_pUpdatePacket, 0, sizeof(GameUpdatePacket) + size);
    m_pUpdatePacket->type = NET_GAME_PACKET_SEND_ITEM_DATABASE_DATA;
    m_pUpdatePacket->netID = -1;
    m_pUpdatePacket->flags |= NET_GAME_PACKET_FLAG_EXTENDED;
    m_pUpdatePacket->dataLength = size;
    nova_memcopy(&m_pUpdatePacket->data, pCharData, size, offset);

    LogMsg("loaded items from binary file(v%d).", m_version);
    DumpItemDefinitions();
    return true;
}

void ItemInfoManager::Serialize(const uint16_t& version)
{
    size_t itemsDataLen = 6;
    for (int i = 0; i < m_items.size(); i++)
    {
        ItemInfo* pItem = m_items[i];
        if (pItem == NULL)
        {
            continue;
        }

        itemsDataLen += pItem->GetMemoryEstimated(version);
    }

    int offsetIn = 0;
    uint16_t ver = version;
    int items = (int)m_items.size();

    m_pUpdatePacket = (GameUpdatePacket*)std::malloc(sizeof(GameUpdatePacket) + itemsDataLen);
    if (m_pUpdatePacket == NULL)
    {
        return;
    }

    std::memset(m_pUpdatePacket, 0, sizeof(GameUpdatePacket) + itemsDataLen);
    m_pUpdatePacket->type = NET_GAME_PACKET_SEND_ITEM_DATABASE_DATA;
    m_pUpdatePacket->netID = -1;
    m_pUpdatePacket->flags |= NET_GAME_PACKET_FLAG_EXTENDED;
    m_pUpdatePacket->dataLength = (uint32_t)itemsDataLen;

    MemorySerializeRaw(ver, m_pUpdatePacket->data, offsetIn, true);
    MemorySerializeRaw(items, m_pUpdatePacket->data, offsetIn, true);
    for (int i = 0; i < m_items.size(); i++)
    {
        ItemInfo* pItem = m_items[i];
        if (pItem == NULL)
        {
            continue;
        }

        pItem->SerializeToMem(version, m_pUpdatePacket->data, offsetIn);
    }

    m_hash = Utils::HashString(m_pUpdatePacket->data, (uint32_t)itemsDataLen);
    LogMsg("serializing items data for V%d completed, hash: %d", version, m_hash);
}

void ItemInfoManager::DumpItemDefinitions()
{
    nova_fstream defs("dumps/item_definitions.txt", nova_io::out);
    if (!defs.is_open())
    {
        LogError("failed to dump item_definitions.txt");
        return;
    }

    defs << "#add_item|tileID|name|eItemType|soundType|eVisualType|eTileStorage|textureX|textureY|texture|textureHash|layer|eTileCollision|hardness|regenTime|\n";
    defs << "#add_clothes|tileID|name|eItemType|soundType|eVisualType|eTileStorage|textureX|textureY|texture|textureHash|eClothingType|\n";
    defs << "#setup_seed|tileID|seedBase|seedOver|treeBase|treeOver|seed_color|r,g,b,a|treeColor|r,g,b,a|seed1|seed2|bloomTime|\n";
    defs << "#set_flags|FLAG |FLAG1 |FLAG2 |FLAG3 |\n";
    defs << "#set_cook|cookingTime|\n";
    defs << "#set_extra_string|tileID|extraTexture|anim_ms|<animMS>|\n";
    defs << "#force_rarity|rarity|\n";
    defs << "#set_max_count|count|\n";
    defs << "#set_pet_info|petName|petPrefix|petSuffix|petAbility|\n";
    defs << "#set_anims|animationType|animString|animTexture|animString2|\n";
    defs << "#set_layers|DLayer1|DLayer2|\n";
    defs << "#set_flags2|FLAG |FLAG1 |FLAG2 |FLAG3 |\n";
    defs << "#set_client_data|0,1,2,3,...|\n";
    defs << "#tile_range|tileRange|\n";
    defs << "#pile_range|pileRange|\n";
    defs << "#custom_punch|customPunch|\n";
    defs << "#set_fx_flags|FLAG |FLAG1 |FLAG2 |FLAG3 |\n";
    defs << "#body_parts|0,1,2,3,...|\n";
    defs << "#set_clock|clockDivider|\n";
    defs << "#set_parent|parentID|\n";
    defs << "#set_sit_info|sitable|sitOffsetX|sitOffsetY|sitOverlayX|sitOverlayY|sitOverlayOffsetX|sitOverlayOffsetY|sitTexture|\n";
    defs << "#set_render_info|rendererFile|unwantedV17|rendererHash|\n";
    defs << "#desc|description|\n";
    defs << "#lock_power|lockPower|\n";
    defs << "#set_element|eElement|\n";
    defs << "#is_locked|\n";
    defs << "#link_playmod|playmodID|\n";
    defs << "\n\n\n";

    for (int i = 0; i < m_items.size(); i++)
    {
        ItemInfo* pItem = m_items[i];
        if (pItem == NULL)
        {
            continue;
        }

        if (pItem->type == TYPE_SEED)
        {
            nova_str seedRGBA = std::to_string((int)GET_RED(pItem->seedColor)) + "," + std::to_string((int)GET_GREEN(pItem->seedColor)) + "," + std::to_string((int)GET_BLUE(pItem->seedColor)) + "," + std::to_string((int)GET_ALPHA(pItem->seedColor));
            nova_str treeRGBA = std::to_string((int)GET_RED(pItem->treeColor)) + "," + std::to_string((int)GET_GREEN(pItem->treeColor)) + "," + std::to_string((int)GET_BLUE(pItem->treeColor)) + "," + std::to_string((int)GET_ALPHA(pItem->treeColor));
            defs << "setup_seed|" + std::to_string((int)pItem->ID) + "|" + std::to_string((int)pItem->seedBase) + "|" + std::to_string((int)pItem->seedOver) + "|" + std::to_string((int)pItem->treeBase) + "|" + std::to_string((int)pItem->treeOver) + "|seedColor|" + seedRGBA + "|treeColor|" + treeRGBA + "|" + std::to_string((int)pItem->seed1) + "|" + std::to_string((int)pItem->seed2) + "|" + std::to_string((int)pItem->bloomTime) + "|\n";
        }
        else if (pItem->type == TYPE_CLOTHES)
        {
            defs << "add_clothes|" + std::to_string((int)pItem->ID) + "|" + pItem->name + "|" + ItemTypeToString(pItem->type) + "|" + std::to_string((int)pItem->soundType) + "|" + TileVisualEffectToString(pItem->visualType) + "|" + TileStorageToString(pItem->tileStorage) + "|" + std::to_string((int)pItem->textureX) + "|" + std::to_string((int)pItem->textureY) + "|" + pItem->texture + "|" + std::to_string((int)pItem->textureHash) + "|" + std::to_string((int)pItem->bodyPart) + "|\n";
        }
        else
        {
            defs << "add_item|" + std::to_string((int)pItem->ID) + "|" + pItem->name + "|" + ItemTypeToString(pItem->type) + "|" + std::to_string((int)pItem->soundType) + "|" + TileVisualEffectToString(pItem->visualType) + "|" + TileStorageToString(pItem->tileStorage) + "|" + std::to_string((int)pItem->textureX) + "|" + std::to_string((int)pItem->textureY) + "|" + pItem->texture + "|" + std::to_string((int)pItem->textureHash) + "|" + std::to_string((int)pItem->layer) + "|" + TileCollisionToString(pItem->tileCollision) + "|" + std::to_string((int)pItem->hardness) + "|" + std::to_string((int)pItem->regenTime) + "|\n";
        }

        if (pItem->editableTypes != 0)
        {
            nova_str bits = "";
            if (pItem->editableTypes & FLIPPED) bits.append(ItemFlagToString(FLIPPED) + "|");
            if (pItem->editableTypes & EDITABLE) bits.append(ItemFlagToString(EDITABLE) + "|");
            if (pItem->editableTypes & SEEDLESS) bits.append(ItemFlagToString(SEEDLESS) + "|");
            if (pItem->editableTypes & PERMANENT) bits.append(ItemFlagToString(PERMANENT) + "|");
            if (pItem->editableTypes & DROPLESS) bits.append(ItemFlagToString(DROPLESS) + "|");
            if (pItem->editableTypes & NOSELF) bits.append(ItemFlagToString(NOSELF) + "|");
            if (pItem->editableTypes & NOSHADOW) bits.append(ItemFlagToString(NOSHADOW) + "|");
            if (pItem->editableTypes & WORLD_LOCK) bits.append(ItemFlagToString(WORLD_LOCK) + "|");
            if (pItem->editableTypes & BETA) bits.append(ItemFlagToString(BETA) + "|");
            if (pItem->editableTypes & AUTOPICKUP) bits.append(ItemFlagToString(AUTOPICKUP) + "|");
            if (pItem->editableTypes & MOD) bits.append(ItemFlagToString(MOD) + "|");
            if (pItem->editableTypes & RANDGROW) bits.append(ItemFlagToString(RANDGROW) + "|");
            if (pItem->editableTypes & PUBLIC) bits.append(ItemFlagToString(PUBLIC) + "|");
            if (pItem->editableTypes & FOREGROUND) bits.append(ItemFlagToString(FOREGROUND) + "|");
            if (pItem->editableTypes & HOLIDAY) bits.append(ItemFlagToString(HOLIDAY) + "|");
            if (pItem->editableTypes & UNTRADEABLE) bits.append(ItemFlagToString(UNTRADEABLE) + "|");

            defs << std::format("set_flags|{}\n", bits);
        }

        if (pItem->cookingTime != -1)
        {
            defs << std::format("set_cook|{}|\n", (int)pItem->cookingTime);
        }

        if (!pItem->textureExtra.empty() || pItem->animMS != 400)
        {
            defs << "set_extra_string|" + std::to_string(pItem->ID) + "|" + pItem->textureExtra + "|anim_ms|" + std::to_string((int)pItem->animMS) + "|\n";
        }

        defs << std::format("force_rarity|{}|\n", (int)pItem->rarity);
        if (pItem->maxCount != 200)
        {
            defs << std::format("set_max_count|{}|\n", (int)pItem->maxCount);
        }

        if (!pItem->petName.empty() || !pItem->petPrefix.empty() || !pItem->petSuffix.empty() || !pItem->petAbility.empty())
        {
            defs << std::format("set_pet_info|{}|{}|{}|{}|\n", pItem->petName, pItem->petPrefix, pItem->petSuffix, pItem->petAbility);
        }

        if (!pItem->animString.empty() || !pItem->animTexture.empty() || !pItem->animString2.empty() || pItem->animationType != 0)
        {
            defs << "set_anims|" + std::to_string((int)pItem->animationType) + "|" + pItem->animString + "|" + pItem->animTexture + "|" + pItem->animString2 + "|\n";
        }

        if (pItem->DLayer1 != 0 || pItem->DLayer2 != 0)
        {
            defs << "set_layers|" + std::to_string((int)pItem->DLayer1) + "|" + std::to_string((int)pItem->DLayer2) + "|\n";
        }

        if (pItem->flags != 0)
        {
            nova_str bits = "";
            if (pItem->flags & ROBOT_DEADLY) bits.append(ItemFlag2ToString(ROBOT_DEADLY) + "|");
            if (pItem->flags & ROBOT_SHOOT_LEFT) bits.append(ItemFlag2ToString(ROBOT_SHOOT_LEFT) + "|");
            if (pItem->flags & ROBOT_SHOOT_RIGHT) bits.append(ItemFlag2ToString(ROBOT_SHOOT_RIGHT) + "|");
            if (pItem->flags & ROBOT_SHOOT_DOWN) bits.append(ItemFlag2ToString(ROBOT_SHOOT_DOWN) + "|");
            if (pItem->flags & ROBOT_SHOOT_UP) bits.append(ItemFlag2ToString(ROBOT_SHOOT_UP) + "|");
            if (pItem->flags & ROBOT_CAN_SHOOT) bits.append(ItemFlag2ToString(ROBOT_CAN_SHOOT) + "|");
            if (pItem->flags & ROBOT_LAVA) bits.append(ItemFlag2ToString(ROBOT_LAVA) + "|");
            if (pItem->flags & ROBOT_POINTY) bits.append(ItemFlag2ToString(ROBOT_POINTY) + "|");
            if (pItem->flags & ROBOT_SHOOT_DEADLY) bits.append(ItemFlag2ToString(ROBOT_SHOOT_DEADLY) + "|");
            if (pItem->flags & GUILD_ITEM) bits.append(ItemFlag2ToString(GUILD_ITEM) + "|");
            if (pItem->flags & GUILD_FLAG) bits.append(ItemFlag2ToString(GUILD_FLAG) + "|");
            if (pItem->flags & STARSHIP_HELP) bits.append(ItemFlag2ToString(STARSHIP_HELP) + "|");
            if (pItem->flags & STARSHIP_REACTOR) bits.append(ItemFlag2ToString(STARSHIP_REACTOR) + "|");
            if (pItem->flags & STARSHIP_VIEWSCREEN) bits.append(ItemFlag2ToString(STARSHIP_VIEWSCREEN) + "|");
            if (pItem->flags & SUPERMOD) bits.append(ItemFlag2ToString(SUPERMOD) + "|");
            if (pItem->flags & TILE_DEADLY_IF_ON) bits.append(ItemFlag2ToString(TILE_DEADLY_IF_ON) + "|");
            if (pItem->flags & LONG_HAND_ITEM) bits.append(ItemFlag2ToString(LONG_HAND_ITEM) + "|");
            if (pItem->flags & GEMLESS) bits.append(ItemFlag2ToString(GEMLESS) + "|");
            if (pItem->flags & TRANSMUTABLE) bits.append(ItemFlag2ToString(TRANSMUTABLE) + "|");
            if (pItem->flags & DUNGEON_ITEM) bits.append(ItemFlag2ToString(DUNGEON_ITEM) + "|");
            if (pItem->flags & PVE_MELEE) bits.append(ItemFlag2ToString(PVE_MELEE) + "|");
            if (pItem->flags & PVE_RANGED) bits.append(ItemFlag2ToString(PVE_RANGED) + "|");
            if (pItem->flags & PVE_AUTO_AIM) bits.append(ItemFlag2ToString(PVE_AUTO_AIM) + "|");
            if (pItem->flags & ONE_IN_WORLD) bits.append(ItemFlag2ToString(ONE_IN_WORLD) + "|");
            if (pItem->flags & ONLY_FOR_WORLD_OWNER) bits.append(ItemFlag2ToString(ONLY_FOR_WORLD_OWNER) + "|");

            defs << std::format("set_flags2|{}\n", bits);
        }

        nova_str v9 = "";
        for (int i = 0; i < 60; i++)
        {
            v9.append(std::format("{},", (int)pItem->clientData[i]));
        }

        v9.resize(v9.size() - 1);
        defs << "set_client_data|" + v9 + "\n";
        if (pItem->tileRange != 0)
        {
            defs << "tile_range|" + std::to_string((int)pItem->tileRange) + "|\n";
        }
        
        if (pItem->pileRange != 0)
        {
            defs << "pile_range|" + std::to_string((int)pItem->pileRange) + "|\n";
        }

        if (!pItem->customPunch.empty())
        {
            defs << "custom_punch|" + pItem->customPunch + "|\n";
        }

        if (pItem->fxFlags != 0)
        {
            nova_str fxBits = "";
            if (pItem->fxFlags & MULTI_ANIM_START) fxBits.append(ItemFxFlagToString(MULTI_ANIM_START) + "|");
            if (pItem->fxFlags & PING_PONG_ANIM) fxBits.append(ItemFxFlagToString(PING_PONG_ANIM) + "|");
            if (pItem->fxFlags & OVERLAY_OBJECT) fxBits.append(ItemFxFlagToString(OVERLAY_OBJECT) + "|");
            if (pItem->fxFlags & OFFSET_UP) fxBits.append(ItemFxFlagToString(OFFSET_UP) + "|");
            if (pItem->fxFlags & DUAL_LAYER) fxBits.append(ItemFxFlagToString(DUAL_LAYER) + "|");
            if (pItem->fxFlags & MULTI_ANIM2_START) fxBits.append(ItemFxFlagToString(MULTI_ANIM2_START) + "|");
            if (pItem->fxFlags & UNK_0x40) fxBits.append(ItemFxFlagToString(UNK_0x40) + "|");
            if (pItem->fxFlags & USE_SKIN_TINT) fxBits.append(ItemFxFlagToString(USE_SKIN_TINT) + "|");
            if (pItem->fxFlags & SEED_TINT_LAYER1) fxBits.append(ItemFxFlagToString(SEED_TINT_LAYER1) + "|");
            if (pItem->fxFlags & SEED_TINT_LAYER2) fxBits.append(ItemFxFlagToString(SEED_TINT_LAYER2) + "|");
            if (pItem->fxFlags & RAINBOW_TINT_LAYER1) fxBits.append(ItemFxFlagToString(RAINBOW_TINT_LAYER1) + "|");
            if (pItem->fxFlags & RAINBOW_TINT_LAYER2) fxBits.append(ItemFxFlagToString(RAINBOW_TINT_LAYER2) + "|");
            if (pItem->fxFlags & GLOW) fxBits.append(ItemFxFlagToString(GLOW) + "|");
            if (pItem->fxFlags & NO_ARMS) fxBits.append(ItemFxFlagToString(NO_ARMS) + "|");
            if (pItem->fxFlags & FRONT_ARM_PUNCH) fxBits.append(ItemFxFlagToString(FRONT_ARM_PUNCH) + "|");
            if (pItem->fxFlags & RENDER_OFFHAND) fxBits.append(ItemFxFlagToString(RENDER_OFFHAND) + "|");
            if (pItem->fxFlags & SLOWFALL_OBJECT) fxBits.append(ItemFxFlagToString(SLOWFALL_OBJECT) + "|");
            if (pItem->fxFlags & REPLACEMENT_SPRITE) fxBits.append(ItemFxFlagToString(REPLACEMENT_SPRITE) + "|");
            if (pItem->fxFlags & ORB_FLOAT) fxBits.append(ItemFxFlagToString(ORB_FLOAT) + "|");
            if (pItem->fxFlags & UNK_0x80000) fxBits.append(ItemFxFlagToString(UNK_0x80000) + "|");
            if (pItem->fxFlags & RENDER_FX_VARIANT_VERSION) fxBits.append(ItemFxFlagToString(RENDER_FX_VARIANT_VERSION) + "|");

            defs << std::format("set_fx_flags|{}\n", fxBits);
        }

        nova_str parts = "";
        for (int i = 0; i < 9; i++)
        {
            parts.append(std::format("{},", (int)pItem->bodyParts[i]));
        }

        parts.resize(parts.size() - 1);
        defs << "body_parts|" + parts + "\n";
        if (pItem->clockDivider != 0)
        {
            defs << "set_clock|" + std::to_string((int)pItem->clockDivider) + "|\n";
        }

        if (pItem->parentID != 0)
        {
            defs << "set_parent|" + std::to_string((int)pItem->parentID) + "|\n";
        }

        if (!pItem->sitTexture.empty())
        {
            defs << "set_sit_info|" + std::to_string((int)pItem->sitable) + "|" + std::to_string((int)pItem->sitOffsetX) + "|" + std::to_string((int)pItem->sitOffsetY) + "|" + std::to_string((int)pItem->sitOverlayX) + "|" + std::to_string((int)pItem->sitOverlayY) + "|" + std::to_string((int)pItem->sitOverlayOffsetX) + "|" + std::to_string((int)pItem->sitOverlayOffsetY) + "|" + pItem->sitTexture + "|\n";
        }

        if (!pItem->rendererFile.empty() || pItem->unwantedV17 != 0 || pItem->rendererHash != 0)
        {
            defs << "set_render_info|" + pItem->rendererFile + "|" + std::to_string((int)pItem->unwantedV17) + "|" + std::to_string((int)pItem->rendererHash) + "|\n";
        }

        if (pItem->lockPower != 0)
        {
            defs << "lock_power|" + std::to_string((int)pItem->lockPower) + "|\n";
        }

        if (pItem->chi != 5)
        {
            defs << "set_element|" + std::to_string((int)pItem->chi) + "|\n";
        }

        if (pItem->bLocked)
        {
            defs << "is_locked|\n";
        }

        if (pItem->playmodID != -1)
        {
            defs << "link_playmod|" + std::to_string((int)pItem->playmodID) + "|\n";
        }

        if (!pItem->desc.empty() && pItem->desc != "This item has no description.")
        {
            defs << "desc|" + pItem->desc + "|\n";
        }

        defs << "\n";
    }

    defs.close();
    LogMsg("dumped item_definitions.txt to dumps/");

    std::vector<nova_str> dumped_hashes;
    nova_fstream hashes("dumps/file_hashes.txt", nova_io::out);
    if (!hashes.is_open())
    {
        LogError("failed to dump file_hashes.txt");
        return;
    }

    hashes << "#file name|file hash|\n\n";
    for (int i = 0; i < m_items.size(); i++)
    {
        ItemInfo* pItem = m_items[i];
        if (pItem == NULL)
        {
            continue;
        }

        if (!pItem->texture.empty() && pItem->textureHash != 0)
        {
            bool bFileFound = std::find(dumped_hashes.begin(), dumped_hashes.end(), pItem->texture) != dumped_hashes.end();
            if (!bFileFound)
            {
                hashes << pItem->texture + "|" + std::to_string(pItem->textureHash) + "|\n";
                dumped_hashes.push_back(pItem->texture);
            }
        }

        if (!pItem->textureExtra.empty() && pItem->textureExtraHash != 0)
        {
            bool bFileFound = std::find(dumped_hashes.begin(), dumped_hashes.end(), pItem->textureExtra) != dumped_hashes.end();
            if (!bFileFound)
            {
                hashes << pItem->textureExtra + "|" + std::to_string(pItem->textureExtraHash) + "|\n";
                dumped_hashes.push_back(pItem->textureExtra);
            }
        }
    }

    hashes.close();
    LogMsg("dumped file_hashes.txt to dumps/");
}