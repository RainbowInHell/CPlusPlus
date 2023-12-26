#include "stdafx.h"
#include "Target.h"

Target::Target()
{
}

//Target::Target(Render::Texture* _texture, IRect _rect, IPoint _direction, unsigned short int _healsCount)
//{
//	this->_texture = _texture;
//	this->_rect = _rect;
//	this->_direction = _direction;
//	this->_healthPoint = _healthPoint;
//}

void Target::Draw()
{
	if (_healthPoints > 0)
	{
		_texture->Draw(IPoint{ _rect.x,_rect.y });
	}
}

void Target::Update()
{
	_rect.x += _direction.x;
	_rect.y += _direction.y;

	if (_rect.x < 0 || _rect.x > WINDOW_WIDTH - 2 * (_texture->Width() + _texture->Height()) / 3.5)
	{
		_direction.x = -_direction.x;
		_rect.x = _rect.x < 0 ? 1 : (WINDOW_WIDTH - 2 * (_texture->Width() + _texture->Height()) / 3.5);
	}

	if (_rect.y < 0 || _rect.y > WINDOW_HEIGHT - 2 * (_texture->Width() + _texture->Height()) / 3.5)
	{
		_direction.y = -_direction.y;
		_rect.y = _rect.y < 0 ? 1 : (WINDOW_HEIGHT - 2 * (_texture->Width() + _texture->Height()) / 3.5);
	}
}

void Target::SetTexture(Render::Texture* _texture)
{
	if (_texture != NULL)
	{
		this->_texture = _texture;
		this->_rect.width = this->_texture->Width();
		this->_rect.height = this->_texture->Height();
	}
}

void Target::SetRandomPosition(int windowWidth, int windowHeight)
{
	_rect.x = GetRandom(_rect.width, windowWidth - _rect.width);
	_rect.y = GetRandom(0, windowHeight - _rect.height);
}

void Target::DecreaseHealthCount(unsigned short int _healthPointsToSubstract)
{
	_healthPoints -= _healthPointsToSubstract;
}

void Target::SetDirection(IPoint _direction)
{
	this->_direction = _direction;
}

IPoint Target::GetPosition()
{
	return { _rect.x, _rect.y };
}

void Target::SetPosition(IPoint point)
{
	_rect.x = point.x;
	_rect.y = point.y;
}
