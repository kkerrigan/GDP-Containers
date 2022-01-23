/**
 * @file
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#include <vector>
#include <iostream>

#include "MarkingCode.h"

#include "cStandardVector.h"
#include "cStandardList.h"
#include "cStandardMap.h"
#include "cCustomVector.h"
#include "cCustomList.h"
#include "cCustomMap.h"


int main() {

	/*
		Some notes on using this application
			- The iPersonMotron is inherited by each of the includes above, so accessing the functions
			  of the interface are done with the dot operator
			- The LoadDataFilesIntoContainer() function ONLY loads the files into the class so that the 
			  people are ready to be generated.
			- To actually generate the people and store them into the container, I created a function called
			  generatePeople() that must be called after the LoadDataFilesIntoContainer
			- It has two ways to be used, which I will demonstrate below
			- The marking code was added to this application; however:
				- Any spot that was using #include "cExample_StudentFile.h", extern cExample g_StudentThing, or
				  the std::back_inserter was commented out with a //TODO: was an error so it could be easily found
				  because they were causing compile errors.
	*/
	
	cStandardVector standardVec;
	standardVec.LoadDataFilesIntoContainer("dist.female.first.txt", "dist.male.first.txt", "US_LastNames.txt");

	// Calling the generatePeople() without passing a number will generate all male names with a random
	// Last name and all female names with a random last name
	standardVec.generatePeople();

	std::vector<sPerson> vecPeople;
	standardVec.SortPeople(vecPeople, iPersonMotron::eSortType::ASC_FIRST_THEN_LAST);

	/*for (size_t i = 0; i < vecPeople.size(); ++i) {

		std::cout << vecPeople[i].first << " " << vecPeople[i].last << " " << vecPeople[i].age
			<< " " << vecPeople[i].health << std::endl;
	}*/

	// Calling the generatePeople() and passing an integer to the function will generate that number
	// of people with the male, female, and last name randomized
	cCustomMap customMap;
	customMap.LoadDataFilesIntoContainer("dist.female.first.txt", "dist.male.first.txt", "US_LastNames.txt");

	customMap.generatePeople(25);

	std::vector<sPerson> vecPeopleMap;
	customMap.SortPeople(vecPeopleMap, iPersonMotron::eSortType::DESC_BY_HEALTH);

	for (size_t i = 0; i < vecPeopleMap.size(); ++i) {

		std::cout << vecPeopleMap[i].first << " " << vecPeopleMap[i].last << " " << vecPeopleMap[i].age
			<< " " << vecPeopleMap[i].health << std::endl;
	}
	system("pause");

	return 0;
}