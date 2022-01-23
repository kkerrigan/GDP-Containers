/**
 * @file cCustomList.cpp
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 *
 * @section DESCRIPTION
 *
 * Implementation of the cCustomList class.
 */

#include <stdexcept>
#include <time.h>
#include <Windows.h>
#include <psapi.h>
#include <string>
#include <fstream>
#include <iostream>

#include "cCustomList.h"

cCustomList::cCustomList() {

	this->mHead = nullptr;
	this->mFirstNode = nullptr;
	this->mLastNode = nullptr;
	this->mSize = 0;

	return;
}

cCustomList::~cCustomList() {

	if (this->mHead == nullptr) return;

	this->setFirst();

	while (this->mHead->next != nullptr) {

		this->moveBackward();
		delete this->mHead->prev;
	}

	delete this->mHead;

	return;
}

void cCustomList::push_back(sPerson& person) {

	if (!this->mLastNode == NULL) {

		sNode* newNode = new sNode;
		newNode->data = person;
		newNode->prev = this->mLastNode;
		this->mLastNode->next = newNode;
		this->mLastNode = newNode;
		this->mSize++;
	}
	else {

		// This is the first item in the list
		sNode* newNode = new sNode;
		newNode->data = person;
		this->mHead = newNode;
		this->mFirstNode = newNode;
		this->mLastNode = newNode;
		this->mSize++;
	}

	return;
}

void cCustomList::resize(size_t newSize) {



	return;
}

size_t cCustomList::size() {
	
	return this->mSize;
}

sPerson& cCustomList::getObject() {
	
	if (this->mHead == nullptr) {

		throw new std::runtime_error("There was no object on the cCustomList to return\n");
	}

	return this->mHead->data;
}

void cCustomList::insert(sPerson& person) {

	sNode* newNode = new sNode;
	newNode->data = person;

	if (this->mHead == nullptr) {

		this->mHead = newNode;
		this->mFirstNode = newNode;
		this->mLastNode = newNode;
		return;
	}

	if (this->mHead->prev == nullptr) {

		newNode->next = this->mHead;
		this->mHead->prev = newNode;
		this->mFirstNode = newNode;
		this->moveBackward();
		return;
	}

	newNode->prev = this->mHead->prev;
	newNode->next = this->mHead;
	this->mHead->prev->next = newNode;
	this->mHead->prev = newNode;
	this->moveBackward();

	return;
}

void cCustomList::erase() {

	if (this->mHead == nullptr) return;

	if (this->mHead->next == nullptr && this->mHead->prev == nullptr) {
		
		delete this->mHead;
		this->mHead = nullptr;
		this->mFirstNode = nullptr;
		this->mLastNode = nullptr;
		return;
	}

	if (this->mHead->next != nullptr && this->mHead->prev == nullptr) {
		
		this->moveForward();
		delete this->mHead->prev;
		this->mHead->prev = nullptr;
		this->mFirstNode = this->mHead;
		return;
	}

	if (this->mHead->prev != nullptr && this->mHead->next == nullptr) {
		
		this->moveBackward();
		delete this->mHead->next;
		this->mHead->next = nullptr;
		this->mLastNode = this->mHead;
		return;
	}

	this->mHead->prev->next = this->mHead->next;
	this->mHead->next->prev = this->mHead->prev;
	sNode* orphan = this->mHead;
	this->moveBackward();
	delete orphan;

	return;
}

void cCustomList::clear() {

	if (this->mHead == nullptr) return;

	this->setFirst();

	while (this->mHead->next != nullptr) {
		
		this->moveForward();
		delete this->mHead->prev;
	}

	delete this->mHead;
	this->mHead = nullptr;
	
	this->mFirstNode = nullptr;
	this->mLastNode = nullptr;
	this->mSize = 0;

	return;
}

void cCustomList::setFirst() {

	if (this->mFirstNode == nullptr) return;

	this->mHead = this->mFirstNode;

	return;
}

void cCustomList::setLast() {

	if (this->mLastNode == nullptr) return;

	this->mHead = this->mLastNode;

	return;
}

void cCustomList::moveForward() {

	if (this->mHead == nullptr) return;

	if (this->mHead->next == nullptr) return;

	sNode* nextNode = this->mHead->next;
	this->mHead = nextNode;

	return;
}

void cCustomList::moveBackward() {

	if (this->mHead == nullptr) return;

	if (this->mHead->prev == nullptr) return;

	sNode* prevNode = this->mHead->prev;
	this->mHead = prevNode;

	return;
}

bool cCustomList::isAtLastNode() {
	
	return this->mHead == this->mLastNode;
}

sPerfData customListPerformanceData;
void cCustomList::generatePeople(int numPeople) {

	if (numPeople == INT_MAX) {

		this->mGenerateAllPeople();
	}
	else {

		this->mGenerateNumPeople(numPeople);
	}

	return;
}

// ****** Interface functions start ********//
std::vector<std::string>clMaleNames;
std::vector<std::string>clFemaleNames;
std::vector<std::string>clLastNames;

bool cCustomList::LoadDataFilesIntoContainer(std::string firstNameFemaleFileName, std::string firstNameMaleFileName, std::string lastNameFileName) {
	
	// Start recording performance data
	this->mStartPerformanceData(customListPerformanceData);

	// Read the female first names into the array
	std::ifstream file(firstNameFemaleFileName.c_str());
	if (!file.is_open()) {

		std::cout << "Did not open " << firstNameFemaleFileName << std::endl;
		this->mEndPerformanceData(customListPerformanceData);
		return false;
	}

	std::string placeHolder;
	while (!file.eof()) {

		placeHolder = "";
		file >> placeHolder;
		if (placeHolder != "") clFemaleNames.push_back(placeHolder);
		file >> placeHolder;
		file >> placeHolder;
		file >> placeHolder;

		this->mUpdatePerformanceData(customListPerformanceData);
	}

	file.close();

	// Read the male names into the array
	file.open(firstNameMaleFileName.c_str());
	if (!file.is_open()) {

		std::cout << "Did not open " << firstNameMaleFileName << std::endl;
		this->mEndPerformanceData(customListPerformanceData);
		return false;
	}

	while (!file.eof()) {

		placeHolder = "";
		file >> placeHolder;
		if (placeHolder != "") clMaleNames.push_back(placeHolder);
		file >> placeHolder;
		file >> placeHolder;
		file >> placeHolder;

		this->mUpdatePerformanceData(customListPerformanceData);
	}

	file.close();

	// Read the last names into the array
	file.open(lastNameFileName.c_str());
	if (!file.is_open()) {

		std::cout << "Did not open " << lastNameFileName << std::endl;
		this->mEndPerformanceData(customListPerformanceData);
		return false;
	}

	while (!file.eof()) {

		placeHolder = "";
		file >> placeHolder;
		if (placeHolder != "") clLastNames.push_back(placeHolder);
		file >> placeHolder;
		file >> placeHolder;
		file >> placeHolder;

		this->mUpdatePerformanceData(customListPerformanceData);
	}

	file.close();

	// Check that there are first names to use
	if (clFemaleNames.empty() && clMaleNames.empty()) {

		this->mEndPerformanceData(customListPerformanceData);
		return false;
	}

	// Check to make sure we have last names
	if (clLastNames.empty()) {

		this->mEndPerformanceData(customListPerformanceData);
		return false;
	}

	this->mEndPerformanceData(customListPerformanceData);

	return true;
}

bool cCustomList::FindPeopleByName(std::vector<sPerson>& vecPeople, sPerson personToMatch, int maxNumberOfPeople) {
	
	// Start recording performance data
	this->mStartPerformanceData(customListPerformanceData);

	vecPeople.clear();

	if (maxNumberOfPeople < 0) {

		this->mEndPerformanceData(customListPerformanceData);
		return false;
	}

	size_t listSize = this->mSize;
	this->setFirst();

	// if both names are blank, return everyone
	if (personToMatch.first == "" && personToMatch.last == "") {

		for (int index = 0; index < listSize; ++index) {

			this->mUpdatePerformanceData(customListPerformanceData);


			if ((int)vecPeople.size() >= maxNumberOfPeople) {

				this->mEndPerformanceData(customListPerformanceData);
				return true;
			}

			vecPeople.push_back(this->getObject());
			this->moveForward();
		}

		if (!vecPeople.empty()) {

			this->mEndPerformanceData(customListPerformanceData);
			return true;
		}
		else {

			this->mEndPerformanceData(customListPerformanceData);
			return false;
		}
	}

	// if last name is "", search only by first name
	if (personToMatch.last == "") {

		for (int index = 0; index < listSize; ++index) {

			this->mUpdatePerformanceData(customListPerformanceData);
			sPerson currPerson = this->getObject();

			if (personToMatch.first == currPerson.first) {

				if ((int)vecPeople.size() >= maxNumberOfPeople) {

					this->mEndPerformanceData(customListPerformanceData);
					return true;
				}
				vecPeople.push_back(currPerson);
			}
			this->moveForward();
		}

		if (!vecPeople.empty()) {

			this->mEndPerformanceData(customListPerformanceData);
			return true;
		}
		else {

			this->mEndPerformanceData(customListPerformanceData);
			return false;
		}
	}

	// if first name is "", search only by last name
	if (personToMatch.first == "") {

		for (int index = 0; index < listSize; ++index) {

			this->mUpdatePerformanceData(customListPerformanceData);
			sPerson currPerson = this->getObject();
			if (personToMatch.last == currPerson.last) {

				if ((int)vecPeople.size() >= maxNumberOfPeople) {

					this->mUpdatePerformanceData(customListPerformanceData);
					return true;
				}
				vecPeople.push_back(currPerson);
			}
			this->moveForward();
		}

		if (!vecPeople.empty()) {

			this->mEndPerformanceData(customListPerformanceData);
			return true;
		}
		else {

			this->mEndPerformanceData(customListPerformanceData);
			return false;
		}
	}

	// Otherwise compare both first and last name
	for (int index = 0; index < listSize; ++index) {

		this->mUpdatePerformanceData(customListPerformanceData);
		sPerson currPerson = this->getObject();
		if (personToMatch.first == currPerson.first &&
			personToMatch.last == currPerson.last) {

			if ((int)vecPeople.size() >= maxNumberOfPeople) {

				this->mUpdatePerformanceData(customListPerformanceData);
				return true;
			}
			vecPeople.push_back(currPerson);
		}
		this->moveForward();
	}

	if (!vecPeople.empty()) {

		this->mEndPerformanceData(customListPerformanceData);
		return true;
	}
	else {

		this->mEndPerformanceData(customListPerformanceData);
		return false;
	}
}

bool cCustomList::FindPeopleByName(std::vector<sPerson>& vecPeople, std::vector<sPerson>& vecPeopleToMatch, int maxNumberOfPeople) {
	
	// Start recording performance data
	this->mStartPerformanceData(customListPerformanceData);

	vecPeople.clear();

	if (maxNumberOfPeople < 0) {

		this->mEndPerformanceData(customListPerformanceData);
		return false;
	}

	size_t listSize = this->mSize;

	for (int people = 0; people < vecPeopleToMatch.size(); ++people) {

		this->mUpdatePerformanceData(customListPerformanceData);
		this->setFirst();

		sPerson vecCurrPerson = vecPeopleToMatch[people];

		// if both names are blank, return everyone
		if (vecCurrPerson.first == "" && vecCurrPerson.last == "") {

			for (int index = 0; index < listSize; ++index) {

				this->mUpdatePerformanceData(customListPerformanceData);


				if ((int)vecPeople.size() >= maxNumberOfPeople) {

					this->mUpdatePerformanceData(customListPerformanceData);
					break;
				}

				vecPeople.push_back(this->getObject());
			}

			this->mEndPerformanceData(customListPerformanceData);
			return true;
		}

		// if last name is "", search only by first name
		if (vecCurrPerson.last == "") {

			for (int index = 0; index < listSize; ++index) {

				this->mUpdatePerformanceData(customListPerformanceData);
				sPerson currPerson = this->getObject();

				if (vecCurrPerson.first == currPerson.first) {

					if ((int)vecPeople.size() >= maxNumberOfPeople) {

						this->mUpdatePerformanceData(customListPerformanceData);
						break;
					}
					vecPeople.push_back(currPerson);
				}
				this->moveForward();
			}

			this->mUpdatePerformanceData(customListPerformanceData);
			continue;
		}

		// if first name is "", search only by last name
		if (vecCurrPerson.first == "") {

			for (int index = 0; index < listSize; ++index) {

				this->mUpdatePerformanceData(customListPerformanceData);
				sPerson currPerson = this->getObject();
				
				if (vecCurrPerson.last == currPerson.last) {

					if ((int)vecPeople.size() >= maxNumberOfPeople) {

						this->mUpdatePerformanceData(customListPerformanceData);
						break;
					}
					vecPeople.push_back(currPerson);
				}
				this->moveForward();
			}

			this->mUpdatePerformanceData(customListPerformanceData);
			continue;
		}

		// Otherwise compare both first and last name
		for (int index = 0; index < listSize; ++index) {

			this->mUpdatePerformanceData(customListPerformanceData);
			sPerson currPerson = this->getObject();
			
			if (vecCurrPerson.first == currPerson.first &&
				vecCurrPerson.last == currPerson.last) {

				if ((int)vecPeople.size() >= maxNumberOfPeople) {

					this->mUpdatePerformanceData(customListPerformanceData);
					break;
				}
				vecPeople.push_back(currPerson);
			}
			this->moveForward();
		}

		this->mUpdatePerformanceData(customListPerformanceData);
		continue;
	}

	if (!vecPeople.empty()) {

		this->mEndPerformanceData(customListPerformanceData);
		return true;
	}
	else {

		this->mEndPerformanceData(customListPerformanceData);
		return false;
	}
}

bool cCustomList::FindPersonByID(sPerson& person, unsigned long long uniqueID) {
	
	// Start recording performance data
	this->mStartPerformanceData(customListPerformanceData);

	size_t listSize = this->mSize;
	this->setFirst();
	for (size_t index = 0; index < listSize; ++index) {

		this->mUpdatePerformanceData(customListPerformanceData);
		sPerson currPerson = this->getObject();
		if (currPerson.uniqueID == uniqueID) {

			person = currPerson;
			this->mEndPerformanceData(customListPerformanceData);
			return true;
		}
	}
	this->mEndPerformanceData(customListPerformanceData);

	return false;
}

bool cCustomList::FindPeople(std::vector<sPerson>& vecPeople, sPoint location, float radius, int maxPeopleToReturn) {
	
	// Start recording performance data
	this->mStartPerformanceData(customListPerformanceData);
	vecPeople.clear();

	if (maxPeopleToReturn < 0) {

		this->mEndPerformanceData(customListPerformanceData);
		return false;
	}

	size_t listSize = this->mSize;
	this->setFirst();
	float distance;
	for (size_t index = 0; index < listSize; ++index) {

		this->mUpdatePerformanceData(customListPerformanceData);
		sPerson currPerson = this->getObject();
		distance = 0.0f;
		if (this->mIsInsideSphere(location, radius, currPerson, distance)) {

			if ((int)vecPeople.size() >= maxPeopleToReturn) {

				this->mUpdatePerformanceData(customListPerformanceData);
				break;
			}

			vecPeople.push_back(currPerson);
		}
	}

	if (!vecPeople.empty()) {

		this->mEndPerformanceData(customListPerformanceData);
		return true;
	}
	else {

		this->mEndPerformanceData(customListPerformanceData);
		return false;
	}
}

bool cCustomList::FindPeople(std::vector<sPerson>& vecPeople, float minHealth, float maxHealth, int maxPeopleToReturn) {
	
	// Start recording performance data
	this->mStartPerformanceData(customListPerformanceData);
	vecPeople.clear();

	size_t listSize = this->mSize;
	this->setFirst();
	for (size_t index = 0; index < listSize; ++index) {

		this->mUpdatePerformanceData(customListPerformanceData);
		if ((int)vecPeople.size() > maxPeopleToReturn) {

			this->mEndPerformanceData(customListPerformanceData);
			return true;
		}

		sPerson currPerson = this->getObject();
		if (currPerson.health >= minHealth && currPerson.health <= maxHealth) {

			vecPeople.push_back(currPerson);
			this->mUpdatePerformanceData(customListPerformanceData);
		}
	}

	if (!vecPeople.empty()) {

		this->mEndPerformanceData(customListPerformanceData);
		return true;
	}
	else {

		this->mEndPerformanceData(customListPerformanceData);
		return false;
	}
}

bool cCustomList::FindPeople(std::vector<sPerson>& vecPeople, sPoint location, float radius, float minHealth, float maxHealth, int maxPeopleToReturn) {
	
	// Start recording performance data
	this->mStartPerformanceData(customListPerformanceData);
	vecPeople.clear();

	if (maxPeopleToReturn < 0) {

		this->mEndPerformanceData(customListPerformanceData);
		return false;
	}

	size_t listSize = this->mSize;
	this->setFirst();
	float distance;
	for (size_t index = 0; index < listSize; ++index) {

		sPerson currPerson = this->getObject();
		if (currPerson.health >= minHealth && currPerson.health <= maxHealth) {

			if (this->mIsInsideSphere(location, radius, currPerson, distance)) {

				if ((int)vecPeople.size() > maxPeopleToReturn) {

					this->mEndPerformanceData(customListPerformanceData);
					return true;
				}

				vecPeople.push_back(currPerson);
				this->mUpdatePerformanceData(customListPerformanceData);
			}
		}
	}

	if (!vecPeople.empty()) {

		this->mEndPerformanceData(customListPerformanceData);
		return true;
	}
	else {

		this->mEndPerformanceData(customListPerformanceData);
		return false;
	}
}

bool cCustomList::SortPeople(std::vector<sPerson>& vecPeople, eSortType sortType) {
	
	// Start recording performance data
	this->mStartPerformanceData(customListPerformanceData);

	vecPeople.clear();
	if (this->mSize == 0) {

		this->mEndPerformanceData(customListPerformanceData);
		return false;
	}

	switch (sortType) {
	case iPersonMotron::ASC_FIRST_THEN_LAST:
		this->mSortFirstLastNamesAsc();
		this->mUpdatePerformanceData(customListPerformanceData);
		break;
	case iPersonMotron::DESC_FIRST_THEN_LAST:
		this->mSortFirstLastNamesDesc();
		this->mUpdatePerformanceData(customListPerformanceData);
		break;
	case iPersonMotron::ASC_LAST_THEN_FIRST:
		this->mSortLastFirstNamesAsc();
		this->mUpdatePerformanceData(customListPerformanceData);
		break;
	case iPersonMotron::DESC_LAST_THEN_FIRST:
		this->mSortLastFirstNamesDesc();
		this->mUpdatePerformanceData(customListPerformanceData);
		break;
	case iPersonMotron::ASC_BY_ID:
		this->mSortIdAsc();
		this->mUpdatePerformanceData(customListPerformanceData);
		break;
	case iPersonMotron::DESC_BY_ID:
		this->mSortIdDesc();
		this->mUpdatePerformanceData(customListPerformanceData);
		break;
	case iPersonMotron::ASC_BY_HEALTH:
		this->mSortHealthAsc();
		this->mUpdatePerformanceData(customListPerformanceData);
		break;
	case iPersonMotron::DESC_BY_HEALTH:
		this->mSortHealthDesc();
		this->mUpdatePerformanceData(customListPerformanceData);
		break;
	default:
		this->mUpdatePerformanceData(customListPerformanceData);
		return false;
		break;
	}

	this->setFirst();
	for (size_t index = 0; index < this->mSize; ++index) {
		
		vecPeople.push_back(this->getObject());
		this->moveForward();
	}

	this->mEndPerformanceData(customListPerformanceData);
	return true;
}

bool cCustomList::GetPerformanceFromLastCall(sPerfData& callStats) {
	
	callStats = customListPerformanceData;

	return true;
}

eContainerType cCustomList::getContainerType(void) {
	
	this->mStartPerformanceData(customListPerformanceData);

	this->mEndPerformanceData(customListPerformanceData);
	
	return eContainerType::CUSTOM_DIY_LIST;
}
//******* Interface functions end ***************//

//******* Sorting functions start ***************//

void cCustomList::mSortFirstLastNamesAsc() {

	// First pass sort them by first names

	bool IsFinished = false;

	while (!IsFinished)
	{
		// List iterator
		this->setFirst();

		// Flag to exit
		IsFinished = true;
		for (size_t index = 0; index < this->mSize; ++index) {
			
			sPerson p1 = this->getObject();

			// Nothing to do if next hop is the end of the list (empty)
			if (this->isAtLastNode()) break;

			// helper iterator iterator
			this->moveForward();
			sPerson p2 = this->getObject();

			if (p2.first < p1.first) {
				
				this->erase();
				this->insert(p2);
				IsFinished = false;
				this->moveForward();
			}
		}
	}

	// Second pass sort them by last names
	IsFinished = false;

	while (!IsFinished) {
		
		// List iterator
		this->setFirst();

		// Flag to exit
		IsFinished = true;

		for (size_t index = 0; index < this->mSize; ++index) {
			
			sPerson p1 = this->getObject();

			// Nothing to do if next hop is the end of the list (empty)
			if (this->isAtLastNode()) break;

			// helper iterator iterator
			this->moveForward();
			sPerson p2 = this->getObject();

			if (p2.last < p1.last && p2.first == p1.first) {
				
				this->erase();
				this->insert(p2);
				IsFinished = false;
				this->moveForward();
			}
		}
	}

	return;
}

void cCustomList::mSortFirstLastNamesDesc() {

	// First pass sort them by first names
	bool IsFinished = false;

	while (!IsFinished) {
		
		this->setFirst();

		IsFinished = true;
		for (size_t index = 0; index < this->mSize; ++index) {
			
			sPerson p1 = this->getObject();

			// Nothing to do if next hop is the end of the list (empty)
			if (this->isAtLastNode()) break;

			this->moveForward();
			sPerson p2 = this->getObject();

			if (p2.first > p1.first) {

				this->erase();
				this->insert(p2);
				IsFinished = false;
				this->moveForward();
			}
		}
	}

	// Second pass sort them by last names
	IsFinished = false;
	while (!IsFinished) {
		
		this->setFirst();

		IsFinished = true;

		for (size_t index = 0; index < this->mSize; ++index) {
			
			sPerson p1 = this->getObject();

			// Nothing to do if next hop is the end of the list (empty)
			if (this->isAtLastNode()) break;

			this->moveForward();
			sPerson p2 = this->getObject();

			if (p2.last > p1.last && p2.first == p1.first) {
				
				this->erase();
				this->insert(p2);
				IsFinished = false;
				this->moveForward();
			}
		}
	}

	return;
}

void cCustomList::mSortLastFirstNamesAsc() {

	// First pass sort them by last names
	bool IsFinished = false;

	while (!IsFinished) {
		
		this->setFirst();

		IsFinished = true;
		for (size_t index = 0; index < this->mSize; ++index) {
			
			sPerson p1 = this->getObject();

			// Nothing to do if next hop is the end of the list (empty)
			if (this->isAtLastNode()) break;

			this->moveForward();
			sPerson p2 = this->getObject();

			if (p2.last < p1.last) {
				
				this->erase();
				this->insert(p2);
				IsFinished = false;
				this->moveForward();
			}
		}
	}

	// Second pass sort them by last names
	IsFinished = false;
	while (!IsFinished) {
		
		this->setFirst();

		IsFinished = true;

		for (size_t index = 0; index < this->mSize; ++index) {
			
			sPerson p1 = this->getObject();

			// Nothing to do if next hop is the end of the list (empty)
			if (this->isAtLastNode()) break;

			this->moveForward();
			sPerson p2 = this->getObject();

			if (p2.first < p1.first && p2.first == p1.first) {
				
				this->erase();
				this->insert(p2);
				IsFinished = false;
				this->moveForward();
			}
		}
	}

	return;
}

void cCustomList::mSortLastFirstNamesDesc() {

	// First pass sort them by last names
	bool IsFinished = false;
	while (!IsFinished) {
		
		this->setFirst();

		IsFinished = true;
		for (size_t index = 0; index < this->size(); ++index) {
			
			sPerson p1 = this->getObject();

			// Nothing to do if next hop is the end of the list (empty)
			if (this->isAtLastNode()) break;

			this->moveForward();
			sPerson p2 = this->getObject();

			if (p2.last > p1.last) {
				
				this->erase();
				this->insert(p2);
				IsFinished = false;
				this->moveForward();
			}
		}
	}

	// Second pass sort them by last names
	IsFinished = false;
	while (!IsFinished) {
		
		this->setFirst();

		IsFinished = true;
		for (size_t index = 0; index < this->mSize; ++index) {
			
			sPerson p1 = this->getObject();

			// Nothing to do if next hop is the end of the list (empty)
			if (this->isAtLastNode()) break;

			this->moveForward();
			sPerson p2 = this->getObject();

			if (p2.first > p1.first && p2.first == p1.first) {
				
				this->erase();
				this->insert(p2);
				IsFinished = false;
				this->moveForward();
			}
		}
	}

	return;
}

void cCustomList::mSortIdAsc() {

	bool IsFinished = false;
	while (!IsFinished) {
		
		this->setFirst();

		IsFinished = true;
		for (size_t index = 0; index < this->mSize; ++index) {
			
			sPerson p1 = this->getObject();

			// Nothing to do if next hop is the end of the list (empty)
			if (this->isAtLastNode()) break;

			this->moveForward();
			sPerson p2 = this->getObject();

			if (p2.uniqueID < p1.uniqueID) {
				
				this->erase();
				this->insert(p2);
				IsFinished = false;
			}
		}
	}

	return;
}

void cCustomList::mSortIdDesc() {

	bool IsFinished = false;
	while (!IsFinished) {

		this->setFirst();

		IsFinished = true;
		for (size_t index = 0; index < this->mSize; ++index) {

			sPerson p1 = this->getObject();

			// Nothing to do if next hop is the end of the list (empty)
			if (this->isAtLastNode()) break;

			this->moveForward();
			sPerson p2 = this->getObject();

			if (p2.uniqueID > p1.uniqueID) {

				this->erase();
				this->insert(p2);
				IsFinished = false;
			}
		}
	}

	return;
}

void cCustomList::mSortHealthAsc() {

	bool IsFinished = false;
	while (!IsFinished) {

		this->setFirst();

		IsFinished = true;
		for (size_t index = 0; index < this->mSize; ++index) {

			sPerson p1 = this->getObject();

			// Nothing to do if next hop is the end of the list (empty)
			if (this->isAtLastNode()) break;

			this->moveForward();
			sPerson p2 = this->getObject();

			if (p2.health < p1.health) {

				this->erase();
				this->insert(p2);
				IsFinished = false;
			}
		}
	}

	return;
}

void cCustomList::mSortHealthDesc() {

	bool IsFinished = false;
	while (!IsFinished) {

		this->setFirst();

		IsFinished = true;
		for (size_t index = 0; index < this->mSize; ++index) {

			sPerson p1 = this->getObject();

			// Nothing to do if next hop is the end of the list (empty)
			if (this->isAtLastNode()) break;

			this->moveForward();
			sPerson p2 = this->getObject();

			if (p2.health > p1.health) {

				this->erase();
				this->insert(p2);
				IsFinished = false;
			}
		}
	}

	return;
}
//******* Sorting functions end ***************//

//******* Performance functions start ***************//
clock_t customListStartTime;
void cCustomList::mStartPerformanceData(sPerfData& pd) {

	// Start the clock
	customListStartTime = clock();

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

void cCustomList::mUpdatePerformanceData(sPerfData& pd) {

	// Update elapsed time
	pd.elapsedCallTime_ms = ((float)(clock() - customListStartTime) / CLOCKS_PER_SEC) * 1000.0f;

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

void cCustomList::mEndPerformanceData(sPerfData& pd){

	// Update elapsed time
	pd.elapsedCallTime_ms = ((float)(clock() - customListStartTime) / CLOCKS_PER_SEC) * 1000.0f;

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
//******* Performance functions end ***************//


bool cCustomList::mIsInsideSphere(sPoint point, float radius, sPerson person, float& distance) {
	
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

void cCustomList::mGenerateAllPeople() {

	this->mStartPerformanceData(customListPerformanceData);

	// Now generate the sPerson objects
	size_t ID = 0;
	srand(time(NULL));

	// Generate all the males with a random last name
	for (size_t index = 0; index < clMaleNames.size(); ++index) {

		sPerson newPerson;
		newPerson.uniqueID = ID;
		int maleIndex = rand() % clMaleNames.size();
		newPerson.first = clMaleNames[maleIndex];

		int lastIndex = rand() % clLastNames.size();
		newPerson.last = clLastNames[lastIndex];

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
		this->mUpdatePerformanceData(customListPerformanceData);

		ID++;
		this->mUpdatePerformanceData(customListPerformanceData);
	}

	// Now create all the female names
	for (size_t index = 0; index < clFemaleNames.size(); ++index) {

		sPerson newPerson;
		newPerson.uniqueID = ID;
		int femaleIndex = rand() % clFemaleNames.size();
		newPerson.first = clFemaleNames[femaleIndex];

		int lastIndex = rand() % clLastNames.size();
		newPerson.last = clLastNames[lastIndex];

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
		this->mUpdatePerformanceData(customListPerformanceData);

		ID++;
		this->mUpdatePerformanceData(customListPerformanceData);
	}

	return;
}

void cCustomList::mGenerateNumPeople(int numPeople) {

	this->mStartPerformanceData(customListPerformanceData);

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
			int femaleIndex = rand() % clFemaleNames.size();
			newPerson.first = clFemaleNames[femaleIndex];

			int lastIndex = rand() % clLastNames.size();
			newPerson.last = clLastNames[lastIndex];

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
			this->mUpdatePerformanceData(customListPerformanceData);
		}
		break;
		case 1:
		{
			sPerson newPerson;
			newPerson.uniqueID = ID;
			int maleIndex = rand() % clMaleNames.size();
			newPerson.first = clMaleNames[maleIndex];

			int lastIndex = rand() % clLastNames.size();
			newPerson.last = clLastNames[lastIndex];

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
			this->mUpdatePerformanceData(customListPerformanceData);
		}
		default:
			this->mUpdatePerformanceData(customListPerformanceData);
			break;
		}

		ID++;
		this->mUpdatePerformanceData(customListPerformanceData);
	}

	this->mEndPerformanceData(customListPerformanceData);

	return;
}