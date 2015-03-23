#include <fstream>
#include <iostream>
#include <time.h>


int main(int argc, char* argv[]) {
	
	std::ifstream infile;
	std::ofstream outfile;

	int n = std::atoi(argv[1]);

	infile.open("american");
	outfile.open(std::string(argv[1])+"_american.txt");

	srand (time(NULL));

	int* wordLineNumbers = new int[n];
	std::string* wordArray = new std::string[n];


	for(int i = 0; i < n; i++)
	{
		wordLineNumbers[i] = rand() % 305089 + 1;	
		std::cout << wordLineNumbers[i] << std::endl;
	}

	int lineNum = 1;
	std::string line;

	while(std::getline(infile, line))
	{
		for (int i = 0; i < n; i++)
		{
			if(lineNum == wordLineNumbers[i]) wordArray[i] = line;
		}
		lineNum++;
	}


	for(int i = 0; i < n; i++)
	{
		std::cout << wordArray[i] << std::endl;
		outfile << wordArray[i] << std::endl;
	}
	infile.close();
	outfile.close();

}
