#include "OctrisPiece.hpp"

#include "logging.hpp"

#include <random>

#define CONST_PI 3.14159265358979323846f
#define ROT_FACTOR (CONST_PI / 2.0f)

// To be set during scene loading
Scene::Drawable::Pipeline CUBE_PIPELINE_RED;
Scene::Drawable::Pipeline CUBE_PIPELINE_ORANGE;
Scene::Drawable::Pipeline CUBE_PIPELINE_YELLOW;
Scene::Drawable::Pipeline CUBE_PIPELINE_GREEN;
Scene::Drawable::Pipeline CUBE_PIPELINE_TEAL;
Scene::Drawable::Pipeline CUBE_PIPELINE_BLUE;
Scene::Drawable::Pipeline CUBE_PIPELINE_PURPLE;

Scene::Drawable *OCTRIS_BASE = nullptr;

static Scene::Drawable *get_drawable(
		float offset_x, float offset_y, float offset_z,
		Scene::Transform &offset, Scene::Transform *parent,
		Scene::Drawable::Pipeline &pipeline) {
	offset.position.x = offset_x;
	offset.position.y = offset_y;
	offset.position.z = offset_z;
	offset.parent = parent;

	Scene::Drawable *drawable = new Scene::Drawable(&offset);
	drawable->pipeline = pipeline;
	return drawable;
}

static OctrisPieceType get_random_type() {
	return static_cast<OctrisPieceType>(std::rand() % 7);
}

OctrisPiece::OctrisPiece() : OctrisPiece(get_random_type()) {
}

OctrisPiece::OctrisPiece(OctrisPieceType type) {
	// Create a cube piece
	origin = Scene::Transform();
	origin.position = SPAWN_POS;

	switch (type) {
	case O:
		blocks.emplace_back(get_drawable(-1.0f, -1.0f, -1.0f, block_offsets[0], &origin, CUBE_PIPELINE_YELLOW));
		blocks.emplace_back(get_drawable(-1.0f, -1.0f,  1.0f, block_offsets[1], &origin, CUBE_PIPELINE_YELLOW));
		blocks.emplace_back(get_drawable(-1.0f,  1.0f, -1.0f, block_offsets[2], &origin, CUBE_PIPELINE_YELLOW));
		blocks.emplace_back(get_drawable(-1.0f,  1.0f,  1.0f, block_offsets[3], &origin, CUBE_PIPELINE_YELLOW));
		blocks.emplace_back(get_drawable( 1.0f, -1.0f, -1.0f, block_offsets[4], &origin, CUBE_PIPELINE_YELLOW));
		blocks.emplace_back(get_drawable( 1.0f, -1.0f,  1.0f, block_offsets[5], &origin, CUBE_PIPELINE_YELLOW));
		blocks.emplace_back(get_drawable( 1.0f,  1.0f, -1.0f, block_offsets[6], &origin, CUBE_PIPELINE_YELLOW));
		blocks.emplace_back(get_drawable( 1.0f,  1.0f,  1.0f, block_offsets[7], &origin, CUBE_PIPELINE_YELLOW));
		return;
	case I:
		blocks.emplace_back(get_drawable(1.0f, -1.0f, -3.0f, block_offsets[0], &origin, CUBE_PIPELINE_TEAL));
		blocks.emplace_back(get_drawable(1.0f, -1.0f, -1.0f, block_offsets[1], &origin, CUBE_PIPELINE_TEAL));
		blocks.emplace_back(get_drawable(1.0f, -1.0f,  1.0f, block_offsets[2], &origin, CUBE_PIPELINE_TEAL));
		blocks.emplace_back(get_drawable(1.0f, -1.0f,  3.0f, block_offsets[3], &origin, CUBE_PIPELINE_TEAL));
		blocks.emplace_back(get_drawable(1.0f,  1.0f, -3.0f, block_offsets[4], &origin, CUBE_PIPELINE_TEAL));
		blocks.emplace_back(get_drawable(1.0f,  1.0f, -1.0f, block_offsets[5], &origin, CUBE_PIPELINE_TEAL));
		blocks.emplace_back(get_drawable(1.0f,  1.0f,  1.0f, block_offsets[6], &origin, CUBE_PIPELINE_TEAL));
		blocks.emplace_back(get_drawable(1.0f,  1.0f,  3.0f, block_offsets[7], &origin, CUBE_PIPELINE_TEAL));
		return;
	case T:
		blocks.emplace_back(get_drawable(-1.0f, -1.0f,  1.0f, block_offsets[0], &origin, CUBE_PIPELINE_PURPLE));
		blocks.emplace_back(get_drawable(-1.0f,  1.0f,  1.0f, block_offsets[1], &origin, CUBE_PIPELINE_PURPLE));
		blocks.emplace_back(get_drawable(-1.0f,  3.0f,  1.0f, block_offsets[2], &origin, CUBE_PIPELINE_PURPLE));
		blocks.emplace_back(get_drawable(-1.0f,  1.0f, -1.0f, block_offsets[3], &origin, CUBE_PIPELINE_PURPLE));
		blocks.emplace_back(get_drawable( 1.0f, -1.0f,  1.0f, block_offsets[4], &origin, CUBE_PIPELINE_PURPLE));
		blocks.emplace_back(get_drawable( 1.0f,  1.0f,  1.0f, block_offsets[5], &origin, CUBE_PIPELINE_PURPLE));
		blocks.emplace_back(get_drawable( 1.0f,  3.0f,  1.0f, block_offsets[6], &origin, CUBE_PIPELINE_PURPLE));
		blocks.emplace_back(get_drawable( 1.0f,  1.0f, -1.0f, block_offsets[7], &origin, CUBE_PIPELINE_PURPLE));
		return;
	case J:
		blocks.emplace_back(get_drawable(-1.0f, -1.0f,  1.0f, block_offsets[0], &origin, CUBE_PIPELINE_BLUE));
		blocks.emplace_back(get_drawable(-1.0f, -1.0f, -1.0f, block_offsets[1], &origin, CUBE_PIPELINE_BLUE));
		blocks.emplace_back(get_drawable(-1.0f,  1.0f, -1.0f, block_offsets[2], &origin, CUBE_PIPELINE_BLUE));
		blocks.emplace_back(get_drawable(-1.0f,  3.0f, -1.0f, block_offsets[3], &origin, CUBE_PIPELINE_BLUE));
		blocks.emplace_back(get_drawable( 1.0f, -1.0f,  1.0f, block_offsets[4], &origin, CUBE_PIPELINE_BLUE));
		blocks.emplace_back(get_drawable( 1.0f, -1.0f, -1.0f, block_offsets[5], &origin, CUBE_PIPELINE_BLUE));
		blocks.emplace_back(get_drawable( 1.0f,  1.0f, -1.0f, block_offsets[6], &origin, CUBE_PIPELINE_BLUE));
		blocks.emplace_back(get_drawable( 1.0f,  3.0f, -1.0f, block_offsets[7], &origin, CUBE_PIPELINE_BLUE));
		return;
	case L:
		blocks.emplace_back(get_drawable(-3.0f, -3.0f,  1.0f, block_offsets[0], &origin, CUBE_PIPELINE_ORANGE));
		blocks.emplace_back(get_drawable(-3.0f, -1.0f,  1.0f, block_offsets[1], &origin, CUBE_PIPELINE_ORANGE));
		blocks.emplace_back(get_drawable(-3.0f,  1.0f,  1.0f, block_offsets[2], &origin, CUBE_PIPELINE_ORANGE));
		blocks.emplace_back(get_drawable(-1.0f, -3.0f,  1.0f, block_offsets[3], &origin, CUBE_PIPELINE_ORANGE));
		blocks.emplace_back(get_drawable(-1.0f, -1.0f,  1.0f, block_offsets[4], &origin, CUBE_PIPELINE_ORANGE));
		blocks.emplace_back(get_drawable(-1.0f,  1.0f,  1.0f, block_offsets[5], &origin, CUBE_PIPELINE_ORANGE));
		blocks.emplace_back(get_drawable( 1.0f, -3.0f,  1.0f, block_offsets[6], &origin, CUBE_PIPELINE_ORANGE));
		blocks.emplace_back(get_drawable( 1.0f, -1.0f,  1.0f, block_offsets[7], &origin, CUBE_PIPELINE_ORANGE));
		return;
	case S:
		blocks.emplace_back(get_drawable(-1.0f, -1.0f, -1.0f, block_offsets[0], &origin, CUBE_PIPELINE_GREEN));
		blocks.emplace_back(get_drawable(-1.0f,  1.0f, -1.0f, block_offsets[1], &origin, CUBE_PIPELINE_GREEN));
		blocks.emplace_back(get_drawable( 1.0f, -1.0f, -1.0f, block_offsets[2], &origin, CUBE_PIPELINE_GREEN));
		blocks.emplace_back(get_drawable( 1.0f,  1.0f, -1.0f, block_offsets[3], &origin, CUBE_PIPELINE_GREEN));
		blocks.emplace_back(get_drawable(-1.0f,  1.0f,  1.0f, block_offsets[4], &origin, CUBE_PIPELINE_GREEN));
		blocks.emplace_back(get_drawable(-1.0f,  3.0f,  1.0f, block_offsets[5], &origin, CUBE_PIPELINE_GREEN));
		blocks.emplace_back(get_drawable( 1.0f,  1.0f,  1.0f, block_offsets[6], &origin, CUBE_PIPELINE_GREEN));
		blocks.emplace_back(get_drawable( 1.0f,  3.0f,  1.0f, block_offsets[7], &origin, CUBE_PIPELINE_GREEN));
		return;
	case Z:
		blocks.emplace_back(get_drawable(-1.0f, -1.0f, -1.0f, block_offsets[0], &origin, CUBE_PIPELINE_RED));
		blocks.emplace_back(get_drawable(-1.0f,  1.0f, -1.0f, block_offsets[1], &origin, CUBE_PIPELINE_RED));
		blocks.emplace_back(get_drawable( 1.0f, -1.0f, -1.0f, block_offsets[2], &origin, CUBE_PIPELINE_RED));
		blocks.emplace_back(get_drawable( 1.0f,  1.0f, -1.0f, block_offsets[3], &origin, CUBE_PIPELINE_RED));
		blocks.emplace_back(get_drawable( 1.0f,  1.0f,  1.0f, block_offsets[4], &origin, CUBE_PIPELINE_RED));
		blocks.emplace_back(get_drawable( 1.0f,  3.0f,  1.0f, block_offsets[5], &origin, CUBE_PIPELINE_RED));
		blocks.emplace_back(get_drawable( 3.0f,  1.0f,  1.0f, block_offsets[6], &origin, CUBE_PIPELINE_RED));
		blocks.emplace_back(get_drawable( 3.0f,  3.0f,  1.0f, block_offsets[7], &origin, CUBE_PIPELINE_RED));
		return;
	default:
		assert(false);
	}
}

void OctrisPiece::translate(int x, int y, int z) {
	origin.position.x += (float)(x * BLOCK_LENGTH);
	origin.position.y += (float)(y * BLOCK_LENGTH);
	origin.position.z += (float)(z * BLOCK_LENGTH);
}

void OctrisPiece::rotate(int phi, int theta, int psi) {
	if (phi == 0 && theta == 0 && psi == 0) return;
	origin.rotation *=
		glm::quat(glm::vec3((float)theta * ROT_FACTOR, (float)phi * ROT_FACTOR, (float)psi * ROT_FACTOR));
}

bool OctrisPiece::collides(OctrisStage &stage) {
	int levels = (int)stage.size();
	for (auto &block_transform : block_offsets) {
		LOG("Checking block collision");
		glm::mat4x3 absolute = block_transform.make_local_to_world();
		LOG("World coords: (" << absolute[3][0] << ", " << absolute[3][1] << ", " << absolute[3][2] << ")");
		int x = CONVERT_LENGTH(absolute[3][0]);
		int y = CONVERT_LENGTH(absolute[3][1]);
		int z = CONVERT_HEIGHT(absolute[3][2]);
		LOG("Stage coords: (" << x << ", " << y << ", " << z << ")");

		if (0 > x || x >= STAGE_LENGTH || 0 > y || y >= STAGE_LENGTH) {
			LOG("bounds collide");
			return true;
		}

		if (z < 0) {
			LOG("floor collide");
			return true;
		}

		if (z >= levels) {
			LOG("block too high, skipping");
			continue;
		}

		if (stage[z][GET_SLICE_IDX(y, x)] != nullptr) {
			LOG("stage collide");
			return true;
		}
	}

	return false;
}

void OctrisPiece::add_to_board(OctrisStage &stage) {
	for (auto block : blocks) {
		LOG("Adding block to board!");
		glm::mat4x3 absolute = block->transform->make_local_to_world();
		LOG("World coords: (" << absolute[3][0] << ", " << absolute[3][1] << ", " << absolute[3][2] << ")");

		/**
		 * Create a new Transform that's relative to the world
		 * The origin is centered and rotations should be only by 90 degrees around x, y, or z axis, so we can safely
		 * ignore.
		 */
		block->transform = new Scene::Transform();
		glm::vec3 &pos = block->transform->position;
		pos.x = absolute[3][0];
		pos.y = absolute[3][1];
		pos.z = absolute[3][2];

		int x = CONVERT_LENGTH(pos.x);
		int y = CONVERT_LENGTH(pos.y);
		int z = CONVERT_HEIGHT(pos.z);
		LOG("Stage coords: (" << x << ", " << y << ", " << z << ")");

		assert(0 <= x && x < STAGE_LENGTH);
		assert(0 <= y && y < STAGE_LENGTH);
		assert(0 <= z);

		while (z >= (int)stage.size()) {
			stage.emplace_back();
		}

		assert(stage[z][GET_SLICE_IDX(y, x)] == nullptr);
		stage[z][GET_SLICE_IDX(y, x)] = block;
	}
}

bool OctrisPiece::move(OctrisStage &stage, int x, int y, int z, int phi, int theta, int psi) {
	glm::vec3 old_pos = origin.position;
	glm::quat old_rot = origin.rotation;

	rotate(phi, theta, psi);
	translate(x, y, z);

	if (collides(stage)) {
		// undo
		origin.position = old_pos;
		origin.rotation = old_rot;
		return false;
	}
	return true;
}

bool OctrisPiece::fall(OctrisStage &stage) {
	if (!move(stage, 0, 0, -1, 0, 0, 0)) {
		add_to_board(stage);
		return false;
	}
	return true;
}