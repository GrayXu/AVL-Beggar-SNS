﻿#include <AvlUtils.h>
void traInsertAVL(FNode * node);

/*
reNode would lead a AVL-Tree of friends set. GLOBAL
MUST BE FREE after show
*/
FNode * reNode;
FNode * getSetIntersec(FNode * aFNode, FNode * bFNode);
/**
Second friend functions¡£
A new AVL-Tree would be created to save those second-friends and return it for traversing outside.
*/
void getSecFriend_U2T(FNode * fNode, UNode * uRoot);

FNode * getSecFriend(UNode * uNode, UNode * uRoot) {
	reNode = NULL;
	//traverse this user-tree to call U21-func
	FNode * friRoot = getSetIntersec(uNode->followed, uNode->following);//get first-friends
//    traInsertAVL(friRoot);
    getSecFriend_U2T(friRoot, uRoot);
    destroyAVL_F(friRoot);
	reNode = deleteAVL_F(uNode->info->name, reNode);//delete itself
	//now we would get the root node we want
	return reNode;
}
/**
recursion, unode into tree
call this func outside with a root node of friends
*/
void getSecFriend_U2T(FNode * fNode, UNode * uRoot) {
	if (fNode == NULL) {
		return;
	} else {
	    UNode * uNode = getUNodeFromName_U(fNode->info->name, uRoot);
	    FNode * friRoot = getSetIntersec(uNode->followed, uNode->following);
        traInsertAVL(friRoot);
        destroyAVL_F(friRoot);
		getSecFriend_U2T(fNode->left, uRoot);
		getSecFriend_U2T(fNode->right, uRoot);
	}
}

/**
SET_UNION.
*/
FNode * getSetUnion(FNode * aFNode, FNode * bFNode) {
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
		reNode = insertAVL_F(node->info, reNode);//keep updating reNode
		traInsertAVL(node->left);
		traInsertAVL(node->right);
	}
}

FNode * reNodeIntersec = NULL;
/**
Traverse a FNode-AVL-Tree, check whether it exist in another F-A-L or not. yes->insert into returning tree
Call this func outside with a root node.
FOR "getSetIntersec"
*/
void getIntersec_check_insert(FNode * node, FNode * sampleRoot) {
	if (node == NULL) {
		return;
	} else {
		FNode * resultNode = getFNodeFromName_F(node->info->name, sampleRoot);
		if (resultNode) {//exist in another avl tree
			reNodeIntersec = insertAVL_F(resultNode->info, reNodeIntersec);
		}
		getIntersec_check_insert(node->left, sampleRoot);
		getIntersec_check_insert(node->right, sampleRoot);
	}
}

/**
set_intersection
And the return-FNode should be destroyed outside.
*/

FNode * getSetIntersec(FNode * aFNode, FNode * bFNode) {
	reNodeIntersec = NULL;
	getIntersec_check_insert(aFNode, bFNode);
	return reNodeIntersec;
}


FNode * getSetDifference(FNode * aFNode, FNode * bFNode);
/**
set_equal.
1->equal, 0->not equal
*/
int setEqual(FNode * aFNode, FNode * bFNode) {
	int flag = 0;
	FNode * resultA = getSetDifference(aFNode, bFNode);
	FNode * resultB = getSetDifference(bFNode, aFNode);
	if (resultA == NULL && resultB == NULL) {
		flag = 1;
	}
	destroyAVL_F(resultA);//free this temporary tree
	destroyAVL_F(resultB);
	return flag;
}

void getSetDiff_recursion(FNode * minusNode, FNode * sampleRoot);
/*
Set_diffrence -> a-b,
Traverse each node of a=tree, find out if it's exist in b-tree or not.
*/
FNode * getSetDifference(FNode * aFNode, FNode * bFNode) {
	reNode = NULL;
	getSetDiff_recursion(aFNode, bFNode);
	return reNode;
}
/**
Traverse a FNode-AVL-Tree, check whether it exist in another F-A-L or not. yes->insert into returning tree
Call this func outside with a root node to be para "node"
*/
void getSetDiff_recursion(FNode * minusNode, FNode * sampleRoot) {
	if (minusNode == NULL) {
		return;
	} else {
		FNode * resultNode = getFNodeFromName_F(minusNode->info->name, sampleRoot);
		if (resultNode == NULL) {//not exist
			reNode = insertAVL_F(minusNode->info, reNode);
		}
		getSetDiff_recursion(minusNode->left, sampleRoot);
		getSetDiff_recursion(minusNode->right, sampleRoot);
	}
}

/**
set_subset
If a is subset of b, which means that a-b=0
@return 1->yes 0->no
*/
int isAsub2B(FNode * aFNode, FNode * bFNode) {
	FNode * resultNode = getSetDifference(aFNode, bFNode);
	int flag = 0;
	if (resultNode) {
		flag = 1;
	}
	destroyAVL_F(resultNode);
	return flag;
}

/**
Set-remove
Logistic : a=a-b
@return a new A-node for updating data
*/
FNode * setRemove(FNode * aFNode, FNode * bFNode) {
	FNode * resultNode = getSetDifference(aFNode, bFNode);
	destroyAVL_F(aFNode);
	return resultNode;
}

/*
set_insert.
Insert a-set into b-set.
WARNING: it would change b-set, if you wanna get a new tree to storage those sum-data then use getComFriend
Traverse a FNode-AVL-Tree, and insert all nodes into sample tree
*/

FNode * insertA2B(FNode * aFNode, FNode * sampleRoot) {
	if (aFNode == NULL) {
		return NULL;
	} else {
		sampleRoot = insertAVL_F(aFNode->info, sampleRoot);
		sampleRoot = insertA2B(aFNode->left, sampleRoot);
		sampleRoot = insertA2B(aFNode->right, sampleRoot);
		return sampleRoot;
	}
}

/*
get a avl tree filled with users having same hobby with sampleNode
*/
void getSameHobbyUsers_re(UNode * node, char * sampleHobby) {
	if (!node) return;

	if (strcmp(node->info->hobby, sampleHobby) == 0) {
		reNode = insertAVL_F(node->info, reNode);
	}
	getSameHobbyUsers_re(node->left, sampleHobby);
	getSameHobbyUsers_re(node->right, sampleHobby);
}
FNode * getSameHobbyUsers(UNode * sampleNode, UNode * root) {
	if (sampleNode == NULL) {
		return NULL;
	} else {
		reNode = NULL;
		getSameHobbyUsers_re(root, sampleNode->info->hobby);
		return reNode;
	}
}
