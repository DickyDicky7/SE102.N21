#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <cstdint>
#include <cstddef>
#include <numbers>

namespace Constants
{
	// A linear HDR colour. Channel values above +1.0f are deliberate: the post
	// process bloom pass keys off the overbright range, so they are not clamped.
	struct Rgba
	{
		float r;
		float g;
		float b;
		float a;
	};

	// Engine & Loop Tuning
	namespace Engine
	{
		inline constexpr int    MAX_TICKS_PER_ITERATION              = +5;
		inline constexpr DWORD  INACTIVE_WINDOW_SLEEP_MILLISECONDS   = +16;
		// The fixed logic step.  Kept as a double because the frame loop's
		// accumulator is a double: rounding it to float first would make every
		// tick a couple of nanoseconds long, which the accumulator then sums.
		inline constexpr double TICKS_PER_SECOND                     = +60.0;
		inline constexpr double SECONDS_PER_TICK                     = +1.0 / TICKS_PER_SECOND;
		// Windows' DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2, which is a handle
		// value rather than an enumerator and so has no header constant.
		inline constexpr INT_PTR DPI_AWARENESS_PER_MONITOR_AWARE_V2  = -4;
	}

	// Screen, Window and Rendering Dimensions
	namespace Screen
	{
		inline constexpr UINT  WIDTH                                 = +640;
		inline constexpr UINT  HEIGHT                                = +600;
		inline constexpr float SCALING_X                             = +2.5f;
		inline constexpr float SCALING_Y                             = +2.5f;
		inline constexpr float IN_GAME_WIDTH                         = +256.0f; // WIDTH / SCALING_X
		inline constexpr float IN_GAME_HEIGHT                        = +240.0f; // HEIGHT / SCALING_Y
		inline constexpr float TILE_SIZE                             = +16.0f;
	}

	// Input Configuration
	namespace Input
	{
		inline constexpr DWORD  DIRECT_INPUT_VERSION                 = +0x0800;
		// DirectInput reports the keyboard as one byte per DIK_* scan code.
		inline constexpr size_t KEYBOARD_KEY_COUNT                   = +256;
	}

	// Graphics & Renderer Tuning
	namespace Graphics
	{
		inline constexpr float  TEXEL_INSET_EPSILON                  = +0.05f;
		inline constexpr float  PARTICLE_AIR_RESISTANCE              = +0.96f;
		inline constexpr float  FRAME_DELTA_TIME                     = static_cast<float>(Engine::SECONDS_PER_TICK);
		inline constexpr bool   DRAW_HITBOXES                        = false;
		inline constexpr int    DEFAULT_ANIMATION_DELAY_MILLISECONDS = +300;
		inline constexpr float  DEFAULT_CAMERA_ZOOM_STEP             = +0.1f;
		inline constexpr float  CAMERA_TRANSITION_STEP_FACTOR        = +2.0f;
		inline constexpr size_t PARTICLE_SYSTEM_RESERVE_CAPACITY     = +256;
		inline constexpr size_t PARTICLE_SYSTEM_MAX_CAPACITY         = +1024;
		inline constexpr float  PARTICLE_LASER_SEGMENT_STEP          = +2.0f;
		inline constexpr UINT   PARTICLE_TEXTURE_SIZE                = +64;
		inline constexpr float  PARTICLE_GLOW_FALLOFF_EXPONENT       = +4.0f;

		// Instances are streamed into the buffer as a ring, so it is sized for "a
		// lot of batches' worth" rather than "one batch's worth": the bigger it
		// is, the rarer a wrap - and a wrap is the only time the whole buffer is
		// renamed.
		inline constexpr UINT   INSTANCE_RING_CAPACITY               = +16384; // 1.5 MB
		// Largest number of instances one flush will submit. Bounded well below
		// the ring so that a full batch still has somewhere to go without wrapping.
		inline constexpr UINT   MAX_SPRITES_PER_BATCH                = +4096;
		inline constexpr UINT   MAX_DEBUG_VERTICES                   = +4096;

		// One BGRA texel is four bytes; a colour channel is one of those bytes.
		inline constexpr UINT   BYTES_PER_PIXEL                      = +4;
		inline constexpr float  COLOUR_CHANNEL_MAX                   = +255.0f;
		inline constexpr UINT8  COLOUR_CHANNEL_MAX_BYTE              = +255;

		// D3DCOLOR is packed ARGB, one byte per channel.
		inline constexpr UINT   COLOUR_SHIFT_ALPHA                   = +24;
		inline constexpr UINT   COLOUR_SHIFT_RED                     = +16;
		inline constexpr UINT   COLOUR_SHIFT_GREEN                   = +8;
		inline constexpr UINT   COLOUR_SHIFT_BLUE                    = +0;
		inline constexpr UINT   COLOUR_CHANNEL_MASK                  = +0xFF;

		// A 4x4 matrix, flattened.
		inline constexpr size_t MATRIX_ELEMENT_COUNT                 = +16;

		// The sprite quad is two triangles sharing an edge, so six indices, and
		// its wireframe is four edges, so eight line vertices.
		inline constexpr UINT   QUAD_INDEX_COUNT                     = +6;
		inline constexpr size_t DEBUG_VERTICES_PER_BOX               = +8;
		// The unit quad is bottom-centre anchored, so x spans [-1/2, +1/2].
		inline constexpr float  QUAD_HALF_WIDTH                      = +0.5f;

		// Upper bound on a compiled shader blob; anything larger is a bad file
		// rather than a shader worth reading into memory.
		inline constexpr long long MAX_SHADER_BYTECODE_BYTES         = 1LL << 22; // 4 MB
		// Diagnostics are formatted into fixed stack buffers.
		inline constexpr size_t REPORT_MESSAGE_BUFFER_LENGTH         = +512;
		inline constexpr size_t FATAL_MESSAGE_BUFFER_LENGTH          = +1024;

		// The sprite pass is orthographic, so the depth range only has to be wide
		// enough to hold every layer the game draws.
		inline constexpr float  ORTHOGRAPHIC_NEAR_PLANE              = -1.0f;
		inline constexpr float  ORTHOGRAPHIC_FAR_PLANE               = +1000.0f;
	}

	// Audio
	namespace Audio
	{
		// DirectSound volume is set as a percentage of the device's decibel range.
		inline constexpr float VOLUME_PERCENTAGE_MAX                 = +100.0f;
		inline constexpr float DEFAULT_VOLUME_PERCENTAGE             = +100.0f;
	}

	// Physics & Space Partitioning
	namespace Physics
	{
		inline constexpr float    DEFAULT_INITIAL_ACCELERATION_X       = +0.1f;
		inline constexpr float    DEFAULT_INITIAL_ACCELERATION_Y       = +0.1f;
		inline constexpr float    DEFAULT_INITIAL_VELOCITY_X           = +1.0f;
		inline constexpr float    DEFAULT_INITIAL_VELOCITY_Y           = +1.0f;
		inline constexpr float    GRAVITY_ACCELERATION                 = +9.8f;
		inline constexpr float    CANDIDATE_EXPAND_EPSILON             = +4.0f;
		// The quadtree stops subdividing at a node this many tiles across; only
		// QUADTREE_NODE_SMALLEST_* below are read, the factors are spelled out so
		// the tile grid the size derives from stays visible.
		inline constexpr float    QUADTREE_TILE_COLUMNS                = +8.0f;
		inline constexpr float    QUADTREE_TILE_ROWS                   = +8.0f;
		inline constexpr float    QUADTREE_TILE_WIDTH                  = +16.0f;
		inline constexpr float    QUADTREE_TILE_HEIGHT                 = +16.0f;
		inline constexpr float    QUADTREE_NODE_SMALLEST_WIDTH         = QUADTREE_TILE_WIDTH * QUADTREE_TILE_COLUMNS;
		inline constexpr float    QUADTREE_NODE_SMALLEST_HEIGHT        = QUADTREE_TILE_HEIGHT * QUADTREE_TILE_ROWS;
		inline constexpr size_t   SPATIAL_QUERY_RESERVE_CAPACITY       = +256;
		inline constexpr uint32_t BVH_MAX_LEAF_PRIMITIVES              = +4;
		// Depth bound for the iterative BVH walk.  BuildRecursive splits on the
		// centroid median, so depth stays near log2(primitives): 64 frames is far
		// past anything a level can produce, and a fixed array keeps Query free of
		// heap allocations.
		inline constexpr size_t   BVH_TRAVERSAL_STACK_CAPACITY         = +64;
		inline constexpr size_t   QUADTREE_PRINT_BUFFER_LENGTH         = +64;
		inline constexpr float    MORTON_COORD_MAX                     = +1023.0f;
		inline constexpr float    DEFAULT_MOTION_INTEGRATION_DELTA_TIME = +0.05f;
		// The 1/2 in the kinematic displacement equation s = ut + 1/2*a*t^2.
		inline constexpr float    KINEMATIC_DISPLACEMENT_COEFFICIENT   = +0.5f;
		// Below this magnitude a component is treated as zero, so that dividing by
		// it to recover a direction cannot blow up.
		inline constexpr float    DIRECTION_EPSILON                    = +0.0001f;
		// Swept collisions resolve a hair early, so a contact never resolves to
		// exactly touching (which the next sweep would read as already embedded).
		inline constexpr float    SWEEP_ENTRY_TIME_BACKOFF             = +0.1f;
		// One full turn in radians; angular frequency is TURN_RADIANS / period.
		inline constexpr float    TURN_RADIANS                         = +2.0f * std::numbers::pi_v<float>;
		// The same turn in degrees, for the angle bookkeeping done in degrees.
		inline constexpr float    FULL_CIRCLE_DEGREES                  = +360.0f;
		inline constexpr float    HALF_CIRCLE_DEGREES                  = +180.0f;
		inline constexpr float    QUARTER_CIRCLE_DEGREES               = +90.0f;
	}

	// Player (Bill) Mechanics
	namespace Bill
	{
		// Where the constructor parks Bill before a stage's "respawnposition"
		// object moves him; the same role as every other DEFAULT_SPAWN_* below.
		inline constexpr float     DEFAULT_SPAWN_X                           = +50.0f;
		inline constexpr float     DEFAULT_SPAWN_Y                           = +0.0f;
		inline constexpr float     MOVE_SPEED                                = +2.0f;
		inline constexpr float     DEAD_SPEED_X                              = +1.0f;
		inline constexpr float     DEAD_SPEED_Y                              = +2.5f;
		inline constexpr float     DEAD_ACCELERATION_Y                       = -0.1f;
		inline constexpr float     JUMP_SPEED_X                              = +2.0f;
		inline constexpr float     JUMP_SPEED_Y                              = +4.0f;
		inline constexpr float     JUMP_ACCELERATION_Y                       = -0.1f;
		inline constexpr float     FALL_SPEED_Y                              = -2.5f;
		inline constexpr float     FALL_ACCELERATION_Y                       = -0.1f;
		inline constexpr float     BULLET_SPEED_X                            = +3.0f;
		inline constexpr float     BULLET_SPEED_Y_UP                         = +3.0f;
		inline constexpr float     BULLET_SPEED_Y_DIAGONAL                   = +2.0f;
		inline constexpr float     SWIM_SPEED_X                              = +2.0f;
		inline constexpr float     SWIM_BUOYANCY_VELOCITY_Y                  = +1.0f;
		inline constexpr float     SWIM_BUOYANCY_ACCELERATION_Y              = +1.0f;
		inline constexpr int       SWIM_BEGIN_DELAY_FRAMES                   = +50;
		inline constexpr float     NORMAL_RESET_VELOCITY_Y                   = +1.0f;
		inline constexpr float     NORMAL_RESET_ACCELERATION_Y               = +1.0f;
		// A corpse that has already hit a boundary wall keeps drifting upward off
		// screen rather than resting on it.
		inline constexpr float     WALL_DEAD_DRIFT_VELOCITY_Y                = +1.0f;
		// Clipping a solid block side-on parks Bill on top of it and starts him
		// falling again, so he slides off instead of sticking to the wall.
		inline constexpr float     LEDGE_SLIDE_VELOCITY_Y                    = -1.0f;
		inline constexpr float     MAX_STEP_HEIGHT                           = +48.0f; // 3 tiles (16px each)
		inline constexpr ULONGLONG DEFAULT_FIRING_RATE_MILLISECONDS          = +200;
		inline constexpr int       IMMORTAL_DURATION_FRAMES                  = +200;
		inline constexpr ULONGLONG DEFAULT_REVIVAL_COOLDOWN_MILLISECONDS     = +100;
		inline constexpr int       ANIMATION_RUN_DELAY_MILLISECONDS          = +100;
		inline constexpr int       ANIMATION_JUMP_DELAY_MILLISECONDS         = +60;
		inline constexpr int       ANIMATION_DEAD_DELAY_MILLISECONDS         = +300;
		inline constexpr int       ANIMATION_DEFAULT_DELAY_MILLISECONDS      = +100;
		inline constexpr int       DEAD_FINAL_FRAME                          = +3;
		inline constexpr float     GUN_SPAWN_OFFSET_RATIO_JUMP_Y             = +0.5f;
		inline constexpr float     GUN_SPAWN_OFFSET_RATIO_LAYDOWN_Y          = +0.4f;
		inline constexpr float     GUN_SPAWN_OFFSET_RATIO_RUN_ANGLE_DOWN_Y   = +0.4f;
		inline constexpr float     GUN_SPAWN_OFFSET_RATIO_NORMAL_SHOT_Y      = +0.6f;
		inline constexpr float     GUN_SPAWN_OFFSET_RATIO_RUN_SHOT_Y         = +0.6f;
		inline constexpr float     GUN_SPAWN_OFFSET_RATIO_STRAIGHT_UP_X      = +0.6f;
		inline constexpr float     GUN_SPAWN_OFFSET_RATIO_RUN_ANGLE_UP_Y     = +0.9f;
		inline constexpr float     GUN_SPAWN_OFFSET_RATIO_SWIM_ANGLE_UP_Y    = +0.9f;
		inline constexpr float     GUN_SPAWN_OFFSET_RATIO_SWIM_NORMAL_SHOT_Y = +0.2f;
		inline constexpr float     GUN_SPAWN_OFFSET_RATIO_SWIM_RUN_SHOT_Y    = +0.2f;
	}

	// Weapon Mechanics
	namespace Weapons
	{
		inline constexpr ULONGLONG DEFAULT_FIRING_RATE_MILLISECONDS         = +150;
		inline constexpr float SPREAD_DEGREE_10                             = +10.0f;
		inline constexpr float SPREAD_DEGREE_20                             = +20.0f;
		inline constexpr float BULLET_FIRE_SPIRAL_RADIUS                    = +20.0f;
		inline constexpr float BULLET_FIRE_ANGULAR_VELOCITY_DELTA           = +20.0f;
		inline constexpr float BULLET_EXPLODE_DURATION_FRAMES               = +20.0f;
		inline constexpr int   ANIMATION_DEFAULT_DELAY_MILLISECONDS         = +150;
		inline constexpr int   ANIMATION_SPREAD_AMMO_DELAY_MILLISECONDS     = +400;
		inline constexpr float SCUBA_BULLET_MOTION_DELTA_TIME               = +0.05f;
		inline constexpr float SCUBA_BULLET_PROJECTILE_DELTA_TIME           = +0.01f;
		inline constexpr float SCUBA_BULLET_EXPLOSION_OFFSET_Y              = +5.0f;
		inline constexpr float BULLET_EXPLOSION_OFFSET                      = +3.0f;
		inline constexpr float BULLET_F_SPIRAL_ANGLE_45                     = +45.0f;
		inline constexpr float BULLET_F_SPIRAL_ANGLE_90                     = +90.0f;
		inline constexpr float BULLET_F_SPIRAL_ANGLE_135                    = +135.0f;
		inline constexpr float BULLET_F_SPIRAL_ANGLE_180                    = +180.0f;
		inline constexpr float BULLET_F_OFFSET_X                            = +5.0f;
		inline constexpr float BULLET_F_OFFSET_Y                            = +10.0f;
		inline constexpr float BULLET_L_OFFSET_X                            = +2.0f;
		inline constexpr float BULLET_L_OFFSET_Y_UP                         = +3.0f;
		inline constexpr float BULLET_L_OFFSET_Y_DOWN                       = +2.0f;
		inline constexpr float BULLET_L_ANGLE_45                            = +45.0f;
		inline constexpr float BULLET_L_ANGLE_90                            = +90.0f;
		inline constexpr float BULLET_L_ANGLE_135                           = +135.0f;
		// The boss-stage-1 shot is fired as a projectile straight down-range; its
		// launch angle is clamped so a steep aim cannot turn it into a lob.
		inline constexpr float BULLET_BOSS_MAX_LAUNCH_ANGLE                 = +6.0f;
		inline constexpr float BULLET_BOSS_PROJECTILE_ANGLE_DEGREES         = +180.0f;
		inline constexpr float BULLET_BOSS_PROJECTILE_DELTA_TIME            = +0.01f;
		inline constexpr float BULLET_BOSS_EXPLOSION_OFFSET_Y               = +5.0f;
	}

	// Bullet Particle Effects
	// Trail sparks stream behind a travelling bullet, the explosion burst fires
	// once on impact, and the glow is the additive sprite drawn on the bullet.
	namespace Particles
	{
		inline constexpr float ALPHA_OPAQUE                                 = +1.0f;

		// Shared RNG shaping: rand() % MODULO, recentred by HALF for a signed spread.
		inline constexpr int   RANDOM_PERCENT_MODULO                        = +100;
		inline constexpr int   RANDOM_PERCENT_HALF                          = +50;

		// Trail sparks inherit a reversed fraction of the bullet's velocity.
		inline constexpr float TRAIL_DRAG_RATIO                             = +0.10f;
		inline constexpr float TRAIL_DRAG_RATIO_M                           = +0.15f;
		inline constexpr float TRAIL_DRAG_RATIO_S                           = +0.20f;

		inline constexpr float TRAIL_SIZE_R                                 = +5.0f;
		inline constexpr float TRAIL_LIFE_R                                 = +0.15f;
		inline constexpr Rgba  TRAIL_COLOUR_R                               = { +3.0f, +1.2f, +0.2f, ALPHA_OPAQUE };
		inline constexpr float TRAIL_R_JITTER_DIVISOR                       = +100.0f;
		inline constexpr float TRAIL_R_JITTER_SCALE                         = +0.4f;

		inline constexpr float TRAIL_SIZE_M                                 = +5.5f;
		inline constexpr float TRAIL_LIFE_M                                 = +0.12f;
		inline constexpr Rgba  TRAIL_COLOUR_M                               = { +0.2f, +2.0f, +3.5f, ALPHA_OPAQUE };

		inline constexpr float TRAIL_SIZE_S                                 = +7.0f;
		inline constexpr float TRAIL_LIFE_S                                 = +0.18f;
		inline constexpr Rgba  TRAIL_COLOUR_S                               = { +3.0f, +0.2f, +2.0f, ALPHA_OPAQUE };

		inline constexpr float TRAIL_SIZE_L                                 = +6.0f;
		inline constexpr float TRAIL_LIFE_L                                 = +0.08f;
		inline constexpr Rgba  TRAIL_COLOUR_L                               = { +0.2f, +1.5f, +4.0f, ALPHA_OPAQUE };
		// The laser trail is sampled along the beam, one spark per 1/N of a step.
		inline constexpr int   TRAIL_L_SEGMENT_COUNT                        = +3;
		inline constexpr float TRAIL_L_SEGMENT_DIVISOR                      = static_cast<float>(TRAIL_L_SEGMENT_COUNT);

		inline constexpr float TRAIL_SIZE_F_CORE                            = +9.0f;
		inline constexpr float TRAIL_LIFE_F_CORE                            = +0.25f;
		inline constexpr Rgba  TRAIL_COLOUR_F_CORE                          = { +4.0f, +0.8f, +0.0f, ALPHA_OPAQUE };
		inline constexpr float TRAIL_SIZE_F_SPARK                           = +6.0f;
		inline constexpr float TRAIL_LIFE_F_SPARK                           = +0.15f;
		inline constexpr Rgba  TRAIL_COLOUR_F_SPARK                         = { +3.0f, +1.2f, +0.0f, ALPHA_OPAQUE };
		inline constexpr float TRAIL_F_JITTER_DIVISOR                       = +150.0f;
		inline constexpr int   TRAIL_F_POSITION_JITTER_MODULO               = +10;
		inline constexpr int   TRAIL_F_POSITION_JITTER_HALF                 = +5;

		inline constexpr float TRAIL_SIZE_ENEMY                             = +5.0f;
		inline constexpr float TRAIL_LIFE_ENEMY                             = +0.15f;
		inline constexpr Rgba  TRAIL_COLOUR_ENEMY                           = { +3.0f, +0.5f, +0.1f, ALPHA_OPAQUE };

		inline constexpr float TRAIL_SIZE_BOSS                              = +6.0f;
		inline constexpr float TRAIL_LIFE_BOSS                              = +0.18f;
		inline constexpr Rgba  TRAIL_COLOUR_BOSS                            = { +1.5f, +0.1f, +3.0f, ALPHA_OPAQUE };

		// Explosion burst: a ring of sparks with per-particle jitter.
		inline constexpr int   EXPLODE_COUNT_BASE                           = +10;
		inline constexpr int   EXPLODE_COUNT_JITTER_MODULO                  = +5;
		inline constexpr float EXPLODE_ANGLE_JITTER_DIVISOR                 = +500.0f;
		inline constexpr float EXPLODE_SPEED_BASE                           = +0.8f;
		inline constexpr float EXPLODE_SPEED_JITTER_DIVISOR                 = +80.0f;
		inline constexpr float EXPLODE_LIFE_BASE                            = +0.15f;
		inline constexpr float EXPLODE_LIFE_JITTER_DIVISOR                  = +1000.0f;
		inline constexpr float EXPLODE_SIZE_BASE                            = +4.0f;
		inline constexpr int   EXPLODE_SIZE_JITTER_MODULO                   = +4;

		inline constexpr Rgba  EXPLODE_COLOUR_DEFAULT                       = { +3.0f, +1.0f, +0.2f, ALPHA_OPAQUE };
		inline constexpr Rgba  EXPLODE_COLOUR_F                             = { +4.0f, +0.8f, +0.0f, ALPHA_OPAQUE };
		inline constexpr Rgba  EXPLODE_COLOUR_L                             = { +0.2f, +1.5f, +4.0f, ALPHA_OPAQUE };
		inline constexpr Rgba  EXPLODE_COLOUR_M                             = { +0.2f, +2.0f, +3.5f, ALPHA_OPAQUE };
		inline constexpr Rgba  EXPLODE_COLOUR_S                             = { +3.0f, +0.2f, +2.0f, ALPHA_OPAQUE };
		inline constexpr Rgba  EXPLODE_COLOUR_BOSS                          = { +1.5f, +0.1f, +3.0f, ALPHA_OPAQUE };

		// Additive glow drawn on the bullet itself.
		inline constexpr Rgba  GLOW_COLOUR_DEFAULT                          = { +4.0f, +4.0f, +4.0f, ALPHA_OPAQUE };
		inline constexpr float GLOW_SIZE_DEFAULT                            = +6.0f;
		inline constexpr Rgba  GLOW_COLOUR_R                                = { +4.0f, +3.0f, +1.0f, ALPHA_OPAQUE };
		inline constexpr float GLOW_SIZE_R                                  = +6.0f;
		inline constexpr Rgba  GLOW_COLOUR_M                                = { +1.0f, +4.0f, +4.0f, ALPHA_OPAQUE };
		inline constexpr float GLOW_SIZE_M                                  = +7.0f;
		inline constexpr Rgba  GLOW_COLOUR_S                                = { +4.0f, +1.0f, +4.0f, ALPHA_OPAQUE };
		inline constexpr float GLOW_SIZE_S                                  = +8.0f;
		inline constexpr Rgba  GLOW_COLOUR_L                                = { +1.5f, +3.5f, +5.0f, ALPHA_OPAQUE };
		inline constexpr float GLOW_SIZE_L                                  = +7.0f;
		inline constexpr Rgba  GLOW_COLOUR_F                                = { +5.0f, +2.5f, +0.5f, ALPHA_OPAQUE };
		inline constexpr float GLOW_SIZE_F                                  = +9.0f;
		inline constexpr Rgba  GLOW_COLOUR_ENEMY                            = { +4.0f, +2.0f, +0.5f, ALPHA_OPAQUE };
		inline constexpr float GLOW_SIZE_ENEMY                              = +6.0f;
		inline constexpr Rgba  GLOW_COLOUR_BOSS                             = { +3.0f, +1.0f, +4.0f, ALPHA_OPAQUE };
		inline constexpr float GLOW_SIZE_BOSS                               = +8.0f;
	}

	// Interactive Objects
	namespace Objects
	{
		namespace Item
		{
			inline constexpr float POP_INITIAL_VELOCITY                     = +7.0f;
			inline constexpr float POP_LAUNCH_ANGLE_DEGREES                 = +80.0f;
			inline constexpr float POP_DELTA_TIME                           = +0.03f;
			inline constexpr int   ANIMATION_DELAY_MILLISECONDS             = +150;
		}

		namespace Fire
		{
			inline constexpr float MOVE_DISTANCE                            = +110.0f;
			inline constexpr float INITIAL_SPEED_X                          = +1.0f;
			inline constexpr float INITIAL_SPEED_Y                          = +1.0f;
			inline constexpr int   ANIMATION_DELAY_MILLISECONDS             = +150;
		}

		namespace RockFly
		{
			inline constexpr float HITBOX_HEIGHT_OFFSET                     = +7.0f;
			inline constexpr int   ANIMATION_DELAY_MILLISECONDS             = +150;
		}

		namespace Explosion
		{
			// Where the constructors park an explosion before its spawner moves
			// it onto the entity that died.
			inline constexpr float DEFAULT_SPAWN_X                          = +200.0f;
			inline constexpr float DEFAULT_SPAWN_Y                          = +200.0f;
			inline constexpr int DROWN_DURATION_FRAMES                      = +100;
			inline constexpr int ANIMATION_DELAY_MILLISECONDS               = +100;
		}

		namespace Letter
		{
			inline constexpr float  WIDTH                                   = +8.0f;
			inline constexpr float  HEIGHT                                  = +8.0f;
			inline constexpr size_t LETTER_SPRITE_ID_RESERVE_CAPACITY       = +14;
			inline constexpr float  SPACING_RATIO                           = +0.5f;
		}
	}

	// Enemy Behavior, Timers & Dimensions
	namespace Enemies
	{
		namespace AirCraft
		{
			// Where the constructor parks the entity before the stage loader places it.
			inline constexpr float DEFAULT_SPAWN_X                          = +100.0f;
			inline constexpr float DEFAULT_SPAWN_Y                          = +200.0f;
			inline constexpr float OSCILLATION_DELTA_TIME                   = +0.03f;
			inline constexpr float OSCILLATION_PERIOD                       = +2.5f;
			inline constexpr float OSCILLATION_RADIUS                       = +35.0f;
			inline constexpr float INITIAL_SPEED_Y                          = +0.5f;
			inline constexpr int   HEALTH_POINTS                            = +1;
			inline constexpr int   ANIMATION_DELAY_MILLISECONDS             = +150;
		}

		namespace Cannon
		{
			// Where the constructor parks the entity before the stage loader places it.
			inline constexpr float DEFAULT_SPAWN_X                          = +50.0f;
			inline constexpr float DEFAULT_SPAWN_Y                          = +0.0f;
			inline constexpr int   SHOOT_DELAY_FRAMES                       = +100;
			inline constexpr int   SHOOT_DELAY_PER_BULLET_FRAMES            = +20;
			inline constexpr int   SHOOT_BURST_COUNT                        = +3;
			inline constexpr int   HEALTH_POINTS                            = +8;
			inline constexpr int   ANIMATION_APPEAR_DELAY_MILLISECONDS      = +50;
			inline constexpr int   ANIMATION_DEFAULT_DELAY_MILLISECONDS     = +165;
			inline constexpr float AIM_ANGLE_UP_30_DEGREES                  = +30.0f;
			inline constexpr float AIM_ANGLE_UP_60_DEGREES                  = +60.0f;
			inline constexpr float AIM_ANGLE_THRESHOLD_45_DEGREES           = +45.0f;
			inline constexpr float AIM_ANGLE_THRESHOLD_75_DEGREES           = +75.0f;
		}

		namespace RifleMan
		{
			// The standing variant's placeholder spawn; the bush variant is
			// parked by HIDE_ON_BUSH_SPAWN_* below.
			inline constexpr float STANDING_DEFAULT_SPAWN_X                 = +300.0f;
			inline constexpr float STANDING_DEFAULT_SPAWN_Y                 = +200.0f;
			inline constexpr int   SHOOT_DELAY_FRAMES                       = +120;
			inline constexpr int   SHOOT_DELAY_PER_BULLET_FRAMES            = +20;
			inline constexpr int   SHOOT_BURST_COUNT                        = +3;
			inline constexpr float HIDING_INTERVAL_MILLISECONDS             = +2000.0f;
			inline constexpr float STANDING_SHOOT_DELAY_MILLISECONDS        = +1500.0f;
			inline constexpr float STANDING_INTERVAL_MILLISECONDS           = +3000.0f;
			inline constexpr int   HEALTH_POINTS                            = +1;
			inline constexpr int   ANIMATION_HIDE_ON_BUSH_DELAY_MILLISECONDS = +150;
			inline constexpr int   ANIMATION_STANDING_DELAY_MILLISECONDS    = +165;
			inline constexpr float SHOOTING_ANGLE_THRESHOLD_DEGREES         = +65.0f;
			inline constexpr float STRAIGHT_UP_ANGLE_DEGREES                = +90.0f;
			inline constexpr float AIM_DOWN_GUN_OFFSET_RATIO_Y              = +0.375f;
			inline constexpr float NORMAL_GUN_OFFSET_RATIO_Y                = +0.70f;
			inline constexpr float HIDE_ON_BUSH_BULLET_OFFSET_Y             = +10.0f;
			// Where a bush rifleman is parked before a stage moves it into place.
			inline constexpr float HIDE_ON_BUSH_SPAWN_DIVISOR_X             = +3.0f;
			inline constexpr float HIDE_ON_BUSH_SPAWN_DIVISOR_Y             = +2.0f;
			inline constexpr float HIDE_ON_BUSH_SPAWN_OFFSET_Y              = +200.0f;
		}

		namespace Falcon
		{
			// Where the constructor parks the entity before the stage loader places it.
			inline constexpr float DEFAULT_SPAWN_X                          = +300.0f;
			inline constexpr float DEFAULT_SPAWN_Y                          = +100.0f;
			inline constexpr float DETECT_DISTANCE                          = +130.0f;
			inline constexpr int   OPENING_DELAY_FRAMES                     = +30;
			inline constexpr int   HEALTH_POINTS                            = +5;
			inline constexpr int   ANIMATION_DELAY_MILLISECONDS             = +150;
		}

		namespace WallTurret
		{
			// Where the constructor parks the entity before the stage loader places it.
			inline constexpr float DEFAULT_SPAWN_X                          = +50.0f;
			inline constexpr float DEFAULT_SPAWN_Y                          = +50.0f;
			inline constexpr int   SHOOT_DELAY_FRAMES                       = +80;
			inline constexpr float TARGET_RANGE_TILES                       = +6.0f;
			inline constexpr int   STATE_CHANGE_DELAY_FRAMES                = +20;
			inline constexpr float WIDTH                                    = +32.0f;
			inline constexpr float HEIGHT                                   = +32.0f;
			inline constexpr int   HEALTH_POINTS                            = +10;
			inline constexpr int   ANIMATION_DELAY_MILLISECONDS             = +150;
			inline constexpr float AIM_ANGLE_15_DEGREES                     = +15.0f;
			inline constexpr float AIM_ANGLE_30_DEGREES                     = +30.0f;
			inline constexpr float AIM_ANGLE_45_DEGREES                     = +45.0f;
			inline constexpr float AIM_ANGLE_60_DEGREES                     = +60.0f;
			inline constexpr float AIM_ANGLE_75_DEGREES                     = +75.0f;
			inline constexpr float AIM_ANGLE_90_DEGREES                     = +90.0f;
			inline constexpr float AIM_ANGLE_105_DEGREES                    = +105.0f;
			inline constexpr float AIM_ANGLE_120_DEGREES                    = +120.0f;
			inline constexpr float AIM_ANGLE_135_DEGREES                    = +135.0f;
			inline constexpr float AIM_ANGLE_150_DEGREES                    = +150.0f;
			inline constexpr float AIM_ANGLE_165_DEGREES                    = +165.0f;
		}

		namespace ScubaSoldier
		{
			inline constexpr float     DEFAULT_SPAWN_X                      = +300.0f;
			inline constexpr float     DEFAULT_SPAWN_Y                      = +0.0f;
			inline constexpr ULONGLONG FIRING_RATE_MILLISECONDS             = +2000;
			inline constexpr float     DETECT_ANGLE_MIN_DEGREES             = -110.0f;
			inline constexpr float     DETECT_ANGLE_MAX_DEGREES             = +110.0f;
			inline constexpr float     BULLET_SPEED_Y                       = +6.0f;
			inline constexpr float     BULLET_ACCELERATION_Y                = -0.1f;
			inline constexpr int       HEALTH_POINTS                        = +1;
			inline constexpr int       ANIMATION_DELAY_MILLISECONDS         = +150;
			inline constexpr float     INITIAL_BILL_ANGLE_DEGREES           = -90.0f;
			// The volley is four bullets: a straight-up pair plus a fanned pair.
			inline constexpr float     SPREAD_ANGLE_STRAIGHT_DEGREES        = +90.0f;
			inline constexpr float     SPREAD_ANGLE_FANNED_DEGREES          = +45.0f;
			inline constexpr float     SPREAD_STRAIGHT_DRIFT_X              = +0.0f;
			inline constexpr float     SPREAD_FANNED_DRIFT_X                = +2.0f;
			inline constexpr float     SPREAD_LEAD_BULLET_SPEED_Y_OFFSET    = +1.0f;
		}

		namespace Soldier
		{
			inline constexpr float     DEFAULT_SPAWN_X                      = +100.0f;
			inline constexpr float     DEFAULT_SPAWN_Y                      = +0.0f;
			inline constexpr ULONGLONG FIRING_RATE_MILLISECONDS             = +2000;
			// One threshold, read from both sides: beyond it the soldier closes and
			// shoots standing, within it he drops prone.  Two names could drift and
			// silently open a gap (or an overlap) between the two branches.
			inline constexpr float     DETECT_DISTANCE                      = +64.0f;
			inline constexpr float     MAX_STEP_HEIGHT                      = +48.0f; // 3 tiles (16px each)
			inline constexpr float     RUN_SPEED                            = +1.5f;
			inline constexpr float     RUN_ACCELERATION                     = +0.01f;
			inline constexpr float     JUMP_SPEED_X                         = +1.0f;
			inline constexpr float     JUMP_SPEED_Y                         = +2.0f;
			inline constexpr float     JUMP_ACCELERATION_Y                  = -0.1f;
			inline constexpr float     DIE_SPEED_X                          = +1.0f;
			inline constexpr float     DIE_SPEED_Y                          = +2.0f;
			inline constexpr float     DIE_ACCELERATION_Y                   = -0.1f;
			inline constexpr float     DIE_DURATION_SECONDS                 = +1.5f;
			inline constexpr float     MOTION_INTEGRATION_DELTA_TIME        = +0.05f;
			inline constexpr int       HEALTH_POINTS                        = +1;
			inline constexpr int       ANIMATION_RUN_DELAY_MILLISECONDS     = +70;
			inline constexpr int       ANIMATION_JUMP_DELAY_MILLISECONDS    = +60;
			inline constexpr int       ANIMATION_DEFAULT_DELAY_MILLISECONDS = +150;
			inline constexpr float     GUN_MOUNT_OFFSET_RATIO_STANDING      = +0.75f;
			inline constexpr float     GUN_MOUNT_OFFSET_RATIO_PRONE         = +0.20f;
			inline constexpr float     SHOOT_ANGLE_TOLERANCE_MIN_DEGREES    = +85.0f;
			inline constexpr float     SHOOT_ANGLE_TOLERANCE_MAX_DEGREES    = +95.0f;
			// A soldier that is not running holds station: the shooting and
			// lay-down poses both park the two axes at these values.
			inline constexpr float     PARK_VELOCITY                        = +1.0f;
			inline constexpr float     PARK_ACCELERATION                    = +1.0f;
		}

		namespace Bridge
		{
			// Where the constructor parks the entity before the stage loader places it.
			inline constexpr float DEFAULT_SPAWN_X                          = +50.0f;
			inline constexpr float DEFAULT_SPAWN_Y                          = +0.0f;
			inline constexpr int   PART_COUNT                               = +4;
			inline constexpr int   ANIMATION_DEFAULT_DELAY_MILLISECONDS     = +165;
			inline constexpr int   ANIMATION_EXPLOSION_DELAY_MILLISECONDS   = +50;
			inline constexpr float ENEMY_TOUCH_TRIGGER_OFFSET_RATIO         = +0.25f;
			// The bridge hitbox is accumulated from its parts rather than measured.
			inline constexpr float HITBOX_WIDTH_RATIO_PER_PART              = +1.25f;
			inline constexpr float HITBOX_HEIGHT_RATIO                      = +0.75f;
		}

		namespace RockFall
		{
			// Where the constructor parks the entity before the stage loader places it.
			inline constexpr float DEFAULT_SPAWN_X                          = +0.0f;
			inline constexpr float DEFAULT_SPAWN_Y                          = +0.0f;
			inline constexpr float DELAY_TO_FALL_FRAMES                     = +70.0f;
			inline constexpr float BOUNCE_SPEED_Y                           = -1.0f;
			inline constexpr float BOUNCE_REBOUND_VELOCITY_Y                = +1.5f;
			inline constexpr float BOUNCE_REBOUND_ACCELERATION_Y            = -0.1f;
			inline constexpr int   SHAKE_FRAME_MODULO                       = +5;
			inline constexpr float SHAKE_DISPLACEMENT_FACTOR                = +2.0f;
			inline constexpr float MOTION_INTEGRATION_DELTA_TIME            = +0.05f;
			inline constexpr int   HEALTH_POINTS                            = +8;
			inline constexpr int   ANIMATION_DELAY_MILLISECONDS             = +150;
		}

		namespace BossStage1
		{
			// Where the constructors park the boss and its guns before the stage
			// loader places them.
			inline constexpr float DEFAULT_SPAWN_X                          = +50.0f;
			inline constexpr float DEFAULT_SPAWN_Y                          = +0.0f;
			inline constexpr int   ANIMATION_DELAY_MILLISECONDS             = +165;
			inline constexpr float FINAL_BOSS_EXPLOSION_OFFSET_RATIO_X      = +0.25f;
			inline constexpr float FINAL_BOSS_EXPLOSION_OFFSET_RATIO_Y      = +0.25f;
			inline constexpr float FINAL_BOSS_EXPLOSION_SPACING_RATIO_X     = +0.75f;
			inline constexpr float FINAL_BOSS_EXPLOSION_SPACING_RATIO_Y     = +0.75f;

			namespace Gun
			{
				inline constexpr int       HEALTH_POINTS                    = +8;
				inline constexpr ULONGLONG NORMAL_STEP_INTERVAL_MILLISECONDS  = +400;
				inline constexpr ULONGLONG PREPARE_SHOOT_INTERVAL_MILLISECONDS = +500;
				// The muzzle sits fractionally in from the gun's leading edge.
				inline constexpr float     MUZZLE_OFFSET_RATIO_X            = +0.8f;
				// Bill's horizontal distance is spread over this many pixels of
				// travel to give the shot its speed.
				inline constexpr float     SHOT_SPEED_DISTANCE_DIVISOR      = +32.0f;
				inline constexpr float     STEP_DISTANCE_X                  = +1.0f;
				// The two gun variants settle into slightly different death poses.
				inline constexpr int       TYPE_UPPER                       = +1;
				inline constexpr int       TYPE_LOWER                       = +2;
				inline constexpr float     DESTROY_OFFSET_X_UPPER           = +7.0f;
				inline constexpr float     DESTROY_OFFSET_X_LOWER           = +4.0f;
				inline constexpr float     DESTROY_OFFSET_Y_UPPER           = +1.0f;
				inline constexpr float     DESTROY_OFFSET_Y_LOWER           = +0.0f;
			}

			namespace FinalBoss
			{
				inline constexpr int   HEALTH_POINTS                        = +32;
				inline constexpr int   DEAD_TURN_COUNT                      = +5;
				inline constexpr int   SUB_EXPLOSIONS_TOTAL_COUNT           = +11;
				inline constexpr float DESTROY_POSITION_OFFSET_X            = +37.4f;
				inline constexpr float DESTROY_POSITION_OFFSET_Y            = -8.0f;
			}
		}

		namespace BossStage3
		{
			inline constexpr int   ANIMATION_DELAY_MILLISECONDS             = +150;
			// Where the constructors park the head, the gate and each hand
			// before the stage loader places them.
			inline constexpr float DEFAULT_SPAWN_X                          = +200.0f;
			inline constexpr float DEFAULT_SPAWN_Y                          = +200.0f;

			namespace Head
			{
				inline constexpr int   HEALTH_POINTS                        = +50;
				inline constexpr int   OPEN_DURATION_FRAMES                 = +100;
				inline constexpr int   MIDDLE_STATE_DELAY_FRAMES            = +20;
				inline constexpr float EXPLOSION_GRID_OFFSET_RATIO          = +0.25f;
				inline constexpr int   DEATH_EFFECT_GRID_MIN_COL            = -2;
				inline constexpr int   DEATH_EFFECT_GRID_MAX_COL            = +4;
				inline constexpr int   DEATH_EFFECT_VERTICAL_MIN_COL        = +0;
				inline constexpr int   DEATH_EFFECT_VERTICAL_MAX_COL        = +2;
				inline constexpr int   DEATH_EFFECT_VERTICAL_DEPTH          = +10;
			}

			namespace Hand
			{
				inline constexpr int       HEALTH_POINTS                    = +30;
				inline constexpr ULONGLONG FIRING_RATE_MILLISECONDS         = +500;
				inline constexpr size_t    TOTAL_JOINTS_COUNT               = +5;
				// Index 0 is the shoulder anchor and the last index is the hand,
				// so everything before it is an arm segment.
				inline constexpr size_t    ARM_JOINTS_COUNT                 = TOTAL_JOINTS_COUNT - 1;
				inline constexpr size_t    HAND_JOINT_INDEX                 = TOTAL_JOINTS_COUNT - 1;
				inline constexpr float     JOINT_WIDTH                      = +15.0f;
				inline constexpr float     JOINT_HEIGHT                     = +15.0f;
				inline constexpr float     ATTACK_SPEED                     = +5.0f;
				inline constexpr int       ATTACK_FRAME_DURATION            = +10;
				inline constexpr int       ATTACK_STATE_DELAY_FRAMES        = +40;
				inline constexpr int       DIRECT_PLAYER_ATTACK_TIME_FRAMES = +60;
				inline constexpr int       DIRECT_PLAYER_DELAY_FRAMES       = +280;
				inline constexpr float     SPIN_SPEED_ANGLE                 = +5.7f;
				inline constexpr float     SPIN_TOTAL_FRAME_DIVIDEND        = +400.0f;
				inline constexpr int       SPIN_DELAY_CHANGE_STATE_FRAMES   = +6;
				inline constexpr int       START_STATE_DELAY_FRAMES         = +80;
				inline constexpr float     START_DIRECTION_ANGLE_LEFT_DEGREES  = +110.0f;
				inline constexpr float     START_DIRECTION_ANGLE_RIGHT_DEGREES = +70.0f;
				inline constexpr float     START_DISTANCE_TILES                = +4.0f;
				inline constexpr float     WAVE_SPEED                       = +5.0f;
				inline constexpr int       WAVE_MAX_CYCLES                  = +6;
				inline constexpr float     WAVE_REVERSE_ANGLE_LEFT_POS_DEGREES  = -60.0f;
				inline constexpr float     WAVE_REVERSE_ANGLE_RIGHT_POS_DEGREES = +100.0f;
				inline constexpr float     WAVE_REVERSE_ANGLE_LEFT_NEG_DEGREES  = +80.0f;
				inline constexpr float     WAVE_REVERSE_ANGLE_RIGHT_NEG_DEGREES = -120.0f;
				inline constexpr float     ATTACK_LIMIT_FRAME_DIVIDEND      = +100.0f;
				inline constexpr float     ATTACK_FIRE_ANGLE_OFFSET         = -0.5f;
				inline constexpr float     JOINT_2_SPEED_DECAY              = +0.99f;
				inline constexpr float     JOINT_3_SPEED_BOOST              = +1.10f;
				inline constexpr float     JOINT_4_SPEED_DECAY              = +0.99f;
				inline constexpr float     DIRECT_MOVE_ANGLE_FACTOR         = +15.0f;
				inline constexpr float     DIRECT_PLAYER_ANGLE_TOLERANCE_DEGREES = +5.0f;
				inline constexpr float     JOINT_MOVE_AROUND_RADIUS         = +16.0f;
				inline constexpr float     JOINT_MOVE_AROUND_SPEED_FACTOR   = +12.5f;
				inline constexpr int       JOINT_MOVE_AROUND_FRAMES         = +5;
				// A negative frame count means "keep going until told to stop".
				inline constexpr int       JOINT_FOLLOW_FRAMES_UNBOUNDED    = -1;
				// A trailing joint eases onto the one it follows: gently on the
				// first hand-off, harder on every one after it.
				inline constexpr float     JOINT_FOLLOW_SPEED_RATIO_FIRST   = +0.2f;
				inline constexpr float     JOINT_FOLLOW_SPEED_RATIO_REPEAT  = +0.7f;
				inline constexpr float     JOINT_FOLLOW_ACCELERATOR_DIVISOR = +11.0f;
				inline constexpr float     JOINT_FOLLOW_FRAME_SPEED_FACTOR  = +2.5f;
			}
		}
	}

	// Scene & UI Dimensions
	namespace Scene
	{
		inline constexpr float  BLACK_OVERLAY_WIDTH                         = +256.0f;
		inline constexpr float  BLACK_OVERLAY_HEIGHT                        = +240.0f;
		inline constexpr float  START_SCENE_WIDTH                           = +256.0f;
		inline constexpr float  START_SCENE_HEIGHT                          = +224.0f;
		inline constexpr float  START_SCENE_SPEED_X                         = -0.5f;
		inline constexpr int    START_SCENE_TRANSITION_TURNS                = +300;
		inline constexpr int    LOADING_SCENE_TRANSITION_TURNS              = +300;
		// Scene backdrops start and stop one and a half screens out, so the wipe
		// is already at full speed by the time any of it is visible.
		inline constexpr float  SCENE_OFFSCREEN_WIDTH_FACTOR                = +1.5f;
		// Where a backdrop rests once it has finished sliding in: centred, i.e.
		// half its own width across.
		inline constexpr float  SCENE_CENTRE_WIDTH_FACTOR                   = +0.5f;
		inline constexpr float  BLACK_TRANSITION_SPEED_X                    = +3.0f;
		inline constexpr int    STAGE_CLEAR_TRANSITION_TURNS                = +500;
		inline constexpr int    GAME_OVER_TRANSITION_TURNS                  = +300;
		inline constexpr int    ENDING_EXPLOSION_DELAY_FRAMES               = +120;
		// Counted in logic turns, not milliseconds: CreditSceneState gates the two
		// pages on its own per-Render tick, not on the wall clock.
		inline constexpr ULONGLONG CREDIT_PAGE_ONE_DURATION_TURNS           = +900;
		inline constexpr ULONGLONG CREDIT_PAGE_TWO_DURATION_TURNS           = +1000;
		inline constexpr int    ENDING_TOTAL_FRAME_COUNT                    = +52;
		inline constexpr DWORD  ENDING_FRAME_DELAY_MILLISECONDS             = +150;
		inline constexpr DWORD  TEXT_BLINK_INTERVAL_MILLISECONDS            = +300;
		inline constexpr DWORD  TEXT_BLINK_PERIOD_MILLISECONDS              = +600;
		// Two different life counts, deliberately: a fresh run starts here...
		inline constexpr int    INITIAL_LIVES_COUNT                         = +50;
		// ...while picking CONTINUE at the game-over screen grants only this many.
		inline constexpr int    CONTINUE_LIVES_COUNT                        = +3;
		inline constexpr int    DEFAULT_HIGH_SCORE                          = +20000;
		inline constexpr float  START_SCENE_CURSOR_POSITION_X               = +83.0f;
		inline constexpr float  START_SCENE_CURSOR_POSITION_Y               = +80.0f;
		inline constexpr float  START_SCENE_FALCON_LOGO_POSITION_X          = +25.0f;
		inline constexpr float  START_SCENE_FALCON_LOGO_POSITION_Y          = +79.0f;
		inline constexpr float  GAME_OVER_SELECTION_ROW_CONTINUE            = +10.5f;
		inline constexpr float  GAME_OVER_SELECTION_ROW_END                 = +8.5f;
		inline constexpr float  GAME_OVER_FALCON_CURSOR_COLUMN              = +11.0f;
		inline constexpr size_t FILEPATH_BUFFER_MAX_LENGTH                  = +128;

		// UI Text Rows and Columns
		inline constexpr float UI_ROW_PLAYER_ONE                            = +25.0f;
		inline constexpr float UI_COLUMN_PLAYER_ONE                         = +5.0f;
		inline constexpr float UI_ROW_HIGH_SCORE                            = +19.0f;
		inline constexpr float UI_COLUMN_HIGH_SCORE                         = +10.0f;
		inline constexpr float UI_ROW_REST                                  = +23.0f;
		inline constexpr float UI_COLUMN_REST                               = +5.0f;
		inline constexpr float UI_ROW_STAGE_NUMBER                          = +14.0f;
		inline constexpr float UI_COLUMN_STAGE_NUMBER                       = +12.0f;
		inline constexpr float UI_ROW_STAGE_NAME                            = +12.0f;
		inline constexpr float UI_COLUMN_STAGE_NAME                         = +12.0f;
		inline constexpr float UI_ROW_SCORE_VALUE                           = +25.0f;
		inline constexpr float UI_COLUMN_SCORE_VALUE                        = +10.0f;
		inline constexpr float UI_ROW_HIGH_SCORE_VALUE                      = +19.0f;
		inline constexpr float UI_COLUMN_HIGH_SCORE_VALUE                   = +15.0f;
		inline constexpr float UI_ROW_LIVES_LEFT_VALUE                      = +23.0f;
		inline constexpr float UI_COLUMN_LIVES_LEFT_VALUE                   = +10.0f;
		inline constexpr float UI_ROW_GAME_OVER_TITLE                       = +14.0f;
		inline constexpr float UI_COLUMN_GAME_OVER_TITLE                    = +12.0f;
		inline constexpr float UI_ROW_GAME_OVER_SELECTION_CONTINUE_TEXT     = +11.0f;
		inline constexpr float UI_COLUMN_GAME_OVER_SELECTION_CONTINUE_TEXT  = +13.0f;
		inline constexpr float UI_ROW_GAME_OVER_SELECTION_END_TEXT          = +9.0f;
		inline constexpr float UI_COLUMN_GAME_OVER_SELECTION_END_TEXT       = +13.0f;
		inline constexpr float UI_ROW_HUD_LIVES_ICONS                       = +27.0f;
		inline constexpr float UI_COLUMN_HUD_LIVES_ICONS_START              = +2.0f;
		inline constexpr float UI_COLUMN_CREDIT_PAGE_ONE                    = +1.0f;
		inline constexpr float UI_COLUMN_CREDIT_PAGE_TWO                    = +2.0f;
		inline constexpr float UI_ROW_CREDIT_LINE_1                         = +16.0f;
		inline constexpr float UI_ROW_CREDIT_LINE_2                         = +14.0f;
		inline constexpr float UI_ROW_CREDIT_LINE_3                         = +12.0f;
		inline constexpr float UI_ROW_CREDIT_LINE_4                         = +10.0f;
	}

	// Stages
	namespace Stages
	{
		inline constexpr float  BOUNDARY_WALL_THICKNESS                     = +20.0f;
		inline constexpr float  CHECKPOINT_FALL_THRESHOLD                   = -3.0f;
		inline constexpr float  EXPLOSION_SPAWN_OFFSET_Y                    = +20.0f;
		inline constexpr size_t DEFAULT_CANDIDATE_RESERVE_CAPACITY          = +16;
		inline constexpr int    TOTAL_STAGES                                = +2;
		inline constexpr int    STAGE_1_INDEX                               = +1;
		inline constexpr int    STAGE_2_INDEX                               = +2;
		inline constexpr float  CAMERA_TRANSLATE_STEP                       = +1.0f;
		inline constexpr float  REVIVAL_DEFAULT_HEIGHT_FACTOR               = +1.0f;

		namespace Stage1
		{
			inline constexpr float BILL_SPAWN_OFFSET_FACTOR_W               = +2.0f;
		}

		namespace Stage2
		{
			inline constexpr float WALL_CAMERA_BOTTOM_OFFSET_RATIO          = +0.90f;
			inline constexpr float BILL_SPAWN_OFFSET_RATIO_X                = +1.75f;
			inline constexpr float BILL_SPAWN_OFFSET_RATIO_Y                = +2.00f;
			inline constexpr float ROCKFLY_SPAWN_1_BEGIN                    = +80.0f;
			inline constexpr float ROCKFLY_SPAWN_1_END                      = +176.0f;
			inline constexpr float ROCKFLY_SPAWN_2_BEGIN                    = +64.0f;
			inline constexpr float ROCKFLY_SPAWN_2_END                      = +144.0f;
			inline constexpr float ROCKFLY_SPAWN_3_BEGIN                    = +80.0f;
			inline constexpr float ROCKFLY_SPAWN_3_END                      = +224.0f;
		}
	}
}
