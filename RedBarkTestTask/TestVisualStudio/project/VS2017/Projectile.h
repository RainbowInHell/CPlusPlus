#pragma once

#include <PlayrixEngine.h>
#include "IGameObject.h"

#define SPEED_CONSTANT 0.02f
#define PROJECTILE_WIDTH 32
#define PROJECTILE_HEIGHT 8
#define INITIAL_DAMAGE 1;

class Projectile : public IGameObject
{
public:
	Projectile();
	Projectile(FPoint _position, FPoint _direction);

	void Draw() override;
	void Update() override;
	void SetTexture(Render::Texture* _texture) override;

	void SetWidth(int w);
	void SetHeight(int h);

	void SetPosition(FPoint pos);
	void SetDirection(FPoint dir);

	FPoint& GetPosition() { return _position; }
	IRect GetBody() { return { (int)_position.x, (int)_position.y, _rect.width, _rect.height }; }
	unsigned short int GetDamage() const { return _damage; }

private:
	IRect _rect;

	Render::Texture* _texture;

	FPoint _position;

	FPoint _direction;

	unsigned short int _damage = INITIAL_DAMAGE;
	//bool _isAlive = true;
};