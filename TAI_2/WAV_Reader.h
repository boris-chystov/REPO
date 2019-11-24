#include <string>
#include <fstream>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>


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


auto WavReader2(const char* fileName) -> vector<int>
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
        uint16_t bytesPerSample = wavHeader.bitsPerSample / 8;      //Number     of bytes per sample
        uint64_t numSamples = wavHeader.ChunkSize / bytesPerSample; //How many samples are in the wav file?
        static const uint16_t BUFFER_SIZE = 4096;
        int8_t* buffer = new int8_t[BUFFER_SIZE];
        
			auto sample_size = wavHeader.bitsPerSample / 8;
			int samples_count = wavHeader.ChunkSize * 8 / wavHeader.bitsPerSample;

            auto* value = new short int[samples_count];
			
			memset(value, 0, sizeof(short int) * samples_count);

			//Reading data
			for (auto i = 0; i < samples_count; i++)
			{
				fread(&value[i], sample_size, 1, wavFile);
			}

			//Write data into the file
			for (auto i = 0; i < samples_count; i++)
			{
                // Adding noise :: generating rand number and sum at the result
                int numRand = rand() % 101;
		        if (value[i] == 0) continue;
				vecResult.push_back(value[i] + numRand);
			}
        
       }
    fclose(wavFile);

	return  vecResult;
}

