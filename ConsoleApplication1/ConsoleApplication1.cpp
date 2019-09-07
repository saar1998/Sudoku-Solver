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
int board[height][width] = {2, 5, 0, 0, 0, 3, 0, 6, 0,
														1, 3, 0, 0, 0, 0, 8, 0, 0,
														0, 0, 0, 0, 1, 0, 0, 0 ,0,
														0, 4, 0, 0, 0, 7, 0, 0, 0,
														0, 0, 9, 5, 2, 8, 3, 0, 0,
														0, 0, 0, 4, 0, 0, 0, 5, 0,
														0, 0, 0, 0, 9, 0, 0, 0, 0,
														0, 0, 2, 0, 0, 0, 0, 7, 6, 
														0, 6, 0, 7, 0, 0, 0, 4, 3};
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

void insertToCoordinate(coordinate place, int num) {
	board[place.x][place.y] = num;
	possibilitiesArray[place.x][place.y].clear();												
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

void removePossibilityFromCoordinate(coordinate cor, int possibility) {
	possibilitiesArray[cor.x][cor.y].remove(possibility);
}


void basicUpdatePossibilities(coordinate place) {
  coordinate* coordianates = getNeighbors(place);
	for (int i = 0; i < 20; i++) {
		removePossibilityFromCoordinate(place, getCellContent(coordianates[i]));
	}	
}

template <typename T>
bool checkIfListContainsItem(list<T> listToCheck, T item) {
  typename list<T>::iterator it;
  it = std::find(listToCheck.begin(), listToCheck.end(), item);
  return it != listToCheck.end();
}

template <typename T>
bool checkIfListContainsAnyOfTheItems(list<T> listToCheck, list<T> itemsList) {
	typename list<T>::iterator it;
	for (it = itemsList.begin(); it != itemsList.end(); it++) {
		if (checkIfListContainsItem<T>(listToCheck, *it)) {
			return true;
		}
	}
	return false;
}

void removePossibilitiesFromCoordinate(coordinate cor, list<int> possibilities) {
	typename list<int>::iterator it;
	for (it = possibilities.begin(); it != possibilities.end(); it++) {
		removePossibilityFromCoordinate(cor, *it);
	}
}

void keepOnlyPossibilitiesInCoordinate(coordinate cor, list<int> possibilities) {
	for (int i = 0; i < 9; i++) {
		if (!checkIfListContainsItem<int>(possibilities, i)) {
			removePossibilityFromCoordinate(cor, i);
		}
	}
}

list<int> getPossibilitiesByCoordinate(coordinate c) {
  return possibilitiesArray[c.x][c.y];
}

list<coordinate>* getNumbersPossibleOccurences(coordinate* neighbors) {
	list<coordinate>* possibleOccurenceList = new list<coordinate>[9];
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (getCellContent(neighbors[j]) == 0) {
				if (checkIfListContainsItem<int>(getPossibilitiesByCoordinate(neighbors[j]), i + 1)) {
				  possibleOccurenceList[i].push_back(neighbors[j]);
				}
			}
		}
	}
	return possibleOccurenceList;
}

string checkNeighborsType(list<coordinate> neighbors) {
	bool sameRow = true, sameColumn = false;
  int rowNum = neighbors.front().x, columnNum = neighbors.front().y;
	list<coordinate>::iterator it;
	for (it = neighbors.begin(); it != neighbors.end(); it++) {
    sameRow = (rowNum == it -> x);
		sameColumn = (columnNum == it -> y);
	}
	if (sameRow) {
		return "row";
	}
	if (sameColumn) {
		return "column";
	}
	return "none";
}

void eliminateSquarePossibilitiesByTwoNeighborsType(list<coordinate> possbilities, int content) {
  string type = checkNeighborsType(possbilities);
	if (type == "none") {
		return;
	}
	if (type == "row") {
    coordinate cor = possbilities.front(), secondCor = possbilities.back();
		coordinate* neighbors = getNeighborRow(cor);
		for (int i = 0; i < 8; i++) {
			if (neighbors[i].y != secondCor.y) {
				removePossibilityFromCoordinate(neighbors[i], content);
			}
		}
	}
	if (type == "column") {
    coordinate cor = possbilities.front(), secondCor = possbilities.back();
		coordinate* neighbors = getNeighborColumn(cor);
		for (int i = 0; i < 8; i++) {
			if (neighbors[i].x != secondCor.x) {
				removePossibilityFromCoordinate(neighbors[i], content);
			}
		}
	}
}

list<coordinate> crossCoordinatesList(list<coordinate> list1, list<coordinate> list2) {
  list<coordinate>::iterator it;
	list<coordinate> crossedList = list2;
	for (it = list1.begin(); it != list1.end(); it++) {
		if (!checkIfListContainsItem<coordinate>(list2, *it)) {
      crossedList.push_back(*it);
		}
	}
	return crossedList;
}

struct nakedGroup {
  list<int> numbers;
	list<coordinate> coordinates;
};

nakedGroup revertNakedGroup(nakedGroup group) {
	group.numbers.pop_back();
	list<coordinate> coordinatesList = group.coordinates;
	list<coordinate>::iterator coordinatesIterator;
	for (coordinatesIterator = coordinatesList.begin(); coordinatesIterator != coordinatesList.end(); coordinatesIterator++) {
		coordinate cor = *coordinatesIterator;
		if (!checkIfListContainsAnyOfTheItems<int>(possibilitiesArray[cor.x][cor.y], group.numbers)) {
			group.coordinates.remove(cor);
		}
	}
	return group;
}

nakedGroup checkNakedGroupExistence(list<coordinate>* numbersOccuerence, nakedGroup group, int numberToCheck, int groupSize) {
	if (checkIfListContainsItem<int>(group.numbers, numberToCheck)) {
		return checkNakedGroupExistence(numbersOccuerence, group, numberToCheck + 1, groupSize);
	}
	if (numberToCheck > 9) {
		if (group.numbers.size() == 1 || group.numbers.size() == groupSize) {
			return group;
		}
		int lastNumberAdded = group.numbers.back();
		nakedGroup oldGroup = revertNakedGroup(group);
		return checkNakedGroupExistence(numbersOccuerence, oldGroup, lastNumberAdded + 1, groupSize);
	}
	if (numbersOccuerence[numberToCheck - 1].size() < 2) {
		return checkNakedGroupExistence(numbersOccuerence, group, numberToCheck + 1, groupSize);
	}
	list<coordinate> crossedCoordinatesList = crossCoordinatesList(group.coordinates, numbersOccuerence[numberToCheck - 1]);
	if (crossedCoordinatesList.size() > groupSize) {
		return checkNakedGroupExistence(numbersOccuerence, group, numberToCheck + 1, groupSize);
	}
	if (group.numbers.size() + 1 == groupSize && crossedCoordinatesList.size() < groupSize) {
		return checkNakedGroupExistence(numbersOccuerence, group, numberToCheck + 1, groupSize);
	}
	group.coordinates = crossedCoordinatesList;
	group.numbers.push_back(numberToCheck);
	if (crossedCoordinatesList.size() < groupSize || group.numbers.size() < groupSize) {
		return checkNakedGroupExistence(numbersOccuerence, group, numberToCheck + 1, groupSize);
	}
	return group;
}

void eliminatePossibilitisBasedOnNakedGroup(nakedGroup group, coordinate* neighbors) {
  list<coordinate> coordinatesList = group.coordinates;
	list<coordinate>::iterator coordinatesIterator;
	for (int i = 0; i < 9; i++) {
		coordinate cor = neighbors[i];
		if (checkIfListContainsItem<coordinate>(group.coordinates, cor)) {
			keepOnlyPossibilitiesInCoordinate(cor, group.numbers);
		}
		else {
			removePossibilitiesFromCoordinate(cor, group.numbers);
		}
	}
}

void eliminateNeighborsPossibilitiesByNakedGroups(coordinate* neighbors) {
  list<coordinate>* numbersOccuerences = getNumbersPossibleOccurences(neighbors);
	for (int i = 0; i < 9; i++) {
		list<int> numbers;
		numbers.push_back(i + 1);
		if (numbersOccuerences[i].size() > 1) {
			nakedGroup group = nakedGroup{ numbers, numbersOccuerences[i] };
			for (int j = 2; j < 9; j++) {
				nakedGroup newGroup = checkNakedGroupExistence(numbersOccuerences, group, i + 2, j);
				if (newGroup.numbers.size() > 1) {
					eliminatePossibilitisBasedOnNakedGroup(newGroup, neighbors);
				}
			}
		}
	}
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

void eliminatePossbilitiesByNakedGroups() {
	for (int m = 0; m < 9; m++) {
		coordinate* neighbors = getNeighborsByType(coordinate{0, m}, "column", true);
		eliminateNeighborsPossibilitiesByNakedGroups(neighbors);
		neighbors = getNeighborsByType(coordinate{m, 0}, "row", true);
		eliminateNeighborsPossibilitiesByNakedGroups(neighbors);
		neighbors = getNeighborsByType(coordinate{(m % 3) * 3 , (m / 3) * 3}, "square", true);
		eliminateNeighborsPossibilitiesByNakedGroups(neighbors);
	}
}

void eliminatePossibilitiesByTwoNeighborsType() {
  for (int m = 0; m < 9; m++) {
		coordinate* sqaure = getNeighborSquare(coordinate{(m % 3) * 3 , (m / 3) * 3}, true);
		list<coordinate>* numbersPossibleOccurence = getNumbersPossibleOccurences(sqaure);
		for (int i = 0; i < 9; i++) {
			if (numbersPossibleOccurence[i].size() == 2) {
				eliminateSquarePossibilitiesByTwoNeighborsType(numbersPossibleOccurence[i], i + 1);
			}
		}
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
	eliminatePossibilitiesByTwoNeighborsType();
	eliminatePossbilitiesByNakedGroups();
}

void updateNeighborsOfChange(coordinate currentCoordinate) {
  int content = getCellContent(currentCoordinate);
	coordinate* neighbors = getNeighbors(currentCoordinate);
	for (int i = 0; i < 20; i++) {
		coordinate neighbor = neighbors[i];
		removePossibilityFromCoordinate(neighbor, content);
	}
}

bool updateBoardByPossibilties(bool lastUpdated) {
	bool updated = false;
  updatePossibilitieArrays();
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (getCellContent(coordinate{i, j}) == 0 && possibilitiesArray[i][j].size() == 1) {
			  updated = true;
				insertToCoordinate(coordinate{ i, j }, possibilitiesArray[i][j].front());
				updateNeighborsOfChange(coordinate{i,j});
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

bool updateNeighborsByPossibleOccurenes(coordinate* neighbors) {
  bool updated = false;
	list<coordinate>* list = getNumbersPossibleOccurences(neighbors);
  for (int i = 0; i < 9; i++) {
	  if (list[i].size() == 1) {
	    coordinate toPlace = list[i].back();		
			insertToCoordinate(toPlace, i +1);
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