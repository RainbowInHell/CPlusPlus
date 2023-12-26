#pragma once

#include "../projects/VS2017/Target.h"
#include "../projects/VS2017/Player.h"
#include "../projects/VS2017/Projectile.h"

#define MAX_TARGETS_COUNT 10
#define MAX_BULLETS_COUNT 30
#define MAX_SECONDS_TO_PASS_LEVEL 15
///
/// Виджет - основной визуальный элемент на экране.
/// Он отрисовывает себя, а также может содержать другие виджеты.
///
class TestWidget : public GUI::Widget
{
public:
	TestWidget(const std::string& name, rapidxml::xml_node<>* elem);

	void Draw() override;
	void Update(float dt) override;
	
	void AcceptMessage(const Message& message) override;
	
	bool MouseDown(const IPoint& mouse_pos) override;
	void MouseMove(const IPoint& mouse_pos) override;
	void MouseUp(const IPoint& mouse_pos) override;

	void KeyPressed(int keyCode) override;
	void CharPressed(int unicodeChar) override;

private:
	void Init();

private:
	int bulletsCnt;
	int secondsLeft;

	float _timer;
	
	float _scale;
	float _angle;
	
	Render::Texture* _backGroundTexture;
	Render::Texture* _aimTexture;
	Render::Texture* _starTargetTexture;
	Render::Texture* _heroTexture;
	Render::Texture* _bulletTexture;
	int _curTex;
	
	EffectsContainer _effCont;
	ParticleEffectPtr _eff;

	TimedSpline<FPoint> spline;

	std::vector<Target*> vecOfTargets;
	Player player;

	std::vector<Projectile*> vecOfProjectiles;
	Projectile projectile;

	int shootTimer = 0;
};
