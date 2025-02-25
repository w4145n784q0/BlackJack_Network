#pragma once
#include "Engine/GameObject.h"
class WaitScene :
    public GameObject
{
private:
	// ポート番号
	const unsigned short SERVER_PORT = 8888;
	// 送受信するメッセージの最大値
	const unsigned int MESSAGE_LENGTH = 1024;

	struct PLAYER
	{
		int id;//プレイヤーID
		int MyCardNum;//自分の持ってるカードの総数
		int MyScore;//自分の持ってるスコア
		bool isMyTurn;//自分のターンか判定用
		bool isHit;//自分がヒットをつかえるか確認用
		bool isStand;//自分がスタンドをつかえるか確認用
	};
	SOCKET clientSocks[3];
	//CIRCLE clientInfos[3];
	PLAYER clientCard[3];
	int clientCount = 0;
	SOCKET listenSock;
public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	WaitScene(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
};

