#ifndef ANNAS_LIBRARY_FONT_H
#define ANNAS_LIBRARY_FONT_H

#include <SFML/Graphics/Font.hpp>
#include <iostream>
#include <cstdlib>
#include <string>

//FONTS_DIR is supplied by CMake as the absolute path to src/fonts
#ifndef FONTS_DIR
#error "FONTS_DIR must be defined (see CMakeLists.txt) before including annas_library/font.h"
#endif

namespace annas_library {

//loads and caches the font:
inline sf::Font& get_default_font() {
    static sf::Font font = [] {
        sf::Font f;
        if (!f.loadFromFile(std::string(FONTS_DIR) + "/arial.ttf")) { //the projects all use arial
            std::cerr << "annas_library: failed to load font: " << FONTS_DIR "/arial.ttf" << std::endl;
            std::exit(EXIT_FAILURE);
        }
        f.setSmooth(true);
        return f;
    }();
    return font;
}

}

#endif //ANNAS_LIBRARY_FONT_H
