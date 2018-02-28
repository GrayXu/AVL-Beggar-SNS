#include <AvlCal.h>
#define NULLCODE 90017
#define INIT_USER_LENGTH 100000
#define INIT_RELATION_LENGTH 1000

// "," would be the split mark
char** setDoubleCharArray(char** out, char* in) {
	if (out == NULL || in[0] == '\0') {
		return NULL;
	}
	register int allIndex = 0;
	register int outIndex = 0;
	register int inIndex = 0;
	char read = 0;

	while ((read = *(in + allIndex)), read != '\n') {
		if (read == ',') {
			out[outIndex][inIndex] = '\0';//end this string
			outIndex++;//reset
			inIndex = 0;
		} else {
			out[outIndex][inIndex] = read;
			inIndex++;
		}
		allIndex++;
	}
	out[outIndex][inIndex] = '\0';//end the final string
	return out;
}

//free stake memory( for the double char array)
void freeDoubleCharArray(int num, char ** info) {
	register int i;
	for (i = 0; i < num; i++) {
		free(info[i]);
	}
	free(info);

}

char ** readUser(char * in) {
	char** strArray = (char**)malloc(sizeof(char *) * 4);
	strArray[0] = (char*)malloc(sizeof(char) * 30);
	strArray[1] = (char*)malloc(sizeof(char) * 10);
	strArray[2] = (char*)malloc(sizeof(char) * 10);
	strArray[3] = (char*)malloc(sizeof(char) * 10);
	if (setDoubleCharArray(strArray, in) != NULL) {
		return strArray;
	} else {
		freeDoubleCharArray(4, strArray);
		return NULL;
	}
}

char ** readRelation(char * in) {
	char ** strArray = (char**)malloc(sizeof(char *) * 2);
	//former is "follow" split with ";"
	strArray[0] = (char*)malloc(sizeof(char) * 30);
	strArray[1] = (char*)malloc(sizeof(char) * 30);
	if (setDoubleCharArray(strArray, in) != NULL) {
		return strArray;
	} else {
		freeDoubleCharArray(2, strArray);
		return NULL;
	}
}

//get users' info from disk and make them become avl tree
UNode * getUsers(FILE * file) {
	UNode * newRoot = NULL;

	UNode * unode = (UNode*)malloc(sizeof(UNode));
	unode->followed = NULL; unode->following = NULL; unode->left = NULL; unode->right = NULL;
	Info * info = (Info *)malloc(sizeof(Info));
	info->name = (char *)malloc(sizeof(char) * 30);
	info->hobby = (char *)malloc(sizeof(char) * 10);
	unode->info = info;

	char StrLine[200];//one line one user
	fgets(StrLine, 200, file);
	char ** property = readUser(StrLine);
	if (!property) {
		free(info->name);
		free(info->hobby);
		free(info);
		return NULL;
	}
	strcpy(info->name, property[0]);
	strcpy(info->hobby, property[1]);
	info->age = atoi(property[2]);
	info->sex = atoi(property[3]);
	freeDoubleCharArray(4, property);

	newRoot = unode;

	while (1) {
		info = (Info *)malloc(sizeof(Info));

		info->name = (char *)malloc(sizeof(char) * 30);
		info->hobby = (char *)malloc(sizeof(char) * 10);

		char StrLine[200];//one line one user
		fgets(StrLine, 200, file);

		if (feof(file)) {
			free(info->name);
			free(info->hobby);
			free(info);
			break;
		}

		char ** property = readUser(StrLine);
		if (!property) return NULL;
		strcpy(info->name, property[0]);
		strcpy(info->hobby, property[1]);
		info->age = atoi(property[2]);
		info->sex = atoi(property[3]);
		freeDoubleCharArray(4, property);

		newRoot = insertAVL_U(info, newRoot);

	}

	return newRoot;
}

//addRelation
void getRelation(UNode * root, FILE * file) {
	char StrLine[200];//one line one user
	fgets(StrLine, 200, file);
	char ** property = readRelation(StrLine);
	if (!property) return;
	UNode * followingNode = getUNodeFromName_U(property[0], root);
	UNode * followedNode = getUNodeFromName_U(property[1], root);
	if(followedNode == NULL || followingNode == NULL || followedNode->info == NULL || followingNode->info == NULL){
        followingNode->following = insertAVL_F(followedNode->info, followingNode->following);
        followedNode->followed = insertAVL_F(followingNode->info, followedNode->followed);
	}
	freeDoubleCharArray(2, property);

	while (1) {
		char StrLine[200];//one line one user
		fgets(StrLine, 200, file);

		if (feof(file)) {
			break;
		}

		char ** property = readRelation(StrLine);
		if (!property) return;
		UNode * followingNode = getUNodeFromName_U(property[0], root);
		UNode * followedNode = getUNodeFromName_U(property[1], root);
		if(followedNode == NULL || followedNode == NULL) continue;
        if(followedNode->info == NULL || followingNode->info == NULL) continue;
		followingNode->following = insertAVL_F(followedNode->info, followingNode->following);
		followedNode->followed = insertAVL_F(followingNode->info, followedNode->followed);
		freeDoubleCharArray(2, property);

	}
}

//generate heights in avl trees including Users and all follow-trees
void generateHeight_F(FNode * node) {
	if (!node) return;
	node->height = getDepth_F(node);

	generateHeight_F(node->left);
	generateHeight_F(node->right);
}
void generateHeight_U(UNode * node) {
	if (!node) {
		return;
	}
	node->height = getDepth_U(node);

	generateHeight_F(node->followed);
	generateHeight_F(node->following);

	generateHeight_U(node->left);
	generateHeight_U(node->right);
}
//Overload
void genHeightAfterLoad(UNode * uRoot) {
	generateHeight_U(uRoot);
}
