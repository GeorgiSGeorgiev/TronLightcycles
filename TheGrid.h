#ifndef THEGRID_H
#define THEGRID_H

#include <SFML/Graphics.hpp>
/* Represents the borders of the playing field. Draws the "floor" of the field.
*/
class TheGrid {
public:
    std::vector<sf::RectangleShape> bounds;
    /* Constructs the grid. "freq" defines the space between two grid lines.
    */
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