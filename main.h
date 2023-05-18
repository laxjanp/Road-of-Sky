//=============================================================================
// �T���v������ [main.h]
// �����:���{�b��
//=============================================================================
#pragma once

//*****************************************************************************
// �C���N���[�h�K�[�h(#include)
//*****************************************************************************
#include <windows.h>
#include <tchar.h>
#include <mmsystem.h>
#include <d3d11.h>
#include <DirectXMath.h>

#include "resource.h"

// �{���̓w�b�_�ɏ����Ȃ������ǂ�
using namespace DirectX;

//*****************************************************************************
// �}�N����`(#define)
//*****************************************************************************
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) {if(x){(x)->Release();x=nullptr;}}
#endif
#ifndef SAFE_DELETE
#define SAFE_DELETE(x) {if(x){delete(x);x=nullptr;}}
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) {if(x){delete[](x);x=nullptr;}}
#endif

#define SCREEN_WIDTH	(1280)				// �E�C���h�E�̕�
#define SCREEN_HEIGHT	(720)				// �E�C���h�E�̍���
#define SCREEN_CENTER_X	(SCREEN_WIDTH/2)	// �E�C���h�E�̒��S�w���W
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT/2)	// �E�C���h�E�̒��S�x���W

#define	NUM_VERTEX		(4)					// ���_��
#define	NUM_POLYGON		(2)					// �|���S����

//*****************************************************************************
// �񋓑̒�`(#define)
//*****************************************************************************
enum EBlendState 
{
	BS_NONE = 0,	// ��������������
	BS_ALPHABLEND,	// ����������
	BS_ADDITIVE,	// ���Z����
	BS_SUBTRACTION, // ���Z����
	MAX_BLENDSTATE
};

// ���_�t�H�[�}�b�g( ���_���W[2D] / ���ˌ� / �e�N�X�`�����W )
typedef struct {
	XMFLOAT3 vtx;		// ���_���W
	XMFLOAT4 diffuse;	// �g�U���ˌ�
	XMFLOAT2 tex;		// �e�N�X�`�����W
} VERTEX_2D;

//*****************************************************************************
// �v���g�^�C�v�錾(�֐��Ăяo��)
//*****************************************************************************
HWND GetMainWnd();
HINSTANCE GetInstance();
ID3D11Device* GetDevice();
ID3D11DeviceContext* GetDeviceContext();
void SetBlendState(int nBlendState);
