#include "WavDic.h"
#include "WAV_Reader.h"
#include "Utils.h"

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <filesystem>
#include <iterator>
#include <algorithm>
#include <iterator>

using namespace std;
Utils utils;
int block_size;
int overlap_factor;


int main() {

	string directory;
	cout << "Please insert the directory in which the codebooks are placed: ";
	cin >> directory;

	string samplePath;
	cout << "Please insert the path of the sample of the audio file: ";
	cin >> samplePath;

	namespace fs = filesystem;
	string line;
	int idBlock = 0;
	vector<Music> dict;

	for (const auto& entry : fs::directory_iterator(directory))
	{

		ifstream infile(entry.path().string().c_str());
		auto music = new Music();
		music->Name = entry.path().string();
		auto block = new Blocks(idBlock);
		string line1;

		getline(infile, line1);
		std::vector<std::string> x = utils.split(line1, ' ');

		block_size = stoi(x[0]);
		overlap_factor = stoi(x[1]);
		music->Name = x[2];

		while (getline(infile, line))
		{
			auto values = utils.split(line, ' ');

			for (auto&& x : values)
				block->vec.push_back(stod(x));

			music->CodeBook.push_back(*block);
			block = new Blocks(++idBlock);			
		}
		infile.close();
		dict.push_back(*music);
	}


	Sample sample;
	auto data = WavReader2(samplePath.c_str(),0);
	auto block = new Blocks(idBlock);

	for (auto i = 1; i < data.size(); i++)
	{
		block->vec.push_back(data[i]);
		if((i % block_size == 0 and block->vec.size() == block_size) or block->vec.size() == block_size)
		{
			sample.SampleBlocks.push_back(*block);
			block = new Blocks();
			i -= overlap_factor;
		}
	}

	for (auto& music : dict)
	{
		auto totalError = 0.0;
		for (auto& sampleElement : sample.SampleBlocks)
		{
			auto error = 1000000000.0;
			for (auto& musicCodeBook : music.CodeBook)
			{
				auto distance = utils.euclideanDistance(sampleElement.vec, musicCodeBook.vec);
				if (distance < error)
				{
					error = distance;
				}
			}
			totalError += error;
		}
		music.errorToSample = totalError;
	}

	auto max = min_element( dict.begin(), dict.end(),
	[](const Music &a, const Music &b )
	{
		return a.errorToSample < b.errorToSample;
	});

	cout << "RESULTADO: " << max->Name << endl; 
	
	return 0;
}


