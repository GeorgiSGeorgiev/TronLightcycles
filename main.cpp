// Created by: Georgi S. Georgiev
// All images and the complete game design were created by me.
// Inspired by different Tron designs. I used some of the original Tron colors.
// The "TRON" franchise is owned by DISNEY ENTERPRISES, Inc.

#include "TheGrid.h" // contains the "SFML/Graphics.hpp" import
#include "Lightcycle.h"
#include "Collidable.h"
#include "AI.h"
#include "MainMenu.h"
#include <iostream>
#include <vector>

using std::vector;
using std::shared_ptr;

enum class State { InTheMainMenu, Playing }; // Main game state.

/* Handles the lightcycle keyboard key-press controlls.
*/
void handle_lightcycle_key_press(const sf::Event& the_event, Lightcycle& lightcycle) {
    if ((the_event.key.code == sf::Keyboard::Right || the_event.key.code == sf::Keyboard::D) &&
        !lightcycle.turned_right && !lightcycle.turned_left) {
        lightcycle.turned_right = true;
        lightcycle.turn_right();

    }
    if ((the_event.key.code == sf::Keyboard::Left || the_event.key.code == sf::Keyboard::A) &&
        !lightcycle.turned_right && !lightcycle.turned_left) {
        lightcycle.turned_left = true;
        lightcycle.turn_left();
    }
    if ((the_event.key.code == sf::Keyboard::Up || the_event.key.code == sf::Keyboard::W) &&
        !lightcycle.accelerated && !lightcycle.slowed_down) {
        lightcycle.accelerated = true;
    }
    if ((the_event.key.code == sf::Keyboard::Down || the_event.key.code == sf::Keyboard::S) &&
        !lightcycle.slowed_down && !lightcycle.accelerated) {
        lightcycle.slowed_down = true;
    }
    if (the_event.key.code == sf::Keyboard::Space) {
        lightcycle.turn_OnOff_light_trace();
    }
}

/* Handles the lightcycle keyboard key-release controlls.
*/
void handle_lightcycle_key_release(const sf::Event& the_event, Lightcycle& lightcycle) {
    if ((the_event.key.code == sf::Keyboard::Right || the_event.key.code == sf::Keyboard::D) &&
        lightcycle.turned_right && !lightcycle.turned_left) {
        lightcycle.turned_right = false;
    }
    if ((the_event.key.code == sf::Keyboard::Left || the_event.key.code == sf::Keyboard::A) &&
        lightcycle.turned_left && !lightcycle.turned_right) {
        lightcycle.turned_left = false;
    }
    if ((the_event.key.code == sf::Keyboard::Up || the_event.key.code == sf::Keyboard::W) && lightcycle.accelerated) {
        lightcycle.accelerated = false;
    }
    if ((the_event.key.code == sf::Keyboard::Down || the_event.key.code == sf::Keyboard::S) && lightcycle.slowed_down) {
        lightcycle.slowed_down = false;
    }
}

/* Updates the timer which it gets as an argument.
*/
void update_timer(float& timer, float delta_time) {
    timer += delta_time;
    if (timer + 100 > FLT_MAX) {
        timer = 1000;
    }
}

/* Sets the starting countdown Texts. Loads the selected font from a file.
*/
void set_countdown(sf::Text& text, sf::Font& font, std::string font_file) {
    if (!font.loadFromFile(font_file)) {
        std::cout << "Font NOT loaded! Trying one more time." << std::endl;
        if (!font.loadFromFile(font_file)) {
            std::cout << "Font NOT loaded for the second time! Aborting action." << std::endl;
        }
    }
    text.setCharacterSize(400);
    text.setFillColor(sf::Color(216, 218, 231));
    text.setFont(font);
}

/* Shows the contdown according to the value of the timer.
*/
void show_countdown(float timer, float offset_from_0, const sf::View& view, sf::RenderWindow& window, sf::Text& text) {
    if (timer < 3 + offset_from_0) {
        if (timer >= 2 + offset_from_0) {
            text.setString("GO!");
        }
        else if (timer >= 1 + offset_from_0) {
            text.setString("STEADY");
        }
        else if (timer >= 0 + offset_from_0) {
            text.setString("READY");
        }
        text.setPosition(sf::Vector2f(view.getCenter().x - text.getGlobalBounds().width / 2, view.getCenter().y - 50));
        window.draw(text);
    }
}

/* Shows the number of players alive.
*/
void show_alive_counter(sf::RenderWindow& window, sf::Text& text, const sf::Font& font, const std::string& to_be_shown) {
    text.setCharacterSize(180);
    text.setFillColor(sf::Color(216, 218, 231));
    text.setFont(font);
    text.setString(to_be_shown);
    text.setPosition(sf::Vector2f(window.getView().getCenter().x - window.getView().getSize().x / 2 + 10,
                                  window.getView().getCenter().y - window.getView().getSize().y / 2));
    window.draw(text);
}

void show_velocity(sf::RenderWindow& window, sf::Text& text, const sf::Font& font, const std::string& to_be_shown) {
    text.setCharacterSize(160);
    text.setFillColor(sf::Color(216, 218, 231));
    text.setFont(font);
    text.setString(to_be_shown);
    text.setPosition(sf::Vector2f(window.getView().getCenter().x + window.getView().getSize().x / 2 - text.getGlobalBounds().width - 60,
                     window.getView().getCenter().y + window.getView().getSize().y / 2 - text.getGlobalBounds().height * 2));
    window.draw(text);
}

/* Adds a prototype lightcycle to each of the ais.
* "player_inx" is the index of the player's lightcycle which doesn't need any ai.
*/
void init_ai(std::vector<AI>& all_ai, std::vector<Lightcycle> prototypes, std::shared_ptr<Lightcycle> target, size_t player_inx) {
    for (size_t i = 0; i < prototypes.size(); ++i) {
        if (i != player_inx) {
            all_ai.push_back(AI(prototypes[i], target));
        }
    }
}

/* The final "game over" text initialization.
*/
void show_ending_text(sf::RenderWindow& window, sf::Text& text, const sf::Font& font, const std::string& to_be_shown, bool won) {
    text.setCharacterSize(300);
    text.setFillColor(sf::Color(5, 13, 16));
    if (!won)
        text.setOutlineColor(sf::Color(255, 170, 42));
    else
        text.setOutlineColor(sf::Color(10, 201, 211));
    text.setOutlineThickness(5);
    text.setFont(font);
    text.setString(to_be_shown);
    text.setPosition(sf::Vector2f(window.getView().getCenter().x - text.getGlobalBounds().width / 2, window.getView().getCenter().y + 150));
    window.draw(text);
}


int main() {
    // ****************************************** INITIALIZATIONS ****************************************** //

    State state = State::InTheMainMenu;

    sf::Text countdown_txt, lightcycle_count_txt, ending_text;
    sf::Font font;
    set_countdown(countdown_txt, font, "arial.ttf");
    size_t functional_lightcycles;

    TheGrid grid(32000.f, 32000.f, 800.f);
    Collidable collidables(grid.bounds);


    // Set window
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode(); // I can change different window styles
    float desktop_ratio = (float)desktopMode.height / (float)desktopMode.width;
    sf::RenderWindow window(desktopMode, "Tron: Lightcycles", sf::Style::Fullscreen, settings);
    window.setVerticalSyncEnabled(true);
    
    MainMenu main_menu((float)desktopMode.width, (float)desktopMode.height);


    // Set lightcycles
    sf::Texture texture;
    if (!texture.loadFromFile("LightcycleV5.png")) {
        std::cout << "The Texture can not be loaded." << std::endl;
    }

    // the following numbers position the lightcycles at different places every time the game starts
    // they just change the X coordinate of the lightcycles
    int rand_num = rand() % 4 + 2; // random number from 2 to 5
    int rand_num_sum = rand() % 801 - 400; // random number from -400 to 400

    size_t cheating_level = 1;
    vector<Lightcycle> prototypes;
    for (int i = 0; i <= 7; ++i) {
        sf::Color color;
        if (i == 3) cheating_level = 0;
        else 	{
            cheating_level = 1;
        }
        if (i <= 3) {
            color.r = 255 - i * 32;
            color.g = 255 - i * 10;
            color.b = 255;
        }
        else {
            color.r = 255;
            color.g = 255 - i * 10;
            color.b = 255 - i * 32;
        }
        Lightcycle new_lightcycle(sf::Vector2f(42, 120), // size
                              sf::Vector2f(2000.f + i * 800.f * rand_num + rand_num_sum, 33500.f - 120 * i), // position
                              color, //color
                              &texture, i, cheating_level); // texture and rotation
        prototypes.push_back(new_lightcycle);
        rand_num_sum = rand() % 801 - 400; // generate a new random number for each lightcycle
    }
    size_t player_inx = 3;
    auto lightcycle = std::make_shared<Lightcycle>(prototypes[player_inx]);

    std::vector<AI> all_ai;
    init_ai(all_ai, prototypes,lightcycle, player_inx);

    functional_lightcycles = prototypes.size();


    // Set the main clock and the main timer
    sf::Clock clock;
    float timer = 0;


    // Set main camera view
    float viewWidth = (float)desktopMode.width + 8000; //4200;
    float viewHeight = viewWidth * desktop_ratio;
    sf::View view(sf::FloatRect(0.f, 0.f, viewWidth, viewHeight));

    // Win/Lose texts
    std::string final_words_win = "CONGRATULATIONS USER!\nYOU ARE THE WINNER!\n\nHit ENTER to proceed...";
    std::string final_words_lose = "DEREZZED!\n\nHit ENTER to proceed...";

    // ************************************************* MAIN WHILE LOOP ************************************************* //

    // The main while loop that keeps the game up to date and draws everything on the window
    while (window.isOpen()) {
        functional_lightcycles = 0;
        // main time settings
        sf::Time elapsed = clock.restart(); // gives me the ability to update the game, needed to move the lightcycles
        float delta_time = elapsed.asSeconds();

        if (state == State::Playing) {
            update_timer(timer, delta_time);
        }

        // event handling
        sf::Event the_event;
        while (window.pollEvent(the_event)) {
            if (the_event.type == sf::Event::Closed)
                window.close();

            // handle key-press
            if (the_event.type == sf::Event::KeyPressed) {
                // this event stores all the pressed keys at the same time
                if (the_event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                // if the current state is main menu
                if (state == State::InTheMainMenu) {
                    if (the_event.key.code == sf::Keyboard::Up || the_event.key.code == sf::Keyboard::W) {
                        main_menu.move_up();
                    }
                    if (the_event.key.code == sf::Keyboard::Down || the_event.key.code == sf::Keyboard::S) {
                        main_menu.move_down();
                    }
                    if (the_event.key.code == sf::Keyboard::Left || the_event.key.code == sf::Keyboard::A) {
                        main_menu.move_left();
                    }
                    if (the_event.key.code == sf::Keyboard::Right || the_event.key.code == sf::Keyboard::D) {
                        main_menu.move_right();
                    }
                    if (the_event.key.code == sf::Keyboard::Enter) {
                        if (main_menu.play_selected()) {
                            state = State::Playing;
                            // set difficulty
                            for (size_t i = 0; i < all_ai.size(); ++i) {
                                all_ai[i].set_cheating_lvl(main_menu.get_difficulty_level() - 1);
                            }
                            // reset view size
                            viewWidth = (float)desktopMode.width + 8000;
                            viewHeight = viewWidth * desktop_ratio;
                        }
                        else if (main_menu.quit_selected()) {
                            window.close();
                        }
                    }
                }
                // else if the game is on (and the countdown has already finished)
                else if (state == State::Playing && timer >= 3) {
                    handle_lightcycle_key_press(the_event, *lightcycle);
                    // player's lightcycle was detroyed (DEREZZED) or he/she has already won -> the game asks to press the Enter button
                    if (the_event.key.code == sf::Keyboard::Enter && (!lightcycle->isAlive || lightcycle->stopped)) {
                        // After Enter was pressed reset the game and return it to the starting status
                        state = State::InTheMainMenu;

                        rand_num = rand() % 4 + 2;
                        for (size_t i = 0; i < prototypes.size(); ++i) {
                            prototypes[i].body.setPosition(sf::Vector2f(2000.f + i * 700.f * rand_num + rand_num_sum, 33500.f - 120 * i));
                            rand_num_sum = rand() % 801 - 400;
                        }
                        lightcycle = std::make_shared<Lightcycle>(prototypes[3]);
                        all_ai.clear(); // clear the ai vector
                        init_ai(all_ai, prototypes, lightcycle, player_inx); // initialize the new ai lightcycles
                        view.reset(sf::FloatRect(0.f, 0.f, (float)desktopMode.width, (float)desktopMode.height));
                        timer = 0; 
                        window.setView(view);
                        collidables.reset();
                    }

                    // zoom out
                    if (the_event.key.code == sf::Keyboard::Z) {
                        viewWidth += 200;
                        if (viewWidth > 11000) {
                            viewWidth -= 200;
                            continue;
                        }
                        viewHeight = viewWidth * desktop_ratio;
                    }
                    // zoom in
                    if (the_event.key.code == sf::Keyboard::X) {
                        viewWidth -= 200;
                        if (viewWidth < 1700) {
                            viewWidth += 200;
                            continue;
                        }
                        viewHeight = viewWidth * desktop_ratio;
                    }
                }
            }
            // handle key-release
            if (the_event.type == sf::Event::KeyReleased) {
                // lightcycle controls
                if (state == State::Playing) {
                    handle_lightcycle_key_release(the_event, *lightcycle);
                }
            }
        }
        window.clear(sf::Color(5, 13, 16)); // the original Tron background color
        grid.draw_the_grid(window);

        if (state == State::InTheMainMenu) {
            main_menu.draw(window);
        }
        else if (state == State::Playing) {
            // update player's lightcycle
            if (lightcycle->accelerated) {
                lightcycle->accelerate(delta_time);
            }
            if (timer >= 3) {
                // accelerate until the minimal speed is acheaved
                if (lightcycle->get_velocity() < 4200 && !lightcycle->accelerated) {
                    lightcycle->slowed_down = false;
                    lightcycle->accelerate(delta_time);
                }
            }
            if (lightcycle->slowed_down) {
                lightcycle->slow_down(delta_time);
            }
            if (lightcycle->isAlive) {
                lightcycle->update_position(delta_time);
            }

            // update view
            view.setSize(viewWidth, viewHeight);
            view.setCenter(lightcycle->body.getPosition());
            window.setView(view);

            collidables.add_collidable(grid.bounds);
            collidables.add_collidable(*lightcycle);

            // the game is on => update ai's position and make its move
            if (timer >= 3) {
                for (size_t i = 0; i < all_ai.size(); ++i) {
                    all_ai[i].update_ai(collidables, delta_time);
                }
                for (size_t i = 0; i < all_ai.size(); ++i) {
                    all_ai[i].make_turn(window, collidables);
                    if (all_ai[i].is_alive()) {
                        functional_lightcycles++;
                    }
                }
            }

            // check if the player's lightcycle didn't crash
            collidables.check_for_collision(*lightcycle);

            // If collided, destroy the light trace. Warning!
            // The lighttrace is not destroyed immediately but during multiple iterations of the main while cycle..
            if (!lightcycle->isAlive) {
                lightcycle->destroy_light_trace();
                show_ending_text(window, ending_text, font, final_words_lose, false);
            }
            else {
                functional_lightcycles++;
            }

            window.draw(lightcycle->body);
            lightcycle->draw_trace(window);
            if (functional_lightcycles == 1 && lightcycle->isAlive && timer >= 3) {
                lightcycle->stop();
                show_ending_text(window, ending_text, font, final_words_win, true);
            }
            show_countdown(timer, 0, view, window, countdown_txt);
            std::string to_be_shown = "Alive: " + std::to_string(functional_lightcycles);
            show_alive_counter(window, lightcycle_count_txt, font, to_be_shown);
            show_velocity(window, lightcycle_count_txt, font, std::to_string((size_t)(lightcycle->get_velocity())));
        }
        window.display();
        collidables.reset();
    }

    return 0;
}