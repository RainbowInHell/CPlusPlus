#pragma once

#include <PlayrixEngine.h>
#include "IGameObject.h"

#define GetRandom(min, max) \
    ((rand()%(int)(((max) + 1)-(min)))+ (min))

class Player : public IGameObject
{
public:
	Player();

	void Draw() override;
	void Update() override;
	void SetTexture(Render::Texture* _texture) override;

	void SetPosition(IPoint point);
	void MoveLeft(int x);
	void MoveRight(int x);

	IPoint GetPosition() { return { _rect.x, _rect.y }; }
	IRect& GetBody() { return _rect; }
private:
	IRect _rect;

	Render::Texture* _texture;

	int _healthPoints;
};