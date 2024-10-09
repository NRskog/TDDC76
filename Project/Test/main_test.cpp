#include "../game.h"
#include "../object.h"
#include "../main.h"
#include <stdexcept>
#include <math.h>
#include <fstream>
#include <SFML/System/Clock.hpp>



GameDataReader::GameDataReader(const std::string & json_file)
:self_jsonData{}, self_json_file{json_file}
{
    std::ifstream file(self_json_file);
    file >> self_jsonData;
    file.close();
}

Context::Context(sf::RenderWindow & context_window, std::vector <Button> & context_button, GameDataReader & context_json_data,
std::vector <Font> & context_font, std::vector <Object*> & context_objects_temp, std::vector <int> & context_delete_objects_temp,
std::string & context_name, int context_difficulty)
:self_context_window{context_window}, self_context_button{context_button}, self_context_json_data{context_json_data},
self_context_font{context_font}, self_context_objects_temp{context_objects_temp}, self_context_delete_objects_temp{context_delete_objects_temp}, 
self_context_name{context_name}, self_context_time{}, self_context_difficulty{context_difficulty}
{}

State::State(Context & context)
:self_time{0.0f}, self_window_size{}
{
    self_window_size.x = context.self_context_json_data.self_jsonData["window"]["screen_width"];
    self_window_size.y = context.self_context_json_data.self_jsonData["window"]["screen_width"];
}

bool State::death = false; //So that it changes in every class. 

Button::Button(std::string file, float scale, float posX, float posY, std::string button_type, sf::Texture & text )
:self_file{file}, self_button_scale{scale}, self_button_posX{posX}, self_button_posY{posY}, 
self_button_texture{text}, self_button_sprite{}, self_button_type{button_type}
{
    self_button_texture.loadFromFile(self_file);
    self_button_sprite.setTexture(self_button_texture);
    self_button_sprite.setOrigin(self_button_texture.getSize().x/2, self_button_texture.getSize().y / 2);
    self_button_sprite.setScale(scale, scale);
    self_button_sprite.setPosition(self_button_posX, self_button_posY);
}

std::string Button::button_flag() const
{
    return self_button_type;
}

sf::Sprite Button::get_sprite() const
{
    return self_button_sprite;
}

void Button::render(sf::RenderWindow & window) const
{
    window.draw(self_button_sprite);
}

Font::Font(std::string font_file, int set_text_size, float xPos, float yPos, 
std::string content, std::vector<int> RGB_values, std::string font_flag,
sf::Font &font, sf::Text &text)
:self_font_file{font_file}, self_set_text_size{set_text_size}, self_xPos{xPos}, self_yPos{yPos}, 
self_content{content}, self_RGB_values{RGB_values}, self_font_flag{font_flag}, self_font{font}, self_text{text}
{
    if(!self_font.loadFromFile(font_file))
    {
        throw std::runtime_error {"File does not exist"};
    }

    self_text.setFont(self_font);
    self_text.setString(self_content);
    self_text.setCharacterSize(self_set_text_size);

    sf::FloatRect bounds { self_text.getGlobalBounds()};
    self_text.setOrigin(bounds.width / 2, bounds.height / 2);
    self_text.setPosition(self_xPos, self_yPos);
    self_text.setFillColor(sf::Color(self_RGB_values[0], self_RGB_values[1], self_RGB_values[2]));
}

sf::Text & Font::get_text() const
{
    return self_text;
}

std::string Font::font_flag() const
{
    return self_font_flag;
}

std::string Font::get_content() const
{
    return self_content;
}

void Font::setContent(std::string newContent) const
{
    self_text.setString(newContent);
}

void Font::render(sf::RenderWindow & window) const
{
    window.draw(self_text);
}

MainMenuState::MainMenuState(Context & context)
:State(context)
{}

void MainMenuState::handle(sf::Event & event, std::stack<State*> & state_Stack, Context & context, State * & state_pointer)
{
     [[maybe_unused]] auto unused_state_pointer = state_pointer;
    for(Button buttons : context.self_context_button)
    {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(context.self_context_window);

        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                if (buttons.get_sprite().getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition))
                && buttons.button_flag() == "start" && self_isNameWritten)
                {
                    state_Stack.push(new GameState(context));
                }

                if (buttons.get_sprite().getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition))
                && buttons.button_flag() == "highscore")
                {
                    state_Stack.push(new HighScoreState(context,0));
                }

                if ((buttons.get_sprite().getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition))
                    && buttons.button_flag() == "easy" && context.self_context_difficulty == 0) ||
                    (buttons.get_sprite().getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition))
                    && buttons.button_flag() == "difficult" && context.self_context_difficulty == 1))
                {
                    context.self_context_difficulty = (context.self_context_difficulty + 1) % 2;
                }

                if (buttons.get_sprite().getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition))
                && buttons.button_flag() == "check_name" && context.self_context_name != "")
                {
                    self_isNameWritten = true;
                }
            }
        }
    }
   
    if (event.type == sf::Event::Closed)
    {
        context.self_context_window.close();
    }

    if (event.type == sf::Event::TextEntered && !self_isNameWritten)
    {
        if (event.text.unicode < 128)
        {
            if (event.text.unicode == 8 && !context.self_context_name.empty())
            {
                context.self_context_name.pop_back();
            }
            else if (context.self_context_name.size() < 12)
            {
                context.self_context_name += static_cast<char>(event.text.unicode);
            }

            for (Font fonts: context.self_context_font)
            {
                if (fonts.font_flag()=="menu_text")
                {
                    fonts.get_text().setString(context.self_context_name);
                    sf::FloatRect bounds {fonts.get_text().getGlobalBounds()};
                    fonts.get_text().setOrigin(bounds.width / 2, bounds.height / 2);
                }
            }
        }
    }
}

void MainMenuState::update(sf::Time & delta, std::stack<State*> & stateStack, Context & context)
 {
    [[maybe_unused]] auto unused_stack = stateStack;
    self_time += delta.asSeconds();

    float const period { 2.0f };
    double const scale { 1.0 + 0.1*std::sin( (2 * M_PI) * self_time / period) };

    for(Font & fonts : context.self_context_font)
    {
        if (fonts.font_flag() == "menu_text")
        {
            fonts.get_text().setScale(scale, scale);
        }
    }
}

void MainMenuState::render(Context & context) const
{
    for(Button buttons : context.self_context_button)
    {
        if (buttons.button_flag() == "menu_game_state")
        {
            buttons.render(context.self_context_window);
        }
        
       if(buttons.button_flag() == "start" || buttons.button_flag() == "highscore" || 
       buttons.button_flag() == "check_name" || buttons.button_flag() == "difficulty" || 
       (buttons.button_flag() == "easy" && context.self_context_difficulty == 0) ||
       (buttons.button_flag() == "difficult" && context.self_context_difficulty == 1))
        {
            buttons.render(context.self_context_window);
        }

        if(self_isNameWritten && buttons.button_flag() == "checkmark")
        {
            buttons.render(context.self_context_window);
        }
    }
    for(Font & fonts : context.self_context_font)
    {
        if (fonts.font_flag() == "menu_text")
        {
            fonts.render(context.self_context_window);
        }
    }
}

HighScoreState::HighScoreState(Context & context,int self_score)
:State(context), self_highscore_data{}, self_combine_string_data{}
{
    for(Font & fonts : context.self_context_font)
        {
            if (fonts.font_flag() == "score_text")
            {
                fonts.setContent(fonts.get_content() + " " + std::to_string(self_score));
            }
            fonts.render(context.self_context_window);
        }

    for (std::size_t i = 1; i <= context.self_context_json_data.self_jsonData["Highscore"].size(); ++i)
     {
        
        std::string game{context.self_context_json_data.self_jsonData["Highscore"]["game"+std::to_string(i)]};
        self_highscore_data.push_back(game);
     }

    if (death)
    {
        std::string new_player_instance {};
        if (context.self_context_difficulty == 0)
        {
            new_player_instance  = context.self_context_name + " " + std::to_string(self_score) + " easy" + "\n\n";
        }
        else if (context.self_context_difficulty == 1)
        {
            new_player_instance = context.self_context_name + " " + std::to_string(self_score) + "  diff" + "\n\n";
        }

        size_t insert_position = 0;
        
        for (; insert_position < self_highscore_data.size(); ++insert_position) 
        {
            size_t space_position = self_highscore_data[insert_position].find(' ');
            int score = std::stoi(self_highscore_data[insert_position].substr(space_position + 1));
            if (self_score > score) 
            {
                break;
            }
        }

        self_highscore_data.insert(self_highscore_data.begin() + insert_position, new_player_instance);
        
        if (self_highscore_data.size() > 4) 
        {
            self_highscore_data.pop_back();
        }

        std::ifstream input_file("data.json");
        input_file >> context.self_context_json_data.self_jsonData;

        for (std::size_t i = 1; i <= self_highscore_data.size(); ++i)
        {
            context.self_context_json_data.self_jsonData["Highscore"]["game"+std::to_string(i)] = self_highscore_data[i-1];
            std::ofstream output_file("data.json");
            output_file << std::setw(4) << context.self_context_json_data.self_jsonData << std::endl;
        }
    }

    for(std::string scores : self_highscore_data)
    {
        self_combine_string_data += scores;
    }

    for(Font fonts: context.self_context_font)
    {
        if (fonts.font_flag() == "highscore_vec_text")
        {
            fonts.get_text().setString(self_combine_string_data);
        }
    }
}

void HighScoreState::handle(sf::Event & event, std::stack<State*> & state_Stack, Context & context, State * & state_pointer)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2i mousePosition = sf::Mouse::getPosition(context.self_context_window);
            for(Button buttons : context.self_context_button)
            {
                if (buttons.get_sprite().getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition))
                && buttons.button_flag() == "goback" && !death)
                {
                    delete state_Stack.top();
                    state_Stack.pop();
                    state_pointer = state_Stack.top();
                }

                if (buttons.get_sprite().getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition))
                && buttons.button_flag() == "goback" && death)
                { 
                    //Removing every state on stack to start over. Because we want a new instance.
                    delete state_Stack.top();
                    state_Stack.pop();
                    delete state_Stack.top();
                    state_Stack.pop();
                    delete state_Stack.top();
                    state_Stack.pop();
                    state_pointer = nullptr;
                    
                    context.self_context_name = "";

                    state_Stack.push(new MainMenuState(context));
                    state_pointer = state_Stack.top();

                    for (Font fonts: context.self_context_font)
                    {
                        if (fonts.font_flag()=="menu_text")
                        {fonts.get_text().setString(context.self_context_name);
                            fonts.get_text().setString(fonts.get_content());
                            //Set the origin in the middle of the text. 
                            sf::FloatRect bounds { fonts.get_text().getGlobalBounds()};
                            fonts.get_text().setOrigin(bounds.width / 2, bounds.height / 2);
                        }
                    }
                    death = false;
                }
            } 
        }
    }

    if (event.type == sf::Event::Closed)
    {
        context.self_context_window.close();
    }
}

void HighScoreState::update(sf::Time & delta, std::stack<State*> & stateStack, Context & context)
 {
    [[maybe_unused]] auto unused_stack = stateStack;
    [[maybe_unused]] auto unused_delta = delta;
    [[maybe_unused]] auto unused_context = context;
}

void HighScoreState::render(Context & context) const
{
    for(Button buttons : context.self_context_button)
    {
        if (buttons.button_flag() == "highscore_state")
        {
            buttons.render(context.self_context_window);
        }

        if (buttons.button_flag() == "goback")
        {
            buttons.render(context.self_context_window);
        }
    }

    for(Font & fonts : context.self_context_font)
    {
        if (fonts.font_flag() == "highscore_text" || fonts.font_flag() == "highscore_vec_text" || fonts.font_flag() == "score_text")
        {
            fonts.render(context.self_context_window);
        }
    }
}

GameState::GameState(Context & context)
:State(context), self_objects_vector{}, round_time{9}, self_score{0}
{   
    context.self_context_time.restart();

    self_objects_vector.push_back( new Avatar{
    context.self_context_json_data.self_jsonData["Objects"]["Avatar"]["textfile"],
    context.self_context_json_data.self_jsonData["Objects"]["Avatar"]["scale"],
    context.self_context_json_data.self_jsonData["Objects"]["Avatar"]["xPos"],
    context.self_context_json_data.self_jsonData["Objects"]["Avatar"]["yPos"],
    context.self_context_json_data.self_jsonData["Objects"]["Avatar"]["speed"],
    context.self_context_json_data.self_jsonData["Objects"]["Avatar"]["hp"],
    context } );

    self_objects_vector.push_back( new Shark{
    context.self_context_json_data.self_jsonData["Objects"]["Shark"]["textfile"],
    context.self_context_json_data.self_jsonData["Objects"]["Shark"]["scale"],
    context.self_context_json_data.self_jsonData["Objects"]["Shark"]["xPos"].at(0),
    context.self_context_json_data.self_jsonData["Objects"]["Shark"]["yPos"],
    context.self_context_json_data.self_jsonData["Objects"]["Shark"]["speed"],
    context.self_context_json_data.self_jsonData["Objects"]["Shark"]["hp"],
    context, context.self_context_json_data.self_jsonData["Objects"]["Shark"]["speed_during_game"]});

    self_objects_vector.push_back( new Shark{
    context.self_context_json_data.self_jsonData["Objects"]["Shark"]["textfile"],
    context.self_context_json_data.self_jsonData["Objects"]["Shark"]["scale"],
    context.self_context_json_data.self_jsonData["Objects"]["Shark"]["xPos"].at(1),
    context.self_context_json_data.self_jsonData["Objects"]["Shark"]["yPos"],
    context.self_context_json_data.self_jsonData["Objects"]["Shark"]["speed"],
    context.self_context_json_data.self_jsonData["Objects"]["Shark"]["hp"],
    context, context.self_context_json_data.self_jsonData["Objects"]["Shark"]["speed_during_game"]});
    
    self_objects_vector.push_back( new Shell{
    context.self_context_json_data.self_jsonData["Objects"]["Shell"]["textfile"],
    context.self_context_json_data.self_jsonData["Objects"]["Shell"]["scale"],
    (context.self_context_window.getSize().x)*(static_cast<float>(0.125)),
    context.self_context_json_data.self_jsonData["Objects"]["Shell"]["yPos"],
    context.self_context_json_data.self_jsonData["Objects"]["Shell"]["speed"],
    context.self_context_json_data.self_jsonData["Objects"]["Shell"]["hp"],
    context } );


    self_objects_vector.push_back( new Shell{
    context.self_context_json_data.self_jsonData["Objects"]["Shell"]["textfile"],
    context.self_context_json_data.self_jsonData["Objects"]["Shell"]["scale"],
    (context.self_context_window.getSize().x) *(static_cast<float>(0.5)),
    context.self_context_json_data.self_jsonData["Objects"]["Shell"]["yPos"],
    context.self_context_json_data.self_jsonData["Objects"]["Shell"]["speed"],
    context.self_context_json_data.self_jsonData["Objects"]["Shell"]["hp"],
    context } );

        
    self_objects_vector.push_back( new Shell{
    context.self_context_json_data.self_jsonData["Objects"]["Shell"]["textfile"],
    context.self_context_json_data.self_jsonData["Objects"]["Shell"]["scale"],
   (context.self_context_window.getSize().x)*(static_cast<float>(0.875)) ,
    context.self_context_json_data.self_jsonData["Objects"]["Shell"]["yPos"],
    context.self_context_json_data.self_jsonData["Objects"]["Shell"]["speed"],
    context.self_context_json_data.self_jsonData["Objects"]["Shell"]["hp"],
    context } );
}


void GameState::handle(sf::Event & event, std::stack<State*> & state_Stack,Context & context, State *& state_pointer)
{   
    [[maybe_unused]] auto unused_stack = state_Stack;
    [[maybe_unused]] auto unused_state_pointer = state_pointer;
    
    if (event.type == sf::Event::Closed)
    {
        death = true;
        context.self_context_window.close();
    }
    
    for (Object* object : self_objects_vector)
    {
        if( object->handle(event, context) )
        {
            object->do_something(context);
        }
    }
}

void GameState::update(sf::Time & delta, std::stack<State*> & stateStack, Context & context)
 {
    self_score = context.self_context_time.getElapsedTime().asSeconds();

    if (self_objects_vector.at(0)->object_flag(context) != "Avatar")
    {
        death = true;
    }

    if(death )
    {
        for(auto i{0u}; i < self_objects_vector.size(); i++)
        {   
            delete self_objects_vector.at(i);
        }
    
        self_objects_vector.clear();
        stateStack.push(new HighScoreState(context, self_score));
    }

    for (Object* objects : self_objects_vector)
    {
        objects->update(delta, context);
    }

    for (Object* context_object : context.self_context_objects_temp)
    {
        self_objects_vector.push_back(context_object);
    }

    for (std::size_t i{0} ; i < self_objects_vector.size(); i++)
    {
        if (self_objects_vector.at(i)->shouldRemoveObject(context))
        {
            context.self_context_delete_objects_temp.push_back(i);
        }
    }
    
    for (std::size_t i  {context.self_context_delete_objects_temp.size()}; i !=0 ; i--)
    {

        delete self_objects_vector.at(context.self_context_delete_objects_temp.at(i-1));
        self_objects_vector.erase(self_objects_vector.begin() +
        context.self_context_delete_objects_temp.at(i-1));
    }
    
    //To clear the context vector
    context.self_context_objects_temp.clear();
    context.self_context_delete_objects_temp.clear();

     //More optimized collision. Now only checks all pairs.
    for (unsigned i {0}; i < self_objects_vector.size(); ++i)
    {
        for (unsigned j {i+1}; j < self_objects_vector.size(); ++j)
        {
            if (self_objects_vector.at(i)->collides(self_objects_vector.at(j)))
            {  
                self_objects_vector.at(i)->collision(self_objects_vector.at(j), context);
                self_objects_vector.at(j)->collision(self_objects_vector.at(i), context);
            }
        }
    }
    
    round_time = round_time + delta.asSeconds();
    if(round_time >= 10)
    {
        for(int i{-1};i<4;i++)
        {
            context.self_context_objects_temp.push_back( new Octopus{
            context.self_context_json_data.self_jsonData["Objects"]["Octopus"]["textfile"],
            context.self_context_json_data.self_jsonData["Objects"]["Octopus"]["scale"],
            int(context.self_context_json_data.self_jsonData["Objects"]["Octopus"]["xPos"])+ static_cast<float>(i*90.0),
            context.self_context_json_data.self_jsonData["Objects"]["Octopus"]["yPos"],
            context.self_context_json_data.self_jsonData["Objects"]["Octopus"]["speed"],
            context.self_context_json_data.self_jsonData["Objects"]["Octopus"]["hp"],
            context } );

            round_time = 0;
        }
    }
    
    int chance_number = rand() % int(context.self_context_json_data.self_jsonData["Objects"]["Power"]["probability"].at(1));
        //This is the number we use to decide if the power spawns or not.
    int spawn_frame = rand() % self_window_size.x-200;
        //This is the number we use to decide what y-coordinate it spawns at.

    if (chance_number >= context.self_context_json_data.self_jsonData["Objects"]["Power"]["probability"].at(2))
    {
        self_objects_vector.push_back( new PowerUp{
        context.self_context_json_data.self_jsonData["Objects"]["Power"]["textfile"],
        context.self_context_json_data.self_jsonData["Objects"]["Power"]["scale"],
        int(context.self_context_json_data.self_jsonData["Objects"]["Power"]["xPos"]) +static_cast<float>(spawn_frame),
        context.self_context_json_data.self_jsonData["Objects"]["Power"]["yPos"],
        context.self_context_json_data.self_jsonData["Objects"]["Power"]["speed"],
        context.self_context_json_data.self_jsonData["Objects"]["Power"]["hp"],
        context } );
    }

}

void GameState::render(Context & context) const
{   
    for(Button buttons : context.self_context_button)
    {
        if (buttons.button_flag() == "background_game_state")
        {
            buttons.render(context.self_context_window);
        }
    }
    
    for (Object * objects : self_objects_vector)
    {
        objects -> render(context);
    }
    
    for(Font & fonts : context.self_context_font)
    {
        if (fonts.font_flag() == "game_text" || fonts.font_flag() == "hp_text")
        {
            fonts.render(context.self_context_window);
        }

        if (fonts.font_flag() == "game_text")
        {
            fonts.setContent("Score: " + std::to_string(self_score));
        }
    }
}

/*
int main()
{
   Game game{};
   game.run();
} 
*/


