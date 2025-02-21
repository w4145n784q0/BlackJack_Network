#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <vector>
#include <string>

using std::string;

#pragma comment( lib, "ws2_32.lib" )

#include "DxLib.h"
#include <iostream>


// サーバのIPアドレス
const char* SERVER_ADDRESS = "192.168.43.5";
// サーバのポート番号
const unsigned short SERVER_PORT = 8888;

//struct CIRCLE
//{
//	int id;
//	int centerX;
//	int centerY;
//	int size;
//	int color;
//};
 

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


//CIRCLE clientInfos[3];

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WSADATA	wsaData;
	// WinSock2.2 初期化処理
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		DrawString(0, 0, "winsock初期化", GetColor(255, 255, 255));
		return -1;
	}
	DrawString(0, 10, "winsock成功", GetColor(255, 255, 255));
	// ソケットの作成
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
	{
		DrawString(0, 20, "ソケットの作成", GetColor(255, 255, 255));
		return -1;
	}
	DrawString(0, 30, "socket成功", GetColor(255, 255, 255));

	//ノンブロッキングモード
	u_long arg = 0x01;
	int ret = ioctlsocket(sock, FIONBIO, &arg);
	if (ret == SOCKET_ERROR)
	{
		//エラー処理
		DrawString(0, 40, "nonblockingmode", GetColor(255, 255, 255));
		return -1;
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


	// DxLib周りの初期化・ウィンドウ作成処理
	SetWindowText("クライアント");
	SetGraphMode(800, 600, 32);
	ChangeWindowMode(TRUE);

	if (DxLib_Init() == -1)
	{
		DrawString(0, 90, "dxlib初期化", GetColor(255, 255, 255));
		return -1;
	}
	SetOutApplicationLogValidFlag(FALSE);
	SetBackgroundColor(0, 0, 0);
	SetDrawScreen(DX_SCREEN_BACK);
	SetAlwaysRunFlag(1);


	while (1)
	{
		ClearDrawScreen();

		//circleの描画に関する処理
		/*
		// サイズとか色はお任せ
		// 描画されない原因はcircleの構造体初期値一つ足りませんでした(すみません....)

		CIRCLE circle = { 50, 150, 10, 50, GetColor(0,255,255) };
		// マウス座標取得し、circleのcenterXとcenterTに格納
		GetMousePoint(&circle.centerX, &circle.centerY);
		// 描画
		DrawCircle(circle.centerX, circle.centerY, circle.size, circle.color, 1);

		// サーバ( serverAddr )に●の情報送信
		CIRCLE sendbuff = { htonl(circle.id),htonl(circle.centerX),htonl(circle.centerY),htonl(circle.size),htonl(circle.color) };
		int ret = send(sock, (char*)&sendbuff, sizeof(sendbuff), 0);
		*/

		//合ってるか分らんけどとりあえず手を動かしてみる
		PLAYER player = { 1, 52, 0, 0, 0, 0 };//初期値設定

		
		PLAYER sendbuff = { htonl(player.id),htonl(player.MyCardNum),htonl(player.MyScore),
		                   htonl(player.isMyTurn),htonl(player.isHit),htonl(player.isStand) };

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
		if (ret != SOCKET_ERROR)
		{
			for (int i = 0; i < 4; i++)
			{
				PlayerData[i].id = ntohl(recvPacket[i].id);
				PlayerData[i].MyCardNum = ntohl(recvPacket[i].MyCardNum);
				PlayerData[i].MyScore = ntohl(recvPacket[i].MyScore);
				PlayerData[i].isMyTurn = ntohl(recvPacket[i].isMyTurn);
				PlayerData[i].isHit = ntohl(recvPacket[i].isHit);
				PlayerData[i].isStand = ntohl(recvPacket[i].isStand);


				DrawFormatString(200, 0, GetColor(255, 255, 255), "Player Id:%d", PlayerData[i].id);
				DrawFormatString(200, 30, GetColor(255, 255, 255), "The number of Mycards ALL:%d", PlayerData[i].MyCardNum);
				DrawFormatString(200, 60, GetColor(255, 255, 255), "MyTurn:%d", PlayerData[i].isMyTurn);
				DrawFormatString(200, 90, GetColor(255, 255, 255), "MyScore:%d", PlayerData[i].MyScore);
				DrawFormatString(200, 120, GetColor(255, 255, 255), "is Hit?:%d", PlayerData[i].isHit);
				DrawFormatString(200, 140, GetColor(255, 255, 255), "is Stand?:%d", PlayerData[i].isStand);
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
				return -1;
			}
		}

		ScreenFlip();

		WaitTimer(16);
		if (ProcessMessage() == -1 || CheckHitKey(KEY_INPUT_ESCAPE) == 1)
		{
			break;
		}


	}
	closesocket(sock);

	if (WSACleanup() != 0)
	{
		return -1;
	}
	DxLib_End();

	return 0;
}