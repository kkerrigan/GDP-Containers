/**
 * @file cCustomList.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Enter a short description.
 */

#ifndef _cCustomList_HG_
#define _cCustomList_HG_

#include "iPersonMotron.h"

class cCustomList : public iPersonMotron {
public:
	// Constructors
	cCustomList();
	~cCustomList();

	// List functions
	void push_back(sPerson& person); // Put values at the end of the list
	void resize(size_t newSize);
	size_t size();
	sPerson& getObject(); // Get the head position of the list
	void insert(sPerson& person); // Insert at current head position
	
	void erase(); // delete object at current head position
	void clear();

	void setFirst(); // Set the head to the first node
	void setLast(); // Set the head to the last node
	void moveForward(); // Move head forwards
	void moveBackward(); // Move head backward
	bool isAtLastNode(); // Check if current node is the last node

	void generatePeople(int numPeople = INT_MAX);

	// Interface functions
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
	// List node
	struct sNode {
		sNode() : prev(nullptr), next(nullptr) {}
		sPerson data;
		sNode* prev;
		sNode* next;
	};

	sNode* mHead;
	sNode* mFirstNode;
	sNode* mLastNode;
	size_t mSize;

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

#endif // !_cCustomList_HG_

