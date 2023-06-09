//************************************************************************************
// 
// 編集者：伊地田真衣
// 
//************************************************************************************

// BGM/SE定義 (SoundData.h)

enum eBGM {
	BGM_TITLE = 0,	// TITLE
	BGM_GAME,		// GAME
	BGM_MODE,		// SELECT,SOUSA
	BGM_SUMMARY,	// SUMMARY
	BGM_RESULT,		// RESULT

	MAX_BGM
};

enum eSE {
	SE_NORMALBULLET,	// 普通の弾
	SE_DECIDE,			// 決定音
	SE_SELECT,			// 選択
	SE_DAMAGE,			// ダメージ
	SE_KILL,			// 死亡

	MAX_SE
};
