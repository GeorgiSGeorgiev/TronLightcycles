#ifndef MAINMENU_H
#define MAINMENU_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <array>

class MainMenu {
public:
	const static size_t MAX_NUM_OF_ITEMS = 5;
	MainMenu(float width, float height);

	void draw(sf::RenderWindow& window);
	void move_up();
	void move_down();
	void move_right();
	void move_left();

	bool play_selected();
	bool quit_selected();
	size_t get_difficulty_level();
private:
	size_t selected_item_inx;
	size_t selected_mode_inx;
	sf::Font font;
	sf::Text title;
	std::array<sf::Text, MAX_NUM_OF_ITEMS> items;
};

#endif