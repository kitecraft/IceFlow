#pragma once
#include "BaseScreen.h"
#include "Panels/RFS_Sidebar.h"
#include "../OvenController/ReflowStage.h"
#include "DialogBoxes/MessageBox.h"

enum RFS_MESSAGE_BOX_TYPE {
	RFS_NO_MB,
	RFS_CANCEL_MB,
	RFS_COMPLETE_MB
};

class ReflowScreen : BaseScreen
{
private:
	void ProcessTouch(int x, int y);
	RFS_Sidebar* _sidebar = nullptr;
	REFLOW_STAGE _reflowStage = RS_NOT_ACTIVE;
	unsigned long _nextSideBarUpdate = 0;
	unsigned long _startTime = 0;


	MessageBox* _messageBox = nullptr;
	RFS_MESSAGE_BOX_TYPE _activeMessageBox = RFS_NO_MB;

	void PopUpReflowCompleteMB(char* runTime);
	void HandleMessageBoxTouch(int x, int y);
	void EndMessageBox();

	void StartPreHeat();
	void DrawScreen();
public:
	ReflowScreen(TFT_eSPI* tft);
	~ReflowScreen();

	void UpdateScreen(int inKey, char* value);
	void UpdateScreenOnInterval();

	void HandleTouch(int x, int y);
};


static ReflowScreen* reflowScreen = nullptr;
static void Create_ReflowScreen(TFT_eSPI* TFT)
{
	reflowScreen = new ReflowScreen(TFT);
}

static void Destroy_ReflowScreen()
{
	if (reflowScreen != nullptr)
	{
		delete(reflowScreen);
		reflowScreen = nullptr;
	}
}

static void Update_ReflowScreen(int inKey, char* value)
{
	reflowScreen->UpdateScreen(inKey, value);
}

static void Update_ReflowScreen_OnInterval()
{
	reflowScreen->UpdateScreenOnInterval();
}

static void HandleTouch_ReflowScreen(int x, int y)
{
	reflowScreen->HandleTouch(x, y);
}


