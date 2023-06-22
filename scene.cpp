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

// �N���X��p�ӂ����ϐ���p��
CGame game;				// �Q�[���N���X
CTitle title;			// �^�C�g���N���X
CGameover gameover;		// �Q�[���I�[�o�[�N���X


//������
HRESULT CScene::Init()
{
	HRESULT hr = S_OK;
	SetScene(SCENE_TITLE);	//�ŏ��̓^�C�g����ʂ�
	InitFade();				//�t�F�[�h�̏�����������
	return hr;
}

// �I������
void CScene::Uninit()
{
	SetScene(SCENE_NOME);	//���݂̃V�[�����I�����鏈��
	UninitFade();			//�t�F�[�h�̏I��
}

// �X�V����
void CScene::Update()
{
	switch (g_eScene)
	{
	case SCENE_TITLE:		//�^�C�g����ʂ̌Ăяo��
		title.Update();		//���̊֐��̌Ăяo��������
		break;

	case SCENE_GAME:		//�Q�[����ʂ̌Ăяo��
		game.Update();
		break;

	case SCENE_GAMEOVER:	//�Q�[���I�[�o�[�̌Ăяo��
		gameover.Update();
		break;

	default:
		break;
	}
	UpdateFade();			//�t�F�[�h�̍X�V
}

// �`�揈��
void CScene::Draw()
{
	switch (g_eScene)
	{

	case SCENE_TITLE:		//�^�C�g����ʂ̌Ăяo��
		title.Draw();		//���̊֐��̌Ăяo��������
		break;

	case SCENE_GAME:		//�Q�[����ʂ̌Ăяo��
		game.Draw();
		break;

	case SCENE_GAMEOVER:	//�Q�[���I�[�o�[�̌Ăяo��
		gameover.Draw();
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
void CScene::SetScene(EScene eScene)
{
	switch (g_eScene)
	{
	
	case SCENE_TITLE:		//�^�C�g����ʂ̌Ăяo��
		title.Uninit();		//���̊֐��̌Ăяo��������
		break;
	
	case SCENE_GAME:		//�Q�[����ʂ̌Ăяo��
		game.Uninit();
		break;
	
	case SCENE_GAMEOVER:	//�Q�[���I�[�o�[�̌Ăяo��
		gameover.Uninit();
		break;

	}
	
	//�V�[���ؑ�
	g_eScene = eScene;

	//���̉�ʂֈȍ~
	switch (g_eScene)
	{
	
	case SCENE_TITLE:		//�^�C�g����ʂ̌Ăяo��
		title.Init();		//���̊֐��̌Ăяo��������	
		break;
	
	case SCENE_GAME:		//�Q�[����ʂ̌Ăяo��
		game.Init();
		break;
	
	case SCENE_GAMEOVER:	//�Q�[���I�[�o�[�̌Ăяo��
		gameover.Init();
		break;
	}

}