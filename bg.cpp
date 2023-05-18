//=============================================================================
// �w�i�p�\�[�X�R�[�h�wbg.cpp�x
// �����:���{�b��
//=============================================================================

//*****************************************************************************
//�C���N���[�h�K�[�h(#include)
//*****************************************************************************
#include "bg.h"
#include "polygon.h"
#include "Texture.h"


//*****************************************************************************
// �}�N����`(#define)
//*****************************************************************************
#define PATH_TEXTURE_BG		L"data/texture/sky_002.jpg"
#define POS_X_BG	0.0f
#define POS_Y_BG	0.0f
#define WIDTH_BG	SCREEN_WIDTH
#define HEIGHT_BG	SCREEN_HEIGHT
#define MAX_BG 10

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11ShaderResourceView* g_pTexture;
static XMFLOAT2	g_vPos;		// �ʒu
static int g_nDir;
static int g_nBg;

//������
HRESULT InitBG()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	// �e�N�X�`���Ǎ�
	hr = CreateTextureFromFile(pDevice, PATH_TEXTURE_BG, &g_pTexture);
	if (FAILED(hr))
	{
		return hr;
	}

	//�ϐ��̏�����
	g_vPos.x = POS_X_BG;
	g_vPos.y = POS_Y_BG;
	
	return hr;
}


//�I��
void UninitBG()
{
	//�e�N�X�`���̉��
	SAFE_RELEASE(g_pTexture);
}


//�X�V
void UpdateBG()
{
	//�������Ȃ�
	
}


//�`��
void DrawBG()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(WIDTH_BG, HEIGHT_BG);	//�|���S���̃T�C�Y
	SetPolygonPos(g_vPos.x, g_vPos.y);		//�|���S���̍��W
	SetPolygonTexture(g_pTexture);			//�|���S���̔w�i
	SetPolygonColor(1.0f, 1.0f, 1.0f);		//�F
	DrawPolygon(pDC);

}


