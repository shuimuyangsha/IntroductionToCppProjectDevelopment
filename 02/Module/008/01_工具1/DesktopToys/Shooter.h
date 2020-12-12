#pragma once

#include "IDrawable.h"
#include "IMouseAction.h"

/*
    本游戏中的 发泄工具父类,可以绘自身，及响映鼠标消息
*/

class CShooter 
    : public IDrawable
    , public IMouseAction

{
public:
    CShooter();
    virtual ~CShooter();

};

