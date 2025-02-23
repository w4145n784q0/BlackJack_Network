#pragma once
#include "Engine/GameObject.h"
#include <vector>
#include <map>
#include <string>

using std::vector;
using std::map;
using std::string;

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
<<<<<<< HEAD
private:
=======
	//プレイヤー情報
>>>>>>> Server
	vector<PLAYER> playerInfos; //各プレイヤーの情報 0:自分 1-3: クライアント(クライアント数が変わるかもなので可変)
	PLAYER myInfo; //サーバー側 自分の情報

	//トランプ情報
	int trump[TRUMP_NUM]; //トランプの配列(各4ずつ)->選ばれたら引いていく感じ
	
	//引いたカード情報
	vector<int> myCards; //自分のカードの配列
	map<int, vector<int>> playerCards; //プレイヤーのカードの配列 key:ID value:カードの数字
	map<int, int> playerPoints; //プレイヤーの点数の配列 key:ID value:点数 0-2がクライアント(id対応) 3がディーラー

	//その他
	bool isChosenHit; //ヒットが選ばれたかどうか
	int winnerId;
public:
	PlayScene(GameObject* parent);
	~PlayScene();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void GiveCards(); //カード配る
	int Choose(int clientId); //ヒットとスタンドの処理 全員がスタンドになるまでやる！！
	void Judge(); //勝敗つける、ポイント

	//アクセス関数
	map<int, vector<int>> GetCards() { return playerCards; }
	void ChooseHit(bool hit) { isChosenHit = hit; }
	map<int, int> GetPoint() { return playerPoints; }
};


