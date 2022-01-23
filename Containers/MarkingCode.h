#ifndef _MarkingCodeHeader_HG_
#define _MarkingCodeHeader_HG_

#include "iPersonMotron.h"
#include <string>	// memset

// This is the class the students wrote
// TODO: Was an error
//#include "cExample_StudentFile.h"
//extern cExample g_StudentThing;

class sMarks
{
public:
	sMarks() 
	{ memset( this, 0, sizeof(sMarks) ); }
	struct sFindPeopleByName
	{
		int firstNameOnly;
		int lastNameOnly;
		int firstAndLastName;
		int everyone;
		int specificNumberOfPeople;
	};

	int LoadDataIntoContainer;

	sFindPeopleByName findSinglePerson;
	sFindPeopleByName findManyPeople;

	int FindPersonByID;
	int FindPeopleLoacationRadius;
	int FindPeopleHealth;
	int FindPeopleLocationHealth;

	int SortPeople_ASC_FirstThenLast;
	int SortPeople_DESC_FirstThenLast;
	int SortPeople_ASC_LastThenFirst;
	int SortPeople_DESC_LastThenFirst;
	int SortPeople_ASC_byID;
	int SortPeople_DESC_byID;
	int SortPeople_ASC_ByHealth;
	int SortPeople_DESC_ByHealth;
	void PrintOutMarkSummary(std::string title);
};

void PrintOutPeople( std::vector<sPerson> &vecPeople );

// From: http://stackoverflow.com/questions/7693751/how-can-i-improve-formatting-number-with-commas-performance
template <class T>
std::string FormatNumberWithCommas(T value, int numberOfDecimalPlaces = 1);

sPerson pickARandomPerson( std::vector<sPerson> &vecPeople);

void PrintPerformanceData( const std::string &title );

// returns a mark (out of 10)
void Test_FindPeopleByNameSinglePerson(sMarks &marks);
void Test_FindPeopleByNameManyPeople(sMarks &marks);
void Test_FindPersonByName(sMarks &marks);
void Test_FindPersonByID(sMarks &marks);
void Test_FindPeopleByLocationAndRadius(sMarks &marks);
void Test_FindPeopleByHealth(sMarks &marks);
void Test_FindPeopleLocationRadiusHealth(sMarks &marks);
void Test_SortPeople(sMarks &marks);




#endif
