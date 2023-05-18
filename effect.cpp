//=============================================================================
// �����G�t�F�N�g�p�\�[�X�R�[�h�weffect.cpp�x
// �����:���{�b��
//=============================================================================

//*****************************************************************************
//�C���N���[�h�K�[�h(#include)
//*****************************************************************************
#include "effect.h"
#include "Texture.h"
#include "polygon.h"

//*****************************************************************************
// �}�N����`(#define)
//*****************************************************************************
#define PATH_EFFECT_TEXTURE L"data/texture/effect.png"
#define POS_X_EFFECT 0.0f
#define POS_Y_EFFECT 0.0f
#define EFFECT_WIDTH 64
#define EFFECT_HEIGHT 64

#define MAX_EFFECT 100		//�ő吔

#define COUNT_X_EFFECT 7	//���g��
#define COUNT_Y_EFFECT 2	//�c�g��

//*****************************************************************************
// �\���̒�`(struct)
//*****************************************************************************
struct EFFECT
{
	XMFLOAT2 vPos;			// �ʒu
	int nFrame;				// �gNo
	int nCount;				// �t���[����
};

//*****************************************************************************
// �O���[�o���ϐ�(g_)
//*****************************************************************************
static ID3D11ShaderResourceView* g_pTexture;
static EFFECT g_effect[MAX_EFFECT];		// �G�t�F�N�g���
static int g_nEffect;					// �g�p���G�t�F�N�g��

// ������
HRESULT InitEffect()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	// �e�N�X�`���Ǎ�
	hr = CreateTextureFromFile(pDevice, PATH_EFFECT_TEXTURE, &g_pTexture);
	if (FAILED(hr))
	{
		return hr;
	}

	// ������
	g_nEffect = 0;
	return hr;
}

// �I������
void UninitEffect()
{
	// �e�N�X�`�����
	SAFE_RELEASE(g_pTexture);
}

// �X�V
void UpdateEffect()
{
	EFFECT* pEffect;
	for (int i = g_nEffect - 1; i >= 0; --i)
	{
		pEffect = &g_effect[i];
		
		// �A�j���[�V�����X�V
		pEffect->nCount--;
		if (pEffect->nCount <= 0)
		{
			pEffect->nFrame++;
			if (pEffect->nFrame > 13)
			{
				// ����
				--g_nEffect;
				if (g_nEffect > 0)
				{
					*pEffect = g_effect[g_nEffect];
				}
				continue;
			}
			pEffect->nCount = 3;
		}
	}
}

// �`��
void DrawEffect()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(EFFECT_WIDTH, EFFECT_HEIGHT);
	SetPolygonTexture(g_pTexture);
	SetPolygonFrameSize(1.0f / COUNT_X_EFFECT, 1.0f / COUNT_Y_EFFECT);
	EFFECT* pEffect = g_effect;
	for (int i = 0; i < g_nEffect; ++i, ++pEffect)
	{
		SetPolygonUV((pEffect->nFrame % COUNT_X_EFFECT) / (float)COUNT_X_EFFECT,
			(pEffect->nFrame / COUNT_X_EFFECT) / (float)COUNT_Y_EFFECT);
		SetPolygonPos(pEffect->vPos.x, pEffect->vPos.y);
		DrawPolygon(pDC);
	}
	// ���ɖ߂�
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(0.0f, 0.0f);

}

//========================================
//StartEffect�֐�
//���e:
//�߂�l:
//========================================
int StartEffect(XMFLOAT2 vPos)
{
	if (g_nEffect < MAX_EFFECT)
	{
		EFFECT* pEffect = &g_effect[g_nEffect];
		pEffect->vPos = vPos;
		pEffect->nFrame = 0;
		pEffect->nCount = 3;
		++g_nEffect;
		return g_nEffect;
	}

	return 0;
}