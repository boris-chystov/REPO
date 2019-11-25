#include "WAV_Reader.h"

#include <iterator>
#include <numeric>
#include <iostream>
#include <cmath>
#include <optional>
#include <random>

using namespace std;

int main() {

	char* original_file_location;
	cout << "Please insert the location of the original audio file: ";
	cin >> original_file_location;

	char* file_noise_location;
	cout << "Please insert the location of the audio file with noise: ";
	cin >> file_noise_location;

	auto original_file = WavReader2(original_file_location);
	auto file_noise = WavReader2(file_noise_location);

	auto energy_original = 0;
	auto energy_noise = 0;
	auto max = 0;

	for(auto i = 0; i < original_file.size; i++)
	{
		energy_original += pow(original_file[i], 2);
		int dif = int(original_file[i]) - int(file_noise[i]);
		max = std::max(max, std::abs(dif));
		energy_noise += dif * dif;
	}

	auto snr = (10 * std::log10(energy_original / energy_noise));


	//auto snr = 10 * (log(energy_of_signal / accumulate(energy_of_noise->begin, energy_of_noise->end, 0)));

	cout << "The signal-to-noise ratio of the audio file is: " << snr << endl;

	return 0;

}


