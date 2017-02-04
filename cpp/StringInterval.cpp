#include "StringInterval.h"
//construct an empty StringInterval
StringInterval::StringInterval() {
	length = 0;
}
//construct a defalt StringInterval with content point to string a
//reverse_content is the reverse of a
//range from a.begin() to a.end();
StringInterval::StringInterval(const string& a) {
	string* ar = new string(a);
	reverse(ar->begin(), ar->end());
	content = &a;
	reverse_content = ar;
	range.first = 0;
	range.second = a.size() - 1;
	length = range.second - range.first + 1;
}
StringInterval::StringInterval(const StringInterval& S){
	content = S.content;
	reverse_content = S.reverse_content;
	length = S.length;
	range = S.range;
}
//given an Interval t select part of it
StringInterval::StringInterval(const StringInterval& t, const pair<int, int>& b) {
	content = t.content;
	reverse_content = t.reverse_content;
	range.first = b.first;
	range.second = b.second;
	length = range.second - range.first + 1;
}
//the range size
int StringInterval::size() const {
	return length;
}
//begin of interval
int StringInterval::getBegin() const {
	return range.first;
}
//end of interval
int StringInterval::getEnd() const {
	return range.second;
}
//return one paricular value
char StringInterval::get(const int position, const bool mark) const {
	if (mark) {
		return content->at(range.first + position);
	} else {
		return reverse_content->at(content->size() - 1 - range.second + position);
	}
}
//give 2 postions, decide return '_' or the (*content)[i]
char StringInterval::alignment(const int i, const int position) const{
	char a;
	if (i > 0 && position + 1 == i) {
		a = get(i - 1, true);
	}
	else {
		a = '_';
	}
	return a;
}