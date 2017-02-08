#include <string>
#include <utility>
#include <algorithm>
#ifndef INT_MIN
#define INT_MIN -2147483648
#endif
using namespace std;
//This is the main data structure. String pointer is used to avoid unnecessary copy. 
//The range is to indicate which interval is currently focus on
//This class is final, because it does not need to change after initialization.
class StringInterval final {
	const string* content;
	const string* reverse_content;
	int length;
	pair<int, int> range;
public:
	int size() const;
	int getBegin() const;
	int getEnd() const;
	StringInterval();
	StringInterval(const string&);
	StringInterval(const StringInterval&);
	StringInterval(const StringInterval&, const pair<int, int>&);
	char get(const int position, const bool mark) const;
	char alignment(const int i, const int position) const;
};