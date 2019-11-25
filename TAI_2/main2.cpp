#include "WAV_Reader.h"
#include "WavDic.h"
#include "Utils.h"
#include <thread>
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <random>
#include <numeric>
#include <valarray>

using namespace std;
Utils utils;
int block_size;
int overlap_factor;
int codebook_size;
float stopKnn;
int noise;


void updateCodebook(Music& music)
{
	for (auto& codebookElement : music.CodeBook)
	{
		auto maxError = 0;
		auto blocks = new Blocks();
		auto i = find_if(music.MusicBlocks.begin(), music.MusicBlocks.end(), [&](const Blocks& m) -> bool { return m.blockID == codebookElement.blockID;  });

		vector<Blocks> matches;
		vector<vector<double>> vec;

		copy_if(music.MusicBlocks.begin(), music.MusicBlocks.end(), back_inserter(matches), [&](Blocks v) {
			return v.blockID == codebookElement.blockID;
			});

		if (matches.empty()) continue;

		for (auto& element : matches)
		{
			maxError += element.error;
			vec.push_back(element.vec);
		}

		auto averages = utils.GetVectorAverage(vec);
		codebookElement.vec = averages;
		codebookElement.error = maxError;

	}
}

void createCodebook(Music& music)
{
	cout << "Creating CodeBook for: " << music.Name << endl;

	auto error = 100000000.0;
	auto prevError = 0.0;

	 sample(
		music.MusicBlocks.begin(),
		music.MusicBlocks.end(),
		back_inserter(music.CodeBook),
		codebook_size,
		mt19937{ random_device{}() }
	);

	while (true) {
		for (auto& block : music.MusicBlocks)
		{
			auto error = 100000000.0;
			for (auto codebookElement : music.CodeBook)
			{
				auto distance = utils.euclideanDistance(block.vec, codebookElement.vec);
				if (distance < error)
				{
					error = distance;
					block.error = error;
					block.blockID = codebookElement.blockID;
				}
			}
		}

		updateCodebook(music);
		error = 0.0;
		for (auto& element : music.CodeBook)
		{
			error += element.error;
		}

		cout << "KNN Error " << music.Name << " : " << error << endl;

		//percentagem de erro entre ciclos
		if (fabs((error - prevError) / prevError * 100) < stopKnn) break;
		prevError = error;

	}
	utils.codebookToFile(music);

}

int main()
{

	string directory;
	cout << "Please insert the directory in which the codebooks are placed: ";
	cin >> directory;

	cout << "Please insert the desired block size: ";
	cin >> block_size;

	cout << "Please insert the overlap factor: ";
	cin >> overlap_factor;

	cout << "Please insert the codebook size: ";
	cin >> codebook_size;

	cout << "Percentage(0 - 100) KNN Stop: ";
	cin >> stopKnn;

	cout << "Please insert the noise (0  -  X): ";
	cin >> noise;

	if(block_size <= overlap_factor or overlap_factor < 0){
		cout << "overlap factor cannot be greater than the block size" << endl;
		return 0;
	}

	vector<Music> dict;
	auto id = 1;
	auto idBlock = 1;
	auto path = directory;

	for (const auto& entry : filesystem::directory_iterator(path)) {
		cout << "PATH: " << entry.path() << endl;
		auto music = new Music();
		music->Name = entry.path().filename().string();
		music->MusicID = id;
		music->Shift = overlap_factor;
		music->MusicBlockSize = block_size;

		auto data = WavReader2(entry.path().string().c_str(), noise);
		auto block = new Blocks(idBlock);

		for(auto i = 1; i < data.size(); i++)
		{
			block->vec.push_back(data[i]);

			if((i % block_size == 0 and block->vec.size() == block_size) or block->vec.size() == block_size)
			{
				music->MusicBlocks.push_back(*block);
				block = new Blocks(++idBlock);
				i -= overlap_factor;
			}
		}
		id++;
		dict.push_back(*music);
	}

	vector<thread> thrVec;
	for(auto& element : dict)
	{
	
		thrVec.emplace_back([&](){createCodebook(element); });

	}
	for(auto& t: thrVec)
	{
		t.join();
	}
	return 0;
}