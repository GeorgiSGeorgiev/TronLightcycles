#ifndef AI_H
#define AI_H

#include "Lightcycle.h"
#include "Collidable.h"
#include "TheGrid.h"
#include <array>

class AI {
public:
	/* Initialize the ai class and add its main target.
	*/
	AI(const Lightcycle& lightcycle, std::shared_ptr<Lightcycle> main_target);

	/* Update the ai's timer, location and light trace. Adds the ai to the collidables.
	*/
	void update_ai(Collidable& collidables, float delta_time);

	/*
	* AI's turn. The AI gets all important information from the main Collodable program instance and decides which direction to choose.
	* The method is very complicated mainly because it contains many conditions and some of them overlap.
	* If you want to improve the AI, proceed with great caution because this function may stop working the way it is supposed to be.
	* I do not recomend deleating any constraints (the timer is very important too).
	*/
	void make_turn(sf::RenderWindow& window, Collidable& collidables);
	void add_new_target(std::shared_ptr<Lightcycle> new_one);

	bool is_alive();

	void set_cheating_lvl(size_t cheating_level);
private:
	bool timer_started = false;
	float timer = 0; // AI's main timer
	const float chase_const = 800; // Determines how much the targets will be chased by the ai. If too high, may alter important decisions and cause unwanted behaviour. Normal values: (700-850)
	size_t turns_in_a_row = 0; // Turn counter
	const size_t max_turns_in_a_row = 2; // Determines how many turns in a row may be made (2 is fine).
	std::unique_ptr<Lightcycle> cycle; // The lightcycle of the ai.
	std::vector<std::shared_ptr<Lightcycle>> targets; // Vector of all of the ai's targets, may have more targets but then a new implementation of the make turn function have to be added.
	std::array<float, 3> future_values; // values: turn left, go straight, turn right

	bool turned_right = false;
	bool turned_left = false;

	/**
	* Gets the best value of the future_values array and chooses the direction with the highest value.
	*/
	void choose_direction();
};

#endif