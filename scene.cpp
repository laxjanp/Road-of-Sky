//=============================================================================
// �V�[����ʁwscene.cpp�x
// �����:���{�b��
//=============================================================================

//*****************************************************************************
//�C���N���[�h�K�[�h(#include)
//*****************************************************************************
#include "scene.h"
#include "title.h"
#include "game.h"
#include "gameover.h"
#include "fade.h"

//*****************************************************************************
// �O���[�o���ϐ�(g_)
//*****************************************************************************
static EScene g_eScene = SCENE_NOME;

//������
HRESULT InitScene()
{
	HRESULT hr = S_OK;
	SetScene(SCENE_TITLE);	//�ŏ��̓^�C�g����ʂ�
	InitFade();				//�t�F�[�h�̏�����������
	return hr;
}

// �I������
void UninitScene()
{
	SetScene(SCENE_NOME);	//���݂̃V�[�����I�����鏈��
	UninitFade();			//�t�F�[�h�̏I��
}

// �X�V
void UpdateScene()
{
	switch (g_eScene)
	{
	case SCENE_TITLE:		//�^�C�g����ʂ̌Ăяo��
		UpdateTitle();		//���̊֐��̌Ăяo��������
		break;
	
	case SCENE_GAME:		//�Q�[����ʂ̌Ăяo��
		UpdateGame();
		break;
	
	case SCENE_GAMEOVER:	//�Q�[���I�[�o�[�̌Ăяo��
		UpdateGameOver();
		break;

	default:
		break;
	}
	UpdateFade();			//�t�F�[�h�̍X�V
}

// �`��
void DrawScene()
{
	switch (g_eScene)
	{
	
	case SCENE_TITLE:		//�^�C�g����ʂ̌Ăяo��
		DrawTitle();		//���̊֐��̌Ăяo��������
		break;

	case SCENE_GAME:		//�Q�[����ʂ̌Ăяo��
		DrawGame();
		break;
	
	case SCENE_GAMEOVER:	//�Q�[���I�[�o�[�̌Ăяo��
		DrawGameOver();
		break;
	
	
	default:
		break;
	}
	DrawFade();				//�t�F�[�h�̕`��
}

//=============================================================================
//SetScene�֐�
//���e:�V�[���̐؂�ւ�������֐�
//����:�V�[���؂�ւ���eScene
//�߂�l:�Ȃ�
//=============================================================================
void SetScene(EScene eScene)
{
	switch (g_eScene)
	{
	
	case SCENE_TITLE:		//�^�C�g����ʂ̌Ăяo��
		UninitTitle();		//���̊֐��̌Ăяo��������
		break;
	
	case SCENE_GAME:		//�Q�[����ʂ̌Ăяo��
		UninitGame();
		break;
	
	case SCENE_GAMEOVER:	//�Q�[���I�[�o�[�̌Ăяo��
		UninitGameOver();
		break;

	}
	
	//�V�[���ؑ�
	g_eScene = eScene;

	//���̉�ʂֈȍ~
	switch (g_eScene)
	{
	
	case SCENE_TITLE:		//�^�C�g����ʂ̌Ăяo��
		InitTitle();		//���̊֐��̌Ăяo��������
		break;
	
	case SCENE_GAME:		//�Q�[����ʂ̌Ăяo��
		InitGame();
		break;
	
	case SCENE_GAMEOVER:	//�Q�[���I�[�o�[�̌Ăяo��
		InitGameOver();
		break;
	}

}