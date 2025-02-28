#include <WinSock2.h>
#include <ws2tcpip.h>
#include "Game.h"
#include "Trump.h"

Game::Game(GameObject* parent)
	:GameObject(parent,"Game")
{
	GameState = s_wait;
}

Game::~Game()
{
}

void Game::Initialize()
{
	WSADATA	wsaData;
	// WinSock2.2 初期化処理
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		DrawString(0, 0, "winsock初期化", GetColor(255, 255, 255));
	}
	DrawString(0, 10, "winsock成功", GetColor(255, 255, 255));
	// ソケットの作成
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
	{
		DrawString(0, 20, "ソケットの作成", GetColor(255, 255, 255));
	}
	DrawString(0, 30, "socket成功", GetColor(255, 255, 255));

	//ノンブロッキングモード
	u_long arg = 0x01;
	int ret = ioctlsocket(sock, FIONBIO, &arg);
	if (ret == SOCKET_ERROR)
	{
		//エラー処理
		DrawString(0, 40, "nonblockingmode", GetColor(255, 255, 255));
	}
	DrawString(0, 50, "nonblockingmode 成功", GetColor(255, 255, 255));

	// サーバアドレスの指定
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, SERVER_ADDRESS, &serverAddr.sin_addr.s_addr);
	DrawString(0, 60, "サーバアドレス指定成功", GetColor(255, 255, 255));

	if (connect(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			// 接続要求失敗
			DrawString(0, 70, "接続要求に失敗しました", GetColor(255, 255, 255));
		}
	}

	DrawString(0, 80, "接続成功", GetColor(255, 255, 255));
}

void Game::Update()
{
	switch (GameState)
	{
	case Game::s_wait:
		UpdateWait();
		break;
	case Game::s_play:
		UpdatePlay();
		break;
	default:
		break;
	}
}

void Game::Draw()
{
	switch (GameState)
	{
	case Game::s_wait:
		DrawWait();
		
		break;
	case Game::s_play:
		DrawPlay();
		break;
	default:
		break;
	}
}

void Game::Release()
{
}

void Game::UpdateWait()
{
	//合ってるか分らんけどとりあえず手を動かしてみる
	PLAYER player = { 1, 52, 0, 0, 0};//初期値設定


	PLAYER sendbuff = { htonl(player.id),htonl(player.MyCardNum),htonl(player.isHit),
					   htonl(player.isStand),htonl(player.isConnect)};
	char Serversend[1024];

	int bytesReceived = send(sock, Serversend, sizeof(sendbuff), 0);
	int ret = send(sock, (char*)&sendbuff, sizeof(sendbuff), 0);

	if (ret != SOCKET_ERROR)
	{
		// 送信できた
		DrawString(0, 100, "送信できた", GetColor(255, 255, 255));
	}
	else
	{
		if (WSAGetLastError() == WSAEWOULDBLOCK)
		{
			// 未送信
			DrawString(0, 110, "未送信", GetColor(255, 255, 255));
		}
		else
		{
			// エラー
			DrawString(0, 120, "送信エラー", GetColor(255, 255, 255));
		}
	}

	// サーバから受信
	PLAYER recvPacket[4];
	ret = recv(sock, (char*)recvPacket, sizeof(recvPacket), 0);
	bytesReceived = recv(sock, Serversend, sizeof(Serversend) -1, 0);
	//Serversend[bytesReceived] = '\0';

	if (ret != SOCKET_ERROR)
	{
		for (int i = 0; i < 4; i++)
		{
			PlayerData[i].id = ntohl(recvPacket[i].id);
			PlayerData[i].MyCardNum = ntohl(recvPacket[i].MyCardNum);
			PlayerData[i].isHit = ntohl(recvPacket[i].isHit);
			PlayerData[i].isStand = ntohl(recvPacket[i].isStand);
			PlayerData[i].isConnect = ntohl(recvPacket[i].isConnect);


			/*DrawFormatString(200, 0, GetColor(255, 255, 255), "Player Id:%d", PlayerData[i].id);
			DrawFormatString(200, 30, GetColor(255, 255, 255), "The number of Mycards ALL:%d", PlayerData[i].MyCardNum);
			DrawFormatString(200, 60, GetColor(255, 255, 255), "MyTurn:%d", PlayerData[i].isMyTurn);
			DrawFormatString(200, 90, GetColor(255, 255, 255), "MyScore:%d", PlayerData[i].MyScore);
			DrawFormatString(200, 120, GetColor(255, 255, 255), "is Hit?:%d", PlayerData[i].isHit);
			DrawFormatString(200, 140, GetColor(255, 255, 255), "is Stand?:%d", PlayerData[i].isStand);*/
		}
	}
	else
	{
		if (WSAGetLastError() == WSAEWOULDBLOCK)
		{
			// 未受信
			DrawString(0, 130, "未受信", GetColor(255, 255, 255));
		}
		else
		{
			DrawString(0, 140, "未受信かつエラー", GetColor(255, 255, 255));
		}
	}

	//sock, (char*)recvPacket, sizeof(recvPacket), 0)
	

	// Serverから接続完了のデータがある場合
	if (bytesReceived != SOCKET_ERROR) {
		//Serversend[bytesReceived] = '\0'; // NULL終端

		// サーバーからの特定のメッセージに基づいて処理を行う
		//if (strcmp(Serversend, "connect") == 0) 
		//{
			GameState = s_play;
			DrawString(0, 130, "The scene has been switched! You can now load the new scene.", GetColor(255, 255, 255));
			Instantiate<Trump>(this);
		//}
	}
	// エラー処理や接続切れを確認することもできます
	else if (bytesReceived == 0) {
		DrawString(0, 130, "Server disconnected.", GetColor(255, 255, 255));
	}
	else {
		DrawString(0, 130, "recv failed with error:", GetColor(255, 255, 255));
	}
}

void Game::UpdatePlay()
{
	
}

void Game::DrawWait()
{

}

void Game::DrawPlay()
{
	DrawString(0, 130, "recv:", GetColor(255, 255, 255));
}
