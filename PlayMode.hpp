#pragma once

#include "Mode.hpp"

#include "Scene.hpp"
#include "OctrisPiece.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>

#define DURATION_BTW_MOVES 1.0f


struct PlayMode : Mode {
	PlayMode();
	virtual ~PlayMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//----- game state -----

	//input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up;

	//local copy of the game scene (so code can change it during gameplay):
	Scene scene;

	OctrisPiece *current_piece;
	std::vector< OctrisPiece * > pieces;

	float next_move = 3 * DURATION_BTW_MOVES;
	float timer = 0.0f;
	unsigned int score = 0;

	//camera:
	Scene::Camera *camera = nullptr;

};
