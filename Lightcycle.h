#ifndef LIGHTCYCLE_H
#define LIGHTCYCLE_H

#include <SFML/Graphics.hpp>
#include <deque>
#include <algorithm>
#include <vector>
#include <map>

using std::deque;

class Lightcycle {
public:
	sf::RectangleShape body;
	bool turned_right = false;
	bool turned_left = false;
	bool accelerated = false;
	bool slowed_down = false;
	bool stopped = false;
	size_t ID;
	std::vector<float> possible_directions;
	std::map<float, float> closest_collidables;

	bool isAlive;

	deque<sf::RectangleShape> light_trace; // it is easier to opperate directly with the light trace

	Lightcycle(const Lightcycle& other);
	Lightcycle(Lightcycle&& other) noexcept;
	Lightcycle(sf::Vector2f size, sf::Vector2f pos, sf::Color color, const sf::Texture* texture, size_t ID);
	Lightcycle(sf::Vector2f size, sf::Vector2f pos, sf::Color color, const sf::Texture* texture, size_t ID, size_t cheating_level);

	const sf::FloatRect get_collision_box() const;
	const sf::Vector2f get_position() const;
	float get_rotation();

	bool turn_left();
	bool turn_right();

	sf::Vector2f get_direction_vec();

	// not used
	sf::Vector2f get_direction_vec(float custom_rotation);

	float get_velocity();

	/**
	Gets the possible future lightcycle moves and then adds their rotations to the possible_directions vector.
	Useful if you want to count the value of the future turn via sending a "collision detection ray" in the direction of the future turn (rotation).
	*/
	void update_possible_directions();

	void update_position(float time_s);

	void accelerate(float time_s);

	void slow_down(float time_s);

	void turn_OnOff_light_trace();

	void explode();

	void stop();

	/**
	Checks for collision with light cycle's own trace.
	*/
	void check_for_crash();

	/**
	Checks for collision with another collision box.
	*/
	void check_for_crash(const sf::FloatRect& otherBox);

	/**
	Checks for collision with another light cycle.
	*/
	void check_for_crash(const Lightcycle& other);

	/**
	Checks for collision with the grid boundry (the walls).
	*/
	void check_for_crash_with_wall(const std::vector<sf::RectangleShape>& grid_bounds);

	void draw_trace(sf::RenderWindow& window);

	void destroy_light_trace();

	void set_cheating_lvl(size_t cheating_level);

	bool operator ==(const Lightcycle& other) const {
		if (this->body.getFillColor() != other.body.getFillColor()) return false;
		if (this->light_trace_active != other.light_trace_active) return false;
		if (this->body.getPosition() != other.body.getPosition()) return false;
		if (this->body.getTexture() != other.body.getTexture()) return false;
		if (this->body.getOrigin() != other.body.getOrigin()) return false;
		if (this->body.getScale() != other.body.getScale()) return false;
		if (this->cheating_lvl != other.cheating_lvl) return false;
		if (this->velocity != other.velocity) return false;
		if (this->rotation != other.rotation) return false;
		if (this->ID != other.ID) return false;
		return true;
	}

	bool operator !=(const Lightcycle& other) const {
		return !(*this == other);
	}

private:
	float velocity;
	float rotation = 0.f;
	const float SurfaceFriction = 500.f;
	float Acceleration = 1800.f;
	const size_t MaximumTraceLines = 100;
	float MaxVelocity;
	float trace_scale_x = 1.f;
	float trace_scale_y = 1.f;
	sf::Vector2f last_trace_end;

	size_t cheating_lvl = 0;

	bool light_trace_active = false;

	void init_closest_collidables();

	void bound_rotation();

	void bound_rotation(float& custom_rotation);

	void apply_friction(float time_s);

	bool collided(const sf::FloatRect& otherBox);

	void spawn_light_trace_corner();

	void fit_collider_between(sf::RectangleShape& trace, sf::Vector2f a, sf::Vector2f b);

	void make_light_trace_smaller();
};

#endif