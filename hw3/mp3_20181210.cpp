#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <map>
#include <fstream>
#include <cstring>
#include <stdexcept>
using namespace std;

const int AND8 = 255;//AND with this number to retain last 8 bits
const int INT_SZ_IN_CHAR = 4;
const int SZ_CHAR = 8 * sizeof(char);

/*During traversal of Huffman tree, edge to left child has value 0,
and edge to right child has value 1*/

int toInt(char ch) {
	if (ch == '0') {
		return 0;
	}
	return 1;
}

class HuffmanTreeNode {
public:
	bool isEOF;
	bool isChar;
	char character;
	int frequency;
	HuffmanTreeNode *leftChild = nullptr;
	HuffmanTreeNode *rightChild = nullptr;

	HuffmanTreeNode(char inputChar, bool isEOF, int frequency) {
		character = inputChar;
		this->isEOF = isEOF;
		this->frequency = frequency;
		isChar = !isEOF;
	}

	HuffmanTreeNode(int frequency) {//for internal nodes
		this->frequency = frequency;
		isChar = false;
		isEOF = false;
	}

	void makeLeftChild(HuffmanTreeNode *child) {
		this->leftChild = child;
		return;
	}

	void makeRightChild(HuffmanTreeNode *child) {
		this->rightChild = child;
		return;
	}

	void generateCharacterCodes(string& characterCode, map<char, string>& characterCodeMap, string& eofCode) {

		if (isChar) {
			characterCodeMap[character] = characterCode;
			return;
		}

		if (isEOF) {
			eofCode = characterCode;
			return;
		}

		if (leftChild != nullptr) {
			characterCode.push_back('0');
			(*leftChild).generateCharacterCodes(characterCode, characterCodeMap, eofCode);
			characterCode.pop_back();
		}

		if (rightChild != nullptr) {
			characterCode.push_back('1');
			(*rightChild).generateCharacterCodes(characterCode, characterCodeMap, eofCode);
			characterCode.pop_back();
		}

		return;
	}

};

class HuffmanNodePointer {//created to define comparison between pointers
public:
	HuffmanTreeNode* nodePointer;

	HuffmanNodePointer(HuffmanTreeNode* nodePointer) {
		this->nodePointer = nodePointer;
	}
};

bool operator>(const HuffmanNodePointer& node1, const HuffmanNodePointer& node2) {
	return node1.nodePointer->frequency > node2.nodePointer->frequency;
}

HuffmanTreeNode* buildHuffmanTree(map<char, int>& frequencyMap) {
	priority_queue<HuffmanNodePointer, vector<HuffmanNodePointer>, greater<HuffmanNodePointer>> pq;

	for (auto it : frequencyMap) {
		HuffmanTreeNode* newNode = new HuffmanTreeNode(it.first, false, it.second);
		HuffmanNodePointer newPointer = HuffmanNodePointer(newNode);
		pq.push(newPointer);
	}
	HuffmanTreeNode* newNode = new HuffmanTreeNode('a', true, 1);
	HuffmanNodePointer newPointer = HuffmanNodePointer(newNode);
	pq.push(newPointer);

	while (pq.size() > 1) {
		HuffmanNodePointer node1 = pq.top();
		pq.pop();
		HuffmanNodePointer node2 = pq.top();
		pq.pop();
		HuffmanTreeNode* newNode = new HuffmanTreeNode(node1.nodePointer->frequency + node2.nodePointer->frequency);
		(*newNode).makeLeftChild(node1.nodePointer);
		(*newNode).makeRightChild(node2.nodePointer);
		HuffmanNodePointer newPointer = HuffmanNodePointer(newNode);
		pq.push(newPointer);
	}

	return pq.top().nodePointer;
}


void createFrequencyMap(map<char, int>& frequencyMap, fstream& SourceFile) {
	char ch;
	while (SourceFile >> noskipws >> ch) {
		frequencyMap[ch]++;
	}
	return;
}

void breakInteger(int n, char chArr[]) {//follows Big-Endian format

	for (int i = 0; i < INT_SZ_IN_CHAR; i++) {
		chArr[i] = ((n >> ((INT_SZ_IN_CHAR - i - 1) * SZ_CHAR)) & AND8);
	}

	return;
}

void createHeaderFile(map<char, int>& frequencyMap, fstream& HeaderFile) {

	char chArr[INT_SZ_IN_CHAR];

	for (auto it : frequencyMap) {
		HeaderFile << it.first;
		breakInteger(it.second, chArr);
		for (int i = 0; i < INT_SZ_IN_CHAR; i++) {
			HeaderFile << chArr[i];
		}
	}

	return;
}

void createBodyFile(map<char, int>& frequencyMap, map<char, string>& characterCodeMap, string eofCode, fstream& SourceFile, fstream& BodyFile) {

	char chArr[INT_SZ_IN_CHAR];

	unsigned long cnt = 0;
	for (auto it : frequencyMap) {
		//BodyFile << it.first;
		cnt++;
		breakInteger(it.second, chArr);
		for (int i = 0; i < INT_SZ_IN_CHAR; i++) {
			//BodyFile << chArr[i];
			cnt++;
		}
	}
	BodyFile << cnt;
	BodyFile << '\t';
	for (auto it : frequencyMap) {
		BodyFile << it.first;
		breakInteger(it.second, chArr);
		for (int i = 0; i < INT_SZ_IN_CHAR; i++) {
			BodyFile << chArr[i];
		}
	}

	char ch;
	queue<char> outputQueue;

	while (SourceFile >> noskipws >> ch) {
		for (auto it : characterCodeMap[ch]) {
			outputQueue.push(it);
		}
	}
	for (auto it : eofCode) {
		outputQueue.push(it);
	}

	while (!outputQueue.empty()) {
		char opchar = 0;
		int val = 0;
		for (int i = 7; i >= 0; i--) {
			if (!outputQueue.empty()) {
				val = toInt(outputQueue.front());
				outputQueue.pop();
			}
			opchar |= val << i;
		}
		BodyFile << opchar;

	}

	return;
}


int makeInt(char chArr[]) {
	int result = 0;

	for (int i = 0; i < INT_SZ_IN_CHAR; i++) {
		result |= ((chArr[i] & AND8) << ((INT_SZ_IN_CHAR - i - 1) * SZ_CHAR));
	}

	return result;
}



void createFrequencyMap2(map<char, int>& frequencyMap, fstream& HeaderFile) {
	char ch;
	char chArr[INT_SZ_IN_CHAR];
	unsigned long header_size;
	char padding;
	HeaderFile >> header_size;
	HeaderFile >> noskipws >> padding;

	for (int i = 0; i < (header_size/5); i++) {
		HeaderFile >> noskipws >> ch;
		for (int i = 0; i < INT_SZ_IN_CHAR; i++) {
			try {
				HeaderFile >> chArr[i];
			}
			catch (...) {
				cerr << "Error reading header file\n";
				exit(1);
			}
		}

		frequencyMap[ch] = makeInt(chArr);
	}

	return;
}

void getBinaryString(map<char, int>& frequencyMap, vector<int>& binaryString, fstream& BodyFile) {
	char ch;
	

	while (BodyFile >> noskipws >> ch) {
		for (int i = 7; i >= 0; i--) {
			binaryString.push_back((ch >> i) & 1);
		}
	}

	return;
}

void generateSourceFile(vector<int>& binaryString, fstream& SourceFile, HuffmanTreeNode* root) {
	HuffmanTreeNode* ptr = root;
	char ch;
	//if root is EOF, then source file is empty
	if (root->isEOF) {
		return;//Source file is empty
	}

	for (auto it : binaryString) {
		if (it == 1) {
			ptr = ptr->rightChild;
		}
		else {
			ptr = ptr->leftChild;
		}
		if (ptr->isEOF) {
			break;
		}
		if (ptr->isChar) {
			SourceFile << ptr->character;
			ptr = root;
		}
	}

	return;
}


int main(int argc, char** argv) {//open all files and call functions
	if (argc != 3) {
		printf("Wrong usage\n");
		return -1;
	}


	if (strcmp(argv[1], "-c") == 0) {
		char file_name[128] = { 0 };				//file name

		strcpy(file_name, argv[2]);
		
	

		fstream SourceFile, BodyFile;

		SourceFile.open(argv[2], ios::in | ios::binary);
		if (!SourceFile.is_open()) {
			printf("file open failed\n");
			return -1;
		}
		strcat(file_name, ".zz");
		BodyFile.open(file_name, ios::trunc | ios::out | ios::binary);
		
			
			
		
		

		map<char, int> frequencyMap;
		map<char, string> characterCodeMap;
		string eofCode;
		string characterCode = "";

		createFrequencyMap(frequencyMap, SourceFile);
		SourceFile.clear();
		SourceFile.seekg(0);//reset sourcefile to beginning

		HuffmanTreeNode* root = buildHuffmanTree(frequencyMap);

		root->generateCharacterCodes(characterCode, characterCodeMap, eofCode);

		createBodyFile(frequencyMap, characterCodeMap, eofCode, SourceFile, BodyFile);

		SourceFile.close();
		BodyFile.close();



	}

	else if (strcmp(argv[1], "-d") == 0) {
		char file_name[128] = { 0 };				//file name
		strcpy(file_name, argv[2]);

		fstream FileIn, FileOut;

		FileIn.open(argv[2], ios::in | ios::binary);
		if (!FileIn.is_open()) {
			printf("file open failed\n");
			return -1;
		}
		strcat(file_name, ".yy");
		FileOut.open(file_name, ios::trunc | ios::out | ios::binary);

		


		map<char, int> frequencyMap;
		vector<int> bodyFileBinaryString;

		try {
			createFrequencyMap2(frequencyMap, FileIn);
		}
		catch (...) {
			cerr << "Error reading Header file: file may be corrupted\n";
			exit(1);
		}

		getBinaryString(frequencyMap, bodyFileBinaryString, FileIn);

		HuffmanTreeNode* root = buildHuffmanTree(frequencyMap);

		generateSourceFile(bodyFileBinaryString, FileOut, root);

		FileIn.close();
		FileOut.close();
			
	}

	else {
		printf("Wrong command, select between -c or -d\n");
		return -1;
	}

	
	return 0;
}