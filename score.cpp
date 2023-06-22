//=============================================================================
// �X�R�A�\����p�R�[�h�wscore.cpp�x
// �����:���{�b��
//=============================================================================

//*****************************************************************************
//�C���N���[�h�K�[�h(#include)
//*****************************************************************************
#include "score.h"
#include "number.h"
#include "Texture.h"
#include "polygon.h"

//*****************************************************************************
// �}�N����`(#define)
//*****************************************************************************
#define PATH_TEXTURE_FRAMESCORE L"data/texture/frame_score.png"
#define WIDTH_FRAMESCORE 340.0f
#define HEIGHT_FRAMESCORE 80.0f
#define POS_X_FRAMESCORE (SCREEN_WIDTH - WIDTH_FRAMESCORE) * 0.5f
#define POS_Y_FRAMESCORE (SCREEN_HEIGHT - HEIGHT_FRAMESCORE) * 0.5f

#define CHRCNT_SCORE 8
#define POS_X_SCORE (POS_X_FRAMESCORE - NUMBER_SIZE_X * CHRCNT_SCORE * 0.5f)
#define POS_Y_SCORE (POS_Y_FRAMESCORE + 16.0f)

//*****************************************************************************
// �O���[�o���ϐ�(g_)
//*****************************************************************************
static ID3D11ShaderResourceView* g_pTexture;
static int g_nScore;

CNumber ScoreNumber;		// �i���o�[�N���X�̕ϐ�

// ������
HRESULT InitScore(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	// �e�N�X�`���Ǎ�
	hr = CreateTextureFromFile(pDevice, PATH_TEXTURE_FRAMESCORE, &g_pTexture);
	if (FAILED(hr))
	{
		return hr;
	}

	//�ϐ��̏�����
	g_nScore = 0;

	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void UninitScore()
{
	// �e�N�X�`�����
	SAFE_RELEASE(g_pTexture);
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawScore()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	// ���邢�΂ɐݒ�
	SetPolygonColor(0.0f, 1.0f, 0.0f);
	
	// �X�R�A�g�\��
	SetPolygonSize(WIDTH_FRAMESCORE, HEIGHT_FRAMESCORE);
	SetPolygonPos(POS_X_FRAMESCORE, POS_Y_FRAMESCORE);

	SetPolygonTexture(g_pTexture);
	DrawPolygon(pDC);

	// �X�R�A�̕\��
	ScoreNumber.Draw(XMFLOAT2(POS_X_SCORE, POS_Y_SCORE), (unsigned)g_nScore, CHRCNT_SCORE);
	//DrawNumber(XMFLOAT2(POS_X_SCORE, POS_Y_SCORE),(unsigned)g_nScore, CHRCNT_SCORE);
	
	// �F�����ɖ߂�
	SetPolygonColor(1.0f, 1.0f, 1.0f);

}

//=============================================================================
//AddScore�֐�
//���e:�X�R�A�\�����Z����
//����:�X�R�A�����Z������nScore
//�߂�l:�Ȃ�
//=============================================================================
void AddScore(int nScore)
{
	g_nScore += nScore;
}

//=============================================================================
//GetScore�֐�
//���e:�X�R�A�\���擾����
//����:�Ȃ�
//�߂�l:�X�R�A���擾���邽�߂�g_nScore
//=============================================================================
int GetScore() 
{
	return g_nScore;
}