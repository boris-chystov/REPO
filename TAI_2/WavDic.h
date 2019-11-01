#include <vector>

class Blocks
{
public:
	std::vector<float> vec;
	float error;
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
	std::vector<Blocks> MusicBlocks;

	std::vector<Blocks> CodeBook;

	Music()
	{
		MusicID = 0;
	}

};


