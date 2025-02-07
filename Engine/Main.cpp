#include "global.h"
#include "RootObject.h"

//#include <WinSock2.h>
//#include <WS2tcpip.h>
#pragma comment( lib, "ws2_32.lib" )

#include "DxLib.h"
// ポート番号
const unsigned short SERVER_PORT = 8888;
// 送受信するメッセージの最大値
const unsigned int MESSAGE_LENGTH = 1024;

namespace Screen {
	static const int WIDTH = 1280;
	static const int HEIGHT = 720;
	static const char* WINDOW_NAME = "SampleGame";
	static const int WINDOW_EXTEND = 0;
	static const int WINDOW_MODE = 1;

	const int Max_Connect = 3;
};

struct CIRCLE
{
	int id;
	int centerX;
	int centerY;
	int size;
	int color;
};

SOCKET clientSocks[3];
CIRCLE clientInfos[3];

// エントリーポイント
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//char StrBuf[256];        // データバッファ
	//int NetHandle, LostHandle;    // ネットワークハンドル
	//int DataLength;            // 受信データ量保存用変数
	//IPDATA Ip;            // 接続先ＩＰアドレスデータ
	//SetGraphMode(Screen::WIDTH, Screen::HEIGHT, 32);
	//SetOutApplicationLogValidFlag(FALSE); // ログを出さない
	//SetMainWindowText(Screen::WINDOW_NAME);
	//SetWindowSizeExtendRate(Screen::WINDOW_EXTEND);
	//ChangeWindowMode(Screen::WINDOW_MODE); // Windowモードの場合
	//if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	//{
	//	return -1;			// エラーが起きたら直ちに終了
	//}
	//SetDrawScreen(DX_SCREEN_BACK);
	//SetAlwaysRunFlag(TRUE);
	//SetUseZBuffer3D(TRUE);
	//SetWriteZBuffer3D(TRUE);

		// WinSock2.2 初期化処理
	int ret = 0;
	WSADATA wsaData;
	ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (ret != 0)
	{
		DrawString(0, 0, "Winsock error", GetColor(255, 255, 255));
		return 1;
	}

	// TCPリスンソケットの作成
	SOCKET listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSock == INVALID_SOCKET)
	{
		// エラー処理
		DrawString(0, 0, "socket error", GetColor(255, 255, 255));
		
		return 0;
	}

	// sockをノンブロッキングモードに
	u_long arg = 0x01;
	ret = ioctlsocket(listenSock, FIONBIO, &arg);

	if (ret == SOCKET_ERROR)
	{
		DrawString(0, 0, "ioctlsocket error", GetColor(255, 255, 255));
		return 0;
	}
	else
	{
		DrawString(0, 0, "ioctlsocket success", GetColor(255, 255, 255));
	}

	// 固定アドレスの割り当て
	SOCKADDR_IN bindAddr;
	memset(&bindAddr, 0, sizeof(bindAddr));		// 0クリアで初期化
	bindAddr.sin_family = AF_INET;					// IPv4アドレス使用
	bindAddr.sin_port = htons(SERVER_PORT);			// ポート番号10000指定	マジックナンバーはNG
	bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);	// すべての自分のNICが対象INADDR_ANY 特定のものだけ受け付けるなら直接IPアドレス

	if (bind(listenSock, (SOCKADDR*)&bindAddr, sizeof(bindAddr)) == SOCKET_ERROR)
	{
		DrawString(0, 0, "bind error", GetColor(255, 255, 255));
		return 0;
	}

	// リスン状態へ
	if (listen(listenSock, 3) == SOCKET_ERROR)
	{
		// エラー
		DrawString(0, 0, "listen error", GetColor(255, 255, 255));
		return 0;
	}
	int clientCount = 0;

	// DxLib周りの初期化・ウィンドウ作成処理
	SetWindowText("サーバ");
	SetGraphMode(800, 600, 32);
	ChangeWindowMode(TRUE);

	if (DxLib_Init() == -1)
		return -1;

	SetBackgroundColor(0, 0, 0);
	SetDrawScreen(DX_SCREEN_BACK);
	SetAlwaysRunFlag(1);

	RootObject* pRootObject = new RootObject;
	pRootObject->Initialize();

	/*	// 接続してくるのを待つ状態にする
		PreparationListenNetWork(9850);

		// 接続してくるかＥＳＣキーが押されるまでループ
		NetHandle = -1;
		while (!ProcessMessage() && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
		{
			// 新しい接続があったらそのネットワークハンドルを得る
			NetHandle = GetNewAcceptNetWork();
			if (NetHandle != -1) break;
		}

		// 接続されていたら次に進む
		if (NetHandle != -1)
		{
			// 接続の受付を終了する
			StopListenNetWork();

			// 接続してきたマシンのＩＰアドレスを得る
			GetNetWorkIP(NetHandle, &Ip);

			// データが送られて来るまで待つ
			while (!ProcessMessage())
			{
				// 取得していない受信データ量が０以外のときはループから抜ける
				if (GetNetWorkDataLength(NetHandle) != 0) break;
			}

			// データ受信
			DataLength = GetNetWorkDataLength(NetHandle);    // データの量を取得
			NetWorkRecv(NetHandle, StrBuf, DataLength);    // データをバッファに取得

			// 受信したデータを描画
			DrawString(0, 0, StrBuf, GetColor(255, 0, 0));

			// 受信成功のデータを送信
			NetWorkSend(NetHandle, "tunagattayo", 17);

			// 相手が通信を切断するまで待つ
			while (!ProcessMessage())
			{
				// 新たに切断されたネットワークハンドルを得る
				LostHandle = GetLostNetWork();

				// 切断された接続が今まで通信してた相手だった場合ループを抜ける
				if (LostHandle == NetHandle) break;
			}

			// 切断確認表示
			DrawString(0, 16, "setudan", GetColor(255, 255, 255));

			// キー入力待ち
			WaitKey();
		}*/



	while (true) {

		//ClearDrawScreen();
		// 初期値...画面の範囲外
		CIRCLE circle = { -100, -100, 0, GetColor(255, 255, 255) };

		if (clientCount < 3)
		{
			// 接続要求受付部
			SOCKADDR_IN fromAddr;
			int fromlen = sizeof(fromAddr);
			SOCKET sock = accept(listenSock, (SOCKADDR*)&fromAddr, &fromlen);
			// コネクション確率成功
			if (sock != INVALID_SOCKET)
			{
				// ノンブロッキングにして
				u_long arg = 0x01;
				ioctlsocket(sock, FIONBIO, &arg);
				clientSocks[clientCount] = sock;
				clientInfos[clientCount].id = clientCount;
				clientInfos[clientCount].centerX = 0;
				clientInfos[clientCount].centerY = 0;
				clientInfos[clientCount].size = 0;
				clientInfos[clientCount].color = GetColor(0, 255, 0);

				int playercount = clientCount + 1;
				DrawFormatString(0, clientCount * 200, GetColor(255, 255, 255),"Player: %d connect",playercount);
				clientCount++;
			}
			else
			{
				if (WSAGetLastError() == WSAEWOULDBLOCK)
				{
					// 接続要求なし
					DrawString(0, 0, "waiting connect...", GetColor(255, 255, 255));
				}
				else
				{
					// エラー
					DrawString(0, 0, "connect error", GetColor(255, 255, 255));
					return 0;
				}
			}
		}

		// コネクション確立済みの全クライアントからの受信部
		for (int i = 0; i < clientCount; i++)
		{
			CIRCLE circle;
			int ret = recv(clientSocks[i], (char*)&circle, sizeof(circle), 0);
			// 受信があったら
			if (ret != SOCKET_ERROR)
			{
				// バイトオーダー変換
				clientInfos[i].id = ntohl(circle.id);
				clientInfos[i].centerX = ntohl(circle.centerX);
				clientInfos[i].centerY = ntohl(circle.centerY);
				clientInfos[i].size = ntohl(circle.size);
				clientInfos[i].color = ntohl(circle.color);
				DrawFormatString(0, 100, GetColor(255, 255, 255),"connectID: %d");
			}
		}

		// 送信データの作成
		CIRCLE sendPackets[3];
		for (int i = 0; i < 3; i++)
		{
			sendPackets[i].id = htonl(clientInfos[i].id);
			sendPackets[i].centerX = htonl(clientInfos[i].centerX);
			sendPackets[i].centerY = htonl(clientInfos[i].centerY);
			sendPackets[i].size = htonl(clientInfos[i].size);
			sendPackets[i].color = htonl(clientInfos[i].color);
		}

		// コネクション確立済みの全クライアントへ送信
		for (int i = 0; i < clientCount; i++)
		{
			int ret = send(clientSocks[i], (char*)sendPackets, sizeof(sendPackets), 0);
			if (ret != SOCKET_ERROR)
			{
				// 送信成功
				//DrawString(0, 100, "Send" ,GetColor(255, 255, 255));
			}
			else
			{
				if (WSAGetLastError() == WSAEWOULDBLOCK)
				{
					// 未送信
				}
				else
				{
					// エラー
				}
			}
		}

		// 描画
		for (int i = 0; i < 3; i++)
		{
			DrawCircle(clientInfos[i].centerX, clientInfos[i].centerY, clientInfos[i].size, clientInfos[i].color, 1);
		}


		//DrawString();

		ScreenFlip();
		WaitTimer(16);
		if (ProcessMessage() == -1 || CheckHitKey(KEY_INPUT_ESCAPE) == 1)
		{
			break;
		}


		//ここから下は通常

	//全オブジェクトの更新処理
	//ルートオブジェクトのUpdateを呼んだあと、自動的に子、孫のUpdateが呼ばれる
		pRootObject->UpdateSub();

		if (DxLib::ProcessMessage() == -1 /*|| AppIsExit()*/)
			break;

		ClearDrawScreen();
		//全オブジェクトを描画
		//ルートオブジェクトのDrawを呼んだあと、自動的に子、孫のUpdateが呼ばれる
		pRootObject->DrawSub();

		RefreshDxLibDirect3DSetting();

		//ScreenFlip();
	}
	pRootObject->ReleaseSub();
	delete pRootObject;

	DxLib_End();		// ＤＸライブラリ使用の終了処理

	return 0;
}