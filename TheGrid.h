#ifndef THEGRID_H
#define THEGRID_H

#include <SFML/Graphics.hpp>

class TheGrid {
public:
    std::vector<sf::RectangleShape> bounds;
    TheGrid(float width, float height, float freq);
    ~TheGrid();
    void draw_the_grid(sf::RenderWindow& window);
private:
    float width;
    float height;
    float freq;
    float bounds_size_bias;
    float line_wid;
};

#endif