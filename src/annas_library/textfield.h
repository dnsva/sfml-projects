#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>

#include "font.h"

using std::string;

struct textfield {

    sf::Font& font = annas_library::get_default_font();

    string curr_string; //current string in the box 
    sf::Text curr_text;
    sf::Color text_color = sf::Color::White;

    sf::RectangleShape box;
    sf::Color box_color = sf::Color::Blue;
    sf::Color box_highlight_color = sf::Color::Yellow;

    int x_pos, y_pos, width, height;

    
    textfield(int x_pos_, int y_pos_, int width_, int height_) : x_pos(x_pos_), y_pos(y_pos_), width(width_), height(height_){

        curr_text.setFillColor(text_color);
        curr_text.setFont(font);
        curr_text.setCharacterSize(24);
        curr_text.setPosition(x_pos, y_pos);

        box.setSize(sf::Vector2f(width, height));
        box.setPosition(x_pos, y_pos);
        box.setFillColor(box_color);

        //add a border to the box
        box.setOutlineThickness(2);
        box.setOutlineColor(box_color);
        
    }

    //void textfield_action(); //overriden in where it is implemented
    std::function<void()> textfield_action;

    void draw_textfield(sf::RenderWindow& window){
        window.draw(box);
        window.draw(curr_text);
    }

    void check_textfield_events(sf::RenderWindow& window, sf::Event& event){
        sf::Vector2i mouse_position = sf::Mouse::getPosition(window);

        
        //highlight the box if hovering over
        if(box.getGlobalBounds().contains(static_cast<float>(mouse_position.x), static_cast<float>(mouse_position.y))) {
            box.setFillColor(box_highlight_color);
            box.setOutlineColor(box_highlight_color);
        } else {
            box.setFillColor(box_color);
            box.setOutlineColor(box_color);
        }

        

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
            textfield_action();
        }else if(event.type == sf::Event::TextEntered) {
            if (event.text.unicode == 8 && !curr_string.empty()) {
                curr_string.pop_back();
                curr_text.setString(curr_string);
            }else if (event.text.unicode < 128) {
                curr_string += static_cast<char>(event.text.unicode);
                curr_text.setString(curr_string);
            }
        } 
        
    }

    // Constructor with default values
    //MyStruct(int a = 0, int b = 0) : x(a), y(b) {}
    // Default constructor
    //MyStruct() : x(0), y(0) {}
    // Parameterized constructor
    //MyStruct(int a, int b) : x(a), y(b) {}


};