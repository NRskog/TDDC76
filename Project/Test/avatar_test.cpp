#include "../object.h"

Object::Object(std::string object_file, float scale, float xPos, float yPos, float speed, 
int hp, Context & context)
:self_object_file{object_file}, self_scale{scale}, self_xPos{xPos}, self_yPos{yPos}, self_speed{speed},
self_hp{hp}, self_object_texture{}, self_object_sprite{}, self_remove_object{false}
{
    [[maybe_unused]] auto unused_context = context;
}

void Object::change_hp(int i)
{
    self_hp  += i;
}

    /*  Collides funktionen returnerar true om det egna  objektet överlappar med ett annat. */
bool Object::collides(Object const* other) const
{   
    return this->bounds().intersects(other->bounds());
}

    /* Returnerar true om HP = 0  */
bool Object::shouldRemoveObject(Context & context) const
{
    [[maybe_unused]] auto unused_context = context;
    //For removing objects from context vector.
    if(self_hp <= 0)
    {
        return true;
    }
    return false;
}

Avatar::Avatar(std::string object_file, float scale, float xPos, float yPos, float speed, 
int hp, Context & context)
:Object(object_file, scale, xPos, yPos, speed, hp, context), self_moving{}, self_moving_dir{}, self_score{},
self_time_since_fire{100}, self_time_since_activate{100}
{
    self_object_texture.loadFromFile(object_file);
    self_object_sprite.setTexture(self_object_texture);
    self_object_sprite.setOrigin(self_object_texture.getSize().x/2, self_object_texture.getSize().x/2);
    self_object_sprite.setScale(scale, scale);
    self_object_sprite.setPosition(xPos, yPos);
}

    /* Kontrollerar om space knappen är nedtryckt, som indikerar att avataren skjuter */
bool Avatar::handle(sf::Event & event, Context & context) const
{
    [[maybe_unused]] auto unused_context = context;
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Space)
        {
            return true;
        }
    }
    return false;
}

    /* Kontrollerar om A och D är nedtryckta, som flyttar avataren horisontellt */
void Avatar::update(sf::Time & delta, Context & context)
{
    self_time_since_fire += delta.asSeconds();

    float current_speed = delta.asSeconds() * self_speed;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    {
        // Flytta vänster
        self_object_sprite.move(-current_speed, 0);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {
        // Flytta höger
        self_object_sprite.move(current_speed, 0);
    }

    // Kontrollerar att spritens vänstra kant inte är utanför skärmen
    if (self_object_sprite.getGlobalBounds().left < context.self_context_window.getView().getCenter().x - context.self_context_window.getSize().x / 2)
    {
        self_object_sprite.setPosition(context.self_context_window.getView().getCenter().x - context.self_context_window.getSize().x / 2 + self_object_sprite.getGlobalBounds().width / 2 + 0.5, self_object_sprite.getPosition().y);
    }
    // Kontrollerar att spritens högra kant inte är utanför skärmen
    else if (self_object_sprite.getGlobalBounds().left + self_object_sprite.getGlobalBounds().width > context.self_context_window.getView().getCenter().x + context.self_context_window.getSize().x / 2)
    {
        self_object_sprite.setPosition(context.self_context_window.getView().getCenter().x + context.self_context_window.getSize().x / 2 - self_object_sprite.getGlobalBounds().width / 2 - 0.5, self_object_sprite.getPosition().y);
    }

    if (self_time_since_activate > 5)
    {   //Väntar 5 sekunder efter en powerdown, sedan blir hastigheten normal igen.
        self_speed = context.self_context_json_data.self_jsonData["Objects"]["Avatar"]["speed"];
        self_time_since_activate = 0;
    }

    if (self_speed < context.self_context_json_data.self_jsonData["Objects"]["Avatar"]["speed"])
    {
        self_time_since_activate += delta.asSeconds();
    }
}

    /*  Ritar ut avatarens sprite   */
void Avatar::render(Context & context) const
{
    context.self_context_window.draw(self_object_sprite);
    for(Font & fonts : context.self_context_font)
    {
        if (fonts.font_flag() == "hp_text")
        {
            fonts.setContent("HP: " + std::to_string(self_hp));
        }
    }
}

    /*  Returnerar objektens typ som sträng, i detta fall Avatar    */
std::string Avatar::object_flag(Context & context) const
{
    return context.self_context_json_data.self_jsonData["Objects"]["Avatar"]["flag"];
}
    /*  returnerar avatarens gräns  */
sf::FloatRect Avatar::bounds() const
{
    return self_object_sprite.getGlobalBounds();
}

void Avatar::collision(Object * other, Context & context)
{   
    //Kontrollen om avataren krockar med fiendens projektiler eller hajar resulterar i förlust av HP
    //Kontrollen om avataren krockar med Powerup/down resulterar i att dess effekt aktiveras
    if (other->object_flag(context) == "Projectile_Enemy" || other->object_flag(context) == "Shark")
    {
        self_hp -= 1;
    }

    if (other->object_flag(context) == "Power_Up")
    {
        self_hp += 1;
    }

    if (other->object_flag(context) == "Power_Down")
    {
        self_speed /= 2;
    }
}

void Avatar::do_something(Context & context)
{   
    //Gör att projektiler endast kan avfyras var 0.5:e sekund.
    if (self_time_since_fire >= 0.5)
    {
        context.self_context_objects_temp.push_back(new Projectile{
            context.self_context_json_data.self_jsonData["Objects"]["Projectile"]["textfile"],
            context.self_context_json_data.self_jsonData["Objects"]["Projectile"]["scale"],
            self_object_sprite.getPosition().x,
            self_object_sprite.getPosition().y,
            context.self_context_json_data.self_jsonData["Objects"]["Projectile"]["speed"],
            context.self_context_json_data.self_jsonData["Objects"]["Projectile"]["hp"],
            context, true 
        });
        self_time_since_fire = 0;
    }
}


Projectile::Projectile(std::string object_file, float scale, float xPos, float yPos, float speed, 
int hp,  Context & context, bool avatar_projectile)
:Object(object_file, scale, xPos, yPos, speed, hp, context), self_avatar_projectile{avatar_projectile}
{
    self_object_texture.loadFromFile(object_file);
    self_object_sprite.setTexture(self_object_texture);
    self_object_sprite.setOrigin(self_object_texture.getSize().x/2, self_object_texture.getSize().x/2);
    self_object_sprite.setScale(scale, scale);
    self_object_sprite.setPosition(xPos, yPos);
}

    /* Objekt som ej är avatar returnerar endast false, då de ej är spelar-styrda     */
bool Projectile::handle(sf::Event & event, Context & context) const
{   
    [[maybe_unused]] auto unused_event = event;
    [[maybe_unused]] auto unused_context = context;
    return false;
}

void Projectile::update(sf::Time & delta, Context & context)
{
    [[maybe_unused]] auto unused_context = context;
    //Uppdaterar position.
    self_object_sprite.move(0, -self_speed * delta.asSeconds());
}

void Projectile::render(Context & context) const
{
    context.self_context_window.draw(self_object_sprite);
}

std::string Projectile::object_flag(Context & context) const
{
    //Returnerar olika flaggor beroende på om det är fiende-projektil eller avatarens projektil.
    if (self_avatar_projectile)
    {
        return context.self_context_json_data.self_jsonData["Objects"]["Projectile"]["flag"];
    }

    return context.self_context_json_data.self_jsonData["Objects"]["Projectile_enemy"]["flag"];
}

sf::FloatRect Projectile::bounds() const
{
    return self_object_sprite.getGlobalBounds();
}

void Projectile::collision(Object * other, Context & context)
{   //Tar bort 1 HP vid kollision från diverse objekt beroende på om det är avatarens eller fiendens projektil.
    if(other->object_flag(context) != "Projectile"  && other->object_flag(context) != "Projectile_Enemy")
    {
        if (self_avatar_projectile)
        {
            if(other -> object_flag(context) != "Avatar" &&
            other -> object_flag(context) != "Power_Up" &&
            other -> object_flag(context) != "Power_Down")
            {
                self_hp -= 1;
            }
        }
        else
        {
            if( other->object_flag(context) == "Shell" || 
            other->object_flag(context) == "Avatar" )
            {
                self_hp -= 1;
            }
        }
    }
}

    /* Används endast av avataren. */
void Projectile::do_something(Context & context)
{
    [[maybe_unused]] auto unused_context = context;
}

bool Projectile::shouldRemoveObject(Context & context) const
{   
    //Flaggar för borttagning om objektet är utanför skärmen eller HP = 0. 
    if(self_hp <= 0 || self_object_sprite.getPosition().y <= 0 ||
    self_object_sprite.getPosition().y >= context.self_context_window.getSize().y)
    {
        return true;
    }
    return false;
}

Shark::Shark(std::vector <std::string> object_file, float scale, float xPos, float yPos, float speed, 
        int hp,  Context & context, float speed_during_game)
:Object(object_file.at(0), scale, xPos, yPos, speed, hp, context), self_speed_during_game{speed_during_game}, object_file{object_file},going_left{false}
{
    self_object_texture.loadFromFile(object_file.at(0));
    self_object_sprite.setTexture(self_object_texture);
    self_object_sprite.setOrigin(self_object_texture.getSize().x/2, self_object_texture.getSize().x/2);
    self_object_sprite.setScale(scale, scale);
    self_object_sprite.setPosition(xPos, yPos);
}

bool Shark::handle(sf::Event & event, Context & context) const
{
    [[maybe_unused]] auto unused_event = event;
    [[maybe_unused]] auto unused_context = context;
    return false;
}

void Shark::update(sf::Time & delta, Context & context)
{   
    if(randomSpwan(rand() % int(context.self_context_json_data.self_jsonData["Objects"]["Shark"]["probability"].at(0)), context) && self_speed == 0)
    {
        self_speed = self_speed_during_game; 
    }
    if (self_hp <= 0)
        {   
            self_object_sprite.setRotation(0.f);
            going_left = false;
            self_speed =  context.self_context_json_data.self_jsonData["Objects"]["Shark"]["speed"];
            self_object_sprite.setPosition(
            self_xPos,
            context.self_context_json_data.self_jsonData["Objects"]["Shark"]["yPos"]);
            self_hp = context.self_context_json_data.self_jsonData["Objects"]["Shark"]["hp"];

            self_object_texture.loadFromFile(object_file.at(0));
        }
    //Om hajen kommer hela vägen ned.
    if(self_object_sprite.getPosition().y > int(context.self_context_json_data.self_jsonData["Objects"]["Avatar"]["yPos"]))
    {
        if(self_object_sprite.getPosition().x < context.self_context_window.getSize().x && !going_left)
        {
            self_object_sprite.setRotation(-90.f);
            self_object_sprite.move(self_speed*3*delta.asSeconds(),0);
        }
        else
        {
            self_object_sprite.setRotation(90.f);
            going_left = true;
            self_object_sprite.move(-self_speed*3*delta.asSeconds(),0);
            if(self_object_sprite.getPosition().x < 0)
            {
                self_hp =0;
            }
        }
    }
    else
    {
        self_object_sprite.move(0, self_speed*delta.asSeconds());
    }

    if (self_hp == 1)
    {
        self_object_texture.loadFromFile(object_file.at(1));
    }
}

void Shark::render(Context & context) const
{
    context.self_context_window.draw(self_object_sprite);
}

std::string Shark::object_flag(Context & context) const
{
    return context.self_context_json_data.self_jsonData["Objects"]["Shark"]["flag"];
}

sf::FloatRect Shark::bounds() const
{
    return self_object_sprite.getGlobalBounds();
}

void Shark::collision(Object * other, Context & context)
{   //Minus två för att avataren endast ska förlora 1 HP vid kollision med haj.
    if (other->object_flag(context) == "Projectile")
    {
        self_hp -= 1;
    }

    if(other->object_flag(context) == "Avatar")
    {
        self_hp -= 2;
    }
}

void Shark::do_something(Context & context)
{
    [[maybe_unused]] auto unused_context = context;
}

bool Shark::shouldRemoveObject(Context & context) const
{
    [[maybe_unused]] auto unused_context = context;
    return false;
}

bool Shark::randomSpwan(int chance, Context & context) const
{   //Släpper hajen fri, baserat på en sannolkhet.
    if (int(context.self_context_json_data.self_jsonData["Objects"]["Shark"]["probability"].at(1 + context.self_context_difficulty)) > chance)
    {
        return true;
    }

    return false;
}

Shell::Shell(std::vector <std::string> object_file, float scale, float xPos, float yPos, float speed, 
int hp, Context & context)
:Object(object_file.at(0), scale, xPos, yPos, speed, hp, context), object_file{object_file}
{
    self_object_texture.loadFromFile(object_file.at(0));
    self_object_sprite.setTexture(self_object_texture);
    self_object_sprite.setOrigin(self_object_texture.getSize().x/2, self_object_texture.getSize().x/2);
    self_object_sprite.setScale(scale, scale);
    self_object_sprite.setPosition(xPos, yPos);
}

bool Shell::handle(sf::Event & event, Context & context) const
{
    [[maybe_unused]] auto unused_event = event;
    [[maybe_unused]] auto unused_context = context;
    return false;
}

void Shell::update(sf::Time & delta, Context & context)
{
    [[maybe_unused]] auto unused_delta = delta;
    [[maybe_unused]] auto unused_context = context;
    if(self_hp == 2)
    {
        self_object_texture.loadFromFile(object_file.at(1));
    }
    else if (self_hp == 1)
    {
        self_object_texture.loadFromFile(object_file.at(2));
    }
}

void Shell::render(Context & context) const
{
    context.self_context_window.draw(self_object_sprite);
}

sf::FloatRect Shell::bounds() const
{
    return self_object_sprite.getGlobalBounds();
}

std::string Shell::object_flag(Context & context) const
{
    return context.self_context_json_data.self_jsonData["Objects"]["Shell"]["flag"];
}

void Shell::collision(Object * other, Context & context)
{   //Kontrollerar kollision med båda projektiltyper.
    if (other->object_flag(context) == "Projectile" ||
        other->object_flag(context) ==  "Projectile_Enemy")
    {
        self_hp -= 1;
    }
}

void Shell::do_something(Context & context)
{
    [[maybe_unused]] auto unused_context = context;
}

Octopus::Octopus(std:: string object_file ,float scale, float xPos, float yPos, float speed,
int hp, Context & context)
:Object::Object(object_file, scale, xPos, yPos, speed, hp, context),direction{-1}, level{1}, time_since_fire{100}
{
    self_object_texture.loadFromFile((object_file));
    self_object_sprite.setTexture(self_object_texture);
    self_object_sprite.setOrigin(self_object_texture.getSize().x/2, self_object_texture.getSize().x/2);
    self_object_sprite.setScale(scale, scale);
    self_object_sprite.setPosition(xPos, yPos);
}
    /* Update beskriver bläckfiskens rörelse, den rör sig i en zick-zack liknande bana.  */
void Octopus::update(sf::Time & delta, Context & context)
{
    time_since_fire += delta.asSeconds();
    float current_speed {delta.asSeconds() * self_speed};
    float speed_boost{2*current_speed};
    int x_coordinate = self_object_sprite.getPosition().x; 
    int y_coordinate = self_object_sprite.getPosition().y;
    int last_row(context.self_context_window.getSize().y - (context.self_context_window.getSize().y)/2 );
    int y_limit {int(context.self_context_window.getSize().y)/10};
    int x_limit {int(context.self_context_window.getSize().x)/5};

    if (y_coordinate < last_row)
    {
        if (x_coordinate > self_xPos - x_limit && direction == -1)
        {
            self_object_sprite.move(direction*current_speed, 0);
        }
        else if ((x_coordinate <= self_xPos  - x_limit && y_coordinate <  y_limit*level) && direction == -1) //nedåt
        {
            self_object_sprite.move(0, current_speed);
        }
        else if ((x_coordinate <= self_xPos  - x_limit) && direction == -1)
        {
            level += 1;
            direction = 1;
        }
        else if ((x_coordinate < self_xPos ) && direction == 1) 
        {
            direction = 1;
            self_object_sprite.move(direction* current_speed, 0);
        }
        else if ((x_coordinate > self_xPos  - x_limit && y_coordinate <  y_limit*level) && direction == 1) //nedåt
        {
            self_object_sprite.move(0, current_speed);
        }
        else
        {
            level += 1;
            direction = -1;
        }
    }
    else 
    {   //Rör sig snabbare vid en viss y-koordinat.
        if (x_coordinate > self_xPos  - x_limit && direction == -1)
        {
            self_object_sprite.move(-speed_boost, 0);
        }
        else if ((x_coordinate <= self_xPos  - x_limit) && direction == -1)
        {
            level += 1;
            direction = 1;
        }
        else if ((x_coordinate < self_xPos ) && direction == 1)
        {
            direction = 1;
            self_object_sprite.move(speed_boost, 0);
        }
        else
        {
            level += 1;
            direction = -1;
        }
    }
    if (time_since_fire >= 1)
    {
        //Efter bläckfisken rör sig försöker den skjuta.
       randomShoot(45,context);
    }
}

void Octopus::render(Context & context) const
{
    context.self_context_window.draw(self_object_sprite);
}

bool Octopus::handle(sf::Event & event, Context & context) const
{
    [[maybe_unused]] auto unused_event = event;
    [[maybe_unused]] auto unused_context = context;
    return false;
}

std::string Octopus::object_flag(Context & context) const
{
    return context.self_context_json_data.self_jsonData["Objects"]["Octopus"]["flag"];
}

void Octopus::collision(Object * other, Context & context)
{
    if (other ->object_flag(context) == "Projectile")
    {
        self_hp -= 1;
    }
}

void Octopus::do_something(Context & context)
{
     [[maybe_unused]] auto unused_context = context;
}

sf::FloatRect Octopus::bounds() const
{
    return self_object_sprite.getGlobalBounds();
}

void Octopus::randomShoot(int y_pos, Context & context) const
{   //skjuter med en viss sannolikhet som ökar baserat på tiden som gått.
    [[maybe_unused]] auto unused_ypos = y_pos;
    int shoot_prob = rand() % int(context.self_context_json_data.self_jsonData["Objects"]["Octopus"]["probability"].at(0));
    if (shoot_prob >= int(context.self_context_json_data.self_jsonData["Objects"]["Octopus"]["probability"].at(1 + 
    context.self_context_difficulty)) - context.self_context_time.getElapsedTime().asSeconds())
    {
        shoot_projectile(context);
    }
}

void Octopus::shoot_projectile(Context & context) const
{
    context.self_context_objects_temp.push_back(new Projectile{
            context.self_context_json_data.self_jsonData["Objects"]["Projectile_enemy"]["textfile"],
            context.self_context_json_data.self_jsonData["Objects"]["Projectile_enemy"]["scale"],
            self_object_sprite.getPosition().x,
            self_object_sprite.getPosition().y,
            context.self_context_json_data.self_jsonData["Objects"]["Projectile_enemy"]["speed"].at(0 + 
            context.self_context_difficulty),
            context.self_context_json_data.self_jsonData["Objects"]["Projectile_enemy"]["hp"],
            context, false 
        });
}

PowerUp::PowerUp(std::vector <std::string> object_file, float scale, float xPos, float yPos, float speed,
int hp, Context & context)
:Object::Object(object_file.at(0), scale, xPos, yPos, speed, hp, context), power_type{}
{   //Väljer om det ska bli en powerup/powerdown
    int version = rand() % int(context.self_context_json_data.self_jsonData["Objects"]["Power"]["probability"].at(0)); 

    self_object_texture.loadFromFile(object_file.at(version));
    self_object_sprite.setTexture(self_object_texture);
    self_object_sprite.setOrigin(self_object_texture.getSize().x/2, self_object_texture.getSize().x/2);
    self_object_sprite.setScale(scale, scale);
    self_object_sprite.setPosition(xPos, yPos);
    
    if (version == 0) // POWERUP
    {
        power_type = 0;
    }
    else if (version == 1) //POWERDOWN
    {
        power_type = 1;
    }
}

void PowerUp::update(sf::Time & delta, Context & context)
{       
    [[maybe_unused]] auto unused_context = context;
    sf::Vector2f old_pos{self_object_sprite.getPosition()};
    float current_speed {delta.asSeconds() * self_speed};

    self_object_sprite.move(0, current_speed);
}

void PowerUp::render(Context & context) const
{
    context.self_context_window.draw(self_object_sprite);
}

bool PowerUp::handle(sf::Event & event, Context & context) const
{
    [[maybe_unused]] auto unused_event = event;
    [[maybe_unused]] auto unused_context = context;
    return false;
}

std::string PowerUp::object_flag(Context & context) const 
{
    return context.self_context_json_data.self_jsonData["Objects"]["Power"]["flag"].at(power_type);
}

void PowerUp::collision(Object * other, Context & context) 
{
    if (other->object_flag(context) == "Avatar")
    {
        self_hp -= 1;
    }
}

void PowerUp::do_something(Context & context)
{
     [[maybe_unused]] auto unused_context = context; 
}

bool PowerUp::shouldRemoveObject(Context & context) const
{
    if(self_hp <= 0 || self_object_sprite.getPosition().y <= 0 ||
    self_object_sprite.getPosition().y >= context.self_context_window.getSize().y)
    {
        return true;
    }
    return false;
}

sf::FloatRect PowerUp::bounds() const
{
    return self_object_sprite.getGlobalBounds();
}