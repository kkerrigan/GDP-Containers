/**
 * @file BST.h
 * @author  Kristian Kerrigan <k_kerrigan3@fanshaweonline.ca>
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * Creation of a Binary Search Tree (BST) to be used in 
 * my custom map.
 * Inspired by this link: https://www.geeksforgeeks.org/avl-tree-set-1-insertion/
 */

#ifndef _BST_HG_
#define _BST_HG_

#include <stdio.h>
#include <stdlib.h>

#include "cDIYMap.h"

template<class TK, class TO>
size_t height(sNode<TK, TO>* node) {

	if (node == nullptr) return 0;

	return node->height;
}

template<class T> // HACK: To avoid compling the function repeadly
size_t Max(int a, int b, T) {

	return (a > b) ? a : b;
}

template<class TK, class TO>
sNode<TK, TO>* newNode(TK& key, TO& object) {

	sNode<TK, TO>* node = new sNode<TK, TO>;
	node->key = key;
	node->object = object;
	node->left = nullptr;
	node->right = nullptr;
	node->height = 1; // All new nodes start at the leaf

	return node;
}

template<class TK, class TO>
sNode<TK, TO>* rightRotate(sNode<TK, TO>* y) {

	sNode<TK, TO>* x = y->left;
	sNode<TK, TO>* T2 = x->right;

	// Preform rotation
	x->right = y;
	y->left = T2;

	// Update the height
	y->height = Max(height(y->left), height(y->right), 0) + 1;
	x->height = Max(height(x->left), height(x->right), 0) + 1;

	return x;
}

template<class TK, class TO>
sNode<TK, TO>* leftRotate(sNode<TK, TO>* x) {

	sNode<TK, TO>* y = x->right;
	sNode<TK, TO>* T2 = y->left;

	// Perform rotation
	y->left = x;
	x->right = T2;

	// Update height
	x->height = Max(height(x->left), height(x->right), 0) + 1;
	y->height = Max(height(y->left), height(y->right), 0) + 1;

	return y;
}

template<class TK, class TO>
size_t getBalance(sNode<TK, TO>* node) {

	if (node == nullptr) return 0;

	return height(node->left) - height(node->right);
}

template<class TK, class TO>
sNode<TK, TO>* insertRec(sNode<TK, TO>* node, TO& object, TK& key, bool& equalKey) {

	// Perform the normal BST insertion
	if (node == nullptr) {
		
		sNode<TK, TO>* node = newNode<TK, TO>(key, object);
		return node;
	}

	if (key < node->key) {

		node->left = insertRec(node->left, object, key, equalKey);
	}
	else if (key > node->key) {

		node->right = insertRec(node->right, object, key, equalKey);
	}
	else {

		equalKey = true;
		return node;
	}

	// Update the height of the ancestor node
	node->height = 1 + Max(height(node->left), height(node->right), 0);

	// Get the balance of the ancestor node to check
	// whether this node is now unbalanced
	int balance = getBalance(node);

	// If the node becomes unbalances, there are 4 options

	// Left-Left case
	if (balance > 1 && key < node->left->key) {

		return rightRotate(node);
	}

	// Right-Right case
	if (balance < -1 && key > node->right->key) {

		return leftRotate(node);
	}

	// Left-Right case
	if (balance > 1 && key > node->left->key) {

		node->left = leftRotate(node->left);
		return rightRotate(node);
	}

	// Right-Left case
	if (balance < -1 && key < node->right->key) {

		node->right = rightRotate(node->right);
		return leftRotate(node);
	}

	// return the (unchanged) node
	return node;
}

#endif // !_BST_HG_

