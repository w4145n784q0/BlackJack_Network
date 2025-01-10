#include "ConnectionServer.h"

ConnectionServer::ConnectionServer(GameObject* parent)
	:GameObject(parent, "ConnectionServer")
{
	int col = GetColor(122, 196, 31);
	circle = { 0, 0, 20, col};
}

void ConnectionServer::Initialize()
{
}

void ConnectionServer::Update()
{
}

void ConnectionServer::Draw()
{
	// マウス座標取得し、circleのcenterXとcenterTに格納
	GetMousePoint(&circle.centerX, &circle.centerY);
	// 描画
	DrawCircle(circle.centerX, circle.centerY, circle.size, circle.color, 1);
}

void ConnectionServer::Release()
{
}
