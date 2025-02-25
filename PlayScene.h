#pragma once
#include "Engine/GameObject.h"

class PlayScene :
    public GameObject
{
public:
	PlayScene(GameObject* parent);
	~PlayScene();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

