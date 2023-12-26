#pragma once

#include <PlayrixEngine.h>
#include "IGameObject.h"

#define INITIAL_HEALTH_COUNT 2
#define GetRandom(min, max) \
    ((rand()%(int)(((max) + 1)-(min)))+ (min))

class Target : public IGameObject
{
public:
	Target();

	void Draw() override;
	void Update() override;
	void SetTexture(Render::Texture* _texture) override;

	void SetRandomPosition(int windowWidth, int windowHeight);
	void SetDirection(IPoint _direction);
	
	IPoint GetPosition();
	void SetPosition(IPoint point);

	void DecreaseHealthCount(unsigned short int _healsCountToSubstract);

	IRect& GetBody() { return _rect; };
	IPoint& GetDirection() { return _direction; };
	unsigned short int GetCurrentHealth() const { return _healthPoints; };
private:
	IRect _rect;

	Render::Texture* _texture;
	
	IPoint _direction;

	unsigned short int _damage;

	unsigned short int _healthPoints = INITIAL_HEALTH_COUNT;
};
