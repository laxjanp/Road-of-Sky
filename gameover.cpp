//=============================================================================
// �Q�[���I�[�o�[�̕`��p�wgameover.cpp�x
// �����:���{�b��
//=============================================================================

//*****************************************************************************
//�C���N���[�h�K�[�h(#include)
//*****************************************************************************
#include "gameover.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"
#include "scene.h"
#include "Sound.h"
#include "fade.h"

//*****************************************************************************
// �}�N����`(#define)
//*****************************************************************************
#define POS_X_BG	0.0f
#define POS_Y_BG	0.0f
#define WIDTH_BG	SCREEN_WIDTH
#define HEIGHT_BG	SCREEN_HEIGHT

#define TEX_BG		0
#define MAX_TEXTURE			1

#define TIMER_GAMEOVER		10*60

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPCWSTR g_pszTexture[] = {{L"data/texture/gameover001.png"}};
static ID3D11ShaderResourceView* g_pTexture[MAX_TEXTURE];

// ����������
HRESULT CGameover::Init()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	// �e�N�X�`���Ǎ�
	for (int i = 0; i < MAX_TEXTURE; ++i)
	{
		hr = CreateTextureFromFile(pDevice, g_pszTexture[i], &g_pTexture[i]);
		if (FAILED(hr))
		{
			return hr;
		}
	}

	// �ϐ�������
	m_Timer = TIMER_GAMEOVER;

	return hr;
}

// �I������
void CGameover::Uninit()
{
	// �e�N�X�`�����
	for (int i = 0; i < MAX_TEXTURE; ++i)
	{
		SAFE_RELEASE(g_pTexture[i]);
	}
}

// �X�V����
void CGameover::Update()
{
	// �I��
	if (GetKeyRelease(VK_RETURN) || GetKeyRelease(VK_SPACE))
	{
		StartFadeOut(SCENE_TITLE);
		return;
	}

	// �^�C�}�[�X�V
	--m_Timer;
	if (m_Timer <= 0)
	{
		StartFadeOut(SCENE_TITLE);
	}
}


// �`�揈��
void CGameover::Draw()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(WIDTH_BG, HEIGHT_BG);
	SetPolygonPos(POS_X_BG, POS_Y_BG);
	SetPolygonTexture(g_pTexture[TEX_BG]);
	DrawPolygon(pDC);
}

//������
//HRESULT InitGameOver()
//{
//	HRESULT hr = S_OK;
//	ID3D11Device* pDevice = GetDevice();
//
//	// �e�N�X�`���Ǎ�
//	for (int i = 0; i < MAX_TEXTURE; ++i)
//	{
//		hr = CreateTextureFromFile(pDevice, g_pszTexture[i], &g_pTexture[i]);
//		if (FAILED(hr))
//		{
//			return hr;
//		}
//	}
//
//	// �ϐ�������
//	g_nTimer = TIMER_GAMEOVER;
//
//	return hr;
//}


//�I��
//void UninitGameOver()
//{
//	// �e�N�X�`�����
//	for (int i = 0; i < MAX_TEXTURE; ++i)
//	{
//		SAFE_RELEASE(g_pTexture[i]);
//	}
//
//}

//�X�V
//void UpdateGameOver()
//{
//	// �I��
//	if (GetKeyRelease(VK_RETURN) || GetKeyRelease(VK_SPACE))
//	{
//		StartFadeOut(SCENE_TITLE);
//		return;
//	}
//
//	// �^�C�}�[�X�V
//	--g_nTimer;
//	if (g_nTimer <= 0)
//	{
//		StartFadeOut(SCENE_TITLE);
//	}
//}


//�`��
//void DrawGameOver()
//{
//	ID3D11DeviceContext* pDC = GetDeviceContext();
//	SetPolygonSize(WIDTH_BG, HEIGHT_BG);
//	SetPolygonPos(POS_X_BG, POS_Y_BG);
//	SetPolygonTexture(g_pTexture[TEX_BG]);
//	DrawPolygon(pDC);
//}


