/**
 * @file cStandardMap.cpp
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 *
 * @section DESCRIPTION
 *
 * Implementation of the cStandardMap class.
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <time.h>
#include <Windows.h>
#include <psapi.h>

#include "cStandardMap.h"

sPerfData mapPerformanceData;

void cStandardMap::generatePeople(int numPeople) {

	if (numPeople == INT_MAX) {

		this->mGenerateAllPeople();
	}
	else {

		this->mGenerateNumPeople(numPeople);
	}

	return;
}

// ****** Interface functions start ********//
std::vector<std::string>smMaleNames;
std::vector<std::string>smFemaleNames;
std::vector<std::string>smLastNames;

bool cStandardMap::LoadDataFilesIntoContainer(std::string firstNameFemaleFileName, std::string firstNameMaleFileName, std::string lastNameFileName) {

	// Start recording performance data
	this->mStartPerformanceData(mapPerformanceData);

	// Read the female first names into the array
	std::ifstream file(firstNameFemaleFileName.c_str());
	if (!file.is_open()) {

		std::cout << "Did not open " << firstNameFemaleFileName << std::endl;
		this->mEndPerformanceData(mapPerformanceData);
		return false;
	}

	std::string placeHolder;
	while (!file.eof()) {

		placeHolder = "";
		file >> placeHolder;
		if (placeHolder != "") smFemaleNames.push_back(placeHolder);
		file >> placeHolder;
		file >> placeHolder;
		file >> placeHolder;

		this->mUpdatePerformanceData(mapPerformanceData);
	}

	file.close();

	// Read the male names into the array
	file.open(firstNameMaleFileName.c_str());
	if (!file.is_open()) {

		std::cout << "Did not open " << firstNameMaleFileName << std::endl;
		this->mEndPerformanceData(mapPerformanceData);
		return false;
	}

	while (!file.eof()) {

		placeHolder = "";
		file >> placeHolder;
		if (placeHolder != "") smMaleNames.push_back(placeHolder);
		file >> placeHolder;
		file >> placeHolder;
		file >> placeHolder;

		this->mUpdatePerformanceData(mapPerformanceData);
	}

	file.close();

	// Read the last names into the array
	file.open(lastNameFileName.c_str());
	if (!file.is_open()) {

		std::cout << "Did not open " << lastNameFileName << std::endl;
		this->mEndPerformanceData(mapPerformanceData);
		return false;
	}

	while (!file.eof()) {

		placeHolder = "";
		file >> placeHolder;
		if (placeHolder != "") smLastNames.push_back(placeHolder);
		file >> placeHolder;
		file >> placeHolder;
		file >> placeHolder;

		this->mUpdatePerformanceData(mapPerformanceData);
	}

	file.close();

	// Check that there are first names to use
	if (smFemaleNames.empty() && smMaleNames.empty()) {

		this->mEndPerformanceData(mapPerformanceData);
		return false;
	}

	// Check to make sure we have last names
	if (smLastNames.empty()) {

		this->mEndPerformanceData(mapPerformanceData);
		return false;
	}

	this->mEndPerformanceData(mapPerformanceData);

	return true;
}

bool cStandardMap::FindPeopleByName(std::vector<sPerson>& vecPeople, sPerson personToMatch, int maxNumberOfPeople) {

	// Start recording performance data
	this->mStartPerformanceData(mapPerformanceData);

	vecPeople.clear();

	if (maxNumberOfPeople < 0) {

		this->mEndPerformanceData(mapPerformanceData);
		return false;
	}

	size_t mapSize = this->mData.size();

	// if both names are blank, return everyone
	if (personToMatch.first == "" && personToMatch.last == "") {

		for (int index = 0; index < mapSize; ++index) {

			this->mUpdatePerformanceData(mapPerformanceData);


			if ((int)vecPeople.size() >= maxNumberOfPeople) {

				this->mEndPerformanceData(mapPerformanceData);
				return true;
			}

			vecPeople.push_back(this->mData[index]);
		}

		if (!vecPeople.empty()) {

			this->mEndPerformanceData(mapPerformanceData);
			return true;
		}
		else {

			this->mEndPerformanceData(mapPerformanceData);
			return false;
		}
	}

	// if last name is "", search only by first name
	if (personToMatch.last == "") {

		for (int index = 0; index < mapSize; ++index) {

			this->mUpdatePerformanceData(mapPerformanceData);

			if (personToMatch.first == this->mData[index].first) {

				if ((int)vecPeople.size() >= maxNumberOfPeople) {

					this->mEndPerformanceData(mapPerformanceData);
					return true;
				}
				vecPeople.push_back(this->mData[index]);
			}
		}

		if (!vecPeople.empty()) {

			this->mEndPerformanceData(mapPerformanceData);
			return true;
		}
		else {

			this->mEndPerformanceData(mapPerformanceData);
			return false;
		}
	}

	// if first name is "", search only by last name
	if (personToMatch.first == "") {

		for (int index = 0; index < mapSize; ++index) {

			this->mUpdatePerformanceData(mapPerformanceData);
			if (personToMatch.last == this->mData[index].last) {

				if ((int)vecPeople.size() >= maxNumberOfPeople) {

					this->mUpdatePerformanceData(mapPerformanceData);
					return true;
				}
				vecPeople.push_back(this->mData[index]);
			}
		}

		if (!vecPeople.empty()) {

			this->mEndPerformanceData(mapPerformanceData);
			return true;
		}
		else {

			this->mEndPerformanceData(mapPerformanceData);
			return false;
		}
	}

	// Otherwise compare both first and last name
	for (int index = 0; index < mapSize; ++index) {

		this->mUpdatePerformanceData(mapPerformanceData);
		if (personToMatch.first == this->mData[index].first &&
			personToMatch.last == this->mData[index].last) {

			if ((int)vecPeople.size() >= maxNumberOfPeople) {

				this->mUpdatePerformanceData(mapPerformanceData);
				return true;
			}
			vecPeople.push_back(this->mData[index]);
		}
	}

	if (!vecPeople.empty()) {

		this->mEndPerformanceData(mapPerformanceData);
		return true;
	}
	else {

		this->mEndPerformanceData(mapPerformanceData);
		return false;
	}
}

bool cStandardMap::FindPeopleByName(std::vector<sPerson>& vecPeople, std::vector<sPerson>& vecPeopleToMatch, int maxNumberOfPeople) {

	// Start recording performance data
	this->mStartPerformanceData(mapPerformanceData);

	vecPeople.clear();

	if (maxNumberOfPeople < 0) {

		this->mEndPerformanceData(mapPerformanceData);
		return false;
	}

	size_t mapSize = this->mData.size();

	for (int people = 0; people < vecPeopleToMatch.size(); ++people) {

		this->mUpdatePerformanceData(mapPerformanceData);

		sPerson currPerson = vecPeopleToMatch[people];

		// if both names are blank, return everyone
		if (currPerson.first == "" && currPerson.last == "") {

			for (int index = 0; index < mapSize; ++index) {

				this->mUpdatePerformanceData(mapPerformanceData);


				if ((int)vecPeople.size() >= maxNumberOfPeople) {

					this->mUpdatePerformanceData(mapPerformanceData);
					break;
				}

				vecPeople.push_back(this->mData[index]);
			}

			this->mEndPerformanceData(mapPerformanceData);
			return true;
		}

		// if last name is "", search only by first name
		if (currPerson.last == "") {

			for (int index = 0; index < mapSize; ++index) {

				this->mUpdatePerformanceData(mapPerformanceData);

				if (currPerson.first == this->mData[index].first) {

					if ((int)vecPeople.size() >= maxNumberOfPeople) {

						this->mUpdatePerformanceData(mapPerformanceData);
						break;
					}
					vecPeople.push_back(this->mData[index]);
				}
			}

			this->mUpdatePerformanceData(mapPerformanceData);
			continue;
		}

		// if first name is "", search only by last name
		if (currPerson.first == "") {

			for (int index = 0; index < mapSize; ++index) {

				this->mUpdatePerformanceData(mapPerformanceData);
				if (currPerson.last == this->mData[index].last) {

					if ((int)vecPeople.size() >= maxNumberOfPeople) {

						this->mUpdatePerformanceData(mapPerformanceData);
						break;
					}
					vecPeople.push_back(this->mData[index]);
				}
			}

			this->mUpdatePerformanceData(mapPerformanceData);
			continue;
		}

		// Otherwise compare both first and last name
		for (int index = 0; index < mapSize; ++index) {

			this->mUpdatePerformanceData(mapPerformanceData);
			if (currPerson.first == this->mData[index].first &&
				currPerson.last == this->mData[index].last) {

				if ((int)vecPeople.size() >= maxNumberOfPeople) {

					this->mUpdatePerformanceData(mapPerformanceData);
					break;
				}
				vecPeople.push_back(this->mData[index]);
			}
		}

		this->mUpdatePerformanceData(mapPerformanceData);
		continue;
	}

	if (!vecPeople.empty()) {

		this->mEndPerformanceData(mapPerformanceData);
		return true;
	}
	else {

		this->mEndPerformanceData(mapPerformanceData);
		return false;
	}
}

bool cStandardMap::FindPersonByID(sPerson& person, unsigned long long uniqueID) {

	// Start recording performance data
	this->mStartPerformanceData(mapPerformanceData);

	size_t mapSize = this->mData.size();
	for (size_t index = 0; index < mapSize; ++index) {
		
		this->mUpdatePerformanceData(mapPerformanceData);
		
		if (this->mData[index].uniqueID == uniqueID) {
			
			person = this->mData[index];
			this->mEndPerformanceData(mapPerformanceData);
			return true;
		}
	}

	this->mEndPerformanceData(mapPerformanceData);
	return false;
}

bool cStandardMap::FindPeople(std::vector<sPerson>& vecPeople, sPoint location, float radius, int maxPeopleToReturn) {

	// Start recording performance data
	this->mStartPerformanceData(mapPerformanceData);
	vecPeople.clear();

	if (maxPeopleToReturn < 0) {

		this->mEndPerformanceData(mapPerformanceData);
		return false;
	}

	size_t mapSize = this->mData.size();
	float distance;
	for (size_t index = 0; index < mapSize; ++index) {

		this->mUpdatePerformanceData(mapPerformanceData);
		distance = 0.0f;
		if (this->mIsInsideSphere(location, radius, this->mData[index], distance)) {

			if ((int)vecPeople.size() >= maxPeopleToReturn) {

				this->mUpdatePerformanceData(mapPerformanceData);
				break;
			}
			vecPeople.push_back(this->mData[index]);
		}
	}

	if (!vecPeople.empty()) {

		this->mEndPerformanceData(mapPerformanceData);
		return true;
	}
	else {

		this->mEndPerformanceData(mapPerformanceData);
		return false;
	}
}

bool cStandardMap::FindPeople(std::vector<sPerson>& vecPeople, float minHealth, float maxHealth, int maxPeopleToReturn) {

	// Start recording performance data
	this->mStartPerformanceData(mapPerformanceData);
	vecPeople.clear();

	size_t mapSize = this->mData.size();
	for (size_t index = 0; index < mapSize; ++index) {

		this->mUpdatePerformanceData(mapPerformanceData);
		if ((int)vecPeople.size() > maxPeopleToReturn) {

			this->mEndPerformanceData(mapPerformanceData);
			return true;
		}

		if (this->mData[index].health >= minHealth && this->mData[index].health <= maxHealth) {

			vecPeople.push_back(this->mData[index]);
			this->mUpdatePerformanceData(mapPerformanceData);
		}
	}

	if (!vecPeople.empty()) {

		this->mEndPerformanceData(mapPerformanceData);
		return true;
	}
	else {

		this->mEndPerformanceData(mapPerformanceData);
		return false;
	}
}

bool cStandardMap::FindPeople(std::vector<sPerson>& vecPeople, sPoint location, float radius, float minHealth, float maxHealth, int maxPeopleToReturn) {

	// Start recording performance data
	this->mStartPerformanceData(mapPerformanceData);
	vecPeople.clear();

	if (maxPeopleToReturn < 0) {

		this->mEndPerformanceData(mapPerformanceData);
		return false;
	}

	size_t mapSize = this->mData.size();
	float distance;
	for (size_t index = 0; index < mapSize; ++index) {

		if (this->mData[index].health >= minHealth && this->mData[index].health <= maxHealth) {

			if (this->mIsInsideSphere(location, radius, this->mData[index], distance)) {

				if ((int)vecPeople.size() > maxPeopleToReturn) {

					this->mEndPerformanceData(mapPerformanceData);
					return true;
				}

				vecPeople.push_back(this->mData[index]);
				this->mUpdatePerformanceData(mapPerformanceData);
			}
		}
	}

	if (!vecPeople.empty()) {

		this->mEndPerformanceData(mapPerformanceData);
		return true;
	}
	else {

		this->mEndPerformanceData(mapPerformanceData);
		return false;
	}
}

bool cStandardMap::SortPeople(std::vector<sPerson>& vecPeople, eSortType sortType) {

	// Start recording performance data
	this->mStartPerformanceData(mapPerformanceData);

	if (this->mData.empty()) {

		this->mEndPerformanceData(mapPerformanceData);
		return false;
	}

	switch (sortType) {
	case iPersonMotron::ASC_FIRST_THEN_LAST:
		this->mSortFirstLastNamesAsc();
		this->mUpdatePerformanceData(mapPerformanceData);
		break;
	case iPersonMotron::DESC_FIRST_THEN_LAST:
		this->mSortFirstLastNamesDesc();
		this->mUpdatePerformanceData(mapPerformanceData);
		break;
	case iPersonMotron::ASC_LAST_THEN_FIRST:
		this->mSortLastFirstNamesAsc();
		this->mUpdatePerformanceData(mapPerformanceData);
		break;
	case iPersonMotron::DESC_LAST_THEN_FIRST:
		this->mSortLastFirstNamesDesc();
		this->mUpdatePerformanceData(mapPerformanceData);
		break;
	case iPersonMotron::ASC_BY_ID:
		this->mSortIdAsc();
		this->mUpdatePerformanceData(mapPerformanceData);
		break;
	case iPersonMotron::DESC_BY_ID:
		this->mSortIdDesc();
		this->mUpdatePerformanceData(mapPerformanceData);
		break;
	case iPersonMotron::ASC_BY_HEALTH:
		this->mSortHealthAsc();
		this->mUpdatePerformanceData(mapPerformanceData);
		break;
	case iPersonMotron::DESC_BY_HEALTH:
		this->mSortHealthDesc();
		this->mUpdatePerformanceData(mapPerformanceData);
		break;
	default:
		this->mUpdatePerformanceData(mapPerformanceData);
		return false;
		break;
	}

	for (size_t index = 0; index < this->mData.size(); ++index) {

		vecPeople.push_back(this->mData[index]);
	}
	this->mEndPerformanceData(mapPerformanceData);
	return true;
}

bool cStandardMap::GetPerformanceFromLastCall(sPerfData& callStats) {

	callStats = mapPerformanceData;

	return true;
}

eContainerType cStandardMap::getContainerType(void) {

	this->mStartPerformanceData(mapPerformanceData);

	this->mEndPerformanceData(mapPerformanceData);

	return eContainerType::STD_MAP;
}
// ****** Interface functions end ********//

// ****** Sorting functions start ********//

void cStandardMap::mSortFirstLastNamesAsc() {

	// Make a mirror map, ordering by first, last, the unique ID as a string
	std::map<std::string, sPerson> mirrorMap;
	for (size_t index = 0; index < this->mData.size(); ++index) {

		sPerson currPerson = this->mData[index];

		std::string id = currPerson.first + " " + currPerson.last + " " + std::to_string(currPerson.uniqueID);
		mirrorMap[id] = currPerson;
	}

	// clear the original map
	this->mData.clear();

	// Put the mirror map into the original map
	std::map<std::string, sPerson>::iterator it;
	size_t index = 0;
	for (it = mirrorMap.begin(); it != mirrorMap.end(); ++it) {

		this->mData[index] = it->second;
		index++;
	}

	return;
}

void cStandardMap::mSortFirstLastNamesDesc() {

	// Make a mirror map, ordering by first, last, the unique ID as a string
	std::map<std::string, sPerson> mirrorMap;
	for (size_t index = 0; index < this->mData.size(); ++index) {

		sPerson currPerson = this->mData[index];

		std::string id = currPerson.first + " " + currPerson.last + " " + std::to_string(currPerson.uniqueID);
		mirrorMap[id] = currPerson;
	}

	// clear the original map
	this->mData.clear();

	// Put the mirror map into the original map
	std::map<std::string, sPerson>::iterator it;
	size_t index = mirrorMap.size() - 1;
	for (it = mirrorMap.begin(); it != mirrorMap.end(); ++it) {

		this->mData[index] = it->second;
		index--;
	}

	return;
}

void cStandardMap::mSortLastFirstNamesAsc() {

	// Make a mirror map, ordering by last, first, the unique ID as a string
	std::map<std::string, sPerson> mirrorMap;
	for (size_t index = 0; index < this->mData.size(); ++index) {

		sPerson currPerson = this->mData[index];

		std::string id = currPerson.last + " " + currPerson.first + " " + std::to_string(currPerson.uniqueID);
		mirrorMap[id] = currPerson;
	}

	// clear the original map
	this->mData.clear();

	// Put the mirror map into the original map
	std::map<std::string, sPerson>::iterator it;
	size_t index = 0;
	for (it = mirrorMap.begin(); it != mirrorMap.end(); ++it) {

		this->mData[index] = it->second;
		index++;
	}

	return;
}

void cStandardMap::mSortLastFirstNamesDesc() {

	// Make a mirror map, ordering by last, first, the unique ID as a string
	std::map<std::string, sPerson> mirrorMap;
	for (size_t index = 0; index < this->mData.size(); ++index) {

		sPerson currPerson = this->mData[index];

		std::string id = currPerson.last + " " + currPerson.first + " " + std::to_string(currPerson.uniqueID);
		mirrorMap[id] = currPerson;
	}

	// clear the original map
	this->mData.clear();

	// Put the mirror map into the original map
	std::map<std::string, sPerson>::iterator it;
	size_t index = mirrorMap.size() - 1;
	for (it = mirrorMap.begin(); it != mirrorMap.end(); ++it) {

		this->mData[index] = it->second;
		index--;
	}

	return;
}

void cStandardMap::mSortIdAsc() {

	// Make a mirror map, ordering by unique ID 
	std::map<unsigned long long, sPerson> mirrorMap;
	for (size_t index = 0; index < this->mData.size(); ++index) {

		mirrorMap[this->mData[index].uniqueID] = this->mData[index];
	}

	// clear the original map
	this->mData.clear();

	// Put the mirror map into the original map
	std::map<unsigned long long, sPerson>::iterator it;
	size_t index = 0;
	for (it = mirrorMap.begin(); it != mirrorMap.end(); ++it) {

		this->mData[index] = it->second;
		index++;
	}

	return;
}

void cStandardMap::mSortIdDesc() {

	// Make a mirror map, ordering by unique ID 
	std::map<unsigned long long, sPerson> mirrorMap;
	for (size_t index = 0; index < this->mData.size(); ++index) {

		mirrorMap[this->mData[index].uniqueID] = this->mData[index];
	}

	// clear the original map
	this->mData.clear();

	// Put the mirror map into the original map
	std::map<unsigned long long, sPerson>::iterator it;
	size_t index = mirrorMap.size() - 1;
	for (it = mirrorMap.begin(); it != mirrorMap.end(); ++it) {

		this->mData[index] = it->second;
		index--;
	}

	return;
}

void cStandardMap::mSortHealthAsc() {

	// Make a mirror map, ordering by health, first, last names 
	// and UniqueID (just in case)
	std::map<std::string, sPerson> mirrorMap;
	for (size_t index = 0; index < this->mData.size(); ++index) {
		std::stringstream ss;
		// First the health float number
		// Integral part:
		int integral = (int)this->mData[index].health;
		ss.width(3);
		ss.fill('0');
		ss << integral;
		// Decimal part:
		int decimal = (int)((this->mData[index].health - integral) * 1000000);
		ss << " ";
		ss.width(6);
		ss.fill('0');
		ss << decimal;

		// Other fields just in case 
		// (prevent deletion in case equal health values)
		ss << " " << this->mData[index].first
			<< " " << this->mData[index].last
			<< " " << this->mData[index].uniqueID;
		std::string id = ss.str();
		mirrorMap[id] = this->mData[index];
	}

	// Clear the original map
	this->mData.clear();

	// Put the mirror map back into the original
	std::map<std::string, sPerson>::iterator it;
	size_t index = 0;
	for (it = mirrorMap.begin(); it != mirrorMap.end(); ++it) {

		this->mData[index] = it->second;
		index++;
	}

	return;
}

void cStandardMap::mSortHealthDesc() {

	// Make a mirror map, ordering by health, first, last names 
	// and UniqueID (just in case)
	std::map<std::string, sPerson> mirrorMap;
	for (size_t index = 0; index < this->mData.size(); ++index) {
		std::stringstream ss;
		// First the health float number
		// Integral part:
		int integral = (int)this->mData[index].health;
		ss.width(3);
		ss.fill('0');
		ss << integral;
		// Decimal part:
		int decimal = (int)((this->mData[index].health - integral) * 1000000);
		ss << " ";
		ss.width(6);
		ss.fill('0');
		ss << decimal;

		// Other fields just in case 
		// (prevent deletion in case equal health values)
		ss << " " << this->mData[index].first
			<< " " << this->mData[index].last
			<< " " << this->mData[index].uniqueID;
		std::string id = ss.str();
		mirrorMap[id] = this->mData[index];
	}

	// Clear the original map
	this->mData.clear();

	// Put the mirror map back into the original
	std::map<std::string, sPerson>::iterator it;
	size_t index = mirrorMap.size() - 1;
	for (it = mirrorMap.begin(); it != mirrorMap.end(); ++it) {

		this->mData[index] = it->second;
		index--;
	}

	return;
}
// ****** Sorting functions end ********//

// ****** Performance functions start ********//
clock_t mapStartTime;

void cStandardMap::mStartPerformanceData(sPerfData& pd) {

	// Start the clock
	mapStartTime = clock();

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

void cStandardMap::mUpdatePerformanceData(sPerfData& pd) {

	// Update elapsed time
	pd.elapsedCallTime_ms = ((float)(clock() - mapStartTime) / CLOCKS_PER_SEC) * 1000.0f;

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

void cStandardMap::mEndPerformanceData(sPerfData& pd) {

	// Update elapsed time
	pd.elapsedCallTime_ms = ((float)(clock() - mapStartTime) / CLOCKS_PER_SEC) * 1000.0f;

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
// ****** Performance functions end ********//


bool cStandardMap::mIsInsideSphere(sPoint point, float radius, sPerson person, float& distance) {

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

void cStandardMap::mGenerateAllPeople() {

	this->mStartPerformanceData(mapPerformanceData);

	// Now generate the sPerson objects
	size_t ID = 0;
	size_t mapIndex = 0;
	srand(time(NULL));

	// Generate all the males with a random last name
	for (size_t index = 0; index < smMaleNames.size(); ++index) {

		sPerson newPerson;
		newPerson.uniqueID = ID;
		int maleIndex = rand() % smMaleNames.size();
		newPerson.first = smMaleNames[maleIndex];

		int lastIndex = rand() % smLastNames.size();
		newPerson.last = smLastNames[lastIndex];

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

		this->mData[mapIndex] = newPerson;
		this->mUpdatePerformanceData(mapPerformanceData);

		ID++;
		mapIndex++;
		this->mUpdatePerformanceData(mapPerformanceData);
	}

	// Now create all the female names
	for (size_t index = 0; index < smFemaleNames.size(); ++index) {

		sPerson newPerson;
		newPerson.uniqueID = ID;
		int femaleIndex = rand() % smFemaleNames.size();
		newPerson.first = smFemaleNames[femaleIndex];

		int lastIndex = rand() % smLastNames.size();
		newPerson.last = smLastNames[lastIndex];

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

		this->mData[mapIndex] = newPerson;
		this->mUpdatePerformanceData(mapPerformanceData);

		ID++;
		mapIndex++;
		this->mUpdatePerformanceData(mapPerformanceData);
	}

	return;
}

void cStandardMap::mGenerateNumPeople(int numPeople) {

	this->mStartPerformanceData(mapPerformanceData);

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
			int femaleIndex = rand() % smFemaleNames.size();
			newPerson.first = smFemaleNames[femaleIndex];

			int lastIndex = rand() % smLastNames.size();
			newPerson.last = smLastNames[lastIndex];

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

			this->mData[mapIndex] = newPerson;
			this->mUpdatePerformanceData(mapPerformanceData);
		}
		break;
		case 1:
		{
			sPerson newPerson;
			newPerson.uniqueID = ID;
			int maleIndex = rand() % smMaleNames.size();
			newPerson.first = smMaleNames[maleIndex];

			int lastIndex = rand() % smLastNames.size();
			newPerson.last = smLastNames[lastIndex];

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

			this->mData[mapIndex] = newPerson;
			this->mUpdatePerformanceData(mapPerformanceData);
		}
		default:
			this->mUpdatePerformanceData(mapPerformanceData);
			break;
		}

		ID++;
		mapIndex++;
		this->mUpdatePerformanceData(mapPerformanceData);
	}

	this->mEndPerformanceData(mapPerformanceData);

	return;
}

