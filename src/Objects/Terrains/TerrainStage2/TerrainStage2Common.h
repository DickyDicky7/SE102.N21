#pragma once

template <int prefix> class ID; template <> class std::hash<ID<2>> : public std::hash<std::string> {}; template <> class std::equal_to<ID<2>> : public std::equal_to<std::string> {};
using TERRAIN_STAGE_2_SPRITE_ID = ID<2>; using TERRAIN_STAGE_2_TEXTURE_ID = ID<2>; using TERRAIN_STAGE_2_ANIMATION_ID = ID<2>;
