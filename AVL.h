
// This is the AVL tree template class. It takes the word that gets inserted from main 
// and places it in the tree, while calculating the balance factor and rebalancing the tree
// if needed. It then checks if a user inputed string is located in the tree.
#include<ctime>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <stddef.h>
using namespace std;


template <class eType>
class Node
{
public:
    eType data;
    Node* left;
    Node* right;
	vector<int> locations;

    Node(int loc, eType D = 0, Node* L = NULL, Node* R = NULL){
		locations.push_back(loc);
        data= D;
        left=L;
        right=R;
    }
};


template <class eType>
class AVL
{
protected:
    Node<eType>* root;
    
    int getHeight(Node<eType>* e);
	bool isLeftHeavy(Node<eType>*& e);
    bool isBalanced(Node<eType>*& e);
	void reBalance(Node<eType>*& e);
    
    void singleLeft(Node<eType>*& e);
    void singleRight(Node<eType>*& e);
    void doubleLeft(Node<eType>*& e);
    void doubleRight(Node<eType>*& e);

    void insert(eType& data, Node<eType>*& e, int location);
    eType removeSuccessor(Node<eType>*& e);
    void remove (int value, Node<eType>*& e);
    void printTree(Node<eType>* e);
	void find(Node<eType>*& e, string words);
	vector<int> findWords(Node<eType>*& e, string word);
	void findPrint(vector<int>& documentLocations, string words);
	vector<int> findAND(vector<int>& one, vector<int>& two);
	vector<int> findOR(vector<int>& one, vector<int>& two);
	vector<int> recursiveFind(vector<vector<int>> x);
    
public:
    AVL( ) {root= NULL;}
    void insert(eType data, int location) {insert(data, root, location);}
    void remove(int value) {remove(value, root);}
    void printTree() {printTree(root);}
	void find(string words){ find(root, words);}
};


// this function computes and returns the height of e
// Complexity is n because every node is checked while calculating the height
template <class eType>
int AVL<eType>:: getHeight(Node<eType>* e){
	if(e == NULL)
		return 0;
	if(e->left == NULL && e->right == NULL)
		return 1;
	int leftHeight = 0;
	int rightHeight = 0;
	if(e->left != NULL)
		leftHeight = getHeight(e->left) + 1;
	if(e->right != NULL)
		rightHeight = getHeight(e->right) + 1;
	if(rightHeight > leftHeight)
		return rightHeight;
	else
		return leftHeight;
}

// this function should determine whether the tree is balanced, then
// return a true for a balanced tree, a false for an unbalanced tree
// Complexity is n squared, it checks all the children nodes under itself and it calls the get height function each time which is already n
template <class eType>
bool AVL<eType>::isBalanced( Node<eType>*& e )
{
	if(e == NULL)
		return true;
	if(e->left == NULL && e->right == NULL)		//if it has no children it is balanced, so return true
		return true;

	int leftChild = 0;			//Initial values for children set at 0
	int rightChild = 0;

	if(e->left != NULL)					//If it has a child, find out the height of it
		leftChild = getHeight(e->left);
	if(e->right != NULL)
		rightChild = getHeight(e->right);

	int difference = leftChild - rightChild;		//Find the difference of the two children
	if(difference < -1 || difference > 1)
		return false;								//If the difference is too big, return false
	else if (isBalanced(e->left) && isBalanced(e->right))		//if the left and right children are also balanced, return true
		return true;
	else											//Else the children are not balanced so return false
		return false;
}

// this function returns a boolean for if the node e is left heavy
// if the left depth is larger than the right, then it is left heavy
// and the function should return true
// Complexity is n because it calls getHeight and basically checks every node once
template <class eType>
bool AVL<eType>::isLeftHeavy( Node<eType>*& e )
{
	int leftHeight = getHeight(e->left);
	int rightHeight = getHeight(e->right);
	if(leftHeight > rightHeight)
		return true;
	else
		return false;
}


// this function determines which imbalance the tree has
// then calls the appropriate rotation to fix it
// Complexity is Constant because it makes no recursive calls and has no loops
template <class eType>
void AVL<eType>::reBalance( Node<eType>*& e )
{
	if(isLeftHeavy(e) && isLeftHeavy(e->left))			//single right fixes a left left imbalance
		singleRight(e);
	else if(isLeftHeavy(e) && !isLeftHeavy(e->left))	//Double right fixes a left right imbalance
		doubleRight(e);
	else if(!isLeftHeavy(e) && isLeftHeavy(e->right))	//Double left fixes a right left imbalance
		doubleLeft(e);
	else												//single left fixes a right right imbalance
		singleLeft(e);
	
}


// A single left rotation at e
// Complexity is Constant because it makes no recursive calls and has no loops
template <class eType>
void AVL<eType>::singleLeft( Node<eType>*& e )
{
	Node<eType>* temp = e;
	Node<eType>* tempLeft = e->right->left;
	e = e->right;
	e->left = temp;
	e->left->right = tempLeft;
}
    

// A single right rotation at e
// Complexity is Constant because it makes no recursive calls and has no loops
template <class eType>
void AVL<eType>::singleRight( Node<eType>*& e )
{
	Node<eType>* temp = e;
	Node<eType>* tempRight = e->left->right;
	e = e->left;
	e->right = temp;
	e->right->left = tempRight;
	
}


// A double left rotation at e
// Complexity is Constant because it makes no recursive calls and has no loops
template <class eType>
void AVL<eType>::doubleLeft( Node<eType>*& e )
{
	Node<eType>* temp = e;
	Node<eType>* tempRight = e->right;
	Node<eType>* tempRightChild = e->right->left->right;
	Node<eType>* tempLeftChild = e->right->left->left;
	e = e->right->left;
	e->left = temp;
	e->left->right = tempLeftChild;
	e->right = tempRight;
	e->right->left = tempRightChild;
}


//A double right rotation at e
// Complexity is Constant because it makes no recursive calls and has no loops
template <class eType>
void AVL<eType>::doubleRight( Node<eType>*& e )
{
	Node<eType>* temp = e;
	Node<eType>* tempLeft = e->left;
	Node<eType>* tempLeftChild = e->left->right->left;
	Node<eType>* tempRightChild = e->left->right->right;
	e = e->left->right;
	e->right = temp;
	e->right->left = tempRightChild;
	e->left = tempLeft;
	e->left->right = tempLeftChild;

}


// this function does an insert into the avl tree, then it determines
// on the way out if an imbalance was caused.  If one was, then it should
// call the rebalance function to fix the tree
// Complexity is O nlog(n). log(n) because half of the tree is sorted out each recursive call 
// until the spot where it will be inserted is found, but it also calls isBalanced which is n.
template <class eType>
void AVL<eType>::insert(eType& data, Node<eType>*& e, int location )
{
	if(e == NULL)									//If you're in a NULL spot create the new node with the given value right there
        e = new Node<eType>(location,data, NULL, NULL);

    if(data < e->data)							//If the new data is smaller insert it to the left
		insert(data, e->left, location);
    if(data > e->data)
		insert(data, e->right, location);					//If its bigger insert it to the right

	if (data == e->data){									//If data = data in node
		for(size_t i = 0; i < e->locations.size(); i++){	//check to see if the document location is already counted for
			if(location == e->locations[i])
				return;
		}
		e->locations.push_back(location);
		return;
	}

	if(!isBalanced(e))								//If this node isn't balanced, call the rebalance function passing this node to it
		reBalance(e);
	return;
}


// RemoveSuccessor finds the successor by going all the way left, then
// removing the successor and returning the value.  It is used by the delete function
// Complexity is log(n) because the successor value is found by traversing down one line of the tree the first time
template <class eType>
eType AVL<eType>::removeSuccessor( Node<eType>*& e )
{
	if(e==NULL)
		return NULL;
	if(e->left)
		return removeSuccessor(e->left);
	else{
		eType tempValue = e->data;
		Node<eType>* temp = e;
		e=e->right;
		delete temp;
		return tempValue;
	}
}


// it performs the delete (utilizing the removeSuccessor function), then
// checks for an imbalances on the way back out, calling the reBalance function to fix the tree
// Complexity is (n)log(n) because the call to isBalanced is n and this function traverses through 
// smaller portions of the tree every call resulting in log(n)
template <class eType>
void AVL<eType>::remove(int value, Node<eType>*& e){
	if(e == NULL)
		return;
	if(value == e->data){
        if(e->right != NULL)  //if there is a right child, there is a successor, so get it and delete it
            e->data = removeSuccessor(e->right); //deletes successor and returns the value
        else{ //otherwise, replace e with the left tree and delete it
            Node<eType>* temp = e;
            e = e->left;
            delete temp;
        }
	}
	else if(value < e->data)
		remove(value, e->left);
	else
		remove(value, e->right);
    
	if(!isBalanced(e))
        reBalance(e);
}


// this function prints the entire tree to the console for viewing
// Complexity is n because every node is printed out one time
template <class eType>
void AVL<eType>::printTree( Node<eType>* e)
{  
    if (e==NULL) return;
    printTree(e->left);
    cout << e->data << "   ";
	for(size_t i = 0; i < e->locations.size(); i++){
		cout << e->locations[i] <<" ";
	}
	cout << endl;
    printTree(e->right);
}

//The find function searches through the tree looking for a matching word, if found the locations are printed
//Complexity is O(log n), because half of the results are eliminated on every call of the function.
template <class eType>
void AVL<eType>::find(Node<eType>*& e, string words){
	//Break up the string of searched words into a new vector with all the search words in their own indexes.
	vector<string> wordsVector;
    string buffer;
    stringstream ss(words);
	while (ss >> buffer)
        wordsVector.push_back(buffer);

	vector<vector<int>> allTheLocations;

	for(size_t i = 0; i < wordsVector.size(); i++){
		allTheLocations.push_back(findWords(e, wordsVector[i]));
	}
	findPrint(recursiveFind(allTheLocations), words);
}

//This method recursively combines vectors of integers starting with the outside comparisons and moving in towards the middle.
template <class eType>
vector<int> AVL<eType>::recursiveFind(vector<vector<int>> x){
	int size = x.size();
	if(size == 0 || size == 2 || size == 4 || size == 5 || size == 6){
		vector<int> empty;
		return empty;
	}
	if(size == 1)
		return x[0];
	if(size < 4){
		if(x[1][0] == -1)
			return findAND(x[0], x[2]);
		if(x[1][0] == -2)
			return findOR(x[0], x[2]);
		else{
			vector<int> empty;
			return empty;
		}
	}
	int mid = size/2;
	vector<vector<int>> y;
	for(int i = 0; i < mid; i++){
		y.push_back(x[i]);
	}
	vector<vector<int>> z;
	for(int i = mid+1; i < size; i++){
		z.push_back(x[i]);
	}
	if(x[mid][0] == -1){
		return findAND(recursiveFind(y), recursiveFind(z));
	}
	if(x[mid][0] == -2){
		return findOR(recursiveFind(y), recursiveFind(z));
	}
	else{
		vector<int> empty;
		return empty;
	}
}

//This function takes two vectors<int>, creates a vector of numbers that were contained in both original vectors, and returns the answer.
template <class eType>
vector<int> AVL<eType>::findAND(vector<int>& one, vector<int>& two){
	vector<int> answer;
	for(size_t x = 0; x < one.size(); x++){
		for(size_t y = 0; y < two.size(); y++){
			if(one[x] == two[y])
				answer.push_back(one[x]);
		}
	}
	return answer;
}

//This function takes two vectors<int>, combines them into one vector without adding any duplicate entries, and returns the answer.
template <class eType>
vector<int> AVL<eType>::findOR(vector<int>& one, vector<int>& two){
	vector<int> answer;
	for(size_t x = 0; x < one.size();x++){
		answer.push_back(one[x]);
	}
	for(size_t x = 0; x < two.size(); x++){
		if(std::find(one.begin(), one.end(), two[x]) != one.end()) {} 
		else {
			answer.push_back(two[x]);
		}
	}
	return answer;
}

//findPrint takes a vector<int> and prints out the numbers if any are found.
template <class eType>
void AVL<eType>::findPrint(vector<int>& documentLocations, string words){
	if(!documentLocations.empty()){
		sort(documentLocations.begin(), documentLocations.end()); //sort the results
		cout <<"\"" << words <<"\"" <<" is found in documents:";
		for(size_t i = 0; i < documentLocations.size(); i++){
			cout << " " << documentLocations[i];
		}
		cout <<endl;
	} 
	else {
		cout <<"Your search didn't return any results." <<endl;
	}	
}

//This method searches through the tree recursively searching for one word. The vector of documents that word is stored in is returned.
template <class eType>
vector<int> AVL<eType>::findWords(Node<eType>*& e, string word){
	//if(e==NULL || word == "OR"||word == "AND"){
	if(word=="AND"){
		vector<int> andVect;
		andVect.push_back(-1);
		return andVect;
	}
	if(word=="OR"){
		vector<int> orVect;
		orVect.push_back(-2);
		return orVect;
	}
	if(e==NULL){
		vector<int> empty;
		return empty;
	}
	if(e->data == word){
		return e->locations;
	}
	else if(word < e->data)
		return findWords(e->left, word);
	else if(word > e->data)
		return findWords(e->right, word);
}