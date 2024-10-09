#ifndef AVATAR_TEST_H
#define AVATAR_TEST_H
#include "../main.h"

class Object 
{
    public:
        Object(std::string object_file, float scale, float xPos, float yPos, float speed, 
        int hp, Context & context);
        virtual ~Object() = default;
        virtual bool handle(sf::Event & event,Context & context) const = 0;
        virtual void update(sf::Time & delta, Context & context) = 0;
        virtual void render(Context & context) const = 0;
        virtual void change_hp(int i);
        
        virtual sf::FloatRect bounds() const = 0;
        bool collides(Object const* other) const;
        virtual void collision(Object * other, Context & context) = 0;
        virtual std::string object_flag(Context & context) const = 0;
        virtual void do_something(Context & context) = 0;
        virtual bool shouldRemoveObject(Context & context) const;


        std::string self_object_file;
        float self_scale;
        float self_xPos;
        float self_yPos;
        float self_speed;
        int self_hp;
        sf::Texture self_object_texture;
        sf::Sprite self_object_sprite;
        bool self_remove_object;
};


class Avatar: public Object
{
    public:
        Avatar(std::string object_file, float scale, float xPos, float yPos, float speed, 
        int hp, Context & context);
        bool handle(sf::Event & event, Context & context) const override;
        void update(sf::Time & delta, Context & context) override;
        void render(Context & context) const override;
        std::string object_flag(Context & context) const override;
        void do_something(Context & context) override ;
        sf::FloatRect bounds() const override;
        void collision(Object * other, Context & context) override;

    
        bool self_moving;
        int self_moving_dir;
        int self_score;
        float self_time_since_fire;
        float self_time_since_activate;
};


class Projectile: public Object
{
    public:
        Projectile(std::string object_file, float scale, float xPos, float yPos, float speed, 
        int hp,  Context & context, bool avatar_projectile);
        bool handle(sf::Event & event, Context & context) const override;
        void update(sf::Time & delta, Context & context) override;
        void render(Context & context) const override;
        sf::FloatRect bounds() const override;
        void collision(Object * other, Context & context) override;
        std::string object_flag(Context & context) const override;
        void do_something(Context & context) override;
        bool shouldRemoveObject(Context & context) const override;


        bool self_avatar_projectile;
};


class Shark: public Object
{
    public:
        Shark(std::vector <std::string> object_file, float scale, float xPos, float yPos, float speed, 
        int hp,  Context & context, float speed_during_game);
        bool handle(sf::Event & event, Context & context) const override;
        void update(sf::Time & delta, Context & context) override;
        void render(Context & context) const override;
        sf::FloatRect bounds() const override;
        void collision(Object * other, Context & context) override;
        std::string object_flag(Context & context) const override;
        void do_something(Context & context) override;
        bool shouldRemoveObject(Context & context) const override;
        bool randomSpwan(int chance, Context & context) const;

  
        float self_speed_during_game;
        std::vector <std::string> object_file;
        bool going_left;
};


class Shell: public Object
{
    public:
        Shell(std::vector <std::string> object_file, float scale, float xPos, float yPos, float speed,
        int hp, Context & context);

        bool handle(sf::Event & event, Context & context) const  override ;
        void update(sf::Time & delta, Context & context) override;
        void render(Context & context)const override;
        void collision(Object * other, Context & context) override;
        std::string object_flag(Context & context) const override;
        sf::FloatRect bounds() const override;
        void do_something(Context & context) override;

   
        std::vector <std::string> object_file;
};


class Octopus: public Object
{
    public:
        Octopus(std:: string object_file ,float scale, float xPos, float yPos, float speed,
        int hp, Context & context);
        bool handle(sf::Event & event, Context & context) const override ;
        void update(sf::Time & delta, Context & context) override;
        void render(Context & context)const override;
        void collision(Object * other, Context & context)  override;
        std::string object_flag(Context & context) const override;
        sf::FloatRect bounds() const override;
        void do_something(Context & context) override;

   
        void randomShoot(int y_pos, Context & context) const;
        void shoot_projectile(Context & context) const;
        int direction;
        int level {1};
        float time_since_fire;
};


class PowerUp: public Object
{
    public:
        PowerUp(std::vector <std::string> object_file ,float scale, float xPos, float yPos, float speed,
        int hp, Context & context);
        bool handle(sf::Event & event, Context & context) const override ;
        void update(sf::Time & delta, Context & context) override;
        void render(Context & context)const override;
        void collision(Object * other, Context & context)  override;
        std::string object_flag(Context & context) const override;
        sf::FloatRect bounds() const override;
        void do_something(Context & context) override;
        bool shouldRemoveObject(Context & context) const override;


        int power_type;
};



#endif