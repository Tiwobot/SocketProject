#ifndef _INC_ayar
#define _INC_ayar

#include <stdlib.h>
#include <string.h>
#include "sr.c"

#ifdef _WIN32

#include <conio.h>
#include <windows.h>

void gotoxy(short x, short y)
{
	COORD Cursor_Pos = {x-1, y-1};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cursor_Pos);
}

#else

void gotoxy(short x, short y)
{
 	printf("%c[%d;%df", 0x1B, y, x);
}

int getch(void)
{
	int ch;
	struct termios oldattr, newattr;
	tcgetattr( STDIN_FILENO, &oldattr );
	newattr = oldattr;
   	newattr.c_lflag &= ~( ICANON | ECHO );
	tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
   	ch = getchar();
   	tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
   	return ch;
}

#endif

int check = 0, port;
char ip[16] = {'\0'}, isim[16] = {'\0'};

short x11 = 1, y11 = 3, x22, y22 = 1, yedek = 3;
int yatay, dikey, soket, ilk_blok_sonu, ikinci_blok_basi;

void temizle();
void InitC();
void SetTerminal();
void Cevir(char *);
int Kontrol(char *);
void *VeriAl(void *);
void Protokol(char *);
void TextInput(char *);
void TextOutput(char *, short int, short int *);

#endif
