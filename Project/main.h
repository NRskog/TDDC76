#ifndef MAIN_H
#define MAIN_H

#include <SFML/Graphics.hpp>
#include <stack>
#include <list>
#include <tuple>
#include "json.hpp" //I denna fil finns referens till deras git repo. 
#include <iostream>
#include <sstream>
#include <string>

//Framåt deklarationer så att order inte spelas roll. 
class Object;  
class Button;


class GameDataReader
{
    public:
        GameDataReader(const std::string & json_file);
        nlohmann::ordered_json self_jsonData;

    private:
        std::string self_json_file;
};


class Button
{
    public:
        Button(std::string file, float scale, float posX, float posY, std::string button_type, sf::Texture & button_texture);
        std::string button_flag() const;
        sf::Sprite get_sprite() const;
        void render(sf::RenderWindow & window) const;

    private:
        std::string self_file;
        float self_button_scale;
        float self_button_posX;
        float self_button_posY;
        sf::Texture & self_button_texture;
        sf::Sprite self_button_sprite; 
        std::string self_button_type;
};


class Font
{
    public:
        Font(std::string font_file, int set_text_size, float xPos, float yPos, 
        std::string content, std::vector<int> RGB_values, std::string font_flag,
        sf::Font & font, sf::Text & text);
        
        sf::Text & get_text() const;
        std::string font_flag() const;
        std::string get_content() const;
        void setContent(std::string newContent) const;
        void render(sf::RenderWindow & window) const;

    private:
        std::string self_font_file;
        int self_set_text_size;
        float self_xPos;
        float self_yPos;
        std::string self_content;
        std::vector<int> self_RGB_values;
        std::string self_font_flag;

        sf::Font & self_font;
        sf::Text & self_text;
};


class Context
{
    public: 
        Context(sf::RenderWindow & context_window, std::vector <Button> & context_button, GameDataReader & context_json_data, 
        std::vector<Font> & context_font, std::vector <Object*> & context_objects_temp,
        std::vector <int> & context_delete_objects_temp,  std::string & context_name, int context_difficulty);

        sf::RenderWindow & self_context_window;
        std::vector <Button> & self_context_button;
        GameDataReader & self_context_json_data;
        std::vector <Font> & self_context_font;

        std::vector <Object*> & self_context_objects_temp;
        std::vector <int> & self_context_delete_objects_temp; 
        std::string & self_context_name;
        sf::Clock self_context_time;
        int self_context_difficulty;
};


class State
{
    public:
        State(Context & context);
        virtual ~State() = default;
        virtual void handle(sf::Event & event, std::stack<State*> & stateStack, Context & context, State *& state_pointer) = 0;
        virtual void update(sf::Time & delta, std::stack<State*> & stateStack, Context & context) = 0;
        virtual void render(Context & context) const = 0;

    protected:
        float self_time;
        static bool death;
        sf::Vector2u self_window_size;
};


class MainMenuState: public State
{
    public:
        MainMenuState(Context & context);
        void handle(sf::Event & event, std::stack<State*> & stateStack, Context & context, State *& state_pointer) override;
        void update(sf::Time & delta, std::stack<State*> & stateStack, Context & context) override;
        void render(Context & context) const override;
        void get_mouse_pressed(sf::Event & event, std::stack<State*> & state_Stack, Context & context);
        void get_entered_text(sf::Event & event, Context & context);


    private:
        bool self_isNameWritten{false};     
};


class GameState: public State
{
    public:
        GameState(Context & context);
        void handle(sf::Event & event, std::stack<State*> & stateStack, Context & context, State *& state_pointer) override;
        void update(sf::Time & delta, std::stack<State*> & stateStack, Context & context) override;
        void render(Context & context) const override;
    
    private:
        std::vector<Object*> self_objects_vector;
        float round_time;
        int self_score;
};


class HighScoreState: public State
{
    public:
        HighScoreState(Context & context , int self_score);
        void handle(sf::Event & event, std::stack<State*> & stateStack, Context & context, State *& state_pointer) override;
        void update(sf::Time & delta, std::stack<State*> & stateStack, Context & context) override;
        void render(Context & context) const override;
        void reset_if_death(Context & context, int self_score);
        void get_mouse_pressed(std::stack<State*> & state_Stack, Context & context, State * & state_pointer);

    private:
        std::vector<std::string> self_highscore_data;
        std::string self_combine_string_data;
};


#endif