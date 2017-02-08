#include "compare.h"
#include <stdio.h>

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
int score3(char a, char b, char c) {
	return score(a, b) + score(b, c) + score(a, c);
}

//choose the maximum from int array and its position
pair<int, int> max(int* choice, int length) {
	int ans = choice[0];
	int position = 0;
	for (int i = 1; i < length; i++) {
		if (choice[i] > ans) {
			ans = choice[i];
			position = i;
		}
	}
	return make_pair(ans, position);
}