#include <vector>
#include <string>

class Blocks
{
public:
	std::vector<double> vec;
	double error;
	int blockID;

	Blocks(int id = 0)
	{
		error = 0;
		blockID = id;
	}
};

class Music
{
public:
	int MusicID;
	double errorToSample;
	std::string Name;
	int Shift;
	int MusicBlockSize;

	std::vector<Blocks> MusicBlocks;
	std::vector<Blocks> CodeBook;

	Music(): MusicID(0), errorToSample(0)
	{
	}
};

class Sample
{
public:

	std::vector<Blocks> SampleBlocks;
	std::vector<Blocks> CodeBook;

};


