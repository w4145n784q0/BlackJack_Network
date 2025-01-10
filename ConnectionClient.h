#pragma once
#include "Engine/GameObject.h"
class ConnectionClient :
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
public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	ConnectionClient(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
};

