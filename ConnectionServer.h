#pragma once
#include "Engine/GameObject.h"

//test
class ConnectionServer :
	public GameObject
{
private:
	struct CIRCLE
	{
		int centerX;
		int centerY;
		int size;
		int color;
	};
	CIRCLE circle;

	struct Player
	{
		int MyScore = 0;
	};
public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	ConnectionServer(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
};