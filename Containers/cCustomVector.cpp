/**
 * @file cCustomVector.cpp
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 *
 * @section DESCRIPTION
 *
 * Implementation of the cCutomVector class.
 */

#include <stdexcept>
#include <time.h>
#include <Windows.h>
#include <psapi.h>
#include <string>
#include <fstream>
#include <iostream>

#include "cCustomVector.h"

unsigned int cCustomVector::mINIT_CAPACITY = 50;

cCustomVector::cCustomVector() {

	this->mSize = 0;
	this->mCapacity = cCustomVector::mINIT_CAPACITY;
	this->mData = new sPerson[this->mCapacity];

	return;
}

cCustomVector::~cCustomVector() {

	delete[] this->mData;

	return;
}

void cCustomVector::push_back(const sPerson& person) {

	// Check if the array needs to be resized
	if (this->mSize == this->mCapacity) {

		this->resize(this->mCapacity * 2);
	}

	// Add the new value to the array
	this->mData[this->mSize] = person;
	this->mSize++;

	return;
}

void cCustomVector::resize(size_t size) {

	// Create a new array of sPerson based on the new size
	sPerson* tempVector = new sPerson[size];

	// Copy the current data into the new one
	for (size_t index = 0; index < this->mSize; ++index) {
		tempVector[index] = this->mData[index];
	}

	// Delete the old data and point the data to the temp array
	delete[] this->mData;
	this->mData = tempVector;
	this->mCapacity = size;

	return;
}

size_t cCustomVector::size() const {

	return this->mSize;
}

size_t cCustomVector::capacity() const {
	
	return this->mCapacity;
}

void cCustomVector::clear() {

	// Free all the data
	delete[] this->mData;

	// Set everything to the initial state
	this->mSize = 0;
	this->mData = new sPerson[cCustomVector::mINIT_CAPACITY];
	this->mCapacity = cCustomVector::mINIT_CAPACITY;

	return;
}

sPerson& cCustomVector::operator[](size_t index) {
	
	// Throw an exception if an invalid index is referenced
	if (this->mSize == 0) {

		throw new std::runtime_error("Custom Vector out of range: size = 0\n");
	}
	else if (index > this->mSize - 1) {

		throw new std::runtime_error("Custom Vector out of range: index > size\n");
	}
	else {

		this->mData[index];
	}
}

const sPerson& cCustomVector::operator[](size_t index) const {
	
	// Throw an exception if an invalid index is referenced
	if (this->mSize == 0) {

		throw new std::runtime_error("Custom Vector out of range: size = 0\n");
	}
	else if (index > this->mSize - 1) {

		throw new std::runtime_error("Custom Vector out of range: index > size\n");
	}
	else {

		this->mData[index];
	}
}

sPerfData customVecPerformanceData;
void cCustomVector::generatePeople(int numPeople) {

	if (numPeople == INT_MAX) {

		this->mGenerateAllPeople();
	}
	else {

		this->mGenerateNumPeople(numPeople);
	}

	return;
}

// ****** Interface functions start ********//
std::vector<std::string>cvMaleNames;
std::vector<std::string>cvFemaleNames;
std::vector<std::string>cvLastNames;

bool cCustomVector::LoadDataFilesIntoContainer(std::string firstNameFemaleFileName, std::string firstNameMaleFileName, std::string lastNameFileName) {
	
	// Start recording performance data
	this->mStartPerformanceData(customVecPerformanceData);

	// Read the female first names into the array
	std::ifstream file(firstNameFemaleFileName.c_str());
	if (!file.is_open()) {

		std::cout << "Did not open " << firstNameFemaleFileName << std::endl;
		this->mEndPerformanceData(customVecPerformanceData);
		return false;
	}

	std::string placeHolder;
	while (!file.eof()) {

		placeHolder = "";
		file >> placeHolder;
		if (placeHolder != "") cvFemaleNames.push_back(placeHolder);
		file >> placeHolder;
		file >> placeHolder;
		file >> placeHolder;

		this->mUpdatePerformanceData(customVecPerformanceData);
	}

	file.close();

	// Read the male names into the array
	file.open(firstNameMaleFileName.c_str());
	if (!file.is_open()) {

		std::cout << "Did not open " << firstNameMaleFileName << std::endl;
		this->mEndPerformanceData(customVecPerformanceData);
		return false;
	}

	while (!file.eof()) {

		placeHolder = "";
		file >> placeHolder;
		if (placeHolder != "") cvMaleNames.push_back(placeHolder);
		file >> placeHolder;
		file >> placeHolder;
		file >> placeHolder;

		this->mUpdatePerformanceData(customVecPerformanceData);
	}

	file.close();

	// Read the last names into the array
	file.open(lastNameFileName.c_str());
	if (!file.is_open()) {

		std::cout << "Did not open " << lastNameFileName << std::endl;
		this->mEndPerformanceData(customVecPerformanceData);
		return false;
	}

	while (!file.eof()) {

		placeHolder = "";
		file >> placeHolder;
		if (placeHolder != "") cvLastNames.push_back(placeHolder);
		file >> placeHolder;
		file >> placeHolder;
		file >> placeHolder;

		this->mUpdatePerformanceData(customVecPerformanceData);
	}

	file.close();

	// Check that there are first names to use
	if (cvFemaleNames.empty() && cvMaleNames.empty()) {

		this->mEndPerformanceData(customVecPerformanceData);
		return false;
	}

	// Check to make sure we have last names
	if (cvLastNames.empty()) {

		this->mEndPerformanceData(customVecPerformanceData);
		return false;
	}

	this->mEndPerformanceData(customVecPerformanceData);

	return true;
}

bool cCustomVector::FindPeopleByName(std::vector<sPerson>& vecPeople, sPerson personToMatch, int maxNumberOfPeople) {
	
	// Start recording performance data
	this->mStartPerformanceData(customVecPerformanceData);

	vecPeople.clear();

	if (maxNumberOfPeople < 0) {

		this->mEndPerformanceData(customVecPerformanceData);
		return false;
	}

	size_t vecSize = this->mSize;

	// if both names are blank, return everyone
	if (personToMatch.first == "" && personToMatch.last == "") {

		for (int index = 0; index < vecSize; ++index) {

			this->mUpdatePerformanceData(customVecPerformanceData);


			if ((int)vecPeople.size() >= maxNumberOfPeople) {

				this->mEndPerformanceData(customVecPerformanceData);
				return true;
			}

			vecPeople.push_back(this->mData[index]);
		}

		if (!vecPeople.empty()) {

			this->mEndPerformanceData(customVecPerformanceData);
			return true;
		}
		else {

			this->mEndPerformanceData(customVecPerformanceData);
			return false;
		}
	}

	// if last name is "", search only by first name
	if (personToMatch.last == "") {

		for (int index = 0; index < vecSize; ++index) {

			this->mUpdatePerformanceData(customVecPerformanceData);

			if (personToMatch.first == this->mData[index].first) {

				if ((int)vecPeople.size() >= maxNumberOfPeople) {

					this->mEndPerformanceData(customVecPerformanceData);
					return true;
				}
				vecPeople.push_back(this->mData[index]);
			}
		}

		if (!vecPeople.empty()) {

			this->mEndPerformanceData(customVecPerformanceData);
			return true;
		}
		else {

			this->mEndPerformanceData(customVecPerformanceData);
			return false;
		}
	}

	// if first name is "", search only by last name
	if (personToMatch.first == "") {

		for (int index = 0; index < vecSize; ++index) {

			this->mUpdatePerformanceData(customVecPerformanceData);
			if (personToMatch.last == this->mData[index].last) {

				if ((int)vecPeople.size() >= maxNumberOfPeople) {

					this->mUpdatePerformanceData(customVecPerformanceData);
					return true;
				}
				vecPeople.push_back(this->mData[index]);
			}
		}

		if (!vecPeople.empty()) {

			this->mEndPerformanceData(customVecPerformanceData);
			return true;
		}
		else {

			this->mEndPerformanceData(customVecPerformanceData);
			return false;
		}
	}

	// Otherwise compare both first and last name
	for (int index = 0; index < vecSize; ++index) {

		this->mUpdatePerformanceData(customVecPerformanceData);
		if (personToMatch.first == this->mData[index].first &&
			personToMatch.last == this->mData[index].last) {

			if ((int)vecPeople.size() >= maxNumberOfPeople) {

				this->mUpdatePerformanceData(customVecPerformanceData);
				return true;
			}
			vecPeople.push_back(this->mData[index]);
		}
	}

	if (!vecPeople.empty()) {

		this->mEndPerformanceData(customVecPerformanceData);
		return true;
	}
	else {

		this->mEndPerformanceData(customVecPerformanceData);
		return false;
	}
}

bool cCustomVector::FindPeopleByName(std::vector<sPerson>& vecPeople, std::vector<sPerson>& vecPeopleToMatch, int maxNumberOfPeople) {
	
	// Start recording performance data
	this->mStartPerformanceData(customVecPerformanceData);

	vecPeople.clear();

	if (maxNumberOfPeople < 0) {

		this->mEndPerformanceData(customVecPerformanceData);
		return false;
	}

	size_t vecSize = this->mSize;

	for (int people = 0; people < vecPeopleToMatch.size(); ++people) {

		this->mUpdatePerformanceData(customVecPerformanceData);

		sPerson vecCurrPerson = vecPeopleToMatch[people];

		// if both names are blank, return everyone
		if (vecCurrPerson.first == "" && vecCurrPerson.last == "") {

			for (int index = 0; index < vecSize; ++index) {

				this->mUpdatePerformanceData(customVecPerformanceData);

				if ((int)vecPeople.size() >= maxNumberOfPeople) {

					this->mUpdatePerformanceData(customVecPerformanceData);
					break;
				}

				vecPeople.push_back(this->mData[index]);
			}

			this->mEndPerformanceData(customVecPerformanceData);
			return true;
		}

		// if last name is "", search only by first name
		if (vecCurrPerson.last == "") {

			for (int index = 0; index < vecSize; ++index) {

				this->mUpdatePerformanceData(customVecPerformanceData);

				if (vecCurrPerson.first == this->mData[index].first) {

					if ((int)vecPeople.size() >= maxNumberOfPeople) {

						this->mUpdatePerformanceData(customVecPerformanceData);
						break;
					}
					vecPeople.push_back(this->mData[index]);
				}
			}

			this->mUpdatePerformanceData(customVecPerformanceData);
			continue;
		}

		// if first name is "", search only by last name
		if (vecCurrPerson.first == "") {

			for (int index = 0; index < vecSize; ++index) {

				this->mUpdatePerformanceData(customVecPerformanceData);
				if (vecCurrPerson.last == this->mData[index].last) {

					if ((int)vecPeople.size() >= maxNumberOfPeople) {

						this->mUpdatePerformanceData(customVecPerformanceData);
						break;
					}
					vecPeople.push_back(this->mData[index]);
				}
			}

			this->mUpdatePerformanceData(customVecPerformanceData);
			continue;
		}

		// Otherwise compare both first and last name
		for (int index = 0; index < vecSize; ++index) {

			this->mUpdatePerformanceData(customVecPerformanceData);
			if (vecCurrPerson.first == this->mData[index].first &&
				vecCurrPerson.last == this->mData[index].last) {

				if ((int)vecPeople.size() >= maxNumberOfPeople) {

					this->mUpdatePerformanceData(customVecPerformanceData);
					break;
				}
				vecPeople.push_back(this->mData[index]);
			}
		}

		this->mUpdatePerformanceData(customVecPerformanceData);
		continue;
	}

	if (!vecPeople.empty()) {

		this->mEndPerformanceData(customVecPerformanceData);
		return true;
	}
	else {

		this->mEndPerformanceData(customVecPerformanceData);
		return false;
	}
}

bool cCustomVector::FindPersonByID(sPerson& person, unsigned long long uniqueID) {
	
	// Start recording performance data
	this->mStartPerformanceData(customVecPerformanceData);

	size_t vecSize = this->mSize;
	for (size_t index = 0; index < vecSize; ++index) {

		this->mUpdatePerformanceData(customVecPerformanceData);
		if (this->mData[index].uniqueID == uniqueID) {
			
			person = this->mData[index];
			this->mEndPerformanceData(customVecPerformanceData);
			return true;
		}
	}
	this->mEndPerformanceData(customVecPerformanceData);

	return false;
}

bool cCustomVector::FindPeople(std::vector<sPerson>& vecPeople, sPoint location, float radius, int maxPeopleToReturn) {
	
	// Start recording performance data
	this->mStartPerformanceData(customVecPerformanceData);
	vecPeople.clear();

	if (maxPeopleToReturn < 0) {

		this->mEndPerformanceData(customVecPerformanceData);
		return false;
	}

	size_t vecSize = this->mSize;
	float distance;
	for (size_t index = 0; index < vecSize; ++index) {

		this->mUpdatePerformanceData(customVecPerformanceData);
		distance = 0.0f;
		if (this->mIsInsideSphere(location, radius, this->mData[index], distance)) {

			if ((int)vecPeople.size() >= maxPeopleToReturn) {

				this->mUpdatePerformanceData(customVecPerformanceData);
				break;
			}

			vecPeople.push_back(this->mData[index]);
		}
	}

	if (!vecPeople.empty()) {

		this->mEndPerformanceData(customVecPerformanceData);
		return true;
	}
	else {

		this->mEndPerformanceData(customVecPerformanceData);
		return false;
	}
}

bool cCustomVector::FindPeople(std::vector<sPerson>& vecPeople, float minHealth, float maxHealth, int maxPeopleToReturn) {
	
	// Start recording performance data
	this->mStartPerformanceData(customVecPerformanceData);
	vecPeople.clear();

	size_t vecSize = this->mSize;
	for (size_t index = 0; index < vecSize; ++index) {

		this->mUpdatePerformanceData(customVecPerformanceData);
		if ((int)vecPeople.size() > maxPeopleToReturn) {

			this->mEndPerformanceData(customVecPerformanceData);
			return true;
		}

		if (this->mData[index].health >= minHealth && this->mData[index].health <= maxHealth) {

			vecPeople.push_back(this->mData[index]);
			this->mUpdatePerformanceData(customVecPerformanceData);
		}
	}

	if (!vecPeople.empty()) {

		this->mEndPerformanceData(customVecPerformanceData);
		return true;
	}
	else {

		this->mEndPerformanceData(customVecPerformanceData);
		return false;
	}
}

bool cCustomVector::FindPeople(std::vector<sPerson>& vecPeople, sPoint location, float radius, float minHealth, float maxHealth, int maxPeopleToReturn) {
	
	// Start recording performance data
	this->mStartPerformanceData(customVecPerformanceData);
	vecPeople.clear();

	if (maxPeopleToReturn < 0) {

		this->mEndPerformanceData(customVecPerformanceData);
		return false;
	}

	size_t vecSize = this->mSize;
	float distance;
	for (size_t index = 0; index < vecSize; ++index) {

		if (this->mData[index].health >= minHealth && this->mData[index].health <= maxHealth) {

			if (this->mIsInsideSphere(location, radius, this->mData[index], distance)) {

				if ((int)vecPeople.size() > maxPeopleToReturn) {

					this->mEndPerformanceData(customVecPerformanceData);
					return true;
				}

				vecPeople.push_back(this->mData[index]);
				this->mUpdatePerformanceData(customVecPerformanceData);
			}
		}
	}

	if (!vecPeople.empty()) {

		this->mEndPerformanceData(customVecPerformanceData);
		return true;
	}
	else {

		this->mEndPerformanceData(customVecPerformanceData);
		return false;
	}
}

bool cCustomVector::SortPeople(std::vector<sPerson>& vecPeople, eSortType sortType) {
	
	// Start recording performance data
	this->mStartPerformanceData(customVecPerformanceData);

	vecPeople.clear();
	if (this->mSize == 0) {

		this->mEndPerformanceData(customVecPerformanceData);
		return false;
	}

	switch (sortType) {
	case iPersonMotron::ASC_FIRST_THEN_LAST:
		this->mSortFirstLastNamesAsc();
		this->mUpdatePerformanceData(customVecPerformanceData);
		break;
	case iPersonMotron::DESC_FIRST_THEN_LAST:
		this->mSortFirstLastNamesDesc();
		this->mUpdatePerformanceData(customVecPerformanceData);
		break;
	case iPersonMotron::ASC_LAST_THEN_FIRST:
		this->mSortLastFirstNamesAsc();
		this->mUpdatePerformanceData(customVecPerformanceData);
		break;
	case iPersonMotron::DESC_LAST_THEN_FIRST:
		this->mSortLastFirstNamesDesc();
		this->mUpdatePerformanceData(customVecPerformanceData);
		break;
	case iPersonMotron::ASC_BY_ID:
		this->mSortIdAsc();
		this->mUpdatePerformanceData(customVecPerformanceData);
		break;
	case iPersonMotron::DESC_BY_ID:
		this->mSortIdDesc();
		this->mUpdatePerformanceData(customVecPerformanceData);
		break;
	case iPersonMotron::ASC_BY_HEALTH:
		this->mSortHealthAsc();
		this->mUpdatePerformanceData(customVecPerformanceData);
		break;
	case iPersonMotron::DESC_BY_HEALTH:
		this->mSortHealthDesc();
		this->mUpdatePerformanceData(customVecPerformanceData);
		break;
	default:
		break;
	}

	for (size_t index = 0; index < this->mSize; ++index) {

		vecPeople.push_back(this->mData[index]);
	}
	
	this->mEndPerformanceData(customVecPerformanceData);
	return true;
}

bool cCustomVector::GetPerformanceFromLastCall(sPerfData& callStats) {
	
	callStats = customVecPerformanceData;
	
	return true;
}

eContainerType cCustomVector::getContainerType(void) {
	
	this->mStartPerformanceData(customVecPerformanceData);

	this->mEndPerformanceData(customVecPerformanceData);

	return eContainerType::CUSTOM_DIY_VECTOR;
}
// ***** Interface functions end *************//

// ***** Sort functions start *************//

// Use a static number to control the sort recursion
static size_t recursionLvl = 0;

void cCustomVector::mVectorQuickSort(long long left, long long right, eSortType sortType) {

	long long i = left;
	long long j = right;
	sPerson tempPerson;
	sPerson pivot = this->mData[(left + right) / 2];

	recursionLvl++;

	switch (sortType) {
	case iPersonMotron::ASC_FIRST_THEN_LAST:
		// Partition
		while (i <= j) {

			while (this->mData[i].first < pivot.first) {
				i++;
			}

			while (this->mData[j].first > pivot.first) {
				j--;
			}

			if (i <= j) {

				tempPerson = this->mData[i];
				this->mData[i] = this->mData[j];
				this->mData[j] = tempPerson;
				i++;
				j--;
			}
		}

		// Recursion
		if (left < j) {

			this->mVectorQuickSort(left, j, ASC_FIRST_THEN_LAST);
		}

		if (i < right) {

			this->mVectorQuickSort(i, right, ASC_FIRST_THEN_LAST);
		}

		// Bubble sort for last name
		if (recursionLvl == 1) {

			bool bIsFinished = false;
			while (!bIsFinished) {

				bIsFinished = true;
				size_t vecLength = this->mSize - 1; // Don't need to test last value
				for (size_t index = 0; index < vecLength; ++index) {

					if (this->mData[index].last > this->mData[index + 1].last &&
						this->mData[index].first == this->mData[index + 1].first) {

						tempPerson = this->mData[index];
						this->mData[index] = this->mData[index + 1];
						this->mData[index + 1] = tempPerson;

						bIsFinished = false;
					}
				}
			}
		}
		break;
	case iPersonMotron::DESC_FIRST_THEN_LAST:
		// Partition
		while (i <= j) {

			while (this->mData[i].first > pivot.first) {
				i++;
			}

			while (this->mData[j].first < pivot.first) {
				j--;
			}

			if (i <= j) {

				tempPerson = this->mData[i];
				this->mData[i] = this->mData[j];
				this->mData[j] = tempPerson;
				i++;
				j--;
			}
		}

		// Recursion
		if (left < j) {

			this->mVectorQuickSort(left, j, DESC_FIRST_THEN_LAST);
		}

		if (i < right) {

			this->mVectorQuickSort(i, right, DESC_FIRST_THEN_LAST);
		}

		// Bubble sort last names
		if (recursionLvl == 1) {

			bool bIsFinished = false;
			while (!bIsFinished) {

				bIsFinished = true;
				size_t vecLength = this->mSize - 1;
				for (size_t index = 0; index < vecLength; ++index) {

					if (this->mData[index].last < this->mData[index + 1].last &&
						this->mData[index].first == this->mData[index + 1].first) {

						tempPerson = this->mData[index];
						this->mData[index] = this->mData[index + 1];
						this->mData[index + 1] = tempPerson;
						bIsFinished = false;
					}
				}
			}
		}

		break;
	case iPersonMotron::ASC_LAST_THEN_FIRST:
		// Partition
		while (i <= j) {

			while (this->mData[i].last < pivot.last) {
				i++;
			}

			while (this->mData[j].last > pivot.last) {
				j--;
			}

			if (i <= j) {

				tempPerson = this->mData[i];
				this->mData[i] = this->mData[j];
				this->mData[j] = tempPerson;
				i++;
				j--;
			}
		}

		// Recursion
		if (left < j) {

			this->mVectorQuickSort(left, j, ASC_LAST_THEN_FIRST);
		}

		if (i < right) {

			this->mVectorQuickSort(i, right, ASC_LAST_THEN_FIRST);
		}

		// Bubble sort for first name
		if (recursionLvl == 1) {

			bool bIsFinished = false;
			while (!bIsFinished) {

				bIsFinished = true;
				size_t vecLength = this->mSize - 1; // Don't need to test last value
				for (size_t index = 0; index < vecLength; ++index) {

					if (this->mData[index].first > this->mData[index + 1].first &&
						this->mData[index].last == this->mData[index + 1].last) {

						tempPerson = this->mData[index];
						this->mData[index] = this->mData[index + 1];
						this->mData[index + 1] = tempPerson;
						bIsFinished = false;
					}
				}
			}
		}
		break;
	case iPersonMotron::DESC_LAST_THEN_FIRST:
		// Partition
		while (i <= j) {

			while (this->mData[i].last > pivot.last) {
				i++;
			}

			while (this->mData[j].last < pivot.last) {
				j--;
			}

			if (i <= j) {

				tempPerson = this->mData[i];
				this->mData[i] = this->mData[j];
				this->mData[j] = tempPerson;
				i++;
				j--;
			}
		}

		// Recursion
		if (left < j) {

			this->mVectorQuickSort(left, j, DESC_LAST_THEN_FIRST);
		}

		if (i < right) {

			this->mVectorQuickSort(i, right, DESC_LAST_THEN_FIRST);
		}

		// Bubble sort first names
		if (recursionLvl == 1) {

			bool bIsFinished = false;
			while (!bIsFinished) {

				bIsFinished = true;
				size_t vecLength = this->mSize - 1;
				for (size_t index = 0; index < vecLength; ++index) {

					if (this->mData[index].first < this->mData[index + 1].first &&
						this->mData[index].last == this->mData[index + 1].last) {

						tempPerson = this->mData[index];
						this->mData[index] = this->mData[index + 1];
						this->mData[index + 1] = tempPerson;
						bIsFinished = false;
					}
				}
			}
		}
		break;
	case iPersonMotron::ASC_BY_ID:
		// Partition
		while (i <= j) {

			while (this->mData[i].uniqueID < pivot.uniqueID) {
				i++;
			}

			while (this->mData[j].uniqueID > pivot.uniqueID) {
				j--;
			}

			if (i <= j) {

				tempPerson = this->mData[i];
				this->mData[i] = this->mData[j];
				this->mData[j] = tempPerson;
				i++;
				j--;
			}
		}

		// Recursion
		if (left < j) {

			this->mVectorQuickSort(left, j, ASC_BY_ID);
		}

		if (i < right) {

			this->mVectorQuickSort(i, right, ASC_BY_ID);
		}
		break;
	case iPersonMotron::DESC_BY_ID:
		// Partition
		while (i <= j) {

			while (this->mData[i].uniqueID > pivot.uniqueID) {
				i++;
			}

			while (this->mData[j].uniqueID < pivot.uniqueID) {
				j--;
			}

			if (i <= j) {

				tempPerson = this->mData[i];
				this->mData[i] = this->mData[j];
				this->mData[j] = tempPerson;
				i++;
				j--;
			}
		}

		// Recursion
		if (left < j) {

			this->mVectorQuickSort(left, j, DESC_BY_ID);
		}

		if (i < right) {

			this->mVectorQuickSort(i, right, DESC_BY_ID);
		}
		break;
	case iPersonMotron::ASC_BY_HEALTH:
		// Partition
		while (i <= j) {

			while (this->mData[i].health < pivot.health) {
				i++;
			}

			while (this->mData[j].health > pivot.health) {
				j--;
			}

			if (i <= j) {

				tempPerson = this->mData[i];
				this->mData[i] = this->mData[j];
				this->mData[j] = tempPerson;
				i++;
				j--;
			}
		}

		// Recursion
		if (left < j) {

			this->mVectorQuickSort(left, j, ASC_BY_HEALTH);
		}

		if (i < right) {

			this->mVectorQuickSort(i, right, ASC_BY_HEALTH);
		}
		break;
	case iPersonMotron::DESC_BY_HEALTH:
		// Partition
		while (i <= j) {

			while (this->mData[i].health > pivot.health) {
				i++;
			}

			while (this->mData[j].health < pivot.health) {
				j--;
			}

			if (i <= j) {

				tempPerson = this->mData[i];
				this->mData[i] = this->mData[j];
				this->mData[j] = tempPerson;
				i++;
				j--;
			}
		}

		// Recursion
		if (left < j) {

			this->mVectorQuickSort(left, j, DESC_BY_HEALTH);
		}

		if (i < right) {

			this->mVectorQuickSort(i, right, DESC_BY_HEALTH);
		}
		break;
	default:
		break;
	}

	return;
}

void cCustomVector::mSortFirstLastNamesAsc() {

	this->mVectorQuickSort(0, this->mSize - 1, ASC_FIRST_THEN_LAST);

	return;
}

void cCustomVector::mSortFirstLastNamesDesc() {

	this->mVectorQuickSort(0, this->mSize - 1, DESC_FIRST_THEN_LAST);

	return;
}

void cCustomVector::mSortLastFirstNamesAsc() {

	this->mVectorQuickSort(0, this->mSize - 1, ASC_LAST_THEN_FIRST);

	return;
}

void cCustomVector::mSortLastFirstNamesDesc() {

	this->mVectorQuickSort(0, this->mSize - 1, DESC_LAST_THEN_FIRST);

	return;
}

void cCustomVector::mSortIdAsc() {

	this->mVectorQuickSort(0, this->mSize - 1, ASC_BY_ID);

	return;
}

void cCustomVector::mSortIdDesc() {

	this->mVectorQuickSort(0, this->mSize - 1, DESC_BY_ID);

	return;
}

void cCustomVector::mSortHealthAsc() {

	this->mVectorQuickSort(0, this->mSize - 1, ASC_BY_HEALTH);

	return;
}

void cCustomVector::mSortHealthDesc() {

	this->mVectorQuickSort(0, this->mSize - 1, DESC_BY_HEALTH);

	return;
}
// ***** Sort functions end *************//

// ***** Performance functions start *************//
clock_t startTime;

void cCustomVector::mStartPerformanceData(sPerfData& pd) {

	// Start the clock
	startTime = clock();

	// Clear the performance data to be safe
	pd.elapsedCallTime_ms = 0.0f;
	pd.memoryUsageBytes_min = 0.0f;
	pd.memoryUsageBytes_max = 0.0f;
	pd.memoryUsageBytes_avg = 0.0f;

	HANDLE process;
	PROCESS_MEMORY_COUNTERS counter;

	process = GetCurrentProcess();
	if (process == nullptr) return;

	if (GetProcessMemoryInfo(process, &counter, sizeof(counter))) {

		pd.memoryUsageBytes_min = counter.WorkingSetSize;
		pd.memoryUsageBytes_max = counter.WorkingSetSize;
		pd.memoryUsageBytes_avg = counter.WorkingSetSize;
	}
	else {

		printf("There was an error getting performance data.\n");
	}

	return;
}

void cCustomVector::mUpdatePerformanceData(sPerfData& pd) {

	// Update elapsed time
	pd.elapsedCallTime_ms = ((float)(clock() - startTime) / CLOCKS_PER_SEC) * 1000.0f;

	// Update performance data
	SIZE_T minMemUse = pd.memoryUsageBytes_min;
	SIZE_T maxMemUse = pd.memoryUsageBytes_max;

	HANDLE process;
	PROCESS_MEMORY_COUNTERS counter;

	process = GetCurrentProcess();
	if (process == nullptr) return;

	if (GetProcessMemoryInfo(process, &counter, sizeof(counter))) {

		if (minMemUse > counter.WorkingSetSize) {

			pd.memoryUsageBytes_min = counter.WorkingSetSize;
		}

		if (maxMemUse < counter.WorkingSetSize) {
			pd.memoryUsageBytes_max = counter.WorkingSetSize;
		}

		pd.memoryUsageBytes_avg = (pd.memoryUsageBytes_min + pd.memoryUsageBytes_max) / 2.0f;
	}
	else {

		printf("There was error getting performance data.\n");
	}

	return;
}

void cCustomVector::mEndPerformanceData(sPerfData& pd) {

	// Update elapsed time
	pd.elapsedCallTime_ms = ((float)(clock() - startTime) / CLOCKS_PER_SEC) * 1000.0f;

	// Update performance data
	SIZE_T minMemUse = pd.memoryUsageBytes_min;
	SIZE_T maxMemUse = pd.memoryUsageBytes_max;

	HANDLE process;
	PROCESS_MEMORY_COUNTERS counter;

	process = GetCurrentProcess();
	if (process == nullptr) return;

	if (GetProcessMemoryInfo(process, &counter, sizeof(counter))) {

		if (minMemUse > counter.WorkingSetSize) {

			pd.memoryUsageBytes_min = counter.WorkingSetSize;
		}

		if (maxMemUse < counter.WorkingSetSize) {
			pd.memoryUsageBytes_max = counter.WorkingSetSize;
		}

		pd.memoryUsageBytes_avg = (pd.memoryUsageBytes_min + pd.memoryUsageBytes_max) / 2.0f;
	}
	else {

		printf("There was error getting performance data.\n");
	}

	return;
}
// ***** Performance functions end *************//


bool cCustomVector::mIsInsideSphere(sPoint point, float radius, sPerson person, float& distance) {
	
	// Get the vector
	float vecX = person.location.x - point.x;
	float vecY = person.location.y - point.y;
	float vecZ = person.location.z - point.z;

	// Vector length
	float vecLength = sqrt(pow(vecX, 2) + pow(vecY, 2) + pow(vecZ, 2));

	if (vecLength <= radius) {

		distance = vecLength;
		return true;
	}
	else {

		return false;
	}
}

void cCustomVector::mGenerateAllPeople() {

	this->mStartPerformanceData(customVecPerformanceData);

	// Now generate the sPerson objects
	size_t ID = 0;
	srand(time(NULL));

	// Generate all the males with a random last name
	for (size_t index = 0; index < cvMaleNames.size(); ++index) {

		sPerson newPerson;
		newPerson.uniqueID = ID;
		int maleIndex = rand() % cvMaleNames.size();
		newPerson.first = cvMaleNames[maleIndex];

		int lastIndex = rand() % cvLastNames.size();
		newPerson.last = cvLastNames[lastIndex];

		int age = rand() % 95 + 1;
		newPerson.age = age;

		// Random health from 0 to 100
		float health = (rand() % 100) + (rand() % 100001 / 100000.0f);
		newPerson.health = health;

		// Generate a random position
		// varying from -500 to 500 in the X and Z
		// axes and  0 to 100 in the y
		float x = ((rand() % 1001) + (rand() % 1001 / 1000.0f)) - 500.0f;
		float y = (rand() % 101) + (rand() % 1001 / 1000.0f);
		float z = ((rand() % 1001) + (rand() % 1001 / 1000.0f)) - 500.0f;

		sPoint location;
		location.x = x;
		location.y = y;
		location.z = z;
		newPerson.location = location;

		this->push_back(newPerson);
		this->mUpdatePerformanceData(customVecPerformanceData);

		ID++;
		this->mUpdatePerformanceData(customVecPerformanceData);
	}

	// Now create all the female names
	for (size_t index = 0; index < cvFemaleNames.size(); ++index) {

		sPerson newPerson;
		newPerson.uniqueID = ID;
		int femaleIndex = rand() % cvFemaleNames.size();
		newPerson.first = cvFemaleNames[femaleIndex];

		int lastIndex = rand() % cvLastNames.size();
		newPerson.last = cvLastNames[lastIndex];

		int age = rand() % 95 + 1;
		newPerson.age = age;

		// Random health from 0 to 100
		float health = (rand() % 100) + (rand() % 100001 / 100000.0f);
		newPerson.health = health;

		// Generate a random position
		// varying from -500 to 500 in the X and Z
		// axes and  0 to 100 in the y
		float x = ((rand() % 1001) + (rand() % 1001 / 1000.0f)) - 500.0f;
		float y = (rand() % 101) + (rand() % 1001 / 1000.0f);
		float z = ((rand() % 1001) + (rand() % 1001 / 1000.0f)) - 500.0f;

		sPoint location;
		location.x = x;
		location.y = y;
		location.z = z;
		newPerson.location = location;

		this->push_back(newPerson);
		this->mUpdatePerformanceData(customVecPerformanceData);

		ID++;
		this->mUpdatePerformanceData(customVecPerformanceData);
	}

	return;
}

void cCustomVector::mGenerateNumPeople(int numPeople) {

	this->mStartPerformanceData(customVecPerformanceData);

	// Now generate the sPerson objects
	size_t ID = 0;
	srand(time(NULL));

	for (size_t peopleCount = 0; peopleCount < numPeople; ++peopleCount) {

		int gender = rand() % 2;
		switch (gender) {
		case 0: // Female
		{
			sPerson newPerson;
			newPerson.uniqueID = ID;
			int femaleIndex = rand() % cvFemaleNames.size();
			newPerson.first = cvFemaleNames[femaleIndex];

			int lastIndex = rand() % cvLastNames.size();
			newPerson.last = cvLastNames[lastIndex];

			int age = rand() % 95 + 1;
			newPerson.age = age;

			// Random health from 0 to 100
			float health = (rand() % 100) + (rand() % 100001 / 100000.0f);
			newPerson.health = health;

			// Generate a random position
			// varying from -500 to 500 in the X and Z
			// axes and  0 to 100 in the y
			float x = ((rand() % 1001) + (rand() % 1001 / 1000.0f)) - 500.0f;
			float y = (rand() % 101) + (rand() % 1001 / 1000.0f);
			float z = ((rand() % 1001) + (rand() % 1001 / 1000.0f)) - 500.0f;

			sPoint location;
			location.x = x;
			location.y = y;
			location.z = z;
			newPerson.location = location;

			this->push_back(newPerson);
			this->mUpdatePerformanceData(customVecPerformanceData);
		}
		break;
		case 1:
		{
			sPerson newPerson;
			newPerson.uniqueID = ID;
			int maleIndex = rand() % cvMaleNames.size();
			newPerson.first = cvMaleNames[maleIndex];

			int lastIndex = rand() % cvLastNames.size();
			newPerson.last = cvLastNames[lastIndex];

			int age = rand() % 95 + 1;
			newPerson.age = age;

			// Random health from 0 to 100
			float health = (rand() % 100) + (rand() % 100001 / 100000.0f);
			newPerson.health = health;

			// Generate a random position
			// varying from -500 to 500 in the X and Z
			// axes and  0 to 100 in the y
			float x = ((rand() % 1001) + (rand() % 1001 / 1000.0f)) - 500.0f;
			float y = (rand() % 101) + (rand() % 1001 / 1000.0f);
			float z = ((rand() % 1001) + (rand() % 1001 / 1000.0f)) - 500.0f;

			sPoint location;
			location.x = x;
			location.y = y;
			location.z = z;
			newPerson.location = location;

			this->push_back(newPerson);
			this->mUpdatePerformanceData(customVecPerformanceData);
		}
		default:
			this->mUpdatePerformanceData(customVecPerformanceData);
			break;
		}

		ID++;
		this->mUpdatePerformanceData(customVecPerformanceData);
	}

	this->mEndPerformanceData(customVecPerformanceData);

	return;
}