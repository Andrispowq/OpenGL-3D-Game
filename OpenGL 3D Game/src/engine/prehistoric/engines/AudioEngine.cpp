#include "engine/prehistoric/core/util/Includes.hpp"
#include "AudioEngine.h"

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
}

AudioEngine::~AudioEngine()
{
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

    /*wav_hdr header;
    char* soundData = load_wav("res/sounds/bounce.wav", header);

    if (!soundData)
    {
        PR_LOG_ERROR("ERROR: Could not load wav\n");
        return 0;
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
        return 0;
    }

    alBufferData(buffer, format, soundData, header.Subchunk2Size, header.SamplesPerSec);
    delete[] soundData;

    ALuint source;
    alGenSources(1, &source);

    alSourcef(source, AL_PITCH, 1);
    alSourcef(source, AL_GAIN, 1.0f);
    alSource3f(source, AL_POSITION, 0, 0, 0);
    alSource3f(source, AL_VELOCITY, 0, 0, 0);
    alSourcei(source, AL_LOOPING, AL_FALSE);
    alSourcei(source, AL_BUFFER, buffer);

    alSourcePlay(source);

    ALint state = AL_PLAYING;

    while (state == AL_PLAYING)
    {
        alGetSourcei(source, AL_SOURCE_STATE, &state);
    }

    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);*/
}

/*typedef struct  WAV_HEADER
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

// find the file size
int getFileSize(FILE* inFile)
{
    int fileSize = 0;
    fseek(inFile, 0, SEEK_END);

    fileSize = ftell(inFile);

    fseek(inFile, 0, SEEK_SET);
    return fileSize;
}

char* load_wav(const char* filename, wav_hdr& header)
{
    size_t headerSize = sizeof(wav_hdr), filelength = 0;

    FILE* wavFile = fopen(filename, "r");
    if (wavFile == nullptr)
    {
        fprintf(stderr, "Unable to open wave file: %s\n", filename);
        return nullptr;
    }

    //Read the header
    size_t bytesRead = fread(&header, 1, headerSize, wavFile);
    std::cout << "Header Read " << bytesRead << " bytes." << std::endl;

    char* data = nullptr;

    if (bytesRead > 0)
    {
        //Read the data
        data = new char[header.Subchunk2Size];
        fread(data, sizeof(char), header.Subchunk2Size, wavFile);

        filelength = getFileSize(wavFile);
    }

    fclose(wavFile);

    return data;
}*/
