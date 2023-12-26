#include "stdafx.h"
#include "TestWidget.h"

void ResolveCollision(std::vector<Target*>& vecOfTargets)
{

	for (int i = 0; i < vecOfTargets.size() - 1; i++)
	{
		for (int j = i + 1; j < vecOfTargets.size(); j++)
		{
			// Check if the targets are intersecting
			if (vecOfTargets[i]->GetBody().Intersects(vecOfTargets[j]->GetBody()))
			{
				// Swap the movement directions of both targets
				IPoint tempDirection = vecOfTargets[i]->GetDirection();
				vecOfTargets[i]->SetDirection(vecOfTargets[j]->GetDirection());
				vecOfTargets[j]->SetDirection(tempDirection);

				// Calculate the normal vector of the collision
				FPoint collisionNormal = vecOfTargets[i]->GetPosition() - vecOfTargets[j]->GetPosition();
				collisionNormal.Normalize();

				// Displace one of the targets slightly along the normal vector
				vecOfTargets[i]->SetPosition(vecOfTargets[i]->GetPosition() + (collisionNormal * 0.1f).Rounded());
			}
		}
	}
}

float runningTime{ };
float lastUpdate{ 1.0 };

TestWidget::TestWidget(const std::string& name, rapidxml::xml_node<>* elem)
	: Widget(name)
	, _curTex(0)
	, _timer(0)
	, _angle(0)
	, _eff(NULL)
	, _scale(0.f)
	, vecOfTargets{ MAX_TARGETS_COUNT }
	, bulletsCnt{ MAX_BULLETS_COUNT }
	, secondsLeft{ MAX_SECONDS_TO_PASS_LEVEL }
{
	Init();
}

void TestWidget::Init()
{
	_backGroundTexture = Core::resourceManager.Get<Render::Texture>("Background");
	_aimTexture = Core::resourceManager.Get<Render::Texture>("Aim");
	_starTargetTexture = Core::resourceManager.Get<Render::Texture>("AlienStrong");
	_heroTexture = Core::resourceManager.Get<Render::Texture>("Player");
	_bulletTexture = Core::resourceManager.Get<Render::Texture>("Bullet");


	_curTex = 0;
	_angle = 0;

	srand((unsigned)time(NULL));

	for (int i = 0; i < vecOfTargets.size(); i++)
	{
		vecOfTargets.at(i) = new Target();
		vecOfTargets.at(i)->SetTexture(_starTargetTexture);
		vecOfTargets.at(i)->SetRandomPosition(WINDOW_WIDTH, WINDOW_HEIGHT);
		vecOfTargets.at(i)->SetDirection(IPoint{ GetRandom(3,6), GetRandom(3,6) });
	}

	player.SetTexture(_heroTexture);
	player.SetPosition({ WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - WINDOW_HEIGHT / 2 });

	projectile.SetTexture(_bulletTexture);
};

void TestWidget::Draw()
{
	_backGroundTexture->Draw();

	for (auto& trgt : vecOfTargets) { trgt->Draw(); }

	player.Draw();

	for (auto& proj : vecOfProjectiles)
	{
		projectile.SetPosition(proj->GetPosition());
		projectile.Draw();
	}

	//
	// Получаем текущее положение курсора мыши.
	//
	IPoint mouse_pos = Core::mainInput.GetMousePos();
	IPoint crossPos;

	//
	// Проталкиваем в стек текущее преобразование координат, чтобы в дальнейшем
	// можно было восстановить это преобразование вызовом PopMatrix.
	//
	Render::device.PushMatrix();

	//
	// Изменяем текущее преобразование координат, перемещая центр координат в позицию мыши
	// и поворачивая координаты относительно этого центра вокруг оси z на угол _angle.
	//
	//Render::device.MatrixTranslate((float)mouse_pos.x, (float)mouse_pos.y, 0);
	//Render::device.MatrixRotate(math::Vector3(0, 0, 1), _angle);

	if (!_curTex)
	{
		//привязка текстуры прицела и курсора мыши
		crossPos.x = mouse_pos.x - _aimTexture->Width() / 2;
		crossPos.y = mouse_pos.y - _aimTexture->Height() / 2;

		//
		// Метод Texture::Draw() выводит квадратный спрайт с размерами текстуры
		// в центре координат (0, 0). Центр координат и преобразование координат вершин
		// спрайта устанавливаются с применением текущего преобразования.
		//
		// При вызове метода Texture::Draw() вызывать Texture::Bind() необязательно.
		//
		//_tex1->Draw();
		_aimTexture->Draw(crossPos);
	}
	else
	{
		//IRect texRect = _tex2->getBitmapRect();

		//
		// При отрисовке текстуры можно вручную задавать UV координаты той части текстуры,
		// которая будет натянута на вершины спрайта. UV координаты должны быть нормализованы,
		// т.е., вне зависимости от размера текстуры в текселях, размер любой текстуры
		// равен 1 (UV координаты задаются в диапазоне 0..1, хотя ничто не мешает задать их
		// больше единицы, при этом в случае установленной адресации текстуры REPEAT, текстура
		// будет размножена по этой стороне соответствующее количество раз).
		//

		//FRect rect(texRect);
		//FRect uv(0, 1, 0, 1);

		//_tex2->TranslateUV(rect, uv);

		//Render::device.MatrixScale(_scale);
		//Render::device.MatrixTranslate(-texRect.width * 0.5f, -texRect.height * 0.5f, 0.0f);

		//
		// Привязываем текстуру.
		//
		//_tex2->Bind();

		//
		// Метод DrawQuad() выводит в графическое устройство квадратный спрайт, состоящий их двух
		// примитивов - треугольников, используя при этом текущий цвет для вершин и привязанную (binded) текстуру,
		// если разрешено текстурирование.
		//
		// Перед вызовом DrawQuad() должен быть вызов Texture::Bind() либо SetTexturing(false),
		// иначе визуальный результат будет непредсказуемым.
		//
		//Render::DrawQuad(rect, uv);
	}

	//
	// Воостанавливаем прежнее преобразование координат, снимая со стека изменённый фрейм.
	//
	Render::device.PopMatrix();

	//
	// Получаем текущие координаты объекта, двигающегося по сплайну
	//
	//FPoint currentPosition = spline.getGlobalFrame(math::clamp(0.0f, 1.0f, _timer / 6.0f));

	//
	// И рисуем объект в этих координатах
	//
	//Render::device.PushMatrix();
	//Render::device.MatrixTranslate(currentPosition.x, currentPosition.y, 0);
	//_tex3->Draw();
	//Render::device.PopMatrix();

	//
	// Этот вызов отключает текстурирование при отрисовке.
	//
	Render::device.SetTexturing(false);

	//
	// Метод BeginColor() проталкивает в стек текущий цвет вершин и устанавливает новый.
	//
	Render::BeginColor(Color(255, 128, 0, 255));

	//
	// Метод DrawRect() выводит в графическое устройство квадратный спрайт, состоящий их двух
	// примитивов - треугольников, используя при этом текущий цвет для вершин и привязанную (binded) текстуру,
	// если разрешено текстурирование.
	//
	// Перед вызовом DrawRect() должен быть вызов Texture::Bind(), либо SetTexturing(false),
	// иначе визуальный результат будет непредсказуемым.
	//
	//Render::DrawRect(924, 0, 100, 100);

	//
	// Метод EndColor() снимает со стека текущий цвет вершин, восстанавливая прежний.
	//
	Render::EndColor();

	//
	// Опять включаем текстурирование.
	//
	Render::device.SetTexturing(true);

	//
	// Рисуем все эффекты, которые добавили в контейнер (Update() для контейнера вызывать не нужно).
	//
	_effCont.Draw();

	Render::BindFont("arial");
	Render::PrintString(WINDOW_WIDTH / 2, WINDOW_HEIGHT, 35, utils::lexical_cast(secondsLeft), 1.f, CenterAlign);
	Render::PrintString(WINDOW_WIDTH / 2 - 504, WINDOW_HEIGHT, 35, utils::lexical_cast(bulletsCnt), CenterAlign);
	//Render::PrintString(924 + 100 / 2, 35, utils::lexical_cast(mouse_pos.x) + ", " + utils::lexical_cast(mouse_pos.y), 1.f, CenterAlign);
}

void TestWidget::Update(float dt)
{
	//
	// Обновим контейнер с эффектами
	//
	_effCont.Update(dt);

	//логика таймера
	runningTime += dt;
	if (runningTime >= lastUpdate)
	{
		if (secondsLeft > 0) secondsLeft--;

		runningTime = 0.0;
	}

	//
	// dt - значение времени в секундах, прошедшее от предыдущего кадра.
	// Оно может принимать разные значения, в зависимости от производительности системы
	// и сложности сцены.
	//
	// Для того, чтобы наша анимация зависела только от времени, и не зависела от
	// производительности системы, мы должны рассчитывать её от этого значения.
	//
	// Увеличиваем наш таймер с удвоенной скоростью.
	//
	_timer += dt * 0.1;

	//
	// Зацикливаем таймер в диапазоне (0, 2п).
	// Это нужно делать для предотвращения получения некорректных значений,
	// если вдруг переполнится разрядная сетка (float переполнился) или задержка
	// от предыдущего кадра была слишкой большой (система тормози-и-ит).
	//
	// Диапазон значений выбран равным (0, 2п), потому что мы используем это значение
	// для расчёта синуса, и большие значения будут просто периодически повторять результат.
	//
	while (_timer > 2 * math::PI)
	{
		_timer -= 2 * math::PI;
	}

	//
	// Анимирование параметра масштабирования в зависимости от таймера.
	//
	_scale = 0.8f + 0.25f * sinf(_timer);

	for (int i = 0; i < vecOfTargets.size(); i++)
	{
		vecOfTargets.at(i)->Update();
	};

	player.Update();

	for (std::vector<Projectile*>::iterator projIt = vecOfProjectiles.begin(); projIt != vecOfProjectiles.end();)
	{
		(*projIt)->Update();

		std::vector<Target*>::iterator toRemoveElementIt = std::find_if(vecOfTargets.begin(), vecOfTargets.end(), [&](auto& trgt) {return trgt->GetBody().Intersects((*projIt)->GetBody()); });

		if (toRemoveElementIt != vecOfTargets.end())
		{
			(*toRemoveElementIt)->DecreaseHealthCount((*projIt)->GetDamage());

			delete* projIt;
			*projIt = NULL;
			projIt = vecOfProjectiles.erase(projIt);

			if ((*toRemoveElementIt)->GetCurrentHealth() == 0)
			{
				_eff = _effCont.AddEffect("TargetDestroyed");
				_eff->posX = (*toRemoveElementIt)->GetBody().x + 0.f;
				_eff->posY = (*toRemoveElementIt)->GetBody().y + 0.f;
				_eff->Reset();
				_eff->Finish();
				_eff = NULL;

				delete* toRemoveElementIt;
				*toRemoveElementIt = NULL;
				vecOfTargets.erase(toRemoveElementIt);
			}
		}
		else
		{
			++projIt;
		}
	}

	ResolveCollision(vecOfTargets);
}

bool TestWidget::MouseDown(const IPoint& mouse_pos)
{
	if (Core::mainInput.GetMouseLeftButton())
	{
		if (bulletsCnt != 0)
		{
			vecOfProjectiles.emplace_back(new Projectile());

			int i = vecOfProjectiles.size() - 1;
			vecOfProjectiles.at(i)->SetPosition(player.GetPosition());

			FPoint direction = Core::mainInput.GetMousePos() - vecOfProjectiles.at(i)->GetPosition().Rounded();
			direction.Normalized();

			vecOfProjectiles.at(i)->SetDirection(direction);

			bulletsCnt--;
		}
	}

	return false;
}

void TestWidget::MouseMove(const IPoint& mouse_pos)
{
	if (_eff)
	{
		//
		// Если эффект создан, то перемещаем его в позицию мыши.
		//
		_eff->posX = mouse_pos.x + 0.f;
		_eff->posY = mouse_pos.y + 0.f;
	}
}

void TestWidget::MouseUp(const IPoint& mouse_pos)
{
	if (_eff)
	{
		//
		// Если эффект создан, то при отпускании мыши завершаем его.
		//
		_eff->Finish();
		_eff = NULL;
	}
}

void TestWidget::AcceptMessage(const Message& message)
{
	//
	// Виджету могут посылаться сообщения с параметрами.
	//

	const std::string& publisher = message.getPublisher();
	const std::string& data = message.getData();
}

void TestWidget::KeyPressed(int keyCode)
{
	//
	// keyCode - виртуальный код клавиши.
	// В качестве значений для проверки нужно использовать константы VK_.
	//

	if (keyCode == VK_A)
	{
		player.MoveLeft(10);
		// Реакция на нажатие кнопки A
	}
	if (keyCode == VK_D)
	{
		player.MoveRight(10);
	}
}

void TestWidget::CharPressed(int unicodeChar)
{
	//
	// unicodeChar - Unicode код введённого символа
	//

	if (unicodeChar == L'а') {
		// Реакция на ввод символа 'а'
	}
}