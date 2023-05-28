#pragma once

template <int prefix> class ID; template <> class std::hash<ID<0>> : public std::hash<std::string> {}; template <> class std::equal_to<ID<0>> : public std::equal_to<std::string> {};
using FONT_SPRITE_ID = ID<0>; using FONT_TEXTURE_ID = ID<0>; using FONT_ANIMATION_ID = ID<0>;
