//#include "PlayScene.h"
//#include"numeric"
//#include"Trump.h"
//#include <algorithm>
//
///*
//呼び出すいめーじ↓
//
/////////////////////////////配る///////////////////////////
//playscene->GiveCards(); //とりあえず配る処理だけ
//playscene->GetCards(); //カード配列を取得
//数?カード?の表示
///////////////////////////////////////////////////////////
//
//
/////////////////////////////ヒットかスタンドか///////////////////////////
//for (int i = 0; i < クライアントの配列.size(); i++)
//{
//	while(true) //standが選ばれるまでおなじ人のターン　while怖いけど
//	{
//		//cilentのID?==for文のindexなら自分のターン、自分のターンな人に表示するやつ(?)//
//		自分のターンの人に選択肢提示(ボタンが押されたかとかで判断?)
//		playscene->ChooseHit(hit); //ボタンを押したかのbool変数を送る true:hit false:stand
//		int a=playscene->Choose(i); //ヒットとスタンドの処理をしてもらう 戻り値としてカードの数字が入る
//
//		if(a<0) //standの時は何も処理しない
//		{
//			次のforループへ continue?
//		}
//		else //hitの時
//		{
//			数?カード?表示し、whileの最初(選択肢提示)へ
//		}
//
//		//cilentのID?!=for文のindexなら自分以外のターン、自分以外のターンな人に表示するやつ(?)//
//		ほかのひとのターンです的な表記
//	}
//} //ここまで来たら全員がstandをえらんだということなので終了
//////////////////////////////////////////////////////////////////////////
//
//
//
/////////////////////////////点数計算と結果///////////////////////////
//	playscene->Judge();
//	playscene->GetPoint(); //ポイントの配列が返ってくる
//
//
//
//
//
//
////懸念
////描画部分を呼び出す側に任せてるんだけど、それもこのplaysceneでやるべき？今のとここのクラスでは計算とかしかしてない
////playSceneクラスでは、structを一番最初の情報送られてくる専用くらいのノリで使ってる...(ごめん)
////for文でターン管理してるけど、いいのかなあ、、、ってかIDとfor文のindexは一致してるの？よくわからん; ;
//*/
//
//PlayScene::PlayScene(GameObject* parent)
//	:GameObject(parent, "PlayScene")
//{
//	/* これグローバル
//	struct PLAYER
//{
//	int id;//プレイヤーID
//	int MyCardNum;//自分の持ってるカードの総数
//	int MyScore;//自分の持ってるスコア
//	bool isMyTurn;//自分のターンか判定用
//	bool isHit;//自分がヒットをつかえるか確認用
//	bool isStand;//自分がスタンドをつかえるか確認用
//};
//	*/
//
//
//	//初期化 計算結果に影響出ないように、0で
//	playerInfos.push_back({ 1,0,0,0,0,0 });
//	playerInfos.push_back({ 2,0,0,0,0,0 });
//	playerInfos.push_back({ 3,0,0,0,0,0 });
//	myInfo = { -100,0,0,0,0,0 };
//
//	for (int tr = 0; tr < TRUMP_NUM; tr++)
//	{
//		trump[tr] = TRUMP_MARK_NUM; //トランプを4で初期化(4枚ずつあるので)
//	}
//
//	isChosenHit = false;
//}
//
//PlayScene::~PlayScene()
//{
//}
//
//void PlayScene::Initialize()
//{
//	Instantiate<Trump>(this);
//}
//
//void PlayScene::Update()
//{
//	//ここでstructの受けとり???わかんない
//	//playerCardsを初期化 key:ID
//	for (int i = 0; i < playerInfos.size(); i++)
//	{
//		playerCards[playerInfos[i].id] = {}; //空で初期化
//	}
//}
//
//void PlayScene::Draw()
//{
//}
//
//void PlayScene::Release()
//{
//}
//
//void PlayScene::GiveCards()
//{
//	//ディーラー 微妙な記述すぎる
//	for (int i = 0; i < 2; i++) //2枚引く
//	{
//		while (true)
//		{
//			myCards.push_back(rand() % 13 + 1); //1~13の数字を生成
//			trump[myCards.back() - 1]--; //番号のカードを一枚減らす 末尾指定
//			if (trump[myCards.back() - 1] >= 0)
//			{
//				break;
//			}
//		}
//	}
//
//	//クライアント
//	for (int i = 0; i < playerInfos.size(); i++)
//	{
//		for (int j = 0; j < 2; j++) //2枚引く
//		{
//			while (true)
//			{
//				playerCards[i].push_back(rand() % 13 + 1); //id=iの人の操作 1~13の数字を生成
//				trump[playerCards[i].back() - 1]--; //番号のカードを一枚減らす
//				if (trump[playerCards[i].back() - 1] >= 0) //残りが0枚以上
//				{
//					break; //正常に選ばれていたら終了
//				}
//			}
//		}
//	}
//}
//
//int PlayScene::Choose(int clientId)
//{
//	if (isChosenHit) //ヒットだったら
//	{
//		//数引いてカードの配列に足す
//		while (true)
//		{
//			playerCards[clientId].push_back(rand() % 13 + 1); //id=iの人の操作 1~13の数字を生成
//			trump[playerCards[clientId].back() - 1]--; //番号のカードを一枚減らす
//			if (trump[playerCards[clientId].back() - 1] >= 0) //残りが0枚以上
//			{
//				break; //正常に選ばれていたら終了
//			}
//		}
//		return playerCards[clientId].back(); //今引いた数を返す
//	}
//	return -1;
//
//}
//
//void PlayScene::Judge()
//{
//	//クライアントの点数計算
//	for (int i = 0; i < playerCards.size(); i++)
//	{
//		for (int j = 0; j < playerCards[i].size(); j++)
//		{
//			//10以上の処理
//			if (playerCards[i][j] > 10)
//			{
//				playerCards[i][j] = 10; //10にする
//			}
//		}
//
//		//足す
//		playerPoints[i] = std::reduce(playerCards[i].begin(), playerCards[i].end());
//	}
//
//	//ディーラーの点数計算
//	for (int i = 0; i < myCards.size(); i++)
//	{
//		if (myCards[i] > 10)
//		{
//			myCards[i] = 10;
//		}
//	}
//	//足す
//	playerPoints[3] = std::reduce(myCards.begin(), myCards.end()); //3がディーラー
//
//	//勝者判定
//	vector<std::pair<int, int>> gapPoints(playerPoints.begin(), playerPoints.end()); //21との差をとる
//	for (int i = 0; i < 4; i++)
//	{
//		gapPoints[i].second = 21 - gapPoints[i].second; //21との差をとる
//		if (gapPoints[i].second) //-になったら差がめちゃくちゃ大きい判定(じゃないとうまくいかない)
//		{
//			gapPoints[i].second = 10000;
//		}
//	}
//
//	//value(点数で降順にソート)
//	sort(gapPoints.begin(), gapPoints.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b)
//	{
//		return a.second < b.second; //差が小さい順にソート
//	});
//
//	//21に近い人が勝ち
//	for (int i = 0; i < 4; i++)
//	{
//		//勝者複数の判定
//		if (i != 0)
//		{
//			if (gapPoints[i] == gapPoints[0])
//			{
//				isWin[i] == true;
//			}
//			else
//			{
//				isWin[i] == false;
//			}
//		}
//		else
//		{
//			isWin[i] == true;
//		}
//	}
//
//}
