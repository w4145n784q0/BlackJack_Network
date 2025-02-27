#pragma once
#include "Engine/GameObject.h"
#include"Engine/SceneManager.h"
class WaitScene :
	public GameObject
{
	// サーバのIPアドレス
	const char* SERVER_ADDRESS = "192.168.42.17";
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

	PLAYER PlayerData[4];
	SOCKET sock;
public:
	WaitScene(GameObject* parent);
	~WaitScene();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

