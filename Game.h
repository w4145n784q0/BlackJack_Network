#pragma once
#include "Engine/GameObject.h"
#include <vector>
#include <map>
#include <string>

using std::vector;
using std::map;
using std::string;

namespace
{
	const int TRUMP_NUM = 13; //トランプの番号
	const int TRUMP_MARK_NUM = 4; //トランプの種類
}

class Game : public GameObject

{
private:
	/////////////////////////接続シーン用/////////////////////////
	// ポート番号
	const unsigned short SERVER_PORT = 8888;
	// 送受信するメッセージの最大値
	const unsigned int MESSAGE_LENGTH = 1024;

	//プレイヤーの情報
	//id       : プレイヤーID
	//MyCardNum: 自分の持ってるカードの合計値
	//isHit    : hitかどうか　trueならカードを引く（暫定）
	//isStand  : standかどうか　全員trueなら勝敗処理へ
	//isConnect: 接続できたか
	struct PLAYER
	{
		int id;//プレイヤーID
		int MyCardNum;//自分の持ってるカードの合計値
		bool isHit; //hitかどうか　trueならカードを引く（暫定）
		bool isStand;//standかどうか　全員trueなら勝敗処理へ
		bool isConnect;//接続できたか
	};
	SOCKET clientSocks[3];
	SOCKET listenSock;
	PLAYER clientCard[3];
	int clientCount = 0;


	//ゲーム内のシーン（Gameクラス内で完結）
	enum State {
		s_wait,
		s_play,
	};
	State GameState;

	/////////////////////////プレイシーン用/////////////////////////
	//プレイヤー情報
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

public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	Game(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	void InitWait();
	void InitPlay();

	void UpdateWait();//接続を待つ関数
	void UpdatePlay();//ゲームプレイ中

	void DrawWait();
	void DrawPlay();

	/////////////////////////プレイシーン用/////////////////////////
	void GiveCards(); //カード配る
	int Choose(int clientId); //ヒットとスタンドの処理 全員がスタンドになるまでやる！！
	void Judge(); //勝敗つける、ポイント

	//アクセス関数
	map<int, vector<int>> GetCards() { return playerCards; } //プレイヤーのカードの配列
	void ChooseHit(bool hit) { isChosenHit = hit; } //ヒットが選ばれたか
	map<int, int> GetPoint() { return playerPoints; } //プレイヤー+ディーラーの点数の配列
};