#include <IO.h>
#include <windows.h>
#include <wincon.h>
#include <windef.h>
#include "shlobj.h"
#include <wingdi.h>
#define NULLCODE 90017

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

void initConsole(){
    system("mode con:cols=100 lines=30");
    system("color 3B");
    SetConsoleTitle("基于AVL的SNS社交网络模型系统");
}

int printMenu() {
	int op = 90071;

	printf("---------------这是一个基于AVL自平衡树的SNS社交网络模型系统----------------\n\n\n");

	printf("\t1.添加新用户\t\t\t\t2.删除用户\n");
	printf("\t3.修改用户信息\t\t\t\t4.查询用户信息\n");
	printf("\t5.向用户添加新的关注关系\t\t6.删除用户的关注关系\n\n");

	printf("\t7.查找用户的所有关注\t\t\t8.查找用户的所有粉丝\n");
	printf("\t9.查找用户的所有好友\t\t\t10.查找用户的所有二度好友\n");
	printf("\t11.查找用户共同喜好的其他用户\n\n");

	printf("\t12.查找任意两用户的共同关注\t\t13.查找任意两用户的共同好友\n");
	printf("\t14.查找任意两用户的共同粉丝\t\t15.查找任意两用户的所有好友\n");
	printf("\t16.查找任意两用户的独有好友\n\n");

	printf("\t17.保存系统\t\t\t\t18.读取系统\n\n");

	printf("\t0.退出系统\n\n");

	printf("---------------------------------------------------------------------------\n");
	scanf("%d%*c", &op);

	return op;
}

//add a new user
void addUser() {

	printf("请输入该用户的用户名（不可与已有用户相同）:");
	char * newUserName = (char *)malloc(sizeof(char) * 30);
	fgetsNoN(newUserName, 30, stdin);

	UNode * check = getUNodeFromName_U(newUserName, uRoot);
	if (check) {
		printf("此用户名已存在。\n");
	} else {
		printf("请输入该用户的年龄:");
		int age = 0; scanf("%d%*c", &age);
		int sex = 0;
		while (sex != 1 && sex != 2) {
			printf("请输入数字选择该用户的性别（1->男 2->女）");
			scanf("%d%*c", &sex);
		}

		printf("请输入用户的喜好:");
		char * newhobby = (char *)malloc(sizeof(char) * 10);
		fgetsNoN(newhobby, 10, stdin);

		Info * newInfo = (Info *)malloc(sizeof(Info));
		newInfo->age = age;
		newInfo->sex = sex;
		newInfo->name = newUserName;
		newInfo->hobby = newhobby;
		uRoot = insertAVL_U(newInfo, uRoot);

		if (uRoot) printInfo(2);
		else printf("failed!\n");
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
	free(userName);

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
		while (select != 1 && select != 2 && select != 3) {
			printf("请输入数字来选择你想要修改的属性\n\t 1->年龄  2->性别 3->喜好:");
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
		} else if (select == 3) {
			printf("请输入该用户新的喜好：");
			fgetsNoN(getUNode->info->hobby, 10, stdin);
		}
		printf("修改成功\n");
	}
	free(userName);
}

//show all infomation of a specified user
void showUser() {
	printf("请输入该用户的用户名:");
	char * userName = (char *)malloc(sizeof(char) * 50);
	fgetsNoN(userName, 50, stdin);

	UNode * getUNode = getUNodeFromName_U(userName, uRoot);
	if (!getUNode) {
		printInfo(3);
	} else {
		Info * info = getUNode->info;
		printf("用户名:%s ; 年龄:%d ; 爱好:%s ; 性别:", info->name, info->age, info->hobby);
		if (info->sex == 1) {
			printf("男\n");
		} else {//sex == 2
			printf("女\n");
		}
	}
	free(userName);
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
		free(followedUserName);
	}
	free(followUserName);
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
		free(followedUserName);
	}
	free(followUserName);
}

//print all user's name when traverse whole AVL tree
void traversePrint(FNode * node) {
	if (node == NULL) return;
	printf("%s , ", node->info->name);
	traversePrint(node->left);
	traversePrint(node->right);
}

//print all user's following
void showFollowing() {
	printf("请输入该用户的用户名:");
	char * userName = (char *)malloc(sizeof(char) * 30);
	fgetsNoN(userName, 30, stdin);

	UNode * getUNode = getUNodeFromName_U(userName, uRoot);
	if (!getUNode) {
		printInfo(3);
	} else {
	    if(getUNode->following){
            printf("--------以下为该用户的所有关注--------\n");
            traversePrint(getUNode->following); printf("\n");
	    }else{
            printInfo(4);
	    }
	}
	free(userName);
}

//print all user's followed
void showFollowed() {
	printf("请输入该用户的用户名:");
	char * userName = (char *)malloc(sizeof(char) * 30);
	fgetsNoN(userName, 30, stdin);

	UNode * getUNode = getUNodeFromName_U(userName, uRoot);
	if (!getUNode) {
		printInfo(3);
	} else {

		if(getUNode->followed){
            printf("--------以下为该用户的所有粉丝--------\n");
            traversePrint(getUNode->followed); printf("\n");
		}else{
            printInfo(4);
		}

	}
	free(userName);
}

//print all user's fans ( fans are both following and followed)
void showFriends() {
	printf("请输入该用户的用户名:");
	char * userName = (char *)malloc(sizeof(char) * 30);
	fgetsNoN(userName, 30, stdin);

	UNode * getUNode = getUNodeFromName_U(userName, uRoot);
	if (!getUNode) {
		printInfo(3);
	} else {
		FNode * friendsRoot = getSetIntersec(getUNode->following, getUNode->followed);//BOTH
		if (!friendsRoot) {
			printf("--------以下为该用户的所有好友--------\n");
			traversePrint(friendsRoot); printf("\n");
			destroyAVL_F(friendsRoot);
		} else {
			printInfo(4);
		}

	}
	free(userName);
}

//print second friends
void showSecondFriend() {
	printf("请输入该用户的用户名:");
	char * userName = (char *)malloc(sizeof(char) * 30);
	fgetsNoN(userName, 30, stdin);

	UNode * getUNode = getUNodeFromName_U(userName, uRoot);
	if (!getUNode) {
		printInfo(3);
	} else {
		FNode * secFriRoot = getSecFriend(getUNode);
		if (secFriRoot) {
			printf("--------以下为该用户的二度好友--------\n");
			traversePrint(secFriRoot); printf("\n");
			destroyAVL_F(secFriRoot);
		} else {
			printInfo(4);
		}
	}
	free(userName);
}

//show common following of two specified users
void showCommonFollowing() {
	printf("请输入第一个用户的用户名:");
	char * userNameA = (char *)malloc(sizeof(char) * 30);
	fgetsNoN(userNameA, 30, stdin);

	UNode * uNodeA = getUNodeFromName_U(userNameA, uRoot);
	if (!uNodeA) {
		printInfo(3);
	} else {
		printf("请输入第二个用户的用户名:");
		char * userNameB = (char *)malloc(sizeof(char) * 30);
		fgetsNoN(userNameB, 30, stdin);
		UNode * uNodeB = getUNodeFromName_U(userNameB, uRoot);
		if (!uNodeB) {
			FNode * comFollowing = getSetIntersec(uNodeB->following, uNodeA->following);
			if (comFollowing) {
				printf("--------以下为他们的共同关注--------\n");
				traversePrint(comFollowing); printf("\n");
				destroyAVL_F(comFollowing);
			} else {
				printInfo(4);
			}
		} else {
			printInfo(3);
		}
		free(userNameB);
	}
	free(userNameA);

}

//show common followed of two specified users
void showCommonFollowed() {
	printf("请输入第一个用户的用户名:");
	char * userNameA = (char *)malloc(sizeof(char) * 30);
	fgetsNoN(userNameA, 30, stdin);

	UNode * uNodeA = getUNodeFromName_U(userNameA, uRoot);
	if (!uNodeA) {
		printInfo(3);
	} else {
		printf("请输入第二个用户的用户名:");
		char * userNameB = (char *)malloc(sizeof(char) * 30);
		fgetsNoN(userNameB, 30, stdin);
		UNode * uNodeB = getUNodeFromName_U(userNameB, uRoot);
		if (!uNodeB) {
			FNode * comFollowed = getSetIntersec(uNodeB->followed, uNodeA->followed);
			if (comFollowed) {
				printf("--------以下为他们的共同粉丝--------\n");
				traversePrint(comFollowed); printf("\n");
				destroyAVL_F(comFollowed);
			} else {
				printInfo(4);
			}
		} else {
			printInfo(3);
		}
		free(userNameB);
	}
	free(userNameA);

}

//show common friends of two specified users
void showCommonFriends() {
	printf("请输入第一个用户的用户名:");
	char * userNameA = (char *)malloc(sizeof(char) * 30);
	fgetsNoN(userNameA, 30, stdin);

	UNode * uNodeA = getUNodeFromName_U(userNameA, uRoot);
	if (!uNodeA) {
		printInfo(3);
	} else {
		printf("请输入第二个用户的用户名:");
		char * userNameB = (char *)malloc(sizeof(char) * 30);
		fgetsNoN(userNameB, 30, stdin);
		UNode * uNodeB = getUNodeFromName_U(userNameB, uRoot);
		if (!uNodeB) {
			FNode * friRootA = getSetIntersec(uNodeA->followed, uNodeA->following);
			FNode * friRootB = getSetIntersec(uNodeB->followed, uNodeB->following);
			FNode * comFriRoot = getSetIntersec(friRootA, friRootB);
			destroyAVL_F(friRootA); destroyAVL_F(friRootB);//clear
			if (comFriRoot) {
				printf("--------以下为他们的所有好友--------\n");
				traversePrint(comFriRoot); printf("\n");
				destroyAVL_F(comFriRoot);
			} else {
				printInfo(4);
			}
		} else {
			printInfo(3);
		}
		free(userNameB);
	}
	free(userNameA);
}

//show friends-union of two specified users
void showUnionFriends() {
	printf("请输入第一个用户的用户名:");
	char * userNameA = (char *)malloc(sizeof(char) * 30);
	fgetsNoN(userNameA, 30, stdin);

	UNode * uNodeA = getUNodeFromName_U(userNameA, uRoot);
	if (!uNodeA) {
		printInfo(3);
	} else {
		printf("请输入第二个用户的用户名:");
		char * userNameB = (char *)malloc(sizeof(char) * 30);
		fgetsNoN(userNameB, 30, stdin);
		UNode * uNodeB = getUNodeFromName_U(userNameB, uRoot);
		if (!uNodeB) {
			FNode * friRootA = getSetIntersec(uNodeA->followed, uNodeA->following);
			FNode * friRootB = getSetIntersec(uNodeB->followed, uNodeB->following);
			FNode * unionFriRoot = getSetUnion(friRootA, friRootB);
			destroyAVL_F(friRootA); destroyAVL_F(friRootB);//clear
			if (unionFriRoot) {
				printf("--------以下为他们的所有好友--------\n");
				traversePrint(unionFriRoot); printf("\n");
				destroyAVL_F(unionFriRoot);
			} else {
				printInfo(4);
			}
		} else {
			printInfo(3);
		}
		free(userNameB);
	}
	free(userNameA);
}

//show one's Own Friends
void showOwnFriends() {
	printf("请输入第一个用户的用户名:");
	char * userNameA = (char *)malloc(sizeof(char) * 30);
	fgetsNoN(userNameA, 30, stdin);

	UNode * uNodeA = getUNodeFromName_U(userNameA, uRoot);
	if (!uNodeA) {
		printInfo(3);
	} else {
		printf("请输入第二个用户的用户名:");
		char * userNameB = (char *)malloc(sizeof(char) * 30);
		fgetsNoN(userNameB, 30, stdin);
		UNode * uNodeB = getUNodeFromName_U(userNameB, uRoot);
		if (!uNodeB) {
			FNode * friRootA = getSetIntersec(uNodeA->followed, uNodeA->following);
			FNode * friRootB = getSetIntersec(uNodeB->followed, uNodeB->following);
			FNode * ownFriRootA = getSetDifference(friRootA, friRootB);
			FNode * ownFriRootB = getSetDifference(friRootB, friRootA);
			destroyAVL_F(friRootA); destroyAVL_F(friRootB);//clear

														   //output
			printf("--------以下为%s的独有好友--------\n", userNameA);
			if (ownFriRootA) {
				traversePrint(ownFriRootA); printf("\n");
				destroyAVL_F(ownFriRootA);
			} else {
				printInfo(4);
			}

			printf("\n--------以下为%s的独有好友--------\n", userNameB);
			if (ownFriRootB) {
				traversePrint(ownFriRootB); printf("\n");
				destroyAVL_F(ownFriRootB);
			} else {
				printInfo(4);
			}
		} else {
			printInfo(3);
		}
		free(userNameB);
	}
	free(userNameA);
}

/*---------------save system---------- */
//without sequence
void saveUser(UNode * node, FILE * file){
    if(!node) return;
    Info * info = node->info;
    fprintf(file, "%s;%s;%d;%d\n", info->name, info->hobby, info->age, info->sex);

    saveUser(node->left, file);
    saveUser(node->right, file);
}

//for all users' relationships( here is following tree)
void saveRelation_2(FNode * node, FILE * file, char * following){
    if (!node) return;

    fprintf(file, "%s;%s\n",following, node->info->name);//latter one is the followed

    saveRelation_2(node->left, file, following);
    saveRelation_2(node->right, file, following);
}
//this is for all relationships of one user
void saveRelation_1(UNode * node, FILE * file){
    if(!node) return;

    saveRelation_2(node->following, file, node->info->name);

    saveRelation_1(node->left, file);
    saveRelation_1(node->right, file);
}

void saveSystem() {
    printf("请输入存档名（有重名将覆盖原存档）:");
	char * fileName = (char *)malloc(sizeof(char) * 30);
	char * path = (char *)malloc(sizeof(char) * 35);
	fgetsNoN(fileName, 30, stdin);

    strcpy(path, fileName);
    strcat(path, ".u");
	FILE * fileU = fopen(path, "w+");
	if (!fileU) {
		printInfo(1);
	} else {
        strcpy(path, fileName);
        strcat(path, ".r");
        FILE * fileR = fopen(path , "w+");//to save relationship
        saveUser(uRoot, fileU);
        saveRelation_1(uRoot, fileR);

        //close file
        fclose(fileU);
        fclose(fileR);
        printInfo(2);
	}
}

/*-------------Load System-------------*/
void cleanRelation(UNode * node) {
	if (!node) return;
	destroyAVL_F(node->followed);
	destroyAVL_F(node->following);
}

//load whole system from disk
void loadSystem() {

    printf("请输入存档名:");
	char * fileName = (char *)malloc(sizeof(char) * 30);
	char * path = (char *)malloc(sizeof(char)*35);
	fgetsNoN(fileName, 30, stdin);
	strcpy(path, fileName);
	strcat(path, ".u");
	FILE * fileU = fopen(path, "r+");
	if (!fileU) {
		printf("无以此为名的存档\n");
	} else {
	    //clean cache
        cleanRelation(uRoot);
        destroyAVL_U(uRoot);
        uRoot = NULL;

        uRoot = getUsers(fileU);
        strcpy(path, fileName);
        strcat(path, ".r");
        FILE * fileR = fopen(path, "r+");
        getRelation(uRoot, fileR);

        fclose(fileU);
        fclose(fileR);

        //TODO: update heights!!!!

        printInfo(2);
	}
}

//show those users having same hobby
void showSameHobby(){
    printf("请输入该用户的用户名:");
	char * userName = (char *)malloc(sizeof(char) * 30);
	fgetsNoN(userName, 30, stdin);

	UNode * getUNode = getUNodeFromName_U(userName, uRoot);
	if (!getUNode) {
		printInfo(3);
	} else {
        FNode * resultNode = getSameHobbyUsers(getUNode, uRoot);
        if(resultNode){
            printf("以下为所有相同喜好的用户名单:\n");
            traversePrint(resultNode);printf("\n");
            destroyAVL_F(resultNode);
        }else{
            printInfo(4);
        }
	}
	free(userName);
}

int main() {
	uRoot = NULL;//init
	int op = 1;
    initConsole();
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
            showSameHobby();
            break;
		case 12:
			showCommonFollowing();
			break;
		case 13:
			showCommonFriends();
			break;
		case 14:
			showCommonFollowed();
			break;
		case 15:
			showUnionFriends();
			break;
		case 16:
			showOwnFriends();
			break;
		case 17:
			saveSystem();
			break;
		case 18:
			loadSystem();
			break;
		case 0:
			printf("欢迎下次使用!\n");
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
