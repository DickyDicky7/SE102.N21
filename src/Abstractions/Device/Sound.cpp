#include "Sound.h"


Sound* Sound::_instance = nullptr;

Sound* Sound::GetInstance()
{
	return _instance;
}

Sound::Sound(HWND hWnd)
	: _pDevice(nullptr), _primaryBuffer(nullptr), _volume(Constants::Audio::DEFAULT_VOLUME_PERCENTAGE), _isMute(false)
{

	HRESULT result;

	DSBUFFERDESC bufferDesc; //describe the buffer

	result = DirectSoundCreate8(nullptr, &this->_pDevice, nullptr);

	// Without the return, every line below dereferenced a _pDevice the failed
	// call never wrote to.  LoadSound and Play are already null-safe about it,
	// so a machine with no sound device now runs the game silently instead of
	// crashing before the first frame.
	if (FAILED(result) || !this->_pDevice)
	{
		std::cout << "Can not create device";
		this->_pDevice = nullptr;
		return;
	}

	result = this->_pDevice->SetCooperativeLevel(hWnd, DSSCL_PRIORITY); // set the cooperative level.

	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = nullptr;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	result = this->_pDevice->CreateSoundBuffer(&bufferDesc, &this->_primaryBuffer, nullptr);

	if (FAILED(result))
	{
		std::cout << "Can not create the primary sound buffer";
	}
}

Sound::~Sound()
{
	for (auto& [name, buffer] : this->_soundBufferMap)
	{
		if (buffer)
		{
			buffer->Release();
			buffer = nullptr;
		}

	}
	if (this->_primaryBuffer)
		this->_primaryBuffer->Release();
	if (this->_pDevice)
		this->_pDevice->Release();
	this->_pDevice = nullptr;

	this->_primaryBuffer = nullptr;
}


void Sound::Create(HWND hWnd)
{
	if (_instance == nullptr)
	{
		_instance = new Sound(hWnd);
	}
}

namespace
{
	std::string_view NormalizeSoundName(std::string_view name)
	{
		constexpr std::string_view WAV_SUFFIX = ".wav";
		if (name.size() >= WAV_SUFFIX.size() && name.ends_with(WAV_SUFFIX))
		{
			name.remove_suffix(WAV_SUFFIX.size());
		}
		return name;
	}
}

float Sound::GetVolume() const
{
	return this->_volume;
}

void Sound::LoadSound(const char* fileName, std::string_view name)
{
	name = NormalizeSoundName(name);
	if (this->_soundBufferMap.find(name) != this->_soundBufferMap.end())
		return;
	FILE* filePtr = nullptr;
	WaveHeaderStruct waveHeaderStruct;
	IDirectSoundBuffer* tempBuffer = nullptr;
	DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;
	unsigned char* bufferPtr = nullptr;
	DWORD bufferSize = 0;

	if (!this->_pDevice)
		return;

	int error = fopen_s(&filePtr, fileName, "rb");
	if (error != 0 || !filePtr)
	{
		std::cout << " Can not load " << fileName << "\n";
		return;
	}

	size_t readHeader = fread(&waveHeaderStruct, sizeof(WaveHeaderStruct), 1, filePtr);
	if (readHeader < 1)
	{
		fclose(filePtr);
		return;
	}

	if ((waveHeaderStruct.format[0] != 'W') || (waveHeaderStruct.format[1] != 'A') ||
		(waveHeaderStruct.format[2] != 'V') || (waveHeaderStruct.format[3] != 'E'))
	{
		std::cout << " file format does not support " << fileName << "\n";
		fclose(filePtr);
		return;
	}

	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = waveHeaderStruct.sampleRate;
	waveFormat.wBitsPerSample = waveHeaderStruct.bitsPerSample;
	waveFormat.nChannels = waveHeaderStruct.numChannels;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_GLOBALFOCUS;
	bufferDesc.dwBufferBytes = waveHeaderStruct.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	IDirectSoundBuffer8* secondaryBuffer = nullptr;

	this->_pDevice->CreateSoundBuffer(&bufferDesc, &tempBuffer, nullptr);
	if (!tempBuffer)
	{
		fclose(filePtr);
		return;
	}

	long result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, reinterpret_cast<void**>(&secondaryBuffer));
	tempBuffer->Release();
	tempBuffer = nullptr;

	if (FAILED(result) || !secondaryBuffer)
	{
		std::cout << " Can not create secondaryBuffer " << "\n";
		fclose(filePtr);
		return;
	}

	fseek(filePtr, sizeof(WaveHeaderStruct), SEEK_SET);

	result = secondaryBuffer->Lock(0, waveHeaderStruct.dataSize, reinterpret_cast<void**>(&bufferPtr), &bufferSize, nullptr, 0, 0);

	if (FAILED(result) || !bufferPtr)
	{
		fclose(filePtr);
		secondaryBuffer->Release();
		return;
	}

	fread(bufferPtr, waveHeaderStruct.dataSize, 1, filePtr);
	fclose(filePtr);

	secondaryBuffer->Unlock(bufferPtr, bufferSize, nullptr, 0);

	long tempVolume = static_cast<long>((this->_volume) / Constants::Audio::VOLUME_PERCENTAGE_MAX * (-DSBVOLUME_MIN) + DSBVOLUME_MIN);
	secondaryBuffer->SetVolume(tempVolume);

	this->_soundBufferMap[std::string(name)] = secondaryBuffer;
}


void Sound::Play(std::string_view name, bool infiniteLoop, int times)
{
	if (this->_isMute)
	{
		return;
	}
	name = NormalizeSoundName(name);
	auto it = this->_soundBufferMap.find(name);
	if (it == this->_soundBufferMap.end() || !it->second)
		return;
	if (infiniteLoop)
	{
		it->second->Play(0, 0, DSBPLAY_LOOPING);
	}
	else
	{
		it->second->Stop();
		it->second->SetCurrentPosition(0);
		it->second->Play(0, 0, times - 1);
	}

}

void Sound::Stop(std::string_view name)
{
	if (name.empty())
	{
		for (auto& [soundName, buffer] : this->_soundBufferMap)
		{
			if (buffer)
			{
				buffer->Stop();
				buffer->SetCurrentPosition(0);
			}
		}
	}
	else
	{
		name = NormalizeSoundName(name);
		auto it = this->_soundBufferMap.find(name);
		if (it != this->_soundBufferMap.end() && it->second)
		{
			it->second->Stop();
		}
	}
}

void Sound::SetVolume(float percentage, std::string_view name)
{
	this->_volume = percentage;
	long tempVolume = static_cast<long>((percentage) / Constants::Audio::VOLUME_PERCENTAGE_MAX * (-DSBVOLUME_MIN) + DSBVOLUME_MIN);
	if (name.empty())
	{
		for (auto& [soundName, buffer] : this->_soundBufferMap)
		{
			if (buffer)
			{
				buffer->SetVolume(tempVolume);
			}
		}
	}
	else
	{
		name = NormalizeSoundName(name);
		auto it = this->_soundBufferMap.find(name);
		if (it != this->_soundBufferMap.end() && it->second)
		{
			it->second->SetVolume(tempVolume);
		}
	}
}

void Sound::Mute()
{
	this->_isMute = true;
	this->Stop();
}

void Sound::UnMute()
{
	this->_isMute = false;
}

void Sound::CleanUp()
{
	_instance = nullptr;
	delete this;
}
