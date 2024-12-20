#pragma once
//Trump.h
#include<vector>
#include"Engine/GameObject.h"

class Trump :public GameObject
{
public:
	Trump(GameObject* parent);
	~Trump();
	void Update() override;
	void Draw()override;
private:
	int gHandle;
	struct Cards{
		int mark;
		int number;
		bool moving;//どっちがtrueか変数名でわかるように
		float x, y;
		float startX, startY;
		float endX, endY;
		float rate;//移動の割合
	};
	std::vector<Cards> cards;//山札
	std::vector<Cards> player1;//プレイヤー１
	std::vector<Cards> player2;//プレイヤー２
	
	int counter;
	int playerID;

	bool lastN;
	bool lastM;


};