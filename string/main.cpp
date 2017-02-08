#include <iostream>
#include <time.h>
#include <vector>
#include "compare.h"

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
				//cout << "sp_mx[" << i << "][" << j << "]=" << sp_mx[i][j] << ", sb_mx[" << B.size() - i << "][" << C.size() - j << "]=" << sb_mx[B.size() - i][C.size() - j] << endl;
				if (mx < tmp_sm) {
					mx = tmp_sm;
					mark_b = i;
					mark_c = j;
				}
			}
		}
		//cout<<mark_b <<" "<< mark_c<<endl;
		sp_mx.clear();
		sb_mx.clear();
		//break B and C according to optimal value and construct the alignment separately
		StringInterval B1(B, make_pair(B.getBegin(), B.getBegin() + mark_b - 1));
		StringInterval C1(C, make_pair(C.getBegin(), C.getBegin() + mark_c - 1));
		if (A1.size() >= B1.size() && A1.size() >= C1.size()) {
			find_alignment3(A1, B1, C1, str_a, str_b, str_c);
		} else if (B1.size() >= A1.size() && B1.size() >= C1.size()) {
			find_alignment3(B1, A1, C1, str_b, str_a, str_c);
		} else {
			find_alignment3(C1, B1, A1, str_c, str_b, str_a);
		}
		StringInterval B2(B, make_pair(B.getBegin() + mark_b, B.getEnd()));
		StringInterval C2(C, make_pair(C.getBegin() + mark_c, C.getEnd()));
		if (A2.size() >= B2.size() && A2.size() >= C2.size()) {
			find_alignment3(A2, B2, C2, str_a, str_b, str_c);
		} else if (B2.size() >= A2.size() && B2.size() >= C2.size()) {
			find_alignment3(B2, A2, C2, str_b, str_a, str_c);
		} else {
			find_alignment3(C2, B2, A2, str_c, str_b, str_a);
		}
		return mx;
	} else if (A.size() == 1 || A.size() == 0) {
		return ali1(A, B, C, str_a, str_b, str_c);
	} else if (B.size() == 1 || B.size() == 0) {
		return ali1(B, C, A, str_b, str_c, str_a);
	} else {
		return ali1(C, B, A, str_c, str_b, str_a); 
	}
}

int main() {
	clock_t t;
	t = clock();
	//This is the 3 documents files with the 3 sequences to align.
	char doc1[] = "NM_000558.fasta";
	char doc2[] = "NM_008218.fasta";
	char doc3[] = "NM_013096.fasta";
	//Load the 3 documents for input
	const string a = input(doc1);
	StringInterval A(a);
	cout << doc1 << " open correctly! length is " << a.size() << "." << endl;
	const string b = input(doc2);
	StringInterval B(b);
	cout << doc2 << " open correctly! length is " << b.size() << "." << endl;
	const string c = input(doc3);
	StringInterval C(c);
	cout << doc3 << " open correctly! length is " << c.size() << "." << endl;
	string str_a, str_b, str_c;//The string to store the result alignment
	int total_score;
	if (A.size() >= B.size() && A.size() >= C.size()) {
		total_score = find_alignment3(A, B, C, str_a, str_b, str_c);
	} else if (B.size() >= A.size() && B.size() >= C.size()) {
		total_score = find_alignment3(B, A, C, str_b, str_a, str_c);
	} else {
		total_score = find_alignment3(C, B, A, str_c, str_b, str_a);
	}
	cout << "Total score is " << total_score << "." << endl;
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
	cout << "It tooks me " << ((float)t) / (float)CLOCKS_PER_SEC << " seconds." << endl;
	return 0;
}