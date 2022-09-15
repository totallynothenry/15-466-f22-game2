#include "PlayMode.hpp"

#include "LitColorTextureProgram.hpp"

#include "DrawLines.hpp"
#include "Mesh.hpp"
#include "Load.hpp"
#include "gl_errors.hpp"
#include "data_path.hpp"
#include "logging.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <string>

GLuint octris_meshes_for_lit_color_texture_program = 0;
Load< MeshBuffer > hexapod_meshes(LoadTagDefault, []() -> MeshBuffer const * {
	MeshBuffer const *ret = new MeshBuffer(data_path("octris.pnct"));
	octris_meshes_for_lit_color_texture_program = ret->make_vao_for_program(lit_color_texture_program->program);
	return ret;
});

static void init_pipeline(Scene::Drawable::Pipeline &pipeline, const Mesh &mesh) {
	pipeline = lit_color_texture_program_pipeline;
	pipeline.vao = octris_meshes_for_lit_color_texture_program;
	pipeline.type = mesh.type;
	pipeline.start = mesh.start;
	pipeline.count = mesh.count;
}

Load< Scene > hexapod_scene(LoadTagDefault, []() -> Scene const * {
	return new Scene(data_path("octris.scene"), [&](Scene &scene, Scene::Transform *transform, std::string const &mesh_name){
		Mesh const &mesh = hexapod_meshes->lookup(mesh_name);

		if (mesh_name == CUBE_MESH_NAME_RED) {
			init_pipeline(CUBE_PIPELINE_RED, mesh);
			LOG("Loaded red");
			return;
		}

		if (mesh_name == CUBE_MESH_NAME_ORANGE) {
			init_pipeline(CUBE_PIPELINE_ORANGE, mesh);
			LOG("Loaded orange");
			return;
		}

		if (mesh_name == CUBE_MESH_NAME_YELLOW) {
			init_pipeline(CUBE_PIPELINE_YELLOW, mesh);
			LOG("Loaded yellow");
			return;
		}

		if (mesh_name == CUBE_MESH_NAME_GREEN) {
			init_pipeline(CUBE_PIPELINE_GREEN, mesh);
			LOG("Loaded green");
			return;
		}

		if (mesh_name == CUBE_MESH_NAME_TEAL) {
			init_pipeline(CUBE_PIPELINE_TEAL, mesh);
			LOG("Loaded teal");
			return;
		}

		if (mesh_name == CUBE_MESH_NAME_BLUE) {
			init_pipeline(CUBE_PIPELINE_BLUE, mesh);
			LOG("Loaded blue");
			return;
		}

		if (mesh_name == CUBE_MESH_NAME_PURPLE) {
			init_pipeline(CUBE_PIPELINE_PURPLE, mesh);
			LOG("Loaded purple");
			return;
		}

		LOG("Loaded base");
		OCTRIS_BASE = new Scene::Drawable(transform);
		init_pipeline(OCTRIS_BASE->pipeline, mesh);
	});
});

PlayMode::PlayMode() : scene(*hexapod_scene) {
	//get pointers to leg for convenience:
	// for (auto &transform : scene.transforms) {
	// 	if (transform.name == "Hip.FL") hip = &transform;
	// 	else if (transform.name == "UpperLeg.FL") upper_leg = &transform;
	// 	else if (transform.name == "LowerLeg.FL") lower_leg = &transform;
	// }
	// if (hip == nullptr) throw std::runtime_error("Hip not found.");
	// if (upper_leg == nullptr) throw std::runtime_error("Upper leg not found.");
	// if (lower_leg == nullptr) throw std::runtime_error("Lower leg not found.");

	// hip_base_rotation = hip->rotation;
	// upper_leg_base_rotation = upper_leg->rotation;
	// lower_leg_base_rotation = lower_leg->rotation;

	current_piece = new OctrisPiece();

	//get pointer to camera for convenience:
	if (scene.cameras.size() != 1) throw std::runtime_error("Expecting scene to have exactly one camera, but it has " + std::to_string(scene.cameras.size()));
	camera = &scene.cameras.front();
}

static void cleanup_slice(OctrisSlice &slice) {
	for (auto drawable : slice) {
		if (drawable == nullptr) continue;
		delete drawable->transform;
		delete drawable;
	}
}

PlayMode::~PlayMode() {
	for (auto &slice : stage) {
		cleanup_slice(slice);
	}
	for (auto drawable : current_piece->blocks) {
		delete drawable;
	}
	delete OCTRIS_BASE;
}

PlayMode::Button *PlayMode::get_button(SDL_Event const &evt) {
	switch (evt.key.keysym.sym) {
	case SDLK_ESCAPE:
		SDL_SetRelativeMouseMode(SDL_FALSE);
		return nullptr;
	case SDLK_LEFT:
		return &left;
	case SDLK_RIGHT:
		return &right;
	case SDLK_UP:
		return &up;
	case SDLK_DOWN:
		return &down;
	case SDLK_p:
		return &pause;
	case SDLK_SPACE:
		return &slam;
	case SDLK_w:
		return &trans_pos_y;
	case SDLK_s:
		return &trans_neg_y;
	case SDLK_a:
		return &trans_neg_x;
	case SDLK_d:
		return &trans_pos_x;
	case SDLK_x:
		return &trans_neg_z;
	case SDLK_u:
	case SDLK_KP_7:
		return &rot_countr;
	case SDLK_o:
	case SDLK_KP_9:
		return &rot_clockw;
	case SDLK_i:
	case SDLK_KP_8:
		return &rot_pos_y;
	case SDLK_k:
	case SDLK_KP_5:
		return &rot_neg_y;
	case SDLK_l:
	case SDLK_KP_6:
		return &rot_pos_x;
	case SDLK_j:
	case SDLK_KP_4:
		return &rot_neg_x;
	default:
		return nullptr;
	}
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {
	if (evt.type == SDL_KEYDOWN) {
		Button *button = get_button(evt);
		if (button != nullptr) {
			button->downs += 1;
			button->pressed = true;
			return true;
		}
	} else if (evt.type == SDL_KEYUP) {
		Button *button = get_button(evt);
		if (button != nullptr) {
			button->pressed = false;
			return true;
		}
	} else if (evt.type == SDL_MOUSEBUTTONDOWN) {
		if (SDL_GetRelativeMouseMode() == SDL_FALSE) {
			SDL_SetRelativeMouseMode(SDL_TRUE);
			return true;
		}
	} else if (evt.type == SDL_MOUSEMOTION) {
		if (SDL_GetRelativeMouseMode() == SDL_TRUE) {
			glm::vec2 motion = glm::vec2(
				evt.motion.xrel / float(window_size.y),
				-evt.motion.yrel / float(window_size.y)
			);
			camera->transform->rotation = glm::normalize(
				camera->transform->rotation
				* glm::angleAxis(-motion.x * camera->fovy, glm::vec3(0.0f, 1.0f, 0.0f))
				* glm::angleAxis(motion.y * camera->fovy, glm::vec3(1.0f, 0.0f, 0.0f))
			);
			return true;
		}
	}

	return false;
}

void PlayMode::update_stage() {
	unsigned int shift = 0;

	for (size_t slice_idx = 0; true; slice_idx++) {
		if (slice_idx >= stage.size()) break;

		OctrisSlice &slice = stage[slice_idx];

		//Check if slice is filled
		bool is_filled = true;
		for (auto drawable : slice) {
			is_filled = is_filled && (drawable != nullptr);
		}

		if (is_filled) {
			LOG("Pruning slice " << slice_idx);
			//Get rid of this slice
			cleanup_slice(slice);
			stage.erase(stage.begin() + slice_idx);
			assert(slice_idx >= 0);
			slice_idx--; // keeps slice_idx the same for the next iteration
			shift++;
			continue;
		}

		if (shift == 0) continue;

		//Update z-offset of all blocks in the slice
		LOG("Shifting slice " << slice_idx << " by " << shift);
		for (auto drawable : slice) {
			if (drawable == nullptr) continue;
			drawable->transform->position.z -= (float)(shift * BLOCK_LENGTH);
		}
	}

	// Same scoring as BPS tetris (since we don't have a level mechanic)
	switch (shift) {
	case 0:
		return;
	case 1:
		score += 40;
		return;
	case 2:
		score += 100;
		return;
	case 3:
		score += 300;
		return;
	default:
		score += 1200;
		return;
	}
}

void PlayMode::update(float elapsed) {

	{ //move camera:

		//combine inputs into a move:
		constexpr float PlayerSpeed = 30.0f;
		glm::vec2 move = glm::vec2(0.0f);
		if (left.pressed && !right.pressed) move.x =-1.0f;
		if (!left.pressed && right.pressed) move.x = 1.0f;
		if (down.pressed && !up.pressed) move.y =-1.0f;
		if (!down.pressed && up.pressed) move.y = 1.0f;

		//make it so that moving diagonally doesn't go faster:
		if (move != glm::vec2(0.0f)) move = glm::normalize(move) * PlayerSpeed * elapsed;

		glm::mat4x3 frame = camera->transform->make_local_to_parent();
		glm::vec3 frame_right = frame[0];
		//glm::vec3 up = frame[1];
		glm::vec3 frame_forward = -frame[2];

		camera->transform->position += move.x * frame_right + move.y * frame_forward;
	}

	//toggle pause
	if (pause.downs > 0) game_paused = !game_paused;

	//reset button press counters:
	left.downs = 0;
	right.downs = 0;
	up.downs = 0;
	down.downs = 0;
	pause.downs = 0;

	if (game_over || game_paused) {
		trans_pos_x.downs = 0;
		trans_neg_x.downs = 0;
		trans_pos_y.downs = 0;
		trans_neg_y.downs = 0;
		trans_neg_z.downs = 0;
		rot_pos_x.downs = 0;
		rot_neg_x.downs = 0;
		rot_pos_y.downs = 0;
		rot_neg_y.downs = 0;
		rot_clockw.downs = 0;
		rot_countr.downs = 0;
		slam.downs = 0;

		return;
	}

	timer += elapsed;

	{ //move current piece
		int dx = 0, dy = 0, dz = 0, dphi = 0, dtheta = 0, dpsi = 0;
		if (trans_pos_x.downs > 0) dx++;
		if (trans_neg_x.downs > 0) dx--;
		if (trans_pos_y.downs > 0) dy++;
		if (trans_neg_y.downs > 0) dy--;
		if (trans_neg_z.downs > 0) dz--;

		if (rot_pos_x.downs > 0) dphi--;
		if (rot_neg_x.downs > 0) dphi++;
		if (rot_pos_y.downs > 0) dtheta--;
		if (rot_neg_y.downs > 0) dtheta++;
		if (rot_clockw.downs > 0) dpsi--;
		if (rot_countr.downs > 0) dpsi++;

		if (dx != 0 || dy != 0 || dz != 0 || dphi != 0 || dtheta != 0 || dpsi != 0) {
			current_piece->move(stage, dx, dy, dz, dphi, dtheta, dpsi);
		}

		if (slam.downs > 0) {
			while (current_piece->move(stage, 0, 0, -1, 0, 0, 0));
			bool spawn = !current_piece->fall(stage);
			assert(spawn);
			current_piece = new OctrisPiece();
			if (current_piece->collides(stage)) {
				game_over = true;
			}
		}
	}

	//reset piece movement button press counters
	trans_pos_x.downs = 0;
	trans_neg_x.downs = 0;
	trans_pos_y.downs = 0;
	trans_neg_y.downs = 0;
	trans_neg_z.downs = 0;
	rot_pos_x.downs = 0;
	rot_neg_x.downs = 0;
	rot_pos_y.downs = 0;
	rot_neg_y.downs = 0;
	rot_clockw.downs = 0;
	rot_countr.downs = 0;
	slam.downs = 0;

	while (timer > next_move) {
		//move current piece down if possible
		LOG("move at " << next_move);
		bool spawn = !current_piece->fall(stage);
		update_stage();

		if (spawn) {
			current_piece = new OctrisPiece();
			if (current_piece->collides(stage)) {
				game_over = true;
			}
		}

		next_move += DURATION_BTW_MOVES;
	}
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
	{ //repopulate drawable (yes, this is terrible, but I don't have time to change scene to fix it)
		scene.drawables.clear();

		//base
		scene.drawables.emplace_back(*OCTRIS_BASE);

		//stage
		for (auto &slice : stage) {
			for (auto drawable : slice) {
				if (drawable == nullptr) continue;
				scene.drawables.emplace_back(*drawable);
			}
		}

		// current piece
		for (int i = 0; i < 8; i++) {
			scene.drawables.emplace_back(*(current_piece->blocks[i]));
		}
	}

	//update camera aspect ratio for drawable:
	camera->aspect = float(drawable_size.x) / float(drawable_size.y);

	//set up light type and position for lit_color_texture_program:
	// TODO: consider using the Light(s) in the scene to do this
	glUseProgram(lit_color_texture_program->program);
	glUniform1i(lit_color_texture_program->LIGHT_TYPE_int, 1);
	glUniform3fv(lit_color_texture_program->LIGHT_DIRECTION_vec3, 1, glm::value_ptr(glm::vec3(0.0f, 0.0f,-1.0f)));
	glUniform3fv(lit_color_texture_program->LIGHT_ENERGY_vec3, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 0.95f)));
	glUseProgram(0);

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClearDepth(1.0f); //1.0 is actually the default value to clear the depth buffer to, but FYI you can change it.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); //this is the default depth comparison function, but FYI you can change it.

	GL_ERRORS(); //print any errors produced by this setup code

	scene.draw(*camera);

	{ //use DrawLines to overlay some text:
		std::string text = "Mouse motion rotates camera; Arrow keys moves camera; escape ungrabs mouse";
		if (game_over) {
			text = "Game over! Score: " + std::to_string(score);
		} else if (game_paused) {
			text = "Paused: Press P to resume";
		}
		glDisable(GL_DEPTH_TEST);
		float aspect = float(drawable_size.x) / float(drawable_size.y);
		DrawLines lines(glm::mat4(
			1.0f / aspect, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		));

		constexpr float H = 0.09f;
		lines.draw_text(text,
			glm::vec3(-aspect + 0.1f * H, -1.0 + 0.1f * H, 0.0),
			glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
			glm::u8vec4(0x00, 0x00, 0x00, 0x00));
		float ofs = 2.0f / drawable_size.y;
		lines.draw_text(text,
			glm::vec3(-aspect + 0.1f * H + ofs, -1.0 + + 0.1f * H + ofs, 0.0),
			glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
			glm::u8vec4(0xff, 0xff, 0xff, 0x00));
	}
}
