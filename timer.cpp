//=============================================================================
// �^�C�}�[�̕\���R�[�h�wTimer.cpp�x
// �����:���{�b��
//=============================================================================

//*****************************************************************************
//�C���N���[�h�K�[�h(#include)
//*****************************************************************************
#include "timer.h"
#include "number.h"
#include "Texture.h"
#include "polygon.h"

//*****************************************************************************
// �}�N����`(#define)
//*****************************************************************************
#define PATH_TEXTURE_FRAMETIMER L"data/texture/frame_timer.png"
#define WIDTH_FRAMETIMER 140.0f
#define HEIGHT_FRAMETIMER 80.0f
#define POS_X_FRAMETIMER (SCREEN_WIDTH - WIDTH_FRAMETIMER) * 0.5f
#define POS_Y_FRAMETIMER (SCREEN_HEIGHT - HEIGHT_FRAMETIMER) * 0.5f

#define CHRCNT_TIMER 3
#define POS_X_TIMER (POS_X_FRAMETIMER - NUMBER_SIZE_X * CHRCNT_TIMER * 0.5f)
#define POS_Y_TIMER (POS_Y_FRAMETIMER + 16.0f)
#define START_TIMER 60

//*****************************************************************************
// �O���[�o���ϐ�(g_)
//*****************************************************************************
static ID3D11ShaderResourceView* g_pTexture;
static int g_nTimer;
CNumber TimeNumber;			// �i���o�[�N���X


// ������
HRESULT InitTimer(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	// �e�N�X�`���Ǎ�
	hr = CreateTextureFromFile(pDevice, PATH_TEXTURE_FRAMETIMER, &g_pTexture);
	if (FAILED(hr))
	{
		return hr;
	}

	//�ϐ��̏�����
	g_nTimer = START_TIMER * 60 + 59;

	return hr;
}


//=============================================================================
// �I������
//=============================================================================
void UninitTimer()
{
	// �e�N�X�`�����
	SAFE_RELEASE(g_pTexture);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTimer()
{
	if (g_nTimer > 0)
	{
		--g_nTimer;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTimer()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	//�^�C�}�[�g�̕\��
	SetPolygonSize(WIDTH_FRAMETIMER, HEIGHT_FRAMETIMER);
	SetPolygonPos(POS_X_FRAMETIMER - 400, POS_Y_FRAMETIMER);

	SetPolygonTexture(g_pTexture);
	DrawPolygon(pDC);

	// �^�C�}�[�\��
	TimeNumber.Draw(XMFLOAT2(POS_X_TIMER - 400, POS_Y_TIMER), (unsigned)(g_nTimer / 60), CHRCNT_TIMER);
}

//=============================================================================
//AddTimer�֐�
//���e:�X�R�A�\�����Z����
//����:�X�R�A�����Z������nScore
//�߂�l:�Ȃ�
//=============================================================================
void AddTimer(int nSec)
{
	g_nTimer += nSec * 60;
}

//=============================================================================
//GetTimer�֐�
//���e:�X�R�A�\���擾����
//����:�Ȃ�
//�߂�l:�X�R�A���擾���邽�߂�g_nTimer
//=============================================================================
int GetTimer()
{
	return g_nTimer / 60;
}