#include "game.h"
#include "object.h"
#include <iostream>
#include <cstdlib>

void Game::run() const
{
    GameDataReader GDR("data.json");
    //Bestäm random seed. 
    srand(GDR.self_jsonData["Game_seed"]["game_seed"].at(1));
    
    std::stack<State*> stateStack;
    
    std::string window_name{GDR.self_jsonData["window"]["name"]};
    sf::RenderWindow window(sf::VideoMode {GDR.self_jsonData["window"]["screen_width"]
    , GDR.self_jsonData["window"]["screen_height"]}, window_name);

    sf::Clock clock;
    
    
    //Skapar de nödvändiga vektorer 
    std::vector<Button> self_buttons{};
    std::vector<Font> self_fonts{};
    
    std::vector<sf::Texture> texture_button_vector{};
    std::vector<sf::Texture> texture_background_vector{};
    
    std::vector<sf::Font> font_vector{};
    std::vector<sf::Text> text_vector{};

    //Texture for Knappar
    sf::Texture startT{};
    sf::Texture check_name_texture{};
    sf::Texture checkmark_texture{};
    sf::Texture highscoreT{};
    sf::Texture backMenu{};
    
    //Texture for Bakgrund
    sf::Texture background_game_state{};
    sf::Texture menu_game_state{};
    sf::Texture highscore_state{};
    
    //Fonter
    sf::Font main_menu_font{};
    sf::Font highscore_font{};
    sf::Font highscore_vec_font{};
    sf::Font gamestate_font{};
    sf::Font gamestate_hp_font{};
    sf::Font score_font{};
    
    //Textures som är kopplade till fonterna. 
    sf::Text main_menu_text{};
    sf::Text highscore_text{};
    sf::Text highscore_vec_text{};
    sf::Text gamestate_text{};
    sf::Text gamestate_hp_text{};
    sf::Text score_text{};

    //Pusha in alla knappar i texture vektorn. 
    texture_button_vector.push_back(startT);
    texture_button_vector.push_back(check_name_texture);
    texture_button_vector.push_back(checkmark_texture);
    texture_button_vector.push_back(highscoreT);
    texture_button_vector.push_back(backMenu);

    //Pusha in alla bakgrunder i texture vektorn. 
    texture_background_vector.push_back(background_game_state);
    texture_background_vector.push_back(menu_game_state);
    texture_background_vector.push_back(highscore_state);

    //Pusha in alla fonter i font vektorn.
    font_vector.push_back(main_menu_font);
    font_vector.push_back(highscore_font);
    font_vector.push_back(highscore_vec_font);
    font_vector.push_back(gamestate_font);
    font_vector.push_back(gamestate_hp_font);
    font_vector.push_back(score_font);

    //Pusha in alla textures i text vektorn. 
    text_vector.push_back(main_menu_text);
    text_vector.push_back(highscore_text);
    text_vector.push_back(highscore_vec_text);
    text_vector.push_back(gamestate_text);
    text_vector.push_back(gamestate_hp_text);
    text_vector.push_back(score_text);
     
    //Addera bakgrunder till texture vektorn. 
    for(std::size_t i = 1; i <= GDR.self_jsonData["Backgrounds"].size(); ++i)
    {
        std::string current_background{"background"+std::to_string(i)};
        self_buttons.push_back(
            Button {GDR.self_jsonData["Backgrounds"][current_background]["textfile"], 
        GDR.self_jsonData["Backgrounds"][current_background]["scale"],
        GDR.self_jsonData["Backgrounds"][current_background]["xPos"],
        GDR.self_jsonData["Backgrounds"][current_background]["yPos"],
        GDR.self_jsonData["Backgrounds"][current_background]["flag"],
        texture_background_vector[i-1]}
        );
    }

    //Addera knappar till texture vektorn. 
    for(std::size_t i = 1; i <= GDR.self_jsonData["Buttons"].size(); ++i)
    {
        std::string current_button{"button"+std::to_string(i)};
        self_buttons.push_back(
            Button {GDR.self_jsonData["Buttons"][current_button]["textfile"], 
        GDR.self_jsonData["Buttons"][current_button]["scale"],
        GDR.self_jsonData["Buttons"][current_button]["xPos"],
        GDR.self_jsonData["Buttons"][current_button]["yPos"],
        GDR.self_jsonData["Buttons"][current_button]["flag"],
        texture_button_vector[i-1]}
        );
    }

    //Addera fonts till font vektorn. 
    for(std::size_t i=1; i<= GDR.self_jsonData["Fonts"].size(); ++i)
    {
        std::string current_font{"font"+std::to_string(i)};
        
        self_fonts.push_back(
            Font {GDR.self_jsonData["Fonts"][current_font]["typeface"],
        GDR.self_jsonData["Fonts"][current_font]["size"],
        GDR.self_jsonData["Fonts"][current_font]["xPos"],
        GDR.self_jsonData["Fonts"][current_font]["yPos"],
        GDR.self_jsonData["Fonts"][current_font]["content"],
        GDR.self_jsonData["Fonts"][current_font]["color"],
        GDR.self_jsonData["Fonts"][current_font]["flag"],
        font_vector[i-1], text_vector[i-1]}
        );
    }
    
    std::vector<Object*> self_context_objects; //temporär vektor
    std::vector<int> self_context_objects_delete; //För att ta bort object. 

    std::string self_context_name{""}; //Man börjar alltid med ett tomt namn. 
    int self_difficulty{0}; //Default nivå är lätt. 
    
    Context self_context{window, self_buttons, GDR, self_fonts, self_context_objects, self_context_objects_delete,
    self_context_name, self_difficulty};
    
    stateStack.push(new MainMenuState(self_context));

    State* current_state{};
    
    //Huvud main-loopen börjar här
    while (window.isOpen())
    {
        //Current state är vad som är toppen på stacken. 
        current_state = stateStack.top();
        sf::Event event;
        while (window.pollEvent(event))
        {
            current_state->handle(event, stateStack, self_context, current_state);
        }
        
        sf::Time last = clock.restart();
        
        current_state->update(last, stateStack, self_context);
        
        window.clear();
        
        current_state->render(self_context);
        
        //Sätter frame rate att max vara 60 fps. 
        window.setFramerateLimit(60);
        
        window.display();
        	
    }

    //Raderar states i stacken. 
    while (!stateStack.empty()) 
    {
        delete stateStack.top();
        stateStack.pop();
    }
}