#pragma once

#include "IDrawable.h"
#include "IMouseAction.h"

/*
    ����Ϸ�е� ��й���߸���,���Ի���������ӳ�����Ϣ
*/

class CShooter 
    : public IDrawable
    , public IMouseAction

{
public:
    CShooter();
    virtual ~CShooter();

};

