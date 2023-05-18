//=============================================================================
// �v���C���[�\���p�\�[�X�R�[�h�wplayer.cpp�x
// �����:���{�b��
//=============================================================================

//*****************************************************************************
//�C���N���[�h�K�[�h(#include)
//*****************************************************************************
#include "player.h"
#include "polygon.h"
#include "Texture.h"
#include "bullet.h"
#include "input.h"
#include "enemy.h"
#include "enemy2.h"
#include "Sound.h"
#include "life.h"
#include "timer.h"


//*****************************************************************************
// �}�N����`(#define)
//*****************************************************************************
#define PATH_TEXTURE_PLAYER L"data/texture/player001.png"
#define POS_X_PLAYER		-520.0f
#define POS_Y_PLAYER		0.0f
#define PLAYER_WIDTH		64	
#define PLAYER_HEIGHT		64	
#define PLAYER_COUNT_X		3	
#define PLAYER_COUNT_Y		4	
#define SPEED_PLAYER		5.0f	//�ړ����x
#define COLLISION_WIDTH		20.0f
#define COLLISION_HEIGHT	31.0f
#define DAMAGE_TIMER		180


//*****************************************************************************
// �\���̒�`(struct)
//*****************************************************************************
struct PLAYER				//�v���C���[�̏��
{
	XMFLOAT2 g_vPos;		// �ʒu
	int g_nWalk;			// 0:�ҋ@, 1:���s
	int g_nDir;				// 0:��,1:��,2:�E,3:��
};

//*****************************************************************************
// �O���[�o���ϐ�(g_)
//*****************************************************************************
static ID3D11ShaderResourceView *g_pTexture;
static XMFLOAT2	g_vPos;				// �ʒu
static int g_nWalk;					// 0:�ҋ@, 1:���s
static int g_nDir;					// 0:��,1:��,2:�E,3:��

static int g_nDamage;				// �_�Œ�
static bool g_bGameOver;			// �Q�[���I�[�o�[
static bool g_bGameClear;			// �Q�[���N���A


//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	// �e�N�X�`���Ǎ�
	hr = CreateTextureFromFile(pDevice, PATH_TEXTURE_PLAYER, &g_pTexture);
	if (FAILED(hr))
	{
		return hr;
	}

	// �ϐ�������
	g_vPos.x = POS_X_PLAYER;
	g_vPos.y = POS_Y_PLAYER;
	g_nWalk = 0;
	g_nDir = 2;
	g_nDamage = 3;
	g_bGameOver = false;

	return hr;
}

// �I������
void UninitPlayer()
{
	// �e�N�X�`�����
	SAFE_RELEASE(g_pTexture);
}

// �X�V
void UpdatePlayer()
{
	do {
		// �_���[�W������
		if (g_nDamage > 0)
		{
			--g_nDamage;

			if (g_nDamage <= 0)
			{
				if (GetLife() <= 0 || GetTimer() <= 0)
				{
					g_bGameOver = true;
					return;
				}
			}
			break;
		}

		if (GetTimer() <= 0)
		{
			g_nDamage = DAMAGE_TIMER;
			break;
		}

		// �ړ��̏�����
		g_nWalk = 0;

		//W�L�[����L�[�����������Ɉړ�����
		if (GetKeyPress(VK_W) || GetKeyPress(VK_UP))
		{
			g_nWalk = 1;
			g_nDir = 3;
			g_vPos.y += SPEED_PLAYER;
			if (g_vPos.y < (SCREEN_WIDTH - PLAYER_WIDTH) * -0.5f)
			{
				g_vPos.y = (SCREEN_WIDTH - PLAYER_WIDTH) * -0.5f;

			}
		}

		//S�L�[�����L�[���������牺�Ɉړ�����
		if (GetKeyPress(VK_S) || GetKeyPress(VK_DOWN))
		{
			g_nWalk = 1;
			g_nDir = 0;
			g_vPos.y -= SPEED_PLAYER;
			if (g_vPos.y < (SCREEN_WIDTH - PLAYER_WIDTH) * -0.5f)
			{
				g_vPos.y = (SCREEN_WIDTH - PLAYER_WIDTH) * -0.5f;

			}

		}

		//A�L�[�����L�[���������獶�Ɉړ�����
		if (GetKeyPress(VK_A) || GetKeyPress(VK_LEFT))
		{
			g_nWalk = 1;
			g_nDir = 1;
			g_vPos.x -= SPEED_PLAYER;
			if (g_vPos.x < (SCREEN_WIDTH - PLAYER_WIDTH) * -0.5f)
			{
				g_vPos.x = (SCREEN_WIDTH - PLAYER_WIDTH) * -0.5f;

			}

		}

		//D�L�[���E�L�[����������E�Ɉړ�����
		if (GetKeyPress(VK_D) || GetKeyPress(VK_RIGHT))
		{
			g_nWalk = 1;
			g_nDir = 2;
			g_vPos.x += SPEED_PLAYER;
			if (g_vPos.x > (SCREEN_WIDTH - PLAYER_WIDTH) * 0.5f)
			{
				g_vPos.x = (SCREEN_WIDTH - PLAYER_WIDTH) * 0.5f;

			}

		}

		// Z�L�[����������e����
		if (GetKeyTrigger(VK_Z))
		{
			FireBullet(g_vPos, g_nDir);
		}

		// �G�Ƃ̓����蔻��̏���
		XMFLOAT2 vBox(COLLISION_WIDTH, COLLISION_HEIGHT);

		//�G1or�G2�̂ǂ��炩�ɐG�ꂽ�烉�C�t�����炷
		if (CollisionEnemy(g_vPos, vBox, 0.0f) >= 0 || CollisionEnemy2(g_vPos, vBox, 0.0f) >= 0)
		{
			DelLife();
			g_nDamage = DAMAGE_TIMER;
			CSound::Play(SE_HIT);
		}

	} while (0);

}

// �`��
void DrawPlayer()
{
	if (g_bGameOver)
	{
		return;
	}

	if (g_nDamage > 0)
	{
		if (g_nDamage & 4)
			return;
	}

	ID3D11DeviceContext* pDC = GetDeviceContext();

	SetPolygonSize(PLAYER_WIDTH, PLAYER_HEIGHT);
	SetPolygonPos(g_vPos.x, g_vPos.y);
	SetPolygonAngle(90);
	SetPolygonTexture(g_pTexture);
	DrawPolygon(pDC);
	SetPolygonUV(0.0f, 0.0f);
	SetPolygonFrameSize(1.0f, 1.0f);
}

// �Q�[���I�[�o�[
bool IsGameOver()
{
	return g_bGameOver;
}
