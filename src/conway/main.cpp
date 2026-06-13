#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <vector>
#include <iostream>

using std::vector;

//Compile:
//cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build --config Release

//Build:
//./build/bin/main

/* Rules - https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life#Rules */

//Future Work - Optimize update_grid()

/************************************************************/
void setup(sf::RenderWindow& window, vector<vector<bool>> &gen_a, vector<vector<bool>> &gen_b, int& c, int& r, int & generation, int & population);
void run_animation(sf::RenderWindow& window, vector<vector<bool>> &gen_a, vector<vector<bool>> &gen_b, int c, int r, int &generation, int &population);
void draw_grid(sf::RenderWindow& window, vector<vector<bool>> &gen_a, int c, int r);
void resize_clear_grid(vector<vector<bool>> &gen_a, vector<vector<bool>> &gen_b, int c, int r);
void update_grid(vector<vector<bool>>& gen_a, vector<vector<bool>>& gen_b, int& generation, int& population, int c, int r);
/************************************************************/

void setup(sf::RenderWindow& window, vector<vector<bool>> &gen_a, vector<vector<bool>> &gen_b, int& c, int& r, int & generation, int & population) {
    sf::Font font;
   // font.loadFromFile("src/conway/arial.ttf"); 
   font.loadFromFile(FONT_PATH);

    font.setSmooth( !font.isSmooth() );
    bool isMousePressed = false;
    int max_x = window.getSize().x, max_y = window.getSize().y;
    resize_clear_grid(gen_a, gen_b, c, r);

    sf::Text dims_text, enter_text, start_text;
    dims_text = {" Grid r/c size: ", font, 22};
    dims_text.setFillColor(sf::Color::Black);
    dims_text.setPosition((int)(max_x - 350.f),(int)(max_y - 100.f));
    enter_text = {" Set size", font, 22};
    enter_text.setFillColor(sf::Color::Black);
    enter_text.setPosition((int)(max_x - 145.f), (int)(max_y - 100.f));
    start_text = {" Start Animation", font, 22};
    start_text.setFillColor(sf::Color::Black);
    start_text.setPosition((int)(max_x - 350.f), (int)(max_y - 65.f));

    sf::RectangleShape dims_box(sf::Vector2f(200.f, 30.f)), enter_box(sf::Vector2f(95.f, 30.f)), start_box(sf::Vector2f(300.f, 30.f));
    dims_box.setOutlineColor(sf::Color::Black);
    dims_box.setOutlineThickness(2.f);
    dims_box.setPosition(max_x - 350.f, max_y - 100.f);
    enter_box.setOutlineColor(sf::Color::Black);
    enter_box.setOutlineThickness(2.f);
    enter_box.setPosition(max_x - 145.f, max_y - 100.f);
    enter_box.setFillColor(sf::Color{200, 200, 200});
    start_box.setOutlineColor(sf::Color::Black);
    start_box.setOutlineThickness(2.f);
    start_box.setPosition(max_x - 350.f, max_y - 65.f);
    start_box.setFillColor(sf::Color{200, 200, 200});

    std::string dims_input;

    while (window.isOpen()) {

        sf::Vector2f windowSize = window.getView().getSize(); //set size of cells based on window size
        sf::Vector2f cellSize(windowSize.x / c, windowSize.y / r);

        sf::Event event;
        while(window.pollEvent(event)) {

            if (event.type == sf::Event::Closed) {
                window.close();
            }

            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            //HIGHLIGHT BUTTON IF HOVERING OVER
            if(enter_box.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                enter_box.setFillColor(sf::Color::White);
            } else {
                enter_box.setFillColor(sf::Color{200, 200, 200});
            }
            if(start_box.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                start_box.setFillColor(sf::Color::White);
            } else {
                start_box.setFillColor(sf::Color{200, 200, 200});
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (enter_box.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)) && !dims_input.empty()) {
                    r = std::stoi(dims_input); //convert input to integers
                    c = std::stoi(dims_input);
                    resize_clear_grid(gen_a, gen_b, c, r);
                }else if (start_box.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    run_animation(window, gen_a, gen_b, c, r, generation, population);
                    break;
                }
                isMousePressed = true;
                int i = static_cast<int>((mousePos.x - 5.0f) / cellSize.x); //Toggle the cell under the mouse
                int j = static_cast<int>((mousePos.y - 5.0f) / cellSize.y);
                if (i >= 0 && i < c && j >= 0 && j < r) {
                    population = gen_a[i][j] ? population - 1 : population + 1;
                    gen_a[i][j] = !gen_a[i][j];
                }
            } else if (event.type == sf::Event::MouseButtonReleased) {
                isMousePressed = false;
            } else if (event.type == sf::Event::MouseMoved && isMousePressed) { //Continuous toggle as the mouse moves
                int i = static_cast<int>((mousePos.x - 5.0f) / cellSize.x); //Toggle the cell under the mouse
                int j = static_cast<int>((mousePos.y - 5.0f) / cellSize.y);
                if (i >= 0 && i < c && j >= 0 && j < r) {
                    population = gen_a[i][j] ? population - 1 : population + 1;
                    gen_a[i][j] = !gen_a[i][j];
                }
            }else if( event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Backspace && !dims_input.empty()) {
                dims_input.pop_back();
                dims_text.setString(" Grid r/c size: " + dims_input);
            }else if (event.type == sf::Event::TextEntered && (event.text.unicode >= 48 && event.text.unicode <= 57)) {
                dims_input += static_cast<char>(event.text.unicode);
                dims_text.setString(" Grid r/c size: " + dims_input);
            }  
        }
        window.clear();
        draw_grid(window, gen_a, c, r);
        window.draw(dims_box);
        window.draw(dims_text);
        window.draw(enter_box);
        window.draw(enter_text);
        window.draw(start_box);
        window.draw(start_text);
        window.display();
    }
}

void run_animation(sf::RenderWindow& window, vector<vector<bool>> &gen_a, vector<vector<bool>> &gen_b, int c, int r, int &generation, int &population) {
    
    sf::Font font;
    //font.loadFromFile("src/conway/arial.ttf");
    font.loadFromFile(FONT_PATH);
    font.setSmooth( !font.isSmooth() );

    sf::Text text_generation, text_population;
    text_generation.setFont(font);
    text_generation.setCharacterSize(24);
    text_generation.setFillColor(sf::Color{197, 234, 250});
    text_generation.setPosition((int)10.0f, (int)10.0f);
    text_population.setFont(font);
    text_population.setCharacterSize(24);
    text_population.setFillColor(sf::Color{197, 234, 250});
    text_population.setPosition((int)10.0f, (int)40.0f);

    sf::Clock clock;
    
    sf::Text stop_text = {" Stop Animation", font, 22};
    stop_text.setFillColor(sf::Color::Black);
    stop_text.setPosition((int)10.0f, (int)70.0f);

    sf::RectangleShape stop_box(sf::Vector2f(160.f, 30.f));
    stop_box.setOutlineColor(sf::Color::Black);
    stop_box.setOutlineThickness(2.f);
    stop_box.setPosition(10.0f, 70.0f);

    while (window.isOpen()) {

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        //if hovering, highlight button
        if(stop_box.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
            stop_box.setFillColor(sf::Color::White);
        } else {
            stop_box.setFillColor(sf::Color{200, 200, 200});
        }

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (stop_box.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    setup(window, gen_a, gen_b, c, r, generation, population);
                    break;
                }
            }
        }

        window.clear();
        draw_grid(window, gen_a, c, r);
        text_generation.setString("Generation: "+std::to_string(generation));
        text_population.setString("Population: "+std::to_string(population));
        window.draw(text_generation);
        window.draw(text_population);
        window.draw(stop_box);
        window.draw(stop_text);
        window.display();

        sf::Time elapsed = clock.getElapsedTime();
        if(elapsed.asSeconds() > 1){
            clock.restart();
            update_grid(gen_a, gen_b, generation, population, c, r);
        }
    }
}

void draw_grid(sf::RenderWindow& window, vector<vector<bool>> &gen_a, int c, int r) {
    //set size of cells basd on window size
    sf::Vector2f windowSize = window.getView().getSize();
    sf::Vector2f cellSize(windowSize.x / c, windowSize.y / r);

    //PRINT GEN_A 
    for(int i=0;i<c;i++){
        for(int j=0;j<r;j++){
            sf::RectangleShape cell;
            cell.setSize(cellSize);
            if(gen_a[i][j]){
                cell.setFillColor(sf::Color::White);
            }else{
                cell.setFillColor(sf::Color::Black);
            }
            cell.setPosition(i*cellSize.x + 5.0f, j*cellSize.y + 5.0f);
            window.draw(cell);
        }
    }
}

void resize_clear_grid(vector<vector<bool>> &gen_a, vector<vector<bool>> &gen_b, int c, int r) {
    gen_a.clear();
    gen_a.resize(r, std::vector<bool>(c, false));
    gen_b.clear();
    gen_b.resize(r, std::vector<bool>(c, false));
}

void update_grid(vector<vector<bool>>& gen_a, vector<vector<bool>>& gen_b, int& generation, int& population, int c, int r) {
    // Update the grid
    for (int i = 0; i < c; i++) {
        for (int j = 0; j < r; j++) {
            // Check the rules
            int live_neighbours = 0;

            // Count live neighbors
            if (i - 1 >= 0 && j - 1 >= 0 && gen_a[i - 1][j - 1]) live_neighbours++;
            if (i - 1 >= 0 && gen_a[i - 1][j]) live_neighbours++;
            if (i - 1 >= 0 && j + 1 < r && gen_a[i - 1][j + 1]) live_neighbours++;
            if (j - 1 >= 0 && gen_a[i][j - 1]) live_neighbours++;
            if (j + 1 < r && gen_a[i][j + 1]) live_neighbours++;
            if (i + 1 < c && j - 1 >= 0 && gen_a[i + 1][j - 1]) live_neighbours++;
            if (i + 1 < c && gen_a[i + 1][j]) live_neighbours++; 
            if (i + 1 < c && j + 1 < r && gen_a[i + 1][j + 1]) live_neighbours++;
            
            // Apply rules to update gen_b
            if (gen_a[i][j]) {
                if (live_neighbours < 2 || live_neighbours > 3) {
                    gen_b[i][j] = false;  // Cell dies due to underpopulation or overpopulation
                    population--;
                } else {
                    gen_b[i][j] = true;   // Cell survives
                }
            } else {
                if (live_neighbours == 3) {
                    gen_b[i][j] = true;   // Cell becomes alive due to reproduction
                    population++;
                } else {
                    gen_b[i][j] = false;  // Cell remains dead
                }
            }
        }
    }

    //update gen_a with gen_b
    gen_a = gen_b;

    // Increment the generation counter
    generation++;
}

//-----------------------------------------------------------

int main() {
    int columns = 100; //default value
    int rows = 100; //default value
    int generation = 1;
    int population = 0;
    sf::RenderWindow window(sf::VideoMode(800, 800), "Conway's Game of Life"); //create window
    vector<vector<bool>> gen_a, gen_b; //current state and future state
    setup(window, gen_a, gen_b, columns, rows, generation, population);
    return 0;
}
