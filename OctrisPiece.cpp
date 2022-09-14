#include "OctrisPiece.hpp"

#include "logging.hpp"

#include <random>

// #define DEBUG

#define SKIP_CHECK_DIST 4.0f
#define TOLERANCE 0.5f

// To be set during scene loading
Scene::Drawable::Pipeline CUBE_PIPELINE_RED;
Scene::Drawable::Pipeline CUBE_PIPELINE_ORANGE;
Scene::Drawable::Pipeline CUBE_PIPELINE_YELLOW;
Scene::Drawable::Pipeline CUBE_PIPELINE_GREEN;
Scene::Drawable::Pipeline CUBE_PIPELINE_TEAL;
Scene::Drawable::Pipeline CUBE_PIPELINE_BLUE;
Scene::Drawable::Pipeline CUBE_PIPELINE_PURPLE;

Scene::Drawable *OCTRIS_BASE = nullptr;

static Scene::Drawable get_drawable(
        float offset_x, float offset_y, float offset_z,
        Scene::Transform &offset, Scene::Transform *parent,
        Scene::Drawable::Pipeline &pipeline) {
    offset.position.x = offset_x;
    offset.position.y = offset_y;
    offset.position.z = offset_z;
    offset.parent = parent;

    Scene::Drawable drawable(&offset);
    drawable.pipeline = pipeline;
    return drawable;
}

static OctrisPieceType get_random_type() {
    // TODO select random enum val
    return OctrisPieceType::CUBE;
}

OctrisPiece::OctrisPiece() : OctrisPiece(get_random_type()) {
}

OctrisPiece::OctrisPiece(OctrisPieceType type) {
    // Create a cube piece
    transform = Scene::Transform();
    transform.position = SPAWN_POS;

    switch (type) {
    case CUBE:
        blocks.emplace_back(get_drawable(-1.0f, -1.0f, -1.0f, block_transforms[0], &transform, CUBE_PIPELINE_YELLOW));
        blocks.emplace_back(get_drawable(-1.0f, -1.0f,  1.0f, block_transforms[1], &transform, CUBE_PIPELINE_YELLOW));
        blocks.emplace_back(get_drawable(-1.0f,  1.0f, -1.0f, block_transforms[2], &transform, CUBE_PIPELINE_YELLOW));
        blocks.emplace_back(get_drawable(-1.0f,  1.0f,  1.0f, block_transforms[3], &transform, CUBE_PIPELINE_YELLOW));
        blocks.emplace_back(get_drawable( 1.0f, -1.0f, -1.0f, block_transforms[4], &transform, CUBE_PIPELINE_YELLOW));
        blocks.emplace_back(get_drawable( 1.0f, -1.0f,  1.0f, block_transforms[5], &transform, CUBE_PIPELINE_YELLOW));
        blocks.emplace_back(get_drawable( 1.0f,  1.0f, -1.0f, block_transforms[6], &transform, CUBE_PIPELINE_YELLOW));
        blocks.emplace_back(get_drawable( 1.0f,  1.0f,  1.0f, block_transforms[7], &transform, CUBE_PIPELINE_YELLOW));
        break;
    default:
        assert(false);
    }
}

void OctrisPiece::translate(int x, int y, int z) {
    transform.position.x += (float)(x * BLOCK_LENGTH);
    transform.position.y += (float)(y * BLOCK_LENGTH);
    transform.position.z += (float)(z * BLOCK_LENGTH);
}

bool OctrisPiece::collides(const std::vector< OctrisPiece * > &pieces) {
    /**
     * This is ghetto collision. Since all the blocks should have integer coordinates and only move integer amounts,
     * this just checks that the coordinates of any two blocks aren't the same (w.r.t a <1 tolerance)
     */
    for (auto &block_transform : block_transforms) {
        glm::mat4x3 absolute = block_transform.make_local_to_world();
        float x = absolute[3][0];
        float y = absolute[3][1];
        float z = absolute[3][2];

        LOG("block pos: " << x << " " << y << " " << z);

        if (z < TOLERANCE) {
            LOG("floor collide");
            // Below floor
            return true;
        }

        for (auto piece : pieces) {
            // If this piece is too far away, skip
            if (std::abs(piece->transform.position.x - transform.position.x) > SKIP_CHECK_DIST
                    && std::abs(piece->transform.position.y - transform.position.z) > SKIP_CHECK_DIST
                    && std::abs(piece->transform.position.y - transform.position.z) > SKIP_CHECK_DIST) {
                LOG("skipping check against piece" << piece);
                continue;
            }

            LOG("checking against piece" << piece);

            for (auto &pblock_transform : piece->block_transforms) {
                glm::mat4x3 pabsolute = pblock_transform.make_local_to_world();
                if (std::abs(pabsolute[3][0] - x) < TOLERANCE
                        && std::abs(pabsolute[3][1] - y) < TOLERANCE
                        && std::abs(pabsolute[3][2] - z) < TOLERANCE) {
                    // Block collides with pblock
                    LOG("block collide");
                    return true;
                }
            }
        }
    }

    return false;
}

bool OctrisPiece::fall(const std::vector< OctrisPiece * > &pieces) {
    translate(0, 0, -1);

    if (collides(pieces)) {
        // undo fall
        translate(0, 0, 1);
        return false;
    }
    return true;
}