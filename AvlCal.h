#include <AvlUtils.h>
void traInsertAVL(FNode * node);

/*
reNode would lead a AVL-Tree of friends set. GLOBAL
MUST BE FREE after show
*/
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
		reNode = insertAVL_F(node->info, reNode, 0);//keep updating reNode
		traInsertAVL(node->left);
		traInsertAVL(node->right);
	}
}

/**
set_intersection
Get common friend for SNS.
And the return-FNode should be destroyed outside.
*/
FNode * getSetIntersec(FNode * aFNode, FNode * bFNode) {
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
		if (resultNode) {//exist
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
int setEqual(FNode * aFNode, FNode * bFNode) {
	FNode * resultNode = getSetIntersec(aFNode, bFNode);
	int flag = 0;
	if (resultNode) {
		flag = 1;
	}
	destroyAVL_F(resultNode);//free this temporary tree
	
	return flag;
}


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
			insertAVL_F(resultNode->info, reNode, 0);
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
*/
FNode * insertA2B(FNode * aFNode, FNode * bFNode) {
	insertA2B_recursion(aFNode, bFNode);
}
/*
Traverse a FNode-AVL-Tree, and insert all nodes into sample tree
*/
void * insertA2B_recursion(FNode * aFNode, FNode * sampleRoot) {
	if (aFNode == NULL) {
		return;
	} else {
		insertAVL_F(aFNode->info, sampleRoot, 0);
		insertA2B_recursion(aFNode->left, sampleRoot);
		insertA2B_recursion(aFNode->right, sampleRoot);
	}
}