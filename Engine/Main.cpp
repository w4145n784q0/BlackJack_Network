/// <summary>
/// DxLib を入れたバージョン
///  2023/12/6 花井
/// </summary>

#include "global.h"
#include "RootObject.h"

namespace Screen {
	static const int WIDTH = 1280;
	static const int HEIGHT = 720;
	static const char* WINDOW_NAME = "SampleGame";
	static const int WINDOW_EXTEND = 0;
	static const int WINDOW_MODE = 1;
};

// エントリーポイント
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	char StrBuf[256];    // データバッファ
	IPDATA Ip;        // 接続用ＩＰアドレスデータ
	int NetHandle;        // ネットワークハンドル
	int DataLength;        // 受信データ量保存用変数

	if (DxLib_Init() == -1)    // ＤＸライブラリ初期化処理
	{
		return -1;    // エラーが起きたら直ちに終了
	}

	// ＩＰアドレスを設定( ここにある４つのＩＰ値は仮です )
	Ip.d1 = 192;
	Ip.d2 = 168;
	Ip.d3 = 43;
	Ip.d4 = 5;

	// 通信を確立
	NetHandle = ConnectNetWork(Ip, 9850);

	// 確立が成功した場合のみ中の処理をする
	if (NetHandle != -1)
	{
		// データ送信
		NetWorkSend(NetHandle, "kon", 17);

		// データがくるのを待つ
		while (!ProcessMessage())
		{
			// 取得していない受信データ量を得る
			DataLength = GetNetWorkDataLength(NetHandle);

			// 取得してない受信データ量が０じゃない場合はループを抜ける
			if (DataLength != 0) break;
		}

		// データ受信
		NetWorkRecv(NetHandle, StrBuf, DataLength);    // データをバッファに取得

		// 受信したデータを描画
		DrawString(0, 0, StrBuf, GetColor(0, 255, 255));

		// キー入力待ち
		WaitKey();

		// 接続を断つ
		CloseNetWork(NetHandle);
	}

	SetGraphMode(Screen::WIDTH, Screen::HEIGHT, 32);
	SetOutApplicationLogValidFlag(FALSE); // ログを出さない

	SetMainWindowText(Screen::WINDOW_NAME);
	SetWindowSizeExtendRate(Screen::WINDOW_EXTEND);
	ChangeWindowMode(Screen::WINDOW_MODE); // Windowモードの場合

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}
	SetDrawScreen(DX_SCREEN_BACK);
	SetAlwaysRunFlag(TRUE);
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);

	RootObject* pRootObject = new RootObject;
	pRootObject->Initialize();

	while (true) {
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

		ScreenFlip();
	}
	pRootObject->ReleaseSub();
	delete pRootObject;

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;
}
