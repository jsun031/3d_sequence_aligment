#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define InD -8
#define Mis -4
#define MATCH 5
#define Inf -2147483647
using namespace std;

inline void reverse_arr(char *a, int n,  char *b) {
	for (int i = 0; i<n; i++)
	{
		b[i] = a[n - i - 1];
	}
}
//main data structure use char* to save space
struct StringInterval {
	int length;
	char* content;
	char* reverse_content;
	int begin;
	int end;
	int arraylength;//This is designed to get the value of certain position in reverse string 
	StringInterval(char* a, char* ar, int b, int c) {
		content = a;
		reverse_content = ar;
		begin = b;
		end = c;
		arraylength = c + 1;
		length = c - b + 1;
	}
	StringInterval(const StringInterval& A, int b, int c) {
		content = A.content;
		reverse_content = A.reverse_content;
		begin = b;
		end = c;
		arraylength = A.arraylength;
		length = c - b + 1;
	}
	int size() const {
		return length;
	}
	char get(const int position, const bool mark) const {
		if (mark) {
			return content[position + begin];
		} else {
			return reverse_content[arraylength - 1 - end + position];
		}
	}
	char alignment(const int i, const int position) const {
		char a;
		if (i > 0 && position + 1 == i) {
			a = get(i - 1, true);
		} else {
			a = '_';
		}
		return a;
	}
};
//caculate memory size for the input data
int cnt(char* doc) {
	char data;
	FILE* fp = fopen(doc, "r");
	if (!fp) {
		printf("can't open file\n");
		return -1;
	}
	int i = 0;
	while (!feof(fp)) {
		fscanf(fp, "%c", &data);
		if (data != '\n' && (data == 'A' || data == 'C' || data == 'G' || data == 'T')) {
			i++;
		}
	}
	fclose(fp);
	return i;
}
//input data from a file
int input(char* doc, char* a) {//input data from a file
	char data;
	FILE* fp = fopen(doc, "r");
	if (!fp) {
		printf("can't open file\n");
		return -1;
	}
	int i = 0;
	while (!feof(fp)) {
		fscanf(fp, "%c", &data);
		if (data != '\n' && (data == 'A' || data == 'C' || data == 'G' || data == 'T')) {
			a[i] = data;
			i++;
		}
	}
	fclose(fp);
	return i;
}
//for error check and show result only
void print(char* a, int length) {
	for (int i = 0; i < length; i++) {
		printf("%c", a[i]);
	}
	printf("\n");
}
//get pair alignment score
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
//get 3 position alignment score
inline int score3(char a, char b, char c) {
	return score(a, b) + score(b, c) + score(a, c);
}
//construct the alignment initially when A.size() <= 1
int multi_align0(const StringInterval& A, const StringInterval& B, const StringInterval& C, char*& a_ali, char*& b_ali, char*& c_ali, int& length_ali) {
	int**** s_mx;
	s_mx = (int****)malloc((2) * sizeof(int***));
	for (int i = 0; i < 2; i++) {
		s_mx[i] = (int***)malloc((B.size() + 1) * sizeof(int**));
		for (int j = 0; j < B.size() + 1; j++) {
			s_mx[i][j] = (int**)malloc((C.size() + 1) * sizeof(int*));
			for (int k = 0; k < C.size() + 1; k++) {
				s_mx[i][j][k] = (int*)malloc(4 * sizeof(int));
			}
		}
	}
	//i == 1 indicate it is current layer and i == 0 indicate it is last layer
	s_mx[1][0][0][0] = 0;

	for (int i = 0; i < A.size() + 1; i++) {
		if (i > 0) {
			s_mx[1][0][0][0] = s_mx[0][0][0][0] + 2 * InD;
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
					choice[0] = s_mx[0][j][k][0] + 2 * InD;//sc_col
					if (j > 0) {
						choice[1] = s_mx[0][j - 1][k][0] + 2 * InD + score(A.get(i - 1, true), B.get(j - 1, true));//sc_fd_cr
						if (k > 0) {
							choice[2] = s_mx[0][j - 1][k - 1][0] + score3(A.get(i - 1, true), B.get(j - 1, true), C.get(k - 1, true));//sc_sd
						}
					}
					if (k > 0) {
						choice[3] = s_mx[0][j][k - 1][0] + 2 * InD + score(A.get(i - 1, true), C.get(k - 1, true));//sc_fd_rh
					}
				}
				if (j > 0) {
					choice[4] = s_mx[1][j - 1][k][0] + 2 * InD;//sc_row
					if (k > 0) {
						choice[5] = s_mx[1][j - 1][k - 1][0] + 2 * InD + score(B.get(j - 1, true), C.get(k - 1, true));//sc_fd_ch
					}
				}
				if (k > 0) {
					choice[6] = s_mx[1][j][k - 1][0] + 2 * InD;//sc_hei		        	
				}
				int mx = INT_MIN;
				int mark = -1;
				int position[7][3] = {
						{ i - 1, j, k },
						{ i - 1, j - 1, k },
						{ i - 1, j - 1, k - 1 },
						{ i - 1, j, k - 1 },
						{ i, j - 1, k },
						{ i, j - 1, k - 1 },
						{ i, j, k - 1 }
				};
				for (int l = 0; l < 7; l++) {
					if (choice[l] > mx) {
						mx = choice[l];
						mark = l;
					}
				}
				s_mx[1][j][k][0] = mx;
				for (int l = 1; l < 4; l++) {
					s_mx[1][j][k][l] = position[mark][l - 1];
				}
			}
		}
		for (int j = 0; j < B.size() + 1; j++) {
			for (int k = 0; k < C.size() + 1; k++) {
				for (int l = 0; l < 4; l++) {
					s_mx[0][j][k][l] = s_mx[1][j][k][l];
				}
			}
		}
	}
	int i = A.size();
	int j = B.size();
	int k = C.size();
	int score = s_mx[i][j][k][0];
	char* a = (char*)malloc(sizeof(char) * (A.size() + B.size() + C.size()));
	char* b = (char*)malloc(sizeof(char) * (A.size() + B.size() + C.size()));
	char* c = (char*)malloc(sizeof(char) * (A.size() + B.size() + C.size()));
	int length = 0;
	while (i > 0 || j > 0 || k > 0) {
		a[length] = A.alignment(i, s_mx[i][j][k][1]);
		b[length] = B.alignment(j, s_mx[i][j][k][2]);
		c[length] = C.alignment(k, s_mx[i][j][k][3]);
		int i_next = s_mx[i][j][k][1];
		int j_next = s_mx[i][j][k][2];
		int k_next = s_mx[i][j][k][3];
		i = i_next;
		j = j_next;
		k = k_next;
		length++;
	}
	for (int i = 0; i < length; i++) {
		*a_ali = a[length - 1 - i];
		a_ali++;
		*b_ali = b[length - 1 - i];
		b_ali++;
		*c_ali = c[length - 1 - i];
		c_ali++;
	}
	length_ali += length;
	free(a);
	free(b);
	free(c);
	for (int i = 0; i < A.size() + 1; i++) {
		for (int j = 0; j < B.size() + 1; j++) {
			for (int k = 0; k < C.size() + 1; k++) {
				free(s_mx[i][j][k]);
			}
			free(s_mx[i][j]);
		}
		free(s_mx[i]);
	}
	free(s_mx);
	return score;
}
//calculate the score matrix
void muti_ali(const StringInterval& A, const StringInterval& B, const StringInterval& C, int** s_mx, bool mark) {
	int*** s_mx32;
	s_mx32 = (int***)malloc(2 * sizeof(int**));
	for (int i = 0; i < 2; i++) {
		s_mx32[i] = (int**)malloc((B.size() + 1) * sizeof(int*));
		for (int j = 0; j < B.size() + 1; j++) {
			s_mx32[i][j] = (int*)malloc((C.size() + 1) * sizeof(int));
		}
	}
	//i == 1 indicate it is current layer and i == 0 indicate it is last layer
	s_mx32[1][0][0] = 0;
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
				int mx = INT_MIN;
				for (int l = 0; l < 7; l++) {
					if (choice[l] > mx) {
						mx = choice[l];
					}
				}
				s_mx32[1][j][k] = mx;
			}
		}
		//in the next iteration, current layer becomes next layer
		for (int j = 0; j < B.size() + 1; j++) {
			for (int k = 0; k < C.size() + 1; k++) {
				s_mx32[0][j][k] = s_mx32[1][j][k];
			}
		}
	}
	//copy the value needed
	for (int j = 0; j < B.size() + 1; j++) {
		for (int k = 0; k < C.size() + 1; k++) {
			s_mx[j][k] = s_mx32[1][j][k];
		}
	}
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < B.size() + 1; j++) {
			free(s_mx32[i][j]);
		}
	}
	for (int i = 0; i < 2; i++) {
		free(s_mx32[i]);
	}
	free(s_mx32);
}

int find_aligment3(const StringInterval& A, const StringInterval& B, const StringInterval& C, char*& a, char*& b, char*& c, int& length) {
	if (A.size() > 1 && B.size() > 1 && C.size() > 1) {
		int breakpoint = (A.size() - 1) / 2 + A.begin;
		int** sp_mx;
		sp_mx = (int**)malloc((B.size() + 1) * sizeof(int*));
		for (int i = 0; i < B.size() + 1; i++) {
			sp_mx[i] = (int*)malloc((C.size() + 1) * sizeof(int));
		}
		const StringInterval A1(A, A.begin, breakpoint);
		muti_ali(A1, B, C, sp_mx, true);
		const StringInterval A2(A, breakpoint + 1, A.end);
		int** sb_mx;
		sb_mx = (int**)malloc((B.size() + 1) * sizeof(int*));
		for (int i = 0; i < B.size() + 1; i++) {
			sb_mx[i] = (int*)malloc((C.size() + 1) * sizeof(int));
		}
		muti_ali(A2, B, C, sb_mx, false);
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
		for (int i = 0; i < B.size() + 1; i++) {
			free(sp_mx[i]);
			free(sb_mx[i]);
		}
		free(sp_mx);
		free(sb_mx);
		const StringInterval B1(B, B.begin, B.begin + mark_b - 1);
		const StringInterval B2(B, B.begin + mark_b, B.end);
		const StringInterval C1(C, C.begin, C.begin + mark_c - 1);
		const StringInterval C2(C, C.begin + mark_c, C.end);
		find_aligment3(A1, B1, C1, a, b, c, length);
		find_aligment3(A2, B2, C2, a, b, c, length);
		return mx;
	} else if (A.size() <= 1) {
		return multi_align0(A, B, C, a, b, c, length);
	} else if (C.size() <= 1) {
		return multi_align0(C, B, A, c, b, a, length);
	} else {//if (length_b ==  1||length_b ==  0)
		return multi_align0(B, A, C, b, a, c, length);
	}
}

int main()
{
	clock_t t;
	t = clock();
	char doc1[] = "NM_000558.fasta";
	char doc2[] = "NM_008218.fasta";
	char doc3[] = "NM_013096.fasta";
	int length_a = cnt(doc1);
	char* a;
	a = (char*)malloc(length_a * sizeof(char));
	input(doc1, a);
	printf("%s open correctly! length is %d\n", doc1, length_a);
	int length_b = cnt(doc2);
	char* b;
	b = (char*)malloc(length_b * sizeof(char));
	input(doc2, b);
	printf("%s open correctly! length is %d\n", doc2, length_b);
	int length_c = cnt(doc3);
	char* c;
	c = (char*)malloc(length_c * sizeof(char));
	input(doc3, c);
	printf("%s open correctly! length is %d\n", doc3, length_c); 
	char* ar = (char*)malloc(length_a * sizeof(char));
	char* br = (char*)malloc(length_b * sizeof(char));
	char* cr = (char*)malloc(length_c * sizeof(char));
	reverse_arr(a, length_a, ar);
	reverse_arr(b, length_b, br);
	reverse_arr(c, length_c, cr);
	const StringInterval A(a, ar, 0, length_a - 1);
	const StringInterval B(b, br, 0, length_b - 1);
	const StringInterval C(c, cr, 0, length_c - 1);
	char* a_ali = (char*)malloc((A.size() + B.size() + C.size()) * sizeof(char));
	char* b_ali = (char*)malloc((A.size() + B.size() + C.size()) * sizeof(char));
	char* c_ali = (char*)malloc((A.size() + B.size() + C.size()) * sizeof(char));
	char* a_head = a_ali;
	char* b_head = b_ali;
	char* c_head = c_ali;
	int length = 0;
	int total_score = find_aligment3(A, B, C, a_ali, b_ali, c_ali, length);
	printf("\nTotal score is %d.\n", total_score);
	printf("Length of alignment result sequence of a, b, c is %d.\n", length);
	int perfect_match = 0;
	for (int i = 0; i < length; i++)
	{
		if (a_head[i] == b_head[i] && b_head[i] == c_head[i])
		{
			perfect_match++;
		}
	}
	printf("Perfect match number is %d.\n", perfect_match);
	print(a_head, length);
	print(b_head, length);
	print(c_head, length);
	free(a_head);
	free(b_head);
	free(c_head);
	t = clock() - t;
	printf("It tooks me %d clicks (%lf seconds).\n", t, ((float)t) / (float)CLOCKS_PER_SEC);
	return 0;
}