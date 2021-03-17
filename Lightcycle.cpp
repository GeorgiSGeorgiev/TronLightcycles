#pragma warning(disable:26812) // third party enum warning

#include "Lightcycle.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <deque>
#include <algorithm>
#include <vector>

using std::deque;

Lightcycle::Lightcycle(sf::Vector2f size, sf::Vector2f pos, sf::Color color, const sf::Texture* texture, size_t ID) {
	body.setSize(size);
	body.setPosition(pos);
	body.setFillColor(color);
	body.setOrigin(size.x / 2, size.y + 8);
	body.setTexture(texture);
	MaxVelocity = 0;

	this->isAlive = true;
	this->velocity = 0;
	this->ID = ID;
	this->update_possible_directions();
	this->init_closest_collidables();
}

Lightcycle::Lightcycle(sf::Vector2f size, sf::Vector2f pos, sf::Color color, const sf::Texture* texture, size_t ID, size_t cheating_level) {
	body.setSize(size);
	body.setPosition(pos);
	body.setFillColor(color);
	body.setOrigin(size.x / 2, size.y + 8);
	body.setTexture(texture);
	this->cheating_lvl = cheating_level;
	if (cheating_lvl == 0) {
		MaxVelocity = 6000;
	}
	else if (cheating_lvl == 1) {
		this->MaxVelocity = 6500;
		this->Acceleration = 2300;
	}
	else if (cheating_lvl == 2) {
		this->MaxVelocity = 7000;
		this->Acceleration = 3500;
	}

	this->isAlive = true;
	this->velocity = 0;
	this->ID = ID;
	this->update_possible_directions();
	this->init_closest_collidables();
}

Lightcycle::Lightcycle(const Lightcycle& other) {
	body.setSize(other.body.getSize());
	body.setPosition(other.body.getPosition());
	body.setFillColor(other.body.getFillColor());
	body.setOrigin(other.body.getOrigin());
	body.setTexture(other.body.getTexture());
	this->MaxVelocity = other.MaxVelocity;
	this->Acceleration = other.Acceleration;

	this->isAlive = true;
	this->rotation = other.rotation;
	this->velocity = other.velocity;
	this->ID = other.ID;
	this->possible_directions = other.possible_directions;
	this->closest_collidables = other.closest_collidables;
}

Lightcycle::Lightcycle(Lightcycle&& other) noexcept {
	body.setSize(other.body.getSize());
	body.setPosition(other.body.getPosition());
	body.setFillColor(other.body.getFillColor());
	body.setOrigin(other.body.getOrigin());
	body.setTexture(other.body.getTexture());

	other.body.setSize(sf::Vector2f(0, 0));
	other.body.setPosition(sf::Vector2f(0, 0));
	other.body.setFillColor(sf::Color::White);
	other.body.setOrigin(sf::Vector2f(0, 0));
	other.body.setTexture(NULL);

	this->MaxVelocity = other.MaxVelocity;
	other.MaxVelocity = 0;
	this->Acceleration = other.Acceleration;
	other.Acceleration = 0;
	this->isAlive = true;
	other.isAlive = false;
	this->rotation = other.rotation;
	other.rotation = 0;
	this->velocity = other.velocity;
	other.velocity = 0;
	this->ID = other.ID;
	other.ID = 0;
	this->possible_directions = other.possible_directions;
	other.possible_directions.clear();
	this->closest_collidables = other.closest_collidables;
	other.closest_collidables.clear();
}

void Lightcycle::init_closest_collidables() {
	this->closest_collidables.emplace(0.f, 0.f);
	this->closest_collidables.emplace(90.f, 0.f);
	this->closest_collidables.emplace(180.f, 0.f);
	this->closest_collidables.emplace(270.f, 0.f);
}

void Lightcycle::set_cheating_lvl(size_t cheating_level) {
	this->cheating_lvl = cheating_level;
	if (cheating_lvl == 0) {
		this->MaxVelocity = 6000.f;
		this->Acceleration = 1800.f;
	}
	else if (cheating_lvl == 1) {
		this->MaxVelocity = 6500.f;
		this->Acceleration = 2300.f;
	}
	else if (cheating_lvl == 2) {
		this->MaxVelocity = 8000.f;
		this->Acceleration = 2800.f;
	}
}

const sf::FloatRect Lightcycle::get_collision_box() const {
	return body.getGlobalBounds();
}

const sf::Vector2f Lightcycle::get_position() const {
	return body.getPosition();
}

float Lightcycle::get_rotation() {
	return this->rotation;
}

float Lightcycle::get_velocity() {
	return this->velocity;
}


bool Lightcycle::turn_left() {
	if (velocity < 50.f) {
		return false;
	}
	if (!this->isAlive) return false;

	this->rotation -= 90;
	this->velocity *= 0.88f;
	this->bound_rotation();
	this->body.setRotation(this->rotation);
	this->update_possible_directions();
	this->spawn_light_trace_corner();
	return true;
}

bool Lightcycle::turn_right() {
	if (velocity < 50.f) {
		return false;
	}
	if (!this->isAlive) return false;

	this->rotation += 90;
	this->velocity *= 0.88f;
	this->bound_rotation();
	this->body.setRotation(this->rotation);
	this->update_possible_directions();
	this->spawn_light_trace_corner();
	return true;
}

sf::Vector2f Lightcycle::get_direction_vec() {
	if (rotation == 0) {
		return sf::Vector2f(0.f, -1.f);
	}
	if (rotation == 90) {
		return sf::Vector2f(1.f, 0.f);
	}
	if (rotation == 180) {
		return sf::Vector2f(0.f, 1.f);
	}
	if (rotation == 270) {
		return sf::Vector2f(-1.f, 0.f);
	}
	return sf::Vector2f(0.f, 0.f);
}

// not used
sf::Vector2f Lightcycle::get_direction_vec(float custom_rotation) {
	if (custom_rotation == 0) {
		return sf::Vector2f(0.f, -1.f);
	}
	if (custom_rotation == 90) {
		return sf::Vector2f(1.f, 0.f);
	}
	if (custom_rotation == 180) {
		return sf::Vector2f(0.f, 1.f);
	}
	if (custom_rotation == 270) {
		return sf::Vector2f(-1.f, 0.f);
	}
	return sf::Vector2f(0.f, 0.f);
}

void Lightcycle::update_possible_directions() {
	this->possible_directions.clear();
	float direction = this->rotation;
	for (int i = -1; i < 2; ++i) {
		direction = this->rotation + 90 * i;
		bound_rotation(direction);
		this->possible_directions.push_back(direction);
	}
}

void Lightcycle::update_position(float time_s) {
	sf::Vector2f dir = get_direction_vec();
	apply_friction(time_s);
	body.move(time_s * velocity * dir.x, time_s * velocity * dir.y);

	if (this->light_trace_active) {
		this->fit_collider_between(this->light_trace[light_trace.size() - 1], this->last_trace_end, this->body.getPosition());
	}
}

void Lightcycle::spawn_light_trace_corner() {
	if (this->light_trace_active) {
		this->last_trace_end = this->body.getPosition();

		float x = 0;
		float y = 0;
		float wi = 15.f;
		float hei = 15.f;
		y = this->body.getPosition().y;
		x = this->body.getPosition().x;

		if (this->rotation == 0) {
			x -= 7.5f;
			y -= 7.f;
		}
		else if (this->rotation == 270) {
			x -= 7.f;
			y -= 7.5f;
		}
		else if (this->rotation == 180) {
			x -= 7.5f;
			y += 7.f;
		}
		else if (this->rotation == 90) {
			x += 7.f;
			y -= 7.5f;
		}

		sf::RectangleShape rect(sf::Vector2f(wi, hei));
		rect.setPosition(x, y);
		rect.setFillColor(this->body.getFillColor());
		if (this->light_trace.size() >= MaximumTraceLines) {
			this->light_trace.pop_front();
		}
		this->light_trace.push_back(rect);
	}
}

void Lightcycle::accelerate(float time_s) {
	if (velocity > MaxVelocity) {
		return;
	}
	if (!this->isAlive || this->stopped) {
		this->velocity = 0;
		return;
	}
	velocity += time_s * Acceleration;
}

void Lightcycle::slow_down(float time_s) {
	if (velocity == 4200) {
		return;
	}
	velocity -= time_s * (Acceleration / 2);
	if (velocity < 4200) {
		velocity = 4200;
	}
}

void Lightcycle::turn_OnOff_light_trace() {
	if (this->light_trace_active) {
		this->spawn_light_trace_corner(); // the positioning of the rows here is important, otherwise the trace will not spawn
		this->light_trace_active = false;
		return;
	}
	if (!this->light_trace_active) {
		this->light_trace_active = true; // the positioning of the rows here is important, otherwise the trace will not spawn
		this->spawn_light_trace_corner();
		return;
	}
}

void Lightcycle::explode() {
	this->isAlive = false;
	this->velocity = 0;
}

void Lightcycle::stop() {
	this->velocity = 0;
	this->stopped = true;
}

void Lightcycle::check_for_crash() {
	for (size_t i = 0; i < this->light_trace.size(); i++) {
		if (this->collided(this->light_trace[i].getGlobalBounds())) {
			this->explode();
			return;
		}
	}
}

void Lightcycle::check_for_crash(const sf::FloatRect& otherBox) {
	if (otherBox != this->get_collision_box() && this->collided(otherBox)) {
		this->explode();
	}
}

void Lightcycle::check_for_crash(const Lightcycle& other) {
	if (&other != this && this->collided(other.get_collision_box())) {
		this->explode();
		return;
	}
	for (size_t i = 0; i < other.light_trace.size(); i++) {
		if (this->collided(other.light_trace[i].getGlobalBounds())) {
			this->explode();
			return;
		}
	}
}

void Lightcycle::check_for_crash_with_wall(const std::vector<sf::RectangleShape>& grid_bounds) {
	for (size_t i = 0; i < 4; i++) {
		if (this->collided(grid_bounds[i].getGlobalBounds())) {
			this->explode();
			return;
		}
	}
}

void Lightcycle::destroy_light_trace() {
	// trace is still visible, make it smaller by rescaling
	if (trace_scale_y > 0 && trace_scale_x > 0) {
		this->make_light_trace_smaller();
	}
	// light trace is not visible at this moment but is still in the deque
	if (!this->light_trace.empty() && trace_scale_y == 0 && trace_scale_x == 0) {
		this->light_trace.clear();
		this->light_trace_active = false;
	}
}

void Lightcycle::bound_rotation() {
	if (this->rotation < 0) {
		this->rotation += 360;
	}
	if (this->rotation >= 360) {
		this->rotation -= 360;
	}
}

void Lightcycle::bound_rotation(float& custom_rotation) {
	if (custom_rotation < 0) {
		custom_rotation += 360;
	}
	if (custom_rotation >= 360) {
		custom_rotation -= 360;
	}
}

void Lightcycle::apply_friction(float time_s) {
	if (velocity == 0) return;
	// apply forwards movement friction
	if (velocity > 4200) {
		velocity -= time_s * SurfaceFriction;
		// the friction must not change the movement direction
		if (velocity < 4200) {
			velocity = 4200;
		}
	}
}

bool Lightcycle::collided(const sf::FloatRect& otherBox) {
	if (this->get_collision_box().intersects(otherBox)) {
		return true;
	}
	return false;
}

void Lightcycle::fit_collider_between(sf::RectangleShape& trace, sf::Vector2f a, sf::Vector2f b) {
	// scale the trace
	float dist = (b.x - a.x) + (b.y - a.y);

	if (a.x != b.x) {
		trace.setSize(sf::Vector2f(dist, 15));
	}
	else {
		trace.setSize(sf::Vector2f(15, dist));
	}
}

void Lightcycle::draw_trace(sf::RenderWindow& window) {
	for (size_t i = 0; i < this->light_trace.size(); i++) {
		window.draw(this->light_trace[i]);
	}
}

void Lightcycle::make_light_trace_smaller() {
	for (auto& trace : this->light_trace) {
		trace_scale_x -= 0.003f;
		trace_scale_y -= 0.003f;
		if (trace_scale_x < 0) trace_scale_x = 0;
		if (trace_scale_y < 0) trace_scale_y = 0;
		trace.setScale(trace_scale_x, trace_scale_y);
	}
}