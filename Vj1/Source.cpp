#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

#define MAXL 1024;

int countNumOfLines(const char*);

int main() {

	int numOfLines = countNumOfLines(".txt");

	return 0;
}

int countNumOfLines(const char* fileName) {
	FILE* fp = NULL;
	int numOfLines = 0;
	char buffer[MAXL] = "asdad";
	
	fp = fopen("Studenti.txt", "r");

	if (!fp) {
		return -1;
	}

	while (!feof(fp)) {
		fgets(buffer);
		numOfLines++;
	}

	return 0;
}