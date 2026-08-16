#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <functional>

#include "font.h"

using std::string;

struct button {

    sf::Font& font = annas_library::get_default_font();

    string text; //the string in the button
    sf::Text curr_text;
    sf::Color text_color = sf::Color::White;

    sf::RectangleShape box;
    sf::Color box_color = sf::Color::Blue;
    sf::Color box_highlight_color = sf::Color::Yellow;

    int x_pos, y_pos, width, height;

    button(string text_, int x_pos_, int y_pos_, int width_, int height_) : text(text_), x_pos(x_pos_), y_pos(y_pos_), width(width_), height(height_){

        curr_text.setFillColor(text_color);
        curr_text.setFont(font);
        curr_text.setCharacterSize(24);
        curr_text.setPosition(x_pos, y_pos);
        curr_text.setString(text);

        box.setSize(sf::Vector2f(width, height));
        box.setPosition(x_pos, y_pos);
        box.setFillColor(box_color);

        //add a border to the box
        box.setOutlineThickness(2);
        box.setOutlineColor(box_color);
    }

    //void action(); //overriden in where it is implemented
    std::function<void()> button_action;

    void draw_button(sf::RenderWindow& window){
        window.draw(box);
        window.draw(curr_text);
    }

    void check_button_events(sf::RenderWindow& window, sf::Event& event){
        sf::Vector2i mouse_position = sf::Mouse::getPosition(window);

        //highlight the box if hovering over
        if(box.getGlobalBounds().contains(static_cast<float>(mouse_position.x), static_cast<float>(mouse_position.y))) {
            box.setFillColor(box_highlight_color);
            box.setOutlineColor(box_highlight_color);
        } else {
            box.setFillColor(box_color);
            box.setOutlineColor(box_color);
        }

        if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){
            if(box.getGlobalBounds().contains(static_cast<float>(mouse_position.x), static_cast<float>(mouse_position.y))) {
                button_action();
            }
        }
    }
};