#pragma once
#include "Constants.h"
#include <dsound.h>
#include <Windows.h>
#include <map>
#include <string>
#include <string_view>
#include <iostream>
#include <mmsystem.h>
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

class Sound
{
public:
	struct WaveHeaderStruct
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};
	static void Create(HWND hWnd);
	void SetVolume(float percentage, std::string_view name = "");
	void LoadSound(const char* fileName, std::string_view name);
	void Play(std::string_view name, bool infiniteLoop, int times);
	void Stop(std::string_view name = "");
	float GetVolume() const;
	~Sound();
	static Sound* GetInstance();
	void Mute();
	void UnMute();
	void CleanUp();
private:
	Sound(HWND hWnd);
	static Sound* _instance;
	IDirectSound8* _pDevice;
	IDirectSoundBuffer* _primaryBuffer;
	std::map<std::string, IDirectSoundBuffer8*, std::less<>> _soundBufferMap;
	float _volume;
	bool _isMute;
};
