#include <AvlCal.h>

UNode * uRoot;

char *fgetsNoN(char *buf, int bufsize, FILE *stream);
void printInfo(int mode);

/*
Trick
1->系统堆空间满导致失败
2->操作成功
3->无此名的用户
4->没有找到相关信息
*/
void printInfo(int mode) {
	switch (mode) {
	case 1:
		printf("系统堆空间满导致失败。\n");
		break;
	case 2:
		printf("操作成功。\n");
		break;
	case 3:
		printf("无此名的用户。\n");
		break;
	case 4:
		printf("没有找到相关信息\n");
		break;
	}
}

int printMenu() {
	int op = 90071;

	printf("---------------这是一个基于AVL自平衡树的SNS社交网络模型系统----------------\n\n\n");

	printf("\t1.添加新用户\t\t\t\t2.删除用户\n");
	printf("\t3.修改用户信息\t\t\t\t4.查询用户信息\n");
	printf("\t5.向用户添加新的关注关系\t\t6.删除用户的关注关系\n\n");

	printf("\t7.查找用户的所有关注\t\t\t8.查找用户的所有粉丝\n");
	printf("\t9.查找用户的所有好友\t\t\t10.查找用户的所有二度好友\n\n");

	printf("\t11.查找任意两用户的共同关注\t\t12.查找任意两用户的共同好友\n");
	printf("\t13.查找任意两用户的共同粉丝\t\t14.判断任意两用户的所有好友\n");
	printf("\t15.查找任意两用户的独有好友\t\t16.判断任意两用户的所有好友\n\n");

	printf("\t0.退出系统\n\n");

	printf("---------------------------------------------------------------------------\n");
	scanf("%d%*c", &op);

	return op;
}

//add a new user
void addUser() {

	printf("请输入该用户的用户名（不可与已有用户相同）:");
	char * newUserName = (char *)malloc(sizeof(char) * 50);
	fgetsNoN(newUserName, 50, stdin);

	UNode * check = getUNodeFromName_U(newUserName, uRoot);
	if (check) {
		printf("此用户名已存在。\n");
	} else {
		printf("请输入该用户的年龄:");
		int age = 0; scanf("%d%*c", &age);
		int sex = 0;
		while (sex != 1 || sex != 2) {
			printf("请输入数字选择该用户的性别（1->男 2->女）");
			scanf("%d%*c", &sex);
		}
		Info * newInfo = (Info *)malloc(sizeof(Info));
		newInfo->age = age;
		newInfo->sex = sex;
		newInfo->name = newUserName;
		uRoot = insertAVL_U(newInfo, uRoot);
		printInfo(2);
	}

}

//del a user from this system
void delUser() {

	printf("请输入该用户的用户名:");
	char * userName = (char *)malloc(sizeof(char) * 50);
	fgetsNoN(userName, 50, stdin);

	UNode * check = getUNodeFromName_U(userName, uRoot);
	if (!check) {
		printInfo(3);
	} else {
		uRoot = deleteAVL_U(userName, uRoot);
		printInfo(2);
	}

}

//change user's infomation
void changeUser() {
	printf("请输入该用户的用户名:");
	char * userName = (char *)malloc(sizeof(char) * 50);
	fgetsNoN(userName, 50, stdin);

	UNode * getUNode = getUNodeFromName_U(userName, uRoot);
	if (!getUNode) {
		printInfo(3);
	} else {
		int select = 0;
		while (select != 1 || select != 2) {
			printf("请输入数字来选择你想要修改的属性\n\t 1->年龄  2->性别:");
			scanf("%d%*c", &select);
		}
		if (select == 1) {
			printf("请输入该用户修改后的年龄：");
			int newAge = 0; scanf("%d%*c", &newAge);
			getUNode->info->age = newAge;
		} else if (select == 2) {
			printf("请输入数字选择进行修改用户的性别：(1->男  2->女)");
			int newSex = 0; scanf("%d%*c", &newSex);
			getUNode->info->sex = newSex;
		}
	}
}

//show all infomation of a specify user
void showUser() {
	printf("请输入该用户的用户名:");
	char * userName = (char *)malloc(sizeof(char) * 50);
	fgetsNoN(userName, 50, stdin);

	UNode * getUNode = getUNodeFromName_U(userName, uRoot);
	if (!getUNode) {
		printInfo(3);
	} else {
		Info * info = getUNode->info;
		printf("用户名:%s ; 年龄:%d ; 性别:", info->name, info->age);
		if (info->sex == 1) {
			printf("男\n");
		} else {//sex == 2
			printf("女\n");
		}
	}
}

//add following relationship between two users
void addRelation() {
	printf("请输入主动关注的用户的用户名:");
	char * followUserName = (char *)malloc(sizeof(char) * 50);
	fgetsNoN(followUserName, 50, stdin);

	UNode * followUNode = getUNodeFromName_U(followUserName, uRoot);
	if (!followUNode) {
		printInfo(3);
	} else {
		printf("请输入被关注的用户的用户名:");
		char * followedUserName = (char *)malloc(sizeof(char) * 50);
		fgetsNoN(followedUserName, 50, stdin);
		UNode * followedUNode = getUNodeFromName_U(followedUserName, uRoot);
		if (!followedUNode) {
			printInfo(3);
		} else {
			//assume we have so much stake space
			followedUNode->followed = insertAVL_F(followUNode->info, followedUNode->followed);
			if (followedUNode->followed) {
				followUNode->following = insertAVL_F(followedUNode->info, followUNode->following);
				printInfo(2);
			} else {
				printf("这两个用户已经有了这样的关注关系\n");
			}
		}
	}
}

//delete following relationship between two users
void delRelation() {
	printf("请输入主动关注的用户的用户名:");
	char * followUserName = (char *)malloc(sizeof(char) * 50);
	fgetsNoN(followUserName, 50, stdin);

	UNode * followUNode = getUNodeFromName_U(followUserName, uRoot);
	if (!followUNode) {
		printInfo(3);
	} else {
		printf("请输入被关注的用户的用户名:");
		char * followedUserName = (char *)malloc(sizeof(char) * 50);
		fgetsNoN(followedUserName, 50, stdin);
		UNode * followedUNode = getUNodeFromName_U(followedUserName, uRoot);
		if (!followedUNode) {
			printInfo(3);
		} else {
			deleteAVL_F(followUNode->info->name, followedUNode->followed);
			deleteAVL_F(followedUNode->info->name, followUNode->following);
			printInfo(2);
		}
	}
}

//print all user's name when traverse whole AVL tree
void traversePrint(FNode * node){
	if(node==NULL) return;
	printf("%s , ",node->info->name);
	traversePrint(node->left);
	traversePrint(node->right);
}

//print all user's following
void showFollowing(){
	printf("请输入该用户的用户名:");
	char * userName = (char *)malloc(sizeof(char) * 50);
	fgetsNoN(userName, 50, stdin);

	UNode * getUNode = getUNodeFromName_U(userName, uRoot);
	if (!getUNode) {
		printInfo(3);
	} else {
		traversePrint(getUNode->following);
	}
}

//print all user's followed
void showFollowed(){
	printf("请输入该用户的用户名:");
	char * userName = (char *)malloc(sizeof(char) * 50);
	fgetsNoN(userName, 50, stdin);

	UNode * getUNode = getUNodeFromName_U(userName, uRoot);
	if (!getUNode) {
		printInfo(3);
	} else {
		traversePrint(getUNode->followed);
	}
}

//print all user's fans ( fans are both following and followed)
void showFriends(){
	printf("请输入该用户的用户名:");
	char * userName = (char *)malloc(sizeof(char) * 50);
	fgetsNoN(userName, 50, stdin);

	UNode * getUNode = getUNodeFromName_U(userName, uRoot);
	if (!getUNode) {
		printInfo(3);
	} else {
		FNode * frinedsRoot = getSetIntersec(getUNode->following, getUNode->followed);//BOTH
		if(frinedsRoot){
			traversePrint(frinedsRoot);
		}else{
			printInfo(4);
		}

	}
}

//print second friends
void showSecondFriend(){
	printf("请输入该用户的用户名:");
	char * userName = (char *)malloc(sizeof(char) * 50);
	fgetsNoN(userName, 50, stdin);

	UNode * getUNode = getUNodeFromName_U(userName, uRoot);
	if (!getUNode) {
		printInfo(3);
	} else {
		FNode * secFriRoot = getSecFriend(getUNode);
		if(secFriRoot){
			traversePrint(secFriRoot);
		}else{
			printInfo(4);
		}
	}
}

void showCommonFollow(){
    printf("请输入主动关注的用户的用户名:");
	char * followUserName = (char *)malloc(sizeof(char) * 50);
	fgetsNoN(followUserName, 50, stdin);

	UNode * followUNode = getUNodeFromName_U(followUserName, uRoot);
	if (!followUNode) {
		printInfo(3);
	} else {
		printf("请输入被关注的用户的用户名:");
		char * followedUserName = (char *)malloc(sizeof(char) * 50);
		fgetsNoN(followedUserName, 50, stdin);
		UNode * followedUNode = getUNodeFromName_U(followedUserName, uRoot);
		if (!followedUNode) {
            FNode * comFollows = getSetIntersec(followedUNode->following,followUNode->following);
            if (comFollows){
                traversePrint(comFollows);
            }else{
                printInfo(4);
            }
		} else {
            printInfo(3);
		}
	}

}

void showCommonFollowed(){
    printf("请输入主动关注的用户的用户名:");
	char * followUserName = (char *)malloc(sizeof(char) * 50);
	fgetsNoN(followUserName, 50, stdin);

	UNode * followUNode = getUNodeFromName_U(followUserName, uRoot);
	if (!followUNode) {
		printInfo(3);
	} else {
		printf("请输入被关注的用户的用户名:");
		char * followedUserName = (char *)malloc(sizeof(char) * 50);
		fgetsNoN(followedUserName, 50, stdin);
		UNode * followedUNode = getUNodeFromName_U(followedUserName, uRoot);
		if (!followedUNode) {
            FNode * comFollows = getSetIntersec(followedUNode->followed,followUNode->followed);
            if (comFollows){
                traversePrint(comFollows);
            }else{
                printInfo(4);
            }
		} else {
            printInfo(3);
		}
	}

}

int main() {
	uRoot = NULL;//init
	int op = 1;

	while (op) {
		system("cls");//clean content on screen
		op = printMenu();

		switch (op) {
		case 1:
			addUser();
			break;
		case 2:
			delUser();
			break;
		case 3:
			changeUser();
			break;
		case 4:
			showUser();
			break;
		case 5:
			addRelation();
			break;
		case 6:
			delRelation();
			break;
		case 7:
			showFollowing();
			break;
		case 8:
			showFollowed();
			break;
		case 9:
			showFriends();
			break;
		case 10:
			showSecondFriend();
			break;
		case 11:
            showCommonFollow();
			break;
		case 12:
			break;
		case 13:
			break;
		case 0:
			break;
		}
		system("pause");
	}
    return 0;
}

char *fgetsNoN(char *buf, int bufsize, FILE *stream) {
	char * returnP = fgets(buf, bufsize, stream);
	int i = 0;
	while (buf[i] != '\n') {
		i++;
	}
	buf[i] = '\0';
	return returnP;
}
