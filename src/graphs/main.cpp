#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <vector>
#include <string>
#include <utility>
#include <iostream>

#include "annas_library/font.h"

using namespace std;

//Compile:
//cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build --config Release

//Build:
//./build/bin/main

/*
TO DO
allow directed edges 

make text pos centered in circle (sub radius off x and y)

*/
struct node{
    sf::CircleShape circle;
    sf::Text circle_name;

    string name;
    vector<node> edges;
    int x;
    int y;

    node(){
        name = "";
        edges = {};
        circle.setRadius(20.f);
        circle.setFillColor(sf::Color::Green);
      //  circle.setOutlineThickness(10.f); this is unecessary for now
      //  circle.setOutlineColor(sf::Color::Red);
    }
   
};

struct math_arrow{

    double x1, x2, y1, y2, theta, l, bx, by, cx, cy;

    //vector a is x1 x2 to y1 y2 

    math_arrow(int x1, int y1, int x2, int y2){
        this->x1 = x1;
        this->y1 = y1;
        this->x2 = x2;
        this->y2 = y2;

        theta = 45; //we define this
        l = 20; //we define this

        double vector_a_magnitude = sqrt(pow(x2-x1, 2) + pow(y2-y1, 2));

        bx = (l/vector_a_magnitude)*(cos(theta)*(x1-x2) - sin(theta)*(y1-y2))+x2;
        by = (l/vector_a_magnitude)*(sin(theta)*(x1-x2) + cos(theta)*(y1-y2))+y2;

        cx = (l/vector_a_magnitude)*(cos(theta)*(x1-x2) + sin(theta)*(y1-y2))+x2;
        cy = (l/vector_a_magnitude)*(-sin(theta)*(x1-x2) + cos(theta)*(y1-y2))+y2;
    }

};

void draw_grid(sf::RenderWindow& window, sf::Font& font, int cols, int rows) {
    /* THE FOLLOWING CODE DRAWS GRID LINES AND NUMBERS AT THE TOP AND SIDES */
    for(int i = 0; i < 800; i+=50){
        sf::Text col_text;
        col_text.setString(std::to_string(i));
        col_text.setFont(font);
        col_text.setCharacterSize(12);
        col_text.setFillColor(sf::Color::White);
        col_text.setPosition(i-to_string(i).length()*4.0/2, 0);
        sf::Vertex line[] =
        {
            sf::Vertex(sf::Vector2f(0.f, i),sf::Color(100,100,100)),
            sf::Vertex(sf::Vector2f(800.f, i), sf::Color(100,100,100))
        };
        window.draw(line, 2, sf::Lines);
        window.draw(col_text);
    }
    for(int i = 50; i < 800; i+=50){ //start at 50 since previous loop covered 0
        sf::Text row_text;
        row_text.setString(std::to_string(i));
        row_text.setFont(font);
        row_text.setCharacterSize(12);
        row_text.setFillColor(sf::Color::White);
        row_text.setPosition(0, i-10); 

        sf::Vertex line[] =
        {
            sf::Vertex(sf::Vector2f(i, 0.f), sf::Color(100,100,100)),
            sf::Vertex(sf::Vector2f(i, 800.f), sf::Color(100,100,100))
        };

        window.draw(line, 2, sf::Lines);
        window.draw(row_text);
    }
    /***************************************************************************/
}

sf::Text create_text(sf::Font& font, string text, int x, int y, int size = 12, sf::Color color = sf::Color::Magenta){
    sf::Text t;
    t.setString(text);
    t.setFont(font);
    t.setCharacterSize(size);
    t.setFillColor(color);
    t.setPosition(x, y); 
    return t;
}

void setup_nodes(sf::RenderWindow& window, vector<node>& nodes, sf::Font& font, int cols = 100, int rows = 100){

    /* DRAW BOX (BOTTOM LEFT) WITH THE FOLLOWING 
        - ADD NODE OPTION 
        - REMOVE NODE 
        - ADD/REMOVE EDGE
    */

    /* DRAW A BOX - BOTTOM LEFT */
    sf::RectangleShape box(sf::Vector2f(300.f, 350.f));
    box.setFillColor(sf::Color(100,100,100));
    box.setPosition(0, window.getSize().y - box.getSize().y);

    /* ADD NODE OPTION
        Title (centered): "Add Node"
        Text: "Name: ", Box: (input), Text: (user input if box clicked)
        Text: "X-Coord; ", Box: (input), Text: (user input if box clicked)
        Text: "Y-Coord; ", Box: (input), Text: (user input if box clicked)
        Button: "OK" (if clicked, add node to graph)
    */
    sf::Text add_node_title = create_text(font, "Add Node", 100, window.getSize().y - box.getSize().y + 20);
    sf::Text add_node_name = create_text(font, "Name: ", 10, window.getSize().y - box.getSize().y + 50);
    sf::Text add_node_x = create_text(font, "X-Coord: ", 10, window.getSize().y - box.getSize().y + 70);
    sf::Text add_node_y = create_text(font, "Y-Coord: ", 10, window.getSize().y - box.getSize().y + 90);

    //input boxes:
    sf::RectangleShape add_node_name_box(sf::Vector2f(100.f, 20.f));
    add_node_name_box.setFillColor(sf::Color::White);
    add_node_name_box.setPosition(60, window.getSize().y - box.getSize().y + 50);

    string add_node_name_input = "";
    sf::Text add_node_name_input_text = create_text(font, add_node_name_input, 60, window.getSize().y - box.getSize().y + 50);

    sf::RectangleShape x_box(sf::Vector2f(100.f, 20.f));
    x_box.setFillColor(sf::Color::White);
    x_box.setPosition(60, window.getSize().y - box.getSize().y + 70);

    string x_input = "";
    sf::Text x_input_text = create_text(font, x_input, 60, window.getSize().y - box.getSize().y + 70);

    sf::RectangleShape y_box(sf::Vector2f(100.f, 20.f));
    y_box.setFillColor(sf::Color::White);
    y_box.setPosition(60, window.getSize().y - box.getSize().y + 90);

    string y_input = "";
    sf::Text y_input_text = create_text(font, y_input, 60, window.getSize().y - box.getSize().y + 90);

    //ok button:
    sf::RectangleShape add_node_ok_button(sf::Vector2f(50.f, 20.f));
    add_node_ok_button.setFillColor(sf::Color::White);
    add_node_ok_button.setPosition(60, window.getSize().y - box.getSize().y + 110);

    sf::Text add_node_ok_text = create_text(font, "OK", 50.f, 20.f);
    add_node_ok_text.setFillColor(sf::Color::Black);
    add_node_ok_text.setPosition(60, window.getSize().y - box.getSize().y + 110);


    /* REMOVE NODE OPTION
        Title (centered): "Remove Node"
        Text: "Name: ", Box: (input), Text: (user input if box clicked)
        Button: "OK" (if clicked, remove node from graph)
    */
    sf::Text remove_node_title = create_text(font, "Remove Node", 100, window.getSize().y - box.getSize().y + 140);
    sf::Text remove_node_name = create_text(font, "Name: ", 10, window.getSize().y - box.getSize().y + 170);
    //input box:
    sf::RectangleShape remove_node_name_box(sf::Vector2f(100.f, 20.f));
    remove_node_name_box.setFillColor(sf::Color::White);
    remove_node_name_box.setPosition(60, window.getSize().y - box.getSize().y + 170);
    string remove_node_name_input = "";
    sf::Text remove_node_name_input_text = create_text(font, remove_node_name_input, 60, window.getSize().y - box.getSize().y + 170);
    //ok button:
    sf::RectangleShape remove_node_ok_button(sf::Vector2f(50.f, 20.f));
    remove_node_ok_button.setFillColor(sf::Color::White);
    remove_node_ok_button.setPosition(60, window.getSize().y - box.getSize().y + 190);
    sf::Text remove_node_ok_text = create_text(font, "OK", 50.f, 20.f);
    remove_node_ok_text.setFillColor(sf::Color::Black);
    remove_node_ok_text.setPosition(60, window.getSize().y - box.getSize().y + 190);


    /* ADD/REMOVE EDGE OPTION
        Title (centered): "Add/Remove Edge"
        Text: "Node 1: ", Box: (input), Text: (user input if box clicked)
        Text: "Node 2: ", Box: (input), Text: (user input if box clicked)
        Button: "OK" (if clicked, add edge to graph if already doesnt exist, if it does exist, remove the edge

        Buttons on the right:
        One button with double sided arrows
        One button with a single sided arrow
        One button with a single sided arrow in the opposite directoin of the previous button
    */
    // ADD/REMOVE EDGE OPTION
    sf::Text add_remove_edge_title = create_text(font, "Add/Remove Edge", 100, window.getSize().y - box.getSize().y + 220);
    sf::Text add_remove_edge_node1 = create_text(font, "Node 1: ", 10, window.getSize().y - box.getSize().y + 250);
    sf::Text add_remove_edge_node2 = create_text(font, "Node 2: ", 10, window.getSize().y - box.getSize().y + 270);
    // input boxes:
    sf::RectangleShape add_remove_edge_node1_box(sf::Vector2f(100.f, 20.f));
    add_remove_edge_node1_box.setFillColor(sf::Color::White);
    add_remove_edge_node1_box.setPosition(60, window.getSize().y - box.getSize().y + 250);
    string add_remove_edge_node1_input = "";
    sf::Text add_remove_edge_node1_input_text = create_text(font, add_remove_edge_node1_input, 60, window.getSize().y - box.getSize().y + 250);
    sf::RectangleShape add_remove_edge_node2_box(sf::Vector2f(100.f, 20.f));
    add_remove_edge_node2_box.setFillColor(sf::Color::White);
    add_remove_edge_node2_box.setPosition(60, window.getSize().y - box.getSize().y + 270);
    string add_remove_edge_node2_input = "";
    sf::Text add_remove_edge_node2_input_text = create_text(font, add_remove_edge_node2_input, 60, window.getSize().y - box.getSize().y + 270);
    // ok button:
    sf::RectangleShape add_remove_edge_ok_button(sf::Vector2f(50.f, 20.f));
    add_remove_edge_ok_button.setFillColor(sf::Color::White);
    add_remove_edge_ok_button.setPosition(60, window.getSize().y - box.getSize().y + 290);
    sf::Text add_remove_edge_ok_text = create_text(font, "OK", 50.f, 20.f);
    add_remove_edge_ok_text.setFillColor(sf::Color::Black);
    add_remove_edge_ok_text.setPosition(60, window.getSize().y - box.getSize().y + 290);

    //Button double sided arrow
    sf::Texture double_sided_arrow_png;
    if(!double_sided_arrow_png.loadFromFile(("src/graphs/Arrow 1.png"))){
        cout<<"Error loading image"<<endl;
    }
    sf::Sprite double_sided_arrow;
    double_sided_arrow.setTexture(double_sided_arrow_png);
    double_sided_arrow.setPosition(200, window.getSize().y - box.getSize().y + 250);

    sf::RectangleShape double_sided_arrow_box(sf::Vector2f(15.f, 55.f));
    double_sided_arrow_box.setFillColor(sf::Color::White);
    double_sided_arrow_box.setOutlineThickness(1.f);
    double_sided_arrow_box.setPosition(200, window.getSize().y - box.getSize().y + 250);
    //----------------------------

    //Button single sided arrow (no point up to point down)
    sf::Texture single_sided_arrow_png;
    if(!single_sided_arrow_png.loadFromFile(("src/graphs/Arrow 2.png"))){
        cout<<"Error loading image"<<endl;
    }
    sf::Sprite single_sided_arrow;
    single_sided_arrow.setTexture(single_sided_arrow_png);
    single_sided_arrow.setPosition(230, window.getSize().y - box.getSize().y + 250);

    sf::RectangleShape single_sided_arrow_box(sf::Vector2f(15.f, 55.f));
    single_sided_arrow_box.setFillColor(sf::Color::White);
    single_sided_arrow_box.setOutlineThickness(1.f);
    single_sided_arrow_box.setPosition(230, window.getSize().y - box.getSize().y + 250);

    //----------------------------

    //Button single sided arrow (point up to point down)
    sf::Texture single_sided_arrow_png2;
    if(!single_sided_arrow_png2.loadFromFile(("src/graphs/Arrow 3.png"))){
        cout<<"Error loading image"<<endl;
    }
    sf::Sprite single_sided_arrow2;
    single_sided_arrow2.setTexture(single_sided_arrow_png2);
    single_sided_arrow2.setPosition(260, window.getSize().y - box.getSize().y + 250);

    sf::RectangleShape single_sided_arrow_box2(sf::Vector2f(15.f, 55.f));
    single_sided_arrow_box2.setFillColor(sf::Color::White);
    single_sided_arrow_box2.setOutlineThickness(1.f);
    single_sided_arrow_box2.setPosition(260, window.getSize().y - box.getSize().y + 250);

    //----------------------------

    int active_arrow = 1; //set 1 as default (double sided arrow)

  // double_sided_arrow.setPosition(200, window.getSize().y - box.getSize().y + 250);

    vector<sf::RectangleShape> input_boxes = {add_node_name_box, x_box, y_box, remove_node_name_box, add_remove_edge_node1_box, add_remove_edge_node2_box};
    vector<string> input_texts = {add_node_name_input, x_input, y_input, remove_node_name_input, add_remove_edge_node1_input, add_remove_edge_node2_input};
    vector<bool> active_box = {false, false, false, false, false, false};


    while(window.isOpen()){

        sf::Event event;

        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window.close();
            }

            //Set default arrow direction to whichever is active
            if(active_arrow == 1){
                double_sided_arrow_box.setFillColor(sf::Color::Green);
                single_sided_arrow_box.setFillColor(sf::Color::White);
                single_sided_arrow_box2.setFillColor(sf::Color::White);
            }else if(active_arrow == 2){
                double_sided_arrow_box.setFillColor(sf::Color::White);
                single_sided_arrow_box.setFillColor(sf::Color::Green);
                single_sided_arrow_box2.setFillColor(sf::Color::White);
            }else if(active_arrow == 3){
                double_sided_arrow_box.setFillColor(sf::Color::White);
                single_sided_arrow_box.setFillColor(sf::Color::White);
                single_sided_arrow_box2.setFillColor(sf::Color::Green);
            }


            //if mouse clicks a box in input_boxes, set active_box to true and anything else true to false EFFICIENTLY
            if(event.type == sf::Event::MouseButtonPressed){
                for(int i = 0; i < input_boxes.size(); i++){
                    if(input_boxes[i].getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)){

                        add_node_name_box.setFillColor(sf::Color::White);
                        x_box.setFillColor(sf::Color::White);
                        y_box.setFillColor(sf::Color::White);
                        remove_node_name_box.setFillColor(sf::Color::White);
                        add_remove_edge_node1_box.setFillColor(sf::Color::White);
                        add_remove_edge_node2_box.setFillColor(sf::Color::White);

                        for(int j = 0; j < active_box.size(); j++){
                            active_box[j] = false;
                        }
                        active_box[i] = true;
                        //make it green
                        if(i == 0 && active_box[i]){
                            add_node_name_box.setFillColor(sf::Color::Green);
                        }else if(i == 1 && active_box[i]){
                            x_box.setFillColor(sf::Color::Green);
                        }else if(i == 2 && active_box[i]){
                            y_box.setFillColor(sf::Color::Green);
                        }else if(i == 3 && active_box[i]){
                            remove_node_name_box.setFillColor(sf::Color::Green);
                        }else if(i == 4 && active_box[i]){
                            add_remove_edge_node1_box.setFillColor(sf::Color::Green);
                        }else if(i == 5 && active_box[i]){
                            add_remove_edge_node2_box.setFillColor(sf::Color::Green);
                        }
                    }
                }

                if(double_sided_arrow_box.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)){
                    active_arrow = 1;
                    double_sided_arrow_box.setFillColor(sf::Color::Green);
                    single_sided_arrow_box.setFillColor(sf::Color::White);
                    single_sided_arrow_box2.setFillColor(sf::Color::White);
                }else if(single_sided_arrow_box.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)){
                    active_arrow = 2;
                    double_sided_arrow_box.setFillColor(sf::Color::White);
                    single_sided_arrow_box.setFillColor(sf::Color::Green);
                    single_sided_arrow_box2.setFillColor(sf::Color::White);
                }else if(single_sided_arrow_box2.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)){
                    active_arrow = 3;
                    double_sided_arrow_box.setFillColor(sf::Color::White);
                    single_sided_arrow_box.setFillColor(sf::Color::White);
                    single_sided_arrow_box2.setFillColor(sf::Color::Green);
                }

                //if add_node_ok button pressed, add node to graph IF the coords are in bounds 800,800
                if(add_node_ok_button.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)){

                    x_input = input_texts[1];
                    y_input = input_texts[2];
                    add_node_name_input = input_texts[0];

                    if(stoi(x_input) < 800 && stoi(y_input) < 800 && add_node_name_input.length() > 0){
                        bool nameExists = false;
                        bool positionTaken = false;
                        for(auto& n : nodes){
                            if(n.name == add_node_name_input){
                                nameExists = true;
                                break;
                            }
                            if(n.x == stoi(x_input) && n.y == stoi(y_input)){
                                positionTaken = true;
                                break;
                            }
                        }
                        if(!nameExists && !positionTaken){
                            node n;
                            n.name = add_node_name_input;
                            n.x = stoi(x_input);
                            n.y = stoi(y_input);
                            n.circle.setPosition(n.x - n.circle.getRadius(), n.y - n.circle.getRadius());
                            n.circle_name = create_text(font, n.name, n.x, n.y);
                            nodes.push_back(n);
                        }
                    }
                }

                //if remove_node_ok button pressed, remove node from graph IF the string is in the graph
                if(remove_node_ok_button.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)){
                    remove_node_name_input = input_texts[3];
                    if(remove_node_name_input.length() > 0){
                        for(int i = 0; i < nodes.size(); i++){
                            if(nodes[i].name == remove_node_name_input){
                                nodes.erase(nodes.begin() + i);
                            }
                        }
                    }
                }

                //if add_remove_edge_ok button pressed, add edge to graph IF the nodes exist. If an edge already exists, remove it
                if(add_remove_edge_ok_button.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)){
                    add_remove_edge_node1_input = input_texts[4];
                    add_remove_edge_node2_input = input_texts[5];

                    node* n1, * n2;

                    if(add_remove_edge_node1_input.length() > 0 && add_remove_edge_node2_input.length() > 0 && (add_remove_edge_node1_input != add_remove_edge_node2_input)){
                        
                        for(auto& n : nodes){
                            if(n.name.compare(add_remove_edge_node1_input) == 0){
                                n1 = &n;
                            }
                            if(n.name.compare(add_remove_edge_node2_input) == 0){
                                n2 = &n;
                            }
                        }

                    }

                    //if edges exist, remove them
                    bool exists = false;
                    for(int i = 0; i < (*n1).edges.size(); i++){
                        if((*n1).edges[i].name == (*n2).name){
                            exists = true;
                            (*n1).edges.erase((*n1).edges.begin() + i);
                        }
                    }
                    for(int i = 0; i < (*n2).edges.size(); i++){
                        if((*n2).edges[i].name == (*n1).name){
                            exists = true;
                            (*n2).edges.erase((*n2).edges.begin() + i);
                        }
                    }
                    if(!exists){
                        
                        if(active_arrow == 1){
                            (*n1).edges.push_back(*n2);
                            (*n2).edges.push_back(*n1);
                        }else if(active_arrow == 2){
                            (*n1).edges.push_back(*n2);
                        }else if(active_arrow == 3){
                            (*n2).edges.push_back(*n1);
                        }
                    }

                    for(auto n : nodes){
                        //window.draw(n.circle);
                       // window.draw(n.circle_name);
                        cout<<"curr node name: "<<n.name<<endl;
                        for(auto e : n.edges){
                            cout<<"edge name: "<<e.name<<endl;
                           // sf::Vertex line[] = {
                             //   sf::Vertex(sf::Vector2f(n.x, n.y), sf::Color::White),
                            //    sf::Vertex(sf::Vector2f(e.x, e.y), sf::Color::White)
                           // };
                            //window.draw(line, 2, sf::Lines);
                        }

                    }

                }

            }

            //if active_box is true, then input text
            if(event.type == sf::Event::TextEntered){
                for(int i = 0; i < active_box.size(); i++){

                    if(active_box[i]){
                        
                        if(event.text.unicode == 8 && input_texts[i].size() > 0){ //backspace
                            input_texts[i].pop_back();
                        }else{

                            //note - input can only be a number for i=1 and i=2
                            if(i == 1 || i == 2){
                                if(event.text.unicode >= 48 && event.text.unicode <= 57){
                                    input_texts[i] += event.text.unicode;
                                }
                            }else{
                                input_texts[i] += event.text.unicode; //just normal text
                            }
                            
                        }
                        // Update the input text on the screen
                        if(i == 0){
                            add_node_name_input_text.setString(input_texts[i]);
                        }else if(i == 1){
                            x_input_text.setString(input_texts[i]);
                        }else if(i == 2){
                            y_input_text.setString(input_texts[i]);
                        }else if(i == 3){
                            remove_node_name_input_text.setString(input_texts[i]);
                        }else if(i == 4){
                            add_remove_edge_node1_input_text.setString(input_texts[i]);
                        }else if(i == 5){
                            add_remove_edge_node2_input_text.setString(input_texts[i]);
                        }
                    }
                }
            }
        }

        window.clear();

        draw_grid(window, font, cols, rows);
        //window.clear(sf::Color::White);
        

        window.draw(box);

        //ADD NODE OPTION DRAW ------------------
        window.draw(add_node_title);
        window.draw(add_node_name);
        window.draw(add_node_x);
        window.draw(add_node_y);
        window.draw(add_node_name_box);
        window.draw(add_node_name_input_text);
        window.draw(x_box);
        window.draw(x_input_text);
        window.draw(y_box);
        window.draw(y_input_text);
        window.draw(add_node_ok_button);
        window.draw(add_node_ok_text);
        //---------------------------------------

        //REMOVE NODE OPTION DRAW ------------------
        window.draw(remove_node_title);
        window.draw(remove_node_name);
        window.draw(remove_node_name_box);
        window.draw(remove_node_name_input_text);
        window.draw(remove_node_ok_button);
        window.draw(remove_node_ok_text);
        //---------------------------------------
        
        //ADD/REMOVE EDGE OPTION DRAW ------------------
        window.draw(add_remove_edge_title);
        window.draw(add_remove_edge_node1);
        window.draw(add_remove_edge_node2);
        window.draw(add_remove_edge_node1_box);
        window.draw(add_remove_edge_node1_input_text);
        window.draw(add_remove_edge_node2_box);
        window.draw(add_remove_edge_node2_input_text);
        window.draw(add_remove_edge_ok_button);
        window.draw(add_remove_edge_ok_text);

        //draw double sided arrow
        
        window.draw(double_sided_arrow_box);
        window.draw(double_sided_arrow);

        window.draw(single_sided_arrow_box);
        window.draw(single_sided_arrow);

        window.draw(single_sided_arrow_box2);
        window.draw(single_sided_arrow2);

        //---------------------------------------
        

        for(auto n : nodes){
            window.draw(n.circle);
            window.draw(n.circle_name);

            //cout<<"curr node name: "<<n.name<<endl;
            for(auto e : n.edges){
                //cout<<"edge name: "<<e.name<<endl;

                double quantity_a = n.circle.getRadius() / sqrt(2);

                double x1 = n.x, y1 = n.y, x2 = e.x, y2 = e.y;

                

                if(n.x == e.x){
                    if(n.y > e.y){
                        y1 -= n.circle.getRadius();
                        y2 += e.circle.getRadius();
                    }else{
                        y1 += n.circle.getRadius();
                        y2 -= e.circle.getRadius();
                    }
                }else if(n.y == e.y){
                    if(n.x > e.x){
                        x1 -= n.circle.getRadius();
                        x2 += e.circle.getRadius();
                    }else{
                        x1 += n.circle.getRadius();
                        x2 -= e.circle.getRadius();
                    }
                }else{
                    if(n.x-e.x > 0){
                        x1 -= quantity_a;
                        x2 += quantity_a;
                    }else if(n.x-e.x < 0){
                        x1 += quantity_a;
                        x2 -= quantity_a;
                    }
                    if(n.y-e.y > 0){
                        y1 -= quantity_a;
                        y2 += quantity_a;
                    }else if(n.y-e.y < 0){
                        y1 += quantity_a;
                        y2 -= quantity_a;
                    }
                }

                /*
                if((n.x-e.x)>0&&(n.y-e.y)>0){
                    x1 = n.x - quantity_a;
                    y1 = n.y - quantity_a;
                    x2 = e.x + quantity_a;
                    y2 = e.y + quantity_a;
                }else if((n.x-e.x)<0&&(n.y-e.y)<0){
                    x1 = n.x + quantity_a;
                    y1 = n.y + quantity_a;
                    x2 = e.x - quantity_a;
                    y2 = e.y - quantity_a;
                }else if((n.x-e.x)>0&&(n.y-e.y)<0){
                    x1 = n.x - quantity_a;
                    y1 = n.y + quantity_a;
                    x2 = e.x + quantity_a;
                    y2 = e.y - quantity_a;
                }else if((n.x-e.x)<0&&(n.y-e.y)>0){
                    x1 = n.x + quantity_a;
                    y1 = n.y - quantity_a;
                    x2 = e.x - quantity_a;
                    y2 = e.y + quantity_a;
                }*/

                sf::Vertex line[] = {
                    //sf::Vertex(sf::Vector2f(n.x, n.y), sf::Color::White),
                    //sf::Vertex(sf::Vector2f(e.x, e.y), sf::Color::White)

                    sf::Vertex(sf::Vector2f(x1, y1), sf::Color::White),
                    sf::Vertex(sf::Vector2f(x2, y2), sf::Color::White)

                };

                math_arrow testing = math_arrow(x1, y1, x2, y2);

                sf::Vertex arrow[] = {
                sf::Vertex(sf::Vector2f(x2, y2), sf::Color::White),
                sf::Vertex(sf::Vector2f(testing.bx, testing.by), sf::Color::White)
                };
            
                window.draw(arrow, 2, sf::Lines);

                sf::Vertex arrow2[] = {
                    sf::Vertex(sf::Vector2f(x2, y2), sf::Color::White),
                    sf::Vertex(sf::Vector2f(testing.cx, testing.cy), sf::Color::White)
                };
                
                window.draw(arrow2, 2, sf::Lines);



                window.draw(line, 2, sf::Lines);

                //draw an arrow (2 lines) at the end of the line going from n to e

                
            }

            /*
            window.draw(n.circle);
            window.draw(n.circle_name);*/

        }

        window.display();

    }

    
    
}

void choose_algorithm(){
    //Do this later
    //from here choose the graphinig algorithm to simulate 
}

void run_algorithm(){
    //Do this later
    //from here run the algorithm
}

void draw_setup(sf::RenderWindow& window, sf::RectangleShape box) {
    window.draw(box);

}



int main(){

    int cols = 100, rows = 100;
    sf::RenderWindow window(sf::VideoMode(800,800), "Graphs");

    vector<node> nodes; //important
    sf::Font& font = annas_library::get_default_font();


    //testing rotation to figure out how to draw arrows
    
    /*
    
    while(window.isOpen()){
        sf::Event event;

        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window.close();
            }
        }

        window.clear();

       //line
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(200, 200), sf::Color::White),
            sf::Vertex(sf::Vector2f(200, 100), sf::Color::White)
        };
        window.draw(line, 2, sf::Lines);

        int x1 = 200;
        int y1 = 200;
        int x2 = 200;
        int y2 = 100;

        math_arrow testing = math_arrow(x1, y1, x2, y2);

        sf::Vertex arrow[] = {
            sf::Vertex(sf::Vector2f(x2, y2), sf::Color::White),
            sf::Vertex(sf::Vector2f(testing.bx, testing.by), sf::Color::White)
        };
        window.draw(arrow, 2, sf::Lines);

        sf::Vertex arrow2[] = {
            sf::Vertex(sf::Vector2f(x2, y2), sf::Color::White),
            sf::Vertex(sf::Vector2f(testing.cx, testing.cy), sf::Color::White)
        };
        window.draw(arrow2, 2, sf::Lines);
        

       // tan(theta) = x1-x0/y1-y0
       // theta = atan(x1-x0/y1-y0)

       // rotate amt = theta - 45 <- 45 can change depending on the angle of the arrow 

       // for other side of arrow, rotate by theta + 45      

        window.display();
    }
    
*/



    setup_nodes(window, nodes, font);
  //  choose_algorithm();
    //run_algorithm();

    return 0;
}