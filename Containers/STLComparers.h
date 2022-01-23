/**
 * @file STLComparers.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * These are compare functions that can be used by the std sort function.
 */

#ifndef _STLComparers_HG
#define _STLComparers_HG_

#include "sPerson.h"

inline bool sortFirstLastNamesAsc(const sPerson& p1, const sPerson& p2) {

	if (p1.first == p2.first) {

		return p1.last < p2.last;
	}
	else {

		return p1.first < p2.first;
	}
}

inline bool sortFirstLastNamesDesc(const sPerson& p1, const sPerson& p2) {

	if (p1.first == p2.first) {

		return p1.last > p2.last;
	}
	else {

		return p1.first > p2.first;
	}
}

inline bool sortLastFristNamesAsc(const sPerson& p1, const sPerson& p2) {

	if (p1.last == p2.last) {

		return p1.first < p2.first;
	}
	else {

		return p1.last < p2.last;
	}
}

inline bool sortLastFirstNamesDesc(const sPerson& p1, const sPerson& p2) {

	if (p1.last == p2.last) {

		return p1.first > p2.first;
	}
	else {

		return p1.last > p2.last;
	}
}

inline bool sortIdAsc(const sPerson& p1, const sPerson& p2) {

	return p1.uniqueID < p2.uniqueID;
}

inline bool sortIdDesc(const sPerson& p1, const sPerson& p2) {

	return p1.uniqueID > p2.uniqueID;
}

inline bool sortHealthAsc(const sPerson& p1, const sPerson& p2) {

	return p1.health < p2.health;
}

inline bool sortHealthDesc(const sPerson& p1, const sPerson& p2) {

	return p1.health > p2.health;
}

#endif // !_STLComparers_HG

