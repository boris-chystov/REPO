#include <string>
#include <fstream>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>
#include <random>


using namespace std;
using std::string;
using std::fstream;

typedef struct  WAV_HEADER
{
    /* RIFF Chunk Descriptor */
    uint8_t         RIFF[4];        // RIFF Header Magic header
    uint32_t        ChunkSize;      // RIFF Chunk Size
    uint8_t         WAVE[4];        // WAVE Header
    /* "fmt" sub-chunk */
    uint8_t         fmt[4];         // FMT header
    uint32_t        Subchunk1Size;  // Size of the fmt chunk
    uint16_t        AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw,     257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
    uint16_t        NumOfChan;      // Number of channels 1=Mono 2=Sterio
    uint32_t        SamplesPerSec;  // Sampling Frequency in Hz
    uint32_t        bytesPerSec;    // bytes per second
    uint16_t        blockAlign;     // 2=16-bit mono, 4=16-bit stereo
    uint16_t        bitsPerSample;  // Number of bits per sample
    /* "data" sub-chunk */
    uint8_t         Subchunk2ID[4]; // "data"  string
    uint32_t        Subchunk2Size;  // Sampled data length
} wav_hdr;


auto WavReader2(const char* fileName, int noiseRand = 0) -> vector<int>
{
    vector<int> vecResult;
    wav_hdr wavHeader;
    int headerSize = sizeof(wav_hdr), filelength = 0;

    FILE* wavFile = fopen(fileName, "r");

    //Read the header
    size_t bytesRead = fread(&wavHeader, 1, headerSize, wavFile);

    if (bytesRead > 0)
    {
        //Read the data
        uint16_t sample_size = wavHeader.bitsPerSample / 8;
        uint64_t samples_count = wavHeader.ChunkSize / sample_size; 
        static const uint16_t BUFFER_SIZE = 4096;
        int8_t* buffer = new int8_t[BUFFER_SIZE];

        auto* value = new short int[samples_count];
        
        memset(value, 0, sizeof(short int) * samples_count);

        //Reading data
        for (auto i = 0; i < samples_count; i++)
        {
            fread(&value[i], sample_size, 1, wavFile);
        }

        if(noiseRand != 0) {        
            std::random_device dev;
            std::mt19937 rng(dev());
            std::uniform_int_distribution<mt19937::result_type> dist6(1,noiseRand); // distribution in range [1, 6]

            //Write data into the file
            for (auto i = 0; i < samples_count; i++)
            {
                auto val = value[i];
                if (val == 0) continue;

                vecResult.push_back(value[i] + dist6(rng));
            }
        } else {
            //Write data into the file
            for (auto i = 0; i < samples_count; i++)
            {
                auto val = value[i];
                if (val == 0) continue;

                vecResult.push_back(value[i]);
            }
        }
    
    }
    fclose(wavFile);

	return  vecResult;
}

