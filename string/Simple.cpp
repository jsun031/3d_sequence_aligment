//This is the short C++ version.
#include "compare.h"
#include <vector>
//construct the 3 string intervals alignment when the length of interval c is no more than 1.
int ali1(const StringInterval& A, const StringInterval& B, const StringInterval& C, string& aAli, string& bAli, string& cAli) {
	//A.size() + 1 layers of B.size() * C.size() memory is enough. Each element has its score and its previous postion
	vector<vector<vector<vector<int>>>> s_mx32(A.size() + 1, vector<vector<vector<int>>>(B.size() + 1, vector<vector<int>>(C.size() + 1, vector<int>(4, 0))));
	for (int i = 0; i < A.size() + 1; i++) {
		if (i > 0) {
			s_mx32[1][0][0][0] = s_mx32[0][0][0][0] + 2 * InD;
		}
		for (int j = 0; j < B.size() + 1; j++) {
			for (int k = 0; k < C.size() + 1; k++) {
				if (i == 0 && j == 0 && k == 0) {
					continue;
				}
				int choice[7];
				for (int l = 0; l < 7; l++) {
					choice[l] = INT_MIN; 
				}
				if (i > 0) {
					choice[0] = s_mx32[i - 1][j][k][0] + 2 * InD;//sc_col
					if (j > 0) {
						choice[1] = s_mx32[i - 1][j - 1][k][0] + 2 * InD + score(A.get(i - 1, true), B.get(j - 1, true));//sc_fd_cr
						if (k > 0) {
							choice[2] = s_mx32[i - 1][j - 1][k - 1][0] + score3(A.get(i - 1, true), B.get(j - 1, true), C.get(k - 1, true));//sc_sd
						}
					}
					if (k > 0) {
						choice[3] = s_mx32[i - 1][j][k - 1][0] + 2 * InD + score(A.get(i - 1, true), C.get(k - 1, true));//sc_fd_rh
					}
				}
				if (j > 0) {
					choice[4] = s_mx32[i][j - 1][k][0] + 2 * InD;//sc_row
					if (k > 0) {
						choice[5] = s_mx32[i][j - 1][k - 1][0] + 2 * InD + score(B.get(j - 1, true), C.get(k - 1, true));//sc_fd_ch
					}
				}
				if (k > 0) {
					choice[6] = s_mx32[i][j][k - 1][0] + 2 * InD;//sc_hei	
				}
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
	int ans = s_mx32[i][j][k][0];
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
	return ans;
}

//dynamically calculate the possible alignment score for 3 string intervals
vector<vector<int>> ali(const StringInterval& A, const StringInterval& B, const StringInterval& C, bool mark) {
	//2 layers of B.size() * C.size() memory is enough
	vector<vector<vector<int>>> s_mx32(2, vector<vector<int>>(B.size() + 1, vector<int>(C.size() + 1, 0)));
	for (int i = 0; i < A.size() + 1; i++) {
		if (i > 0) {
			s_mx32[1][0][0] = s_mx32[0][0][0] + 2 * InD;
		}
		for (int j = 0; j < B.size() + 1; j++) {
			for (int k = 0; k < C.size() + 1; k++) {
				if (i == 0 && j == 0 && k == 0) {
					continue;
				}
				int choice[7];
				for (int l = 0; l < 7; l++) {
					choice[l] = INT_MIN; 
				}
				if (i > 0) {
					choice[0] = s_mx32[0][j][k] + 2 * InD;//sc_col
					if (j > 0) {
						choice[1] = s_mx32[0][j - 1][k] + 2 * InD + score(A.get(i - 1, mark), B.get(j - 1, mark));//sc_fd_cr
						if (k > 0) {
							choice[2] = s_mx32[0][j - 1][k - 1] + score3(A.get(i - 1, mark), B.get(j - 1, mark), C.get(k - 1, mark));//sc_sd
						}
					}
					if (k > 0) {
						choice[3] = s_mx32[0][j][k - 1] + 2 * InD + score(A.get(i - 1, mark), C.get(k - 1, mark));//sc_fd_rh
					}
				}
				if (j > 0) {
					choice[4] = s_mx32[1][j - 1][k] + 2 * InD;//sc_row
					if (k > 0) {
						choice[5] = s_mx32[1][j - 1][k - 1] + 2 * InD + score(B.get(j - 1, mark), C.get(k - 1, mark));//sc_fd_ch
					}
				}
				if (k > 0) {
					choice[6] = s_mx32[1][j][k - 1] + 2 * InD;//sc_hei
				}
				s_mx32[1][j][k] = max(choice, 7).first;
			}
		}
		s_mx32[0] = s_mx32[1];
	}
	return s_mx32[1];
}