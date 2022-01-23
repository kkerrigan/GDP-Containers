#ifndef _cCustomVector_HG_
#define _cCustomVector_HG_

#include "iPersonMotron.h"

class cCustomVector : public iPersonMotron {
public:
	// Constructors
	cCustomVector();
	~cCustomVector();

	// Container functions
	void push_back(const sPerson& person);
	void resize(size_t size);
	size_t size() const;
	size_t capacity() const;
	void clear();

	// Container operators
	sPerson& operator[](size_t index);
	const sPerson& operator[](size_t index) const;

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
	sPerson* mData;
	static unsigned int mINIT_CAPACITY;
	size_t mCapacity;
	size_t mSize;
	float mPersonDistance;

	void mGenerateAllPeople();
	void mGenerateNumPeople(int numPeople);

	// Sort functions
	void mVectorQuickSort(long long left, long long right, eSortType sortType);
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

#endif // !_cCustomVector_HG_

