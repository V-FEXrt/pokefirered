#include "lexer.h"
#include "parser.h"

#include <iostream>
#include <map>

void sanity_test()
{
    std::vector<std::string> supported = {
        "../../include/constants/vars.h",
        "../../include/constants/weather.h",
        "../../include/constants/heal_locations.h",
        "../../include/constants/pokemon.h",
        "../../include/constants/map_types.h",
        "../../include/constants/trainer_types.h",
        "../../include/constants/event_object_movement.h",
        "../../include/constants/event_bg.h",
        "../../include/constants/region_map_sections.h",
        "../../include/constants/metatile_labels.h",
        "../../include/constants/metatile_behaviors.h",
        "../../include/constants/global.h",
        "../../include/constants/trainers.h",
        "../../include/constants/map_groups.h",
        // "../../include/constants/flags.h",
    };

    std::map<std::string, int> expected = {
        {"MAP_TYPE_SECRET_BASE", 9},
        {"TRAINER_ID_LENGTH", 4},
        {"PARTY_SIZE", 6},
        {"MULTI_PARTY_SIZE", 3},
        {"MAP_THREE_ISLAND_DUNSPARCE_TUNNEL", (34 | (2 << 8))},
        {"METATILE_GameCorner_PurpleWall_Floor", 0x2CF},
    };

    std::map<std::string, int> dfns;

    core::Lexer lexer;
    core::Parser parser;
    for (const std::string &file : supported)
    {
        auto tokens = lexer.LexFile(file);
        auto defines = parser.Parse(tokens);
        for (const auto &define : defines)
        {
            dfns[define.name()] = define.value();
        }
    }

    for (auto const &x : expected)
    {
        if (dfns[x.first] != x.second)
        {
            std::cout << "[WARNING] Mismatched value for "
                      << x.first
                      << ". Expected = " << x.second << ". Actual = " << dfns[x.first] << std::endl;
        }
    }
}

int main(int argc, char *argv[])
{
    // -- Unknown
    // ? include/constants/opponents.h
    // ? src/data/region_map/region_map_entries.h
    // ? include/constants/secret_bases.h

    // -- Array Parsing
    // src/data/object_events/object_event_graphics_info_pointers.h
    // src/data/object_events/object_event_graphics_info.h
    // src/data/object_events/object_event_pic_tables.h
    // src/data/object_events/object_event_graphics.h
    // src/data/graphics/pokemon.h
    // src/data/heal_locations.h
    // src/data/trainers.h

    // -- Define Parsing
    // #define ITEM_TO_BERRY(itemId)(((itemId - FIRST_BERRY_INDEX) + 1))
    // include/constants/items.h
    // Needs #include and string literal
    // include/constants/flags.h

    // -- Other
    // only needed for constants, but full function decl required for parsing
    // include/fieldmap.h

    // -- Done
    // include/constants/vars.h
    // include/constants/weather.h
    // include/constants/heal_locations.h
    // include/constants/pokemon.h
    // include/constants/map_types.h
    // include/constants/trainer_types.h
    // include/constants/event_object_movement.h
    // include/constants/event_bg.h
    // include/constants/region_map_sections.h
    // include/constants/metatile_labels.h
    // include/constants/metatile_behaviors.h
    // include/constants/global.h
    // include/constants/trainers.h
    // include/constants/map_groups.h

    sanity_test();

    core::Lexer lexer;
    core::Parser parser;

    auto tokens = lexer.LexFile("../../include/constants/flags.h");
    for (auto t : tokens) {
      std::cout << t.ToString() << std::endl;
    }
    // auto defines = parser.Parse(tokens);

    // for (auto statement : defines)
    // {
    //     std::cout << statement.name() << " = " << statement.value() << std::endl;
    // }

    return 0;
}
