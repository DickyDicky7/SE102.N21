#include "GraphicsDatabase.h"

std::unordered_map<TEXTURE_ID, LPDIRECT3DTEXTURE9> GraphicsDatabase::textures;
std::unordered_map<SPRITE_ID, std::pair<RECT*, TEXTURE_ID>> GraphicsDatabase::sprites;
std::unordered_map<ANIMATION_ID, std::pair<DWORD, std::vector<std::pair<SPRITE_ID, DWORD>>>> GraphicsDatabase::animations;