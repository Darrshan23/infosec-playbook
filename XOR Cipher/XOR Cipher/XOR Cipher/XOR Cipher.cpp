//#include<bits/stdc++.h>  NOT AVAILABLE IF USE VS 2026.
#include <cstdio>    // Includes printf
#include <cstring>   // Includes strlen

using namespace std;

void encryptDecrypt(char inpString[]) {
	char xorKey = 'a';

	int len = strlen(inpString);

	for (int i = 0; i < len; i++) {
		printf("%c", inpString[i]);
		inpString[i] = inpString[i] ^ xorKey;
		printf("%c",inpString[i]);
	}
}

int main() {
	char sample[] = "Darrshan";

	printf("Encrypted String: ");
	encryptDecrypt(sample);

	printf("Decrypted String: ");
	encryptDecrypt(sample);

}
