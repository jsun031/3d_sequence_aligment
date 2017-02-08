/* --- Compare 3 sequences to calculate the best score and construct the best alignment for 3 string respectively. 
Pairwisely compare the 3 sequences to get the result. To compare 2 string:
If there is an insert or delete, add -8. If there is a mismatch, add -4. If there is a mactch, add 5.*/
#include <string>
#include <utility>
#include <iostream>
#include <time.h>
#include <vector>
#include <algorithm>
#include "StringInterval.h"
#define InD -8
#define Mis -4
#define MATCH 5
using namespace std;

//read sequence from files. The files are fasta documents with only the sequence after removing its first line.
string input(char*);

//compare two positions and caculte the score
int score(char, char);
//compare 3 positions by comparison pairwisely
int score3(char, char, char);
//choose the maximum from int array and its position
pair<int, int> max(int*, int);

//construct the 3 string intervals alignment when the length of interval c is no more than 1.
int ali1(const StringInterval&, const StringInterval&, const StringInterval&, string&, string&, string&);

//dynamically calculate the best alignment score for 3 string intervals
vector<vector<int>> ali(const StringInterval&, const StringInterval&, const StringInterval&, bool);

//find best alignment for 3 string intervals
int find_alignment3(const StringInterval&, const StringInterval&, const StringInterval&, string&, string&, string&);