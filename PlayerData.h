#pragma once
#include "Engine/GameObject.h"

//プレイヤーのデータをグローバルに保管
class PlayerData :
	public GameObject
{
public:
	struct PLAYER
	{
		int id;//プレイヤーID
		int MyCardNum;//自分の持ってるカードの合計値
		bool isHit; //hitかどうか　trueならカードを引く（暫定）
		bool isStand;//standかどうか　全員trueなら勝敗処理へ
		bool isConnect;//接続できたか（playシーン移行時のみ使う）
	};
	PLAYER player;
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	PlayerData(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	void SetPlayerData(PLAYER p) { 
		player.id = p.id,
		player.MyCardNum = p.MyCardNum,
		player.isHit = p.isHit;
		player.isStand = p.isStand,
		player.isConnect = p.isConnect;
	}
};

