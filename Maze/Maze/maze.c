/*	â��ICT�������� ��ǻ�Ͱ��к� ��ǻ�Ͱ��а� 20150864 ������
	���α׷� ����	:	�ڷᱸ�� 3.5�� ���α׷��� ������ ���� code�м��� �� Ȱ��
	���� �̸�		:	maze.c
	�Է°�			:	������ ������ �ִ�迭��(MAX_ROW,MAX_COL)�� �Ұ�����, ������ ������ �Ұ����� ����
						���Ž���� ����� å�� ���(����Ž��)���� �� ������ ������ ��������(����Ž��)���� �� ������ ����
	��°�			:	�־��� �̷�(MAZE), ������
						��� Ž���� ����� ��ȿ���(back_tracking �� ����)
						��ȿ����� ���� = Ž������� ����
	��¥			:	2018-04-10 ~ 2018-05-05
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//�迭 ������ ���� ��� ����
#define MAX_ROW 10
#define MAX_COL 10
#define MAX_STACK 10000


typedef enum { false, true } bool;					//boolean �ڷ��� ����
													
//����ü ����
typedef struct {									//���� ��ġ�� �˱����� offset ����
	int vert;
	int horiz;
}MOVES;
typedef struct {									//MAZE Ž���� ���� position ������ ���� element ����
	int row;
	int col;
	int dir;
}element;

//�Լ�����
void print_line();									//���� ���
void setup_maze();									//�־��� maze�� �Է�, �̸� bool �迭�� �������.
void init_maze();									//maze Ž�� �˰��� ���� Ž�� maze�� �־��� maze�� �Է��ϰ� �׵θ��� true�� ����
element get_next_pos();								//���� ��ġ�� ������ �޾� ���� ��ġ�� ����
void stack_push();									//stack������ push�Լ�
element stack_pop();								//stack������ pop�ռ�
void get_path();									//������ �˰������� ���Ž�� �� ��θ� ����ϴ� �Լ�
void get_path_rand_dir();							//dir�� ���������� �����޾� ���Ž�� �� ��θ� ����ϴ� �Լ�
void show_maze_value();								//�־��� maze�� ����ϴ� �Լ�
void stack_delete();								//stack�� �� �������� ���ִ� �Լ�
void show_stack();									//stack�� ������ ����ϴ� �Լ�
void show_exit();									//������ exit���� ����ϴ� �Լ�
void init_exit();									//exit���� �ʱ�ȭ �ϴ� �Լ�
void setup_move();									//move ���� å���� ���� ������ �ʱ�ȭ
void get_type_search();								//å�� ������� Ǯ������, ���������� Ǯ������ ����
bool is_one_step_exit();							//�־��� element�� exit���� ��ĭ ������ �ִ��� ã�� �Լ�

int main() {
	//�ֿ� ���� ���� �� �ʱ�ȭ
	bool given_maze[MAX_ROW][MAX_COL];
	bool maz[MAX_ROW + 2][MAX_COL + 2];
	element exit;
	element stack_ele[MAX_STACK];
	int* stack_top;
	int type_search;
	stack_top = (int*)malloc(sizeof(int));

	//Ž���� ���� offset move ���� �� �ʱ�ȭ
	MOVES move[8];
	setup_move(move);

	//�־��� MAZE�� �޴� �Լ�, maz�� �˰��򿡼� �� [MAX_ROW+2][MAX_COL+2]ũ��� ��ȯ
	setup_maze(given_maze);
	init_maze(maz, given_maze);

	//exit�� ����� (MAX_ROW,MAX_COL) �Ǵ� ������ �������� ���Ұ����� ���� exit�� ����
	init_exit(&exit, maz);
	get_type_search(&type_search);
	
	//maze value�� ������ exit�� ����ڿ��� �����ִ� �Լ�
	show_maze_value(maz);
	show_exit(exit);

	//type_search�� �Է°��� ���� �˸��� �Լ� ȣ��
	if (type_search) {
		get_path(maz, stack_ele, stack_top, move, exit);
	}
	else {
		get_path_rand_dir(maz, stack_ele, stack_top, move, exit);
	}
	return 0;
}

void print_line() {
	//�ܼ�â�� �����ϱ����� ���� ����Լ�
	printf("\n-----------------------------------------------------------------\n\n");
}
void init_maze(bool (*maz)[MAX_COL + 2], bool(*maze)[MAX_COL]) {
	//maze Ž�� �˰��� ���� Ž�� maze�� �־��� maze�� �Է��ϰ� �׵θ��� true�� ����
	for (int i = 0; i < MAX_ROW; i++) {
		for (int j = 0; j < MAX_COL; j++) {
			maz[i + 1][j + 1] = maze[i][j];
		}
	}
	for (int i = 0; i < MAX_ROW + 2; i++) {
		for (int j = 0; j < MAX_COL + 2; j++) {
			maz[0][j] = 1;
			maz[MAX_ROW + 1][j] = 1;
		}
		maz[i][0] = 1;
		maz[i][MAX_COL + 1] = 1;
	}


}
void setup_maze(bool(*given_maze)[MAX_COL]) {
	//maze�� �Է��ϴ� �Լ�. ������ ���ǿ� ���߾� ���� �Է��ϴ°��� �ƴ� �ڵ� ������ �־�����.
	int setup_maze[MAX_ROW][MAX_COL] = {
		{ 0,0,1,0,1,1,1,0,1,0 },
		{ 1,0,0,1,1,1,0,1,0,1 },
		{ 1,1,0,1,1,0,1,0,1,1 },
		{ 0,0,1,1,1,0,1,0,0,0 },
		{ 0,1,1,1,1,0,1,0,1,0 },
		{ 1,0,1,1,0,1,1,0,1,0 },
		{ 1,1,0,1,0,1,0,0,1,0 },
		{ 1,0,1,0,1,0,1,0,0,0 },
		{ 0,1,0,1,1,1,0,1,1,0 },
		{ 1,0,0,1,1,1,0,0,0,0 }
	};
	//�Է¹��� ���� �ٷ� given_maze �迭�� ����ִ´�.
	for (int i = 0; i < MAX_ROW; i++) {
		for (int j = 0; j < MAX_COL; j++) {
			given_maze[i][j] = (bool)setup_maze[i][j];
		}
	}
}
element get_next_pos(element cur_position, MOVES* move, int dir) {
	//���� ��ġ�� ������ �޾� ���� ��ġ�� ����
	element next_position;
	next_position.row = cur_position.row + move[dir].vert;
	next_position.col = cur_position.col + move[dir].horiz;
	next_position.dir = dir;
	return next_position;
}
void stack_push(element* a, int* top, element value1) {
	//stack������ push�Լ�
	if (*top > MAX_STACK) {
		printf("stack error");
	}
	(*top) += 1;
	a[(*top)] = value1;
}
element stack_pop(element* a, int* top) {
	//stack������ pop�ռ�
	element temp;
	temp = a[*top];
	if ((*top) == 0)
		(*top) = -1;
	else {
		(*top) -= 1;
	}
	return temp;
}
void get_path(bool (*maz)[MAX_COL + 2], element* stack_ele, int* stack_top, MOVES* move, element exit) {
	//������ �˰������� ���Ž�� �� ��θ� ����ϴ� �Լ�
	bool found = false;														//�������� ã�Ҵ���(�����ִ��� �������� �߿�x)
	element cur_pos = { 0,0,0 };
	element next_pos = { 0,0,0 };
	maz[1][1] = 1, *stack_top = 0;
	stack_ele[0].row = 1, stack_ele[0].col = 1, stack_ele[0].dir = 0;

	//��� Ž���� ���� �ݺ��Լ�
	while (*stack_top > -1 && found != true) {								//stack�� ����ְų�, �������� ã���� Ż��
		cur_pos = stack_pop(stack_ele, stack_top);
		while (cur_pos.dir < 8 && found != true) {		//���� ī��Ʈ�� �����̻��̰ų�, 8���� ���� ã�Ұų�, �������� ã���� Ż��
			next_pos = get_next_pos(cur_pos, move, cur_pos.dir);			//���� ��θ� ������
			if (is_one_step_exit(cur_pos, move, exit)) {					//�������� ã������ = ���� ��ġ�� ���������� 1ĭ �Ÿ��̸�
				found = true;												
				stack_push(stack_ele, stack_top, cur_pos);
			}
			else if (!maz[next_pos.row][next_pos.col]) {
				maz[next_pos.row][next_pos.col] = 1;					//�� �� �ִ°�� + �ѹ��� �Ȱ� ����̸�

				cur_pos.dir++;												
				stack_push(stack_ele, stack_top, cur_pos);
				cur_pos = next_pos;
				cur_pos.dir = 0;
			}
			else {
				++cur_pos.dir;
			}
		}
	}

	//��� Ž���� ���� stack�� push�Ǿ��� ���� ���
	if (maz[exit.row][exit.col] == 0 && found) {
		printf("the path is \n");
		printf("row   col\n");
		for (int i = 0; i <= (*stack_top); i++) {
			if (i % 5 == 0)
				printf("\n");
			printf("(%2d,%5d)", stack_ele[i].row, stack_ele[i].col);
		}
		printf("(%2d,%5d)\n", exit.row, exit.col);
		printf("position_count : %d", *stack_top + 2);
	}
	else {
		printf("The maze does not have a path\n\n");
	}
	print_line();
}
void get_path_rand_dir(bool (*maz)[MAX_COL + 2], element* stack_ele, int* stack_top, MOVES* move, element exit) {
	//dir�� ���������� �����޾� ���Ž�� �� ��θ� ����ϴ� �Լ�
	long error_count = 0;													//���� ��� Ž������ Ž������ ������ ���� ����
	bool found = false;
	element cur_pos = { 0,0,0 };
	element next_pos = { 0,0,0 };
	maz[1][1] = 1, *stack_top = 0;
	stack_ele[0].row = 1, stack_ele[0].col = 1, stack_ele[0].dir = ((int)(rand() % 8));
	while (*stack_top > -1 && found != true) {
		cur_pos = stack_pop(stack_ele, stack_top);
		error_count = 0;
		while (error_count < 100000 && found != true) {						//���� ī��Ʈ�� ������ �ö� = ���Ž���� ���� error�� �ɸ�
			next_pos = get_next_pos(cur_pos, move, cur_pos.dir);
			if (is_one_step_exit(cur_pos, move, exit)) {
				found = true;
				stack_push(stack_ele, stack_top, cur_pos);
			}
			else if (!maz[next_pos.row][next_pos.col]) {
				maz[next_pos.row][next_pos.col] = 1;

				cur_pos.dir = ((int)(rand() % 8));
				stack_push(stack_ele, stack_top, cur_pos);

				cur_pos = next_pos;
				cur_pos.dir = ((int)(rand() % 8));

			}
			else {
				cur_pos.dir = ((int)(rand() % 8));
				error_count++;												//Ž�� �ǽ� ���� �������� ã�ų�, ���� �ִ°�ΰ� �ƴ϶�� count ����
			}
		}
	}
	//��� Ž���� ���� stack�� push�Ǿ��� ���� ���
	if (maz[exit.row][exit.col] == 0 && found == true) {
		printf("the path is \n");
		printf("row   col\n");
		for (int i = 0; i <= (*stack_top); i++) {
			if (i % 5 == 0)
				printf("\n");
			printf("(%2d,%5d)  ", stack_ele[i].row, stack_ele[i].col);
		}
		printf("(%2d,%5d)\n", exit.row, exit.col);
		printf("position_count : %d", *stack_top + 2);
		
	}
	else {
		printf("The maze does not have a path\n\n");
	}
	print_line();
}
void show_maze_value(bool(*maz)[MAX_COL + 2]) {
	//�־��� maze�� ����ϴ� �Լ�
	printf("Index of Maze - Given Maze\n\n");
	for (int i = 1; i < MAX_ROW + 1; i++) {
		for (int j = 1; j < MAX_COL + 1; j++) {
			printf("%d ", maz[i][j]);
		}
		printf("\n");
	}
	print_line();
}
void show_stack(element* stack_ele, int* stack_top) {
	//stack�� ������ ����ϴ� �Լ�
	for (int i = 0; i <= (*stack_top); i++) {
		printf("(%d ,%d, %d ),   ", stack_ele[i].row, stack_ele[i].col, stack_ele[i].dir);
	}
	print_line();
}
void show_exit(element exit) {
	//������ exit���� ����ϴ� �Լ�
	printf("Destinataion : (%d, %d)\n", exit.row, exit.col);
	print_line();
}
void init_exit(element* exit, bool(*maz)[MAX_COL + 2]) {
	//exit���� �ʱ�ȭ �ϴ� �Լ�
	int exit_value;
	printf("Choose MAZE EXIT\nEXIT - RANDOM : Enter 1\nEXIT - (MAX_ROW, MAX_COL) : Enter 0\nyour choice is : ");
	
	scanf_s("%d", &exit_value);
	srand(time(NULL));
	if (exit_value) {
		do {
			exit->row = ((int)(rand() % MAX_ROW + 1));	//������ ��������
			exit->col = ((int)(rand() % MAX_COL + 1));
		} while (maz[exit->row][exit->col] != 0);
	}
	else {
		exit->row = MAX_ROW;						//�־��� MAX_ROW,MAX_COL�� Ž��
		exit->col = MAX_COL;
	}
	exit->dir = 0;
	print_line();
}
void get_type_search(int* type_search) {
	//å�� ������� Ǯ������, ���������� Ǯ������ ����
	printf("Choose Type of Search - \nBook's Algorithm : Enter 1\nRandom Direction Algorith : Enter 0\nYour choice : ");
	scanf_s("%d", type_search);
	print_line();
}
void setup_move(MOVES* move) {
	//move ���� å���� ���� ������ �ʱ�ȭ
	MOVES temp[8] = { { -1,0 },{ -1,1 },{ 0,1 },{ 1,1 },{ 1,0 },{ 1,-1 },{ 0,-1 },{ -1,-1 } };
	for (int i = 0; i < 8; i++) {
		move[i] = temp[i];
	}
}
bool is_one_step_exit(element cur_pos, MOVES* move, element exit) {
	//�־��� element�� exit���� ��ĭ ������ �ִ��� ã�� �Լ�
	element next_pos = { 0,0,0 };
	next_pos = get_next_pos(cur_pos, move, 0);
	for (int i = 1; i < 8; i++) {
		if (next_pos.row == exit.row && next_pos.col == exit.col) {
			return 1;
		}
		next_pos = get_next_pos(cur_pos, move, i);
	}
	return 0;
}