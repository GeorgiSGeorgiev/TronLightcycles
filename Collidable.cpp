#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Lightcycle.h"
#include "Collidable.h"
#include <iostream>

Collidable::Collidable(std::vector<sf::RectangleShape>& bounds) {
	float tmp_bound_location;
	for (size_t i = 0; i < bounds.size(); i++) {
		if (i % 2 == 0) { //-> upper / lower
			tmp_bound_location = bounds[i].getPosition().y;
		}
		else {
			tmp_bound_location = bounds[i].getPosition().x;
		}
		this->bound_location.push_back(tmp_bound_location);
	}
}

bool Collidable::add_collidable(const sf::FloatRect& collidable) {
	if (this->all_collidables.size() < this->all_collidables.max_size()) {
		this->all_collidables.push_back(std::make_unique<sf::FloatRect>(collidable));
		return true;
	}
	return false;
}

bool Collidable::add_collidable(const Lightcycle& lightcycle) {
	for (size_t i = 0; i < lightcycle.light_trace.size(); ++i) {
		if (this->all_collidables.size() < this->all_collidables.max_size()) {
			this->all_collidables.push_back(std::make_unique<sf::FloatRect>(lightcycle.light_trace[i].getGlobalBounds()));
		}
	}
	if (this->all_collidables.size() < this->all_collidables.max_size()) {
		return true;
	}
	return false;
}

bool Collidable::add_collidable(const std::vector<sf::RectangleShape>& vector) {
	for (size_t i = 0; i < vector.size(); ++i) {
		if (this->all_collidables.size() < this->all_collidables.max_size()) {
			this->all_collidables.push_back(std::make_unique<sf::FloatRect>(vector[i].getGlobalBounds()));
		}
	}
	if (this->all_collidables.size() < this->all_collidables.max_size()) {
		return true;
	}
	return false;
}

/**
* Calculates the closest obstacle in all needed directions.
* Directly updates the Lightcycle argument. Returns true if succeeds.
* If there are no collidables then there is no update and the function directly returns false.
*/
bool Collidable::calc_closest_collidable_dist(Lightcycle& cycle) {
	if (!this->is_available()) {
		return false;
	}
	// set precisely the x a y position of the Lightcycle
	float pos_x = cycle.get_position().x;
	if (cycle.get_rotation() == 90) {
		pos_x += cycle.body.getSize().y; // body height
		//std::cout << cycle.body.getSize().y << std::endl;
	}
	if (cycle.get_rotation() == 270) {
		pos_x -= cycle.body.getSize().y;
		//std::cout << cycle.body.getSize().y << std::endl;
	}
	float pos_y = cycle.get_position().y;
	if (cycle.get_rotation() == 0) {
		pos_y -= cycle.body.getSize().y;
		//std::cout << cycle.body.getSize().y << std::endl;
	}
	if (cycle.get_rotation() == 180) {
		pos_y += cycle.body.getSize().y;
		//std::cout << cycle.body.getSize().y << std::endl;
	}

	float new_dist;
	float lowest_x, biggest_x, lowest_y, biggest_y;

	// calculate maximal possible distances
	closest_dist_0 = pos_y - this->bound_location[0];
	closest_dist_90 = this->bound_location[1] - pos_x;
	closest_dist_180 = this->bound_location[2] - pos_y;
	closest_dist_270 = pos_x - this->bound_location[3];
	
	// iterate through all collidables and try to find closer collidables to the given lightcycle
	for (size_t j = 0; j < this->all_collidables.size(); ++j) {
		if (cycle.get_collision_box() == *(this->all_collidables[j])) {
			continue;
		}
		lowest_x = all_collidables[j]->left - 110;
		biggest_x = all_collidables[j]->left + all_collidables[j]->width + 110;
		lowest_y = all_collidables[j]->top - 110;
		biggest_y = all_collidables[j]->top + all_collidables[j]->height + 110;

		// try all directions and update only the relevant ones
		for (size_t i = 0; i < cycle.possible_directions.size(); ++i) {
			if (cycle.possible_directions[i] == 0 && lowest_x <= pos_x && pos_x <= biggest_x) {
				new_dist = pos_y - all_collidables[j]->top;
				if (new_dist < closest_dist_0 && new_dist > 0) {
					closest_dist_0 = new_dist;
				}
				continue;
			}
			if (cycle.possible_directions[i] == 90 && lowest_y <= pos_y && pos_y <= biggest_y) {
				new_dist = all_collidables[j]->left - pos_x;

				if (new_dist < closest_dist_90 && new_dist > 0) {
					closest_dist_90 = new_dist;
				}
				continue;
			}
			if (cycle.possible_directions[i] == 180 && lowest_x <= pos_x && pos_x <= biggest_x) {
				new_dist = all_collidables[j]->top - pos_y;
				if (new_dist < closest_dist_180 && new_dist > 0) {
					closest_dist_180 = new_dist;
				}
				continue;
			}
			if (cycle.possible_directions[i] == 270 && lowest_y <= pos_y && pos_y <= biggest_y) {
				new_dist = pos_x - all_collidables[j]->left;
				if (new_dist < closest_dist_270 && new_dist > 0) {
					closest_dist_270 = new_dist;
				}
				continue;
			}
		}
	}
	cycle.closest_collidables[0] = closest_dist_0;
	cycle.closest_collidables[90] = closest_dist_90;
	cycle.closest_collidables[180] = closest_dist_180;
	cycle.closest_collidables[270] = closest_dist_270;
	return true;
}

void Collidable::check_for_collision(Lightcycle& lightcycle) {
	for (size_t i = 0; i < this->all_collidables.size(); ++i) {
		lightcycle.check_for_crash(*all_collidables[i]);
		if (!lightcycle.isAlive) return;
	}
}

void Collidable::reset() {
	all_collidables.clear();
}

size_t Collidable::get_count() {
	return all_collidables.size();
}

bool Collidable::is_available() {
	return all_collidables.size() != 0;
}