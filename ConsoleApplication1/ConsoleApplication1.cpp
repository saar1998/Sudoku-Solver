#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <set>
#include <algorithm>
#include <iterator>

using namespace std;

const int height = 9;
const int width = 9;
int board[height][width] = {0, 0, 0, 0, 0, 8, 0, 0, 0,
														7, 0, 0, 4, 0, 0, 6, 8, 0,
														3, 0, 0, 5, 0, 0, 0, 2 ,0,
														0, 3, 0, 2, 0, 0, 0, 0, 6,
														4, 0, 0, 6, 0, 0, 0, 0, 0,
														0, 0, 6, 0, 7, 3, 4, 5, 0,
														0, 7, 0, 3, 0, 0, 0, 0, 0,
														0, 5, 1, 0, 0, 2, 0, 0, 0, 
														9, 0, 0, 0, 5, 0, 8, 7, 0};
list<int> possibilitiesArray[9][9];

struct coordinate {
		int x, y;
};

template <typename T>
void printList(list<T> l) {
  typename list<T>::iterator it; 
	for (it = l.begin(); it != l.end(); it++) {
			cout << *it << ',';
	}
	cout << '\n';
}

template <> 
void printList<coordinate>(list<coordinate> l) {
	list<coordinate>::iterator it; 
	for (it = l.begin(); it != l.end(); it++) {
			cout << (*it).x << ',' << (*it).y << '\n' ;
	}
	cout << '\n';
}

int getCellContent(coordinate place) {
	return board[place.x][place.y];
}

void printCoordiante(coordinate place) {
		cout << place.x << ',' << place.y << ':' << getCellContent(place) << endl;
}

bool operator == (const coordinate& a, const coordinate& b) {
  return a.x == b.x && a.y == b.y;
}

void printBoard(int board[height][width]) {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (board[i][j] > 0) {
				cout << board[i][j];
			}
			else {
				cout << ' ';
			}
			cout << '|';
		}
		cout << '\n';
	}  
}		

int insertCoordinateToArray(coordinate* arr, int x, int y, int insertionCounter) {
  arr[insertionCounter] = {x, y};
	return insertionCounter +1;
}

coordinate* getNeighborRow(coordinate place, bool selfIncluded) {
	int neighborsNum = selfIncluded ? 9 : 8,
			insertionCounter = 0;
	coordinate* neighborRow = new coordinate[neighborsNum];

	for (int i = 0; i < 9; i++) {
		if (place.y != i || selfIncluded) {
			insertionCounter = insertCoordinateToArray(neighborRow, place.x, i, insertionCounter);
		}
	}
	return neighborRow;
}

coordinate* getNeighborRow(coordinate place) {
	return getNeighborRow(place, false);
}


coordinate* getNeighborColumn(coordinate place, bool selfIncluded) {
	int neighborsNum = selfIncluded ? 9 : 8,
			insertionCounter = 0;
	coordinate* neighborColumn = new coordinate[neighborsNum];

	for (int i = 0; i < 9; i++) {
		if (place.x != i || selfIncluded) {
			insertionCounter = insertCoordinateToArray(neighborColumn, i, place.y, insertionCounter);
		}
	}
	return neighborColumn;
}

coordinate* getNeighborColumn(coordinate place) {
	return getNeighborColumn(place, false);
}

coordinate* getNeighborSquare(coordinate place, bool selfIncluded) {
	int neighborsNum = selfIncluded ? 9 : 8,
			insertionCounter = 0,
			bottomX = (place.x / 3) * 3,
			bottomY = (place.y / 3) * 3;
	coordinate* neighborSquare = new coordinate[neighborsNum];

	for (int i = bottomX; i < bottomX + 3; i++) {
		for (int j = bottomY; j < bottomY + 3; j++) {
		  if (i != place.x || j != place.y || selfIncluded) {
				insertionCounter = insertCoordinateToArray(neighborSquare, i, j, insertionCounter);
			}
		}			
  }
	return neighborSquare;
}

coordinate* getNeighborSquare(coordinate place) {
	return getNeighborSquare(place, false);
}

coordinate* getFilteredNeighborSquare(coordinate place) {
  int insertionCounter = 0,
			bottomX = (place.x / 3) * 3,
			bottomY = (place.y / 3) * 3;
		coordinate* neighborSquare = new coordinate[4];

	for (int i = bottomX; i < bottomX + 3; i++) {
		for (int j = bottomY; j < bottomY + 3; j++) {
		  if (i != place.x && j != place.y) {
			 	insertionCounter = insertCoordinateToArray(neighborSquare, i, j, insertionCounter);
			}
		}			
	}
	return neighborSquare;
}

coordinate* getNeighbors(coordinate place, bool selfIncluded) {
	int neighborsNum = selfIncluded ? 21 : 20, 
			i = 0;
	coordinate* neighbors = new coordinate[neighborsNum];
	coordinate*	neighborRow = getNeighborRow(place, selfIncluded);
	coordinate* neighborColumn = getNeighborColumn(place);
  coordinate* neighborSquare = getFilteredNeighborSquare(place);
	for (;i < 4; i++) {
		neighbors[i] = neighborSquare[i];
	}
	for (;i < 12; i++) {
		neighbors[i] = neighborColumn[i - 4];
	}
	for (;i < neighborsNum; i++) {
		neighbors[i] = neighborRow[i - 12];
	}

	return neighbors;
}

coordinate* getNeighbors(coordinate place) {
	return getNeighbors(place, false);
}

void basicUpdatePossibilities(coordinate place) {
  coordinate* coordianates = getNeighbors(place);
	for (int i = 0; i < 20; i++) {
		possibilitiesArray[place.x][place.y].remove(getCellContent(coordianates[i]));
	}
}

void updatePossibilitieArrays() {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			coordinate currentCoordinate {i, j};
			if (getCellContent(currentCoordinate) == 0) {
				basicUpdatePossibilities(currentCoordinate);
			}
		}
	}
}

bool updateBoardByPossibilties(bool lastUpdated) {
	bool updated = false;
  updatePossibilitieArrays();
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (getCellContent(coordinate{i, j}) == 0 && possibilitiesArray[i][j].size() == 1) {
			  updated = true;
			  board[i][j] = possibilitiesArray[i][j].front();
		  }
		}
	}
	if (updated) {
		return updateBoardByPossibilties(true);
	}
	return lastUpdated;
}

bool updateBoardByPossibilties() {
	return updateBoardByPossibilties(false);
}

bool checkIfIntListContainsNum(list<int> listToCheck, int num) {
  list<int>::iterator it;
  it = std::find(listToCheck.begin(), listToCheck.end(), num);
  return it != listToCheck.end();
}

list<int> getPossibilitiesByCoordinate(coordinate c) {
  return possibilitiesArray[c.x][c.y];
}

list<coordinate>* getNumbersPossibleOccurences(coordinate* neighbors) {
	list<coordinate>* possibleOccurenceList = new list<coordinate>[9];
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (getCellContent(neighbors[j]) == 0) {
				if (checkIfIntListContainsNum(getPossibilitiesByCoordinate(neighbors[j]), i + 1)) {
				  possibleOccurenceList[i].push_back(neighbors[j]);
				}
			}
		}
	}
	return possibleOccurenceList;
}

coordinate* getNeighborsByType(coordinate cor, string type, bool selfIncluded) {
  if (type == "column") {
		return getNeighborColumn(cor, true);
	}
  if (type == "row") {
		return getNeighborRow(cor, true);
	}
	return getNeighborSquare(cor, true);
}

void updateNeighborsOfChange(coordinate currentCoordinate) {
  int content = getCellContent(currentCoordinate);
	coordinate* neighbors = getNeighbors(currentCoordinate);
	for (int i = 0; i < 20; i++) {
		coordinate neighbor = neighbors[i];
		possibilitiesArray[neighbor.x][neighbor.y].remove(content);
	}
}

bool updateNeighborsByPossibleOccurenes(coordinate* neighbors) {
  bool updated = false;
	list<coordinate>* list = getNumbersPossibleOccurences(neighbors);
  for (int i = 0; i < 9; i++) {
	  if (list[i].size() == 1) {
	    coordinate toPlace = list[i].back();		
	    board[toPlace.x][toPlace.y] = i + 1;
			updateNeighborsOfChange(toPlace);
		  updated = true;
	  }
  }
	return updated;
}

bool updateBoardBySpecificPossibleOccurences(string neighborsType ,bool lastUpdated) {
	bool updated = false;
	coordinate* neighbors;
	if (neighborsType == "column") {
		for (int m = 0; m < 9; m++) {
			neighbors = getNeighborsByType(coordinate{0, m}, neighborsType, true);
			updated = updateNeighborsByPossibleOccurenes(neighbors) || updated;
		}
  }
	else if (neighborsType == "row") {
	  for (int m = 0; m < 9; m++) {
			neighbors = getNeighborsByType(coordinate{m, 0}, neighborsType, true);
			updated = updateNeighborsByPossibleOccurenes(neighbors) || updated;
		}
  }
	else {
		for (int m = 0; m < 9; m++) {
			neighbors = getNeighborsByType(coordinate{(m % 3) * 3 , (m / 3) * 3}, neighborsType, true);
			updated = updateNeighborsByPossibleOccurenes(neighbors) || updated;
		}
	}
	if (updated) {
		return updateBoardBySpecificPossibleOccurences(neighborsType, true);
	}
	return lastUpdated;;
}

bool updateBoardByAllPossibleOccurences(bool lastUpdated) {
	updatePossibilitieArrays();
	bool updatedColumns = updateBoardBySpecificPossibleOccurences("column", false);
	bool updatedRows = updateBoardBySpecificPossibleOccurences("row", false);
	bool updatedSquares = updateBoardBySpecificPossibleOccurences("square", false);
	if (updatedColumns || updatedRows || updatedSquares) {
		return updateBoardByAllPossibleOccurences(true);
	}
	return lastUpdated;
}

bool updateBoardByAllPossibleOccurences() {
  return updateBoardByAllPossibleOccurences(false);
}

void updateBoard() {
	if (updateBoardByPossibilties() || updateBoardByAllPossibleOccurences()) {
		updateBoard();
	}
}

void initializePossibilities() {
  for (int i = 0; i < 9; i++) {
	  for (int j = 0; j < 9; j++) {
			for (int m = 1; m <= 9; m++) {
			  possibilitiesArray[i][j].insert(possibilitiesArray[i][j].end(), m);
      }
		}
  }
}

int main() {
  initializePossibilities();
	updateBoard();
	printBoard(board);
}



