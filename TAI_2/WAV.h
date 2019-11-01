#include <cstdio>
#include <vector>
#include <cstring>


using namespace std;


struct wav_header_t
{
	char chunkID[4]; //"RIFF" = 0x46464952
	unsigned long chunkSize; //28 [+ sizeof(wExtraFormatBytes) + wExtraFormatBytes] + sum(sizeof(chunk.id) + sizeof(chunk.size) + chunk.size)
	char format[4]; //"WAVE" = 0x45564157
	char subchunk1ID[4]; //"fmt " = 0x20746D66
	unsigned long subchunk1Size; //16 [+ sizeof(wExtraFormatBytes) + wExtraFormatBytes]
	unsigned short audioFormat;
	unsigned short numChannels;
	unsigned long sampleRate;
	unsigned long byteRate;
	unsigned short blockAlign;
	unsigned short bitsPerSample;

};

//Chunks
struct chunk_t
{
	char ID[4]; //"data" = 0x61746164
	unsigned long size;  //Chunk data bytes
};

auto WavReader(const char* fileName) -> vector<int>
{
	vector<int> vecResult;
	auto fin = fopen(fileName, "rb");

	//Read WAV header
	wav_header_t header;
	fread(&header, sizeof(header), 1, fin);

	//Reading file
	chunk_t chunk;

	//go to data chunk
	while (true)
	{
		fread(&chunk, sizeof(chunk), 1, fin);
		if (*reinterpret_cast<unsigned int*>(&chunk.ID) == 0x61746164)
			break;
		//skip chunk data bytes
		fseek(fin, chunk.size, SEEK_CUR);
	}

	//Number of samples
	auto sample_size = header.bitsPerSample / 8;
	int samples_count = chunk.size * 8 / header.bitsPerSample;

	auto* value = new short int[samples_count];
	memset(value, 0, sizeof(short int) * samples_count);

	//Reading data
	for (auto i = 0; i < samples_count; i++)
	{
		fread(&value[i], sample_size, 1, fin);
	}

	
	//Write data into the file
	for (auto i = 0; i < samples_count; i++)
	{
		vecResult.push_back(value[i]);
	}
	
	fclose(fin);

	return  vecResult;
	 
}
