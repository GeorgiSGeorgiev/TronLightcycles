#ifndef COLLIDABLE_H
#define COLLIDABLE_H

#include <SFML/Graphics.hpp>
#include "Lightcycle.h"
#include <vector>
#include <memory>

/**
* A class that contains all of the collidable objects in the game.
* Creating different collidable classes allows the programmer to create different collidable levels.
*/
class Collidable {
public:
	/**
	* Creates a new set of collidables. Needs to get the bound location for improved calculations.
	*/
	Collidable(std::vector<sf::RectangleShape>& bounds);
	bool add_collidable(const sf::FloatRect& collidable);
	bool add_collidable(const Lightcycle& lightcycle);
	bool add_collidable(const std::vector<sf::RectangleShape>& vector);
	void reset();
	size_t get_count();
	/* Checks if there are any collidable objects inside this instance of the collidable class.
	*/
	bool is_available();
	/**
	* Calculates the closest obstacle in all needed directions.
	* Directly updates the Lightcycle argument. Returns true if succeeds.
	* If there are no collidables then there is no update and the function directly returns false.
	*/
	bool calc_closest_collidable_dist(Lightcycle& lightcycle);
	/**
	* Checks if the lightcycle from the function argument collides with any of the collidables added to this class.
	*/
	void check_for_collision(Lightcycle& lightcycle);
private:
	std::vector<float> bound_location;
	// The collidables lifespan is longer than the lifespan of the Collidable class. It is part of the core game logic.
	std::vector<std::unique_ptr<sf::FloatRect>> all_collidables;

	float closest_dist_0 = 0;
	float closest_dist_90 = 0;
	float closest_dist_180 = 0;
	float closest_dist_270 = 0;
};

#endif