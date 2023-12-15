#include "compress.h"
#include "huffman.h"
#include <fstream>
#include <iostream>

//#include <unordered_map>
using namespace std;

void encode() {
	const char* FName = "example.txt";

	int header[256] = { 0 };
	ifstream in(FName, ios::binary);
	in.seekg(0, in.end);
	int file_size = in.tellg();
	in.seekg(0, in.beg);
	char* bytes = new char[file_size];
	int z = sizeof(bytes);
	in.read((char*)bytes, file_size);
	for (int i = 0; i < file_size; i++)
	{
		header[(int)bytes[i] + 128]++;
	}
	in.close();

	Node* root = makeHuffmanTree(header);
	unordered_map<char, string> huffmanCode;
	TreeToMap(root, "", huffmanCode);

	ofstream out("example_compress.txt", ios::binary);

	out.write(reinterpret_cast<char*>(header), 256 * sizeof(int));

	char counter = 8;
	char buff = 0;
	for (int i = 0; i < file_size; i++)
	{
		string c = huffmanCode[bytes[i]];
		for (char b : huffmanCode[bytes[i]]) {
			counter--;
			buff |= ((b - '0') << counter);

			if (counter == 0) {
				out << buff;
				buff = 0;
				counter = 8;
			}
		}
	}
	if (buff != 0) {
		out << buff;
		out << counter;
	}
};

void decode() {

	const char* FName = "example_compress.txt";

	int header[256] = {};
	char leftoff = 0;
	ifstream in(FName, ios::binary);
	in.seekg(0, in.end);
	int file_size = in.tellg();
	in.seekg(0, in.beg);
	int s = file_size - 256 * sizeof(int) - 1;
	char* bytes = new char[s];
	int z = sizeof(bytes);
	in.read(reinterpret_cast<char*>(header), 256 * sizeof(int));
	in.read((char*)bytes, s);
	in.read(&leftoff, 1);

	in.close();

	Node* root = makeHuffmanTree(header);
	unordered_map<char, string> huffmanCode;
	TreeToMap(root, "", huffmanCode);

	ofstream out("example_decompress.txt", ios::binary);
	Node* tr = root;
	int size = 0;
	char counter = 7;
	for (int i = 0; i < s; i++) {
		while (counter >= 0) {

			tr = bytes[i] & (1 << counter) ? tr->right : tr->left;
			counter--;
			size++;
			if (!tr->left && !tr->right) {
				out << (char)(tr->ch);
				if (size == s * 8 - leftoff)
					break;
				tr = root;
			}	
		}
		counter = 7;
	}
	out.close();
}