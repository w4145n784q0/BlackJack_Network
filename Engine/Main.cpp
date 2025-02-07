#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <vector>
#pragma comment( lib, "ws2_32.lib" )

#include "DxLib.h"
#include <iostream>


// サーバのIPアドレス
const char* SERVER_ADDRESS = "192.168.43.5";
// サーバのポート番号
const unsigned short SERVER_PORT = 8888;

struct CIRCLE
{
    int id;
    int centerX;
    int centerY;
    int size;
    int color;

};

CIRCLE clientInfos[3];

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    //
    WSADATA	wsaData;
    // WinSock2.2 初期化処理
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        return 0;
    }

    // ソケットの作成
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET)
    {
        return 0;
    }

    //ノンブロッキングモード
    u_long arg = 0x01;
    int ret = ioctlsocket(sock, FIONBIO, &arg);
    if (ret == SOCKET_ERROR)
    {
        //エラー処理
        return -1;
    }

    // サーバアドレスの指定
    SOCKADDR_IN serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDRESS, &serverAddr.sin_addr.s_addr);


    if (connect(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSAEWOULDBLOCK)
        {
            // 接続要求失敗
            std::cout << "error:connect()" << WSAGetLastError() << std::endl;
        }
    }

    std::cout << "succes:connect()" << std::endl;


    // DxLib周りの初期化・ウィンドウ作成処理
    SetWindowText("クライアント");
    SetGraphMode(800, 600, 32);
    ChangeWindowMode(TRUE);

    if (DxLib_Init() == -1)
        return -1;

    SetBackgroundColor(0, 0, 0);
    SetDrawScreen(DX_SCREEN_BACK);
    SetAlwaysRunFlag(1);


    while (1)
    {
        ClearDrawScreen();

        // サイズとか色はお任せ
        CIRCLE circle = { 0, 0, 5, GetColor(0,255,255) };
        // マウス座標取得し、circleのcenterXとcenterTに格納
        GetMousePoint(&circle.centerX, &circle.centerY);
        // 描画
        DrawCircle(circle.centerX, circle.centerY, circle.size, circle.color, 1);

        // サーバ( serverAddr )に●の情報送信
        CIRCLE buff = { htonl(circle.id),htonl(circle.centerX),htonl(circle.centerY),htonl(circle.size),htonl(circle.color) };
        int ret = sendto(sock, (char*)&buff, sizeof(buff), 0, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
        // 未送信以外のエラー
        if (ret == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK)
        {
            // エラー処理
            return -1;
        }

        ScreenFlip();
        WaitTimer(16);
        if (ProcessMessage() == -1 || CheckHitKey(KEY_INPUT_ESCAPE) == 1)
        {
            break;
        }

        // サーバから受信
        CIRCLE recvPacket[3];
        ret = recv(sock, (char*)recvPacket, sizeof(recvPacket), 0);
        if (ret != SOCKET_ERROR)
        {
            for (int i = 0; i < 3; i++)
            {
                clientInfos[i].id = ntohl(recvPacket[i].id);
                clientInfos[i].centerX = ntohl(recvPacket[i].centerX);
                clientInfos[i].centerY = ntohl(recvPacket[i].centerY);
                clientInfos[i].size = ntohl(recvPacket[i].size);
                clientInfos[i].color = ntohl(recvPacket[i].color);
            }
        }
        else
        {
            if (WSAGetLastError() == WSAEWOULDBLOCK)
            {
                // 未受信
            }
            else
            {
                return -1;
            }
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