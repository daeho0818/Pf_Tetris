#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

// 모든 종류의 블럭의 모양을 저장한 배열 (회전 시 모양까지 저장)
char form[7][4][8] =
{
	{{1,0,1,1,1,2,1,3},{0,1,1,1,2,1,3,1},{1,0,1,1,1,2,1,3},{0,1,1,1,2,1,3,1}},
	{{1,1,2,1,1,2,2,2},{1,1,2,1,1,2,2,2},{1,1,2,1,1,2,2,2},{1,1,2,1,1,2,2,2}},
	{{2,1,0,2,1,2,1,1},{1,0,1,1,2,1,2,2},{2,1,0,2,1,2,1,1},{1,0,1,1,2,1,2,2}},
	{{0,1,1,1,1,2,2,2},{2,0,2,1,1,1,1,2},{0,1,1,1,1,2,2,2},{2,0,2,1,1,1,1,2}},
	{{1,0,1,1,1,2,2,2},{0,1,1,1,2,1,0,2},{1,0,2,0,2,1,2,2},{2,1,2,2,1,2,0,2}},
	{{2,0,2,1,2,2,1,2},{0,1,0,2,1,2,2,2},{1,0,2,0,1,1,1,2},{0,1,1,1,2,1,2,2}},
	{{0,1,1,1,2,1,1,2},{2,0,2,1,2,2,1,1},{1,1,1,2,0,2,2,2},{1,0,1,1,1,2,2,1}},
};

int tetris[21][12];
int form_kind;
int rotate_kind;
int screen_x, screen_y;
int score;
int level = 1;
int speed;
int preview;

void Title();
void Clear();
void Delete_Initial();
void Item(int a);
void CurrentXY(int x, int y);
void CursorOff(void);
void CursorOn(void);
void Next_Block();
void Clear_Condition();
void Print_Next_Block();
void Delete_Next_Block();
void Print_form();
void Delete_form();
void Initial();
int Check_board(int location_x, int location_y);
void Print_data();
void Move_data(int row);
int Check_line2(int row);
void Check_line();
void Write_board();
int Go_down();
int Select(void);
void Timing(void);

void main()
{
	int chk, chk2;
	speed = 500;
	srand(time(NULL));

	CursorOff();
	Title();
	Initial();
	Next_Block();
	Clear_Condition();

	preview = rand() % 7;

	do
	{
		screen_y = 0;
		screen_x = 4;
		Delete_Next_Block();
		form_kind = preview;
		preview = rand() % 7;
		rotate_kind = 0;

		CurrentXY(55, 1); printf("score : %d", score);
		CurrentXY(55, 2); printf("level : %d (delay = %d)", level, speed);
		CurrentXY(55, 4); printf("CTRL(아이템) : 랜덤 블럭 변환기");
		Print_form();

		Print_Next_Block();

		chk = Check_board(screen_x, screen_y);
		chk2 = Check_board(screen_x, screen_y + 1);

		if (level == 7)
		{
			Delete_Initial();
			Clear();
			break;
		}
		if (chk == false && chk2 == true)
		{
			Write_board();
			Check_line();
			break;
		}
		else
			if (chk == true && chk2 == true)
			{
				break;
			}
		do {
			Timing();
		} while (Select());
	} while (1);
	CursorOn();
}

/// <summary>
/// 타이틀 화면 출력 및 씬 변환 감지
/// </summary>
void Title()
{
	CurrentXY(25, 0);
	for (int i = 0; i < 57; i++)
	{
		printf("-");
	}
	for (int i = 0; i < 20; i++)
	{
		CurrentXY(25, i);
		printf("│\n");
	}
	CurrentXY(25, 20);
	for (int i = 0; i < 57; i++)
	{
		printf("-");
	}
	for (int i = 0; i < 20; i++)
	{
		CurrentXY(81, i);
		printf("│\n");
	}
	CurrentXY(25, 0); printf("┌");
	CurrentXY(81, 0); printf("┐");
	CurrentXY(25, 20); printf("└");
	CurrentXY(81, 20); printf("┘");

	CurrentXY(43, 9); printf("space를 눌러 시작");

	while (1)
	{
		if (GetAsyncKeyState(VK_SPACE))
		{
			Delete_Initial();
			return;
		}
	}
}

/// <summary>
/// 클리어 화면 출력 및 씬 전환 감지
/// </summary>
void Clear()
{
	CurrentXY(25, 0);
	for (int i = 0; i < 57; i++)
	{
		printf("-");
	}
	for (int i = 0; i < 20; i++)
	{
		CurrentXY(25, i);
		printf("│\n");
	}
	CurrentXY(25, 20);
	for (int i = 0; i < 57; i++)
	{
		printf("-");
	}
	for (int i = 0; i < 20; i++)
	{
		CurrentXY(81, i);
		printf("│\n");
	}
	CurrentXY(25, 0); printf("┌");
	CurrentXY(81, 0); printf("┐");
	CurrentXY(25, 20); printf("└");
	CurrentXY(81, 20); printf("┘");

	CurrentXY(46, 10); printf("게임 클리어!");
	CurrentXY(43, 21);
}

/// <summary>
/// 화면 전체를 지워주는 함수
/// </summary>
void Delete_Initial()
{
	for (int i = 0; i < 50; i++)
	{
		CurrentXY(25, i);
		for (int j = 0; j < 150; j++)
		{
			printf(" ");
		}
	}
}

/// <summary>
/// 출력 좌표를 설정하는 함수 (콘솔 커서 이동)
/// </summary>
/// <param name="x">좌표 x</param>
/// <param name="y">좌표 y</param>
void CurrentXY(int x, int y)
{
	COORD Location;
	Location.X = x;
	Location.Y = y;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Location);
}

/// <summary>
/// 콘솔 커서를 숨기는 함수
/// </summary>
void CursorOff()
{
	CONSOLE_CURSOR_INFO Information;
	Information.dwSize = 1;
	Information.bVisible = false;

	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Information);
}

/// <summary>
/// 콘솔 커서를 나타내는 함수
/// </summary>
void CursorOn()
{
	CONSOLE_CURSOR_INFO Information;

	Information.dwSize = 20;
	Information.bVisible = true;

	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Information);
}

/// <summary>
/// 다음 등장할 블럭을 띄워주는 칸을 출력하는 함수
/// </summary>
void Next_Block()
{
	CurrentXY(55, 6); printf("next block : --------");
	CurrentXY(75, 6); printf("┐");
	for (int i = 0; i < 4; i++)
	{
		CurrentXY(55, 7 + i); printf("│");
	}for (int i = 0; i < 4; i++)
	{
		CurrentXY(75, 7 + i); printf("│");
	}
	for (int i = 0; i < 20; i++)
	{
		CurrentXY(55 + i, 11); printf("-");
	}
	CurrentXY(55, 11); printf("└");
	CurrentXY(75, 11); printf("┘");
}

/// <summary>
/// 클리어 조건을 띄워주는 함수
/// </summary>
void Clear_Condition()
{
	CurrentXY(57, 13); printf("level : 7 달성 시 클리어");
}

/// <summary>
/// 다음 등장할 블럭을 출력하는 함수
/// </summary>
void Print_Next_Block()
{
	for (int i = 0; i < 4; i++)
	{
		CurrentXY(55 + (screen_x + form[preview][0][i * 2]) * 2,
			7 + screen_y + form[preview][0][i * 2 + 1]);
		printf("□");
	}
}

/// <summary>
/// 다음 등장할 블록을 출력하는 위치를 지워주는 함수
/// </summary>
void Delete_Next_Block()
{
	for (int i = 0; i < 4; i++)
	{
		CurrentXY(55 + (screen_x + form[preview][0][i * 2]) * 2,
			7 + screen_y + form[preview][0][i * 2 + 1]);
		printf(" ");
	}
}

/// <summary>
/// 블럭 랜덤 변환 아이템을 관리하는 함수
/// </summary>
void Item()
{
	if (GetAsyncKeyState(VK_CONTROL))
	{
		Delete_form();
		form_kind = rand() % 7;
		rotate_kind = 0;
	}
}

/// <summary>
/// 화면에 블럭을 출력하는 함수
/// </summary>
void Print_form()
{
	for (int i = 0; i < 4; i++)
	{
		Item();
		CurrentXY(30 + (screen_x + form[form_kind][rotate_kind][i * 2]) * 2,
			screen_y + form[form_kind][rotate_kind][i * 2 + 1]);

		tetris[screen_y + form[form_kind][rotate_kind][i * 2 + 1]]
			[screen_x + form[form_kind][rotate_kind][i * 2]] = 1;

		printf("□");
	}
}

/// <summary>
/// 화면에 출력돼있는 블럭을 지워주는 함수
/// </summary>
void Delete_form()
{
	for (int i = 0; i < 4; i++)
	{
		CurrentXY(30 + (screen_x + form[form_kind][rotate_kind][i * 2]) * 2,
			screen_y + form[form_kind][rotate_kind][i * 2 + 1]);

		tetris[screen_y + form[form_kind][rotate_kind][i * 2 + 1]]
			[screen_x + form[form_kind][rotate_kind][i * 2]] = 0;

		printf(" ");
	}
}

/// <summary>
/// 테트리스 게임 초기화 (로직 관리 배열 초기화, 테두리 출력)
/// </summary>
void Initial()
{
	int i, j;

	for (i = 0; i < 21; i++)
	{
		for (j = 0; j < 12; j++) {
			if (i == 20 || j == 0 || j == 11) {
				tetris[i][j] = 3;
			}
			else
				tetris[i][j] = 0;
		}
	}

	for (i = 0; i < 21; i++)
	{
		CurrentXY(30, i);
		printf("│");

		CurrentXY(52, i);
		printf("│");
	}

	CurrentXY(30, 20);
	printf("└");
	for (j = 1; j < 11; j++)
	{
		CurrentXY(30 + j * 2, 20);
		printf("─");
	}
	CurrentXY(52, 20);
	printf("┘");
}

/// <summary>
/// 입력받은 위치에 블럭이나 테두리가 있는지 검사하는 함수
/// </summary>
/// <param name="location_x">위치 x</param>
/// <param name="location_y">위치 y</param>
/// <returns>블럭이나 테두리가 있는지에 대한 여부</returns>
int Check_board(int location_x, int location_y)
{
	int temp = 0;

	for (int i = 0; i < 4; i++)
	{
		temp = tetris[location_y + form[form_kind][rotate_kind][i * 2 + 1]]
			[location_x + form[form_kind][rotate_kind][i * 2]];

		if (temp > 1)
			return true;
	}
	return false;
}

/// <summary>
/// 데이터상 블럭이 설치되어 있다면 사각형, 아니라면 공백 출력
/// </summary>
void Print_data()
{
	for (int i = 0; i < 20; i++)
		for (int j = 1; j < 11; j++)
			if (tetris[i][j] == 2)
			{
				CurrentXY(30 + j * 2, i);
				printf("□");
			}

			else
			{
				CurrentXY(30 + j * 2, i);
				printf(" ");
			}
}

/// <summary>
/// (한 줄이 사라진 후) 위의 모든 블럭을 아래로 내려주는 함수
/// </summary>
/// <param name="row"></param>
void Move_data(int row)
{
	for (int i = row - 1; i >= 0; i--)
		for (int j = 1; j < 11; j++)
			tetris[i + 1][j] = tetris[i][j];
}

/// <summary>
/// 한 줄이 모두 완성되었는지 확인하는 함수
/// </summary>
/// <param name="row">확인할 줄 (y)</param>
/// <returns>한 줄 완성 여부 출력</returns>
int Check_line2(int row)
{
	int chk = 0;

	for (int j = 1; j < 11; j++)
		chk += tetris[row][j];
	bool isLevelUp = false;
	if (chk == 20)
	{
		score += 10;
		if (score % 50 == 0)
		{
			level++;
			CurrentXY(55, 3); printf("level up!");

			isLevelUp = true;
		}
		if (level % 1 == 0 && isLevelUp == true)
		{
			isLevelUp = false;
			speed -= 50;
		}
		return true;
	}
	else 		return false;
}

/// <summary>
/// 완성된 줄이 있다면 해당 줄보다 높은 모든 줄의 블럭들을 내려주는 함수
/// </summary>
void Check_line()
{
	int rst;

	for (int i = screen_y; i < screen_y + 4; i++)
	{
		rst = Check_line2(i);

		if (rst == true)
		{
			Move_data(i);
			Print_data();
			i--;
		}
	}
}

/// <summary>
/// 현재 블럭의 위치에 데이터 저장
/// </summary>
void Write_board()
{
	for (int i = 0; i < 4; i++)
	{
		tetris[screen_y + form[form_kind][rotate_kind][i * 2 + 1]]
			[screen_x + form[form_kind][rotate_kind][i * 2]] = 2;
	}
}

/// <summary>
/// 블럭이 끝까지 도달했다면 해당 위치를 저장하고, 아니라면 블럭을 계속해서 내려주는 함수
/// </summary>
/// <returns>블럭이 마지막에 도달했는지에 대한 여부</returns>
int Go_down()
{
	int chk2;

	chk2 = Check_board(screen_x, screen_y + 1);

	if (chk2 == true)
	{
		Write_board();
		Check_line();
		return true;
	}

	Delete_form();
	screen_y++;
	Print_form();
	return false;
}

/// <summary>
/// 플레이어의 입력에 따른 행동 처리
/// </summary>
/// <returns></returns>
int Select()
{
	int chk1;
	int prev_rotate;
	int new_rotate;
	int rtn_value;

	if (GetAsyncKeyState(VK_LEFT))
	{

		chk1 = Check_board(screen_x - 1, screen_y);

		// 이동하려는 위치가 비어있다면 이동
		if (chk1 == false)
		{
			Delete_form();
			screen_x--;
			Print_form();
			return true;
		}
	}

	if (GetAsyncKeyState(VK_RIGHT))
	{
		chk1 = Check_board(screen_x + 1, screen_y);

		if (chk1 == false)
		{
			Delete_form();
			screen_x++;
			Print_form();
			return true;
		}
	}

	if (GetAsyncKeyState(VK_UP))
	{
		prev_rotate = rotate_kind;

		if (rotate_kind == 3) rotate_kind = 0;
		else 		rotate_kind++;

		new_rotate = rotate_kind;

		chk1 = Check_board(screen_x, screen_y);

		// 막힘 없이 회전이 가능하다면 회전 전 상태를 지우고 회전 후 상태 출력
		if (chk1 == false)
		{
			rotate_kind = prev_rotate;
			Delete_form();

			rotate_kind = new_rotate;
			Print_form();
			return true;
		}
		else
		{
			rotate_kind = prev_rotate;
		}
	}

	// 블럭이 끝까지 내려가게 하기
	if (GetAsyncKeyState(VK_SPACE))
	{
		while (!Go_down());
		return false;
	}

	rtn_value = Go_down();

	if (rtn_value == true)
	{
		return false;
	}
	else
	{
		return true;
	}
}

/// <summary>
/// delay 만큼 대기(while 문) 하는 함수
/// </summary>
void Timing()
{
	DWORD			thisTickCount;
	static DWORD	lastTickCount = 0;
	DWORD			delay = speed;

	thisTickCount = GetTickCount();

	while (1)
	{
		if ((thisTickCount - lastTickCount) > delay)
		{
			lastTickCount = thisTickCount;
			return;
		}
		thisTickCount = GetTickCount();
	}
}