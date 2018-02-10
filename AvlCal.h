#include <AvlUtils.h>
void traInsertAVL(FNode * node);

//reNode would lead a AVL-Tree of friends set. GLOBAL
FNode * reNode;

/**
Second friend functions。
A new AVL-Tree would be created to save those second-friends and return it for traversing outside.
*/
FNode * getSecFriend(UNode * uNode) {
	reNode = NULL;
	//traverse this user-tree to call U21-func
	getSecFriend_U2T(uNode);
	//now we would get the root node we want
	return reNode;
}
/**
recursion, unode into tree
call this func outside with a root node
*/
void getSecFriend_U2T(UNode * uNode) {
	if (uNode == NULL) {
		return;
	} else {
		traInsertAVL(uNode->follow);//insert all info to this returning tree
		getSecFriend_U2T(uNode->left);
		getSecFriend_U2T(uNode->right);
	}
}

/**
Set_union. 
Get common friend for SNS.
*/
FNode * getComFriend(FNode * aFNode, FNode * bFNode) {
	reNode = NULL;
	traInsertAVL(aFNode);
	traInsertAVL(bFNode);
	//now we would get the root node we want
	return reNode;
}

/**
insert all nodes into AVL-tree 
call this func outside with a root node
*/
void traInsertAVL(FNode * node) {
	if (node == NULL) return;
	else {
		reNode = insertAVL_F(node->info, reNode, 0);//keep updating reNode
		traInsertAVL(node->left);
		traInsertAVL(node->right);
	}
}

/**
set_intersection to find common friend.
And the return-FNode should be destroyed.
*/
FNode * getIntersec(FNode * aFNode, FNode * bFNode) {
	reNode = NULL;
	getIntersec_check_insert(aFNode, bFNode);
	//get returning node
	return reNode;
}

/**
Traverse a FNode-AVL-Tree, check whether it exist in another F-A-L or not. yes->insert into returning tree
Call this func outside with a root node
*/
void getIntersec_check_insert(FNode * node, FNode * sampleRoot) {
	if (node == NULL) {
		return;
	} else {
		FNode * resultNode = getFNodeFromName_F(node->info->name, sampleRoot);
		if (resultNode) {//it exists
			insertAVL_F(resultNode->info, reNode, 0);
		}
		getIntersec_check_insert(node->left, sampleRoot);
		getIntersec_check_insert(node->right, sampleRoot);
	}
}

/**
set_equal.
1->equal, 0->not equal
*/
int isEqual(FNode * aFNode, FNode * bFNode) {
	FNode * resultNode = getIntersec(aFNode, bFNode);
	if (resultNode) {
		return 1;
	} else {
		return 0;
	}
}

/*
set_insert.
Insert a-set into b-set.

NOT USEFUL??

*/
//FNode * insertA2B(FNode * aFNode, FNode * bFNode) {
//
//}

/*
Traverse a FNode-AVL-Tree, and insert all nodes into returning tree without check

*/
//void * insertA2B_recursion(FNode * aFNode, FNode * sampleRoot) {
//	if (aFNode == NULL) {
//		return;
//	} else {
//		insertAVL_F
//	}
//
//}


/**
set-remove
*/

/**
set_subset
*/

/*
set_diffrence
*/

