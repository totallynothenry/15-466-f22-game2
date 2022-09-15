#pragma once

#include "Scene.hpp"

#include <glm/glm.hpp>

#include <array>
#include <cmath>

#define BLOCK_LENGTH 2		// Meters, block is 2m x 2m x 2m
#define STAGE_LENGTH 10		// stage floor is 10 blocks x 10 blocks
#define STAGE_HEIGHT 20		// stage is 20 blocks tall not counting floor

#define CONVERT_LENGTH(X) (((int)std::floor(X + 0.5f) - 1) / 2 + 5)
#define CONVERT_HEIGHT(Z) (((int)std::floor(Z + 0.5f) - 1) / 2)
#define GET_SLICE_IDX(Y, X) ((Y) * STAGE_LENGTH + (X))

// Location all pieces initial are at on creation
const glm::vec3 SPAWN_POS = glm::vec3(0.0f, 0.0f, (float)(STAGE_HEIGHT));

// Expected mesh names
const std::string CUBE_MESH_NAME_RED = "CubeRed";
const std::string CUBE_MESH_NAME_ORANGE = "CubeOrange";
const std::string CUBE_MESH_NAME_YELLOW = "CubeYellow";
const std::string CUBE_MESH_NAME_GREEN = "CubeGreen";
const std::string CUBE_MESH_NAME_TEAL = "CubeTeal";
const std::string CUBE_MESH_NAME_BLUE = "CubeBlue";
const std::string CUBE_MESH_NAME_PURPLE = "CubePurple";

extern Scene::Drawable::Pipeline CUBE_PIPELINE_RED;
extern Scene::Drawable::Pipeline CUBE_PIPELINE_ORANGE;
extern Scene::Drawable::Pipeline CUBE_PIPELINE_YELLOW;
extern Scene::Drawable::Pipeline CUBE_PIPELINE_GREEN;
extern Scene::Drawable::Pipeline CUBE_PIPELINE_TEAL;
extern Scene::Drawable::Pipeline CUBE_PIPELINE_BLUE;
extern Scene::Drawable::Pipeline CUBE_PIPELINE_PURPLE;

extern Scene::Drawable *OCTRIS_BASE;

/**
 * Stage is (STAGE_HEIGHT) slices of (BLOCK_LENGTH) by (BLOCK_LENGTH) planes (row-major) stacked together.
 */
typedef std::array< Scene::Drawable *, STAGE_LENGTH * STAGE_LENGTH > OctrisSlice;
typedef std::vector< OctrisSlice > OctrisStage;

// All the piece types
enum OctrisPieceType {
	O, // 2-deep cube
	I, // 2x4 plane
	T, // 2-deep T shape
	S, // 2-deep S shape
	L, // 2-deep L shape
	Z, // Two parallel 2x2 planes touching on one corner
	P, // 3x3 plane with a corner cut out
};

// An 8-block octris piece (pseudo-hierarchy)
struct OctrisPiece {
	OctrisPiece();
	OctrisPiece(OctrisPieceType type);
	OctrisPiece(OctrisPiece const &) = delete;

	// Tracks the origin of this piece
	Scene::Transform origin;

	// Offsets for each of the component blocks making up this piece
	std::array< Scene::Transform, 8> block_offsets;

	// Drawables for each of the component blocks making up this piece
	std::vector< Scene::Drawable * > blocks;

	// Translate piece w.r.t board coordiantes
	void translate(int x, int y, int z);

	// Rotate piece (args are factors of pi/2, angles in radians)
	void rotate(int phi, int theta, int psi);

	// Checks if this piece collides with any block on the board
	bool collides(OctrisStage &stage);

	// Adds this piece's blocks to the board
	void add_to_board(OctrisStage &stage);

	/**
	 * Moves this piece by the specified amounts if it does not collide with stage, returning true.
	 * If movement would cause collision, do not move and return false.
	 */
	bool OctrisPiece::move(OctrisStage &stage, int x, int y, int z, int phi, int theta, int psi);

	/**
	 * Attempts move(board, 0, 0, -1). If false, adds this piece to the board.
	 */
	bool fall(OctrisStage &stage);
};

