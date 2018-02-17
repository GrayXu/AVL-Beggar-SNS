#include <AvlCal.h>

UNode * uRoot;

char *fgetsNoN(char *buf, int bufsize, FILE *stream);

/*
Trick
1->系统堆空间满导致失败
*/
void printInfo(int mode) {
	switch (mode) {
	case 1:
		printf("系统堆空间满导致失败。\n");
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

void addUser() {
	UNode * newUser = (UNode *)malloc(sizeof(UNode));
	if (newUser) {
		printf("请输入该用户的用户名（不可与已有用户相同）:");
		char * newUserName = (char *)malloc(sizeof(char) * 50);
		fgetsNoN(newUserName, 50, stdin);
		
		UNode * check = getUNodeFromName_U(newUserName, uRoot);
		if (check) {
			printf("此用户名已存在，请重新输入");
		}

	} else {
		printInfo(1);
	}
}

int main() {
	uRoot = NULL;//init
	int op = 1;

	while (op) {
		op = printMenu();

		switch (op) {
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			break;
		case 8:
			break;
		case 9:
			break;
		case 10:
			break;
		case 11:
			break;
		case 12:
			break;
		case 13:
			break;
		case 0:
			break;
		}
		
	
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