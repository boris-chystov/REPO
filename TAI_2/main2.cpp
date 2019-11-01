#include "WAV.h"
#include "WavDic.h"
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <random>
#include <numeric>
#include <valarray>

using namespace std;

template <typename T>
double	euclideanDistance(const vector<T>& a, const vector<T>& b)
{
	vector<double>	auxiliary;
	transform(a.begin(), a.end(), b.begin(), back_inserter(auxiliary),[](T element1, T element2) {return pow((element1 - element2), 2); });
	return  sqrt(accumulate(auxiliary.begin(), auxiliary.end(), 0.0));
}

template<class TargetClass>
size_t EqualizeVectors(std::vector<std::vector<TargetClass>>& target, const bool equalize = true)
{
	if (target.empty())
		return 0;
	size_t greatest_size = 0;
	for (auto& elem : target)
		for (auto& elem2 : target)
			if (elem.size() > greatest_size)
				greatest_size = elem.size();
			else if (elem.size() > elem2.size())
				greatest_size = elem.size();
	if (equalize)
		for (auto& elem : target)
			for (auto i = 0; i < signed(greatest_size - elem.size()); i++)
				elem.emplace_back(0);
	return greatest_size;
}

template<class TargetClass>
std::vector<TargetClass> GetVectorTotal(std::vector<std::vector<TargetClass>> target)
{
	const auto size = EqualizeVectors(target);
	std::vector<TargetClass> totals;
	for (auto i = 0; i < size; i++)
	{
		TargetClass sum = 0;
		for (auto j = 0; j < signed(target.size()); j++)
			sum += target[j][i];
		totals.emplace_back(sum);
		sum = 0;
	}
	return totals;
}

template<class TargetClass>
std::vector<TargetClass> GetVectorAverage(std::vector<std::vector<TargetClass>>& target, const bool omit_zero = true)
{
	std::vector<TargetClass> totals = GetVectorTotal(target);
	if (omit_zero)
		for (auto i = 0; i < signed(totals.size()); i++)
			if (totals[i] == 0)
				totals.erase(totals.begin() + i);
	std::vector<TargetClass> averages(totals.size());
	for (auto i = 0; i < signed(totals.size()); i++)
		averages[i] = totals[i] / target.size();
	return averages;
}


void updateCodebook(Music& music)
{
	for (auto& codebookElement : music.CodeBook)
	{
		auto maxError = 0;
		auto blocks = new Blocks();
		auto i = find_if(music.MusicBlocks.begin(), music.MusicBlocks.end(), [&](const Blocks& m) -> bool { return m.blockID == codebookElement.blockID;  });

		vector<Blocks> matches;
		vector<vector<float>> vec;

		copy_if(music.MusicBlocks.begin(), music.MusicBlocks.end(), back_inserter(matches), [&](Blocks v) {
			return v.blockID == codebookElement.blockID;
		});

		for (auto& element : matches)
		{
			maxError += element.error;
			vec.push_back(element.vec);
		}

		auto averages = GetVectorAverage(vec);
		codebookElement.vec = averages;


		codebookElement.error = maxError;
		
	}
}

void createCodebook(Music& music)
{
	auto error = 100000000;
	size_t nelems = 10;
	sample(
		music.MusicBlocks.begin(),
		music.MusicBlocks.end(),
		back_inserter(music.CodeBook),
		nelems,
		mt19937{ random_device{}() }
	);

	for(auto i = 0 ; i< 15; i++)
	{
		for (auto& block : music.MusicBlocks)
		{
			auto error = 100000000;
			for (auto codebookElement : music.CodeBook)
			{
				auto distance = euclideanDistance(block.vec, codebookElement.vec);
				if (distance < error)
				{
					error = distance;
					block.error = error;
					block.blockID = codebookElement.blockID;
				}
			}
		}

		updateCodebook(music);
		auto error =0;
		for (auto& element : music.CodeBook)
		{
			error += element.error;
		}

		cout << error << "\n";
	}
	
}

int main()
{

	vector<Music> dict;
	
	int id = 1;
	int idBlock = 1;
	auto path = "C:/Users/Borys/Desktop/WAV files-20191019";
	for (const auto& entry : filesystem::directory_iterator(path)) {
		
		auto music = new Music();
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
	return 0;
}
