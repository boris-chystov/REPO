#include "WAV.h"
#include "WavDic.h"
#include "Utils.h"

#include <filesystem>
#include <algorithm>
#include <iostream>
#include <random>
#include <numeric>
#include <valarray>

using namespace std;
Utils utils;



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

		if(matches.empty()) continue;

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
	size_t nelems = 150;

	sample(
		music.MusicBlocks.begin(),
		music.MusicBlocks.end(),
		back_inserter(music.CodeBook),
		nelems,
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

		cout << "KNN Error: " << error << endl;
		
		//percentagem de erro entre ciclos
		if (fabs((error - prevError) / prevError * 100) < 1) break;
		prevError = error;

	}
}

int main()
{
	
	vector<Music> dict;
	auto id = 1;
	auto idBlock = 1;
	auto path = "C:/Users/Borys/Desktop/WAV files-20191019";
	srand(time(NULL)); // used for reset rand number
	
	for (const auto& entry : filesystem::directory_iterator(path)) {
		cout << "PATH: " << entry.path() << endl;
		auto music = new Music();
		music->Name = entry.path().filename().string();
		music->MusicID = id;
		
		auto data = WavReader(entry.path().string().c_str());
		auto block = new Blocks(idBlock);

		for (auto i = 1; i < data.size(); i++)
		{
			block->vec.push_back(data[i]);

			if (i % 500 == 0)
			{
				music->MusicBlocks.push_back(*block);
				block = new Blocks(++idBlock);
			}

			/*if (i == data.size() - 1)
			{
				music->MusicBlocks.push_back(*block);
			}*/

		}
		id++;
		dict.push_back(*music);
	}


	createCodebook(dict[0]);
	createCodebook(dict[1]);
	createCodebook(dict[2]);
	createCodebook(dict[3]);
	createCodebook(dict[4]);
	createCodebook(dict[5]);
	createCodebook(dict[6]);

	Sample sample;
	auto data = WavReader("C:/Users/Borys/Desktop/sample03.wav");
	auto block = new Blocks(idBlock);

	for (auto i = 1; i < data.size(); i++)
	{
		block->vec.push_back(data[i]);

		if (i % 500 == 0)
		{
			sample.SampleBlocks.push_back(*block);
			block = new Blocks();
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


	cout << "RESULTADO;" << endl;
	cout << dict[0].errorToSample << endl;
	cout << dict[1].errorToSample << endl;
	cout << dict[2].errorToSample << endl;
	cout << dict[3].errorToSample << endl;
	cout << dict[4].errorToSample << endl;
	cout << dict[5].errorToSample << endl;
	cout << dict[6].errorToSample << endl;
	return 0;
}
