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

	struct PLAYER
	{
		int id;//プレイヤーID
		int MyCardNum;//自分の持ってるカードの合計値
		bool isHit; //hitかどうか　trueならカードを引く（暫定）
		bool isStand;//standかどうか　全員trueなら勝敗処理へ
		bool isConnect;//接続できたか（playシーン移行時のみ使う）
	};

	enum state
	{
		s_wait,
		s_play
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

