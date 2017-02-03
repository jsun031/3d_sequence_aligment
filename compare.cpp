/* --- Compare 3 sequences to calculate the best score and construct the best alignment for 3 string respectively. 
Pairwisely compare the 3 sequences to get the result. To compare 2 string:
If there is an insert or delete, add -8. If there is a mismatch, add -4. If there is a mactch, add 5.*/
#include <string>
#include <utility>
#include <iostream>
#include <time.h>
#include <vector>
#include <algorithm>
#define InD -8
#define Mis -4
#define MATCH 5
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
	int size() const {
		return length;
	}
	int getBegin() const {
		return range.first;
	}
	int getEnd() const {
		return range.second;
	}
	StringInterval() {
		length = 0;
	}
	StringInterval(const string& a) {
		string* ar = new string(a);
		reverse(ar->begin(), ar->end());
		content = &a;
		reverse_content = ar;
		range.first = 0;
		range.second = a.size() - 1;
		length = range.second - range.first + 1;
	}
	StringInterval(const StringInterval& t, const pair<int, int>& b) {
		content = t.content;
		reverse_content = t.reverse_content;
		range.first = b.first;
		range.second = b.second;
		length = range.second - range.first + 1;
	}
	char get(const int position, const bool mark) const {
		if (mark) {
			return content->at(range.first + position);
		} else {
			return reverse_content->at(content->size() - 1 - range.second + position);
		}
	}
};

//read sequence from files. The files are fasta documents with only the sequence after removing its first line.
string input(char* doc) {//input data from a file
	char data;
	FILE* fp = fopen(doc, "r");
	if (!fp) {
		printf("can't open file\n");
		return string();
	}
	int i = 0;
	string a;
	while (!feof(fp)) {
		fscanf(fp, "%c", &data);
		if (data != '\n' && (data == 'A' || data == 'C' || data == 'G' || data == 'T')) {
			a += data;
			i++;
		}
	}
	fclose(fp);
	return a;
}

//compare two positions and caculte the score
int score(char a, char b) {
	char cha[] = { '_', 'A', 'T', 'C', 'G' };
	if (a == '_' && b == '_') {
		return 0;
	} else if (a == '_' || b == '_') {
		return InD;
	} else if (a == b) {
		return MATCH;
	} else {
		return Mis;
	}
}
//compare 3 positions by comparison pairwisely
inline int score3(char a, char b, char c) {
	return score(a, b) + score(b, c) + score(a, c);
}
//choose the maximum from int array 
int max(int* choice, int length) {
	int ans = INT_MIN;
	for (int i = 0; i < length; i++) {
		if (choice[i] > ans) {
			ans = choice[i];
		}
	}
	return ans;
}
//given 2 string intervals, construct the alignment with the best score
pair<string, string> find_pair_aligment(const StringInterval& A, const StringInterval& B) {
	vector<vector<vector<int> > > s_mx(A.size() + 1, vector<vector<int>>(B.size() + 1, vector<int>(3, 0)));
	for (int i = 1; i < A.size() + 1; i++) {
		s_mx[i][0][0] = s_mx[i - 1][0][0] + InD;
		s_mx[i][0][1] = i - 1;
		s_mx[i][0][2] = 0;
	}
	for (int i = 1; i < B.size() + 1; i++) {
		s_mx[0][i][0] = s_mx[0][i - 1][0] + InD;
		s_mx[0][i][1] = 0;
		s_mx[0][i][2] = i - 1;
	}
	int choice2[3];
	for (int i = 1; i < A.size() + 1; i++) {
		for (int j = 1; j < B.size() + 1; j++) {
			vector<pair<int, int>> pos;
			pos.push_back(make_pair(i, j - 1));
			pos.push_back(make_pair(i - 1, j - 1));
			pos.push_back(make_pair(i - 1, j));
			choice2[0] = s_mx[i][j - 1][0] + InD;//sc_row
			choice2[1] = s_mx[i - 1][j - 1][0] + score(B.get(j - 1, true), A.get(i - 1, true));//sc_fd_ch
			choice2[2] = s_mx[i - 1][j][0] + InD;//sc_hei		        	
			int mx = choice2[0];
			pair<int, int> position = make_pair(i, j - 1);
			for (int i = 1; i < 3; i++) {
				if (choice2[i] > mx) {
					mx = choice2[i];
					position = pos[i];
				}
			}
			s_mx[i][j][0] = mx;
			s_mx[i][j][1] = position.first;
			s_mx[i][j][2] = position.second;
		}
	}
	string a;
	string b;

	int i = A.size();
	int j = B.size();
	while (i > 0 || j > 0) {
		if (i > 0 && s_mx[i][j][1] + 1 == i) {
			a += A.get(i - 1, true);
		} else {
			a += '_';
		}
		if (j > 0 && s_mx[i][j][2] + 1 == j) {
			b += B.get(j - 1, true);
		} else {
			b += '_';
		}
		int i_next = s_mx[i][j][1];
		int j_next = s_mx[i][j][2];
		i = i_next;
		j = j_next;
	}
	reverse(a.begin(), a.end());
	reverse(b.begin(), b.end());
	return make_pair(a, b);
}
//construct the 3 string intervals alignment when the length of interval c is no more than 1.
void ali0(const StringInterval& A, const StringInterval& B, const StringInterval& C, string& a, string& b, string& c) {
	pair<string, string> store = find_pair_aligment(A, B);
	//cout <<"begin, store: "<< store.first<< ", " << store.second << endl;
	string c_tmp(store.first.size(), '_');
	if (C.size() == 1) {
		int mx = INT_MIN;
		int mark;
		for (int i = 0; i < store.first.size(); i++) {
			int t = score3(store.first[i], store.second[i], C.get(0, true)) - score3(store.first[i], store.second[i], '_');
			if (mx < t) {
				mx = t;
				mark = i;
			}
		}
		if (mx < 2 * InD) {
			store.first += '_';
			store.second += '_';
			c_tmp += c[0];
		} else {
			c_tmp[mark] = C.get(0, true);
		}
	}

	//cout <<"after, store: "<< store.first<< ", " << store.second << ", "<< c_tmp<< endl;
	//cout <<"before, a = " << a << ", b = " << b << ", c = " << c <<endl;
	a += store.first;
	b += store.second;
	c += c_tmp;
	//cout <<"after, a = " << a << ", b = " << b << ", c = " << c <<endl;
}
//dynamically calculate the possible alignment score for 3 string intervals
vector<vector<int>> ali(const StringInterval& A, const StringInterval& B, const StringInterval& C, bool mark) {
	//2 layers of B.size() * C.size() memory is enough
	vector<vector<vector<int>>> s_mx32(2, vector<vector<int>>(B.size() + 1, vector<int>(C.size() + 1, 0)));
	int choice2[7];
	int mx2 = INT_MIN;
	//initialize the bottom of 3d matrix
	s_mx32[0][0][0] = 0;
	s_mx32[1][0][0] = 2 * InD;
	for (int i = 1; i < C.size() + 1; i++) {
		s_mx32[0][0][i] = s_mx32[0][0][i - 1] + 2 * InD;
	}
	for (int i = 1; i < B.size() + 1; i++) {
		s_mx32[0][i][0] = s_mx32[0][i - 1][0] + 2 * InD;
	}
	char c = B.get(1, mark);
	for (int j = 1; j < B.size() + 1; j++) {
		for (int k = 1; k < C.size() + 1; k++) {
			choice2[0] = s_mx32[0][j - 1][k] + 2 * InD;//sc_row
			choice2[1] = s_mx32[0][j - 1][k - 1] + 2 * InD + score(B.get(j - 1, mark), C.get(k - 1, mark));//sc_fd_ch
			choice2[2] = s_mx32[0][j][k - 1] + 2 * InD;//sc_hei		        	

			s_mx32[0][j][k] = max(choice2, 3);
		}
	}

	int tmp = 0;
	for (int i = 1; i < A.size() + 1; i++) {
		for (int j = 1; j < C.size() + 1; j++) {
			mx2 = INT_MIN;
			choice2[0] = s_mx32[1][0][j - 1] + 2 * InD;//sc_row
			choice2[1] = s_mx32[0][0][j - 1] + 2 * InD + score(A.get(i - 1, mark), C.get(j - 1, mark));//sc_fd_ch
			choice2[2] = s_mx32[0][0][j] + 2 * InD;//sc_hei

			s_mx32[1][0][j] = max(choice2, 3);
		}
		for (int j = 1; j < B.size() + 1; j++) {
			choice2[0] = s_mx32[1][j - 1][0] + 2 * InD;
			choice2[1] = s_mx32[0][j - 1][0] + 2 * InD + score(A.get(i - 1, mark), B.get(j - 1, mark));
			choice2[2] = s_mx32[0][j][0] + 2 * InD;

			s_mx32[1][j][0] = max(choice2, 3);
		}
		for (int j = 1; j < B.size() + 1; j++) {
			for (int k = 1; k < C.size() + 1; k++) {
				choice2[5] = s_mx32[0][j][k] + 2 * InD;//sc_col
				choice2[1] = s_mx32[0][j - 1][k] + 2 * InD + score(A.get(i - 1, mark), B.get(j - 1, mark));//sc_fd_cr
				choice2[0] = s_mx32[0][j - 1][k - 1] + score3(A.get(i - 1, mark), B.get(j - 1, mark), C.get(k - 1, mark));//sc_sd
				choice2[3] = s_mx32[0][j][k - 1] + 2 * InD + score(A.get(i - 1, mark), C.get(k - 1, mark));//sc_fd_rh
				choice2[4] = s_mx32[1][j - 1][k] + 2 * InD;//sc_row
				choice2[2] = s_mx32[1][j - 1][k - 1] + 2 * InD + score(B.get(j - 1, mark), C.get(k - 1, mark));//sc_fd_ch
				choice2[6] = s_mx32[1][j][k - 1] + 2 * InD;//sc_hei		        	

				s_mx32[1][j][k] = max(choice2, 7);
			}
		}
		s_mx32[0] = s_mx32[1];
	}
	return s_mx32[1];
}

//find best alignment for 3 string intervals
int find_alignment3(const StringInterval& A, const StringInterval& B, const StringInterval& C, string& str_a, string& str_b, string& str_c) {
	if (A.size() > 1 && B.size() > 1 && C.size() > 1) {
		//break the interval A from middle into 2 parts to find the best thresholds to break interval B and C. It needs log(A.size()) extra times iteration.
		//This uses only B.size() * C.size() memory, while construct the best alignment directly needs A.size() * B.size() * C.size() memory
		int breakpoint = (A.size() - 1) / 2 + A.getBegin();
		StringInterval A1(A, make_pair(A.getBegin(), breakpoint));
		vector<vector<int>> sp_mx = ali(A1, B, C, true);
		StringInterval A2(A, make_pair(breakpoint + 1, A.getEnd()));
		vector<vector<int>> sb_mx = ali(A2, B, C, false);
		int mx = INT_MIN;
		int tmp_sm = 0;
		int mark_b = 0;
		int mark_c = 0;
		for (int i = 0; i < B.size() + 1; i++) {
			for (int j = 0; j < C.size() + 1; j++) {
				tmp_sm = sp_mx[i][j] + sb_mx[B.size() - i][C.size() - j];
				if (mx < tmp_sm) {
					mx = tmp_sm;
					mark_b = i;
					mark_c = j;
				}
			}
		}
		sp_mx.clear();
		sb_mx.clear();
		/*cout << "A: " << A.getBegin() << ", " << breakpoint << ", " << A.getEnd() << endl;
		cout << "B: " << B.getBegin() << ", " << B.getBegin() + mark_b - 1 << ", " << B.getEnd() << endl;
		cout << "C: " << C.getBegin() << ", " << C.getBegin() + mark_c - 1 << ", " << C.getEnd() << endl;*/
		//break B and C according to optimal value and construct the alignment separately
		StringInterval B1(B, make_pair(B.getBegin(), B.getBegin() + mark_b - 1));
		StringInterval C1(C, make_pair(C.getBegin(), C.getBegin() + mark_c - 1));
		string str_a1, str_a2, str_b1, str_b2, str_c1, str_c2;
		if (A1.size() >= B1.size() && A1.size() >= C1.size()) {
			find_alignment3(A1, B1, C1, str_a1, str_b1, str_c1);
		} else if (B1.size() >= A1.size() && B1.size() >= C1.size()) {
			find_alignment3(B1, A1, C1, str_b1, str_a1, str_c1);
		} else {
			find_alignment3(C1, B1, A1, str_c1, str_b1, str_a1);
		}

		//cout << str_a << endl << str_b << endl << str_c << endl;
		StringInterval B2(B, make_pair(B.getBegin() + mark_b, B.getEnd()));
		StringInterval C2(C, make_pair(C.getBegin() + mark_c, C.getEnd()));
		//find_alignment3(A2, B2, C2, str_a, str_b, str_c);
		if (A2.size() >= B2.size() && A2.size() >= C2.size()) {
			find_alignment3(A2, B2, C2, str_a2, str_b2, str_c2);
		} else if (B2.size() >= A2.size() && B2.size() >= C2.size()) {
			find_alignment3(B2, A2, C2, str_b2, str_a2, str_c2);
		} else {
			find_alignment3(C2, B2, A2, str_c2, str_b2, str_a2);
		}
		str_a += str_a1 + str_a2;
		str_b += str_b1 + str_b2;
		str_c += str_c1 + str_c2;
		//cout << str_a << endl << str_b << endl << str_c << endl;
		return mx;
	} else if (A.size() == 1 || A.size() == 0) {
		ali0(B, C, A, str_b, str_c, str_a);
		//cout << "Size1: str_a = "<< str_a << ", str_b = " << str_b << ", str_c = " << str_c << endl;
		return 0;
	} else if (B.size() == 1 || B.size() == 0) {
		ali0(A, C, B, str_a, str_c, str_b);
		return 0;
	} else {
		ali0(A, B, C, str_a, str_b, str_c);
		return 0; 
	}
}

int main() {
	clock_t t;
	t = clock();
	char doc1[] = "NM_000558.fasta";
	char doc2[] = "NM_008218.fasta";
	char doc3[] = "NM_013096.fasta";
	const string a = input(doc1); //"CATAAACCCTGGCGCGCTCGCGGCCCGGCACTCTTCTGGTCCCCACAGACTCAGAGAGAACCCACCATGG";//
	StringInterval A(a);
	cout << doc1 << " open correctly! length is " << a.size() << "." << endl;
	const string b = input(doc2); //"GACACTTCTGATTCTGACAGACTCAGGAAGAAACCATGGTGCTCTCTGGGGAAGACAAAAGCAACATCAA";//
	StringInterval B(b);
	cout << doc2 << " open correctly! length is " << b.size() << "." << endl;
	const string c = input(doc3); //"ACATTCTCCTTCTGATAGACTCAGGAAGCAATCATGGTGCTCTCTGCAGATGACAAAACCAACATCAAGA";//
	StringInterval C(c);
	cout << doc3 << " open correctly! length is " << c.size() << "." << endl;
	string str_a, str_b, str_c;
	int total_score;
	if (A.size() >= B.size() && A.size() >= C.size()) {
		total_score = find_alignment3(A, B, C, str_a, str_b, str_c);
	} else if (B.size() >= A.size() && B.size() >= C.size()) {
		total_score = find_alignment3(B, A, C, str_b, str_a, str_c);
	} else {
		total_score = find_alignment3(C, B, A, str_c, str_b, str_a);
	}
	cout << "Total score is " << total_score << "." << endl;
	//cout << "Original lengths of sequences a, b, c are " << A.size() << ", " << B.size() << ", and " << C.size() << endl;
	cout << "Lengths of alignment result sequences a, b, c are " << str_a.size() << ", " << str_b.size() << ", and " << str_c.size() << ". " << endl;
	int perfect_match = 0;
	for (int i = 0; i < str_a.size(); i++) {
		if (str_a[i] == str_b[i] && str_b[i] == str_c[i]) {
			perfect_match++;
		}
	}
	cout << "Perfect match number is " << perfect_match << "." << endl;
	cout << str_a << endl << str_b << endl << str_c << endl;
	t = clock() - t;
	cout << "It tooks me %d clicks (" << ((float)t) / (float)CLOCKS_PER_SEC << " seconds)." << endl;
	return 0;
}