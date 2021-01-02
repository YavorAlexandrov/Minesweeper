#include <iostream>
#include <thread>        
#include <chrono> 
using namespace std;
const int SIZE = 10;
void fillHiddenMatrix(char matrix[SIZE][SIZE], int n);
void fillBoolMatrix(bool isFilledMatrix[SIZE][SIZE], int n);
void printMatrix(const char matrix[SIZE][SIZE], int n);
void fillBombs(char matrix[SIZE][SIZE], bool isFilledMatrix[SIZE][SIZE], int n, int amountOfBombs);
int generateCoordinate(int n);
void fillPlayingMatrix(char matrix[SIZE][SIZE], int n);
int countMinesAround(char matrix[SIZE][SIZE], int i, int j, int n);
char intToChar(int x);
void printCurrentField(char hiddenMatrix[SIZE][SIZE], char playingField[SIZE][SIZE], 
						bool isOpenedMatrix[SIZE][SIZE], int n);
void openBox(bool isOpenedMatrix[SIZE][SIZE], char playingField[SIZE][SIZE], char hiddenMatrix[SIZE][SIZE], int n, int x, int y, bool& hasLost);
void markBox(char hiddenMatrix[SIZE][SIZE], int x, int y);
void unmarkBox(char hiddenMatrix[SIZE][SIZE], int x, int y);
bool hasWon(bool isOpenedMatrix[SIZE][SIZE], int n, int amountOfMines);


int main()
{
	int n;
	cout << "Enter the size of the field [3,10]: ";
	cin >> n;
	while (n < 3 || n>10) {
		cout << "Incorrect size! Try again: ";
		cin >> n;
	}
	int minesAmount;
	cout << "Enter the amout of mines [1,3*size]: ";
	cin >> minesAmount;
	while (minesAmount < 1 || minesAmount>3 * n) {
		cout << "Incorrect amount! Try again: ";
		cin >> minesAmount;
	}


	char playingField[SIZE][SIZE];
	char overlayingField[SIZE][SIZE];
	bool isFilledMatrix[SIZE][SIZE];
	bool isOpenedMatrix[SIZE][SIZE];


	fillHiddenMatrix(overlayingField, n);
	fillBoolMatrix(isFilledMatrix, n);
	fillBoolMatrix(isOpenedMatrix, n);
	fillBombs(playingField, isFilledMatrix, n, minesAmount);
	fillPlayingMatrix(playingField, n);

	
	
	int x, y;
	
	bool hasLost = false;
	char command[7];
	

	int turnCount = 0;
	
	
	while (!hasLost) {
		system("cls");
		printCurrentField(overlayingField, playingField, isOpenedMatrix, n);
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		cin.getline(command, 7, ' ');
		cin >> x >> y;
		x--;
		y--;
		if ((x < 0 || x>9) || (y < 0 || y>9)) {
			cout << "Invalid coordinates!";
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			continue;
		}
		if (strcmp(command, "open") == 0) {
			openBox(isOpenedMatrix, playingField, overlayingField, n, x, y, hasLost);
			turnCount++;
		}
		else if (strcmp(command, "mark") == 0) {
			markBox(overlayingField, x, y);
			turnCount++;
		}
		else if (strcmp(command, "unmark") == 0) {
			unmarkBox(overlayingField, x, y);
			turnCount++;
		}
		else {
			std::cout << "Invalid command!";
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			continue;
		}	
		
		if (hasWon(isOpenedMatrix, n, minesAmount)) {
			break;
		}
		
	}
	system("cls");
	cout << x+1 << "," << y+1 << endl;
	if (hasLost) {
		cout << "Game over!";
	}
	else cout << "You won in "<< turnCount<<" turns!";
	cout << endl;
	printCurrentField(overlayingField, playingField, isOpenedMatrix, n);

}

void fillHiddenMatrix(char matrix[SIZE][SIZE], int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			matrix[i][j] = '#';
		}
	}
}

void fillBoolMatrix(bool isFilledMatrix[SIZE][SIZE], int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			isFilledMatrix[i][j] = false;
		}
	}
}

void printMatrix(const char matrix[SIZE][SIZE], int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << '|' << matrix[i][j];
		}
		cout << '|' << endl;
	}
}

int generateCoordinate(int n) {
	return rand()%n;
}

void fillBombs(char matrix[SIZE][SIZE], bool isFilledMatrix[SIZE][SIZE], int n, int amountOfBombs) {
	for (int i = 0; i < amountOfBombs; i++) {
		
		int x = generateCoordinate(n);
		int y = generateCoordinate(n);
		while (isFilledMatrix[x][y]) {
			x = generateCoordinate(n);
			y = generateCoordinate(n);
		}
		matrix[x][y] = '*';
		isFilledMatrix[x][y] = true;

	}
}

int countMinesAround(char matrix[SIZE][SIZE], int i, int j, int n) {
	int count = 0;
	if (j + 1 < n) {//1
		if (matrix[i][j + 1] == '*') {
			count++;
		}
	}
	if (j + 1 < n && i + 1 <n) {//2
		if (matrix[i+1][j + 1] == '*') {
			count++;
		}
	}
	if (i + 1 < n) {//3
		if (matrix[i+1][j] == '*') {
			count++;
		}
	}
	if (i + 1 < n && j-1>=0) {//4
		if (matrix[i+1][j - 1] == '*') {
			count++;
		}
	}
	if (j - 1 >=0) {//5
		if (matrix[i][j - 1] == '*') {
			count++;
		}
	}
	if (j - 1 >=0 && i-1>=0) {//6
		if (matrix[i-1][j - 1] == '*') {
			count++;
		}
	}
	if (i - 1 >= 0) {//7
		if (matrix[i-1][j] == '*') {
			count++;
		}
	}
	if (j + 1 < n && i-1>=0) {//8
		if (matrix[i-1][j + 1] == '*') {
			count++;
		}
	}
	return count;
}

void fillPlayingMatrix(char matrix[SIZE][SIZE], int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (matrix[i][j] == '*') continue;
			int count = countMinesAround(matrix, i, j, n);
			if (count != 0) {
				matrix[i][j] = intToChar(count);
			}
			else {
				matrix[i][j] = ' ';
			}
		}
	}
}

char intToChar(int x) {
	switch (x)
	{
	case 1:return '1';
	case 2:return '2';
	case 3:return '3';
	case 4:return '4';
	case 5:return '5';
	case 6:return '6';
	case 7:return '7';
	case 8:return '8';
	
	
	}

}

void printCurrentField(char hiddenMatrix[SIZE][SIZE], char playingField[SIZE][SIZE], bool isOpenedMatrix[SIZE][SIZE], int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (isOpenedMatrix[i][j]) {
				cout << '|' << playingField[i][j];
			}
			else {
				cout << '|' << hiddenMatrix[i][j];
			}
		}
		cout << '|' << endl;
	}
}

void openBox(bool isOpenedMatrix[SIZE][SIZE], char playingField[SIZE][SIZE], char hiddenMatrix[SIZE][SIZE], int n, int x, int y, bool& hasLost) {
	if (hiddenMatrix[x][y] == 'F') {
		cout << "Unable to open marked box!";
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		return;
	}
	if (isOpenedMatrix[x][y] == true) {
		cout << "Box is already opened!";
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		return;
	}
	isOpenedMatrix[x][y] = true;
	if (playingField[x][y] == '*') {
		
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if (playingField[i][j] == '*') {
					isOpenedMatrix[i][j] = true;
					hasLost = true;
				}
			}
		}
		return;
	}
	
	if (playingField[x][y] == ' ') {
		if (y + 1 < n) {
			if (playingField[x][y + 1] == ' ' && isOpenedMatrix[x][y + 1]==false) {
				openBox(isOpenedMatrix, playingField, hiddenMatrix, n, x, y + 1, hasLost);
			}
			else{
				isOpenedMatrix[x][y + 1] = true;
			}
		}
		if (x + 1 < n && y + 1 < n) {
			if (playingField[x + 1][y+1] == ' ' && isOpenedMatrix[x + 1][y+1] == false) {
				openBox(isOpenedMatrix, playingField, hiddenMatrix, n, x + 1, y+1, hasLost);
			}
			else {
				isOpenedMatrix[x + 1][y + 1] = true;
			}
		}
		
		if (x + 1 < n) {
			if (playingField[x + 1][y] == ' ' && isOpenedMatrix[x+1][y] == false) {
				openBox(isOpenedMatrix, playingField, hiddenMatrix, n, x+1, y, hasLost);
			}
			else{
				isOpenedMatrix[x + 1][y] = true;
			}
		}
		if (x + 1 < n && y-1>=0) {
			if (playingField[x + 1][y-1] == ' ' && isOpenedMatrix[x + 1][y-1] == false) {
				openBox(isOpenedMatrix, playingField, hiddenMatrix, n, x + 1, y-1, hasLost);
			}
			else {
				isOpenedMatrix[x + 1][y - 1] = true;
			}
		}
		
		if (y - 1 >= 0) {
			if (playingField[x][y - 1] == ' ' && isOpenedMatrix[x][y - 1] == false) {
				openBox(isOpenedMatrix, playingField, hiddenMatrix, n, x, y - 1, hasLost);
			}
			else{
				isOpenedMatrix[x][y - 1] = true;
			}
		}
		if (x - 1 >= 0 && y-1>=0) {
			if (playingField[x - 1][y-1] == ' ' && isOpenedMatrix[x - 1][y-1] == false) {
				openBox(isOpenedMatrix, playingField, hiddenMatrix, n, x - 1, y-1, hasLost);
			}
			else {
				isOpenedMatrix[x - 1][y - 1] = true;
			}
		}
		
		if (x - 1 >= 0) {
			if (playingField[x - 1][y] == ' ' && isOpenedMatrix[x-1][y] == false) {
				openBox(isOpenedMatrix, playingField, hiddenMatrix, n, x-1, y, hasLost);
			}
			else{ 
				isOpenedMatrix[x - 1][y] = true;
			}
		}
		if (x - 1 >= 0 && y+1<n) {
			if (playingField[x - 1][y+1] == ' ' && isOpenedMatrix[x - 1][y+1] == false) {
				openBox(isOpenedMatrix, playingField, hiddenMatrix, n, x - 1, y+1, hasLost);
			}
			else {
				isOpenedMatrix[x - 1][y + 1] = true;
			}
		}
		
		
	}
	hasLost = false;
}

void markBox(char hiddenMatrix[SIZE][SIZE], int x, int y) {
	hiddenMatrix[x][y] = 'F';
}

void unmarkBox(char hiddenMatrix[SIZE][SIZE], int x, int y) {
	hiddenMatrix[x][y] = '#';
}

bool hasWon(bool isOpenedMatrix[SIZE][SIZE], int n, int amountOfMines) {
	int count = 0;
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++) {
			if (isOpenedMatrix[i][j]) {
				count++;
			}
		}
	}
	return count == (n * n) - amountOfMines;
}



