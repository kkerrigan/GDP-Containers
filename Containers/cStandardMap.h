/**
 * @file cStandardMap.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#ifndef _cStandardMap_HG_
#define _cStandardMap_HG_

#include <map>

#include "iPersonMotron.h"

class cStandardMap : public iPersonMotron {
public:
	void generatePeople(int numPeople = INT_MAX);

	// Interface functions
	virtual bool LoadDataFilesIntoContainer(std::string firstNameFemaleFileName, std::string firstNameMaleFileName, std::string lastNameFileName) override;
	virtual bool FindPeopleByName(std::vector<sPerson> &vecPeople, sPerson personToMatch, int maxNumberOfPeople = INT_MAX) override;
	virtual bool FindPeopleByName(std::vector<sPerson> &vecPeople, std::vector<sPerson> &vecPeopleToMatch, int maxNumberOfPeople = INT_MAX) override;
	virtual bool FindPersonByID(sPerson &person, unsigned long long uniqueID) override;
	virtual bool FindPeople(std::vector<sPerson> &vecPeople, sPoint location, float radius,
		int maxPeopleToReturn = INT_MAX) override;

	virtual bool FindPeople(std::vector<sPerson> &vecPeople, float minHealth, float maxHealth,
		int maxPeopleToReturn = INT_MAX) override;

	virtual bool FindPeople(std::vector<sPerson> &vecPeople,
		sPoint location, float radius, float minHealth, float maxHealth,
		int maxPeopleToReturn = INT_MAX) override;

	virtual bool SortPeople(std::vector<sPerson> &vecPeople, eSortType sortType) override;
	virtual bool GetPerformanceFromLastCall(sPerfData &callStats) override;
	virtual eContainerType getContainerType(void) override;

private:
	std::map<size_t /*Person index*/, sPerson> mData;

	void mGenerateAllPeople();
	void mGenerateNumPeople(int numPeople);

	// Sort functions
	void mSortFirstLastNamesAsc();
	void mSortFirstLastNamesDesc();
	void mSortLastFirstNamesAsc();
	void mSortLastFirstNamesDesc();
	void mSortIdAsc();
	void mSortIdDesc();
	void mSortHealthAsc();
	void mSortHealthDesc();

	// Performance data functions
	void mStartPerformanceData(sPerfData& pd);
	void mUpdatePerformanceData(sPerfData& pd);
	void mEndPerformanceData(sPerfData& pd);

	// Distance functions
	bool mIsInsideSphere(sPoint point, float radius, sPerson person, float& distance);
};

#endif // !_cStandardMap_HG_

