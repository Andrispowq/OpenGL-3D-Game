#include "engine/prehistoric/core/util/Includes.hpp"
#include "AudioEngine.h"
#include <bit>

#include "efx-creative.h"
#include "EFX-Util.h"

/*std::int32_t convert_to_int(char* buffer, std::size_t len)
{
    std::int32_t a = 0;
    if (std::endian::native == std::endian::little)
        std::memcpy(&a, buffer, len);
    else
        for (std::size_t i = 0; i < len; ++i)
            reinterpret_cast<char*>(&a)[3 - i] = buffer[i];
    return a;
}

bool load_wav_file_header(std::ifstream& file,
    std::uint8_t& channels,
    std::int32_t& sampleRate,
    std::uint8_t& bitsPerSample,
    ALsizei& size)
{
    char buffer[4];
    if (!file.is_open())
        return false;

    // the RIFF
    if (!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read RIFF" << std::endl;
        return false;
    }
    if (std::strncmp(buffer, "RIFF", 4) != 0)
    {
        std::cerr << "ERROR: file is not a valid WAVE file (header doesn't begin with RIFF)" << std::endl;
        return false;
    }

    // the size of the file
    if (!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read size of file" << std::endl;
        return false;
    }

    // the WAVE
    if (!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read WAVE" << std::endl;
        return false;
    }
    if (std::strncmp(buffer, "WAVE", 4) != 0)
    {
        std::cerr << "ERROR: file is not a valid WAVE file (header doesn't contain WAVE)" << std::endl;
        return false;
    }

    // "fmt/0"
    if (!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read fmt/0" << std::endl;
        return false;
    }

    // this is always 16, the size of the fmt data chunk
    if (!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read the 16" << std::endl;
        return false;
    }

    // PCM should be 1?
    if (!file.read(buffer, 2))
    {
        std::cerr << "ERROR: could not read PCM" << std::endl;
        return false;
    }

    // the number of channels
    if (!file.read(buffer, 2))
    {
        std::cerr << "ERROR: could not read number of channels" << std::endl;
        return false;
    }
    channels = convert_to_int(buffer, 2);

    // sample rate
    if (!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read sample rate" << std::endl;
        return false;
    }
    sampleRate = convert_to_int(buffer, 4);

    // (sampleRate * bitsPerSample * channels) / 8
    if (!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read (sampleRate * bitsPerSample * channels) / 8" << std::endl;
        return false;
    }

    // ?? dafaq
    if (!file.read(buffer, 2))
    {
        std::cerr << "ERROR: could not read dafaq" << std::endl;
        return false;
    }

    // bitsPerSample
    if (!file.read(buffer, 2))
    {
        std::cerr << "ERROR: could not read bits per sample" << std::endl;
        return false;
    }
    bitsPerSample = convert_to_int(buffer, 2);

    // data chunk header "data"
    if (!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read data chunk header" << std::endl;
        return false;
    }
    if (std::strncmp(buffer, "data", 4) != 0)
    {
        std::cerr << "ERROR: file is not a valid WAVE file (doesn't have 'data' tag)" << std::endl;
        return false;
    }

    // size of data
    if (!file.read(buffer, 4))
    {
        std::cerr << "ERROR: could not read data size" << std::endl;
        return false;
    }
    size = convert_to_int(buffer, 4);

    // cannot be at the end of file
    if (file.eof())
    {
        std::cerr << "ERROR: reached EOF on the file" << std::endl;
        return false;
    }
    if (file.fail())
    {
        std::cerr << "ERROR: fail state set on the file" << std::endl;
        return false;
    }

    return true;
}

char* load_wav(const std::string& filename,
    std::uint8_t& channels,
    std::int32_t& sampleRate,
    std::uint8_t& bitsPerSample,
    ALsizei& size)
{
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open())
    {
        std::cerr << "ERROR: Could not open \"" << filename << "\"" << std::endl;
        return nullptr;
    }
    if (!load_wav_file_header(in, channels, sampleRate, bitsPerSample, size))
    {
        std::cerr << "ERROR: Could not load wav header of \"" << filename << "\"" << std::endl;
        return nullptr;
    }

    char* data = new char[size];

    in.read(data, size);

    return data;
}*/

typedef struct WAV_HEADER
{
    // RIFF Chunk Descriptor
    uint8_t         RIFF[4];        // RIFF Header Magic header
    uint32_t        ChunkSize;      // RIFF Chunk Size
    uint8_t         WAVE[4];        // WAVE Header
    // "fmt" sub-chunk
    uint8_t         fmt[4];         // FMT header
    uint32_t        Subchunk1Size;  // Size of the fmt chunk
    uint16_t        AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw,     257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
    uint16_t        NumOfChan;      // Number of channels 1=Mono 2=Sterio
    uint32_t        SamplesPerSec;  // Sampling Frequency in Hz
    uint32_t        bytesPerSec;    // bytes per second
    uint16_t        blockAlign;     // 2=16-bit mono, 4=16-bit stereo
    uint16_t        bitsPerSample;  // Number of bits per sample
    // "data" sub-chunk
    uint8_t         Subchunk2ID[4]; // "data"  string
    uint32_t        Subchunk2Size;  // Sampled data length
} wav_hdr;

char* load_wav(const std::string& filename, wav_hdr& header)
{
    size_t headerSize = sizeof(wav_hdr), filelength = 0;

    std::ifstream file(filename.c_str(), std::ios::ate | std::ios::binary);
    size_t fileSize = file.tellg();
    file.seekg(0);

    if (!file.is_open())
    {
        PR_LOG_ERROR("Unable to open wave file: %s\n", filename.c_str());
        return nullptr;
    }

    //Read the header
    char* header_data = new char[headerSize];    
    file.read(header_data, headerSize);
    memcpy(&header, (void*)header_data, headerSize); //Copying the loaded bytes - not safe, but it will be okay for now

    char* data = new char[header.Subchunk2Size];
    file.read(data, header.Subchunk2Size);

    return data;
}

ALuint buffer;
ALuint source;

AudioEngine::AudioEngine()
{
    device = alcOpenDevice(nullptr);
    if (!device)
    {
        PR_LOG_ERROR("Cannot create OpenAL device!\n");
    }

    context = alcCreateContext(device, nullptr);
    if (!context)
    {
        PR_LOG_ERROR("Cannot create OpenAL context!\n");
    }

    if (!alcMakeContextCurrent(context))
    {
        PR_LOG_ERROR("Cannot make the OpenAL context current!\n");
    }

    wav_hdr header;
    char* soundData = load_wav("res/sounds/bounce.wav", header);

    if (!soundData)
    {
        PR_LOG_ERROR("ERROR: Could not load wav\n");
    }

    ALuint buffer;
    alGenBuffers(1, &buffer);

    ALenum format;
    if (header.NumOfChan == 1 && header.bitsPerSample == 8)
        format = AL_FORMAT_MONO8;
    else if (header.NumOfChan == 1 && header.bitsPerSample == 16)
        format = AL_FORMAT_MONO16;
    else if (header.NumOfChan == 2 && header.bitsPerSample == 8)
        format = AL_FORMAT_STEREO8;
    else if (header.NumOfChan == 2 && header.bitsPerSample == 16)
        format = AL_FORMAT_STEREO16;
    else
    {
        std::cerr
            << "ERROR: unrecognised wave format: "
            << header.NumOfChan << " channels, "
            << header.bitsPerSample << " bps" << std::endl;
    }

    /*uint8_t channels, bitsPerSample;
    int32_t sampleRate;
    int32_t size;
    char* soundData = load_wav("res/sounds/sweden.wav", channels, sampleRate, bitsPerSample, size);*/

    //alGenBuffers(1, &buffer);
    //alBufferData(buffer, format, soundData, size, sampleRate);

    alBufferData(buffer, format, soundData, header.Subchunk2Size, header.SamplesPerSec);
    delete[] soundData;

    alGenSources(1, &source);

    alSourcef(source, AL_PITCH, 1.0f);
    alSourcef(source, AL_GAIN, 1.0f);
    alSource3f(source, AL_POSITION, 0, 0, 0);
    alSource3f(source, AL_VELOCITY, 0, 0, 0);
    alSourcei(source, AL_LOOPING, AL_FALSE);
    alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
    alSourcei(source, AL_BUFFER, buffer);

    alSourcePlay(source);
}

AudioEngine::~AudioEngine()
{
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);

    alcDestroyContext(context);

    if (!alcCloseDevice(device))
    {
        PR_LOG_ERROR("Cannot close the OpenAL device!\n");
    }
}

void AudioEngine::Input()
{
}

void AudioEngine::Update(float delta)
{
    //TODO: write loading code for the AudioEngine
    /*ALint state = AL_PLAYING;
    alGetSourcei(source, AL_SOURCE_STATE, &state);

    if (state != AL_PLAYING)
    {
        alSourcePlay(source);
    }*/
}
