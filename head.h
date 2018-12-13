#pragma once
#include <algorithm>
#include <cstring>
#include <stack>
#include <deque>
#include <ctime>
#include <cstdlib>
#include <graphics.h>
#include <vector>
using namespace std;
#define mem(a) memset(a,0,sizeof(a))
#define mvdistance 8

const int Size = 40;
static int xdir[] = { -1, 0, 0, 1 };
static int ydir[] = { 0, -1, 1, 0 };
enum coin_type {
	coin_1,
	coin_5,
	coin_10,
	coin_50,
	coin_100,
	coin_500,
	coin_none = -1
};
int price[] = { 1, 5, 10, 50, 100, 500, 1000 };

struct node {
	int sy;
	bool dir;
	int ey;
	coin_type coin;
};
static vector<node> v[8];
static int erase[8][35];
static int vis[8][35];
static int crashed[8];
class player;
class COIN;
int dfs(int, int, coin_type, COIN&, int a[8][35]);

class COIN {
	deque<coin_type> pile[8];
	int count;
	short statephase;
	const clock_t interval;
	friend player;
	friend int dfs(int, int, coin_type, COIN&, int a[8][35]);
public:
	IMAGE img[6];
	COIN(clock_t interval = 25) :interval(interval) {
		loadimage(&img[0], "pic\\1.jpg", Size, Size);
		loadimage(&img[1], "pic\\5.jpg", Size, Size);
		loadimage(&img[2], "pic\\10.jpg", Size, Size);
		loadimage(&img[3], "pic\\50.jpg", Size, Size);
		loadimage(&img[4], "pic\\100.jpg", Size, Size);
		loadimage(&img[5], "pic\\500.jpg", Size, Size);
		init();
	}
	void init() {
		mem(erase);
		mem(vis);
		memset(crashed, -1, sizeof(crashed));
		count = 0; statephase = 0;
		for (int i = 0; i < 8; i++) pile[i].clear();
		for (int i = 0; i < 5; i++)
			create();
	}
	bool create() {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 35; j++)
				if (erase[i][j]) return 0;
		}
		for (int i = 0; i < 8; i++) {
			if (crashed[i] != -1) crashed[i]++;
			vector<node>::iterator it;
			for (it = v[i].begin(); it != v[i].end();) {
				if ((*it).dir == 0) {
					(*it).ey += Size;
					if ((*it).sy <= (*it).ey) {
						it = v[i].erase(it);
						continue;
					}
				}
				else if ((*it).dir == 1 && (*it).sy < (pile[i].size() + 1) * Size)
					(*it).sy = (pile[i].size() + 1) * Size;
				it++;
			}
			int random = rand() % 40;
			if (random < 8)
				pile[i].push_front(coin_1);
			else if (random < 16)
				pile[i].push_front(coin_10);
			else if (random < 24)
				pile[i].push_front(coin_100);
			else if (random < 30)
				pile[i].push_front(coin_5);
			else if (random < 36)
				pile[i].push_front(coin_50);
			else pile[i].push_front(coin_500);
		}
		count += 8;
		return 1;
	}
	bool check();
	void disappear(player& p);
	void coin_remove(const player& p);
	int getcount() const { return count; }
	void printcount() const {
		bar(8 * Size + 20, 470, 8 * Size + 125, 510);
		setcolor(GREEN);
		settextstyle(35, 0, "宋体");
		char str[10];
		sprintf(str, "%6d", count);
		outtextxy(8 * Size + 20, 470, str);
	}
	void draw() const {
		for (int i = 0; i < 8; i++) {
			int num = 0;
			for (int j = 0; j < v[i].size(); j++)
				if (v[i][j].dir == 0) num++;
			for (int j = 0; j < pile[i].size() - num; j++) {
				putimage(i*Size, j*Size, img + pile[i][j]);
			}
		}
	}
};

class player {
	int pos;
	coin_type coin;
	int count;
	int score;
	int combo;
	int maxcombo;
	const clock_t cleartime;
	clock_t begin;
	IMAGE yukari[5];
	mutable int imgpage;
	const clock_t interval;
	mutable clock_t t;
	clock_t starttime;
	int time;
public:
	player(clock_t clear = 2000, clock_t interval = 100) :cleartime(clear), interval(interval) {
		loadimage(yukari + 0, "pic\\yukari_1.jpg", Size, Size);
		loadimage(yukari + 1, "pic\\yukari_2.jpg", Size, Size);
		loadimage(yukari + 2, "pic\\yukari_3.jpg", Size, Size);
		loadimage(yukari + 3, "pic\\yukari_4.jpg", Size, Size);
		loadimage(yukari + 4, "pic\\yukari_5.jpg", Size, Size);
		init();
	}
	void init(int p = 3, coin_type type = coin_none, int num = 0, int s = 0, int com = 0, int m = 0, int page = 0, int Time = 0) {
		pos = p; coin = type; count = num;
		score = s; combo = com; maxcombo = m;
		imgpage = page; time = Time;
		t = clock(); starttime = clock(); begin = clock();
	}
	void move(int offset, const COIN& c) {
		if (pos + offset < 0 || pos + offset >= 8) return;
		bar(pos*Size, 16 * Size + 1, (pos + 1)*Size - 1, 17 * Size);
		bar(pos*Size, c.pile[pos].size()*Size, (pos + 1)*Size - 1, 16 * Size - 1);
		pos += offset;
		draw(c);
	}
	int getpos() const { return pos; }
	void takecoin(COIN& c) {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < v[i].size(); j++)
				if (v[i][j].dir == 1) return;
		}
		if (c.pile[pos].empty() || erase[pos][c.pile[pos].size() - 1] || !v[pos].empty()) return;
		if ((count == 0 && coin == coin_none) || coin == c.pile[pos].back()) {
			coin_type type = c.pile[pos].back();
			while (!c.pile[pos].empty() && c.pile[pos].back() == type) {
				c.pile[pos].pop_back();
				//c.count--;
				node temp = { c.pile[pos].size()*Size, 1, 16 * Size + 1, type };
				v[pos].push_back(temp);
				count++;
			}
			//c.printcount();
		}
	}
	void pushcoin(COIN& c) {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < v[i].size(); j++)
				if (v[i][j].dir == 1) return;
		}
		if (coin != coin_none/* && count > 0*/) {
			for (int i = 0; i < count; i++) {
				node temp = { (16 + i) * Size + 1, 0, c.pile[pos].size()*Size, coin };
				c.pile[pos].push_back(coin);
				//c.count++;
				v[pos].push_back(temp);
			}
			crashed[pos] = c.pile[pos].size() - 1;
			coin = coin_none;
			count = 0;
			//c.printcount();
		}
	}
	void changecoin(coin_type c) {
		coin = c;
	}
	void increasescore(int s) {
		score += s;
		printscore();
	}
	void printscore() const {
		bar(8 * Size + 20, 300, 8 * Size + 125, 335);
		setcolor(WHITE);
		settextstyle(35, 0, "宋体");
		char str[10];
		sprintf(str, "%6d", score);
		outtextxy(8 * Size + 20, 300, str);
	}
	int getscore() const { return score; }
	void increasecombo() {
		combo++;
		if (maxcombo < combo) maxcombo = combo;
		printcombo();
		begin = clock();
	}
	void clearcombo() {
		if (clock() - begin >= cleartime) {
			combo = 0;
			printcombo();
			begin = clock();
		}
	}
	void printcombo() const {
		bar(8 * Size + 20, 380, 8 * Size + 125, 415);
		setcolor(RGB(0xEE, 0x76, 0));
		settextstyle(35, 0, "宋体");
		char str[10];
		sprintf(str, "%6d", combo);
		outtextxy(8 * Size + 20, 380, str);
	}
	int getcombo() const { return combo; }
	int getmaxcombo() const { return maxcombo; }
	int gettime() const { return (clock() - starttime) / 1000; }
	void keeptime() {
		if (gettime() > time) {
			time++;
			printtime();
		}
	}
	void correcttime(clock_t t) { starttime += t; }
	void printtime() const {
		bar(8 * Size + 20, 16 * Size, 8 * Size + 110, 16 * Size + 35);
		setcolor(YELLOW);
		settextstyle(35, 0, "宋体");
		char str[10];
		sprintf(str, "%02d:%02d", time / 60, time % 60);
		outtextxy(8 * Size + 20, 16 * Size, str);
	}
	void draw(const COIN& c) {
		//if (count == 0 && coin != coin_none) coin = coin_none;
		if (clock() - t >= interval) {
			imgpage = (imgpage + 1) % 5;
			t = clock();
		}
		if (coin == coin_none)
			putimage(pos * Size, 16 * Size + 1, yukari + imgpage);
		else putimage(pos * Size, 16 * Size + 1, c.img + coin);
		if (c.pile[pos].size() >= 16) return;
		setcolor(YELLOW);
		int x = pos * Size, y = c.pile[pos].size() * Size;
		setlinestyle(0, 3);
		line(x + Size / 2, y + 5, x + 5, y + Size - 5);
		line(x + Size / 2, y + 5, x + Size - 5, y + Size - 5);
		setlinestyle(3, 3);
		line(x + Size / 2, y + 5, x + Size / 2, 16 * Size);
	}
};

bool COIN::check() {
	if (statephase != 0) return 1;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < v[i].size(); j++)
			if (v[i][j].dir == 0) return 1;
	}
	bool flag = 0;
	for (int i = 0; i < 8; i++) {
		if (crashed[i] == -1) continue;
		for (int j = crashed[i]; j < pile[i].size(); j++) {
			if (!vis[i][j]) {
				int sum = dfs(i, j, pile[i][j], *this, vis);
				if (((pile[i].back() & 1) && sum >= 2) ||
					(!(pile[i].back() & 1) && sum >= 5)) {
					flag = 1;
					dfs(i, j, pile[i][j], *this, erase);
					erase[i][j] = sum;
				}
			}
		}
	}
	mem(vis);
	memset(crashed, -1, sizeof(crashed));
	if (flag) {
		statephase++;
		return 1;
	}
	for (int i = 0; i < 8; i++)
		if (pile[i].size() > 16) return 0;
	return 1;
}
void COIN::disappear(player& p) {
	if (statephase != 1) return;
	static int k = 1;
	static clock_t start = clock();
	if (clock() - start < interval && k != 1) return;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 35; j++) {
			if (erase[i][j]) {
				bar(i*Size, j*Size, (i + 1)*Size - 1, (j + 1)*Size - 1);
				if (k < Size / 2) {
					IMAGE pic;
					if (pile[i][j] == coin_1)
						loadimage(&pic, "pic\\1.jpg", Size - k * 2, Size - k * 2);
					else if (pile[i][j] == coin_5)
						loadimage(&pic, "pic\\5.jpg", Size - k * 2, Size - k * 2);
					else if (pile[i][j] == coin_10)
						loadimage(&pic, "pic\\10.jpg", Size - k * 2, Size - k * 2);
					else if (pile[i][j] == coin_50)
						loadimage(&pic, "pic\\50.jpg", Size - k * 2, Size - k * 2);
					else if (pile[i][j] == coin_100)
						loadimage(&pic, "pic\\100.jpg", Size - k * 2, Size - k * 2);
					else if (pile[i][j] == coin_500)
						loadimage(&pic, "pic\\500.jpg", Size - k * 2, Size - k * 2);
					putimage(i*Size + k, j*Size + k, &pic);
				}
			}
			if (erase[i][j] > 1) {
				if (k == 1) p.increasescore(price[pile[i][j] + 1] * (((pile[i][j] & 1) ? erase[i][j] - 1 : erase[i][j] - 4) + p.getcombo()));
			}
		}
	}
	if (k == 1) p.increasecombo();
	else if (k == Size / 2) {
		k = 1;
		statephase++;
		return;
	}
	k++;
	start = clock();
}
void COIN::coin_remove(const player& p) {
	if (statephase != 2) return;
	for (int i = 0; i < 8; i++) {
		stack<node> s;
		int j;
		for (j = 0; j < pile[i].size() && !erase[i][j]; j++);
		if (j < pile[i].size()) {
			crashed[i] = j;
			if (p.getpos() == i)
				bar(i*Size, pile[i].size()*Size, (i + 1)*Size - 1, (pile[i].size() + 1)*Size - 1);
		}
		while (pile[i].size() > j) {
			node temp = { (pile[i].size() - 1) * Size, 0 };
			if (!erase[i][pile[i].size() - 1]) {
				temp.coin = pile[i].back();
				s.push(temp);
			}
			else if (erase[i][pile[i].size() - 1] != 1) {
				int type = pile[i].back() + 1;
				if (type <= coin_500) {
					temp.coin = (coin_type)type;
					s.push(temp);
					putimage(i*Size, (pile[i].size() - 1)*Size, img + s.top().coin);
				}
				else count--;
			}
			else count--;
			pile[i].pop_back();
		}
		j = 0;
		while (!s.empty()) {
			s.top().ey = pile[i].size() * Size;
			if (j < v[i].size() && v[i][j].ey == s.top().sy)
				v[i][j++].ey = s.top().ey;
			else v[i].push_back(s.top());
			pile[i].push_back(s.top().coin);
			s.pop();
		}
	}
	printcount();
	mem(erase);
	mem(vis);
	statephase = 0;
}

int dfs(int y, int x, coin_type c, COIN& p, int a[8][35]) {
	int sum = 0;
	a[y][x] = 1;
	sum++;
	for (int i = 0; i < 4; i++) {
		int xx = x + xdir[i];
		int yy = y + ydir[i];
		if (yy >= 0 && yy < 8 && xx >= 0 && xx < p.pile[yy].size() && !a[yy][xx] && p.pile[yy][xx] == c)
			sum += dfs(yy, xx, c, p, a);
	}
	return sum;
}

void pic_move(const COIN& c, player& p) {
	vector<node>::iterator it;
	for (int i = 0; i < 8; i++) {
		for (it = v[i].begin(); it != v[i].end();) {
			if (((*it).dir == 0 && (*it).sy <= (*it).ey) ||
				((*it).dir == 1 && (*it).sy >= (*it).ey)) {
				it = v[i].erase(it);
				continue;
			}
			bar(i*Size, (*it).sy, (i + 1)*Size - 1, (*it).sy + Size - 1);
			setlinestyle(3);
			setcolor(WHITE);
			line(0, 16 * Size, 8 * Size, 16 * Size);
			p.draw(c);
			if ((*it).dir == 1)
				(*it).sy = min((*it).sy + mvdistance, (*it).ey);
			else (*it).sy = max((*it).sy - mvdistance, (*it).ey);
			if ((*it).dir == 1 && (*it).sy == 16 * Size + 1) {
				p.changecoin((*it).coin);
				if (p.getpos() != i) {
					it = v[i].erase(it);
					continue;
				}
			}
			putimage(i*Size, (*it).sy, c.img + (*it).coin);
			if ((*it).sy == (*it).ey) {
				it = v[i].erase(it);
			}
			else it++;
		}
	}
}
