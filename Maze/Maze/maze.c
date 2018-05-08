/*	창의ICT공과대학 컴퓨터공학부 컴퓨터공학과 20150864 조종현
	프로그램 내용	:	자료구조 3.5절 프로그래밍 문제에 대한 code분석과 그 활용
	파일 이름		:	maze.c
	입력값			:	목적지 설정을 최대배열값(MAX_ROW,MAX_COL)로 할것인지, 임의의 값으로 할것인지 정함
						경로탐색의 방법을 책의 방법(순차탐색)으로 할 것인지 랜덤한 방향으로(임의탐색)으로 할 것인지 정함
	출력값			:	주어진 미로(MAZE), 목적지
						경로 탐색중 사용한 유효경로(back_tracking 값 제외)
						유효경로의 숫자 = 탐색경로의 길이
	날짜			:	2018-04-10 ~ 2018-05-05
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//배열 선언을 위한 상수 정의
#define MAX_ROW 10
#define MAX_COL 10
#define MAX_STACK 10000


typedef enum { false, true } bool;					//boolean 자료형 정의
													
//구조체 정의
typedef struct {									//다음 위치를 알기위한 offset 설정
	int vert;
	int horiz;
}MOVES;
typedef struct {									//MAZE 탐색을 위한 position 선정을 위한 element 정의
	int row;
	int col;
	int dir;
}element;

//함수정의
void print_line();									//라인 출력
void setup_maze();									//주어진 maze를 입력, 이를 bool 배열에 집어넣음.
void init_maze();									//maze 탐색 알고리즘에 의한 탐색 maze에 주어진 maze를 입력하고 테두리를 true로 선언
element get_next_pos();								//현재 위치와 방향을 받아 다음 위치를 선정
void stack_push();									//stack에서의 push함수
element stack_pop();								//stack에서의 pop합수
void get_path();									//교재의 알고리즘으로 경로탐색 후 경로를 출력하는 함수
void get_path_rand_dir();							//dir을 랜덤난수로 지정받아 경로탐색 후 경로를 출력하는 함수
void show_maze_value();								//주어진 maze를 출력하는 함수
void stack_delete();								//stack의 맨 위에값을 없애는 함수
void show_stack();									//stack을 참조해 출력하는 함수
void show_exit();									//설정된 exit값을 출력하는 함수
void init_exit();									//exit값을 초기화 하는 함수
void setup_move();									//move 값을 책에서 구한 값으로 초기화
void get_type_search();								//책의 방식으로 풀것인지, 랜덤난수로 풀것인지 정함
bool is_one_step_exit();							//주어진 element가 exit에서 한칸 떨어져 있는지 찾는 함수

int main() {
	//주요 변수 선언 및 초기화
	bool given_maze[MAX_ROW][MAX_COL];
	bool maz[MAX_ROW + 2][MAX_COL + 2];
	element exit;
	element stack_ele[MAX_STACK];
	int* stack_top;
	int type_search;
	stack_top = (int*)malloc(sizeof(int));

	//탐색을 위한 offset move 선언 및 초기화
	MOVES move[8];
	setup_move(move);

	//주어진 MAZE를 받는 함수, maz에 알고리즘에서 쓸 [MAX_ROW+2][MAX_COL+2]크기로 변환
	setup_maze(given_maze);
	init_maze(maz, given_maze);

	//exit의 결과를 (MAX_ROW,MAX_COL) 또는 임의의 도착지로 정할것인지 묻고 exit을 저장
	init_exit(&exit, maz);
	get_type_search(&type_search);
	
	//maze value와 목적지 exit을 사용자에게 보여주는 함수
	show_maze_value(maz);
	show_exit(exit);

	//type_search의 입력값에 따라 알맞은 함수 호출
	if (type_search) {
		get_path(maz, stack_ele, stack_top, move, exit);
	}
	else {
		get_path_rand_dir(maz, stack_ele, stack_top, move, exit);
	}
	return 0;
}

void print_line() {
	//콘솔창을 구분하기위한 라인 출력함수
	printf("\n-----------------------------------------------------------------\n\n");
}
void init_maze(bool (*maz)[MAX_COL + 2], bool(*maze)[MAX_COL]) {
	//maze 탐색 알고리즘에 의한 탐색 maze에 주어진 maze를 입력하고 테두리를 true로 선언
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
	//maze를 입력하는 함수. 문제의 조건에 맞추어 직접 입력하는것이 아닌 코드 내에서 주어진다.
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
	//입력받은 값을 바로 given_maze 배열에 집어넣는다.
	for (int i = 0; i < MAX_ROW; i++) {
		for (int j = 0; j < MAX_COL; j++) {
			given_maze[i][j] = (bool)setup_maze[i][j];
		}
	}
}
element get_next_pos(element cur_position, MOVES* move, int dir) {
	//현재 위치와 방향을 받아 다음 위치를 선정
	element next_position;
	next_position.row = cur_position.row + move[dir].vert;
	next_position.col = cur_position.col + move[dir].horiz;
	next_position.dir = dir;
	return next_position;
}
void stack_push(element* a, int* top, element value1) {
	//stack에서의 push함수
	if (*top > MAX_STACK) {
		printf("stack error");
	}
	(*top) += 1;
	a[(*top)] = value1;
}
element stack_pop(element* a, int* top) {
	//stack에서의 pop합수
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
	//교재의 알고리즘으로 경로탐색 후 경로를 출력하는 함수
	bool found = false;														//목적지를 찾았는지(갈수있는지 없는지는 중요x)
	element cur_pos = { 0,0,0 };
	element next_pos = { 0,0,0 };
	maz[1][1] = 1, *stack_top = 0;
	stack_ele[0].row = 1, stack_ele[0].col = 1, stack_ele[0].dir = 0;

	//경로 탐색을 위한 반복함수
	while (*stack_top > -1 && found != true) {								//stack이 비어있거나, 목적지를 찾으면 탈출
		cur_pos = stack_pop(stack_ele, stack_top);
		while (cur_pos.dir < 8 && found != true) {		//에러 카운트가 일정이상이거나, 8방향 전부 찾았거나, 목적지를 찾으면 탈출
			next_pos = get_next_pos(cur_pos, move, cur_pos.dir);			//다음 경로를 가져옴
			if (is_one_step_exit(cur_pos, move, exit)) {					//목적지를 찾았으면 = 현재 위치가 목적지에서 1칸 거리이면
				found = true;												
				stack_push(stack_ele, stack_top, cur_pos);
			}
			else if (!maz[next_pos.row][next_pos.col]) {
				maz[next_pos.row][next_pos.col] = 1;					//갈 수 있는경로 + 한번도 안간 경로이면

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

	//경로 탐색을 위해 stack에 push되었던 값을 출력
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
	//dir을 랜덤난수로 지정받아 경로탐색 후 경로를 출력하는 함수
	long error_count = 0;													//랜덤 경로 탐색에서 탐색방향 변경을 위한 변수
	bool found = false;
	element cur_pos = { 0,0,0 };
	element next_pos = { 0,0,0 };
	maz[1][1] = 1, *stack_top = 0;
	stack_ele[0].row = 1, stack_ele[0].col = 1, stack_ele[0].dir = ((int)(rand() % 8));
	while (*stack_top > -1 && found != true) {
		cur_pos = stack_pop(stack_ele, stack_top);
		error_count = 0;
		while (error_count < 100000 && found != true) {						//에러 카운트가 수없이 올라감 = 경로탐색중 갇혀 error에 걸림
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
				error_count++;												//탐색 실시 이후 목적지를 찾거나, 갈수 있는경로가 아니라면 count 증가
			}
		}
	}
	//경로 탐색을 위해 stack에 push되었던 값을 출력
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
	//주어진 maze를 출력하는 함수
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
	//stack을 참조해 출력하는 함수
	for (int i = 0; i <= (*stack_top); i++) {
		printf("(%d ,%d, %d ),   ", stack_ele[i].row, stack_ele[i].col, stack_ele[i].dir);
	}
	print_line();
}
void show_exit(element exit) {
	//설정된 exit값을 출력하는 함수
	printf("Destinataion : (%d, %d)\n", exit.row, exit.col);
	print_line();
}
void init_exit(element* exit, bool(*maz)[MAX_COL + 2]) {
	//exit값을 초기화 하는 함수
	int exit_value;
	printf("Choose MAZE EXIT\nEXIT - RANDOM : Enter 1\nEXIT - (MAX_ROW, MAX_COL) : Enter 0\nyour choice is : ");
	
	scanf_s("%d", &exit_value);
	srand(time(NULL));
	if (exit_value) {
		do {
			exit->row = ((int)(rand() % MAX_ROW + 1));	//목적지 랜덤생성
			exit->col = ((int)(rand() % MAX_COL + 1));
		} while (maz[exit->row][exit->col] != 0);
	}
	else {
		exit->row = MAX_ROW;						//주어진 MAX_ROW,MAX_COL로 탐색
		exit->col = MAX_COL;
	}
	exit->dir = 0;
	print_line();
}
void get_type_search(int* type_search) {
	//책의 방식으로 풀것인지, 랜덤난수로 풀것인지 정함
	printf("Choose Type of Search - \nBook's Algorithm : Enter 1\nRandom Direction Algorith : Enter 0\nYour choice : ");
	scanf_s("%d", type_search);
	print_line();
}
void setup_move(MOVES* move) {
	//move 값을 책에서 구한 값으로 초기화
	MOVES temp[8] = { { -1,0 },{ -1,1 },{ 0,1 },{ 1,1 },{ 1,0 },{ 1,-1 },{ 0,-1 },{ -1,-1 } };
	for (int i = 0; i < 8; i++) {
		move[i] = temp[i];
	}
}
bool is_one_step_exit(element cur_pos, MOVES* move, element exit) {
	//주어진 element가 exit에서 한칸 떨어져 있는지 찾는 함수
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