/*
    Note:
    Vi skapade kopior av våra filer istället för att skapa onödiga getters som bara
    hade använts för testprogrammet. Hoppas detta är ok :)

*/


#include "catch.hpp"
#include "avatar_test.h"
#include "../main.h"


#include <SFML/System/Clock.hpp>

#include <stack>
#include <list>
#include <tuple>
#include "../json.hpp"
#include <iostream>
#include <sstream>      
#include <string>

using namespace std;

 GameDataReader GDR("data.json");
   
    std::stack<State*> stateStack;
    std::string window_name{GDR.self_jsonData["window"]["name"]};
    sf::RenderWindow window(sf::VideoMode {GDR.self_jsonData["window"]["screen_width"]
    , GDR.self_jsonData["window"]["screen_height"]}, window_name);

    std::vector<Button> self_buttons{};
    std::vector<Font> self_fonts{};
    
    std::vector<sf::Texture> texture_button_vector{};
    std::vector<sf::Texture> texture_background_vector{};
    
    std::vector<sf::Font> font_vector{};
    std::vector<sf::Text> text_vector{};
    
    std::vector<Object*> self_context_objects; //The temp vector
    std::vector<int> self_context_objects_delete; //For deleting stuff 
    int self_difficulty{0};
     
    std::string self_context_name{""};
    
    Context context{window, self_buttons, GDR, self_fonts, self_context_objects, self_context_objects_delete,
   self_context_name, self_difficulty};
    
 Avatar* avatar = new Avatar {
    context.self_context_json_data.self_jsonData["Objects"]["Avatar"]["textfile"],
    context.self_context_json_data.self_jsonData["Objects"]["Avatar"]["scale"],
    context.self_context_json_data.self_jsonData["Objects"]["Avatar"]["xPos"],
    context.self_context_json_data.self_jsonData["Objects"]["Avatar"]["yPos"],
    context.self_context_json_data.self_jsonData["Objects"]["Avatar"]["speed"],
    context.self_context_json_data.self_jsonData["Objects"]["Avatar"]["hp"],
    context } ;

Octopus* octopus = new Octopus {context.self_context_json_data.self_jsonData["Objects"]["Octopus"]["textfile"],
    context.self_context_json_data.self_jsonData["Objects"]["Octopus"]["scale"],
    50,
    50,
    0,
    context.self_context_json_data.self_jsonData["Objects"]["Octopus"]["hp"],
    context };
 
Object* projectile = new Projectile {context.self_context_json_data.self_jsonData["Objects"]["Projectile"]["textfile"],
    context.self_context_json_data.self_jsonData["Objects"]["Projectile"]["scale"],
    50,
    50,
    0,
    context.self_context_json_data.self_jsonData["Objects"]["Octopus"]["hp"],
    context ,true};
 
Object* projectile2 = new Projectile {context.self_context_json_data.self_jsonData["Objects"]["Projectile"]["textfile"],
    context.self_context_json_data.self_jsonData["Objects"]["Projectile"]["scale"],
    50,
    50,
    0,
    context.self_context_json_data.self_jsonData["Objects"]["Octopus"]["hp"],
    context ,false} ;

TEST_CASE("Avatar and Octopus")
{
    
    SECTION("Initial values")
    {
        CHECK(avatar->self_xPos == context.self_context_json_data.self_jsonData["Objects"]["Avatar"]["xPos"]);
        CHECK(avatar->self_yPos == context.self_context_json_data.self_jsonData["Objects"]["Avatar"]["yPos"]);
        CHECK(avatar->self_hp == context.self_context_json_data.self_jsonData["Objects"]["Avatar"]["hp"]);
        CHECK(avatar->self_speed == context.self_context_json_data.self_jsonData["Objects"]["Avatar"]["speed"]);
        CHECK(avatar->self_scale == float(context.self_context_json_data.self_jsonData["Objects"]["Avatar"]["scale"]));

    }

    SECTION("Collision")
    {
        CHECK(octopus->collides(projectile) == true);
        CHECK(projectile->collides(octopus) == true);
       /// CHECK(avatar -> collides(avatar) != true);
        CHECK(projectile->collides(avatar) != true);

        sf::Time time{};
        avatar -> collision(projectile2,context);
        avatar -> update(time,context);
        CHECK(avatar ->self_hp == 2);
        
    }

    SECTION("Shooting")
    {
    sf::Event event{};
    event.type = sf::Event::KeyPressed;
    event.key.code = sf::Keyboard::Space;
    //sf::Time  time = sf::milliseconds(1);

    if( avatar->handle(event, context) )
        {
            avatar->do_something(context);
        }

    CHECK(self_context_objects.size() == 1);
    CHECK(self_context_objects.at(0) -> object_flag(context) == "Projectile");
   
    avatar -> self_time_since_fire = 10;

    if( avatar->handle(event, context) )
        {
            avatar->do_something(context);
        }

    CHECK(self_context_objects.size() == 2);
    CHECK(self_context_objects.at(1) -> object_flag(context) == "Projectile");
    octopus -> shoot_projectile(context);
        CHECK(self_context_objects.size() == 3);
        CHECK(self_context_objects.at(2) -> object_flag(context) == "Projectile_Enemy");
    }
}

