//This is the optimized C++ version.
#include "compare.h"
#include <iostream>
#include <time.h>
#include <vector>
//construct the 3 string intervals alignment when the length of interval c is no more than 1.
void ali1(const StringInterval& A, const StringInterval& B, const StringInterval& C, string& aAli, string& bAli, string& cAli) {
	//A.size() + 1 layers of B.size() * C.size() memory is enough. Each element has its score and its previous postion
	vector<vector<vector<vector<int>>>> s_mx32(A.size() + 1, vector<vector<vector<int>>>(B.size() + 1, vector<vector<int>>(C.size() + 1, vector<int>(4, 0))));

	//initialize the bottom of 3d matrix
	if (A.size() == 1) {
		s_mx32[1][0][0][0] = 2 * InD;
	}
	for (int i = 1; i < C.size() + 1; i++) {
		s_mx32[0][0][i][0] = s_mx32[0][0][i - 1][0] + 2 * InD;
		s_mx32[0][0][i][3] = i - 1;
	}
	for (int i = 1; i < B.size() + 1; i++) {
		s_mx32[0][i][0][0] = s_mx32[0][i - 1][0][0] + 2 * InD;
		s_mx32[0][i][0][2] = i - 1;
	}
	for (int j = 1; j < B.size() + 1; j++) {
		for (int k = 1; k < C.size() + 1; k++) {
			int choice[3];
			choice[0] = s_mx32[0][j - 1][k][0] + 2 * InD;//sc_row
			choice[1] = s_mx32[0][j - 1][k - 1][0] + 2 * InD + score(B.get(j - 1, true), C.get(k - 1, true));//sc_fd_ch
			choice[2] = s_mx32[0][j][k - 1][0] + 2 * InD;//sc_hei
			vector<vector<int>> position;
			position.push_back(vector<int>{0, j - 1, k});
			position.push_back(vector<int>{0, j - 1, k - 1});
			position.push_back(vector<int>{0, j, k - 1});
			pair<int, int> mx = max(choice, 3);
			s_mx32[0][j][k][0] = mx.first;
			for (int l = 0; l < 3; l++) {
				s_mx32[0][j][k][l + 1] = position[mx.second][l];
			}
		}
	}

	for (int i = 1; i < A.size() + 1; i++) {
		for (int j = 1; j < C.size() + 1; j++) {
			int choice[3];
			choice[0] = s_mx32[1][0][j - 1][0] + 2 * InD;//sc_row
			choice[1] = s_mx32[0][0][j - 1][0] + 2 * InD + score(A.get(i - 1, true), C.get(j - 1, true));//sc_fd_ch
			choice[2] = s_mx32[0][0][j][0] + 2 * InD;//sc_hei
			int position[3][3] = {
				{i, 0, j - 1},
				{i - 1, 0, j - 1},
				{i - 1, 0, j}
			};
			pair<int, int> mx = max(choice, 3);
			s_mx32[i][0][j][0] = mx.first;
			for (int l = 0; l < 3; l++) {
				s_mx32[i][0][j][l + 1] = position[mx.second][l];
			}
		}
		for (int j = 1; j < B.size() + 1; j++) {
			int choice[3];
			choice[0] = s_mx32[i][j - 1][0][0] + 2 * InD;
			choice[1] = s_mx32[i - 1][j - 1][0][0] + 2 * InD + score(A.get(i - 1, true), B.get(j - 1, true));
			choice[2] = s_mx32[i - 1][j][0][0] + 2 * InD;
			int position[3][3] = {
				{i, j - 1, 0},
				{i - 1, j - 1, 0},
				{i - 1, j, 0}
			};
			pair<int, int> mx = max(choice, 3);
			s_mx32[i][j][0][0] = mx.first;
			for (int l = 0; l < 3; l++) {
				s_mx32[i][j][0][l + 1] = position[mx.second][l];
			}
		}
		for (int j = 1; j < B.size() + 1; j++) {
			for (int k = 1; k < C.size() + 1; k++) {
				int choice[7];
				choice[0] = s_mx32[i - 1][j][k][0] + 2 * InD;//sc_col
				choice[1] = s_mx32[i - 1][j - 1][k][0] + 2 * InD + score(A.get(i - 1, true), B.get(j - 1, true));//sc_fd_cr
				choice[2] = s_mx32[i - 1][j - 1][k - 1][0] + score3(A.get(i - 1, true), B.get(j - 1, true), C.get(k - 1, true));//sc_sd
				choice[3] = s_mx32[i - 1][j][k - 1][0] + 2 * InD + score(A.get(i - 1, true), C.get(k - 1, true));//sc_fd_rh
				choice[4] = s_mx32[i][j - 1][k][0] + 2 * InD;//sc_row
				choice[5] = s_mx32[i][j - 1][k - 1][0] + 2 * InD + score(B.get(j - 1, true), C.get(k - 1, true));//sc_fd_ch
				choice[6] = s_mx32[i][j][k - 1][0] + 2 * InD;//sc_hei	
				int position[7][3] = {
					{i - 1, j, k},
					{i - 1, j - 1, k},
					{i - 1, j - 1, k - 1},
					{i - 1, j, k - 1},
					{i, j - 1, k},
					{i, j - 1, k - 1},
					{i, j, k - 1}
				};
				pair<int, int> mx = max(choice, 7);
				s_mx32[i][j][k][0] = mx.first;
				for (int l = 0; l < 3; l++) {
					s_mx32[i][j][k][l + 1] = position[mx.second][l];
				}
			}
		}
	}
	int i = A.size();
	int j = B.size();
	int k = C.size();
	string a, b, c;
	while (i > 0 || j > 0 || k > 0) {
		a += A.alignment(i, s_mx32[i][j][k][1]);
		b += B.alignment(j, s_mx32[i][j][k][2]);
		c += C.alignment(k, s_mx32[i][j][k][3]);
		int i_next = s_mx32[i][j][k][1];
		int j_next = s_mx32[i][j][k][2];
		int k_next = s_mx32[i][j][k][3];
		i = i_next;
		j = j_next;
		k = k_next;
	}

	reverse(a.begin(), a.end());
	reverse(b.begin(), b.end());
	reverse(c.begin(), c.end());
	aAli += a;
	bAli += b;
	cAli += c;
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

	for (int j = 1; j < B.size() + 1; j++) {
		for (int k = 1; k < C.size() + 1; k++) {
			choice2[0] = s_mx32[0][j - 1][k] + 2 * InD;//sc_row
			choice2[1] = s_mx32[0][j - 1][k - 1] + 2 * InD + score(B.get(j - 1, mark), C.get(k - 1, mark));//sc_fd_ch
			choice2[2] = s_mx32[0][j][k - 1] + 2 * InD;//sc_hei		        	

			s_mx32[0][j][k] = max(choice2, 3).first;
		}
	}

	for (int i = 1; i < A.size() + 1; i++) {
		s_mx32[1][0][0] = s_mx32[0][0][0] + 2 * InD;
		for (int j = 1; j < C.size() + 1; j++) {
			mx2 = INT_MIN;
			choice2[0] = s_mx32[1][0][j - 1] + 2 * InD;//sc_row
			choice2[1] = s_mx32[0][0][j - 1] + 2 * InD + score(A.get(i - 1, mark), C.get(j - 1, mark));//sc_fd_ch
			choice2[2] = s_mx32[0][0][j] + 2 * InD;//sc_hei

			s_mx32[1][0][j] = max(choice2, 3).first;
		}
		for (int j = 1; j < B.size() + 1; j++) {
			choice2[0] = s_mx32[1][j - 1][0] + 2 * InD;
			choice2[1] = s_mx32[0][j - 1][0] + 2 * InD + score(A.get(i - 1, mark), B.get(j - 1, mark));
			choice2[2] = s_mx32[0][j][0] + 2 * InD;

			s_mx32[1][j][0] = max(choice2, 3).first;
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

				s_mx32[1][j][k] = max(choice2, 7).first;
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
		//break B and C according to optimal value and construct the alignment separately
		StringInterval B1(B, make_pair(B.getBegin(), B.getBegin() + mark_b - 1));
		StringInterval C1(C, make_pair(C.getBegin(), C.getBegin() + mark_c - 1));
		//find_alignment3(A1, B1, C1, str_a, str_b, str_c);
		if (A1.size() >= B1.size() && A1.size() >= C1.size()) {
			find_alignment3(A1, B1, C1, str_a, str_b, str_c);
		} else if (B1.size() >= A1.size() && B1.size() >= C1.size()) {
			find_alignment3(B1, A1, C1, str_b, str_a, str_c);
		} else {
			find_alignment3(C1, B1, A1, str_c, str_b, str_a);
		}
		StringInterval B2(B, make_pair(B.getBegin() + mark_b, B.getEnd()));
		StringInterval C2(C, make_pair(C.getBegin() + mark_c, C.getEnd()));
		//find_alignment3(A2, B2, C2, str_a, str_b, str_c);
		if (A2.size() >= B2.size() && A2.size() >= C2.size()) {
			find_alignment3(A2, B2, C2, str_a, str_b, str_c);
		} else if (B2.size() >= A2.size() && B2.size() >= C2.size()) {
			find_alignment3(B2, A2, C2, str_b, str_a, str_c);
		} else {
			find_alignment3(C2, B2, A2, str_c, str_b, str_a);
		}
		return mx;
	} else if (A.size() == 1 || A.size() == 0) {
		ali1(A, B, C, str_a, str_b, str_c);
		return 0;
	} else if (B.size() == 1 || B.size() == 0) {
		ali1(B, C, A, str_b, str_c, str_a);
		return 0;
	} else {
		ali1(C, B, A, str_c, str_b, str_a);
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
	cout << "It tooks me " << ((float)t) / (float)CLOCKS_PER_SEC << " seconds." << endl;
	return 0;
}