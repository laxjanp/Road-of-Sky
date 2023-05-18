//=============================================================================
// �t�F�[�h�C���A�t�F�[�h�A�E�g�p�wfade.cpp�x
// �����:���{�b��
//=============================================================================

//*****************************************************************************
//�C���N���[�h�K�[�h(#include)
//*****************************************************************************
#include "fade.h"			//�t�F�[�h�C���A�A�E�g���K�v�̂��ߗp��
#include "polygon.h"		//�|���S���̕\�����K�v�̂��ߗp��
#include "Sound.h"			//BGM�����邽�߂ɗp��

//*****************************************************************************
//�}�N����`(#define)
//*****************************************************************************
#define FADE_RATE 0.02f;	//�t�F�[�h�C���̍�
#define FADE_RATE2 0.02f;	//�t�F�[�h�A�E�g�̍�

//*****************************************************************************
//�O���[�o���ϐ�(g_)
//*****************************************************************************
static float g_fRed = 0.0f;				//�t�F�[�h�A�E�g�̐F(��)
static float g_fGreen = 0.0f;			//�t�F�[�h�A�E�g�̐F(��)
static float g_fBlue = 0.0f;			//�t�F�[�h�A�E�g�̐F(��)
static float g_fAlpha = 1.0f;			//�t�F�[�h�A�E�g�̐F(��������)
static EFade g_eFade = FADE_IN;			//���̏��
static EScene g_eNext = SCENE_TITLE;	//���̃V�[����

// ������
HRESULT InitFade() 
{
	g_fAlpha = 1.0f;
	g_eFade = FADE_IN;
	g_eNext = SCENE_TITLE;
	return S_OK;
}

// �I������
void UninitFade() 
{
	g_fAlpha = 0.0f;
	g_eFade = FADE_NONE;
}

// �X�V
void UpdateFade() 
{
	switch (g_eFade) 
	{
	case FADE_NONE:
		break;
	
	case FADE_OUT:
		g_fAlpha += FADE_RATE2; // �s�����x�𑝂�
		if (g_fAlpha >= 1.0f) 
		{
			// �t�F�[�h�C�������ɐ؂�ւ�
			g_fAlpha = 1.0f;
			g_eFade = FADE_IN;
			
			// �V�[���ؑ�
			SetScene(g_eNext);
		}
		
		// �{�����[�����t�F�[�h�A�E�g
		CSound::SetVolume(1.0f - g_fAlpha);
		break;
	
	case FADE_IN:
		g_fAlpha -= FADE_RATE; // �����x�𑝂�
		if (g_fAlpha <= 0.0f) 
		{
			// �t�F�[�h�C���I��
			g_fAlpha = 0.0f;
			g_eFade = FADE_NONE;
		}
		
		// �{�����[�����t�F�[�h�C��
		CSound::SetVolume(1.0f - g_fAlpha);
		break;
	}
}

// �`��
void DrawFade() 
{
	// ��ʑS�̂ɔ������|���S����`��
	SetPolygonPos(0.0f, 0.0f);
	SetPolygonSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	SetPolygonTexture(nullptr);
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(0.0f, 0.0f);
	SetPolygonColor(g_fRed, g_fGreen, g_fBlue);
	SetPolygonAlpha(g_fAlpha);
	DrawPolygon(GetDeviceContext());
	
	// ���ɖ߂�
	SetPolygonColor(1.0f, 1.0f, 1.0f);
	SetPolygonAlpha(1.0f);
}

// �t�F�[�h�A�E�g�J�n
void StartFadeOut(EScene eNext)
{
	if (g_eFade != FADE_OUT) 
	{
		g_eFade = FADE_OUT;
		g_fAlpha = 0.0f;
		g_eNext = eNext;
	}
}
// ��Ԏ擾
EFade GetFade() 
{
	return g_eFade;
}

// �t�F�[�h�A�E�g�F�ݒ�
void SetFadeColor(float fR, float fG, float fB)
{
	g_fRed = fR;
	g_fGreen = fG;
	g_fBlue = fB;
}
