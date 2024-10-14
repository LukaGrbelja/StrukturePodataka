#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

#define MAX 1024;

int NumberOfRows(const char*);
void DataDisplay(int, const char*);

typedef struct{
    char ime[20] = {};
    char prezime[20] = {};
    int bodovi = 0;
    int rel_bodovi = 0;
}Student;

int main(){

    int number = 0;

    number = NumberOfRows("datoteka.txt");
    printf("There are %d lines in the file.\n", number);

    DataDisplay(number, "datoteka.txt");

    return 0;
}

int NumberOfRows(const char* filename){
    FILE* fp = NULL;
    fp = fopen(filename, "r");
    char buffer[MAX] = {};
    int count = 0;

    if(!fp){
        printf("File opening error\n");
        return -1;
    }

    while(!feof(fp)){
        fgets(buffer, MAX, fp);
        count++;
    }
    fclose(fp);
    return count;
}

void DataDisplay (int n, const char* filename){
    FILE* fp = NULL;
    fp = fopen(filename, "r");
    Student *list = {};
    int max_bodovi = 20;

    if(!fp){
        printf("File opening error\n");
        return;
    }

    list = (Student*)malloc(n*sizeof(Student));

    for(int i=0;i<n;i++){
        fscanf(fp, "%s %s %d", list[i].ime, list[i].prezime, &list[i].bodovi);
    }

    printf("IME\tPREZIME\tBODOVI\tRELATIVNI BODOVI\n");
    for(int j=0;j<n;j++){
        printf("%s\t%s\t%d\t%f\n", list[j].ime, list[j].prezime, list[j].bodovi, (float)list[j].bodovi/max_bodovi*100);
    }

    fclose(fp);
}