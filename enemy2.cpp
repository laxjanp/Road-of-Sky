//=============================================================================
// �G��p�R�[�h�wenemy2.cpp�x
// �����:���{�b��
//=============================================================================

//*****************************************************************************
//�C���N���[�h�K�[�h(#include)
//*****************************************************************************
#include "enemy2.h"
#include "Texture.h"
#include "polygon.h"
#include "collision.h"
#include "bullet.h"
#include "effect.h"
#include "Sound.h"

//*****************************************************************************
// �}�N����`(#define)
//*****************************************************************************
#define PATH_ENEMYTEXTURE L"data/texture/Enemy2.png"
#define ENEMY2_WIDTH 68*2
#define ENEMY2_HEIGHT 30*2
#define ENEMY2_SPEED 8.5f

#define MAX_ENEMY2 3	//�ő�̓G��

#define COLLISION_WIDTH 30
#define COLLISION_HEIGHT 20
#define COLLISION_OFFSETX 0.0f
#define COLLISION_OFFSETY -12.0f


//*****************************************************************************
// �\���̒�`(struct)
//*****************************************************************************
struct ENEMY2 // �G���
{
	XMFLOAT2 vPos;	// �\���ʒu
	XMFLOAT2 vSize;	// ���W�ʒu
	int nDir;		// ����(0:��,1:��,2:�E,3:��)
};

//*****************************************************************************
// �O���[�o���ϐ�(g_)
//*****************************************************************************
static ID3D11ShaderResourceView* g_pTexture;
static ENEMY2 g_enemy2[MAX_ENEMY2]; // �G���
static int g_nEnemy2;				// �G���ݐ�


//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy2()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	// �e�N�X�`���Ǎ�
	hr = CreateTextureFromFile(pDevice, PATH_ENEMYTEXTURE,&g_pTexture);
	if (FAILED(hr))
	{
		return hr;
	}

	// �ϐ�������
	g_nEnemy2 = 0;		//�G�̐���ۑ�

	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy2()
{
	// �e�N�X�`�����
	SAFE_RELEASE(g_pTexture);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy2()
{
	ENEMY2* pEnemy;

	for (int i = g_nEnemy2 - 1; i >= 0; --i)
	{
		pEnemy = &g_enemy2[i];

		//�ړ�����
		bool bOut = false;
		switch (pEnemy->nDir)
		{
		case 0: // ��
			pEnemy->vPos.y -= ENEMY2_SPEED;
			if (pEnemy->vPos.y < (SCREEN_HEIGHT + ENEMY2_HEIGHT) * -0.5f) 
				bOut = true;
			break;
		case 1: // ��
			pEnemy->vPos.x -= ENEMY2_SPEED;
			if (pEnemy->vPos.x < (SCREEN_WIDTH + ENEMY2_WIDTH) * -0.5f)
				bOut = true;
			break;
		case 2: // �E
			pEnemy->vPos.x += ENEMY2_SPEED;
			if (pEnemy->vPos.x > (SCREEN_WIDTH + ENEMY2_WIDTH) * 0.5f)
				bOut = true;
			break;
		case 3: // ��
			pEnemy->vPos.y += ENEMY2_SPEED;
			if (pEnemy->vPos.y > (SCREEN_HEIGHT + ENEMY2_HEIGHT) * 0.5f)
				bOut = true;
			break;
		}

		if (bOut)		//����
		{
			--g_nEnemy2;

			if (g_nEnemy2 > 0)
			{
				*pEnemy = g_enemy2[g_nEnemy2];
				
				continue;

			}
		}
	}

	// ��������
	static int nTimer = 0;
	++nTimer;
	if (nTimer > 120 && g_nEnemy2 < MAX_ENEMY2)
	{
		nTimer = 0;
		pEnemy = &g_enemy2[g_nEnemy2];

		// �����I��
		pEnemy->nDir = 1;

		// �ʒu������
		switch (pEnemy->nDir)
		{
		case 0: // ��
			pEnemy->vPos.y = (SCREEN_HEIGHT + ENEMY2_HEIGHT) * 0.5f;
			pEnemy->vPos.x = rand() % SCREEN_WIDTH - SCREEN_WIDTH * 0.5f;
			break;

		case 3: // ��
			pEnemy->vPos.y = (SCREEN_HEIGHT + ENEMY2_HEIGHT) * -0.5f;
			pEnemy->vPos.x = rand() % SCREEN_WIDTH - SCREEN_WIDTH * 0.5f;
			break;

		case 1: // ��
			pEnemy->vPos.x = (SCREEN_WIDTH + ENEMY2_WIDTH) * 0.5f;
			pEnemy->vPos.y = rand() % SCREEN_HEIGHT - SCREEN_HEIGHT * 0.5f;
			break;

		case 2: // �E
			pEnemy->vPos.x = (SCREEN_WIDTH + ENEMY2_WIDTH) * -0.5f;
			pEnemy->vPos.y = rand() % SCREEN_HEIGHT - SCREEN_HEIGHT * 0.5f;
			break;
		}

		// �ǉ��f�[�^��L����
		++g_nEnemy2;
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy2()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(ENEMY2_WIDTH, ENEMY2_HEIGHT);
	SetPolygonAngle(360);
	SetPolygonTexture(g_pTexture);
	ENEMY2* pEnemy2 = g_enemy2;

	for (int i = 0; i < g_nEnemy2; ++i, ++pEnemy2)
	{
		SetPolygonPos(pEnemy2->vPos.x, pEnemy2->vPos.y);
		DrawPolygon(pDC);
	}

	// ���ɖ߂�
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(0.0f, 0.0f);
}

//=============================================================================
// ������(�Փ�)����
//=============================================================================
int CollisionEnemy2(XMFLOAT2 vCenter, XMFLOAT2 vBox, float fDamage)
{
	XMFLOAT2 vEnemyPos;
	XMFLOAT2 vEnemyBox(COLLISION_WIDTH, COLLISION_HEIGHT);
	ENEMY2* pEnemy = g_enemy2;
	for (int i = 0; i < g_nEnemy2; ++i, ++pEnemy)
	{
		vEnemyPos.x = pEnemy->vPos.x - 30 + COLLISION_OFFSETX;
		vEnemyPos.y = pEnemy->vPos.y + COLLISION_OFFSETY;
		if (CollisionBox(vEnemyPos, vEnemyBox, vCenter, vBox))
		{
			if (fDamage > 0.0f)	//��
			{
				CSound::Play(SE_EXPLOSION);
				StartEffect(vEnemyPos);
				--g_nEnemy2;
				if (g_nEnemy2 > 0)
				{
					*pEnemy = g_enemy2[g_nEnemy2];
				}

			}
			return i;
		}
	}
	return -1;
}