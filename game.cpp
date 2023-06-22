//=============================================================================
// �Q�[�����wgame.cpp�x
// �����:���{ �b��
//=============================================================================

//*****************************************************************************
//�C���N���[�h�K�[�h(#include)
//*****************************************************************************
#include "game.h"

#include "polygon.h"
#include "Sound.h"
#include "scene.h"
#include "fade.h"
#include "input.h"

#include "player.h"
#include "bg.h"
#include "bullet.h"
#include "effect.h"
#include "enemy.h"
#include "enemy2.h"
#include "score.h"
#include "timer.h"
#include "life.h"
#include "pause.h"

//CTimer GameTimer;

// ����������
HRESULT CGame::Init()
{
	HRESULT hr = S_OK;

	// �v���C���[������
	hr = InitPlayer();
	if (FAILED(hr))
	{
		MessageBox(GetMainWnd(), _T("�v���C���[���������s"), NULL, MB_OK);
		return hr;
	}

	// �w�i������
	hr = InitBG();
	if (FAILED(hr))
	{
		MessageBox(GetMainWnd(), _T("�w�i���������s"), NULL, MB_OK);
		return hr;
	}

	// �e������
	hr = InitBullet();
	if (FAILED(hr))
	{
		MessageBox(GetMainWnd(), _T("�e���������s"), NULL, MB_OK | MB_ICONSTOP);
		return hr;
	}

	// �G�t�F�N�g�\��������
	hr = InitEffect();
	if (FAILED(hr)) {
		MessageBox(GetMainWnd(), _T("�G�t�F�N�g�̏��������s"), NULL, MB_OK | MB_ICONSTOP);
		return hr;
	}

	//�G�̏�����
	hr = InitEnemy();
	if (FAILED(hr))
	{
		MessageBox(GetMainWnd(), _T("�G�̏��������s"), NULL, MB_OK | MB_ICONSTOP);
		return hr;
	}

	hr = InitEnemy2();
	if (FAILED(hr))
	{
		MessageBox(GetMainWnd(), _T("�G�̏��������s"), NULL, MB_OK | MB_ICONSTOP);
		return hr;
	}

	// �X�R�A�\��������
	hr = InitScore();
	if (FAILED(hr))
	{
		MessageBox(GetMainWnd(), _T("�X�R�A�\�����������s"), NULL, MB_OK | MB_ICONSTOP);
		return hr;
	}

	// �^�C�}�[�\��������
	hr = InitTimer();
	//hr = GameTimer.Init();
	if (FAILED(hr))
	{
		MessageBox(GetMainWnd(), _T("�^�C�}�[�\�����������s"), NULL, MB_OK | MB_ICONSTOP);
		return hr;
	}

	// ���C�t�\��������
	hr = InitLife();
	if (FAILED(hr))
	{
		MessageBox(GetMainWnd(), _T("���C�t�̕\�����������s"), NULL, MB_OK | MB_ICONSTOP);
		return hr;
	}

	// �ꎞ��~������
	hr = InitPause();
	if (FAILED(hr))
	{
		MessageBox(GetMainWnd(), _T("�ꎞ��~�������������s"), NULL, MB_OK | MB_ICONSTOP);
		return hr;
	}

	// �t���O�Ǘ��̏�����
	m_bPause = false;

	// BGM�Đ�
	CSound::Play(BGM_CYBER41);

	return hr;
}

// �I������
void CGame::Uninit()
{
	// BGM�Đ���~
	CSound::Stop(BGM_CYBER41);

	// �ꎞ��~�I������
	UninitPause();

	// ���C�t�\���I������
	UninitLife();

	// �^�C�}�[�I������
	UninitTimer();
	//GameTimer.Uninit();

	// �X�R�A�\���I������
	UninitScore();

	// �G�I������
	UninitEnemy();
	UninitEnemy2();

	// �G�t�F�N�g�\���I������
	UninitEffect();

	// �e�I������
	UninitBullet();

	// �w�i�\���I������
	UninitBG();

	// �v���C���[�I������
	UninitPlayer();
}


// �X�V����
void CGame::Update()
{
	// �ꎞ��~��
	if (m_bPause)
	{
		// �ꎞ��~�̍X�V
		UpdatePause();
	}
	else
	{
		// �w�i�X�V
		UpdateBG();

		// �v���C���[�X�V
		UpdatePlayer();
		if (IsGameOver())
		{
			StartFadeOut(SCENE_GAMEOVER);
			return;
		}

		// �e�X�V
		UpdateBullet();

		// �����G�t�F�N�g�\���X�V
		UpdateEffect();

		// �G�X�V
		UpdateEnemy();
		UpdateEnemy2();

		// �^�C�}�[�X�V
		//GameTimer.Update();
		UpdateTimer();
	}

	// �ꎞ��~ON/OFF
	if (GetKeyTrigger(VK_P) || GetKeyTrigger(VK_PAUSE))
	{

		// �t�F�[�h�C��/�A�E�g���łȂ����
		if (GetFade() == FADE_NONE)
		{
			m_bPause = !m_bPause;	// ���]

			if (m_bPause)
			{
				CSound::Play(SE_DECIDE);
				ResetPauseMenu();
			}
			else
			{
				CSound::Play(SE_CANCEL);
				CSound::Resume();
			}
		}
	}

	// �ꎞ��~��(���t�F�[�h�C��/�A�E�g���Ă��Ȃ�)?
	if (m_bPause && GetFade() == FADE_NONE)
	{
		CSound::Stop(SE_DECIDE);
		CSound::Stop(SE_SELECT);

		// [Enter]�������ꂽ?
		if (GetKeyTrigger(VK_RETURN))
		{
			switch (GetPauseMenu())
			{	// ���݂̑I������
			case PAUSE_MENU_CONTINUE:
				m_bPause = false;
				CSound::Play(SE_CANCEL);
				CSound::Resume();
				break;

			case PAUSE_MENU_RETRY:
				CSound::Play(SE_DECIDE);
				StartFadeOut(SCENE_GAME);
				break;

			case PAUSE_MENU_QUIT:
				CSound::Play(SE_DECIDE);
				StartFadeOut(SCENE_TITLE);
				break;
			}
		}
	}
}

// �`�揈��
void CGame::Draw()
{
	// �w�i�`��
	DrawBG();
	
	// �v���C���[�`��
	DrawPlayer();
	
	// �G�`��
	DrawEnemy();
	DrawEnemy2();
	
	// �e�`��
	DrawBullet();
	
	// �����G�t�F�N�g�\��
	DrawEffect();
	
	// �X�R�A�\��
	DrawScore();
	
	// �^�C�}�[�\��
	//GameTimer.Draw();
	DrawTimer();
	
	// ���C�t�\��
	DrawLife();

	// �ꎞ��~�`��
	if (m_bPause) 
	{
		DrawPause();
	}
}
