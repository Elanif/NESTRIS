#ifndef WINDOW_H
#define WINDOW_H

#include"Input.h"
#include <SFML/Graphics.hpp>
#include"Engine.h"
#include<cstdio>

class Window : public sf::NonCopyable
{
    public:
        enum OPT {
            GENERAL,
            SMALLEST,
            SPAM,
            FULL,
            NOTHING,
            ARRAY,
            ARRAYLOG,
            ARRAYBUCKET
        };
        Window(const std::size_t& width, const std::size_t& height, sf::Vector2f _scale, const OPT& optimized);
    private:
        void general_delay_manager(sf::Int64 target_delay);
        void smallest_delay_manager(sf::Int64 target_delay);
        void full_thread_delay_manager(sf::Int64 target_delay);
        void nothing_delay_manager(sf::Int64 target_delay);
        void array_delay_manager(sf::Int64 target_delay);
        void array_delay_manager_log(sf::Int64 target_delay);
        void array_delay_manager_bucket(sf::Int64 target_delay);
        void spam_delay_manager(sf::Int64 target_delay);
        void sleep(sf::Int64 _delay);
        sf::Int64 sleep_for_how_long(sf::Int64 _delay);
        Input inputManager;
};

#endif
