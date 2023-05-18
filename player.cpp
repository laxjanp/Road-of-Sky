//=============================================================================
// プレイヤー表示用ソースコード『player.cpp』
// 制作者:松本恵太
//=============================================================================

//*****************************************************************************
//インクルードガード(#include)
//*****************************************************************************
#include "player.h"
#include "polygon.h"
#include "Texture.h"
#include "bullet.h"
#include "input.h"
#include "enemy.h"
#include "enemy2.h"
#include "Sound.h"
#include "life.h"
#include "timer.h"


//*****************************************************************************
// マクロ定義(#define)
//*****************************************************************************
#define PATH_TEXTURE_PLAYER L"data/texture/player001.png"
#define POS_X_PLAYER		-520.0f
#define POS_Y_PLAYER		0.0f
#define PLAYER_WIDTH		64	
#define PLAYER_HEIGHT		64	
#define PLAYER_COUNT_X		3	
#define PLAYER_COUNT_Y		4	
#define SPEED_PLAYER		5.0f	//移動速度
#define COLLISION_WIDTH		20.0f
#define COLLISION_HEIGHT	31.0f
#define DAMAGE_TIMER		180


//*****************************************************************************
// 構造体定義(struct)
//*****************************************************************************
struct PLAYER				//プレイヤーの情報
{
	XMFLOAT2 g_vPos;		// 位置
	int g_nWalk;			// 0:待機, 1:歩行
	int g_nDir;				// 0:下,1:左,2:右,3:上
};

//*****************************************************************************
// グローバル変数(g_)
//*****************************************************************************
static ID3D11ShaderResourceView *g_pTexture;
static XMFLOAT2	g_vPos;				// 位置
static int g_nWalk;					// 0:待機, 1:歩行
static int g_nDir;					// 0:下,1:左,2:右,3:上

static int g_nDamage;				// 点滅中
static bool g_bGameOver;			// ゲームオーバー
static bool g_bGameClear;			// ゲームクリア


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	// テクスチャ読込
	hr = CreateTextureFromFile(pDevice, PATH_TEXTURE_PLAYER, &g_pTexture);
	if (FAILED(hr))
	{
		return hr;
	}

	// 変数初期化
	g_vPos.x = POS_X_PLAYER;
	g_vPos.y = POS_Y_PLAYER;
	g_nWalk = 0;
	g_nDir = 2;
	g_nDamage = 3;
	g_bGameOver = false;

	return hr;
}

// 終了処理
void UninitPlayer()
{
	// テクスチャ解放
	SAFE_RELEASE(g_pTexture);
}

// 更新
void UpdatePlayer()
{
	do {
		// ダメージ処理中
		if (g_nDamage > 0)
		{
			--g_nDamage;

			if (g_nDamage <= 0)
			{
				if (GetLife() <= 0 || GetTimer() <= 0)
				{
					g_bGameOver = true;
					return;
				}
			}
			break;
		}

		if (GetTimer() <= 0)
		{
			g_nDamage = DAMAGE_TIMER;
			break;
		}

		// 移動の初期化
		g_nWalk = 0;

		//Wキーか上キーを押したら上に移動する
		if (GetKeyPress(VK_W) || GetKeyPress(VK_UP))
		{
			g_nWalk = 1;
			g_nDir = 3;
			g_vPos.y += SPEED_PLAYER;
			if (g_vPos.y < (SCREEN_WIDTH - PLAYER_WIDTH) * -0.5f)
			{
				g_vPos.y = (SCREEN_WIDTH - PLAYER_WIDTH) * -0.5f;

			}
		}

		//Sキーか下キーを押したら下に移動する
		if (GetKeyPress(VK_S) || GetKeyPress(VK_DOWN))
		{
			g_nWalk = 1;
			g_nDir = 0;
			g_vPos.y -= SPEED_PLAYER;
			if (g_vPos.y < (SCREEN_WIDTH - PLAYER_WIDTH) * -0.5f)
			{
				g_vPos.y = (SCREEN_WIDTH - PLAYER_WIDTH) * -0.5f;

			}

		}

		//Aキーか左キーを押したら左に移動する
		if (GetKeyPress(VK_A) || GetKeyPress(VK_LEFT))
		{
			g_nWalk = 1;
			g_nDir = 1;
			g_vPos.x -= SPEED_PLAYER;
			if (g_vPos.x < (SCREEN_WIDTH - PLAYER_WIDTH) * -0.5f)
			{
				g_vPos.x = (SCREEN_WIDTH - PLAYER_WIDTH) * -0.5f;

			}

		}

		//Dキーか右キーを押したら右に移動する
		if (GetKeyPress(VK_D) || GetKeyPress(VK_RIGHT))
		{
			g_nWalk = 1;
			g_nDir = 2;
			g_vPos.x += SPEED_PLAYER;
			if (g_vPos.x > (SCREEN_WIDTH - PLAYER_WIDTH) * 0.5f)
			{
				g_vPos.x = (SCREEN_WIDTH - PLAYER_WIDTH) * 0.5f;

			}

		}

		// Zキーが押したら弾発射
		if (GetKeyTrigger(VK_Z))
		{
			FireBullet(g_vPos, g_nDir);
		}

		// 敵との当たり判定の処理
		XMFLOAT2 vBox(COLLISION_WIDTH, COLLISION_HEIGHT);

		//敵1or敵2のどちらかに触れたらライフを減らす
		if (CollisionEnemy(g_vPos, vBox, 0.0f) >= 0 || CollisionEnemy2(g_vPos, vBox, 0.0f) >= 0)
		{
			DelLife();
			g_nDamage = DAMAGE_TIMER;
			CSound::Play(SE_HIT);
		}

	} while (0);

}

// 描画
void DrawPlayer()
{
	if (g_bGameOver)
	{
		return;
	}

	if (g_nDamage > 0)
	{
		if (g_nDamage & 4)
			return;
	}

	ID3D11DeviceContext* pDC = GetDeviceContext();

	SetPolygonSize(PLAYER_WIDTH, PLAYER_HEIGHT);
	SetPolygonPos(g_vPos.x, g_vPos.y);
	SetPolygonAngle(90);
	SetPolygonTexture(g_pTexture);
	DrawPolygon(pDC);
	SetPolygonUV(0.0f, 0.0f);
	SetPolygonFrameSize(1.0f, 1.0f);
}

// ゲームオーバー
bool IsGameOver()
{
	return g_bGameOver;
}
