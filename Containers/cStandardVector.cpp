/**
 * @file cStandardVector.cpp
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 *
 * @section DESCRIPTION
 *
 * Implementation of th cStandardVector class.
 */

#include <fstream>
#include <iostream>
#include <algorithm>
#include <time.h>
#include <Windows.h>
#include <psapi.h>

#include "cStandardVector.h"
#include "STLComparers.h"

sPerfData performanceData;
void cStandardVector::generatePeople(int numPeople) {

	if (numPeople == INT_MAX) {

		this->mGenerateAllPeople();
	}
	else {

		this->mGenerateNumPeople(numPeople);
	}

	return;
}

// ****** Interface functions start ********//

std::vector<std::string>svMaleNames;
std::vector<std::string>svFemaleNames;
std::vector<std::string>svLastNames;

bool cStandardVector::LoadDataFilesIntoContainer(std::string firstNameFemaleFileName, std::string firstNameMaleFileName, std::string lastNameFileName) {

	// Start recording performance data
	this->mStartPerformanceData(performanceData);

	// Read the female first names into the array
	std::ifstream file(firstNameFemaleFileName.c_str());
	if (!file.is_open()) {

		std::cout << "Did not open " << firstNameFemaleFileName << std::endl;
		this->mEndPerformanceData(performanceData);
		return false;
	}

	std::string placeHolder;
	while (!file.eof()) {

		placeHolder = "";
		file >> placeHolder;
		if (placeHolder != "") svFemaleNames.push_back(placeHolder);
		file >> placeHolder;
		file >> placeHolder;
		file >> placeHolder;
	
		this->mUpdatePerformanceData(performanceData);
	}

	file.close();

	// Read the male names into the array
	file.open(firstNameMaleFileName.c_str());
	if (!file.is_open()) {

		std::cout << "Did not open " << firstNameMaleFileName << std::endl;
		this->mEndPerformanceData(performanceData);
		return false;
	}

	while (!file.eof()) {

		placeHolder = "";
		file >> placeHolder;
		if (placeHolder != "") svMaleNames.push_back(placeHolder);
		file >> placeHolder;
		file >> placeHolder;
		file >> placeHolder;
		
		this->mUpdatePerformanceData(performanceData);
	}

	file.close();

	// Read the last names into the array
	file.open(lastNameFileName.c_str());
	if (!file.is_open()) {

		std::cout << "Did not open " << lastNameFileName << std::endl;
		this->mEndPerformanceData(performanceData);
		return false;
	}

	while (!file.eof()) {

		placeHolder = "";
		file >> placeHolder;
		if (placeHolder != "") svLastNames.push_back(placeHolder);
		file >> placeHolder;
		file >> placeHolder;
		file >> placeHolder;
		
		this->mUpdatePerformanceData(performanceData);
	}

	file.close();

	// Check that there are first names to use
	if (svFemaleNames.empty() && svMaleNames.empty()) {

		this->mEndPerformanceData(performanceData);
		return false;
	}

	// Check to make sure we have last names
	if (svLastNames.empty()) {

		this->mEndPerformanceData(performanceData);
		return false;
	}

	this->mEndPerformanceData(performanceData);

	return true;
}

bool cStandardVector::FindPeopleByName(std::vector<sPerson>& vecPeople, sPerson personToMatch, int maxNumberOfPeople) {

	// Start recording performance data
	this->mStartPerformanceData(performanceData);

	vecPeople.clear();

	if (maxNumberOfPeople < 0) {

		this->mEndPerformanceData(performanceData);
		return false;
	}

	size_t vecSize = this->mData.size();

	// if both names are blank, return everyone
	if (personToMatch.first == "" && personToMatch.last == "") {

		for (int index = 0; index < vecSize; ++index) {

			this->mUpdatePerformanceData(performanceData);


			if ((int)vecPeople.size() >= maxNumberOfPeople) {

				this->mEndPerformanceData(performanceData);
				return true;
			}

			vecPeople.push_back(this->mData[index]);
		}

		if (!vecPeople.empty()) {

			this->mEndPerformanceData(performanceData);
			return true;
		}
		else {

			this->mEndPerformanceData(performanceData);
			return false;
		}
	}

	// if last name is "", search only by first name
	if (personToMatch.last == "") {

		for (int index = 0; index < vecSize; ++index) {

			this->mUpdatePerformanceData(performanceData);

			if (personToMatch.first == this->mData[index].first) {

				if ((int)vecPeople.size() >= maxNumberOfPeople) {

					this->mEndPerformanceData(performanceData);
					return true;
				}
				vecPeople.push_back(this->mData[index]);
			}
		}

		if (!vecPeople.empty()) {

			this->mEndPerformanceData(performanceData);
			return true;
		}
		else {

			this->mEndPerformanceData(performanceData);
			return false;
		}
	}

	// if first name is "", search only by last name
	if (personToMatch.first == "") {

		for (int index = 0; index < vecSize; ++index) {

			this->mUpdatePerformanceData(performanceData);
			if (personToMatch.last == this->mData[index].last) {

				if ((int)vecPeople.size() >= maxNumberOfPeople) {

					this->mUpdatePerformanceData(performanceData);
					return true;
				}
				vecPeople.push_back(this->mData[index]);
			}
		}

		if (!vecPeople.empty()) {

			this->mEndPerformanceData(performanceData);
			return true;
		}
		else {

			this->mEndPerformanceData(performanceData);
			return false;
		}
	}

	// Otherwise compare both first and last name
	for (int index = 0; index < vecSize; ++index) {

		this->mUpdatePerformanceData(performanceData);
		if (personToMatch.first == this->mData[index].first &&
			personToMatch.last == this->mData[index].last) {

			if ((int)vecPeople.size() >= maxNumberOfPeople) {

				this->mUpdatePerformanceData(performanceData);
				return true;
			}
			vecPeople.push_back(this->mData[index]);
		}
	}

	if (!vecPeople.empty()) {

		this->mEndPerformanceData(performanceData);
		return true;
	}
	else {

		this->mEndPerformanceData(performanceData);
		return false;
	}
}

bool cStandardVector::FindPeopleByName(std::vector<sPerson>& vecPeople, std::vector<sPerson>& vecPeopleToMatch, int maxNumberOfPeople) {

	// Start recording performance data
	this->mStartPerformanceData(performanceData);

	vecPeople.clear();

	if (maxNumberOfPeople < 0) {

		this->mEndPerformanceData(performanceData);
		return false;
	}

	size_t vecSize = this->mData.size();

	for (int people = 0; people < vecPeopleToMatch.size(); ++people) {
		
		this->mUpdatePerformanceData(performanceData);

		sPerson currPerson = vecPeopleToMatch[people];

		// if both names are blank, return everyone
		if (currPerson.first == "" && currPerson.last == "") {

			for (int index = 0; index < vecSize; ++index) {

				this->mUpdatePerformanceData(performanceData);


				if ((int)vecPeople.size() >= maxNumberOfPeople) {

					this->mUpdatePerformanceData(performanceData);
					break;
				}

				vecPeople.push_back(this->mData[index]);
			}

			this->mEndPerformanceData(performanceData);
			return true;
		}

		// if last name is "", search only by first name
		if (currPerson.last == "") {

			for (int index = 0; index < vecSize; ++index) {

				this->mUpdatePerformanceData(performanceData);

				if (currPerson.first == this->mData[index].first) {

					if ((int)vecPeople.size() >= maxNumberOfPeople) {

						this->mUpdatePerformanceData(performanceData);
						break;
					}
					vecPeople.push_back(this->mData[index]);
				}
			}

			this->mUpdatePerformanceData(performanceData);
			continue;
		}

		// if first name is "", search only by last name
		if (currPerson.first == "") {

			for (int index = 0; index < vecSize; ++index) {

				this->mUpdatePerformanceData(performanceData);
				if (currPerson.last == this->mData[index].last) {

					if ((int)vecPeople.size() >= maxNumberOfPeople) {

						this->mUpdatePerformanceData(performanceData);
						break;
					}
					vecPeople.push_back(this->mData[index]);
				}
			}

			this->mUpdatePerformanceData(performanceData);
			continue;
		}

		// Otherwise compare both first and last name
		for (int index = 0; index < vecSize; ++index) {

			this->mUpdatePerformanceData(performanceData);
			if (currPerson.first == this->mData[index].first &&
				currPerson.last == this->mData[index].last) {

				if ((int)vecPeople.size() >= maxNumberOfPeople) {

					this->mUpdatePerformanceData(performanceData);
					break;
				}
				vecPeople.push_back(this->mData[index]);
			}
		}

		this->mUpdatePerformanceData(performanceData);
		continue;
	}

	if (!vecPeople.empty()) {

		this->mEndPerformanceData(performanceData);
		return true;
	}
	else {

		this->mEndPerformanceData(performanceData);
		return false;
	}
}

bool cStandardVector::FindPersonByID(sPerson& person, unsigned long long uniqueID) {

	// Start recording performance data
	this->mStartPerformanceData(performanceData);

	std::vector<sPerson>::iterator it = std::find_if(this->mData.begin(), this->mData.end(), [uniqueID](sPerson& person) {
		return person.uniqueID == uniqueID;
	});
	this->mUpdatePerformanceData(performanceData);

	if (it != this->mData.end()) {

		person = *it;
		this->mEndPerformanceData(performanceData);
		return true;
	}
	else {

		this->mEndPerformanceData(performanceData);
		return false;
	}
}

bool cStandardVector::FindPeople(std::vector<sPerson>& vecPeople, sPoint location, float radius, int maxPeopleToReturn) {

	// Start recording performance data
	this->mStartPerformanceData(performanceData);
	vecPeople.clear();

	if (maxPeopleToReturn < 0) {

		this->mEndPerformanceData(performanceData);
		return false;
	}

	size_t vecSize = this->mData.size();
	float distance;
	for (size_t index = 0; index < vecSize; ++index) {

		this->mUpdatePerformanceData(performanceData);
		distance = 0.0f;
		if (this->mIsInsideSphere(location, radius, this->mData[index], distance)) {

			if ((int)vecPeople.size() >= maxPeopleToReturn) {

				this->mUpdatePerformanceData(performanceData);
				break;
			}
			vecPeople.push_back(this->mData[index]);
		}
	}

	if (!vecPeople.empty()) {

		this->mEndPerformanceData(performanceData);
		return true;
	}
	else {

		this->mEndPerformanceData(performanceData);
		return false;
	}
}

bool cStandardVector::FindPeople(std::vector<sPerson>& vecPeople, float minHealth, float maxHealth, int maxPeopleToReturn) {

	// Start recording performance data
	this->mStartPerformanceData(performanceData);
	vecPeople.clear();

	size_t vecSize = this->mData.size();
	for (size_t index = 0; index < vecSize; ++index) {

		this->mUpdatePerformanceData(performanceData);
		if ((int)vecPeople.size() > maxPeopleToReturn) {

			this->mEndPerformanceData(performanceData);
			return true;
		}

		if (this->mData[index].health >= minHealth && this->mData[index].health <= maxHealth) {

			vecPeople.push_back(this->mData[index]);
			this->mUpdatePerformanceData(performanceData);
		}
	}

	if (!vecPeople.empty()) {

		this->mEndPerformanceData(performanceData);
		return true;
	}
	else {

		this->mEndPerformanceData(performanceData);
		return false;
	}
}

bool cStandardVector::FindPeople(std::vector<sPerson>& vecPeople, sPoint location, float radius, float minHealth, float maxHealth, int maxPeopleToReturn) {

	// Start recording performance data
	this->mStartPerformanceData(performanceData);
	vecPeople.clear();

	if (maxPeopleToReturn < 0) {

		this->mEndPerformanceData(performanceData);
		return false;
	}

	size_t vecSize = this->mData.size();
	float distance;
	for (size_t index = 0; index < vecSize; ++index) {

		if (this->mData[index].health >= minHealth && this->mData[index].health <= maxHealth) {

			if (this->mIsInsideSphere(location, radius, this->mData[index], distance)) {

				if ((int)vecPeople.size() > maxPeopleToReturn) {

					this->mEndPerformanceData(performanceData);
					return true;
				}

				vecPeople.push_back(this->mData[index]);
				this->mUpdatePerformanceData(performanceData);
			}
		}
	}

	if (!vecPeople.empty()) {

		this->mEndPerformanceData(performanceData);
		return true;
	}
	else {

		this->mEndPerformanceData(performanceData);
		return false;
	}
}

bool cStandardVector::SortPeople(std::vector<sPerson>& vecPeople, eSortType sortType) {

	// Start recording performance data
	this->mStartPerformanceData(performanceData);

	if (this->mData.empty()) {

		this->mEndPerformanceData(performanceData);
		return false;
	}

	switch (sortType) {
	case iPersonMotron::ASC_FIRST_THEN_LAST:
		std::sort(this->mData.begin(), this->mData.end(), sortFirstLastNamesAsc);
		this->mUpdatePerformanceData(performanceData);
		break;
	case iPersonMotron::DESC_FIRST_THEN_LAST:
		std::sort(this->mData.begin(), this->mData.end(), sortFirstLastNamesDesc);
		this->mUpdatePerformanceData(performanceData);
		break;
	case iPersonMotron::ASC_LAST_THEN_FIRST:
		std::sort(this->mData.begin(), this->mData.end(), sortLastFristNamesAsc);
		this->mUpdatePerformanceData(performanceData);
		break;
	case iPersonMotron::DESC_LAST_THEN_FIRST:
		std::sort(this->mData.begin(), this->mData.end(), sortLastFirstNamesDesc);
		this->mUpdatePerformanceData(performanceData);
		break;
	case iPersonMotron::ASC_BY_ID:
		std::sort(this->mData.begin(), this->mData.end(), sortIdAsc);
		this->mUpdatePerformanceData(performanceData);
		break;
	case iPersonMotron::DESC_BY_ID:
		std::sort(this->mData.begin(), this->mData.end(), sortIdDesc);
		this->mUpdatePerformanceData(performanceData);
		break;
	case iPersonMotron::ASC_BY_HEALTH:
		std::sort(this->mData.begin(), this->mData.end(), sortHealthAsc);
		this->mUpdatePerformanceData(performanceData);
		break;
	case iPersonMotron::DESC_BY_HEALTH:
		std::sort(this->mData.begin(), this->mData.end(), sortHealthDesc);
		this->mUpdatePerformanceData(performanceData);
		break;
	default:
		this->mUpdatePerformanceData(performanceData);
		return false;
		break;
	}

	vecPeople = this->mData;
	this->mEndPerformanceData(performanceData);
	return true;
}

bool cStandardVector::GetPerformanceFromLastCall(sPerfData& callStats) {

	callStats = performanceData;
	return true;
}

eContainerType cStandardVector::getContainerType(void) {

	this->mStartPerformanceData(performanceData);

	this->mEndPerformanceData(performanceData);

	return eContainerType::STD_VECTOR;
}

// ******Intferface functions end ***********//


// ****** Performance functions start ***********//
clock_t vectorStartTime;

void cStandardVector::mStartPerformanceData(sPerfData& pd) {

	// Start the clock
	vectorStartTime = clock();

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

void cStandardVector::mUpdatePerformanceData(sPerfData& pd) {

	// Update elapsed time
	pd.elapsedCallTime_ms = ((float)(clock() - vectorStartTime) / CLOCKS_PER_SEC) * 1000.0f;

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

void cStandardVector::mEndPerformanceData(sPerfData& pd) {

	// Update elapsed time
	pd.elapsedCallTime_ms = ((float)(clock() - vectorStartTime) / CLOCKS_PER_SEC) * 1000.0f;

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

// ****** Performance functions end ***********//

bool cStandardVector::mIsInsideSphere(sPoint point, float radius, sPerson person, float& distance) {

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

void cStandardVector::mGenerateAllPeople() {

	this->mStartPerformanceData(performanceData);

	// Now generate the sPerson objects
	size_t ID = 0;
	srand(time(NULL));

	// Generate all the males with a random last name
	for (size_t index = 0; index < svMaleNames.size(); ++index) {

		sPerson newPerson;
		newPerson.uniqueID = ID;
		int maleIndex = rand() % svMaleNames.size();
		newPerson.first = svMaleNames[maleIndex];

		int lastIndex = rand() % svLastNames.size();
		newPerson.last = svLastNames[lastIndex];

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
		this->mUpdatePerformanceData(performanceData);

		ID++;
		this->mUpdatePerformanceData(performanceData);
	}

	// Now create all the female names
	for (size_t index = 0; index < svFemaleNames.size(); ++index) {

		sPerson newPerson;
		newPerson.uniqueID = ID;
		int femaleIndex = rand() % svFemaleNames.size();
		newPerson.first = svFemaleNames[femaleIndex];

		int lastIndex = rand() % svLastNames.size();
		newPerson.last = svLastNames[lastIndex];

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
		this->mUpdatePerformanceData(performanceData);

		ID++;
		this->mUpdatePerformanceData(performanceData);
	}

	return;
}

void cStandardVector::mGenerateNumPeople(int numPeople) {

	this->mStartPerformanceData(performanceData);

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
			int femaleIndex = rand() % svFemaleNames.size();
			newPerson.first = svFemaleNames[femaleIndex];

			int lastIndex = rand() % svLastNames.size();
			newPerson.last = svLastNames[lastIndex];

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
			this->mUpdatePerformanceData(performanceData);
		}
		break;
		case 1:
		{
			sPerson newPerson;
			newPerson.uniqueID = ID;
			int maleIndex = rand() % svMaleNames.size();
			newPerson.first = svMaleNames[maleIndex];

			int lastIndex = rand() % svLastNames.size();
			newPerson.last = svLastNames[lastIndex];

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
			this->mUpdatePerformanceData(performanceData);
		}
		default:
			this->mUpdatePerformanceData(performanceData);
			break;
		}

		ID++;
		this->mUpdatePerformanceData(performanceData);
	}

	this->mEndPerformanceData(performanceData);

	return;
}