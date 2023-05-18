
//=============================================================================
// ������(�Փ�)����̃v���O�����wcollision.cpp�x
// �����:���{�b��
//=============================================================================

//*****************************************************************************
//�C���N���[�h�K�[�h(#include)
//*****************************************************************************
#include "collision.h"


//�~���m�̓����蔻����쐬
bool CollisionSphere(XMFLOAT2 vA, float fAr, XMFLOAT2 vB, float fBr)
{
	float dx = vA.x - vB.x;
	float dy = vA.y - vB.y;
	float dr = fAr + fBr;
	return dx * dx + dy * dy <= dr * dr;

}

// ��`���m�̓����蔻����쐬
bool CollisionBox(XMFLOAT2 vA, XMFLOAT2 vAs, XMFLOAT2 vB, XMFLOAT2 vBs)
{
	return vA.x - vAs.x <= vB.x + vBs.x &&
		vB.x - vBs.x <= vA.x + vAs.x &&
		vA.y - vAs.y <= vB.y + vBs.y &&
		vB.y - vBs.y <= vA.y + vAs.y;

}