#include "MarkingCode.h"
#include <iostream>
#include <sstream>

void PrintOutPeople( std::vector<sPerson> &vecPeople )
{
	unsigned int numPeople = static_cast<unsigned int>( vecPeople.size() );
	for ( unsigned int index = 0; index != numPeople; index++ ) 
	{
		std::cout << index << ":" << vecPeople[index].first << " " << vecPeople[index].last;
		std::cout << " ID:" << vecPeople[index].uniqueID;
		std::cout << " age:" << vecPeople[index].age;
		std::cout << " health:" << vecPeople[index].health;
		std::cout << " location:" << vecPeople[index].location.x << ", ";
		std::cout << vecPeople[index].location.y << ", ";
		std::cout << vecPeople[index].location.z << std::endl;
	}
	return;
}

// From: http://stackoverflow.com/questions/7693751/how-can-i-improve-formatting-number-with-commas-performance
template <class T>
std::string FormatNumberWithCommas(T value, int numberOfDecimalPlaces)
{
    std::stringstream ss;
    ss.imbue(std::locale(""));
    ss.precision(numberOfDecimalPlaces);
    ss << std::fixed << value;
    return ss.str();
}

sPerson pickARandomPerson( std::vector<sPerson> &vecPeople)
{
	int index = rand() % vecPeople.size();

	return vecPeople[index];
}

void PrintPerformanceData( const std::string &title )
{
	sPerfData callStats;
	// TODO: Was an error
	//g_StudentThing.GetPerformanceFromLastCall(callStats);
	std::cout << "***************************************************" << std::endl;
	std::cout << "Performance data for " << title << ":" << std::endl;
	std::cout << "\t Elapsed Time: " << FormatNumberWithCommas<float>(callStats.elapsedCallTime_ms, 3) << " ms" << std::endl;
	std::cout << "\t Memory use, min: " << FormatNumberWithCommas<float>(callStats.memoryUsageBytes_min, 0) << " bytes" << std::endl;
	std::cout << "\t Memory use, avg: " << FormatNumberWithCommas<float>(callStats.memoryUsageBytes_avg, 0) << " bytes" << std::endl;;
	std::cout << "\t Memory use, max: " << FormatNumberWithCommas<float>(callStats.memoryUsageBytes_max, 0) << " bytes" << std::endl;;
	std::cout << "***************************************************" << std::endl;
	return;
}


void sMarks::PrintOutMarkSummary(std::string title)
{
	std::cout << "*******************************************************" << std::endl;
	std::cout << "Marks for " << title << std::endl;
	std::cout << std::endl;
	std::cout << "People loaded into the database: " << this->LoadDataIntoContainer << std::endl;
	std::cout << std::endl;
	std::cout << "FindPeopleByName(), SINGLE person requested, first only: " << this->findSinglePerson.firstNameOnly << std::endl;
	std::cout << "FindPeopleByName(), SINGLE person requested, last only: " << this->findSinglePerson.lastNameOnly << std::endl;
	std::cout << "FindPeopleByName(), SINGLE person requested, first and last: " << this->findSinglePerson.firstAndLastName << std::endl;
	std::cout << "FindPeopleByName(), SINGLE person requested, everyone: " << this->findSinglePerson.everyone << std::endl;
	std::cout << "FindPeopleByName(), SINGLE person requested, maxNumberOfPeople: " << this->findSinglePerson.specificNumberOfPeople << std::endl;
	std::cout << std::endl;
	std::cout << "FindPeopleByName(), MULTIPLE person requested, first only: " << this->findManyPeople.firstNameOnly << std::endl;
	std::cout << "FindPeopleByName(), MULTIPLE person requested, last only: " << this->findManyPeople.lastNameOnly << std::endl;
	std::cout << "FindPeopleByName(), MULTIPLE person requested, first and last: " << this->findManyPeople.firstAndLastName << std::endl;
	std::cout << "FindPeopleByName(), MULTIPLE person requested, everyone: " << this->findManyPeople.everyone << std::endl;
	std::cout << "FindPeopleByName(), MULTIPLE person requested, maxNumberOfPeople: " << this->findManyPeople.specificNumberOfPeople << std::endl;
	std::cout << std::endl;
	std::cout << "FindPersonByID() :" << this->FindPersonByID << std::endl;
	std::cout << "FindPeople(location, radius) :" << this->FindPeopleLoacationRadius << std::endl;
	std::cout << "FindPeople(minHealth, maxHealth) :" << this->FindPeopleHealth << std::endl;
	std::cout << "FindPeople(location, radius, minHealth, maxHealth) :" << this->FindPeopleLocationHealth << std::endl;
	std::cout << std::endl;
	std::cout << "SortPeople(ASC_FIRST_THEN_LAST) :" << this->SortPeople_ASC_FirstThenLast << std::endl;
	std::cout << "SortPeople(DESC_FIRST_THEN_LAST) :" << this->SortPeople_DESC_FirstThenLast << std::endl;
	std::cout << "SortPeople(ASC_LAST_THEN_FIRST) :" << this->SortPeople_ASC_LastThenFirst << std::endl;
	std::cout << "SortPeople(DESC_LAST_THEN_FIRST) :" << this->SortPeople_DESC_LastThenFirst << std::endl;
	std::cout << "SortPeople(ASC_BY_ID) :" << this->SortPeople_ASC_byID << std::endl;
	std::cout << "SortPeople(DESC_BY_ID) :" << this->SortPeople_DESC_byID << std::endl;
	std::cout << "SortPeople(ASC_BY_HEALTH) :" << this->SortPeople_ASC_ByHealth << std::endl;
	std::cout << "SortPeople(DESC_BY_HEALTH) :" << this->SortPeople_DESC_ByHealth << std::endl;
	std::cout << std::endl;
	std::cout << "*******************************************************" << std::endl;
	return;
}

