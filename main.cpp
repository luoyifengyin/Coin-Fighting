#include "head.h"
#include <iostream>
#include <conio.h>
#include <cctype>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>

COIN c;
player p;
void init() {
	initgraph(12 * Size + 1, 17 * Size + 1);
	srand(time(NULL));
}

int menu() {
	setbkcolor(WHITE);
	cleardevice();
	setcolor(BLACK);
	settextstyle(65, 0, "宋体");
	outtextxy(45, 150, "零用钱大作战");
	setfillcolor(RGB(0xFF, 0x7F, 0));
	bar(152, 350, 328, 400);
	bar(152, 450, 328, 500);
	bar(152, 550, 328, 600);
	setbkmode(TRANSPARENT);
	setcolor(WHITE);
	settextstyle(36, 0, "宋体");
	outtextxy(168, 357, "开始游戏");
	outtextxy(168, 457, "游戏规则");
	outtextxy(168, 557, "结束游戏");
	MOUSEMSG msg;
	while (1) {
		msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN && 152 <= msg.x && msg.x <= 328) {
			if (350 <= msg.y && msg.y <= 400)return 0;
			else if (450 <= msg.y && msg.y <= 500)return 1;
			else if (550 <= msg.y && msg.y <= 600)return 2;
		}
	}
}

void Rule() {
	setbkcolor(BLUE);
	cleardevice();
	setcolor(RGB(0xFF, 0x45, 0));
	settextstyle(35, 0, "宋体");
	outtextxy(165, 20, "游戏规则");

	setcolor(RGB(0xFF, 0x7F, 0));
	settextstyle(22, 0, "宋体");
	int x = 100, y = 40;
	outtextxy(10, 0 * y + x, "1.开始游戏后，系统将随机的在玩家的游戏界面");
	outtextxy(10, 1 * y + x, "中生成5层随机排列面值不同的硬币。");
	outtextxy(10, 2 * y + x, "2.玩家使用方向键（←左 右→键）进行移动，");
	outtextxy(10, 3 * y + x, "并使用吸取（ A 键）和放出（ S 键）两个按钮");
	outtextxy(10, 4 * y + x, "来移动硬币。");
	outtextxy(10, 5 * y + x, "3.硬币的面值分为：1、5、10 、50 、100、");
	outtextxy(10, 6 * y + x, "500。当相连的同种硬币数量达到2或5时，就可");
	outtextxy(10, 7 * y + x, "以转换下一级硬币。比如5个或5个以上面值1的");
	outtextxy(10, 8 * y + x, "硬币可以转换1个面值5的硬币;2个或2个以上面");
	outtextxy(10, 9 * y + x, "值5的硬币2个或2个以上面值5的硬币可以转换1");
	outtextxy(10, 10 * y + x, "个面值10的硬币。");
	outtextxy(10, 11 * y + x, "4.如果玩家没有成功消除硬币反而让硬币达到");
	outtextxy(10, 12 * y + x, "底端,则游戏失败。");

	setfillcolor(RED);
	bar(350, 13 * y + x, 400, 650);
	setcolor(WHITE);
	settextstyle(22, 0, "宋体");
	outtextxy(353, 13 * y + x + 5, "返回");
	MOUSEMSG msg;
	while (1) {
		msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN && 350 <= msg.x && msg.x <= 400 && 13 * y + x <= msg.y && msg.y <= 650)
			return;
	}
}

int GameOver() {
	system("md record");
	int bestscore = 0;
	bool newbest = 0;
	fstream file;
	file.open("record\\score.txt", ios::in);
	if (file) {
		file >> bestscore;
	}
	if (p.getscore() > bestscore) {
		newbest = 1;
		bestscore = p.getscore();
	}
	file.close();
	if (newbest) {
		file.open("record\\score.txt", ios::out);
		file << bestscore;
	}
	string str;
	stringstream ss;
	ss << "Your score: " << p.getscore();
	if (newbest) ss << " New Best!";
	ss << "\nBest score: " << bestscore;
	ss << "\nMax Combo: " << p.getmaxcombo();
	ss << "\n游戏用时: ";
	ss << setw(2) << setfill('0') << p.gettime() / 60 << ':';
	ss << setw(2) << setfill('0') << p.gettime() % 60;
	str = ss.str();
	return  MessageBox(0, str.c_str(), "Game over!", MB_RETRYCANCEL | MB_SYSTEMMODAL);
}

void init_game(int lv) {
	setbkcolor(RGB(0x2F, 0x4F, 0x4F));
	cleardevice();
	setcolor(WHITE);
	setlinestyle(0);
	line(8 * Size, 0, 8 * Size, 17 * Size);
	c.draw();
	setlinestyle(3);
	line(0, 16 * Size, 8 * Size, 16 * Size);
	p.draw(c);
	setfillcolor(RGB(0x2F, 0x4F, 0x4F));
	setcolor(RGB(0x93, 0x70, 0xDB));
	settextstyle(50, 0, "宋体");
	outtextxy(8 * Size + 5, 10, "零用钱");
	outtextxy(8 * Size + 5, 70, "大作战");

	setcolor(RED);
	settextstyle(35, 0, "宋体");
	outtextxy(8 * Size + 20, 220, "LV: ");
	char s[10];
	sprintf(s, "%d", lv);
	outtextxy(10 * Size + 5, 220, s);

	setcolor(WHITE);
	settextstyle(35, 0, "宋体");
	outtextxy(8 * Size + 20, 260, "Score: ");
	p.printscore();

	setcolor(RGB(0xEE, 0x76, 0));
	outtextxy(8 * Size + 20, 340, "Combo:");
	p.printcombo();

	setcolor(GREEN);
	settextstyle(24, 0, "宋体");
	outtextxy(8 * Size + 5, 440, "硬币剩余数量：");
	c.printcount();

	setcolor(YELLOW);
	settextstyle(30, 0, "宋体");
	outtextxy(8 * Size + 20, 15 * Size, "游戏时间：");
	p.printtime();
}

bool GameStart() {
	bool gameover = 0;
	int lv = 1;
	clock_t interval = 10000;
	c.init();
	p.init();
	init_game(lv);
	clock_t start = clock(), a = clock();
	while (!gameover) {
		while (!kbhit()) {
			pic_move(c, p);
			if (c.check() == 0) {
				gameover = 1;
				break;
			}
			c.disappear(p);
			c.coin_remove(p);
			if (clock() - start >= interval) {
				if (c.create()) {
					c.draw();
					c.printcount();
					start = clock();
				}
			}
			p.clearcombo();
			p.draw(c);
			p.keeptime();
		}
		if (gameover) break;
		char ch = getch();
		switch (ch) {
		case 75: p.move(-1, c); break;
		case 77: p.move(1, c); break;
		case 'A':
		case 'a':
			p.takecoin(c); break;
		case 'S':
		case 's':
			p.pushcoin(c); break;
		case 27:
			clock_t temp = clock();
			if (MessageBox(0, "是否继续游戏？", "暂停中", MB_YESNO | MB_SYSTEMMODAL | MB_ICONQUESTION) == IDNO)
				return 0;
			start += clock() - temp;
			a += clock() - temp;
			p.correcttime(clock() - temp);
		}
		if (lv < 6) {
			if (p.getscore() > lv * 10000) {
				interval -= 1000;
				lv++;
			}
		}
		else if (lv < 10) {
			if (p.getscore() > 60000 + (lv - 5) * 20000) {
				interval -= 1000 - lv * 100;
				lv++;
			}
		}
		if (c.getcount() < 20 && clock() - a >= interval) {
			c.create();
			c.draw();
			p.increasescore(lv * 1000);
			a = clock();
		}
		bar(10 * Size + 5, 220, 10 * Size + 40, 255);
		setcolor(RED);
		settextstyle(35, 0, "宋体");
		char s[10];
		sprintf(s, "%d", lv);
		outtextxy(10 * Size + 5, 220, s);
	}
	if (gameover) {
		if (GameOver() == IDRETRY) return 1;
		else return 0;
	}
	return 0;
}

int main() {
	init();
	while (1) {
		int ret = menu();
		switch (ret) {
		case 0: while (GameStart()); break;
		case 1: Rule(); break;
		case 2: exit(0);
		}
	}
	closegraph();
	return 0;
}
