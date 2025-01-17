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
	char StrBuf[256];        // データバッファ
	int NetHandle, LostHandle;    // ネットワークハンドル
	int DataLength;            // 受信データ量保存用変数
	IPDATA Ip;            // 接続先ＩＰアドレスデータ


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

	// 接続してくるのを待つ状態にする
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
	}

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
