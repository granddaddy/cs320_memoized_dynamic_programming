#include <iostream>
#include <assert.h>
#include <climits>

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
	while ((sum + wordArray[j-1].length() + 1) <= W && (j - 1) >= 0) {
		sum += wordArray[j-1].length() + 1;
		index--;		
	}
	return index;
}

int findLeastSumSlackNaiveHelper(std::string* wordArray, int j, int c, int W)
{

	// base case
	if(j < 0) return 0;
	assert(c > 0);
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
		assert(slack >= 0);
		
		int ret = (slack * slack) + findLeastSumSlackNaiveHelper(wordArray, m - 1, c - lineWeight - 1, W);
		
		if (ret < min) min = ret;
	}
	
	return min;

}

int findLeastSumSlackMemoHelper(std::string* wordArray, int j, int c, int W, int* soln)
{

	// base case
	if(j < 0) return soln[0] = 0;
	assert(c > 0);
	if(c <= W) return soln[j] = (W - c)*(W - c);
	
	
	int p_j = smallestPossibleIndex(wordArray, j, W);
	
	
	for (int m = j; m >= p_j; m--) {
		int lineWeight = 0;
		for (int n = j; n >= m; n--) {
			lineWeight += wordArray[n].length() + 1;
		}
		lineWeight--;
		int slack = W - lineWeight;
		assert(slack >= 0);
		
		int ret;
		
		if(soln[m-1] != INT_MAX) ret = (slack * slack) + soln[m-1];
		
		else ret = (slack * slack) + findLeastSumSlackMemoHelper(wordArray, m - 1, c - lineWeight - 1, W, soln);
		
		if (ret < soln[j]) soln[j] = ret;
	}
	
	return soln[j];

}

int findLeastSumSlackDPHelper(std::string* wordArray, int j, int c, int W)
{	
	int* solnArray = new int[j+1]; 
	for (int i = 0; i < j; i++) solnArray[i] = INT_MAX;
	
	
		
	
	for (int m = -1; m <= j; m++) {
		
		if(j < 0) return soln[0] = 0;;
		if(c <= W) return soln[j] = (W - c)*(W - c);
		
		int p_j = smallestPossibleIndex(wordArray, m, W);
		
		for(int n = i; n >= p_j; n--) {
			int lineWeight = 0;
			for (int n = j; n >= m; n--) {
				lineWeight += wordArray[n].length() + 1;
			}
			lineWeight--;
			
		}
		
		
	}
	delete solnArray;
}

int findLeastSumSlack(std::string listOfWords, int W)
{
	instance newInstance = createInstance(listOfWords);
	
	std::string* wordArray = newInstance.stringArray;
	int n = newInstance.numberOfWords;
	int totalLengthOfCharacters = newInstance.totalLengthOfCharacters;
	
	int* solnArray = new int[n];
	
	for(int i = 0; i < n; i++)
	{
		solnArray[i] = INT_MAX;
	}
	
	int soln = findLeastSumSlackMemoHelper(wordArray, n-1, totalLengthOfCharacters, W, solnArray);
	
	delete solnArray;
	delete newInstance.stringArray;
	delete newInstance.lengthOfWords;
	
	
	// int soln = findLeastSumSlackNaiveHelper(wordArray, n-1, totalLengthOfCharacters, W);
	
	// delete newInstance.stringArray;
	// delete newInstance.lengthOfWords;
	
	
	return soln;
	
}


int testNaive()
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
	
	listOfWords = "He l lo l lool He l lo l lool He l lo l lool";
	listOfWords = "He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool He l lo l lool";
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
}

int main()
{
	testCreateInstance();
	testSmallestPossibleIndex();
	testNaive();
}