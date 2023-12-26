#include "stdafx.h"
#include "Player.h"

Player::Player()
{
}

void Player::Draw()
{
	_texture->Draw(IPoint{ _rect.x,_rect.y });
}

void Player::Update()
{
	if (_rect.x < 0)
		_rect.x += 8;
	if (_rect.x > WINDOW_WIDTH - 40)
		_rect.x -= 8;
}

void Player::SetTexture(Render::Texture* _texture)
{
	this->_texture = _texture;
	this->_rect.width = this->_texture->Width();
	this->_rect.height = this->_texture->Height();
}

void Player::SetPosition(IPoint point)
{
	_rect.x = point.x;
	_rect.y = point.y;
}

void Player::MoveLeft(int x)
{
	_rect.x -= x;
}

void Player::MoveRight(int x)
{
	_rect.x += x;
}
