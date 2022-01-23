#include "MarkingCode.h"
#include <iostream>
#include <algorithm>
#include <iterator>


bool FindMatchesFirst( std::vector<sPerson> vecResults, std::vector<sPerson> vecSearchValues )
{
	std::vector<sPerson> vecMatches;
	for ( std::vector<sPerson>::iterator itSearch = vecSearchValues.begin(); itSearch != vecSearchValues.end(); itSearch++ )
	{
		for ( std::vector<sPerson>::iterator itResults = vecResults.begin(); itResults != vecResults.end(); itResults++ )	
		{
			if ( itResults->first == itSearch->first )
			{	// Found a match
				vecMatches.push_back( *itResults );
				itResults->first = "";	// Clear the first name so it won't be found again
			}
		}
	}
	// At this point, the number of matches should be the same as the results
	if ( vecMatches.size() != vecResults.size() )
	{
		return false;
	}
	return true;
}

bool FindMatchesLast( std::vector<sPerson> vecResults, std::vector<sPerson> vecSearchValues )
{
	std::vector<sPerson> vecMatches;
	for ( std::vector<sPerson>::iterator itSearch = vecSearchValues.begin(); itSearch != vecSearchValues.end(); itSearch++ )
	{
		for ( std::vector<sPerson>::iterator itResults = vecResults.begin(); itResults != vecResults.end(); itResults++ )	
		{
			if ( itResults->last == itSearch->last )
			{	// Found a match
				vecMatches.push_back( *itResults );
				itResults->last = "";	// Clear the first name so it won't be found again
			}
		}
	}
	// At this point, the number of matches should be the same as the results
	if ( vecMatches.size() != vecResults.size() )
	{
		return false;
	}
	return true;
}

bool FindMatchesFirstAndLast( std::vector<sPerson> vecResults, std::vector<sPerson> vecSearchValues )
{
	std::vector<sPerson> vecMatches;
	for ( std::vector<sPerson>::iterator itSearch = vecSearchValues.begin(); itSearch != vecSearchValues.end(); itSearch++ )
	{
		for ( std::vector<sPerson>::iterator itResults = vecResults.begin(); itResults != vecResults.end(); itResults++ )	
		{
			if ( ( itResults->last == itSearch->last ) && ( itResults->first == itResults->first ) ) 
			{	// Found a match
				vecMatches.push_back( *itResults );
				itResults->last = "";	// Clear the first name so it won't be found again
				itResults->first = "";
			}
		}
	}
	// At this point, the number of matches should be the same as the results
	if ( vecMatches.size() != vecResults.size() )
	{
		return false;
	}
	return true;
}

void Test_FindPeopleByNameManyPeople(sMarks &marks)
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

	std::vector<sPerson> vecToFind;
	vecToFind.push_back( pickARandomPerson(vecPeopleAll) );
	vecToFind.push_back( pickARandomPerson(vecPeopleAll) );
	vecToFind.push_back( pickARandomPerson(vecPeopleAll) );
	vecToFind.push_back( pickARandomPerson(vecPeopleAll) );
	vecToFind.push_back( pickARandomPerson(vecPeopleAll) );

	std::vector<sPerson> vecPeopleResults;

	//fisrt only
	std::vector<sPerson> vecToFindFirstOnly;
	// TODO: Was an error
	//std::copy( vecToFind.begin(), vecToFind.end(), std::back_inserter(vecToFindFirstOnly.begin()) );
	for ( std::vector<sPerson>::iterator itPerson = vecToFindFirstOnly.begin(); itPerson != vecToFindFirstOnly.end(); itPerson++ )
	{	
		itPerson->last = "";	
	}
	// TODO: Was an error
	//g_StudentThing.FindPeopleByName( vecPeopleResults, vecToFindFirstOnly, INT_MAX );
	std::cout << "Found " << vecPeopleResults.size() << " people with first name " << std::endl;
	if ( FindMatchesFirst( vecPeopleResults, vecToFindFirstOnly ) )
	{
		marks.findManyPeople.firstNameOnly = 10;
	}
	PrintPerformanceData("FindPeopleByName(), many, last names");

	//last only	
	std::vector<sPerson> vecToFindLastOnly;
	// TODO: Was an error
	//std::copy( vecToFind.begin(), vecToFind.end(), std::back_inserter(vecToFindLastOnly.begin()) );
	for ( std::vector<sPerson>::iterator itPerson = vecToFindLastOnly.begin(); itPerson != vecToFindLastOnly.end(); itPerson++ )
	{	
		itPerson->first = "";	
	}
	// TODO: Was an error
	//g_StudentThing.FindPeopleByName( vecPeopleResults, vecToFindLastOnly, INT_MAX );
	std::cout << "Found " << vecPeopleResults.size() << " people with last name " << std::endl;
	if ( FindMatchesFirst( vecPeopleResults, vecToFindLastOnly ) )
	{
		marks.findManyPeople.lastNameOnly = 10;
	}
	PrintPerformanceData("FindPeopleByName(), many, first names");


	//first and last
	// TODO: Was an error
	//g_StudentThing.FindPeopleByName( vecPeopleResults, vecToFind, INT_MAX );
	std::cout << "Found " << vecPeopleResults.size() << " people with last name " << std::endl;
	if ( FindMatchesFirstAndLast( vecPeopleResults, vecToFind ) )
	{
		marks.findManyPeople.firstAndLastName = 10;
	}
	PrintPerformanceData("FindPeopleByName(), many, first and last names");


	//Deals with maxNumberOfPeople
	sPerson blankPerson;
	// TODO: Was an error
	//g_StudentThing.FindPeopleByName( vecPeopleResults, vecToFind, 10 );
	std::cout << "Asked for 10 people, returned " << vecPeopleResults.size() << " people" << std::endl;
	if ( vecPeopleResults.size() == 10 )
	{
		marks.findManyPeople.specificNumberOfPeople = 10;
	}
	PrintPerformanceData("FindPeopleByName(), many, limit to 10");


	return;
}

