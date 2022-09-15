# Octris

Author: Henry Du

## Design:

Octris is a 3D variant of Tetris! Players familiar with Tetris will recognize many similar features and mechanics, but
with a 3D spin. Notably:
- Octris pieces can be move along the X, Y, and Z axis (but only down on Z of course). Rotation is supported in pitch, yaw, and roll.
- Octris pieces consist of 8 blocks (hence the name). The O, I, J, and S pieces are very reminiscent of Tetris (they've just been given depth). The L and Z piece however have been changed in interesting ways! The L piece is now a 3x3 plane with a corner taken out. The Z piece is two parallel 2x2 planes, stacked on top of each other with one corner touching. These changes are needed since 3-axis rotation now makes the original Tetris L identical to J and Z identical to S.
- The stage is now 20 tall, 10 wide, and 10 deep.
- When a piece can no longer move down, it is "cemented" into the world and a new piece is spawned.
- If a freshly spawned piece collides with the world, then the game is over!
- If any horizontal slice of the 20x10x10 stage is fully filled, that slice is clear and the player is rewarded with score. Clearing more slices at one time rewards more score.

## Screen Shot:

![Screen Shot](screenshot.png)

## How To Play:

The goal of Octris is to get a high a score as possible. As movement and rotation are much more complex, the game is slower than classic Tetris. In addition, a pause mechanic has been added as 3D camera movement can be difficult when under a time constraint.

Currently, pieces fall down 1 block every 3 seconds when the game is not paused. The game begins in a paused state and there is a 9 second grace period where no falling will occur. If a block collides when attempting to fall, it is cemeneted.

General controls:
1. Mouse to rotate the camera (either click captures the mouse, escape releases the mouse)
2. Arrow keys to move on the horizontal plane of the camera
3. P to toggle pause on the game

Octris piece translation controls:
1. W and S to move along the Y axis (W is positive)
2. D and A to move along the X axis (D is positive)
3. X to move down the Z axis
4. SPACEBAR to "slam" a piece all the way downward and cement it early

Octris piece rotation controls (w.r.t to Bird's eye view) (NUMPAD):
1. 9 and 7 controls clockwise and counter clockwise rotation about the Z axis (roll)
2. 8 and 5 controls rotation towards positive Y and negative Y about the X axis (pitch)
3. 6 and 4 controls rotation towards positive X and negative X about the Y axis (yaw)

For those without a numpad, there are alternate rotation controls:
1. O and U for roll
2. I and K for pitch
3. L and J for yaw

## This game was built with [NEST](NEST.md).
