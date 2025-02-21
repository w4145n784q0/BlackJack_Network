#include "PlayScene.h"

namespace
{

}

PlayScene::PlayScene()
{
	/* これグローバル
	struct PLAYER
{
	int id;//プレイヤーID
	int MyCardNum;//自分の持ってるカードの総数
	int MyScore;//自分の持ってるスコア
	bool isMyTurn;//自分のターンか判定用
	bool isHit;//自分がヒットをつかえるか確認用
	bool isStand;//自分がスタンドをつかえるか確認用
};
	*/


	//かりそめの初期化
	playerInfos.push_back({1,0,0,0,0,0});
	playerInfos.push_back({2,0,0,0,0,0});
	playerInfos.push_back({3,0,0,0,0,0});
	myInfo = { -1,0,0,0,0,0 };

	for (int tr=0;tr<TRUMP_NUM;tr++)
	{
		trump[tr] = 4; //トランプを4で初期化
	}


}

PlayScene::~PlayScene()
{
}

void PlayScene::Initialize()
{
	//ここで受け取りたい
}

void PlayScene::Update()
{
	/*
	自分のstruct/mainから送られてきたstruct3<=配列とかにいれる？

	
	*/
}

void PlayScene::Draw()
{
}

void PlayScene::Release()
{
}

void PlayScene::GiveCards()
{
	//////////カードを配る//////////
	
	//ディーラー
	while (true)
	{
		myCard1 = rand() % 13 + 1; //1~13の数字を生成
		trump[myCard1 - 1]--; //番号のカードを一枚減らす
		if (trump[myCard1 - 1] >= 0)
		{
			break;
		}
	}
	while (true)
	{
		myCard2 = rand() % 13 + 1; //1~13の数字を生成
		trump[myCard2 - 1]--; //番号のカードを一枚減らす
		if (trump[myCard2 - 1] >= 0)
		{
			break;
		}
	}

	//クライアント
	for (int i = 0; i< playerInfos.size(); i++)
	{
		while (true)
		{	
			card1[i] = rand() % 13 + 1; //1~13の数字を生成
			trump[card1[i] - 1]--; //番号のカードを一枚減らす
			if (trump[card1[i] - 1] >= 0) //残りが0枚以上
			{
				break; //正常に選ばれていたら終了
			}
		}
	
		while (true)
		{
			//カードが正常に選ばれたか
			card2[i] = rand() % 13 + 1; //1~13の数字を生成
			trump[card2[i] - 1]--; //番号のカードを一枚減らす
			if (trump[card2[i] - 1] >= 0) //残りが0枚以上
			{
				break; //正常に選ばれていたら終了
			}
		}
		//////////カードを配るおわり//////////



		
	}
}

void PlayScene::Choose()
{
	//ディーラーは？
	
	//ヒットは満足するまで
	for (int i = 0; i < playerInfos.size(); i++)
	{

	}
}

void PlayScene::Judge()
{
	//選んだ数字を送る
	/*	if (card1 > 10)
	{
		card1 = 10;
	}
	if (card2 > 10)
	{
		card2 = 10;
	}
*/
}
