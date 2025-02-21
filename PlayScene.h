#pragma once
#include "Engine/GameObject.h"
#include <vector>

using std::vector;

//動作確認用
struct PLAYER
{
	int id;//プレイヤーID
	int MyCardNum;//自分の持ってるカードの総数
	int MyScore;//自分の持ってるスコア
	bool isMyTurn;//自分のターンか判定用
	bool isHit;//自分がヒットをつかえるか確認用
	bool isStand;//自分がスタンドをつかえるか確認用
};

namespace
{
	const int TRUMP_NUM = 13;
}

class PlayScene :
	public GameObject
{
private:
	vector<PLAYER> playerInfos; //各プレイヤーの情報 0:自分 1-3: クライアント(クライアント数が変わるかもなので可変)
	PLAYER myInfo; //サーバー側 自分の情報
	int trump[TRUMP_NUM]; //トランプの配列(各4ずつ)->選ばれたら引いていく感じ
	vector<int> card1; //一枚目に引いたカードの数字の配列
	vector<int> card2; //二枚目に引いたカードの配列
	int myCard1; //ディーラーのカード1の番号
	int myCard2; //ディーラーのカード2の番号
	int hitCard; //ヒットしたカードの番号
	//ヒットしたカードを保存しておくなにかが必要

public:
	PlayScene(GameObject* parent);
	~PlayScene();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void GiveCards(); //カード配る
	void Choose(); //ヒットかスタンドか選ぶ 全員がスタンドになるまでやる！！
	void Judge(); //勝敗つける、ポイント


	//アクセス関数
	vector<int> GetCard1Num() { return card1; } //一枚目に引いたカードの数を返す
	vector<int> GetCard2Num() { return card2; } //二枚目に引いたカードの数を返す
	int GetHitCardNum() { return hitCard; } //ヒットした時のカードの数を返す
	
};


