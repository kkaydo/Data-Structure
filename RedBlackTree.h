#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <stack>
using namespace std;

// implement and test a Red-black tree template class to implement an ordered map.

template<typename K, typename V>
class NodeT {
public:
	V data;
	K key;
	bool isBlack;
	NodeT* left;
	NodeT* right;
	NodeT* parent;
	NodeT(K key, V value) : key(key), data(value), isBlack(false), left(nullptr), right(nullptr), parent(nullptr) {};
	
};

template<typename K, typename V>
class RedBlackTree {
public:
	// Constructors
	RedBlackTree();
	~RedBlackTree();
	RedBlackTree(const RedBlackTree& RBT);
	RedBlackTree& operator=(const RedBlackTree& rbt);
	
	// Mutators
	bool insert(K key, V value);
	bool remove(K key);
	bool search(K key);
	vector<K> search(K k1, K k2);

	
	// Accessors
	vector<V> values();
	vector<K> keys();
	int size();
	NodeT<K,V>* getRoot();
	
	
private:
	NodeT<K, V>* root;
	map<K, V> key_value;
	NodeT<K, V>* predecessor(NodeT<K, V>* x);
	void leftRotation(NodeT<K, V>*& x);
	void rightRotation(NodeT<K, V>*& x);
	void fixInsert(NodeT<K, V>*& x);
	void fixRemove(NodeT<K, V>*& x, NodeT<K, V>*& x_parent);
	NodeT<K, V>* findNode(K key);
	NodeT<K, V>* copy(NodeT<K, V>* nd);
	void clear(NodeT<K, V>* nd);

};

template<typename K, typename V>
RedBlackTree<typename K, typename V>::RedBlackTree() {
	root = nullptr;
}

template<typename K, typename V>
RedBlackTree<typename K, typename V>::~RedBlackTree() {
	clear(root);
	key_value.clear();
	root = nullptr;
}

// Copy constructor. creates a deep copy 
template<typename K, typename V>
RedBlackTree<typename K, typename V>::RedBlackTree(const RedBlackTree& rbt) {
	root=copy(rbt.root);

}

// Overloaded assignment operator
template<typename K, typename V>
RedBlackTree<typename K, typename V>& RedBlackTree<typename K, typename V>::operator = (const RedBlackTree& rbt) {
	if (root == rbt.root) {
		return *this;
	}
	root=copy(rbt.root);

	return *this;
}

//if the tree does not contain the method's first parameter which represents the key, 
//inserts the key and value (the second parameter) and returns true; 
//otherwise  returns false without insertion
template<typename K, typename V>
bool RedBlackTree<typename K, typename V>::insert(K key, V value) {
	NodeT<K, V>* newNode = new NodeT<K, V>(key,value);
	NodeT<K, V>* parentOfnew = root;
	NodeT<K, V>* next = root;

	//return false when the key is already exist
	if (search(key)) {
		return false;
	}
	

	key_value.insert(pair<K, V>(key, value));
	

	if (root==nullptr) { //root is null
		root = newNode;
		root->isBlack = true;
	}
	else {
		// Find parent of new node
		while (next != nullptr) {
			parentOfnew = next;
			if (key < parentOfnew->key) {
				next = parentOfnew->left;
			}
			else {
				next = parentOfnew->right;
			}
		}
		// Insert new node
		newNode->parent = parentOfnew;
		if (newNode->key > parentOfnew->key) {
			parentOfnew->right = newNode;
		}
		else {
			parentOfnew->left = newNode;
		}
		//fixing the color
		fixInsert(newNode);

	}

	return true;
}

//removes the key and associated value from the tree where the key matches the method's 
//single parameter and returns true; if the tree does not contain the a key matching 
//the parameter returns false
template<typename K, typename V>
bool RedBlackTree<typename K, typename V>::remove(K key) {
	
	//return false when the key does not exist
	if (search(key)==false) {
		return false;
	}

	//find the node to be deleted
	NodeT<K, V>* nd = findNode(key);
	NodeT<K, V>* y;
	NodeT<K, V>* x;
	NodeT<K, V>* x_parent;

	//when nd is the only node in the tree
	if ((nd == root)&&(!nd->left)&&(!nd->right)) {
		key_value.erase(key);
		delete nd;
		root = nullptr;
		return true;
	}
	
	//nd has one or no children
	if (nd->left == nullptr || nd->right == nullptr) {
		y = nd;

	}
	// nd has two children
	else {
		y = predecessor(nd);

	}

	//detach and attach
	if (y->left != nullptr) {
		x = y->left;

	}
	else{
		x = y->right;

	}
	if (x != nullptr) {
		x->parent = y->parent;
	}
	else {
		x_parent = y->parent;
	}

	//when y is the root
	if (y->parent == nullptr) {
		if (x->key != NULL) {
			root = x;
		}
		else {
			root = nullptr;
		}
	}
	else {
		//y is left child
		if (y == y->parent->left) {
			y->parent->left = x;
		}
		//y is right child
		else if (y == y->parent->right) {
			y->parent->right = x;
		}
	}
	
	if (y != nd) {
		nd->key = y->key;
		nd->data = y->data;
	}
	key_value.erase(key);

	//fix colour violation
	if (y->isBlack == true) {
		fixRemove(x,x_parent);
	}
	delete y;
	return true;
}

//searches the tree for the key that matches the method's single parameter and 
//returns true if it is found and false otherwise
template<typename K, typename V>
bool RedBlackTree<typename K, typename V>::search(K key) {
	if (key_value.count(key) > 0) {
		return true;
	}
	else {
		return false;
	}
}


//returns a vector that contains all of the values whose keys are between the method's first and 
//second parameters (including both parameter keys if they are in the tree); the contents of the 
//returned vector are in ascending order of the keys, not the values; 
//if there are no matching keys the returned vector should be empty
template<typename K, typename V>
vector<K> RedBlackTree<typename K, typename V>::search(K k1, K k2) {
	vector<K> include_keys;
	vector<K> cur_keys = keys();
	int size = cur_keys.size();
	int i = 0;
	//empty
	if (k1 > cur_keys[size - 1] || k2 < cur_keys[0]) {

		return include_keys;
	}
	else {//finding the minimum value of key between k1 and k2
		while (cur_keys[i] < k1) {
			i++;
		}
		while (cur_keys[i] <= k2 && i <= (size-1) ) {
			include_keys.push_back(cur_keys[i]);
			i++;
			if (i >= size) {
				return include_keys;
			}
		}
	}

	return include_keys;
}


//returns a vector that contains all of the values in the tree; the contents of the vector are in 
//ascending key - not value - order; if the tree is empty the returned vector should also be empty
template<typename K, typename V>
vector<K> RedBlackTree<typename K, typename V>::keys() {
	vector<K> keys;
	stack<NodeT<K, V>*> st;
	NodeT<K, V>* p = root;
	while (p != nullptr || st.empty() == false) {
		if (p != nullptr) {
			st.push(p);
			p = p->left;
		}
		else {
			p = st.top();
			st.pop();
			keys.push_back(p->key);
			p = p->right;
		}
	}
	return keys;
}

//returns a vector that contains all of the keys in the tree; the contents of the vector are in ascending order;
//if the tree is empty the returned vector should also be empty
template<typename K, typename V>
vector<V> RedBlackTree<typename K, typename V>::values() {
	vector<V> keyValue;
	vector<K> key = keys();
	for (int i = 0;i < key.size();i++) {
		keyValue.push_back(key_value[key[i]]);
	}
	return keyValue;
}

//returns the number of items stored in the tree
template<typename K, typename V>
int RedBlackTree<typename K, typename V>::size() {
	
	return key_value.size();
}


template<typename K, typename V>
NodeT<K, V>* RedBlackTree<typename K, typename V>::getRoot() {
	
	NodeT<K, V>* p = root;
	return p;

}






//helper methods:

template<typename K, typename V>
void RedBlackTree<typename K, typename V>::leftRotation(NodeT<K, V>* &x) {
	NodeT<K, V>* y = x->right;
	x->right = y->left;
	if (y->left != nullptr) {
		y->left->parent = x;
	}
	y->parent = x->parent;

	if (x->parent == nullptr) {  // x is root
		root = y;
	}
	else if (x == x->parent->left) { //left child
		x->parent->left = y;
	}
	else {
		x->parent->right = y;
	}
	y->left = x;
	x->parent = y;
}

template<typename K, typename V>
void RedBlackTree<typename K, typename V>::rightRotation(NodeT<K, V>*& x) {
	NodeT<K, V>* y = x->left;
	x->left = y->right;
	if (y->right != nullptr) {
		y->right->parent = x;
	}
	y->parent = x->parent;

	if (x->parent == nullptr) {  // x is root
		root = y;
	}
	else if (x == x->parent->left) { 
		x->parent->left = y;
	}
	else {
		x->parent->right = y;
	}
	y->right = x;
	x->parent = y;
}

template<typename K, typename V>
void RedBlackTree<typename K, typename V>::fixInsert(NodeT<K, V>*& x) {
	NodeT<K, V>* x_parent = nullptr;
	NodeT<K, V>* x_grandparent = nullptr;

	//x is not the root. Both x and x parent are red
	while (x != root && x->isBlack==false && x->parent->isBlack == false) {
		x_parent = x->parent;
		x_grandparent = x_parent->parent;

		//x parent is left child
		if (x_parent == x_grandparent->left) {
			NodeT<K, V>* x_uncle = x_grandparent->right;

			//x uncle is red
			if (x_uncle != nullptr && x_uncle->isBlack == false) {
				x_grandparent->isBlack = false;
				x_parent->isBlack = true;
				x_uncle->isBlack = true;
				x = x_grandparent;
			}
			else {
				//x is right child(left rotation to make it left child case)
				if (x == x_parent->right) {
					leftRotation(x_parent);
					x = x_parent;
					x_parent = x->parent;
				}
				//x is left child
				rightRotation(x_grandparent);
				x_parent->isBlack = true;
				x_grandparent->isBlack = false;
				x = x_parent;
			}
		}
		//x parent is right child
		else if (x_parent == x_grandparent->right) {
			NodeT<K, V>* x_uncle = x_grandparent->left;

			//x uncle is red
			if (x_uncle != nullptr && x_uncle->isBlack == false) {
				x_grandparent->isBlack = false;
				x_parent->isBlack = true;
				x_uncle->isBlack = true;
				x = x_grandparent;
			}
			else {
				//x is left child(right rotation to make it right child case)
				if (x == x_parent->left) {
					rightRotation(x_parent);
					x = x_parent;
					x_parent = x->parent;
				}
				//x is right child
				leftRotation(x_grandparent);
				x_parent->isBlack = true;
				x_grandparent->isBlack = false;
				x = x_parent;
			}
		}
		root->isBlack = true;
	}

}

template<typename K, typename V>
void RedBlackTree<typename K, typename V>::fixRemove(NodeT<K, V>*& x,NodeT<K,V>*& x_parent) {

	NodeT<K, V>* y;
		while (x != root &&(!x|| x->isBlack == true)) {
			if (x == x_parent->left) {
				y = x_parent->right;
				if (y->isBlack == false) {
					y->isBlack = true;
					x_parent->isBlack = false;
					leftRotation(x_parent);
					y = x_parent->right;
				}
	
		        if ((!y->left||y->left->isBlack == true) && (!y->right||y->right->isBlack == true)) {
						y->isBlack = false;
						x = x_parent;
					}
				else {
				       if (!y->right || y->right->isBlack == true) {
						   if (y->left!=nullptr) {
							   y->left->isBlack = true;
						   }
					        y->isBlack = false;
					        rightRotation(y);
						    y = x_parent->right;
				        }
						y->isBlack = x_parent->isBlack;
						x_parent->isBlack = true;
						if (y->right != nullptr) {
							y->right->isBlack = true;
						}
						leftRotation(x_parent);
						x = root;
					}
				
			}
			else {
				y = x_parent->left;
				if (y->isBlack == false) {
					y->isBlack = true;
					x_parent->isBlack = false;
					rightRotation(x_parent);
					y = x_parent->left;
				}
				if ((!y->left||y->left->isBlack == true) && (!y->right||y->right->isBlack == true)) {
					y->isBlack = false;
					x = x_parent;
				}
				else {
					if (!y->left||y->left->isBlack == true) {
						if (y->right!=nullptr) {
							y->right->isBlack = true;
						}
						y->isBlack = false;
						leftRotation(y);
						y = x_parent->left;
					}
					y->isBlack = x_parent->isBlack;
					x_parent->isBlack = true;
					if (y->left != nullptr) {
						y->left->isBlack = true;
					}
					rightRotation(x_parent);
					x = root;
				}
			}
		}
		x->isBlack = true;
	}
	


template<typename K, typename V>
NodeT<K,V>* RedBlackTree<typename K, typename V>::findNode(K key) {

	NodeT<K, V>* p = root;
	

	while (p != nullptr) {
		if (key == p->key) {
			break;
		}
		else if (key < p->key) {
			p = p->left;
		}
		else {
			p = p->right;
		}
	}

	return p;
}

template<typename K, typename V>
NodeT<K, V>* RedBlackTree<typename K, typename V>::predecessor(NodeT<K, V>* x) {
	//maximum value in the left subtree is predecessor
	
	NodeT<K, V>* p = x->left;
	while (p->right != nullptr) {
		p = p->right;
	}

	return p;
}

template<typename K, typename V>
NodeT<K, V>* RedBlackTree<typename K, typename V>::copy(NodeT<K,V>* nd) {
	if (nd == nullptr) {
		return nullptr;
	 }
	NodeT<K, V>* new_nd = new NodeT<K, V>(nd->key, nd->data);
	new_nd->isBlack = nd->isBlack;
	new_nd->left = copy(nd->left);
	new_nd->right = copy(nd->right);
	if (nd->left) {
		nd->left->parent = nd;
	}
	if (nd->right) {
		nd->right->parent = nd;
	}

	key_value.insert(pair<K, V>(nd->key, nd->data));
	
	return new_nd;
}

template<typename K, typename V>
void RedBlackTree<typename K, typename V>::clear(NodeT<K, V>* nd) {
	
	if (nd != nullptr) {
		clear(nd->left);
		clear(nd->right);
		delete nd;
	}
}



