#include <iostream>
#include <assert.h>
#include <climits>
#include <sstream>
#include <time.h>
#include <fstream>


typedef struct {
	std::string* stringArray;
	// lengthOfWords not used, just used for testing
	int* lengthOfWords;
	int numberOfWords;
	int totalLengthOfCharacters;
} instance;

instance createInstance(std::string listOfWords) {
	std::string* wordArray = new std::string[1024];
	int* lengthOfWords = new int[1024];
	
	std::string delimiter = " ";
	
	std::string token;
	size_t pos = 0;
	int numberOfWords = 0;	
	// we maintain the size of the sum of all words with a space after each word except the last
	// it should make checking whether sum of all words is less than max line length
	int totalLengthOfCharacters = 0;
	
	while ((pos = listOfWords.find(delimiter)) != std::string::npos) {
		token = listOfWords.substr(0, pos);	
		listOfWords.erase(0, pos + delimiter.length());	
		wordArray[numberOfWords] = token;
		lengthOfWords[numberOfWords] = token.length();
		totalLengthOfCharacters += token.length() + 1;
		numberOfWords++;

	}
	
	// add in last word
	if(listOfWords != "") {
		wordArray[numberOfWords] = listOfWords;
		lengthOfWords[numberOfWords] = listOfWords.length();
		totalLengthOfCharacters += listOfWords.length();
		numberOfWords++;
	}
	

	// create struct with nice parameters for instance
	instance newInstance;
	newInstance.stringArray = wordArray;
	newInstance.numberOfWords = numberOfWords;
	newInstance.lengthOfWords = lengthOfWords;
	newInstance.totalLengthOfCharacters = totalLengthOfCharacters;
	
	return newInstance;
}

// returns the smallest index where the sum of wi ... wj < W
int smallestPossibleIndex(std::string* wordArray, int j, int W)
{
	int sum = wordArray[j].length();
	int index = j;
	while ((sum + wordArray[index-1].length() + 1) <= W && (j - 1) >= 0) {
		sum += wordArray[index-1].length() + 1;
		index--;		
	}
	return index;
}

int findLeastSumSlackNaiveHelper(std::string* wordArray, int j, int c, int W)
{

	// base case
	if(j < 0) return 0;
	if(c <= W) return (W - c)*(W - c);
	
	
	
	int min = INT_MAX;
	int p_j = smallestPossibleIndex(wordArray, j, W);
	
	
	for (int m = j; m >= p_j; m--) {
		int lineWeight = 0;
		for (int n = j; n >= m; n--) {
			lineWeight += wordArray[n].length() + 1;
		}
		lineWeight--;
		int slack = W - lineWeight;
		//assert(slack >= 0);
		
		int ret = (slack * slack) + findLeastSumSlackNaiveHelper(wordArray, m - 1, c - lineWeight - 1, W);
		
		if (ret < min) min = ret;
	}
	
	return min;

}

int findLeastSumSlackMemoHelper(std::string* wordArray, int j, int c, int W, int* soln)
{

	// base case
	if(j < 0) return 0;
	if(c <= W) return soln[j] = (W - c)*(W - c);
	
	
	int p_j = smallestPossibleIndex(wordArray, j, W);
	
	
	for (int m = j; m >= p_j; m--) {
		int lineWeight = 0;
		for (int n = j; n >= m; n--) {
			lineWeight += wordArray[n].length() + 1;
		}
		lineWeight--;
		int slack = W - lineWeight;
		//assert(slack >= 0);
		
		int ret;
		
		if(soln[m-1] != INT_MAX) ret = (slack * slack) + soln[m-1];
		
		else ret = (slack * slack) + findLeastSumSlackMemoHelper(wordArray, m - 1, c - lineWeight - 1, W, soln);
		
		if (ret < soln[j]) soln[j] = ret;
	}
	
	return soln[j];

}

int findLeastSumSlackDPHelper(std::string* wordArray, int j, int W, int* soln)
{	
	for (int a = 0; a <= j; a++) {
		
		if(a == 0) soln[0] = 0;		
		
		else 
		{
			int c = 0;
			for(int i = 0; i < a; i++) c += wordArray[i].length() + 1;
			c--;
			
			if(c <= W) soln[a] = (W - c)*(W - c);
				
			else 
			{
			
				int p_j = smallestPossibleIndex(wordArray, a-1, W);
				
				// calculations are done with indexing, stored in index of total numbers
				for(int m = a-1; m >= p_j; m--) {
					int lineWeight = 0;
					for (int n = a-1; n >= m; n--) {
						lineWeight += wordArray[n].length() + 1;
					}
					lineWeight--;
					int slack = W - lineWeight;
					//assert(slack >= 0);
					
					int ret;
					
					if(soln[m] != INT_MAX) ret = (slack * slack) + soln[m];
					
					else ret = (slack * slack) + findLeastSumSlackDPHelper(wordArray, m, W, soln);
					
					if (ret < soln[a]) soln[a] = ret;
									
				}
			
			}
		}
	}
	
	return soln[j];
}

std::string extractDPSolution(std::string* wordArray, int* soln, int n, int W)
{
	
	int* newLineIndex = new int[n];
	std::fill_n(newLineIndex, n, 0);
	
	int a = n;
	
	while(a >= 0)
	{
		int min = INT_MAX;
		int minIndex = INT_MAX;
		int p_j = smallestPossibleIndex(wordArray, a-1, W);
		
		for(int i = a-1; i >= p_j; i--)
		{
			int c = 0;
			for (int j = a-1; j >= i; j--)
			{
				c += wordArray[j].length() + 1;
			}
			c--;
			
			if (((W-c)*(W-c) + soln[i]) < min)
			{
				min = ((W-c)*(W-c) + soln[i]);
				minIndex = i;
			}
		}

		newLineIndex[minIndex-1] = 1;
		if(minIndex == 0) a = -1;
		else a = minIndex;

	}
	
	
	std::stringstream ss;
	for(int w = 0; w < n; w++)
	{
		if(newLineIndex[w] == 1) ss << wordArray[w] << "\n";
		else if (w == n-1) ss << wordArray[w];
		else ss << wordArray[w] << " ";
	}

	return ss.str();

}

int findLeastSumSlack(std::string listOfWords, int W)
//int findLeastSumSlack(std::string* wordArray, int n, int totalLengthOfCharacters, int W)
{
	instance newInstance = createInstance(listOfWords);
	
	std::string* wordArray = newInstance.stringArray;
	int n = newInstance.numberOfWords;
	int totalLengthOfCharacters = newInstance.totalLengthOfCharacters;
	
	// DP needs an n+1 array because solutions are indexed by number of words and needs the 0 solution
	int* solnArray = new int[n+1];
	
	for(int i = 0; i <= n; i++)
	{
		solnArray[i] = INT_MAX;
	}
	
	clock_t start = clock();
	int soln = findLeastSumSlackDPHelper(wordArray, n, W, solnArray);
	clock_t diff = clock() - start;
	std::cout << "dp took:\t\t" << diff << "clock cycles" << std::endl;
	
	std::string solnString = extractDPSolution(wordArray, solnArray, n, W);
	
	
	// memoized version solutions are based on the index of the words
//	int* solnArrayMemo = new int[n];
//	for(int i = 0; i <= n; i++)
//	{
//	     solnArrayMemo[i] = INT_MAX;
//	}

//	clock_t start2 = clock();
//	int soln2 = findLeastSumSlackMemoHelper(wordArray, n-1, totalLengthOfCharacters, W, solnArrayMemo);
//	clock_t diff2 = clock() - start2;
//	std::cout << "memo took:\t\t" << diff2 << "clock cycles" << std::endl;
	

	
	// naive version
//	clock_t start3 = clock();
//	int soln3 = findLeastSumSlackNaiveHelper(wordArray, n-1, totalLengthOfCharacters, W);
//	clock_t diff3 = clock() - start3;
//	std::cout << "naive took:\t\t" << diff3 << "clock cycles" << std::endl;
	
	std::string fiftychars = "12345678901234567890123456789012345678901234567890";
	
	std::cout << fiftychars << std::endl;
	std::cout << solnString << std::endl;
	
	return soln;
	
}


int findLeastSumSlackCustom(std::string* wordArray, int n, int totalLengthOfCharacters, int W)
{
	// DP needs an n+1 array because solutions are indexed by number of words and needs the 0 solution
	int* solnArray = new int[n+1];
	
	for(int i = 0; i <= n; i++)
	{
		solnArray[i] = INT_MAX;
	}
	
	clock_t start = clock();
	int soln = findLeastSumSlackDPHelper(wordArray, n, W, solnArray);
	clock_t diff = clock() - start;
	std::cout << "dp took:\t\t" << diff << "clock cycles" << std::endl;
	
	std::string solnString = extractDPSolution(wordArray, solnArray, n, W);

	std::string fiftychars = "12345678901234567890123456789012345678901234567890";
	
	std::cout << fiftychars << std::endl;
	std::cout << solnString << std::endl;
	
	return soln;
	
}

//int testFindLeastSumSlackWordsOutput()
//{
//	// test for empty word list
//	std::string listOfWords = "";
//	assert(findLeastSumSlack(listOfWords, 3) == "");
	
	
//	// case when W is larger then given input
//	listOfWords = "He l l";
//	assert(findLeastSumSlack(listOfWords, 10) == "He l l");
	

//	// non trivial cases
//	listOfWords = "He l l";
//	assert(findLeastSumSlack(listOfWords, 3) == "He\nl l");
	
	
//	listOfWords = "He l l l";
//	assert(findLeastSumSlack(listOfWords, 4) == "He l\nl l");
	
//	listOfWords = "He l lo l lool";
//	assert(findLeastSumSlack(listOfWords, 4) == "He l\nlo l\nlool");
	
//	return 0;	
//}

int testFindLeastSumSlack()
{
	// test for empty word list
	std::string listOfWords = "";
	assert(findLeastSumSlack(listOfWords, 3) == 0);
      
      
	// case when W is larger then given input
	listOfWords = "He l l";
	assert(findLeastSumSlack(listOfWords, 10) == (10 - 6)*(10 - 6));
      

	// non trivial cases
	assert(findLeastSumSlack(listOfWords, 3) == 1);
      
      
	listOfWords = "He l l l";
	assert(findLeastSumSlack(listOfWords, 4) == 1);
      
	listOfWords = "He l lo l lool";
	assert(findLeastSumSlack(listOfWords, 4) == 0);
      
	return 0;
      
}

int testCreateInstance()
{
	// test for empty word list
	std::string listOfWords = "";
	assert(createInstance(listOfWords).numberOfWords == 0);
	
	
	// test whether createInstance finds correct number of words
	assert(createInstance("Hello world").numberOfWords == 2);
	assert(createInstance("Hello world").lengthOfWords[0] == 5);
	assert(createInstance("Hello world").lengthOfWords[1] == 5);
	

	listOfWords = "He l l";
	assert(createInstance(listOfWords).numberOfWords == 3);
	assert(createInstance(listOfWords).lengthOfWords[0] == 2);
	assert(createInstance(listOfWords).lengthOfWords[1] == 1);
	assert(createInstance(listOfWords).lengthOfWords[2] == 1);
	assert(createInstance(listOfWords).totalLengthOfCharacters == 6);
	
	listOfWords = "He l lo l lool";
	assert(createInstance(listOfWords).numberOfWords == 5);
	assert(createInstance(listOfWords).lengthOfWords[0] == 2);
	assert(createInstance(listOfWords).lengthOfWords[1] == 1);
	assert(createInstance(listOfWords).lengthOfWords[2] == 2);
	assert(createInstance(listOfWords).lengthOfWords[3] == 1);
	assert(createInstance(listOfWords).lengthOfWords[4] == 4);
	assert(createInstance(listOfWords).totalLengthOfCharacters == 14);
	
	return 0;
	
}

int testSmallestPossibleIndex()
{
	std::string testWordArray [] = { "abc", "a", "c", "b", "abc" };
	assert(smallestPossibleIndex(testWordArray, 0, 3) == 0);
	assert(smallestPossibleIndex(testWordArray, 4, 3) == 4);
	assert(smallestPossibleIndex(testWordArray, 3, 3) == 2);
	assert(smallestPossibleIndex(testWordArray, 2, 3) == 1);

	return 0;
}

int main(int argc, char* argv[])
{
	testCreateInstance();
	testSmallestPossibleIndex();
	testFindLeastSumSlack();
	//testFindLeastSumSlackWordsOutput();
	
	
	int n = std::atoi(argv[1]);
	std::ifstream infile;

	infile.open(std::to_string(n)+"_american.txt");
	std::string* wordArray = new std::string[n];

	std::string line;
	int lineNum = 0;
	int totalLengthOfCharacters = 0;

	while(std::getline(infile, line))
	{
		wordArray[lineNum] = line;
		lineNum++;
		totalLengthOfCharacters += line.length() + 1;
	}
	totalLengthOfCharacters--;

	std::cout << totalLengthOfCharacters << std::endl;

	findLeastSumSlackCustom(wordArray, n, totalLengthOfCharacters, 50);


	return 0;
}
