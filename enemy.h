#pragma once
//=============================================================================
// �w�i�`��wenemy.h�x
// �����:���{�b��
//=============================================================================
#pragma once
#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemy(void);	//������
void UninitEnemy(void);		//�C��
void UpdateEnemy(void);		//�X�V
void DrawEnemy(void);		//�`��
int CollisionEnemy(XMFLOAT2 vCenter, XMFLOAT2 vBBox, float fDamage);//������(�Փ�)����