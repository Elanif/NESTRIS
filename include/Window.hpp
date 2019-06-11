#ifndef WINDOW_H
#define WINDOW_H

#include"Input.hpp"
#include <SFML/Graphics.hpp>
#include"Engine.hpp"
#include"enums.hpp"

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
        void general_delay_manager(largest_uint target_delay);
        void smallest_delay_manager(largest_uint target_delay);
        void full_thread_delay_manager(largest_uint target_delay);
        void nothing_delay_manager(largest_uint target_delay);
        void array_delay_manager(largest_uint target_delay);
        void array_delay_manager_log(largest_uint target_delay);
        void array_delay_manager_bucket(largest_uint target_delay);
        void spam_delay_manager(largest_uint target_delay);
        void sleep(largest_uint _delay);
        largest_uint sleep_for_how_long(largest_uint _delay);
        Input inputManager;
};

#endif
