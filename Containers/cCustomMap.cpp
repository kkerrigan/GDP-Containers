/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#include <stdexcept>
#include <time.h>
#include <Windows.h>
#include <psapi.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "cCustomMap.h"

sPerfData customMapPerformanceData;
void cCustomMap::generatePeople(int numPeople) {

	if (numPeople == INT_MAX) {

		this->mGenerateAllPeople();
	}
	else {

		this->mGenerateNumPeople(numPeople);
	}

	return;
}

// *************** Interface Functions start ****************//
std::vector<std::string>cmMaleNames;
std::vector<std::string>cmFemaleNames;
std::vector<std::string>cmLastNames;

bool cCustomMap::LoadDataFilesIntoContainer(std::string firstNameFemaleFileName, std::string firstNameMaleFileName, std::string lastNameFileName) {

	// Start recording performance data
	this->mStartPerformanceData(customMapPerformanceData);

	// Read the female first names into the array
	std::ifstream file(firstNameFemaleFileName.c_str());
	if (!file.is_open()) {

		std::cout << "Did not open " << firstNameFemaleFileName << std::endl;
		this->mEndPerformanceData(customMapPerformanceData);
		return false;
	}

	std::string placeHolder;
	while (!file.eof()) {

		placeHolder = "";
		file >> placeHolder;
		if (placeHolder != "") cmFemaleNames.push_back(placeHolder);
		file >> placeHolder;
		file >> placeHolder;
		file >> placeHolder;

		this->mUpdatePerformanceData(customMapPerformanceData);
	}

	file.close();

	// Read the male names into the array
	file.open(firstNameMaleFileName.c_str());
	if (!file.is_open()) {

		std::cout << "Did not open " << firstNameMaleFileName << std::endl;
		this->mEndPerformanceData(customMapPerformanceData);
		return false;
	}

	while (!file.eof()) {

		placeHolder = "";
		file >> placeHolder;
		if (placeHolder != "") cmMaleNames.push_back(placeHolder);
		file >> placeHolder;
		file >> placeHolder;
		file >> placeHolder;

		this->mUpdatePerformanceData(customMapPerformanceData);
	}

	file.close();

	// Read the last names into the array
	file.open(lastNameFileName.c_str());
	if (!file.is_open()) {

		std::cout << "Did not open " << lastNameFileName << std::endl;
		this->mEndPerformanceData(customMapPerformanceData);
		return false;
	}

	while (!file.eof()) {

		placeHolder = "";
		file >> placeHolder;
		if (placeHolder != "") cmLastNames.push_back(placeHolder);
		file >> placeHolder;
		file >> placeHolder;
		file >> placeHolder;

		this->mUpdatePerformanceData(customMapPerformanceData);
	}

	file.close();

	// Check that there are first names to use
	if (cmFemaleNames.empty() && cmMaleNames.empty()) {

		this->mEndPerformanceData(customMapPerformanceData);
		return false;
	}

	// Check to make sure we have last names
	if (cmLastNames.empty()) {

		this->mEndPerformanceData(customMapPerformanceData);
		return false;
	}

	this->mEndPerformanceData(customMapPerformanceData);

	return true;
}

bool cCustomMap::FindPeopleByName(std::vector<sPerson>& vecPeople, sPerson personToMatch, int maxNumberOfPeople) {

	// Start recording performance data
	this->mStartPerformanceData(customMapPerformanceData);

	vecPeople.clear();

	if (maxNumberOfPeople < 0) {

		this->mEndPerformanceData(customMapPerformanceData);
		return false;
	}

	size_t mapSize = this->mData.size();

	// if both names are blank, return everyone
	if (personToMatch.first == "" && personToMatch.last == "") {

		for (size_t index = 0; index < mapSize; ++index) {

			this->mUpdatePerformanceData(customMapPerformanceData);


			if ((int)vecPeople.size() >= maxNumberOfPeople) {

				this->mEndPerformanceData(customMapPerformanceData);
				return true;
			}

			sPerson currPerson;
			this->mData.find(index, currPerson);

			vecPeople.push_back(currPerson);
		}

		if (!vecPeople.empty()) {

			this->mEndPerformanceData(customMapPerformanceData);
			return true;
		}
		else {

			this->mEndPerformanceData(customMapPerformanceData);
			return false;
		}
	}

	// if last name is "", search only by first name
	if (personToMatch.last == "") {

		for (size_t index = 0; index < mapSize; ++index) {

			this->mUpdatePerformanceData(customMapPerformanceData);
			sPerson currPerson;
			this->mData.find(index, currPerson);

			if (personToMatch.first == currPerson.first) {

				if ((int)vecPeople.size() >= maxNumberOfPeople) {

					this->mEndPerformanceData(customMapPerformanceData);
					return true;
				}
				vecPeople.push_back(currPerson);
			}
		}

		if (!vecPeople.empty()) {

			this->mEndPerformanceData(customMapPerformanceData);
			return true;
		}
		else {

			this->mEndPerformanceData(customMapPerformanceData);
			return false;
		}
	}

	// if first name is "", search only by last name
	if (personToMatch.first == "") {

		for (size_t index = 0; index < mapSize; ++index) {

			this->mUpdatePerformanceData(customMapPerformanceData);
			sPerson currPerson;
			this->mData.find(index, currPerson);

			if (personToMatch.last == currPerson.last) {

				if ((int)vecPeople.size() >= maxNumberOfPeople) {

					this->mUpdatePerformanceData(customMapPerformanceData);
					return true;
				}
				vecPeople.push_back(currPerson);
			}
		}

		if (!vecPeople.empty()) {

			this->mEndPerformanceData(customMapPerformanceData);
			return true;
		}
		else {

			this->mEndPerformanceData(customMapPerformanceData);
			return false;
		}
	}

	// Otherwise compare both first and last name
	for (size_t index = 0; index < mapSize; ++index) {

		this->mUpdatePerformanceData(customMapPerformanceData);
		sPerson currPerson;
		this->mData.find(index, currPerson);

		if (personToMatch.first == currPerson.first &&
			personToMatch.last == currPerson.last) {

			if ((int)vecPeople.size() >= maxNumberOfPeople) {

				this->mUpdatePerformanceData(customMapPerformanceData);
				return true;
			}
			vecPeople.push_back(currPerson);
		}
	}

	if (!vecPeople.empty()) {

		this->mEndPerformanceData(customMapPerformanceData);
		return true;
	}
	else {

		this->mEndPerformanceData(customMapPerformanceData);
		return false;
	}
}

bool cCustomMap::FindPeopleByName(std::vector<sPerson>& vecPeople, std::vector<sPerson>& vecPeopleToMatch, int maxNumberOfPeople) {

	// Start recording performance data
	this->mStartPerformanceData(customMapPerformanceData);

	vecPeople.clear();

	if (maxNumberOfPeople < 0) {

		this->mEndPerformanceData(customMapPerformanceData);
		return false;
	}

	size_t mapSize = this->mData.size();

	for (size_t people = 0; people < vecPeopleToMatch.size(); ++people) {

		this->mUpdatePerformanceData(customMapPerformanceData);

		sPerson vecCurrPerson = vecPeopleToMatch[people];

		// if both names are blank, return everyone
		if (vecCurrPerson.first == "" && vecCurrPerson.last == "") {

			for (size_t index = 0; index < mapSize; ++index) {

				this->mUpdatePerformanceData(customMapPerformanceData);


				if ((int)vecPeople.size() >= maxNumberOfPeople) {

					this->mUpdatePerformanceData(customMapPerformanceData);
					break;
				}

				sPerson currPerson;
				this->mData.find(index, currPerson);
				vecPeople.push_back(currPerson);
			}

			this->mEndPerformanceData(customMapPerformanceData);
			return true;
		}

		// if last name is "", search only by first name
		if (vecCurrPerson.last == "") {

			for (size_t index = 0; index < mapSize; ++index) {

				this->mUpdatePerformanceData(customMapPerformanceData);
				sPerson currPerson;
				this->mData.find(index, currPerson);

				if (vecCurrPerson.first == currPerson.first) {

					if ((int)vecPeople.size() >= maxNumberOfPeople) {

						this->mUpdatePerformanceData(customMapPerformanceData);
						break;
					}
					this->mData.find(index, currPerson);
					vecPeople.push_back(currPerson);
				}
			}

			this->mUpdatePerformanceData(customMapPerformanceData);
			continue;
		}

		// if first name is "", search only by last name
		if (vecCurrPerson.first == "") {

			for (size_t index = 0; index < mapSize; ++index) {

				this->mUpdatePerformanceData(customMapPerformanceData);
				sPerson currPerson;
				this->mData.find(index, currPerson);

				if (vecCurrPerson.last == currPerson.last) {

					if ((int)vecPeople.size() >= maxNumberOfPeople) {

						this->mUpdatePerformanceData(customMapPerformanceData);
						break;
					}
					vecPeople.push_back(currPerson);
				}
			}

			this->mUpdatePerformanceData(customMapPerformanceData);
			continue;
		}

		// Otherwise compare both first and last name
		for (size_t index = 0; index < mapSize; ++index) {

			this->mUpdatePerformanceData(customMapPerformanceData);
			sPerson currPerson;
			this->mData.find(index, currPerson);

			if (vecCurrPerson.first == currPerson.first &&
				vecCurrPerson.last == currPerson.last) {

				if ((int)vecPeople.size() >= maxNumberOfPeople) {

					this->mUpdatePerformanceData(customMapPerformanceData);
					break;
				}
				vecPeople.push_back(currPerson);
			}
		}

		this->mUpdatePerformanceData(customMapPerformanceData);
		continue;
	}

	if (!vecPeople.empty()) {

		this->mEndPerformanceData(customMapPerformanceData);
		return true;
	}
	else {

		this->mEndPerformanceData(customMapPerformanceData);
		return false;
	}
}

bool cCustomMap::FindPersonByID(sPerson& person, unsigned long long uniqueID) {

	// Start recording performance data
	this->mStartPerformanceData(customMapPerformanceData);

	size_t mapSize = this->mData.size();
	for (size_t index = 0; index < mapSize; ++index) {

		this->mUpdatePerformanceData(customMapPerformanceData);
		sPerson currPerson;
		this->mData.find(index, currPerson);

		if (currPerson.uniqueID == uniqueID) {

			person = currPerson;
			this->mEndPerformanceData(customMapPerformanceData);
			return true;
		}
	}
	this->mEndPerformanceData(customMapPerformanceData);

	return false;
}

bool cCustomMap::FindPeople(std::vector<sPerson>& vecPeople, sPoint location, float radius, int maxPeopleToReturn) {

	// Start recording performance data
	this->mStartPerformanceData(customMapPerformanceData);
	vecPeople.clear();

	if (maxPeopleToReturn < 0) {

		this->mEndPerformanceData(customMapPerformanceData);
		return false;
	}

	size_t mapSize = this->mData.size();
	float distance;
	for (size_t index = 0; index < mapSize; ++index) {

		this->mUpdatePerformanceData(customMapPerformanceData);
		sPerson currPerson;
		this->mData.find(index, currPerson);
		distance = 0.0f;

		if (this->mIsInsideSphere(location, radius, currPerson, distance)) {

			if ((int)vecPeople.size() >= maxPeopleToReturn) {

				this->mUpdatePerformanceData(customMapPerformanceData);
				break;
			}

			vecPeople.push_back(currPerson);
		}
	}

	if (!vecPeople.empty()) {

		this->mEndPerformanceData(customMapPerformanceData);
		return true;
	}
	else {

		this->mEndPerformanceData(customMapPerformanceData);
		return false;
	}
}

bool cCustomMap::FindPeople(std::vector<sPerson>& vecPeople, float minHealth, float maxHealth, int maxPeopleToReturn) {

	// Start recording performance data
	this->mStartPerformanceData(customMapPerformanceData);
	vecPeople.clear();

	size_t mapSize = this->mData.size();
	for (size_t index = 0; index < mapSize; ++index) {

		this->mUpdatePerformanceData(customMapPerformanceData);
		if ((int)vecPeople.size() > maxPeopleToReturn) {

			this->mEndPerformanceData(customMapPerformanceData);
			return true;
		}

		sPerson currPerson;
		this->mData.find(index, currPerson);
		if (currPerson.health >= minHealth && currPerson.health <= maxHealth) {

			vecPeople.push_back(currPerson);
			this->mUpdatePerformanceData(customMapPerformanceData);
		}
	}

	if (!vecPeople.empty()) {

		this->mEndPerformanceData(customMapPerformanceData);
		return true;
	}
	else {

		this->mEndPerformanceData(customMapPerformanceData);
		return false;
	}
}

bool cCustomMap::FindPeople(std::vector<sPerson>& vecPeople, sPoint location, float radius, float minHealth, float maxHealth, int maxPeopleToReturn) {

	// Start recording performance data
	this->mStartPerformanceData(customMapPerformanceData);
	vecPeople.clear();

	if (maxPeopleToReturn < 0) {

		this->mEndPerformanceData(customMapPerformanceData);
		return false;
	}

	size_t mapSize = this->mData.size();
	float distance;
	for (size_t index = 0; index < mapSize; ++index) {

		sPerson currPerson;
		this->mData.find(index, currPerson);

		if (currPerson.health >= minHealth && currPerson.health <= maxHealth) {

			if (this->mIsInsideSphere(location, radius, currPerson, distance)) {

				if ((int)vecPeople.size() > maxPeopleToReturn) {

					this->mEndPerformanceData(customMapPerformanceData);
					return true;
				}

				vecPeople.push_back(currPerson);
				this->mUpdatePerformanceData(customMapPerformanceData);
			}
		}
	}

	if (!vecPeople.empty()) {

		this->mEndPerformanceData(customMapPerformanceData);
		return true;
	}
	else {

		this->mEndPerformanceData(customMapPerformanceData);
		return false;
	}
}

bool cCustomMap::SortPeople(std::vector<sPerson>& vecPeople, eSortType sortType) {

	// Start recording performance data
	this->mStartPerformanceData(customMapPerformanceData);

	if (this->mData.size() == 0) {

		this->mEndPerformanceData(customMapPerformanceData);
		return false;
	}

	switch (sortType) {
	case iPersonMotron::ASC_FIRST_THEN_LAST:
		this->mSortFirstLastNamesAsc();
		this->mUpdatePerformanceData(customMapPerformanceData);
		break;
	case iPersonMotron::DESC_FIRST_THEN_LAST:
		this->mSortFirstLastNamesDesc();
		this->mUpdatePerformanceData(customMapPerformanceData);
		break;
	case iPersonMotron::ASC_LAST_THEN_FIRST:
		this->mSortLastFirstNamesAsc();
		this->mUpdatePerformanceData(customMapPerformanceData);
		break;
	case iPersonMotron::DESC_LAST_THEN_FIRST:
		this->mSortLastFirstNamesDesc();
		this->mUpdatePerformanceData(customMapPerformanceData);
		break;
	case iPersonMotron::ASC_BY_ID:
		this->mSortIdAsc();
		this->mUpdatePerformanceData(customMapPerformanceData);
		break;
	case iPersonMotron::DESC_BY_ID:
		this->mSortIdDesc();
		this->mUpdatePerformanceData(customMapPerformanceData);
		break;
	case iPersonMotron::ASC_BY_HEALTH:
		this->mSortHealthAsc();
		this->mUpdatePerformanceData(customMapPerformanceData);
		break;
	case iPersonMotron::DESC_BY_HEALTH:
		this->mSortHealthDesc();
		this->mUpdatePerformanceData(customMapPerformanceData);
		break;
	default:
		this->mUpdatePerformanceData(customMapPerformanceData);
		return false;
		break;
	}

	sPerson currPerson;
	for (size_t index = 0; index < this->mData.size(); ++index) {

		this->mData.find(index, currPerson);
		vecPeople.push_back(currPerson);
	}

	this->mEndPerformanceData(customMapPerformanceData);
	return true;
}

bool cCustomMap::GetPerformanceFromLastCall(sPerfData& callStats) {

	callStats = customMapPerformanceData;
	return true;
}

eContainerType cCustomMap::getContainerType(void) {

	this->mStartPerformanceData(customMapPerformanceData);

	this->mEndPerformanceData(customMapPerformanceData);

	return eContainerType::CUSTOM_DIY_MAP;
}
// *************** Interface Functions end ****************//

// *************** Sort Functions start ****************//

void cCustomMap::mSortFirstLastNamesAsc() {

	// Make a mirror map, ordering by first, last, the unique ID as a string
	cDIYMap<std::string, sPerson> mirrorMap;
	for (size_t index = 0; index < this->mData.size(); ++index) {

		sPerson currPerson;
		this->mData.find(index, currPerson);

		std::string id = currPerson.first + " " + currPerson.last + " " + std::to_string(currPerson.uniqueID);
		mirrorMap.insert(id, currPerson);
	}

	// clear the original map
	this->mData.clear();

	// Put the mirror map into the original map
	for (size_t i = 0; i < mirrorMap.size(); ++i) {

		sPerson currPerson;
		mirrorMap.getObjectAt(currPerson, i);

		this->mData.insert(i, currPerson);
	}

	return;
}

void cCustomMap::mSortFirstLastNamesDesc() {

	// Make a mirror map, ordering by first, last, the unique ID as a string
	cDIYMap<std::string, sPerson> mirrorMap;
	for (size_t index = 0; index < this->mData.size(); ++index) {

		sPerson currPerson;
		this->mData.find(index, currPerson);

		std::string id = currPerson.first + " " + currPerson.last + " " + std::to_string(currPerson.uniqueID);
		mirrorMap.insert(id, currPerson);
	}

	// clear the original map
	this->mData.clear();

	// Put the mirror map into the original map
	size_t index = mirrorMap.size() - 1;
	for (size_t i = 0; i < mirrorMap.size(); ++i) {

		sPerson currPerson;
		mirrorMap.getObjectAt(currPerson, i);
		this->mData.insert(index, currPerson);
		index--;
	}

	return;
}

void cCustomMap::mSortLastFirstNamesAsc() {

	// Make a mirror map, ordering by last, first, the unique ID as a string
	cDIYMap<std::string, sPerson> mirrorMap;
	for (size_t index = 0; index < this->mData.size(); ++index) {

		sPerson currPerson;
		this->mData.find(index, currPerson);

		std::string id = currPerson.last + " " + currPerson.first + " " + std::to_string(currPerson.uniqueID);
		mirrorMap.insert(id, currPerson);
	}

	// clear the original map
	this->mData.clear();

	// Put the mirror map into the original map
	for (size_t i = 0; i < mirrorMap.size(); ++i) {

		sPerson currPerson;
		mirrorMap.getObjectAt(currPerson, i);

		this->mData.insert(i, currPerson);
	}

	return;
}

void cCustomMap::mSortLastFirstNamesDesc() {

	// Make a mirror map, ordering by last, first, the unique ID as a string
	cDIYMap<std::string, sPerson> mirrorMap;
	for (size_t index = 0; index < this->mData.size(); ++index) {

		sPerson currPerson;
		this->mData.find(index, currPerson);

		std::string id = currPerson.last + " " + currPerson.first + " " + std::to_string(currPerson.uniqueID);
		mirrorMap.insert(id, currPerson);
	}

	// clear the original map
	this->mData.clear();

	// Put the mirror map into the original map
	size_t index = mirrorMap.size() - 1;
	for (size_t i = 0; i < mirrorMap.size(); ++i) {

		sPerson currPerson;
		mirrorMap.getObjectAt(currPerson, i);

		this->mData.insert(i, currPerson);
		index--;
	}

	return;
}

void cCustomMap::mSortIdAsc() {

	// Make a mirror map, ordering by unique ID 
	cDIYMap<unsigned long long, sPerson> mirrorMap;
	for (size_t index = 0; index < this->mData.size(); ++index) {

		sPerson currPerson;
		this->mData.find(index, currPerson);

		mirrorMap.insert(currPerson.uniqueID, currPerson);
	}

	// clear the original map
	this->mData.clear();

	// Put the mirror map into the original map
	for (size_t i = 0; i < mirrorMap.size(); ++i) {

		sPerson currPerson;
		mirrorMap.getObjectAt(currPerson, i);

		this->mData.insert(i, currPerson);
	}

	return;
}

void cCustomMap::mSortIdDesc() {

	// Make a mirror map, ordering by unique ID 
	cDIYMap<unsigned long long, sPerson> mirrorMap;
	for (size_t index = 0; index < this->mData.size(); ++index) {

		sPerson currPerson;
		this->mData.find(index, currPerson);

		mirrorMap.insert(currPerson.uniqueID, currPerson);
	}

	// clear the original map
	this->mData.clear();

	// Put the mirror map into the original map
	size_t index = mirrorMap.size() - 1;
	for (size_t i = 0; i < mirrorMap.size(); ++i) {

		sPerson currPerson;
		mirrorMap.getObjectAt(currPerson, i);

		this->mData.insert(i, currPerson);
		index--;
	}

	return;
}

void cCustomMap::mSortHealthAsc() {

	// Make a mirror map, ordering by health, first, last names 
	// and UniqueID (just in case)
	cDIYMap<std::string, sPerson> mirrorMap;
	for (size_t index = 0; index < this->mData.size(); ++index) {

		std::stringstream ss;

		sPerson currPerson;
		this->mData.find(index, currPerson);

		// First the health float number
		// Integral part:
		int integral = (int)currPerson.health;
		ss.width(3);
		ss.fill('0');
		ss << integral;
		// Decimal part:
		int decimal = (int)((currPerson.health - integral) * 1000000);
		ss << " ";
		ss.width(6);
		ss.fill('0');
		ss << decimal;

		// Other fields just in case 
		// (prevent deletion in case equal health values)
		ss << " " << currPerson.first
			<< " " << currPerson.last
			<< " " << currPerson.uniqueID;

		std::string id = ss.str();
		mirrorMap.insert(id, currPerson);
	}

	// Clear the original map
	this->mData.clear();

	// Put the mirror map back into the original
	for (size_t i = 0; i < mirrorMap.size(); ++i) {

		sPerson currPerson;
		mirrorMap.getObjectAt(currPerson, i);

		this->mData.insert(i, currPerson);
	}

	return;
}

void cCustomMap::mSortHealthDesc() {

	// Make a mirror map, ordering by health, first, last names 
	// and UniqueID (just in case)
	cDIYMap<std::string, sPerson> mirrorMap;
	for (size_t index = 0; index < this->mData.size(); ++index) {

		sPerson currPerson;
		this->mData.find(index, currPerson);

		std::stringstream ss;
		// First the health float number
		// Integral part:
		int integral = (int)currPerson.health;
		ss.width(3);
		ss.fill('0');
		ss << integral;
		// Decimal part:
		int decimal = (int)((currPerson.health - integral) * 1000000);
		ss << " ";
		ss.width(6);
		ss.fill('0');
		ss << decimal;

		// Other fields just in case 
		// (prevent deletion in case equal health values)
		ss << " " << currPerson.first
			<< " " << currPerson.last
			<< " " << currPerson.uniqueID;

		std::string id = ss.str();
		mirrorMap.insert(id, currPerson);
	}

	// Clear the original map
	this->mData.clear();

	// Put the mirror map back into the original
	size_t index = mirrorMap.size() - 1;
	for (size_t i = 0; i < mirrorMap.size(); ++i) {

		sPerson currPerson;
		mirrorMap.getObjectAt(currPerson, i);

		this->mData.insert(i, currPerson);
		index--;
	}

	return;
}
// *************** Sort Functions end ****************//

// *************** Performance Functions start ****************//
clock_t customMapStartTime;
void cCustomMap::mStartPerformanceData(sPerfData& pd) {

	// Start the clock
	customMapStartTime = clock();

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

void cCustomMap::mUpdatePerformanceData(sPerfData& pd) {

	// Update elapsed time
	pd.elapsedCallTime_ms = ((float)(clock() - customMapStartTime) / CLOCKS_PER_SEC) * 1000.0f;

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

void cCustomMap::mEndPerformanceData(sPerfData& pd) {

	// Update elapsed time
	pd.elapsedCallTime_ms = ((float)(clock() - customMapStartTime) / CLOCKS_PER_SEC) * 1000.0f;

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
// *************** Performance Functions end ****************//

bool cCustomMap::mIsInsideSphere(sPoint point, float radius, sPerson person, float& distance) {

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

void cCustomMap::mGenerateAllPeople() {

	this->mStartPerformanceData(customMapPerformanceData);

	// Now generate the sPerson objects
	size_t ID = 0;
	size_t mapIndex = 0;
	srand(time(NULL));

	// Generate all the males with a random last name
	for (size_t index = 0; index < cmMaleNames.size(); ++index) {

		sPerson newPerson;
		newPerson.uniqueID = ID;
		int maleIndex = rand() % cmMaleNames.size();
		newPerson.first = cmMaleNames[maleIndex];

		int lastIndex = rand() % cmLastNames.size();
		newPerson.last = cmLastNames[lastIndex];

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

		this->mData.insert(mapIndex, newPerson);
		this->mUpdatePerformanceData(customMapPerformanceData);

		ID++;
		mapIndex++;
		this->mUpdatePerformanceData(customMapPerformanceData);
	}

	// Now create all the female names
	for (size_t index = 0; index < cmFemaleNames.size(); ++index) {

		sPerson newPerson;
		newPerson.uniqueID = ID;
		int femaleIndex = rand() % cmFemaleNames.size();
		newPerson.first = cmFemaleNames[femaleIndex];

		int lastIndex = rand() % cmLastNames.size();
		newPerson.last = cmLastNames[lastIndex];

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

		this->mData.insert(mapIndex, newPerson);
		this->mUpdatePerformanceData(customMapPerformanceData);

		ID++;
		mapIndex++;
		this->mUpdatePerformanceData(customMapPerformanceData);
	}
	
	return;
}

void cCustomMap::mGenerateNumPeople(int numPeople) {

	this->mStartPerformanceData(customMapPerformanceData);

	// Now generate the sPerson objects
	size_t ID = 0;
	size_t mapIndex = 0;
	srand(time(NULL));

	for (size_t peopleCount = 0; peopleCount < numPeople; ++peopleCount) {

		int gender = rand() % 2;
		switch (gender) {
		case 0: // Female
		{
			sPerson newPerson;
			newPerson.uniqueID = ID;
			int femaleIndex = rand() % cmFemaleNames.size();
			newPerson.first = cmFemaleNames[femaleIndex];

			int lastIndex = rand() % cmLastNames.size();
			newPerson.last = cmLastNames[lastIndex];

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

			this->mData.insert(mapIndex, newPerson);
			this->mUpdatePerformanceData(customMapPerformanceData);
		}
		break;
		case 1:
		{
			sPerson newPerson;
			newPerson.uniqueID = ID;
			int maleIndex = rand() % cmMaleNames.size();
			newPerson.first = cmMaleNames[maleIndex];

			int lastIndex = rand() % cmLastNames.size();
			newPerson.last = cmLastNames[lastIndex];

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

			this->mData.insert(mapIndex, newPerson);
			this->mUpdatePerformanceData(customMapPerformanceData);
		}
		default:
			this->mUpdatePerformanceData(customMapPerformanceData);
			break;
		}

		ID++;
		mapIndex++;
		this->mUpdatePerformanceData(customMapPerformanceData);
	}

	this->mEndPerformanceData(customMapPerformanceData);

	return;
}
