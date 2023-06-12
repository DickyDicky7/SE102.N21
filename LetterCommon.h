#pragma once

template <int prefix> class ID; template <> class std::hash<ID<0>> : public std::hash<std::string> {}; template <> class std::equal_to<ID<0>> : public std::equal_to<std::string> {};
using LETTER_SPRITE_ID = ID<0>; using LETTER_TEXTURE_ID = ID<0>; using LETTER_ANIMATION_ID = ID<0>;
