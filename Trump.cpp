#include "Trump.h"
#include<math.h>
#include<assert.h>

Trump::Trump(GameObject* parent)
{
	gHandle = LoadGraph("Assets/trump.png");
	//失敗するとgHandleが0より小さくなる
	assert(gHandle >= 0);
	int i = 0;
	//ここでcardsに52枚のカードを入れる
	for (int m = 0; m < 4; m++) {
		for (int n = 1; n < 14; n++) {
			Cards c;
			c.mark = m;
			c.number = n;
			c.moving = false;
			cards.push_back(c);
		}
	}
	//ここでシャッフル
	for (int r = 0; r < cards.size(); r++) {
		int lft = rand() % cards.size();
		Cards tmp = cards[r];
		cards[r] = cards[lft];
		cards[lft] = tmp;
	}
	counter = 0;
	playerID = 0;
	/*for (int i = 0; i < num; i++) //例２
	{
		cards[i].mark = i / 13;
		cards[i].number = (i % 13) + 1;
	}*/

}

Trump::~Trump()
{
}

void Trump::Update()
{
	counter -= 1;
	if (counter <= 0) {
		counter = 10;
		if (cards.size() > 0) {
			Cards c = cards[0];
			cards.erase(cards.begin());
			c.moving = true;
			c.startX = 0;
			c.startY = 300;
			c.rate = 0.0;
			c.x = c.startX;
			c.y = c.startY;
			if (playerID == 0) {
				c.endX = player1.size() *20;
				c.endY = 50;
				player1.push_back(c);
				playerID = 1;
			}
			else {
				c.endX = player2.size() * 20;
				c.endY = 550;
				player2.push_back(c);
				playerID = 0;
			}
		}
	}
	for (Cards& c : player1) {
		if (c.moving) {
			c.rate += 0.01f;
			float r = sinf(c.rate * DX_PI / 2.0f);
			c.x = (c.endX - c.startX) * r + c.startX;
			c.y = (c.endY - c.startY) * r + c.startY;

			if (c.rate >= 1.0f) {
				c.moving = false;
			}
		}
	}

	for (Cards& c : player2) {
		if (c.moving) {
			c.rate += 0.01f;
			float r = sinf(c.rate * DX_PI / 2.0f);
			c.x = (c.endX - c.startX) * r + c.startX;
			c.y = (c.endY - c.startY) * r + c.startY;
			

			if (c.rate >= 1.0f) {
				c.moving = false;
			}
		}
	}
#if 0
	//Nで数字を変える
	if (CheckHitKey(KEY_INPUT_N) > 0)//キーを押してるとき
	{
		if (lastN == false) {
			number += 1;
			if (number > 13){
				number = 1;
			}
		}
		lastN = true;
	}
	else
	{
		lastN = false;
	}
	//Mでマークを変える
	if (CheckHitKey(KEY_INPUT_M) > 0)//キーを押してるとき
	{
		if (lastM == false) {
			mark += 1;
			if (mark > 3) {
				mark = 0;
			}
			lastM = true;
		}
	}
	else
	{
		lastM = false;
	}
#endif 
}

void Trump::Draw()
{
	//52毎並べて表示
	for (int i = 0; i < cards.size(); i++) {
		//cardsのデータ通りに表示する
		DrawRectGraph(0 + i * 20, 300, 56 * cards[i].number,
			80 * cards[i].mark, 56, 80, gHandle, true);
		//
	}
	for (int i = 0; i < player1.size(); i++)
	{//プレイヤー1の表示
		if (player1[i].moving) {
			DrawRectGraph(player1[i].x,player1[i].y, 56 * player1[i].number,
				80 * player1[i].mark, 56, 80, gHandle, true);
		}
		else {
			DrawRectGraph(0 + i * 20, 50, 56 * player1[i].number,
				80 * player1[i].mark, 56, 80, gHandle, true);
		}
	}

	for (int i = 0; i < player2.size(); i++)
	{
		if (player2[i].moving) {
			DrawRectGraph(player2[i].x, player2[i].y, 56 * player2[i].number,
				80 * player2[i].mark, 56, 80, gHandle, true);
		}
		else {
			DrawRectGraph(0 + i * 20, 550, 56 * player2[i].number,
				80 * player2[i].mark, 56, 80, gHandle, true);
		}
	}
}