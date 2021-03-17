#include "AI.h"
#include <SFML/Graphics.hpp>
#include <iostream>

AI::AI(const Lightcycle& lightcycle, std::shared_ptr<Lightcycle> main_target) {
	this->cycle = std::make_unique<Lightcycle>(lightcycle);
	this->cycle->turn_OnOff_light_trace();
	// the new_one is not present in the targets vector so add it
	if (*this->cycle == *main_target) {
		return;
	}
	this->targets.push_back(main_target);
}

void AI::update_ai(Collidable& collidables, float delta_time) {
	// update the ai's timer
	if (timer_started) {
		timer += delta_time;
	}
	cycle->accelerate(delta_time);
	cycle->update_position(delta_time);
	collidables.add_collidable(*cycle);
	if (!cycle->isAlive) {
		cycle->destroy_light_trace();
	}
}

void AI::set_cheating_lvl(size_t cheating_level) {
	this->cycle->set_cheating_lvl(cheating_level);
}

void AI::make_turn(sf::RenderWindow& window, Collidable& collidables) {
	// If you want to improve the AI, the best way doing so is to increase the "sight" of the AI.
	// Try increasing some of  the if statement constants below.
	int rand_num = rand() % 100; // used to add some randomness to the decisions made by the AI. Determines only some close cases.

	// get the distances to the closest objects in all of the relevant directions (left, straight, right)
	collidables.calc_closest_collidable_dist(*cycle);

	// get the needed values and add some randomness to them
	for (size_t i = 0; i < future_values.size(); ++i) {
		future_values[i] = cycle->closest_collidables[cycle->possible_directions[i]] + rand_num * 3;
		rand_num = rand() % 100;
	}

	// emergency plan, denies the rules and tries the best to survive
	if (future_values[1] < 400) {
		future_values[1] -= 20000; // if close obstacle in the front detected then make the priority of going forward very small
		if (future_values[0] > future_values[2]) { // choose turning direction according to its priority
			if (!timer_started)
				cycle->turn_right();
			else if (timer_started && timer > 0.3f) { // don't make 2 reactions too fast, without this the AI may do many pointless turns
				cycle->turn_left();
			}
			turned_left = true;
			this->turns_in_a_row++;
			timer_started = true;
		}
		else { // same but in the opposite direction
			if (!timer_started)
			cycle->turn_right();
			else if (timer_started && timer > 0.3f){
				cycle->turn_right();
			}
			turned_right = true;
			this->turns_in_a_row++;
			timer_started = true;
		}
	}

	// In close situations prioritize the turning direction which is less dangerous
	if (future_values[2] < 450 && future_values[0] > 700) {
		future_values[0] += 2000;
	}
	if (future_values[0] < 450 && future_values[2] > 700) {
		future_values[2] += 2000;
	}

	// don't choose the same direction two times in a row
	if (turned_left) {
		// if turned to the left last turn then prioritize right turn this time
		if (future_values[2] > 450) {
			future_values[2] += 1000 * (turns_in_a_row + 1);
		}
		if (future_values[0] < 350) { // if left turn is very dangerous and turned to the left last time then prioritize turning right even more
			future_values[2] += 6500 * (turns_in_a_row + 1);
		}
		turned_left = false;
	}
	if (turned_right) { // same but the opposite direction
		// if turned to the right last turn then prioritise left turn this time
		if (future_values[0] > 450) {
			future_values[0] += 1000 * (turns_in_a_row + 1);
		}
		if (future_values[2] < 350) {
			future_values[0] += 6500 * (turns_in_a_row + 1);
		}
		turned_right = false;
	}

	// The constraints below could be different functions but that would make the conditions "black boxes"

	float x = cycle->get_position().x;
	float y = cycle->get_position().y;
	float target_x = targets[0]->get_position().x;
	float target_y = targets[0]->get_position().y;

	// ********************************************** THE MAIN CONSTRAINTS ********************************************** \\

	if (cycle->get_rotation() == 0 && targets[0]->get_rotation() == 0) { // target and the ai heading upwards
		// target below to the right and the right turn is not too dangerous -> cut the target off
		if (x < target_x && target_x < x + 2500 && y + 400 < target_y && target_y < y + 10000 && future_values[2] >= 400) {
			if (future_values[2] >= 600) { // increase the priorities differently according to the danger level of the turn
				future_values[2] += 30000;
			}
			else {
				future_values[2] += 500;
			}
		} // target below to the left and the left turn is not too dangerous -> cut the target off
		else if (x - 2500 < target_x && target_x < x && y + 400 < target_y && target_y < y + 10000 && future_values[0] >= 400) {
			if (future_values[0] >= 600) { // the same logic as above
				future_values[0] += 30000;
			}
			else {
				future_values[0] += 500;
			}
		}
		else { // search for the target in its direction. If the turn may cause a later dissaster, then don't make it 
			if (target_x < x && future_values[0] >= 1000) { // the same logic is used in all of the constraints below
				future_values[0] += 2500;
			}
			if (target_x > x && future_values[2] >= 1000) {
				future_values[2] += 2500;
			}
		}
	}
	else if (cycle->get_rotation() == 90 && targets[0]->get_rotation() == 90) { // ai and target heading to the right
		// target below and behind the ai, the same logic as above
		if (y < target_y && target_y < y + 2500 && x - 10000 < target_x && target_x < x - 400 && future_values[2] >= 400) {
			if (future_values[2] >= 600) {
				future_values[2] += 50000;
			}
			else {
				future_values[2] += 500;
			}
		} // the opposite side
		else if (y - 2500 < target_y && target_y < y && x - 10000 < target_x && target_x < x - 400 && future_values[0] >= 400) {
			if (future_values[0] >= 600) {
				future_values[0] += 50000;
			}
			else {
				future_values[0] += 500;
			}
		}
		else {
			if (target_y < y && future_values[0] >= 1000) {
				future_values[0] += 2000;
			}
			if (target_y > y && future_values[2] >= 1000) {
				future_values[2] += 2000;
			}
		}
	}
	else if (cycle->get_rotation() == 180 && targets[0]->get_rotation() == 180) { // ai and the target heading to the bottom
		// the target is above (behind the ai) and to the right of the ai
		if (x < target_x && target_x < x + 2500 && y - 10000 < target_y && target_y < y - 400 && future_values[0] >= 400) {
			if (future_values[0] >= 600) {
				future_values[0] += 50000;
			}
			else {
				future_values[0] += 500;
			}
		} // the opposite side
		else if (x - 2500 < target_x && target_x < x && y - 10000 < target_y && target_y < y - 400 && future_values[2] >= 400) {
			if (future_values[2] >= 600) {
				future_values[2] += 50000;
			}
			else {
				future_values[2] += 500;
			}
		}
		else {
			if (target_x < x && future_values[2] >= 1000) {
				future_values[2] += 2000;
			}
			if (target_x > x && future_values[0] >= 1000) {
				future_values[0] += 2000;
			}
		}
	}
	else if (cycle->get_rotation() == 270 && targets[0]->get_rotation() == 270) { // ai and the target heading to the left
		// the target is to the right (behind the ai) and below the ai
		if (y < target_y && target_y < y + 2500 && x + 400 < target_x && target_x < x + 10000 && future_values[0] >= 400) {
			if (future_values[0] >= 600) {
				future_values[0] += 50000;
			}
			else {
				future_values[0] += 500;
			}
		} // behind, above the ai
		else if (y - 2500 < target_y && target_y < y && x + 400 < target_x && target_x < x + 10000 && future_values[2] >= 400) {
			if (future_values[2] >= 600) {
				future_values[2] += 50000;
			}
			else {
				future_values[2] += 500;
			}
		}
		else {
			if (target_y < y && future_values[2] >= 1000) {
				future_values[2] += 2000;
			}
			if (target_y > y && future_values[0] >= 1000) {
				future_values[0] += 2000;
			}
		}
	}

	// more constraints below: different cut-offs

	if (cycle->get_rotation() == 0 && targets[0]->get_rotation() == 90) { // ai heading up, target comming from the left side
		// the ai is a little bit in front of the target
		if (y - 300 < target_y && target_y < y + 2000 && x - 5000 < target_x && target_x < x && future_values[1] >= 400) {
			// when the target is just a little bit behind the ai, then the ai must keep heading forward otherwise it will create an escape route for the target
			if (future_values[1] >= 600) { 
				future_values[1] += 50000;
			}
			else {
				future_values[1] += 500;
			}
		} // the target is far behind the ai but close enough to ai's trace-> cut it off by turning to the direction in which the target is located
		else if (target_y > y + 2000 && x - 5000 < target_x && target_x < x && future_values[0] >= 400) {
			if (future_values[0] >= 600) {
				future_values[0] += 50000;
			}
			else {
				future_values[0] += 500;
			}
		}
		else { // otherwise same as above: target too far away -> search for the target
			if (target_y < y && future_values[0] >= 1000) {
				future_values[0] += chase_const;
			}
			if (target_y > y && future_values[2] >= 1000) {
				future_values[2] += chase_const;
			}
		}

	}

	if (cycle->get_rotation() == 0 && targets[0]->get_rotation() == 270) { // ai heading up, target coming from the right
		// the ai is a little bit in front of the target
		if (y - 300 < target_y && target_y < y + 2000 && x < target_x && target_x < x + 5000 && future_values[1] >= 400) {
			if (future_values[1] >= 600) {
				future_values[1] += 50000;
			}
			else {
				future_values[1] += 500;
			}
		} // the target is far behind the ai but close enough to ai's trace-> cut it off by turning to the direction in which the target is located
		else if (target_y > y + 2000 && x < target_x && target_x < x + 5000 && future_values[2] >= 400) {
			if (future_values[2] >= 600) {
				future_values[2] += 50000;
			}
			else {
				future_values[2] += 500;
			}
		}
		else { // otherwise same as above: target too far away -> search for the target
			if (target_y < y && future_values[2] >= 1000) {
				future_values[2] += chase_const;
			}
			if (target_y > y && future_values[0] >= 1000) {
				future_values[0] += chase_const;
			}
		}
	}

	if (cycle->get_rotation() == 180 && targets[0]->get_rotation() == 90) { // ai going down, target coming from the left
		// the ai is a little bit in front of the target
		if (y - 2000 < target_y && target_y < y + 300 && x - 5000 < target_x && target_x < x && future_values[1] >= 400) {
			if (future_values[1] >= 600) {
				future_values[1] += 50000;
			}
			else {
				future_values[1] += 500;
			}
		} // the target is far behind the ai but close enough to ai's trace-> cut it off by turning to the direction in which the target is located
		else if (target_y < y - 2000 && x - 5000 < target_x && target_x < x && future_values[2] >= 400) {
			if (future_values[2] >= 600) {
				future_values[2] += 50000;
			}
			else {
				future_values[2] += 500;
			}
		}
		else { // otherwise same as above: target too far away -> search for the target
			if (target_y < y && future_values[0] >= 1000) {
				future_values[0] += chase_const;
			}
			if (target_y > y && future_values[2] >= 1000) {
				future_values[2] += chase_const;
			}
		}

	}

	if (cycle->get_rotation() == 180 && targets[0]->get_rotation() == 270) { // ai going down, target coming from the right
		if (y - 2000 < target_y && target_y < y + 300 && x < target_x && target_x < x + 5000 && future_values[1] >= 400) {
			if (future_values[1] >= 600) {
				future_values[1] += 50000;
			}
			else {
				future_values[1] += 500;
			}
		}
		else if (target_y < y - 2000 && x < target_x && target_x < x + 5000 && future_values[2] >= 400) {
			if (future_values[2] >= 600) {
				future_values[2] += 50000;
			}
			else {
				future_values[2] += 500;
			}
		}
		else {
			if (target_y < y && future_values[2] >= 1000) {
				future_values[2] += chase_const;
			}
			if (target_y > y && future_values[0] >= 1000) {
				future_values[0] += chase_const;
			}
		}
	}

	if (cycle->get_rotation() == 90 && targets[0]->get_rotation() == 180) { // ai going to the right, target coming from above
		if (y - 5000 < target_y && target_y < y && x - 2000 < target_x && target_x < x + 300 && future_values[1] >= 400) {
			if (future_values[1] >= 600) {
				future_values[1] += 50000;
			}
			else {
				future_values[1] += 500;
			}
		}
		else if (y - 5000 < target_y && target_y < y && target_x < x - 2000 && future_values[0] >= 400) {
			if (future_values[0] >= 600) {
				future_values[0] += 50000;
			}
			else {
				future_values[0] += 500;
			}
		}
		else {
			if (target_x < x && future_values[2] >= 1000) {
				future_values[2] += chase_const;
			}
			if (target_x > x && future_values[0] >= 1000) {
				future_values[0] += chase_const;
			}
		}
	}

	if (cycle->get_rotation() == 90 && targets[0]->get_rotation() == 0) { // ai going to the right, target coming from below
		if (y < target_y && target_y < y + 5000 && x - 2000 < target_x && target_x < x + 300 && future_values[1] >= 400) {
			if (future_values[1] >= 600) {
				future_values[1] += 50000;
			}
			else {
				future_values[1] += 500;
			}
		}
		else if (y < target_y && target_y < y + 5000 && target_x < x - 2000 && future_values[2] >= 400) {
			if (future_values[2] >= 600) {
				future_values[2] += 50000;
			}
			else {
				future_values[2] += 500;
			}
		}
		else {
			if (target_x < x && future_values[0] >= 1000) {
				future_values[0] += chase_const;
			}
			if (target_x > x && future_values[2] >= 1000) {
				future_values[2] += chase_const;
			}
		}
	}

	if (cycle->get_rotation() == 270 && targets[0]->get_rotation() == 180) { // ai going to the left, target coming from above
		if (y - 5000 < target_y && target_y < y && x - 300 < target_x && target_x < x + 2000 && future_values[1] >= 400) {
			if (future_values[1] >= 600) {
				future_values[1] += 50000;
			}
			else {
				future_values[1] += 500;
			}
		}
		else if (y - 5000 < target_y && target_y < y && target_x > x + 2000 && future_values[2] >= 400) {
			if (future_values[2] >= 600) {
				future_values[2] += 50000;
			}
			else {
				future_values[2] += 500;
			}
		}
		else {
			if (target_x < x && future_values[2] >= 1000) {
				future_values[2] += chase_const;
			}
			if (target_x > x && future_values[0] >= 1000) {
				future_values[0] += chase_const;
			}
		}
	}

	if (cycle->get_rotation() == 270 && targets[0]->get_rotation() == 0) { // ai going to the left, target coming from below
		if (y < target_y && target_y < y + 5000 && x - 300 < target_x && target_x < x + 2000 && future_values[1] >= 400) {
			if (future_values[1] >= 600) {
				future_values[1] += 50000;
			}
			else {
				future_values[1] += 500;
			}
		}
		else if (y < target_y && target_y < y + 5000 && target_x > x + 2000 && future_values[0] >= 400) {
			if (future_values[0] >= 600) {
				future_values[0] += 50000;
			}
			else {
				future_values[0] += 500;
			}
		}
		else {
			if (target_x < x && future_values[0] >= 1000) {
				future_values[0] += chase_const;
			}
			if (target_x > x && future_values[2] >= 1000) {
				future_values[2] += chase_const;
			}
		}
	}

	// according to the already set values choose the right direction of the available ones (no going backwards allowed)
	this->choose_direction();

	collidables.check_for_collision(*cycle);
	// if the ai's lightcycle is still alive, then its lighttrace is drawn
	cycle->draw_trace(window);
	window.draw(cycle->body);
}

/* Choose the right direction according to the values set by the make_turn function.
* Warning! Before choosing direction the future_values array has to be set and/or updated.
*/
void AI::choose_direction() {
	// get the direction with the highest priority
	float maximal_value = future_values[0];
	size_t max_inx = 0;
	for (size_t i = 1; i < future_values.size(); ++i) {
		if (future_values[i] > maximal_value) {
			maximal_value = future_values[i];
			max_inx = i;
		}
	}
	// don't make more than 2 turns in a row, this is very important for the ai to work correctly,
	// otherwise it may do many pointless turns (just to remain at a "safe" position)
	if (turns_in_a_row <= max_turns_in_a_row) { // if not too many turns in a row then:
		switch (max_inx) {
		case 0:
			timer_started = true;
			cycle->turn_left();
			turned_left = true;
			if (timer < 0.5f) { // if there was a turn right before this one, less than 0.5 sec ago, then increment the turn counter
				this->turns_in_a_row++;
			}
			else {
				timer = 0; // else reset the timer
			}
			break;
		case 2:
			timer_started = true;
			cycle->turn_right();
			turned_right = true;
			if (timer < 0.5f) {
				this->turns_in_a_row++;
			}
			else {
				timer = 0;
			}
		default:
			break;
		}
	}
	else { // else too many turns in a row:
		if (timer >= 1.f) { // then wait until the timer hits 1 second
			turns_in_a_row = 0; // after that reset the timer and start counting the turns again
			timer = 0;
			timer_started = false;
		}
	}
}

void AI::add_new_target(std::shared_ptr<Lightcycle> new_one) {
	// check if the lightcycle was not already added
	for (size_t i = 0; i < targets.size(); ++i) {
		// the new lightcycle has to be different from "this" one
		if (*this->cycle == *targets[i]) {
			return;
		}
		// the lightcycle was already added
		if (targets[i] == new_one) {
			return;
		}
	}
	// the new_one is not present in the targets vector so add it
	this->targets.push_back(new_one);
}

bool AI::is_alive() {
	return cycle->isAlive;
}