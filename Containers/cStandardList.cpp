/**
 * @file cStandardList.cpp
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 *
 * @section DESCRIPTION
 *
 * Implementation of the cStandardList class.
 */

#include <fstream>
#include <iostream>
#include <algorithm>
#include <time.h>
#include <Windows.h>
#include <psapi.h>

#include "cStandardList.h"
#include "STLComparers.h"


sPerfData listPerformanceData;
void cStandardList::generatePeople(int numPeople) {

	if (numPeople == INT_MAX) {

		this->mGenerateAllPeople();
	}
	else {

		this->mGenerateNumPeople(numPeople);
	}

	return;
}

// ****** Interface functions start ********//
std::vector<std::string>slMaleNames;
std::vector<std::string>slFemaleNames;
std::vector<std::string>slLastNames;

bool cStandardList::LoadDataFilesIntoContainer(std::string firstNameFemaleFileName, std::string firstNameMaleFileName, std::string lastNameFileName) {
	
	// Start recording performance data
	this->mStartPerformanceData(listPerformanceData);

	// Read the female first names into the array
	std::ifstream file(firstNameFemaleFileName.c_str());
	if (!file.is_open()) {

		std::cout << "Did not open " << firstNameFemaleFileName << std::endl;
		this->mEndPerformanceData(listPerformanceData);
		return false;
	}

	std::string placeHolder;
	while (!file.eof()) {

		placeHolder = "";
		file >> placeHolder;
		if (placeHolder != "") slFemaleNames.push_back(placeHolder);
		file >> placeHolder;
		file >> placeHolder;
		file >> placeHolder;

		this->mUpdatePerformanceData(listPerformanceData);
	}

	file.close();

	// Read the male names into the array
	file.open(firstNameMaleFileName.c_str());
	if (!file.is_open()) {

		std::cout << "Did not open " << firstNameMaleFileName << std::endl;
		this->mEndPerformanceData(listPerformanceData);
		return false;
	}

	while (!file.eof()) {

		placeHolder = "";
		file >> placeHolder;
		if (placeHolder != "") slMaleNames.push_back(placeHolder);
		file >> placeHolder;
		file >> placeHolder;
		file >> placeHolder;

		this->mUpdatePerformanceData(listPerformanceData);
	}

	file.close();

	// Read the last names into the array
	file.open(lastNameFileName.c_str());
	if (!file.is_open()) {

		std::cout << "Did not open " << lastNameFileName << std::endl;
		this->mEndPerformanceData(listPerformanceData);
		return false;
	}

	while (!file.eof()) {

		placeHolder = "";
		file >> placeHolder;
		if (placeHolder != "") slLastNames.push_back(placeHolder);
		file >> placeHolder;
		file >> placeHolder;
		file >> placeHolder;

		this->mUpdatePerformanceData(listPerformanceData);
	}

	file.close();

	// Check that there are first names to use
	if (slFemaleNames.empty() && slMaleNames.empty()) {

		this->mEndPerformanceData(listPerformanceData);
		return false;
	}

	// Check to make sure we have last names
	if (slLastNames.empty()) {

		this->mEndPerformanceData(listPerformanceData);
		return false;
	}

	this->mEndPerformanceData(listPerformanceData);

	return true;
}

bool cStandardList::FindPeopleByName(std::vector<sPerson>& vecPeople, sPerson personToMatch, int maxNumberOfPeople) {
	
	// Start recording performance data
	this->mStartPerformanceData(listPerformanceData);
	vecPeople.clear();

	if (maxNumberOfPeople < 0) {

		this->mEndPerformanceData(listPerformanceData);
		return false;
	}

	size_t listSize = this->mData.size();
	std::list<sPerson>::iterator it = this->mData.begin();
	// If both names are blank, return everyone
	if (personToMatch.first == "" && personToMatch.last == "") {

		for (size_t index = 0; index < listSize; ++index) {

			this->mUpdatePerformanceData(listPerformanceData);
			if ((int)vecPeople.size() >= maxNumberOfPeople) {

				this->mEndPerformanceData(listPerformanceData);
				return true;
			}
			vecPeople.push_back(*it);
			it++;
		}

		if (!vecPeople.empty()) {

			this->mEndPerformanceData(listPerformanceData);
			return true;
		}
		else {

			this->mEndPerformanceData(listPerformanceData);
			return false;
		}
	}

	// If last name is "", search only by first name
	if (personToMatch.last == "") {

		for (size_t index = 0; index < listSize; ++index) {

			this->mUpdatePerformanceData(listPerformanceData);
			if (personToMatch.first == it->first) {

				if ((int)vecPeople.size() >= maxNumberOfPeople) {

					this->mEndPerformanceData(listPerformanceData);
					return true;
				}
				vecPeople.push_back(*it);
			}
			it++;
		}

		if (!vecPeople.empty()) {

			this->mEndPerformanceData(listPerformanceData);
			return true;
		}
		else {

			this->mEndPerformanceData(listPerformanceData);
			return false;
		}
	}

	// If first name is "", search only by last name
	if (personToMatch.first == "") {

		for (size_t index = 0; index < listSize; ++index) {

			this->mUpdatePerformanceData(listPerformanceData);
			if (personToMatch.last == it->last) {

				if ((int)vecPeople.size() >= maxNumberOfPeople) {

					this->mEndPerformanceData(listPerformanceData);
					return true;
				}
				vecPeople.push_back(*it);
			}
			it++;
		}

		if (!vecPeople.empty()) {

			this->mEndPerformanceData(listPerformanceData);
			return true;
		}
		else {

			this->mEndPerformanceData(listPerformanceData);
			return false;
		}
	}

	// Otherwise match on both names
	for (size_t index = 0; index < listSize; ++index) {

		this->mUpdatePerformanceData(listPerformanceData);
		if (personToMatch.first == it->first &&
			personToMatch.last == it->last) {

			if ((int)vecPeople.size() >= maxNumberOfPeople) {

				this->mEndPerformanceData(listPerformanceData);
				return true;
			}
			vecPeople.push_back(*it);
		}
		it++;
	}

	if (!vecPeople.empty()) {

		this->mEndPerformanceData(listPerformanceData);
		return true;
	}
	else {

		this->mEndPerformanceData(listPerformanceData);
		return false;
	}
}

bool cStandardList::FindPeopleByName(std::vector<sPerson>& vecPeople, std::vector<sPerson>& vecPeopleToMatch, int maxNumberOfPeople) {
	
	// Start recording performance data
	this->mStartPerformanceData(listPerformanceData);
	vecPeople.clear();

	if (maxNumberOfPeople < 0) {

		this->mEndPerformanceData(listPerformanceData);
		return false;
	}

	size_t listSize = this->mData.size();
	for (size_t people = 0; people < vecPeopleToMatch.size(); ++people) {

		sPerson currPerson = vecPeopleToMatch[people];
		std::list<sPerson>::iterator it = this->mData.begin();
		this->mUpdatePerformanceData(listPerformanceData);

		// if both names are blank, return everyone
		if (currPerson.first == "" && currPerson.last == "") {

			for (size_t index = 0; index < listSize; ++index) {

				this->mUpdatePerformanceData(listPerformanceData);


				if ((int)vecPeople.size() >= maxNumberOfPeople) {

					this->mUpdatePerformanceData(listPerformanceData);
					break;
				}

				vecPeople.push_back(*it);
				it++;
			}

			this->mEndPerformanceData(listPerformanceData);
			return true;
		}

		// if last name is "", search only by first name
		if (currPerson.last == "") {

			for (size_t index = 0; index < listSize; ++index) {

				this->mUpdatePerformanceData(listPerformanceData);

				if (currPerson.first == it->first) {

					if ((int)vecPeople.size() >= maxNumberOfPeople) {

						this->mUpdatePerformanceData(listPerformanceData);
						break;
					}
					vecPeople.push_back(*it);
				}
				it++;
			}

			this->mUpdatePerformanceData(listPerformanceData);
			continue;
		}

		// if first name is "", search only by last name
		if (currPerson.first == "") {

			for (size_t index = 0; index < listSize; ++index) {

				this->mUpdatePerformanceData(listPerformanceData);
				if (currPerson.last == it->last) {

					if ((int)vecPeople.size() >= maxNumberOfPeople) {

						this->mUpdatePerformanceData(listPerformanceData);
						break;
					}
					vecPeople.push_back(*it);
				}
				it++;
			}

			this->mUpdatePerformanceData(listPerformanceData);
			continue;
		}

		// Otherwise compare both first and last name
		for (size_t index = 0; index < listSize; ++index) {

			this->mUpdatePerformanceData(listPerformanceData);
			if (currPerson.first == it->first &&
				currPerson.last == it->last) {

				if ((int)vecPeople.size() >= maxNumberOfPeople) {

					this->mUpdatePerformanceData(listPerformanceData);
					break;
				}
				vecPeople.push_back(*it);
			}
			it++;
		}

		this->mUpdatePerformanceData(listPerformanceData);
		continue;
	}

	if (!vecPeople.empty()) {

		this->mEndPerformanceData(listPerformanceData);
		return true;
	}
	else {

		this->mEndPerformanceData(listPerformanceData);
		return false;
	}
}

bool cStandardList::FindPersonByID(sPerson& person, unsigned long long uniqueID) {
	
	// Start recording performance data
	this->mStartPerformanceData(listPerformanceData);

	std::list<sPerson>::iterator it = std::find_if(this->mData.begin(), this->mData.end(), [uniqueID](sPerson& person) {
		return person.uniqueID == uniqueID;
	});
	this->mUpdatePerformanceData(listPerformanceData);

	if (it != this->mData.end()) {

		person = *it;
		this->mEndPerformanceData(listPerformanceData);
		return true;
	}
	else {

		this->mEndPerformanceData(listPerformanceData);
		return false;
	}
}

bool cStandardList::FindPeople(std::vector<sPerson>& vecPeople, sPoint location, float radius, int maxPeopleToReturn) {
	
	// Start recording performance data
	this->mStartPerformanceData(listPerformanceData);
	vecPeople.clear();

	if (maxPeopleToReturn < 0) {

		this->mEndPerformanceData(listPerformanceData);
		return false;
	}

	size_t listSize = this->mData.size();
	std::list<sPerson>::iterator it = this->mData.begin();
	float distance;
	for (size_t index = 0; index < listSize; ++index) {

		this->mUpdatePerformanceData(listPerformanceData);
		distance = 0.0f;
		if (this->mIsInsideSphere(location, radius, *it, distance)) {

			if ((int)vecPeople.size() >= maxPeopleToReturn) {

				this->mUpdatePerformanceData(listPerformanceData);
				break;
			}

			vecPeople.push_back(*it);
		}
		it++;
	}

	if (!vecPeople.empty()) {

		this->mEndPerformanceData(listPerformanceData);
		return true;
	}
	else {

		this->mEndPerformanceData(listPerformanceData);
		return false;
	}
}

bool cStandardList::FindPeople(std::vector<sPerson>& vecPeople, float minHealth, float maxHealth, int maxPeopleToReturn) {
	
	// Start recording performance data
	this->mStartPerformanceData(listPerformanceData);
	vecPeople.clear();

	size_t listSize = this->mData.size();
	std::list<sPerson>::iterator it = this->mData.begin();
	for (size_t index = 0; index < listSize; ++index) {

		this->mUpdatePerformanceData(listPerformanceData);
		if ((int)vecPeople.size() > maxPeopleToReturn) {

			this->mEndPerformanceData(listPerformanceData);
			return true;
		}

		if (it->health >= minHealth && it->health <= maxHealth) {

			vecPeople.push_back(*it);
			this->mUpdatePerformanceData(listPerformanceData);
		}
		it++;
	}

	if (!vecPeople.empty()) {

		this->mEndPerformanceData(listPerformanceData);
		return true;
	}
	else {

		this->mEndPerformanceData(listPerformanceData);
		return false;
	}
}

bool cStandardList::FindPeople(std::vector<sPerson>& vecPeople, sPoint location, float radius, float minHealth, float maxHealth, int maxPeopleToReturn) {
	
	// Start recording performance data
	this->mStartPerformanceData(listPerformanceData);
	vecPeople.clear();

	if (maxPeopleToReturn < 0) {

		this->mEndPerformanceData(listPerformanceData);
		return false;
	}

	size_t listSize = this->mData.size();
	std::list<sPerson>::iterator it = this->mData.begin();
	float distance;
	for (size_t index = 0; index < listSize; ++index) {

		if (it->health >= minHealth && it->health <= maxHealth) {

			if (this->mIsInsideSphere(location, radius, *it, distance)) {

				if ((int)vecPeople.size() > maxPeopleToReturn) {

					this->mEndPerformanceData(listPerformanceData);
					return true;
				}

				vecPeople.push_back(*it);
				this->mUpdatePerformanceData(listPerformanceData);
			}
		}
		it++;
	}

	if (!vecPeople.empty()) {

		this->mEndPerformanceData(listPerformanceData);
		return true;
	}
	else {

		this->mEndPerformanceData(listPerformanceData);
		return false;
	}
}

bool cStandardList::SortPeople(std::vector<sPerson>& vecPeople, eSortType sortType) {
	
	// Start recording performance data
	this->mStartPerformanceData(listPerformanceData);

	if (this->mData.empty()) {

		this->mEndPerformanceData(listPerformanceData);
		return false;
	}

	switch (sortType) {
	case iPersonMotron::ASC_FIRST_THEN_LAST:
		this->mData.sort(sortFirstLastNamesAsc);
		this->mUpdatePerformanceData(listPerformanceData);
		break;
	case iPersonMotron::DESC_FIRST_THEN_LAST:
		this->mData.sort(sortFirstLastNamesDesc);
		this->mUpdatePerformanceData(listPerformanceData);
		break;
	case iPersonMotron::ASC_LAST_THEN_FIRST:
		this->mData.sort(sortLastFristNamesAsc);
		this->mUpdatePerformanceData(listPerformanceData);
		break;
	case iPersonMotron::DESC_LAST_THEN_FIRST:
		this->mData.sort(sortLastFirstNamesDesc);
		this->mUpdatePerformanceData(listPerformanceData);
		break;
	case iPersonMotron::ASC_BY_ID:
		this->mData.sort(sortIdAsc);
		this->mUpdatePerformanceData(listPerformanceData);
		break;
	case iPersonMotron::DESC_BY_ID:
		this->mData.sort(sortIdDesc);
		this->mUpdatePerformanceData(listPerformanceData);
		break;
	case iPersonMotron::ASC_BY_HEALTH:
		this->mData.sort(sortHealthAsc);
		this->mUpdatePerformanceData(listPerformanceData);
		break;
	case iPersonMotron::DESC_BY_HEALTH:
		this->mData.sort(sortHealthDesc);
		this->mUpdatePerformanceData(listPerformanceData);
		break;
	default:
		this->mUpdatePerformanceData(listPerformanceData);
		return false;
		break;
	}

	std::list<sPerson>::iterator it;
	for (it = this->mData.begin(); it != this->mData.end(); ++it) {

		vecPeople.push_back(*it);
	}
	this->mEndPerformanceData(listPerformanceData);
	return true;
}

bool cStandardList::GetPerformanceFromLastCall(sPerfData& callStats) {
	
	callStats = listPerformanceData;

	return true;
}

eContainerType cStandardList::getContainerType(void) {
	
	this->mStartPerformanceData(listPerformanceData);

	this->mEndPerformanceData(listPerformanceData);

	return eContainerType::STD_LIST;
}
//******* Interface functions end ************//

//******* Performance functions start ************//
clock_t listStartTime;
void cStandardList::mStartPerformanceData(sPerfData& pd) {

	// Start the clock
	listStartTime = clock();

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

void cStandardList::mUpdatePerformanceData(sPerfData& pd) {

	// Update elapsed time
	pd.elapsedCallTime_ms = ((float)(clock() - listStartTime) / CLOCKS_PER_SEC) * 1000.0f;

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

void cStandardList::mEndPerformanceData(sPerfData& pd) {

	// Update elapsed time
	pd.elapsedCallTime_ms = ((float)(clock() - listStartTime) / CLOCKS_PER_SEC) * 1000.0f;

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
//******* Performance functions end ************//

bool cStandardList::mIsInsideSphere(sPoint point, float radius, sPerson person, float& distance) {
	
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

void cStandardList::mGenerateAllPeople() {

	this->mStartPerformanceData(listPerformanceData);

	// Now generate the sPerson objects
	size_t ID = 0;
	srand(time(NULL));

	// Generate all the males with a random last name
	for (size_t index = 0; index < slMaleNames.size(); ++index) {

		sPerson newPerson;
		newPerson.uniqueID = ID;
		int maleIndex = rand() % slMaleNames.size();
		newPerson.first = slMaleNames[maleIndex];

		int lastIndex = rand() % slLastNames.size();
		newPerson.last = slLastNames[lastIndex];

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

		this->mData.push_back(newPerson);
		this->mUpdatePerformanceData(listPerformanceData);

		ID++;
		this->mUpdatePerformanceData(listPerformanceData);
	}

	// Now create all the female names
	for (size_t index = 0; index < slFemaleNames.size(); ++index) {

		sPerson newPerson;
		newPerson.uniqueID = ID;
		int femaleIndex = rand() % slFemaleNames.size();
		newPerson.first = slFemaleNames[femaleIndex];

		int lastIndex = rand() % slLastNames.size();
		newPerson.last = slLastNames[lastIndex];

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

		this->mData.push_back(newPerson);
		this->mUpdatePerformanceData(listPerformanceData);

		ID++;
		this->mUpdatePerformanceData(listPerformanceData);
	}

	return;
}

void cStandardList::mGenerateNumPeople(int numPeople) {

	this->mStartPerformanceData(listPerformanceData);

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
			int femaleIndex = rand() % slFemaleNames.size();
			newPerson.first = slFemaleNames[femaleIndex];

			int lastIndex = rand() % slLastNames.size();
			newPerson.last = slLastNames[lastIndex];

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

			this->mData.push_back(newPerson);
			this->mUpdatePerformanceData(listPerformanceData);
		}
		break;
		case 1:
		{
			sPerson newPerson;
			newPerson.uniqueID = ID;
			int maleIndex = rand() % slMaleNames.size();
			newPerson.first = slMaleNames[maleIndex];

			int lastIndex = rand() % slLastNames.size();
			newPerson.last = slLastNames[lastIndex];

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

			this->mData.push_back(newPerson);
			this->mUpdatePerformanceData(listPerformanceData);
		}
		default:
			this->mUpdatePerformanceData(listPerformanceData);
			break;
		}

		ID++;
		this->mUpdatePerformanceData(listPerformanceData);
	}

	this->mEndPerformanceData(listPerformanceData);

	return;
}