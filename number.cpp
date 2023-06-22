//=============================================================================
// �i���o�[��p�R�[�h�wnumber.cpp�x
// �����:���{�b��
//=============================================================================

//*****************************************************************************
//�C���N���[�h�K�[�h(#include)
//*****************************************************************************
#include "number.h"
#include "Texture.h"
#include "polygon.h"

//*****************************************************************************
// �}�N����`(#define)
//*****************************************************************************
#define PATH_TEXTURE_NUMBER L"data/texture/number000.png"
#define COUNT_X_NUMBER 10
#define COUNT_Y_NUMBER 1

//*****************************************************************************
// �O���[�o���ϐ�(g_)
//*****************************************************************************
static ID3D11ShaderResourceView* g_pTexture;


// ����������
HRESULT CNumber::Init()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	// �e�N�X�`���Ǎ�
	hr = CreateTextureFromFile(pDevice, PATH_TEXTURE_NUMBER, &g_pTexture);

	return hr;
}

// �I������
void CNumber::Uninit()
{
	// �e�N�X�`�����
	SAFE_RELEASE(g_pTexture);
}

// �`�揈��
void CNumber::Draw(XMFLOAT2 vPos, unsigned uNumber, int nWidth,float fSizeX, float fSizeY)
{
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(fSizeX, fSizeY);
	SetPolygonTexture(g_pTexture);
	SetPolygonFrameSize(1.0f / COUNT_X_NUMBER, 1.0f / COUNT_Y_NUMBER);

	vPos.y -= fSizeY * 0.5f;
	vPos.x += fSizeX * (nWidth - 0.5f);

	for (; nWidth > 0; --nWidth, vPos.x -= fSizeX)
	{
		unsigned n = uNumber % 10;
		uNumber /= 10;
		SetPolygonPos(vPos.x, vPos.y);
		SetPolygonUV((n % COUNT_X_NUMBER) / (float)COUNT_X_NUMBER,
			(n / COUNT_X_NUMBER) / (float)COUNT_Y_NUMBER);
		DrawPolygon(pDC);
	}

	// ���ɖ߂�
	SetPolygonUV(0.0f, 0.0f);
	SetPolygonFrameSize(1.0f, 1.0f);
}
