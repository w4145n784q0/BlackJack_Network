
#include <iostream>
#include <vector>
#include <string>

using std::string;

#pragma comment( lib, "ws2_32.lib" )
///#include <WS2tcpip.h>
//#include <WinSock2.h>


#include "DxLib.h"
#include <iostream>



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	

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

	

		ScreenFlip();

		WaitTimer(16);
		if (ProcessMessage() == -1 || CheckHitKey(KEY_INPUT_ESCAPE) == 1)
		{
			break;
		}


	}
	/*closesocket(sock);

	if (WSACleanup() != 0)
	{
		return -1;
	}*/
	DxLib_End();

	return 0;
}