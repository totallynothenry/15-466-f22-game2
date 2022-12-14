#pragma once

#include "Mode.hpp"

#include "Scene.hpp"
#include "OctrisPiece.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>

#define DURATION_BTW_MOVES 3.0f

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
	} left, right, down, up, pause, slam,
	  trans_pos_x, trans_neg_x, trans_pos_y, trans_neg_y, trans_neg_z,
	  rot_pos_x, rot_neg_x, rot_pos_y, rot_neg_y, rot_clockw, rot_countr;
	//note: rotation denominations w.r.t to bird's eye view

	Button *get_button(SDL_Event const &evt);

	//local copy of the game scene (so code can change it during gameplay):
	Scene scene;

	//current piece that's being moved
	OctrisPiece *current_piece;

	//game board
	OctrisStage stage;
	void update_stage();

	float next_move = 3 * DURATION_BTW_MOVES;
	float timer = 0.0f;
	unsigned int score = 0;
	bool game_paused = true;
	bool game_over = false;

	//camera:
	Scene::Camera *camera = nullptr;

};
