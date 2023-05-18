//=============================================================================
// �G��p�\�[�X�R�[�h�wenemy.cpp�x
// �����:���{�b��
//=============================================================================

//*****************************************************************************
//�C���N���[�h�K�[�h(#include)
//*****************************************************************************
#include "enemy.h"
#include "Texture.h"
#include "polygon.h"
#include "collision.h"
#include "effect.h"
#include "Sound.h"

//*****************************************************************************
// �}�N����`(#define)
//*****************************************************************************
#define PATH_ENEMY_TEXTURE L"data/texture/enemy001.png"
#define ENEMY_WIDTH 64
#define ENEMY_HEIGHT 64
#define ENEMY_SPEED 4.0f

#define MAX_ENEMY 30	//�ő�̓G��

#define COLLISION_WIDTH 30
#define COLLISION_HEIGHT 20
#define COLLISION_OFFSETX 0.0f
#define COLLISION_OFFSETY -12.0f


//*****************************************************************************
// �\���̒�`(struct)
//*****************************************************************************
struct ENEMY			// �G���
{
	XMFLOAT2 vPos;		// �\���ʒu
	int nDir;			// ����(0:��,1:��,2:�E,3:��)
};

//*****************************************************************************
// �O���[�o���ϐ�(g_)
//*****************************************************************************
static ID3D11ShaderResourceView* g_pTexture;
static ENEMY g_enemy[MAX_ENEMY];	// �G���
static int g_nEnemy;				// �G���ݐ�


//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	// �e�N�X�`���Ǎ�
	hr = CreateTextureFromFile(pDevice, PATH_ENEMY_TEXTURE, &g_pTexture);
	if (FAILED(hr))
	{
		return hr;
	}

	// �ϐ�������
	g_nEnemy = 0;

	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy()
{
	// �e�N�X�`�����
	SAFE_RELEASE(g_pTexture);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy()
{
	ENEMY* pEnemy;
	for (int i = g_nEnemy - 1; i >= 0; --i)
	{
		pEnemy = &g_enemy[i];

		// �ړ�
		bool bOut = false;
		switch (pEnemy->nDir)
		{
		case 0: // ��
			pEnemy->vPos.y -= ENEMY_SPEED;
			if (pEnemy->vPos.y < (SCREEN_HEIGHT + ENEMY_HEIGHT) * -0.5f)
				bOut = true;
			break;
		case 1: // ��
			pEnemy->vPos.x -= ENEMY_SPEED;
			if (pEnemy->vPos.x < (SCREEN_WIDTH + ENEMY_WIDTH) * -0.5f)
				bOut = true;
			break;
		case 2: // �E
			pEnemy->vPos.x += ENEMY_SPEED;
			if (pEnemy->vPos.x > (SCREEN_WIDTH + ENEMY_WIDTH) * 0.5f)
				bOut = true;
			break;
		case 3: // ��
			pEnemy->vPos.y += ENEMY_SPEED;
			if (pEnemy->vPos.y > (SCREEN_HEIGHT + ENEMY_HEIGHT) * 0.5f)
				bOut = true;
			break;
		}

		// ����
		if (bOut)
		{
			--g_nEnemy;

			if (g_nEnemy > 0)
			{
				*pEnemy = g_enemy[g_nEnemy];

				continue;
			}
		}

	}

	// ��������
	static int nTimer = 0;
	++nTimer;
	if (nTimer > 20 && g_nEnemy < MAX_ENEMY)
	{
		nTimer = 0;
		pEnemy = &g_enemy[g_nEnemy];

		// �����I��
		pEnemy->nDir = 1;

		// �ʒu������
		switch (pEnemy->nDir)
		{
		case 0: // ��
			pEnemy->vPos.y = (SCREEN_HEIGHT + ENEMY_HEIGHT) * 0.5f;
			pEnemy->vPos.x = rand() % SCREEN_WIDTH - SCREEN_WIDTH * 0.5f;
			break;

		case 3: // ��
			pEnemy->vPos.y = (SCREEN_HEIGHT + ENEMY_HEIGHT) * -0.5f;
			pEnemy->vPos.x = rand() % SCREEN_WIDTH - SCREEN_WIDTH * 0.5f;
			break;

		case 1: // ��
			pEnemy->vPos.x = (SCREEN_WIDTH + ENEMY_WIDTH) * 0.5f;
			pEnemy->vPos.y = rand() % SCREEN_HEIGHT - SCREEN_HEIGHT * 0.5f;
			break;

		case 2: // �E
			pEnemy->vPos.x = (SCREEN_WIDTH + ENEMY_WIDTH) * -0.5f;
			pEnemy->vPos.y = rand() % SCREEN_HEIGHT - SCREEN_HEIGHT * 0.5f;
			break;
		}

		// �ǉ��f�[�^��L����
		++g_nEnemy;
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(ENEMY_WIDTH, ENEMY_HEIGHT);
	SetPolygonAngle(-90);
	SetPolygonTexture(g_pTexture);
	ENEMY* pEnemy = g_enemy;

	for (int i = 0; i < g_nEnemy; ++i, ++pEnemy)
	{
		SetPolygonPos(pEnemy->vPos.x, pEnemy->vPos.y);
		DrawPolygon(pDC);
	}

	// ���ɖ߂�
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(0.0f, 0.0f);
}

//=============================================================================
// ������(�Փ�)����
//=============================================================================
int CollisionEnemy(XMFLOAT2 vCenter, XMFLOAT2 vBox, float fDamage)
{
	XMFLOAT2 vEnemyPos;
	XMFLOAT2 vEnemyBox(COLLISION_WIDTH, COLLISION_HEIGHT);
	ENEMY* pEnemy = g_enemy;
	for (int i = 0; i < g_nEnemy; ++i, ++pEnemy)
	{
		vEnemyPos.x = pEnemy->vPos.x + COLLISION_OFFSETX;
		vEnemyPos.y = pEnemy->vPos.y + COLLISION_OFFSETY;
		if (CollisionBox(vEnemyPos, vEnemyBox, vCenter, vBox))
		{

			if (fDamage > 0.0f)	//��
			{
				CSound::Play(SE_EXPLOSION);
				StartEffect(vEnemyPos);
				--g_nEnemy;
				if (g_nEnemy > 0)
				{
					*pEnemy = g_enemy[g_nEnemy];
				}

			}
			return i;
		}
	}
	return -1;
}