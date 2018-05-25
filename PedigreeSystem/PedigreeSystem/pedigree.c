#include <stdio.h>
#include <stdlib.h>

//structrue ����
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

//method ����
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
	//�⺻ ���� �ʱ�ȭ
	//first : tree�� ���� ���� ����Ű��(binary tree�� head) ��� ������
	//person : �ӽ� ����
	//input : �Է� �޴� ���ڿ�
	ptrPerson first=NULL;
	char input[10];

	//���� ����(Tree ���ο��� sub tree�� ���ϱ� ���ؼ� ���)
	//head�� �����ϰ�, head�� stacklinked list�� ����
	ptrStack stackPath = (ptrStack)malloc(sizeof(stack_node));
	stackPath->data = '0';
	stackPath->next = stackPath;

	//�Է°��� 0�̸� �����ϸ�, �� ������ �Էµ� ���� ���� ����� �ٸ�����.
	while (input[0] != '0') {
		//�Է��� �޴� �κ�.
		printf(">>");
		fgets(input, 10, stdin);
		//������ ���
		if (input[2] == '?') {
			question(first, input, stackPath);
		}
		//F, M�� �Է��Ͽ�tree�� �Է��ϴ� ���
		else if(input[2] == 'F' || input[2] == 'M'){
			pushInTree(&first, input);
		}
		//������ ��� : �Է� ����
		else {
			printf("Enter value.\n");
		}
	}
	return 0;
}
void pushInTree(ptrPerson* first, char* input) {
	//tree�� input�� �˸��� ���� push��.
	if (!(*first)) {
		//tree�� ���� �ϳ��� ���ٸ� ���ο� head�� ����.
		(*first) = initPerson(input[0]);
	}
	if (checkInPedigree((*first), input[0])) {
		//leaf�� �߰��ϴ� ����Լ�.
		_pushInTree((*first), input, initPerson(input[4]));
		showTree(*first);
	}
	else if (checkInPedigree(*first, input[4])) {
		//leaf�� �ƴ�, head�� �ٲپ�� �Ҷ� ���� �Լ�.
		changeFirst(first, input);
		showTree(*first);
	}
	printf("\n");
}
ptrPerson initPerson(char name) {
	//tree�� ���ο� ��带 �����ѵ� �ּҸ� ��ȯ��. default : (NULL,name,NULL)
	ptrPerson people;
	people = (ptrPerson)malloc(sizeof(person_node));
	people->mother = NULL;
	people->father = NULL;
	people->name = name;
	return people;
}
void showTree(ptrPerson first) {
	//preorder����� ����Ͽ� Ʈ���� ��� ��带 Ž��, �����.
	if (first) {
		printf("%c", first->name);
		showTree(first->father);
		showTree(first->mother);
	}
}
void _pushInTree(ptrPerson first, char* input, ptrPerson parent) {
	//tree�� leaf�� �߰��ϴ� method, input[0](child)�� input[2](FM)�� ���� ��ġ�� �����ȴ�.
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
			//��͠�
			_pushInTree(first->father, input, parent);
			_pushInTree(first->mother, input, parent);
		}
	}
	return;
}
void changeFirst(ptrPerson* first, char* input) {
	//tree�� head�� �ٲٴ�method.
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
	//name���� �־��� ���ڰ� tree���ο� �����ϴ��� ���� method �����ϸ�1, ������0�� ��ȯ.
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
	//�Է°��� (value-?-value)�϶� value�� ������踦 ã�Ƽ� ����ϴ� method

	//stackó���� ���� stack pointer temp1, temp2
	ptrStack temp1 = NULL;
	ptrStack temp2 = NULL;

	//tree�� Ž�� ��θ� �˾Ƴ��� ���� stack�� ����Ͽ���.
	ptrStack pathToLeft = (ptrStack)malloc(sizeof(stack_node));
	pathToLeft->next = pathToLeft;
	pathToLeft->data = '0';
	ptrStack pathToRight = (ptrStack)malloc(sizeof(stack_node));
	pathToRight->next = pathToRight;
	pathToRight->data = '0';
	//tree�� first ���� value������ ��θ� Ž��, ���ð��� ������ stackRetMem�� ����.
	path(first, input[0], stackPath, &pathToLeft);
	path(first, input[4], stackPath, &pathToRight);

	//value1���� value������ ��� = reverse(value1) + reverse(value2)����
	//�ߺ��� �κл��� + value1���� ��θ� FM�� �ƴ� SDC�� ǥ���Ѵ�.

	//value������ ��θ� SDC�� ǥ���ϱ� ���� �κ��̴�.
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

	//value1, value2������ �ߺ��� ��θ� �����.
	//�� ������ ���� �����ϸ� SDC�� ǥ������ �ٲٱ� ����� �� ������ ���� �����Ͽ���.
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

	//value1 ������ ��ο� value2������ ��θ� ��ġ�� �κ�. ����� pathToLeft�� ����.
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

	//��� ���
	printStack(pathToLeft);
}
void path(ptrPerson child, char parent, ptrStack stackPath, ptrStack* stackRet) {
	//child(���� Ʈ���� head)���� parent(ã�� ����� data)������ ��θ� stackRet�� ��´�.
	if (child) {
		push(stackPath, child->name);
		if (child->name == parent) {
			//��θ� ã���� ���� stack��Ȳ�� stackRet�� �����Ͽ� ���� �ֵ��� �Ѵ�.
			stackCopy(stackPath, *stackRet);
			pop(stackPath);
		}
		else {
			//Father�� ��� F�� push
			push(stackPath, 'F');
			path(child->father, parent, stackPath, stackRet);
			pop(stackPath);
			//Mother�� ��� M�� push
			push(stackPath, 'M');
			path(child->mother, parent, stackPath, stackRet);
			pop(stackPath);
			pop(stackPath);
		}
	}
}
ptrStack initStackNode(char data) {
	//stack�� data�� �޾� ���ο� stack node�� �����Ͽ� �ּҸ� ��ȯ�Ѵ�.
	ptrStack node;
	node = (ptrStack)malloc(sizeof(stack_node));
	node->next = NULL;
	node->data = data;
	return node;
}
void push(ptrStack stack, char data) {
	//������ stack�� push�Լ�.
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
	//������ stack�� pop�Լ�.
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
	//���� stack�� ������ ����ϴ� method
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
	//stackPath�� ���� ������ stackRet�� �����Ѵ�.
	stackPath = stackPath->next;
	while (stackPath->data != '0') {
		push(stackRet, stackPath->data);
		stackPath = stackPath->next;
	}
}
void reverseStack(ptrStack* stack) {
	//stack�� ������Ų��.
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