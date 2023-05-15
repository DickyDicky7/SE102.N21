#pragma once

template <int prefix> class ID; template <> class std::hash<ID<1>> : public std::hash<std::string> {}; template <> class std::equal_to<ID<1>> : public std::equal_to<std::string> {};
using TERRAIN_STAGE_1_SPRITE_ID = ID<1>; using TERRAIN_STAGE_1_TEXTURE_ID = ID<1>; using TERRAIN_STAGE_1_ANIMATION_ID = ID<1>;
