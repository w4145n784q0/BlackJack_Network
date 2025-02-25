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

	bool Connect = false;
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

struct PLAYER
{
	int id;//プレイヤーID
	int MyCardNum;//自分の持ってるカードの総数
	int MyScore;//自分の持ってるスコア
	bool isMyTurn;//自分のターンか判定用
	bool isHit;//自分がヒットをつかえるか確認用
	bool isStand;//自分がスタンドをつかえるか確認用
};

SOCKET clientSocks[3];
//CIRCLE clientInfos[3];
PLAYER clientCard[3];

// エントリーポイント
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
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

	/*SceneManager s=pRootObject->FindChildObject("SceneManager");*/

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


	while (true) 
	{

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

void DrawMessage()
{

}

void AddMessage(int x, int y)
{

}