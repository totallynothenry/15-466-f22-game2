#pragma once

#include "Scene.hpp"

#include <glm/glm.hpp>

#include <array>

#define BLOCK_LENGTH 2							// Meters, block is 2m x 2m x 2m
#define STAGE_LENGTH (10 * (BLOCK_LENGTH))		// Meters, stage floor is 10 blocks x 10 blocks
#define STAGE_HEIGHT (8 * (BLOCK_LENGTH))		// Meters, stage is 20 blocks tall not counting floor

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

enum OctrisPieceType {
	CUBE
};

// An 8-block octris piece (pseudo-hierarchy)
struct OctrisPiece {
	OctrisPiece();
	OctrisPiece(OctrisPieceType type);
	OctrisPiece(OctrisPiece const &) = delete;

	// Transform tracking the origin of this piece
	Scene::Transform transform;

	// Transforms for each of the component blocks making up this piece
	std::array< Scene::Transform, 8> block_transforms;

	// Drawables for each of the component blocks making up this piece
	std::vector< Scene::Drawable > blocks;

	// Whether we should be hiding each of the component blocks (set to true once cleared)
	std::array< bool, 8 > should_hide = { false, false, false, false, false, false, false, false };

	void translate(int x, int y, int z);

	bool collides(const std::vector< OctrisPiece * > &pieces);

	bool fall(const std::vector< OctrisPiece * > &pieces);
};

// TODO: More pieces here

