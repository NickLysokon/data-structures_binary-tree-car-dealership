#ifndef _MY_AVL_TREE_H
#define _MY_AVL_TREE_H

#include <iostream>
#include <exception>


static const int EMPTY_NODE_HEIGHT = -1;

template<class T, class S>
struct TreeNode {
	S key;
	T data;
	TreeNode<T, S> *left, *right, *up;
	int height;

	TreeNode() { //Fixes random visual studio error
		left = nullptr;
		right = nullptr;
		up = nullptr;
		height = -1;
	} 
};



template<class T, class S>
class AvlTree {
	TreeNode<T, S>* root;

	TreeNode<T, S>* copyBranch(TreeNode<T, S>* br);
	TreeNode<T, S>* insertInBranch(TreeNode<T, S>* br, TreeNode<T, S>* node);
	TreeNode<T, S>* rotate(TreeNode<T, S>* br, TreeNode<T, S>* node);

public:
	AvlTree();
	~AvlTree();
	AvlTree(const AvlTree& tree);
	void insert(const S& key, const T& element);
	void remove(const S& key);
	TreeNode<T, S>* getMinNode();
	TreeNode<T, S>* getMaxNode();
	TreeNode<T, S>* findNode(const S& key);
	TreeNode<T, S>* getRoot(); //Returns the root, or null if empty
	TreeNode<T, S>* getInOrderNext(TreeNode<T, S>* node);

	class REPEATED_NODE :public std::exception {};

	//for debug purposes:
	void inOrderPrint();
};






template<class T, class S>
AvlTree<T, S>::AvlTree() : root(new TreeNode<T, S>) {
	//Dummy node:
	root->up = nullptr;
	root->left = nullptr;
	root->right = nullptr;
	root->height = EMPTY_NODE_HEIGHT;
}

template<class T, class S>
static void destroyBranch(TreeNode<T, S>* br)
{
	if (br == nullptr) return;

	destroyBranch(br->left);
	destroyBranch(br->right);
	delete br;
}

template<class T, class S>
AvlTree<T, S>::~AvlTree() {
	destroyBranch(root);
}


template<class T, class S>
TreeNode<T, S>* AvlTree<T, S>::copyBranch(TreeNode<T, S>* br)
{
	if (br == nullptr) return nullptr;

	TreeNode<T, S>* ptr = new TreeNode<T, S>;
	ptr->data = br->data;
	ptr->height = br->height;

	TreeNode<T, S>* left_copy = copyBranch(br->left), right_copy = copyBranch(br->right);
	ptr->left = left_copy;
	ptr->right = right_copy;

	if (left_copy != nullptr) left_copy->up = ptr;
	if (right_copy != nullptr) right_copy->up = ptr;

	return ptr;
}


template<class T, class S>
AvlTree<T, S>::AvlTree(const AvlTree& tree)
{
	root = copyBranch(tree.root);
}


static int max(int a, int b)
{
	return a > b ? a : b;
}

template<class T, class S>
static int safeGetHeight(TreeNode<T, S>* ptr)
{
	return ptr == nullptr ? EMPTY_NODE_HEIGHT : ptr->height;
}


template<class T, class S>
static int getBalanceFactor(TreeNode<T, S>* node)
{
	if (node == nullptr)
		return 0;

	return safeGetHeight(node->left) - safeGetHeight(node->right);
}

//Preform a left rotation in O(1), and return the new root
template<class T, class S>
static TreeNode<T, S>* rotateLeft(TreeNode<T, S>* node)
{
	TreeNode<T, S>* new_root = node->right;

	node->right = new_root->left;
	if (new_root->left != nullptr) {
		new_root->left->up = node;
	}

	new_root->up = node->up;
	new_root->left = node;
	node->up = new_root;

	node->height = 1 + max(safeGetHeight(node->left), safeGetHeight(node->right));
	new_root->height = 1 + max(safeGetHeight(new_root->left), safeGetHeight(new_root->right));

	return new_root;
}

//Preform a right rotation in O(1), and return the new root
template<class T, class S>
static TreeNode<T, S>* rotateRight(TreeNode<T, S>* node)
{
	TreeNode<T, S>* new_root = node->left;

	node->left = new_root->right;
	if (new_root->right != nullptr) {
		new_root->right->up = node;
	}

	new_root->up = node->up;
	new_root->right = node;
	node->up = new_root;

	node->height = 1 + max(safeGetHeight(node->left), safeGetHeight(node->right));
	new_root->height = 1 + max(safeGetHeight(new_root->left), safeGetHeight(new_root->right));

	return new_root;
}


template<class T, class S>
TreeNode<T, S>* AvlTree<T, S>::rotate(TreeNode<T, S>* br, TreeNode<T, S>* node)
{
	int balance_factor = getBalanceFactor(br);

	if (balance_factor > 1 && br->left != nullptr && node->key < br->left->key) { //LL
		return rotateRight(br);
	}
	else if (balance_factor < -1 && br->right != nullptr && node->key > br->right->key) { //RR
		return rotateLeft(br);
	}
	else if (balance_factor > 1 && br->left != nullptr && node->key > br->left->key) { //LR
		br->left = rotateLeft(br->left);
		return rotateRight(br);
	}
	else if (balance_factor < -1 && br->right != nullptr && node->key < br->right->key) { //RL
		br->right = rotateRight(br->right);
		return rotateLeft(br);
	}

	return br;
}


template<class T, class S>
TreeNode<T, S>* AvlTree<T, S>::insertInBranch(TreeNode<T, S>* br, TreeNode<T, S>* node)
{
	if (br == nullptr) {
		return node;
	}

	if (br->key > node->key) {
		node->up = br;
		br->left = insertInBranch(br->left, node);
	}
	else if (br->key < node->key) {
		node->up = br;
		br->right = insertInBranch(br->right, node);
	}
	else {
		throw REPEATED_NODE();
	}

	br->height = 1 + max(safeGetHeight(br->left), safeGetHeight(br->right));

	return AvlTree<T, S>::rotate(br, node);
}

template<class T, class S>
void AvlTree<T, S>::insert(const S& key, const T& element)
{
	TreeNode<T, S>* ptr = new TreeNode<T, S>;
	ptr->data = element;
	ptr->key = key;
	ptr->height = 0;
	ptr->left = nullptr;
	ptr->right = nullptr;
	ptr->up = nullptr;

	try {
	root->left = insertInBranch(root->left, ptr);
	}
	catch (REPEATED_NODE& ex) {
		delete ptr;
		throw REPEATED_NODE();
	}
}


template<class T, class S>
static void copyInfo(TreeNode<T, S>* src, TreeNode<T, S>* dest)
{
	if (!src || !dest) return;

	dest->data = src->data;
	dest->height = src->height;
	dest->key = src->key;
}

//Removes a given node and balances the tree
template<class T, class S>
static TreeNode<T, S>* removeNode(TreeNode<T, S>* br, const S& key)
{
	if (br == nullptr) return nullptr;

	if (br->key > key) {
		br->left = removeNode(br->left, key);
	}
	else if (br->key < key) {
		br->right = removeNode(br->right, key);
	}
	else {   //br->key = key
		if (br->left == nullptr || br->right == nullptr) {
			TreeNode<T, S>* ptr = br->left ? br->left : br->right;

			if (ptr == nullptr) {  //br has no children
				ptr = br;
				br = nullptr;
			}
			else {  //br has one child
				br->key = ptr->key;
				br->data = ptr->data;  //T operator=
				br->height = 0;

				br->left = nullptr;
				br->right = nullptr;
			}

			delete(ptr);
		}
		else {
			TreeNode<T, S>* ptr = br->right;
			while (ptr->left != nullptr) { //Get next smallest key, O(logn)
				ptr = ptr->left;
			}

			br->key = ptr->key;
			br->data = ptr->data;
			br->right = removeNode(br->right, ptr->key); //Remove the switched node
		}
	}

	if (br == nullptr) return nullptr;

	br->height = 1 + max(safeGetHeight(br->left), safeGetHeight(br->right));
	int balance_factor = getBalanceFactor(br), left_balance_factor = getBalanceFactor(br->left),
		right_balance_factor = getBalanceFactor(br->right);

	if (balance_factor > 1 && left_balance_factor >= 0) //LL
		return rotateRight(br);

	if (balance_factor > 1 && left_balance_factor < 0) { //LR
		br->left = rotateLeft(br->left);
		return rotateRight(br);
	}

	if (balance_factor < -1 && right_balance_factor <= 0) //RR
		return rotateLeft(br);

	if (balance_factor < -1 && right_balance_factor > 0) { //RL
		br->right = rotateRight(br->right);
		return rotateLeft(br);
	}

	return br;
}


template<class T, class S>
void AvlTree<T, S>::remove(const S& key)
{
	if (root->left == nullptr) return; //Empty tree

	root->left = removeNode(root->left, key);
}


template<class T, class S>
TreeNode<T, S>* AvlTree<T, S>::getMinNode()
{
	TreeNode<T, S>* ptr = root;
	while (ptr->left != nullptr) {
		ptr = ptr->left;
	}

	return ptr == root ? nullptr : ptr;
}


template<class T, class S>
TreeNode<T, S>* AvlTree<T, S>::getMaxNode()
{
	if (root->left == nullptr) { //Empty tree
		return nullptr;
	}

	TreeNode<T, S>* ptr = root->left;
	while (ptr->right != nullptr) {
		ptr = ptr->right;
	}

	return ptr;
}


template<class T, class S>
TreeNode<T, S>* AvlTree<T, S>::findNode(const S& key)
{
	TreeNode<T, S>* iter = root->left;

	while (iter != nullptr) {
		if (iter->key > key) {
			iter = iter->left;
		}
		else if (iter->key < key) {
			iter = iter->right;
		}
		else return iter;
	}

	return nullptr;
}


template<class T, class S>
TreeNode<T, S>* AvlTree<T, S>::getRoot()
{
	return root->left ? root->left : nullptr;
}


template<class T, class S>
static TreeNode<T, S>* mostLeftNode(TreeNode<T, S>* node) {
	if (node == nullptr) return nullptr;

	while (node->left != nullptr) {
		node = node->left;
	}
	return node;
}


template<class T, class S>
TreeNode<T, S>* AvlTree<T, S>::getInOrderNext(TreeNode<T, S>* node)
{
	if (node->right != nullptr) {
		return mostLeftNode(node->right);
	}

	TreeNode<T, S>* father = node->up;
	while (father != nullptr && father->right == node) {
		node = father;
		father = father->up;
	}
	return father;
}

template<class T, class S>
void AvlTree<T, S>::inOrderPrint()
{
	inOrderPrintAux(root->left);
	std::cout << "End";
}


template<class T, class S>
static void inOrderPrintAux(const TreeNode<T, S>* br)
{
	if (br == nullptr) return;

	inOrderPrintAux(br->left);
	std::cout << br->data << ", ";
	inOrderPrintAux(br->right);
}



#endif /* _MY_AVL_TREE_ */