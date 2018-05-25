#include <stdio.h>
#include <stdlib.h>

//structrue 정의
typedef struct _person* ptrPerson;
typedef struct _person{
	ptrPerson father;
	ptrPerson mother;
	char name;
}person_node;
typedef struct _stack* ptrStack;
typedef struct _stack {
	ptrStack next;
	char data;
}stack_node;

//method 정의
void question();
void pushInTree();
ptrPerson initPerson();
void reverseStack();
void path();
void showTree();
void _pushInTree();
void changeFirst();
int checkInPedigree();
ptrStack initStackNode();
void push();
char pop();
void printStack();
void stackCopy();

//main method
int main(vooid) {
	//기본 변수 초기화
	//first : tree의 가장 위를 가르키는(binary tree의 head) 노드 포인터
	//person : 임시 공간
	//input : 입력 받는 문자열
	ptrPerson first=NULL;
	char input[10];

	//스택 정의(Tree 내부에서 sub tree를 구하기 위해서 사용)
	//head가 존재하고, head가 stacklinked list로 구현
	ptrStack stackPath = (ptrStack)malloc(sizeof(stack_node));
	stackPath->data = '0';
	stackPath->next = stackPath;

	//입력값이 0이면 종료하며, 그 전까진 입력된 값에 따라 출력을 다르게함.
	while (input[0] != '0') {
		//입력을 받는 부분.
		printf(">>");
		fgets(input, 10, stdin);
		//질문의 경우
		if (input[2] == '?') {
			question(first, input, stackPath);
		}
		//F, M을 입력하여tree를 입력하는 경우
		else if(input[2] == 'F' || input[2] == 'M'){
			pushInTree(&first, input);
		}
		//나머지 경우 : 입력 오류
		else {
			printf("Enter value.\n");
		}
	}
	return 0;
}
void pushInTree(ptrPerson* first, char* input) {
	//tree에 input에 알맞은 값을 push함.
	if (!(*first)) {
		//tree에 값이 하나도 없다면 새로운 head를 만듬.
		(*first) = initPerson(input[0]);
	}
	if (checkInPedigree((*first), input[0])) {
		//leaf를 추가하는 재귀함수.
		_pushInTree((*first), input, initPerson(input[4]));
		showTree(*first);
	}
	else if (checkInPedigree(*first, input[4])) {
		//leaf가 아닌, head를 바꾸어야 할때 쓰는 함수.
		changeFirst(first, input);
		showTree(*first);
	}
	printf("\n");
}
ptrPerson initPerson(char name) {
	//tree의 새로운 노드를 생성한뒤 주소를 반환함. default : (NULL,name,NULL)
	ptrPerson people;
	people = (ptrPerson)malloc(sizeof(person_node));
	people->mother = NULL;
	people->father = NULL;
	people->name = name;
	return people;
}
void showTree(ptrPerson first) {
	//preorder방법을 사용하여 트리의 모든 노드를 탐색, 출력함.
	if (first) {
		printf("%c", first->name);
		showTree(first->father);
		showTree(first->mother);
	}
}
void _pushInTree(ptrPerson first, char* input, ptrPerson parent) {
	//tree의 leaf를 추가하는 method, input[0](child)의 input[2](FM)에 따라 위치가 결정된다.
	if (first) {
		if (first->name == input[0]) {
			if (input[2] == 'F') {
				first->father = parent;
				return;
			}
			else if (input[2] == 'M') {
				first->mother = parent;
				return;
			}
		}
		else {
			//재귀젹
			_pushInTree(first->father, input, parent);
			_pushInTree(first->mother, input, parent);
		}
	}
	return;
}
void changeFirst(ptrPerson* first, char* input) {
	//tree의 head를 바꾸는method.
	ptrPerson person = initPerson(input[0]);
	if (input[2] == 'F') {
		person->father = *first;
	}
	else if (input[2] == 'M'){
		person->mother = *first;
	}
	*first = person;
}
int checkInPedigree(ptrPerson people,char name) {
	//name으로 주어진 인자가 tree내부에 존재하는지 묻는 method 존재하면1, 없으면0을 반환.
	if (people) {
		if (name == people->name) {
			return 1;
		}
		else if (checkInPedigree(people->father, name)) {
			return 1;
		}
		else if (checkInPedigree(people->mother, name)) {
			return 1;
		}
		
	}
	return 0;
}
void question(ptrPerson first, char* input, ptrStack stackPath) {
	//입력값이 (value-?-value)일때 value간 상관관계를 찾아서 출력하는 method

	//stack처리를 돕는 stack pointer temp1, temp2
	ptrStack temp1 = NULL;
	ptrStack temp2 = NULL;

	//tree내 탐색 경로를 알아내기 위해 stack을 사용하였다.
	ptrStack pathToLeft = (ptrStack)malloc(sizeof(stack_node));
	pathToLeft->next = pathToLeft;
	pathToLeft->data = '0';
	ptrStack pathToRight = (ptrStack)malloc(sizeof(stack_node));
	pathToRight->next = pathToRight;
	pathToRight->data = '0';
	//tree의 first 부터 value까지의 경로를 탐색, 스택값을 저장해 stackRetMem에 저장.
	path(first, input[0], stackPath, &pathToLeft);
	path(first, input[4], stackPath, &pathToRight);

	//value1에서 value까지의 경로 = reverse(value1) + reverse(value2)에서
	//중복된 부분삭제 + value1까지 경로를 FM이 아닌 SDC로 표현한다.

	//value까지의 경로를 SDC로 표현하기 위한 부분이다.
	reverseStack(&pathToLeft);
	if (pathToLeft->next->next != pathToLeft) {
		while (pathToLeft->next->next->data != '0') {
			pathToLeft = pathToLeft->next->next;
			if (pathToLeft->next->next->data == 'F') {
				pathToLeft->data = 'S';
			}
			if (pathToLeft->next->next->data == 'M') {
				pathToLeft->data = 'D';
			}
		}
		pathToLeft->data = 'C';
		pathToLeft = pathToLeft->next->next;
	}
	reverseStack(&pathToLeft);

	//value1, value2까지의 중복된 경로를 지운다.
	//이 과정을 먼저 시행하면 SDC의 표현으로 바꾸기 힘들어 위 과정을 먼저 실행하였다.
	while (pathToLeft->next->next->next->data == pathToRight->next->next->next->data) {
		temp1 = pathToLeft->next;
		temp2 = temp1->next;
		pathToLeft->next = temp2->next;
		free(temp1);
		free(temp2);
		temp1 = pathToRight->next;
		temp2 = temp1->next;
		pathToRight->next = temp2->next;
		free(temp1);
		free(temp2);
	}

	//value1 까지의 경로와 value2까지의 경로를 합치는 부분. 결과는 pathToLeft에 저장.
	reverseStack(&pathToLeft);
	temp1 = pathToLeft;
	temp2 = pathToRight;
	while (temp1->next->next != pathToLeft) {
		temp1 = temp1->next;
	}
	while (temp2->next != pathToRight) {
		temp2 = temp2->next;
	}
	temp1->next = pathToRight->next;
	temp2->next = pathToLeft;

	//결과 출력
	printStack(pathToLeft);
}
void path(ptrPerson child, char parent, ptrStack stackPath, ptrStack* stackRet) {
	//child(서브 트리의 head)부터 parent(찾는 노드의 data)까지의 경로를 stackRet에 담는다.
	if (child) {
		push(stackPath, child->name);
		if (child->name == parent) {
			//경로를 찾았을 때의 stack상황을 stackRet에 저장하여 쓸수 있도록 한다.
			stackCopy(stackPath, *stackRet);
			pop(stackPath);
		}
		else {
			//Father의 경우 F를 push
			push(stackPath, 'F');
			path(child->father, parent, stackPath, stackRet);
			pop(stackPath);
			//Mother의 경우 M을 push
			push(stackPath, 'M');
			path(child->mother, parent, stackPath, stackRet);
			pop(stackPath);
			pop(stackPath);
		}
	}
}
ptrStack initStackNode(char data) {
	//stack의 data를 받아 새로운 stack node를 생성하여 주소를 반환한다.
	ptrStack node;
	node = (ptrStack)malloc(sizeof(stack_node));
	node->next = NULL;
	node->data = data;
	return node;
}
void push(ptrStack stack, char data) {
	//구현한 stack의 push함수.
	ptrStack temp;
	temp = (ptrStack)malloc(sizeof(stack_node));
	temp->data = data;
	temp->next = stack;
	while (stack->next != temp->next) {
		stack = stack->next;
	}
	stack->next = temp;
}
char pop(ptrStack stack) {
	//구현한 stack의 pop함수.
	ptrStack temp = stack;
	ptrStack prev;
	char data;
	if (stack->next != stack) {
		while (temp->next->next != stack) {
			temp = temp->next;
		}
		prev = temp;
		temp = temp->next;

		prev->next = stack;
		data = temp->data;
		free(temp);
	}
	else {
		printf("no data in stack");
	}
	return data;
}
void printStack(ptrStack stack) {
	//현재 stack의 내용을 출력하는 method
	ptrStack temp = stack->next;
	while (temp != stack) {
		printf("%c", temp->data);
		temp = temp->next;
		if (temp != stack) {
			printf("-");
		}
	}
	printf("\n");
}
void stackCopy(ptrStack stackPath, ptrStack stackRet) {
	//stackPath로 받은 스택을 stackRet에 저장한다.
	stackPath = stackPath->next;
	while (stackPath->data != '0') {
		push(stackRet, stackPath->data);
		stackPath = stackPath->next;
	}
}
void reverseStack(ptrStack* stack) {
	//stack을 반전시킨다.
	ptrStack temp0 = (*stack);
	ptrStack temp1 = temp0->next;
	ptrStack temp2 = temp1->next;

	do {
		temp1->next = temp0;
		temp0 = temp1;
		temp1 = temp2;
		temp2 = temp2->next;
	} while (temp0 != (*stack));

}