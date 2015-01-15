
// This is the main file for our mini-search program. It opens a file located in the 
// project directory called stopwords.txt, it then opens each of the 50 documents 
// called cranfield00XX ,x stands for the number of the document, it then inserts
// each word into the avl tree located in the header AVL.h. It then prompts the user
// to enter a word to find and then reports the document number the entered word or words
// it was found in
#include<ctime>
#include<vector>
#include<fstream>
#include<sstream>
#include <iterator>
#include "AVL.h"
#include <algorithm>
using namespace std;

int main()
{
	AVL<string> tree;
	vector<string> stopwords;
	bool match = false;
	string word,line;
	string common = "documents/cranfield000";

	ifstream input;
	
	ifstream input2("stopwords.txt");
	
	std::clock_t duration;

	if(!input2)
	{
		cout<<"Failed to open stop word file"<<endl;
	}

	duration = clock();									//Start clock for stop word timer

	while(getline(input2,line))				            //get the stop word and push it back into the vector of stop words
	{                                                    
		stopwords.push_back(line);
	}
	duration = clock()-duration;
	cout<<"\n Stopword seconds: "<<((long double)(duration/CLOCKS_PER_SEC))<<endl; //report the time in seconds it took to insert all the stop words

	duration = clock();								     //Start clock to time insert
	for(int number = 1; number <=50; number++){          //loop 50 times while concatenating the count to the name of the file

		if(number > 9){
			common = "documents/cranfield00";
		}
		stringstream ss;
		ss << common << number;
		string filename = ss.str();

		input.open(filename);
		if (input.fail()){
			cout << "Failed to open document" << number <<endl;
			cin.get();
			return 0;
		}

		ostringstream convert;
		convert << number;
		string documentNumber = convert.str();
		
		while (!input.eof()){
			input >> word;
			for(int i = 0; i<stopwords.size(); ++i)
			{
				if(word == stopwords[i])
				{
					word.erase();
				}

			}
			if(word[0] !='<' && word !="\0") //If word starts with a < then dont insert it.
			{
				word.erase(remove(word.begin(),word.end(),','), word.end()); //if the word contans a "," at the end of the word, erase it.
																			 // So the word like "curves," will show a match when the user searches for "curves"
				word.erase(remove(word.begin(),word.end(),'.'), word.end()); 
				tree.insert(word, number);
			}
		}
		input.close();
	}												
	duration = clock()-duration;
	cout<<"\n Inserting Documents seconds "<<((long double)(duration/CLOCKS_PER_SEC))<<endl;
	//tree.printTree();															//debuging function
	int queryCount = 0;												
	
	while(1){
		
		cout <<"Please enter a word to find: " <<endl;
		string userInput;
		
		getline(cin,userInput);	
	
		cout<<"------------Query "<<++queryCount<<"------------"<<endl;   
		duration = clock();											//Start clock to time search
		
		tree.find(userInput);
		
		duration = clock()-duration;
		cout<<"Query elapsed time: "<<(((long float)duration/CLOCKS_PER_SEC))<<endl;
		cout<<endl;
		
	}

	cin.get();
	return 0;
	
}
