
#ifndef _TIMER_H_
#define _TIMER_H_
//=============================================================================
// タイマーの表示専用コード『timer.h』
// 制作者:松本恵太
//=============================================================================

//*****************************************************************************
//インクルードガード(#include)
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// プロトタイプ宣言(関数呼び出し)
//*****************************************************************************
HRESULT InitTimer(void);	//初期化
void UninitTimer(void);		//終了
void UpdateTimer(void);
void DrawTimer(void);		//描画
void AddTimer(int nSec = 30);
int GetTimer(void);			//更新

#endif // !_TIMER_H_


