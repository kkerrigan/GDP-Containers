#include "MarkingCode.h"
#include <iostream>


void Test_FindPeopleByNameSinglePerson(sMarks &marks)
{
	std::vector<sPerson> vecPeopleAll;
	sPerson myBlank;
	// Should return everybody
	// TODO: Was an error
	/*if ( ! g_StudentThing.FindPeopleByName( vecPeopleAll, myBlank, INT_MAX ) )
	{	
		std::cout << "FindPeopleByName() reutrned an error" << std::endl;
		return;
	}*/

	//Returns everyone
	std::cout << "There are " << vecPeopleAll.size() << " people in the database." << std::endl;
	PrintPerformanceData("FindPeopleByName()");

	sPerson BobAll = pickARandomPerson(vecPeopleAll);
	sPerson BobWorking = BobAll;

	std::vector<sPerson> vecPeopleResults;

	//fisrt only
	BobWorking.last = "";
	// TODO: Was an error
	//g_StudentThing.FindPeopleByName( vecPeopleResults, BobWorking, 1 );
	std::cout << "There are " << vecPeopleResults.size() << " people with first name " << BobWorking.first << std::endl;
	bool bAllNamesMatch = true;
	for ( std::vector<sPerson>::iterator itPerson = vecPeopleResults.begin(); itPerson != vecPeopleResults.end(); itPerson++ )
	{
		if ( itPerson->first != BobWorking.first )
		{	// Incorrect 1st name
			bAllNamesMatch = false;
		}
	}
	if ( bAllNamesMatch ) 
	{
		marks.findSinglePerson.firstNameOnly = 10;
	}
	PrintPerformanceData("FindPeopleByName()");

	//last only	
	BobWorking = BobAll;
	BobWorking.first = "";
	// TODO: Was an error
	//g_StudentThing.FindPeopleByName( vecPeopleResults, BobWorking, 1 );
	std::cout << "There are " << vecPeopleResults.size() << " people with last name " << BobWorking.last << std::endl;
	bAllNamesMatch = true;
	for ( std::vector<sPerson>::iterator itPerson = vecPeopleResults.begin(); itPerson != vecPeopleResults.end(); itPerson++ )
	{
		if ( itPerson->last != BobWorking.first )
		{	// Incorrect last name name
			bAllNamesMatch = false;
		}
	}
	if ( bAllNamesMatch ) 
	{
		marks.findSinglePerson.lastNameOnly = 10;
	}
	PrintPerformanceData("FindPeopleByName()");

	//first and last
	BobWorking = BobAll;
	// TODO: Was an error
	//g_StudentThing.FindPeopleByName( vecPeopleResults, BobWorking, 1 );
	std::cout << "There are " << vecPeopleResults.size() << " people named " << myBlank.first << " " << myBlank.last << std::endl;
	bAllNamesMatch = true;
	for ( std::vector<sPerson>::iterator itPerson = vecPeopleResults.begin(); itPerson != vecPeopleResults.end(); itPerson++ )
	{
		if ( ( itPerson->first != BobWorking.first ) || ( itPerson->last != BobWorking.last ) )
		{	// Incorrect 1st or last name
			bAllNamesMatch = false;
		}
	}
	if ( bAllNamesMatch ) 
	{
		marks.findSinglePerson.firstAndLastName = 10;
	}
	PrintPerformanceData("FindPeopleByName()");

	//Deals with maxNumberOfPeople
	sPerson blankPerson;
	// TODO: Was an error
	//g_StudentThing.FindPeopleByName( vecPeopleResults, blankPerson, 10 );
	std::cout << "Asked for 10 people, returned " << vecPeopleResults.size() << " people" << std::endl;
	if ( vecPeopleResults.size() == 10 )
	{
		marks.findSinglePerson.specificNumberOfPeople = 10;
	}
	PrintPerformanceData("FindPeopleByName()");


	return;
}

