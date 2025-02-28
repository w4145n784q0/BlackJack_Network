#pragma once
#include "Engine/GameObject.h"

/// <summary>
/// ゲームの処理　待機・プレイ画面を担う
/// </summary>
class Game :public GameObject
{
public:
	// サーバのIPアドレス
	const char* SERVER_ADDRESS = "192.168.42.80";
	// サーバのポート番号
	const unsigned short SERVER_PORT = 8888;

	//ID            :プレイヤーID
    //MyCord  :自分の持ってるカードの総数
    //MyScore :自分の持ってるScore
    //MyTurn   :自分のターンか判定
    //isHit        :自分がヒットを使えるか判定
    //isStand   :自分がスタンドを使えるか判定
	struct PLAYER
	{
		int id;//プレイヤーID
		int MyCardNum;//自分の持ってるカードの総数
		int MyScore;//自分の持ってるスコア
		bool isMyTurn;//自分のターンか判定用
		bool isHit;//自分がヒットをつかえるか確認用
		bool isStand;//自分がスタンドをつかえるか確認用
	};

	enum state
	{
		s_Wait,
		s_Play
	};
	state GameState;
	
	PLAYER PlayerData[4];
	SOCKET sock;


	Game(GameObject* parent);
	~Game();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	//待機時の処理
	void UpdateWait();

	//ゲーム中の処理
	void UpdatePlay();

	//待機時の描画処理
	void DrawWait();

	//ゲーム中の描画処理
	void DrawPlay();
};

