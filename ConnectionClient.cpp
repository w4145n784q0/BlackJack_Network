#include "ConnectionClient.h"

ConnectionClient::ConnectionClient(GameObject* parent)
	:GameObject(parent, "ConnectionClient")
{
	int col = GetColor(122, 196, 31);
	circle = { 0, 0, 20, col };
}

void ConnectionClient::Initialize()
{
}

void ConnectionClient::Update()
{
}

void ConnectionClient::Draw()
{
	// マウス座標取得し、circleのcenterXとcenterTに格納
	GetMousePoint(&circle.centerX, &circle.centerY);
	// 描画
	DrawCircle(circle.centerX, circle.centerY, circle.size, circle.color, 1);
}

void ConnectionClient::Release()
{
}
