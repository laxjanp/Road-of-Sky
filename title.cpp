//=============================================================================
// �^�C�g����ʗp�wtltie.cpp�x
// �����:���{�b��
//=============================================================================

//*****************************************************************************
//�C���N���[�h�K�[�h(#include)
//*****************************************************************************
#include "title.h"
#include "enemy2.h"
#include "polygon.h"
#include "Texture.h"
#include "input.h"
#include "scene.h"
#include "Sound.h"
#include "fade.h"

//*****************************************************************************
// �}�N����`(#define)
//*****************************************************************************
// �w�i
#define POS_X_BG	0.0f
#define POS_Y_BG	0.0f
#define WIDTH_BG	SCREEN_WIDTH
#define HEIGHT_BG	SCREEN_HEIGHT

// �^�C�g�����S
#define WIDTH_LOGO	1280
#define HEIGHT_LOGO	720
#define POS_X_LOGO 0.0f
#define POS_Y_LOGO 200.0f

// �G���^�[�L�[���S
#define WIDTH_ENTER 600
#define HEIGHT_ENTER 100
#define POS_X_ENTER 0.0f
#define POS_Y_ENTER (SCREEN_HEIGHT - HEIGHT_ENTER) *- 0.35f

// ����҃��S
#define WIDTH_PRODUCTION 1280/2
#define HEIGHT_PRODUCTION 720/2
#define POS_X_PRODUCTION 500.0f
#define POS_Y_PRODUCTION -320.0f

#define TEX_BG 0
#define TEX_LOGO 1
#define TEX_ENTER 2
#define TEX_PRODUCTION 3
#define MAX_TEXTURE 4

#define BLINK_TIMER 30
#define BLINK_START_TIMER 5

//*****************************************************************************
//�\���̒�`(struct)
//*****************************************************************************
struct ANIM_PAT
{
	int nFrame;
	int nCount;
};

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPCWSTR g_pszTexture[] = { L"data/texture/sky_003.jpg",
								 L"data/texture/title_rogo001.png",
								 L"data/texture/press_enter.png" ,
								 L"data/texture/production.png" };
static ID3D11ShaderResourceView* g_pTexture[MAX_TEXTURE];
static int g_nAnimFrame;
static int g_nFrameCount;
static int g_nBlink;
static int g_nStart;


//������
HRESULT InitTitle()
{
	InitEnemy2();
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

	//�ϐ��̏�����
	g_nBlink = BLINK_TIMER;
	g_nStart = 0;
	
	// BGM�Đ�
	CSound::Play(BGM_000);

	return hr;
}


//�I��
void UninitTitle()
{
	// BGM�Đ���~
	CSound::Stop(BGM_000);

	// �e�N�X�`�����
	for (int i = 0; i < MAX_TEXTURE; ++i)
	{
		SAFE_RELEASE(g_pTexture[i]);
	}
	UninitEnemy2();
}


//�X�V
void UpdateTitle()
{
	UpdateEnemy2();

	// �_�Ő���
	--g_nBlink;
	if (g_nBlink <= 0)
	{
		g_nBlink = (g_nStart) ? BLINK_START_TIMER :
			BLINK_TIMER;
	}


	//�Q�[���X�^�[�g
	if (GetKeyRelease(VK_RETURN) || GetKeyRelease(VK_SPACE))
	{
		CSound::Play(SE_DECIDE);
		StartFadeOut(SCENE_GAME);
		g_nStart = 1; // �J�n�t���O
		g_nBlink = BLINK_START_TIMER;
		return;
	}

}


//�`��
void DrawTitle()
{
	//�w�i�̕`��
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(WIDTH_BG, HEIGHT_BG);
	SetPolygonPos(POS_X_BG, POS_Y_BG);
	SetPolygonTexture(g_pTexture[TEX_BG]);
	DrawPolygon(pDC);
	
	//���o
	DrawEnemy2();
	
	//�^�C�g�����S�̕`��
	SetPolygonSize(WIDTH_LOGO, HEIGHT_LOGO);
	SetPolygonPos(POS_X_LOGO, POS_Y_LOGO);
	SetPolygonTexture(g_pTexture[TEX_LOGO]);
	DrawPolygon(pDC);

	//����҂̕`��
	SetPolygonSize(WIDTH_PRODUCTION, HEIGHT_PRODUCTION);
	SetPolygonPos(POS_X_PRODUCTION, POS_Y_PRODUCTION);
	SetPolygonTexture(g_pTexture[TEX_PRODUCTION]);
	DrawPolygon(pDC);
	
	if (g_nStart)
	{
		if (g_nBlink < BLINK_START_TIMER / 2)
		{
			return;
		}
	}
	else
	{
		if (g_nBlink < BLINK_TIMER / 2)
		{
			return;
		}
	}

	//Enter�L�[�̕`��
	SetPolygonSize(WIDTH_ENTER, HEIGHT_ENTER);
	SetPolygonPos(POS_X_ENTER, POS_Y_ENTER);
	SetPolygonTexture(g_pTexture[TEX_ENTER]);
	DrawPolygon(pDC);

	
}
