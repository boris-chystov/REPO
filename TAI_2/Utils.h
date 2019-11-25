#include <vector>
#include <numeric>
#include <math.h>
#include <fstream>
#include <filesystem>
#include <iostream>


using namespace std;

class Utils
{
public:

	void codebookToFile(Music& music) {
		if ( !filesystem::exists( "models" ) ) {
			std::filesystem::create_directory("models");
		}

		cout << "Writing in file CodeBook for: " << music.Name << endl;

		string filename = music.Name + ".txt";
		std::ofstream file("models/" + filename);

		file << std::to_string(music.MusicBlockSize) << " " << std::to_string(music.Shift) << " " << music.Name <<std::endl;
		for (auto codebookElement : music.CodeBook) {
			for (auto vecElement : codebookElement.vec) {
				file << std::to_string(vecElement) << " ";
			}
			file << std::endl;
		}
		file.close();

	}

	template <typename Out>
	void split(const std::string &s, char delim, Out result) {
		std::istringstream iss(s);
		std::string item;
		while (std::getline(iss, item, delim)) {
			*result++ = item;
		}
	}

	std::vector<std::string> split(const std::string &s, char delim) {
		std::vector<std::string> elems;
		split(s, delim, std::back_inserter(elems));
		return elems;
	}

	template <typename T>
	double	euclideanDistance(const vector<T>& a, const vector<T>& b)
	{
		vector<double>	auxiliary;
		transform(a.begin(), a.end(), b.begin(), back_inserter(auxiliary), [](T element1, T element2) {return pow((element1 - element2), 2); });
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
	std::vector<TargetClass> GetVectorAverage(std::vector<std::vector<TargetClass>>& target)
	{
		std::vector<TargetClass> totals = GetVectorTotal(target);
		std::vector<TargetClass> averages(totals.size());
		for (auto i = 0; i < signed(totals.size()); i++)
			averages[i] = totals[i] / target.size();
		return averages;
	}
};

