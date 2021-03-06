#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

void saveTableToFile(const unsigned int size, double** table, const string &fileName) {
	ofstream file;
	file.open(fileName);
	for (unsigned int i = 0; i < size; i++) {
		for (unsigned int j = 0; j < size; j++) {
			if (j != 0)
				file << ",";
			file << table[j][i];
		}
		file << "\n";
	}
	file.close();
}

double ** createNewTable(const unsigned int size) {
	auto** table = new double*[size];
	for (unsigned int i = 0; i < size; ++i)
		table[i] = new double[size];
	return table;
}

double ** getTableFromFile(const unsigned int size, const string &fileName) {
	string line;
	ifstream file(fileName);
	if (file.is_open()) {
		double** table = createNewTable(size);
		unsigned int k = 0;
		vector<double> vector;
		while (getline(file, line)) {
			stringstream ss(line);
			double i;
			while (ss >> i) {
				vector.push_back(i);
				if (ss.peek() == ',')
					ss.ignore();
			}
			for (unsigned n = 0; n < vector.size(); n++) {
				table[n][k] = vector.at(n);
			}
			k++;
			vector.clear();
		}
		file.close();
		return table;
	}
	else {
		cout << "Unable to open file: " << fileName << endl;
		return nullptr;
	}
}

int getSize(const string &fileName) {
	string line;
	unsigned int size = 0;
	ifstream file(fileName);
	if (file.is_open()) {
		int counterX = 0;
		while (getline(file, line)) {
			vector<double> vector;
			double i;
			stringstream ss(line);
			while (ss >> i) {
				vector.push_back(i);
				if (ss.peek() == ',')
					ss.ignore();
			}
			counterX++;
			if (size == 0)
				size = vector.size();
			else if (size != vector.size()) {
				cout << "Size of the rows are not equal or contains other characters than numbers in file: " << fileName << endl;
				file.close();
				return -1;
			}
		}
		if (counterX != size) {
			cout << "Number of the rows and columns are not equal in file: " << fileName << endl;
			file.close();
			return -1;
		}
	}
	else {
		cout << "Unable to open file: " << fileName << endl;
		return -1;
	}
	file.close();
	return size;
}

double ** codeTable1(const unsigned int size, double** table) {
	double **newTable = createNewTable(size);
	unsigned int counter = 0;
	for (unsigned int i = 0; i < size; i++) {
		if (counter == 0)
			for (unsigned int j = 0; j < size; j++) {
				newTable[j][i] = table[i][counter];
				counter++;
			}
		else {
			counter = size - 1;
			for (unsigned int j = 0; j < size; j++) {
				newTable[j][i] = table[i][counter];
				counter--;
			}
			counter = 0;
		}
	}
	return newTable;
}

bool checkCoord(int x, int j, const unsigned int size) {
	return x >= 0 && j >= 0 && x < size && j < size;
}

double ** codeTable2(const unsigned int size, double** table) {
	auto* tableHelper = new double[size * 2];
	double **newTable = createNewTable(size);
	unsigned int indexX = 0;
	unsigned int indexY = 0;
	unsigned int counter = 0;
	bool check = false;
	for (unsigned int k = 0; k < size; k++) {
		tableHelper[counter] = table[k][0];
		counter++;
		int i = k - 1;
		int j = 1;
		while (checkCoord(i, j, size)) {
			tableHelper[counter] = table[i][j];
			counter++;
			i--;
			j++;
		}
		if (check)
			for (unsigned int p = 0; p < counter; p++) {
				if (indexX > size - 1) {
					indexX = 0;
					indexY++;
				}
				newTable[indexX][indexY] = tableHelper[p];
				indexX++;
			}
		else
			for (int p = counter - 1; p >= 0; p--) {
				if (indexX > size - 1) {
					indexX = 0;
					indexY++;
				}
				newTable[indexX][indexY] = tableHelper[p];
				indexX++;
			}
		counter = 0;
		check = !check;
	}
	counter = 0;
	check = false;
	for (unsigned int k = 1; k < size; k++) {
		tableHelper[counter] = table[size - 1][k];
		counter++;
		int i = size - 2;
		int j = k + 1;
		while (checkCoord(i, j, size)) {
			tableHelper[counter] = table[i][j];
			counter++;
			i--;
			j++;
		}
		if (check)
			for (unsigned int p = 0; p < counter; p++) {
				if (indexX > size - 1) {
					indexX = 0;
					indexY++;
				}
				newTable[indexX][indexY] = tableHelper[p];
				indexX++;
			}
		else
			for (int p = counter - 1; p >= 0; p--) {
				if (indexX > size - 1) {
					indexX = 0;
					indexY++;
				}
				newTable[indexX][indexY] = tableHelper[p];
				indexX++;
			}
		counter = 0;
		check = !check;
	}
	return newTable;
}

double ** codeTable3(const unsigned int size, double** table) {
	double **newTable = createNewTable(size);
	unsigned int counter = 0;
	for (unsigned int i = 0; i < size; i++) {
		if (counter == 0)
			for (unsigned int j = 0; j < size; j++) {
				newTable[j][i] = table[counter][i];
				counter++;
			}
		else {
			counter = size - 1;
			for (unsigned int j = 0; j < size; j++) {
				newTable[j][i] = table[counter][i];
				counter--;
			}
			counter = 0;
		}
	}
	return newTable;
}

bool executeMatrix(const string &fileName) {
	const int tempSize = getSize(fileName);
	if (tempSize == -1) {
		cout << "Problem with reading file: " << fileName << endl;
		return false;
	}
	const auto size = static_cast<const unsigned int>(tempSize);
	const auto table = getTableFromFile(size, fileName);
	saveTableToFile(size, codeTable1(size, table), "Encrypted1_" + fileName);
	saveTableToFile(size, codeTable2(size, table), "Encrypted2_" + fileName);
	saveTableToFile(size, codeTable3(size, table), "Encrypted3_" + fileName);
	return true;
}

void displayHelp() {
	cout << "-------------help-------------" << endl;
	cout << "Enter file names which you want to code" << endl;
	cout << "For example: \"program matrix1.txt matrix2.txt\"" << endl;
	cout << "For more help write \"program h\"" << endl << endl;
	cout << "Created by Szymon Gajdzica" << endl;
	system("pause");
}

int main(int argc, char *argv[]) {

	if (argc < 2) {
		displayHelp();
		return 0;
	}

	auto *fileNames = new string[argc];
	for (unsigned int i = 1; i < argc; i++) {
		const string parameter = argv[i];
		if (parameter == "h") {
			displayHelp();
			return 0;
		}
		fileNames[i - 1] = parameter;
	}

	for (unsigned int i = 0; i < argc - 1; i++) {
		const auto fileName = fileNames[i];
		if (executeMatrix(fileName))
			cout << "Encryption of file: " + fileName + " SUCCESS";
		else
			cout << "Encryption of file: " + fileName + " FAIL";
		cout << endl << endl;
	}
	system("pause");
	return 0;
}


