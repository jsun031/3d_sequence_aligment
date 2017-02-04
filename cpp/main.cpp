#include <iostream>
#include <time.h>
#include "compare.h"

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