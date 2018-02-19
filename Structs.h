/**
Basic Data Structre
*/

/**
Info is a struct including all infomations of one user. One user would have only one real-object.
*/
typedef struct info {
	char * name;
	int age;
	int sex;//1->man 2->woman
	//and othe infomations
} Info;

/**
FNode here means those node in AVL-Trees of users' follow and followed.
*/
typedef struct followNode {
	Info * info;
	int height;
	struct followNode * left;
	struct followNode * right;
} FNode;

/**
UserNode would be used to set up a AVL-Tree to manage all users without any relationships among
*/
typedef struct userNode {
	Info * info;
	int height;
	FNode * following;
	FNode * followed;
	struct userNode * left;
	struct userNode * right;
} UNode;
