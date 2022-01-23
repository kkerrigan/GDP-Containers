/**
 * @file cDIYMap.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * The actual container that stores the data for cCustomMap.
 */

#ifndef _cDIYMap_HG_
#define _cDIYMap_HG_

template <class TK, class TO>
struct sNode {
	sNode() :
		parent(nullptr),
		left(nullptr),
		right(nullptr),
		height(0),
		bIsIndex(false)
	{}

	TK key;
	TO object;

	sNode* parent; // Used to reference by index
	sNode* left;
	sNode* right;

	size_t index; // Used to reference by index
	bool bIsIndex; // Used to reference by index
	size_t height;
};

template<class TK, class TO>
class cDIYMap {
public:
	// Constructors
	cDIYMap();
	~cDIYMap();

	void insert(TK& key, TO& object);
	bool find(TK& key, TO& object);
	bool getObjectAt(TO& object, size_t index);

	size_t size() const;
	void clear();

private:
	sNode<TK, TO>* mRoot;
	size_t mSize;
	size_t mRefIndex;
	bool mIndicesChanged;

	bool mGetObjectAtRecursive(sNode<TK, TO>* node, TO& object, size_t index);
	bool mFindRecursive(TO& object, sNode<TK, TO>* node, TK key);
	void mPopulateIndices();
	void mPopulateIndicesRecursive(sNode<TK, TO>* node);
	void mDeleteNodesRecursive(sNode<TK, TO>* rootNode);
};

#include "cDIYMap.inl"


#endif // !_cDIYMap_HG_

