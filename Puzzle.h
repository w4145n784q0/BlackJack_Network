#pragma once
#include"Engine/GameObject.h"
#include<vector>

//puzzle.h

/// <summary>
/// パズドラのプレイ画面　w.n(名前)
/// <Author>←本来いれるもの
/// </summary>
class Puzzle :public GameObject
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="親のポインタ"></param>
	Puzzle(GameObject* parent);
	~Puzzle();
	void Update() override;
	void Draw()override;
private:
	struct Piece {
		int color;
//		bool selected;
		int erase;//消えるピースが0:消えない　>0:消える
		float fallAdjust; //落下の補正
	
	};//	Piece field[5][6];   c言語
	std::vector<std::vector<Piece>> field;//c++
	int selX, selY;//選んでいる場所
	int mouseX, mouseY;//マウス座標
	bool lastButton;//前のフレームのボタン状態
	float eraseTimer;//消える演出の時間（秒）を管理
	int eraseAlpha;//消すときのαの値
	//状態遷移
	enum State {
		S_IDLE,
		S_MOVE,
		S_ERASE,
		S_FALL,
		S_ATTACK,
	};
	State state; //状態変数
	void UpdateIdle();
	void UpdateMove();
	void UpdateErase();
	void UpdateFall();
	void UpdateAttack();

	/// <summary>
	/// 
	/// </summary>
	/// <returns>消えるものがあればtrue</returns>
	bool CanErase();
	void PrepareFall();
	int soundErase;
	int soundFall;
	bool fallSoundRequested;
};
