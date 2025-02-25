#include "TestScene.h"
#include"Trump.h"
#include"ConnectionServer.h"

//コンストラクタ
//test(Client班より)
TestScene::TestScene(GameObject * parent)
	: GameObject(parent, "TestScene")
{
}

//初期化
void TestScene::Initialize()
{
	Instantiate<ConnectionServer>(this);
	//Instantiate<ConnectionClient>(this);

	//Instantiate<Trump>(this);
	//Instantiate<Puzzle>(this);
}

//更新
void TestScene::Update()
{
}

//描画
void TestScene::Draw()
{
}

//開放
void TestScene::Release()
{
	
}
