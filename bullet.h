//=============================================================================
// �w�i�`��wbullet.h�x
// �����:���{�b��
//=============================================================================

#pragma once

#include "main.h"

//�֐��̃v���g�^�C�v
HRESULT InitBullet();			// ������
void UninitBullet();			// �I��
void UpdateBullet();			// �X�V
void DrawBullet();				// �`��
int FireBullet(XMFLOAT2, int);	// ����