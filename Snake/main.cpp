#include<iostream>
#include<time.h>
#include<easyx.h>
#include<vector>
#include<random>
#include<conio.h>
#include<Windows.h>

#define HENG 640
#define ZONG 480

enum Direct
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class Block		//定义块体类
{
public:
	Block() :Block(0, 0) {};	//初始化函数一
	Block(int x, int y, COLORREF color = BLUE) {	//初始化函数二
		B_x = x;
		B_y = y;
		B_color = color;
	}

	int getx() {
		return B_x;
	}

	int gety() {
		return B_y;
	}

	virtual void fresh(int x, int y) {
		B_x = x;
		B_y = y;
	}

	virtual void draw() {
		setfillcolor(B_color);
		fillrectangle(B_x, B_y, B_x + size, B_y + size);
	}

	virtual void erase() {
		clearrectangle(B_x, B_y, B_x + size, B_y + size);
	}

	bool operator==(const Block& b) {
		return ((this->B_x-b.B_x<10)&&(this->B_x - b.B_x > -10)) && ((this->B_y-b.B_y<10)&&(this->B_y - b.B_y >-10));
	}

protected:
	int B_x;
	int B_y;
	COLORREF B_color;	//块体颜色
	int size = 10;
};




class food : public Block
{
public:
	food() {
		std::random_device seed;	//硬件生成随机数种子
		std::ranlux48 engine(seed());//利用种子生成随机数引擎
		std::uniform_int_distribution<> h(1, HENG - 1);//设置随机数范围，并为均匀分布
		int rx = h(engine);//随机数
		std::uniform_int_distribution<> s(1, ZONG - 1);//设置随机数范围，并为均匀分布
		int ry = s(engine);//随机数
		B_x = rx - rx%10;
		B_y = ry - rx%10;
		B_color = DARKGRAY;
	};
	Block coo() {
		return Block(B_x, B_y);
	}

	void born() {
		std::random_device seed;	//硬件生成随机数种子
		std::ranlux48 engine(seed());//利用种子生成随机数引擎
		std::uniform_int_distribution<> h(1, HENG - 1);//设置随机数范围，并为均匀分布
		int rx = h(engine);//随机数
		std::uniform_int_distribution<> s(1, ZONG - 1);//设置随机数范围，并为均匀分布
		int ry = s(engine);//随机数
		B_x = rx - rx % 10;
		B_y = ry - ry % 10;
		B_color = DARKGRAY;
	}
};

class snake : public Block
{
public:
	snake() {
		body.push_back(Block(HENG/2, ZONG/2));
		body.push_back(Block(HENG/2 - size, ZONG/2));
		body.push_back(Block(HENG/2 - size * 2, ZONG/2));
	}
	snake(int x, int y) {
		body.push_back(Block(x, y));
		body.push_back(Block(x - size, y));
		body.push_back(Block(x - size * 2, y));

	}

	Block getHead() {
		return body[0];
	}

	Direct getDir() {
		return headD;
	}

	void draw() override {
		setfillcolor(BLUE);
		for (int i = 0;i < body.size();i++) {
			body[i].draw();
		}
	}


	void move(int i=1) {
		switch (headD)
		{
		case UP:
			if (i) {
				body.back().erase();
				body.pop_back();
			}
			body.insert(body.begin(), Block(body.front().getx(), body.front().gety() + size));

			break;
		case DOWN:
			if (i) {
				body.back().erase();
				body.pop_back();
			}
			body.insert(body.begin(), Block(body.front().getx(), body.front().gety() - size));

			break;
		case LEFT:
			if (i) {
				body.back().erase();
				body.pop_back();
			}
			body.insert(body.begin(), Block(body.front().getx() - size, body.front().gety()));

			break;
		case RIGHT:
			if (i) {
				body.back().erase();
				body.pop_back();
			}
			body.insert(body.begin(), Block(body.front().getx() + size, body.front().gety()));

			break;
		}
	}

	void changeDir(Direct a) {
		headD = a;
	}
	bool conflict() {
		for (int i = 1.;i < body.size();i++) {
			if (body[0] == body[i]) {
				return true;
			}
		}
		if (body[0].getx() < 2*size || body[0].getx() > HENG - 2*size || body[0].gety() < 2*size || body[0].gety() > ZONG - 2*size) {
			return true;
		}
		else {
			return false;
		}
	}

private:
	std::vector<Block> body;
	Direct headD = RIGHT;
};

class GameScene
{
public:
	GameScene()
	{
		initgraph(HENG, ZONG);
		setbkcolor(WHITE);
		cleardevice();

		shit.draw();
		Kawaii.draw();
	}

	int run()
	{

		ExMessage msg = { 0 };
		while (peekmessage(&msg, EX_KEY))
		{
			React(msg);
		}
		
		if (Kawaii.getHead() == shit.coo()) {
			shit.erase();
			shit.born();
			shit.draw();
			Kawaii.move(0);
		}
		else {
			Kawaii.move();
		}
		if (Kawaii.conflict()) {
			return 1;
		}
		else {
			Kawaii.draw();
			return 0;
		}

	}


	void React(const ExMessage& msg) {
		switch ((int)msg.vkcode)
		{
		case 37:
			if (Kawaii.getDir() != RIGHT) {
				Kawaii.changeDir(LEFT);
			}
			break;
		case 38:
			if (Kawaii.getDir() != UP) {
				Kawaii.changeDir(DOWN);
			}
			break;
		case 39:
			if (Kawaii.getDir() != LEFT) {
				Kawaii.changeDir(RIGHT);
			}
			break;
		case 40:
			if (Kawaii.getDir() != DOWN) {
				Kawaii.changeDir(UP);
			}
			break;
		}
	}


private:
	snake Kawaii;
	food shit;
};



int main()
{
	GameScene scene;
	int state = 0;
	while(1) {
		state = scene.run();
		if (state == 1) {
			MessageBox(NULL, (LPCTSTR)TEXT("Oops!"), (LPCTSTR)TEXT("Game over!"), MB_OK);
			return 0;
		}
		Sleep(50);
	}

	return 0;
}