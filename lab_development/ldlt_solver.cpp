
#include <iostream>
#include <fstream>
#include <string>

#define DATA_TYPE int
#define LENGTH 10

// size of char = 1 byte
// size of int = 4 bytes
// size of float = 4 bytes
// size of double = 8 bytes
//

using namespace std;

int read(string fileName, DATA_TYPE* memoryPointer, int bytesToRead) {
	fstream readingFile;
	readingFile.open(fileName);

	char* buffer = new char [bytesToRead];
	readingFile.read(buffer, 21);
	readingFile.close();

	int number;
	for (int i = 0; i < 21; i++ ) {
		cout << buffer[i];
		//number = buffer[i * 2] - '0';
		//cout << "buf:" << buffer[i] << " "  << number << endl;
	}
	return 0;
}