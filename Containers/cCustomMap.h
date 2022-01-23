/**
 * @file cCustomMap.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Creation of a map using a size_t as the key and a sPerson object as the value.
 */

#ifndef _cCustomMap_HG_
#define _cCustomMap_HG_

#include "cDIYMap.h"
#include "iPersonMotron.h"


class cCustomMap : public iPersonMotron {
public:

	void generatePeople(int numPeople = INT_MAX);

	// Interface Functions
	virtual bool LoadDataFilesIntoContainer(std::string firstNameFemaleFileName, std::string firstNameMaleFileName, std::string lastNameFileName) override;
	virtual bool FindPeopleByName(std::vector<sPerson> &vecPeople, sPerson personToMatch, int maxNumberOfPeople = INT_MAX) override;
	virtual bool FindPeopleByName(std::vector<sPerson> &vecPeople, std::vector<sPerson> &vecPeopleToMatch, int maxNumberOfPeople = INT_MAX) override;
	virtual bool FindPersonByID(sPerson &person, unsigned long long uniqueID) override;
	virtual bool FindPeople(std::vector<sPerson> &vecPeople, sPoint location, float radius, int maxPeopleToReturn = INT_MAX) override;
	virtual bool FindPeople(std::vector<sPerson> &vecPeople, float minHealth, float maxHealth, int maxPeopleToReturn = INT_MAX) override;
	virtual bool FindPeople(std::vector<sPerson> &vecPeople, sPoint location, float radius, float minHealth, float maxHealth, int maxPeopleToReturn = INT_MAX) override;
	virtual bool SortPeople(std::vector<sPerson> &vecPeople, eSortType sortType) override;
	virtual bool GetPerformanceFromLastCall(sPerfData &callStats) override;
	virtual eContainerType getContainerType(void) override;

private:
	cDIYMap<size_t, sPerson> mData;

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

	// Performance functions
	void mStartPerformanceData(sPerfData& pd);
	void mUpdatePerformanceData(sPerfData& pd);
	void mEndPerformanceData(sPerfData& pd);

	// Distance functions
	bool mIsInsideSphere(sPoint point, float radius, sPerson person, float& distance);
};

#endif // !_cCustomMap_HG_
