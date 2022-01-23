/**
 * @file cDIYMap.inl
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 *
 * @section DESCRIPTION
 *
 * Inline file to declare templated functions.
 */

#include "cDIYMap.h"
#include "BST.h"

template<class TK, class TO>
cDIYMap<TK, TO>::cDIYMap() {

	this->mSize = 0;
	this->mRoot = nullptr;
	this->mRefIndex = 0;
}

template<class TK, class TO>
cDIYMap<TK, TO>::~cDIYMap() {

	this->mDeleteNodesRecursive(this->mRoot);

	return;
}

template<class TK, class TO>
void cDIYMap<TK, TO>::insert(TK& key, TO& object) {

	this->mIndicesChanged = true; // Flag the insertion (To be used in populate indices)

	bool bEqualKey = false; // Controls the size
	this->mRoot = insertRec(this->mRoot, object, key, bEqualKey);

	if (!bEqualKey) {

		this->mSize++;
	}

	return;
}

template<class TK, class TO>
bool cDIYMap<TK, TO>::find(TK& key, TO& object) {

	if (this->mSize == 0) {

		return false; // No element at all
	}
	else {

		this->mFindRecursive(object, this->mRoot, key);
	}

	return true;
}

template<class TK, class TO>
bool cDIYMap<TK, TO>::getObjectAt(TO& object, size_t index) {

	if (this->mRoot == nullptr) return false;

	this->mPopulateIndices();

	return this->mGetObjectAtRecursive(this->mRoot, object, index);
}

template<class TK, class TO>
size_t cDIYMap<TK, TO>::size() const {

	return this->mSize;
}

template<class TK, class TO>
void cDIYMap<TK, TO>::clear() {

	this->mDeleteNodesRecursive(this->mRoot);
	this->mRoot = nullptr;
	this->mSize = 0;

	return;
}

template<class TK, class TO>
bool cDIYMap<TK, TO>::mGetObjectAtRecursive(sNode<TK, TO>* node, TO& object, size_t index) {

	if (node == nullptr) return false;

	if (node->index == index) {

		object = node->object;
		return true;
	}
	else if (index < node->index) {

		return this->mGetObjectAtRecursive(node->left, object, index);
	}
	else {

		return this->mGetObjectAtRecursive(node->right, object, index);
	}
}

template<class TK, class TO>
bool cDIYMap<TK, TO>::mFindRecursive(TO& object, sNode<TK, TO>* node, TK key) {

	if (node == nullptr) return false;

	if (node->key == key) {

		object = node->object;
		return true;
	}
	else if (key < node->key) {

		return this->mFindRecursive(object, node->left, key);
	}
	else {

		return this->mFindRecursive(object, node->right, key);
	}
}

template<class TK, class TO>
void cDIYMap<TK, TO>::mPopulateIndices() {

	if (!this->mIndicesChanged) {

		// Indices were untouched
		return;
	}
	else {

		this->mRefIndex = 0;
		this->mPopulateIndicesRecursive(this->mRoot);
	}

	return;
}

template<class TK, class TO>
void cDIYMap<TK, TO>::mPopulateIndicesRecursive(sNode<TK, TO>* node) {

	if (node == nullptr) return;

	this->mPopulateIndicesRecursive(node->left);
	node->index = this->mRefIndex;
	this->mRefIndex++;
	this->mPopulateIndicesRecursive(node->right);

	// Finished
	this->mIndicesChanged = true;

	return;
}

template<class TK, class TO>
void cDIYMap<TK, TO>::mDeleteNodesRecursive(sNode<TK, TO>* rootNode) {

	if (rootNode == nullptr) return;

	this->mDeleteNodesRecursive(rootNode->left);
	this->mDeleteNodesRecursive(rootNode->right);

	delete rootNode;

	return;
}