#include "stdafx.h"
#include "Projectile.h"

Projectile::Projectile()
{
}

Projectile::Projectile(FPoint position, FPoint direction) :
	_position(position), 
	_direction(direction)
{

}

void Projectile::Draw()
{
	_texture->Draw(FPoint{ _position.x,_position.y });
}

void Projectile::Update()
{
	_position += _direction * SPEED_CONSTANT;
}

void Projectile::SetWidth(int w)
{
	_rect.width = w;
}

void Projectile::SetHeight(int h)
{
	_rect.height = h;
}

void Projectile::SetPosition(FPoint pos)
{
	_rect.x = pos.x;
	_rect.y = pos.y;

	_position = pos;
}

void Projectile::SetDirection(FPoint dir)
{
	_direction = dir;
}

void Projectile::SetTexture(Render::Texture* _texture)
{
	this->_texture = _texture;
}
