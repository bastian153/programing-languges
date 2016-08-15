#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <iterator>
#include <regex>
#include <algorithm>
#include <cctype>
#include <map>
#include <queue>
#include <locale>
using namespace std;

class words {
	vector<string> stopWords;
	map<string, int> fileWords;

public:
	void createStopVector(ifstream& stopWordReader);
	void createFileWordsMapCount(ifstream& fileWordsReader);
	bool validWord(string& temp);
	void getTopTF();
	int getRank(string& str);
};

class mycomparison
{
	bool reverse;
public:
	mycomparison(const bool& revparam = false)
	{
		reverse = revparam;
	}
	bool operator() (const pair<string, int>& lhs, const pair<string, int>&rhs) const
	{
		if (reverse) return (lhs.second>rhs.second);
		else return (lhs.second<rhs.second);
	}
};


void words::createStopVector(ifstream& stopWordReader)
{
	string temp;
	string temp2;
	getline(stopWordReader, temp);
	
	regex r("\\w+");
	regex_token_iterator<string::iterator> rit(temp.begin(), temp.end(), r);
	regex_token_iterator<string::iterator> rend;
	while (rit != rend) {
		temp2 = rit->str();
		for (int i = 0; i < temp2.size(); ++i) {
			temp2[i] = tolower(temp2[i]);
		}
		stopWords.push_back(temp2);
		++rit;
	}
}

bool words::validWord(string& temp)
{
	for (auto& word : stopWords)
	{
		if (temp == word)
			return false;
	}
	return true;
}

void words::createFileWordsMapCount(ifstream& fileWordsReader)
{
	string temp;
	string temp2;
	while (getline(fileWordsReader, temp))
	{
		
		for (int i = 0; i < temp.size(); ++i) {
			temp[i] = tolower(temp[i]);
		}
		regex r("\[a-z]{2,}");
		regex_token_iterator<string::iterator> rit(temp.begin(), temp.end(), r);
		regex_token_iterator<string::iterator> rend;
		while (rit != rend) {
			temp2 = rit->str();
			if (validWord(temp2)) 
			{
				if (fileWords.find(temp2) != fileWords.end()) //exist
					fileWords[temp2] = fileWords[temp2] + 1;
				else
					fileWords[temp2] = 1;
			}
			
			++rit;
		}
	}
	cout << "map size = " << fileWords.size()<<endl;
}

int fact(int n)
{
    return (n <= 1)? 1 :n * fact(n-1);
}
 
int findSmallerInRight(string& str, int low, int high)
{
    int countRight = 0, i;
 
    for (i = low+1; i <= high; ++i)
        if (str[i] < str[low])
            ++countRight;
 
    return countRight;
}
 
int words::getRank(string& str)
{
    int len = str.size();
    int mul = fact(len);
    int rank = 1;
    int countRight;
 
    int i;
    for (i = 0; i < len; ++i)
    {
        mul /= len - i;
 
        countRight = findSmallerInRight(str, i, len-1);
 
        rank += countRight * mul ;
    }
 
    return rank;
}


void words::getTopTF()
{
	priority_queue<pair<string, int>, vector<pair<string, int>>, mycomparison> wordCountQ (mycomparison(false));
	for (auto element : fileWords)
	{
		wordCountQ.emplace(element);
	}
	for (int i = 0; i < 25; ++i)
	{
		if (wordCountQ.empty())
			break;
		string curWord = wordCountQ.top().first;
		cout << curWord << "\t-\t" << wordCountQ.top().second << "\t-\t" << getRank(curWord) << endl;
		wordCountQ.pop();
	}
}

int main(int argc, char** argv)
{
	if(argc != 2){
		cout << "wrong number of inputs"<<endl;
		exit(101);
	}
	
	ifstream wordStopFile("/home/ubuntu/workspace/stop_words.txt");
	ifstream fileReader(argv[1]);
	words MostWord;

	if (!wordStopFile.is_open())
		exit(101);
	if (!fileReader.is_open())
		exit(101);
	MostWord.createStopVector(wordStopFile);
	MostWord.createFileWordsMapCount(fileReader);
	MostWord.getTopTF();

	return 0;
}
