#ifndef UTILS_H
#define UTILS_H

int readFile(const char* filename, char** txt);
size_t fileSize(const char* filename);
void fprintfnTab(FILE* fp, int n);
void clearBuffer();
int cmpf(double num1, double num2);

#endif
