#pragma warning(disable:26812) // third party enum warning

#include "TheGrid.h"
#include <vector>
#include <SFML/Graphics.hpp>

/* Represents the borders of the playing field. Draws the "floor" of the field.
*/
TheGrid::TheGrid(float width, float height, float freq) {
    this->width = width;
    this->height = height;
    this->freq = freq;
    this->bounds_size_bias = 3100;
    this->line_wid = 100;
    float position_bias = (bounds_size_bias - line_wid) / 2;

    // initialization of the different bounds
    sf::RectangleShape horUpperBound(sf::Vector2f(this->width + bounds_size_bias, line_wid));
    horUpperBound.setFillColor(sf::Color(100, 35, 185));
    horUpperBound.setPosition(-position_bias, -position_bias);
    bounds.push_back(horUpperBound);

    sf::RectangleShape vertRightBound(sf::Vector2f(line_wid, this->height + bounds_size_bias));
    vertRightBound.setFillColor(sf::Color(100, 35, 185));
    vertRightBound.setPosition(this->width + position_bias, -position_bias);
    bounds.push_back(vertRightBound);

    sf::RectangleShape horLowerBound(sf::Vector2f(this->width + bounds_size_bias, line_wid));
    horLowerBound.setFillColor(sf::Color(100, 35, 185));
    horLowerBound.setPosition(-position_bias, this->height + position_bias);
    bounds.push_back(horLowerBound);

    sf::RectangleShape vertLeftBound(sf::Vector2f(line_wid, this->height + bounds_size_bias));
    vertLeftBound.setFillColor(sf::Color(100, 35, 185));
    vertLeftBound.setPosition(-position_bias, -position_bias);
    bounds.push_back(vertLeftBound);
}

TheGrid::~TheGrid() {}

void TheGrid::draw_the_grid(sf::RenderWindow& window) {
    float horNum = this->height / freq;
    float vertNum = this->width / freq;

    sf::RectangleShape horLine(sf::Vector2f(this->width, 18));
    horLine.setFillColor(sf::Color(10, 25, 35));

    sf::RectangleShape vertLine(sf::Vector2f(18, this->height));
    vertLine.setFillColor(sf::Color(10, 25, 35));

    for (size_t i = 0; i <= horNum; i++) {
        horLine.setPosition(0, 1 + i * this->freq);
        window.draw(horLine);
    }
    for (size_t j = 0; j <= vertNum; j++) {
        vertLine.setPosition(1 + j * this->freq, 0);
        window.draw(vertLine);
    }

    window.draw(bounds[0]);
    window.draw(bounds[1]);
    window.draw(bounds[2]);
    window.draw(bounds[3]);
}