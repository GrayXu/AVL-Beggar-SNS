#include <Structs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
Basic Function For AVL-Trees
*/

//-------------For Follow Node-------------

/**
This func is set for getting a specific Node.
@nullptr means we can't find such a node with this name.
Calling this func when incoming root node as parameters.
THIS IS "SearchAVL"
*/
FNode * getFNodeFromName_F(char * name, FNode * node) {
	if (node == NULL) return NULL;//which means that we can't find such a node with this name
	else {
		char * nodeName = node->info->name;
		int result = strcmp(name, nodeName);
		if (result == 0) {
			return node;
		} else if (result < 0) {
			return getFNodeFromName_F(name, node->left);
		} else {//result > 0
			return getFNodeFromName_F(name, node->right);
		}
	}
}

/**
Destroy this tree and free all space it had taken except those user info
*/
void destroyAVL_F(FNode * node) {
	if (node == NULL) return;
	destroyAVL_F(node->left); node->left = NULL;
	destroyAVL_F(node->right); node->right = NULL;
	free(node);
}

/**
Get a AVL-Tree's size.
@retrun a integer.
*/
int getAvlSize_F(FNode * node) {
	if (node == NULL) return 0;
	return getAvlSize_F(node->left) + getAvlSize_F(node->right) + 1;
}

//This func is only called to reduce checking nullptr when you want to get height of FNode.
int getHeight_F(FNode * node) {
	if (node) return node->height;
	else return 0;
}

//update parameter node's height.
void updateHeight_F(FNode * node) {
	int leftHeight = 0;
	int rightHeight = 0;
	if (node->left) leftHeight = node->left->height;
	if (node->right) leftHeight = node->right->height;
	node->height = (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

//compare keys between a and b( To reduce my typing time outside this func
int compare_F(FNode * a, FNode * b) {
	return strcmp(a->info->name, b->info->name);
}

//Do left-left rotation, and return a new root for outside to update
FNode * LL_Rotation_F(FNode * oldHead) {
	FNode * oldLeft = oldHead->left;
	oldHead->left = oldLeft->right;
	oldLeft->right = oldHead;

	//change height
	updateHeight_F(oldLeft);
	updateHeight_F(oldHead);

	return oldLeft;
}

//Do right-right rotation, and return a new root for outside to update
FNode * RR_Rotation_F(FNode * oldHead) {
	FNode * oldRight = oldHead->right;
	oldHead->right = oldRight->left;
	oldRight->left = oldHead;

	//change height
	updateHeight_F(oldRight);
	updateHeight_F(oldHead);

	return oldRight;
}

//Do left-right rotation, and return a new root for outside to update
FNode * LR_Rotation_F(FNode * oldHead) {
	oldHead->left = RR_Rotation_F(oldHead->left);
	return LL_Rotation_F(oldHead);
}

//Do right-left rotation, and return a new root for outside to update
FNode * RL_Rotation_F(FNode * oldHead) {
	oldHead->right = LL_Rotation_F(oldHead->right);
	return RR_Rotation_F(oldHead);
}

/**
@return: new root. Nullptr->something bad happen.
the parameter-newInfo is set for creat new space for new FNode
*/
FNode * insertAVL_F(Info * newInfo, FNode * node) {

	if (node == NULL) {//find a right position to insert this new node
					   //initalize new node
		FNode * newNode = (FNode *)malloc(sizeof(FNode));
		newNode->info = newInfo;
		newNode->left = NULL;
		newNode->right = NULL;//add to be a leave
		newNode->height = 0;
		node = newNode;
	} else {
		int result = strcmp(newInfo->name, node->info->name);

		if (result == 0) {//there is a same name already
			return NULL;
		} else {
			if (result < 0) {
				node->left = insertAVL_F(newInfo, node->left);

				//check balance( the part below would be run for many times
				if (getHeight_F(node->left) - getHeight_F(node->right) == 2) {
					if (strcmp(newInfo->name, node->left->info->name) < 0) {
						node = LL_Rotation_F(node);//make right rotation
					} else {
						node = LR_Rotation_F(node);//make left-riight rotation
					}
				}

			} else {//result > 0
				node->right = insertAVL_F(newInfo, node->right);

				//check balance( the part below would be run for many times
				if (getHeight_F(node->right) - getHeight_F(node->left) == 2) {
					if (strcmp(newInfo->name, node->right->info->name) > 0) {
						node = RR_Rotation_F(node);//make right rotation
					} else {
						node = RL_Rotation_F(node);//make left-riight rotation
					}
				}
			}
		}
	}

	//updata height
	updateHeight_F(node);//would update for enough times
	return node;
}

//get the max FNode in AVL-Tree
FNode * getMaxFNode_F(FNode * node) {
	if (!node) return NULL;
	while (node->right) node = node->right;
	return node;
}

//get the min FNode in AVL-Tree
FNode * getMinFNode_F(FNode * node) {
	if (!node) return NULL;
	while (node->left) node = node->left;
	return node;
}

/**
Delete a FNode in this AVL-tree with parameter key( user's name). WOUNDN'T FREE INFO SPACE
@parameter	key: the key as a user's name to find the correct FNode
node: sub-tree's root node
@return: a new root node for outside updating
*/
FNode * deleteAVL_F(char * key, FNode * node) {
	if (node == NULL || key == NULL) {
		return NULL;
	}

	int result = strcmp(key, node->info->name);
	if (result < 0) {//go left
		node->left = deleteAVL_F(key, node);
		//check balance
		if (getHeight_F(node->right) - getHeight_F(node->left) == 2) {
			if (getHeight_F(node->right->left) > getHeight_F(node->right->right)) {
				node = RL_Rotation_F(node);
			} else {
				node = RR_Rotation_F(node);
			}
		}
	} else if (result > 0) {//go right
		node->right = deleteAVL_F(key, node);
		//check balance
		if (getHeight_F(node->left) - getHeight_F(node->right) == 2) {
			if (getHeight_F(node->left->right) > getHeight_F(node->left->left)) {
				node = LR_Rotation_F(node);
			} else {
				node = LL_Rotation_F(node);
			}
		}
	} else {//result == 0 ( found the node
		if (node->left != NULL && node->right != NULL) {
			if (getHeight_F(node->left) > getHeight_F(node->right)) {
				FNode * maxFNode = getMaxFNode_F(node->left);//get the max node in left sub-tree
				node->info = maxFNode->info;
				node->left = deleteAVL_F(maxFNode->info->name, node->left);
			} else {
				FNode * minFNode = getMinFNode_F(node->right);//get the min node in right sub-tree
				node->info = minFNode->info;
				node->right = deleteAVL_F(minFNode->info->name, node->right);
			}
		} else {
			if (node->left) node = node->left;
			else node = node->right;
		}
	}

	return node;
}

//based on pre-order traverse
void traverseAVL_F(FNode * node) {
	if (!node) return;
	printf("%s;", node->info->name);
	traverseAVL_F(node->left);
	traverseAVL_F(node->right);
}



//------------------For User Node-------------

/**
This func is set for getting a specific Node.
@nullptr means we can't find such a node with this name.
Calling this func when incoming root node as parameters.
THIS IS "SearchAVL"
*/
UNode * getUNodeFromName_U(char * name, UNode * node) {
	if (node == NULL) return NULL;//which means that we can't find such a node with this name
	else {
		char * nodeName = node->info->name;
		int result = strcmp(name, nodeName);
		if (result == 0) {
			return node;
		} else if (result < 0) {
			return getUNodeFromName_U(name, node->left);
		} else {//result > 0
			return getUNodeFromName_U(name, node->right);
		}
	}
}

/**
Destroy this tree and free all space it had taken except those user info
*/
void destroyAVL_U(UNode * node) {
	if (node == NULL) return;
	destroyAVL_U(node->left); node->left = NULL;
	destroyAVL_U(node->right); node->right = NULL;
	if (node->info) {
		free(node->info->name);
		free(node->info);
	}
	free(node);
}

/**
Get a AVL-Tree's size.
@retrun a integer.
*/
int getAvlSize_U(UNode * node) {
	if (node == NULL) return 0;
	return getAvlSize_U(node->left) + getAvlSize_U(node->right) + 1;
}

//This func is only called to reduce checking nullptr when you want to get height of UNode.
int getHeight_U(UNode * node) {
	if (node) return node->height;
	else return 0;
}

//update parameter node's height.
void updateHeight_U(UNode * node) {
	int leftHeight = 0;
	int rightHeight = 0;
	if (node->left) leftHeight = node->left->height;
	if (node->right) leftHeight = node->right->height;
	node->height = (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

//compare keys between a and b( To reduce my typing time outside this func
int compare_U(UNode * a, UNode * b) {
	return strcmp(a->info->name, b->info->name);
}

//Do left-left rotation, and return a new root for outside to update
UNode * LL_Rotation_U(UNode * oldHead) {
	UNode * oldLeft = oldHead->left;
	oldHead->left = oldLeft->right;
	oldLeft->right = oldHead;

	//change height
	updateHeight_U(oldLeft);
	updateHeight_U(oldHead);

	return oldLeft;
}

//Do right-right rotation, and return a new root for outside to update
UNode * RR_Rotation_U(UNode * oldHead) {
	UNode * oldRight = oldHead->right;
	oldHead->right = oldRight->left;
	oldRight->left = oldHead;

	//change height
	updateHeight_U(oldRight);
	updateHeight_U(oldHead);

	return oldRight;
}

//Do left-right rotation, and return a new root for outside to update
UNode * LR_Rotation_U(UNode * oldHead) {
	oldHead->left = RR_Rotation_U(oldHead->left);
	return LL_Rotation_U(oldHead);
}

//Do right-left rotation, and return a new root for outside to update
UNode * RL_Rotation_U(UNode * oldHead) {
	oldHead->right = LL_Rotation_U(oldHead->right);
	return RR_Rotation_U(oldHead);
}

/**
Make sure unode is not a nullptr.
@return: new root. Nullptr->something bad happen, but it's hard to capture outside,
but it would show up at cutting this avl-tree
*/
void printInfo(int mode);
UNode * insertAVL_U(Info * newInfo, UNode * node) {

	if (node == NULL) {//find a right position to insert this new node
		UNode * newNode = (UNode *)malloc(sizeof(UNode));
		if (newNode == NULL) {
			printInfo(1);
			exit(1);
		}
		//initalize new node
		newNode->left = NULL;
		newNode->right = NULL;
		newNode->info = newInfo;
		newNode->height = 0;
		node = newNode;
	} else {
		int result = strcmp(newInfo->name, node->info->name);

		if (result == 0) {//there is a same name already
			return NULL;
		} else {
			if (result < 0) {
				node->left = insertAVL_U(newInfo, node->left);

				//check balance( the part below would be run for many times
				if (getHeight_U(node->left) - getHeight_U(node->right) == 2) {
					if (strcmp(newInfo->name, node->left->info->name) < 0) {
						node = LL_Rotation_U(node);//make right rotation
					} else {
						node = LR_Rotation_U(node);//make left-riight rotation
					}
				}

			} else {//result > 0
				node->right = insertAVL_U(newInfo, node->right);

				//check balance( the part below would be run for many times
				if (getHeight_U(node->right) - getHeight_U(node->left) == 2) {
					if (strcmp(newInfo->name, node->right->info->name) > 0) {
						node = RR_Rotation_U(node);//make right rotation
					} else {
						node = RL_Rotation_U(node);//make left-riight rotation
					}
				}
			}
		}
	}

	

	//updata height
	updateHeight_U(node);
	return node;
}

//get the max UNode in AVL-Tree
UNode * getMaxUNode_U(UNode * node) {
	if (!node) return NULL;
	while (node->right) node = node->right;
	return node;
}

//get the min UNode in AVL-Tree
UNode * getMinUNode_U(UNode * node) {
	if (!node) return NULL;
	while (node->left) node = node->left;
	return node;
}

/**
Delete a UNode in this AVL-tree with parameter key( user's name)
@parameter	key: the key as a user's name to find the correct UNode
node: sub-tree's root node
@return: a new root node for outside updating
*/
UNode * deleteAVL_U(char * key, UNode * node) {
	if (node == NULL || key == NULL) {
		return NULL;
	}

	int result = strcmp(key, node->info->name);
	if (result < 0) {//go left
		node->left = deleteAVL_U(key, node);
		//check balance
		if (getHeight_U(node->right) - getHeight_U(node->left) == 2) {
			if (getHeight_U(node->right->left) > getHeight_U(node->right->right)) {
				node = RL_Rotation_U(node);
			} else {
				node = RR_Rotation_U(node);
			}
		}
	} else if (result > 0) {//go right
		node->right = deleteAVL_U(key, node);
		//check balance
		if (getHeight_U(node->left) - getHeight_U(node->right) == 2) {
			if (getHeight_U(node->left->right) > getHeight_U(node->left->left)) {
				node = LR_Rotation_U(node);
			} else {
				node = LL_Rotation_U(node);
			}
		}
	} else {//result == 0 ( found the node
		if (node->left != NULL && node->right != NULL) {
			if (getHeight_U(node->left) > getHeight_U(node->right)) {
				UNode * maxUNode = getMaxUNode_U(node->left);//get the max node in left sub-tree
				node->info = maxUNode->info;
				node->left = deleteAVL_U(maxUNode->info->name, node->left);
			} else {
				UNode * minUNode = getMinUNode_U(node->right);//get the min node in right sub-tree
				node->info = minUNode->info;
				node->right = deleteAVL_U(minUNode->info->name, node->right);
			}
		} else {
			UNode * delNode = node;
			if (node->left) node = node->left;
			else node = node->right;

			//when you want to delete a UNode, which means this guy would be deleted including all infomation
			if (delNode->info) {
				free(delNode->info->name);
				free(delNode->info);
			}
			free(delNode);
		}
	}

	return node;
}

//based on pre-order traverse
void traverseAVL_U(UNode * node) {
	if (!node) return;
	printf("%s;", node->info->name);
	traverseAVL_U(node->left);
	traverseAVL_U(node->right);
}
