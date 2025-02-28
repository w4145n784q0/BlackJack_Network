//#include "WaitScene.h"
//#include"Engine/SceneManager.h"
//#include"PlayScene.h"
//
//WaitScene::WaitScene(GameObject* parent)
//	: GameObject(parent, "WaitScene")
//{
//}
//
//void WaitScene::Initialize()
//{
//	// WinSock2.2 初期化処理
//	int ret = 0;
//	WSADATA wsaData;
//	ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
//	if (ret != 0)
//	{
//		DrawString(0, 0, "Winsock error", GetColor(255, 255, 255));
//		return;
//	}
//
//	// TCPリスンソケットの作成
//	listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//	if (listenSock == INVALID_SOCKET)
//	{
//		// エラー処理
//		DrawString(0, 0, "socket error", GetColor(255, 255, 255));
//
//		return;
//	}
//
//	// sockをノンブロッキングモードに
//	u_long arg = 0x01;
//	ret = ioctlsocket(listenSock, FIONBIO, &arg);
//
//	if (ret == SOCKET_ERROR)
//	{
//		DrawString(0, 0, "ioctlsocket error", GetColor(255, 255, 255));
//		return;
//	}
//	else
//	{
//		DrawString(0, 0, "ioctlsocket success", GetColor(255, 255, 255));
//	}
//
//	// 固定アドレスの割り当て
//	SOCKADDR_IN bindAddr;
//	memset(&bindAddr, 0, sizeof(bindAddr));		// 0クリアで初期化
//	bindAddr.sin_family = AF_INET;					// IPv4アドレス使用
//	bindAddr.sin_port = htons(SERVER_PORT);			// ポート番号10000指定	マジックナンバーはNG
//	bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);	// すべての自分のNICが対象INADDR_ANY 特定のものだけ受け付けるなら直接IPアドレス
//
//	if (bind(listenSock, (SOCKADDR*)&bindAddr, sizeof(bindAddr)) == SOCKET_ERROR)
//	{
//		DrawString(0, 0, "bind error", GetColor(255, 255, 255));
//		return;
//	}
//
//	// リスン状態へ
//	if (listen(listenSock, 3) == SOCKET_ERROR)
//	{
//		// エラー
//		DrawString(0, 0, "listen error", GetColor(255, 255, 255));
//		return;
//	}
//}
//
//void WaitScene::Update()
//{
//	if (clientCount == 3)
//	{
//		PLAYER Packets[3];
//		for (int i = 0; i < clientCount; i++)
//		{
//			//int ret = send(clientSocks[i], (char*)sendPackets, sizeof(sendPackets), 0);
//			Packets[i].id = i;
//			Packets[i].MyCardNum = 0;
//			Packets[i].isHit = false;
//			Packets[i].isStand = false;
//			Packets[i].isConnect = true;
//
//			int ret = send(clientSocks[i], (char*)Packets, sizeof(Packets), 0);
//			if (ret != SOCKET_ERROR)
//			{
//				// 送信成功
//				DrawFormatString(0, 150, GetColor(255, 255, 255), "Scene move", i + 1);
//			}
//			else
//			{
//				if (WSAGetLastError() == WSAEWOULDBLOCK)
//				{
//					// 未送信
//				}
//				else
//				{
//					// エラー
//				}
//			}
//			SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
//			pSceneManager->ChangeScene(SCENE_ID_PLAY);
//		}
//
//		//ClearDrawScreen();
//		// 初期値...画面の範囲外
//		//CIRCLE circle = { -100, -100, 0, GetColor(255, 255, 255) };
//
//		if (clientCount < 3)
//		{
//			// 接続要求受付部
//			SOCKADDR_IN fromAddr;
//			int fromlen = sizeof(fromAddr);
//			SOCKET sock = accept(listenSock, (SOCKADDR*)&fromAddr, &fromlen);
//			// コネクション確率成功
//			if (sock != INVALID_SOCKET)
//			{
//				// ノンブロッキングにして
//				u_long arg = 0x01;
//				ioctlsocket(sock, FIONBIO, &arg);
//				clientSocks[clientCount] = sock;
//				/*clientInfos[clientCount].id = clientCount;
//				clientInfos[clientCount].centerX = 0;
//				clientInfos[clientCount].centerY = 0;
//				clientInfos[clientCount].size = 0;
//				clientInfos[clientCount].color = GetColor(0, 255, 0);*/
//
//				clientCard[clientCount].id = clientCount;
//				clientCard[clientCount].MyCardNum = 0;
//				clientCard[clientCount].isHit = false;
//				clientCard[clientCount].isStand = false;
//				clientCard[clientCount].isConnect = true;
//
//				//DrawFormatString(0, clientCount * 25, GetColor(255, 255, 255), "Player: %d connect", playercount);
//				clientCount++;
//			}
//			else
//			{
//				if (WSAGetLastError() == WSAEWOULDBLOCK)
//				{
//					// 接続要求なし
//					DrawString(0, 0, "waiting connect...", GetColor(255, 255, 255));
//				}
//				else
//				{
//					// エラー
//					DrawString(0, 0, "connect error", GetColor(255, 255, 255));
//					return;
//				}
//			}
//		}
//
//		// コネクション確立済みの全クライアントからの受信部
//		for (int i = 0; i < clientCount; i++)
//		{
//			//受信用
//			PLAYER player;
//			//int ret = recv(clientSocks[i], (char*)&circle, sizeof(circle), 0);
//			int ret = recv(clientSocks[i], (char*)&player, sizeof(player), 0);
//			// 受信があったら
//			if (ret != SOCKET_ERROR)
//			{
//				// バイトオーダー変換
//				/*clientInfos[i].id = ntohl(circle.id);
//				clientInfos[i].centerX = ntohl(circle.centerX);
//				clientInfos[i].centerY = ntohl(circle.centerY);
//				clientInfos[i].size = ntohl(circle.size);
//				clientInfos[i].color = ntohl(circle.color);*/
//
//				clientCard[i].id = ntohl(player.id);
//				clientCard[i].MyCardNum = ntohl(player.MyCardNum);
//				clientCard[i].isHit = ntohl(player.isHit);
//				clientCard[i].isStand = ntohl(player.isStand);
//				clientCard[i].isConnect = ntohl(player.isConnect);
//				//DrawFormatString(0, (i+1) * 25, GetColor(255, 255, 255), "Player:%d connected", i + 1);
//
//				//DrawFormatString(0, (i + 1) * 25, GetColor(255, 255, 255), "PlayerMYCard:%d connected", clientCard[i].MyCardNum);
//				//DrawFormatString(0, 100, GetColor(255, 255, 255), "connectID: %d", clientInfos[i].id);
//			}
//		}
//
//		// 送信データの作成
//		//CIRCLE sendPackets[3];
//		//PLAYER Packets[3];
//
//		for (int i = 0; i < 3; i++)
//		{
//			/*sendPackets[i].id = htonl(clientInfos[i].id);
//			sendPackets[i].centerX = htonl(clientInfos[i].centerX);
//			sendPackets[i].centerY = htonl(clientInfos[i].centerY);
//			sendPackets[i].size = htonl(clientInfos[i].size);
//			sendPackets[i].color = htonl(clientInfos[i].color);*/
//
//
//			Packets[i].id = htonl(clientCard[clientCount].id);
//			Packets[i].MyCardNum = htonl(clientCard[clientCount].MyCardNum);
//			Packets[i].isHit = htonl(clientCard[clientCount].isHit);
//			Packets[i].isStand = htonl(clientCard[clientCount].isStand);
//			Packets[i].isConnect = htonl(clientCard[clientCount].isConnect);
//
//		}
//
//		// コネクション確立済みの全クライアントへ送信
//		for (int i = 0; i < clientCount; i++)
//		{
//			//int ret = send(clientSocks[i], (char*)sendPackets, sizeof(sendPackets), 0);
//			int ret = send(clientSocks[i], (char*)Packets, sizeof(Packets), 0);
//			if (ret != SOCKET_ERROR)
//			{
//				// 送信成功
//				DrawFormatString(0, 150, GetColor(255, 255, 255), "send: Player%d", i + 1);
//				//DrawString(0, 150, "Send" ,GetColor(255, 255, 255));
//			}
//			else
//			{
//				if (WSAGetLastError() == WSAEWOULDBLOCK)
//				{
//					// 未送信
//				}
//				else
//				{
//					// エラー
//				}
//			}
//		}
//	}
//}
//
//void WaitScene::Draw()
//{
//	for (int i = 0; i < clientCount; i++)
//	{
//		if(clientCount > 0)
//			DrawFormatString(0, (i + 1) * 25, GetColor(255, 255, 255), "PlayerID:%d connected", clientCard[i].id);
//	}
//}
//
//void WaitScene::Release()
//{
//}
