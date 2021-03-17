#include "MainMenu.h"

MainMenu::MainMenu(float width, float height) {
    if (!font.loadFromFile("arial.ttf")) {
        std::cout << "Font NOT loaded! Trying one more time." << std::endl;
        if (!font.loadFromFile("arial.ttf")) {
            std::cout << "Font NOT loaded for the second time! Aborting action." << std::endl;
        }
    }
    title.setFont(font);
    title.setFillColor(sf::Color(5, 13, 16));
    title.setOutlineColor(sf::Color(10, 201, 211));
    title.setOutlineThickness(5);
    title.setString("TRON: LIGHTCYCLES");
    title.setCharacterSize(110);
    title.setPosition(sf::Vector2f(width / 2 - title.getGlobalBounds().width / 2,
                      height / 11 * 1.f - title.getGlobalBounds().height / 2));

    items[0].setFont(font);
    items[0].setFillColor(sf::Color(102, 251, 251));
    items[0].setString("Start");
    items[0].setCharacterSize(110);
    items[0].setPosition(sf::Vector2f(width / 2 - items[0].getGlobalBounds().width / 2,
                                      height / 11 * 5 - items[0].getGlobalBounds().height / 2));

    items[1].setFont(font);
    items[1].setFillColor(sf::Color(216, 218, 231));
    items[1].setString("Easy");
    items[1].setCharacterSize(80);
    items[1].setPosition(sf::Vector2f((width / 2 - items[1].getGlobalBounds().width / 2) / 2,
                                      height / 11 * 7 - items[1].getGlobalBounds().height / 2));

    items[2].setFont(font);
    items[2].setFillColor(sf::Color(255, 170, 42));
    items[2].setString("Normal");
    items[2].setCharacterSize(70);
    items[2].setPosition(sf::Vector2f((width / 2 - items[2].getGlobalBounds().width / 2),
                                      height / 11 * 7 - items[2].getGlobalBounds().height / 2));

    items[3].setFont(font);
    items[3].setFillColor(sf::Color(216, 218, 231));
    items[3].setString("Hard");
    items[3].setCharacterSize(70);
    items[3].setPosition(sf::Vector2f((width / 2 - items[3].getGlobalBounds().width / 2) / 2 * 3,
                                      height / 11 * 7 - items[3].getGlobalBounds().height / 2));

    items[4].setFont(font);
    items[4].setFillColor(sf::Color(216, 218, 231));
    items[4].setString("Quit");
    items[4].setCharacterSize(100);
    items[4].setPosition(sf::Vector2f((width / 2 - items[4].getGlobalBounds().width / 2),
                         height / 11 * 9 - items[4].getGlobalBounds().height / 2));

    this->selected_item_inx = 0;
    this->selected_mode_inx = 2;
}

void MainMenu::draw(sf::RenderWindow& window) {
    window.draw(title);
    for (size_t i = 0; i < MainMenu::MAX_NUM_OF_ITEMS; ++i) {
        window.draw(items[i]);
    }
}

void MainMenu::move_up() {
    // selectables: 0, {1,2,3}, 4 -> { Start, Modes, Quit }
    // we don't want to move below the button number limit
    if (selected_item_inx - 1 >= 0) {
        if (selected_item_inx == 4) {
            items[4].setFillColor(sf::Color(216, 218, 231));
            items[4].setCharacterSize(100);
            selected_item_inx = selected_mode_inx;
            items[selected_mode_inx].setCharacterSize(80);
            return;
        }
        if (0 < selected_item_inx && selected_item_inx < 4) {
            items[selected_mode_inx].setCharacterSize(70);
            selected_item_inx = 0;
            items[0].setCharacterSize(110);
            items[0].setFillColor(sf::Color(102, 251, 251));
            return;
        }
    }
}

void MainMenu::move_down() {
    // we don't want to move above the button number limit
    if (selected_item_inx + 1 <= MainMenu::MAX_NUM_OF_ITEMS) {
        if (selected_item_inx == 0) {
            items[0].setFillColor(sf::Color(216, 218, 231));
            items[0].setCharacterSize(100);
            selected_item_inx = selected_mode_inx;
            items[selected_mode_inx].setCharacterSize(80);
            return;
        }
        if (0 < selected_item_inx && selected_item_inx < 4) {
            items[selected_mode_inx].setCharacterSize(70);
            selected_item_inx = 4;
            items[4].setCharacterSize(110);
            items[4].setFillColor(sf::Color(102, 251, 251));
            return;
        }
    }
}

void MainMenu::move_left() {
    if (selected_item_inx == 0 || selected_item_inx == 4) return;
    if (selected_mode_inx - 1 >= 1) {
        items[selected_mode_inx].setFillColor(sf::Color(216, 218, 231));
        items[selected_mode_inx].setCharacterSize(70);
        selected_mode_inx--;
        selected_item_inx = selected_mode_inx;
        items[selected_mode_inx].setCharacterSize(80);
        items[selected_mode_inx].setFillColor(sf::Color(255, 170, 42));
    }
}

void MainMenu::move_right() {
    if (selected_mode_inx == 0 || selected_mode_inx == 4) return;
    if (selected_item_inx + 1 <= 3) {
        items[selected_mode_inx].setFillColor(sf::Color(216, 218, 231));
        items[selected_mode_inx].setCharacterSize(70);
        selected_mode_inx++;
        selected_item_inx = selected_mode_inx;
        items[selected_mode_inx].setCharacterSize(80);
        items[selected_mode_inx].setFillColor(sf::Color(255, 170, 42));
    }
}

bool MainMenu::play_selected() {
    return selected_item_inx == 0;
}

bool MainMenu::quit_selected() {
    return selected_item_inx == 4;
}

size_t MainMenu::get_difficulty_level() {
    return this->selected_mode_inx;
}