#pragma once

#include <PlayrixEngine.h>

class IGameObject
{
public:

    virtual void Draw() = 0;
    virtual void Update() = 0;
    virtual void SetTexture(Render::Texture* _texture) = 0;
};