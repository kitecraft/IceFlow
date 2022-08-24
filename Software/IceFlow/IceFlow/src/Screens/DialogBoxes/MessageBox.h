#pragma once
#include "DialogBase.h"
#include "DialogButton.h"

enum MESSAGE_BOX_ICON {
    MESSAGE_BOX_ICON_ERROR = 0,
    MESSAGE_BOX_ICON_WARNING,
    MESSAGE_BOX_ICON_INFORMATION,
    MESSAGE_BOX_ICON_QUESTION,
};

enum MESSAGE_BOX_BUTTONS {
    MESSAGE_BOX_BUTTONS_OK = 0,
    MESSAGE_BOX_BUTTONS_OK_CANCEL,
    MESSAGE_BOX_BUTTONS_SUBMIT,
    MESSAGE_BOX_BUTTONS_SUBMIT_CANCEL,
    MESSAGE_BOX_BUTTONS_CONTINUE,
    MESSAGE_BOX_BUTTONS_CONTINUE_CANCEL
};

#define MESSAGE_BOX_WIDTH 200
#define MESSAGE_BOX_TEXT_X 10
#define MESSAGE_BOX_TEXT_Y_START (LARGE_FONT_TEXT_BOX_H + 14)
#define MESSAGE_BOX_MAX_LINE_LENGTH 27
#define MESSAGE_BOX_LINE_HEIGHT 16

class MessageBox :
    public DialogBase
{
private:
    DialogButton* _buttonA = nullptr;
    DialogButton* _buttonB = nullptr;

    int _singleButtonX;
    int _dualButtons_ButtonA_X;
    int _dualButtons_ButtonB_X;
    int _buttonY;

    void PrintMessage(String message);

    void SetButtons(MESSAGE_BOX_BUTTONS buttons);
    void CreateOK();
    void CreateOKCancel();
    void CreateSubmit();
    void CreateSubmitCancel();
    void CreateContinue();
    void CreateContinueCancel();

    void DrawIcon(MESSAGE_BOX_ICON icon);
    void DrawErrorIcon();
    void DrawWarningIcon();
    void DrawInfoIcon();
    void DrawQuestionIcon();
public:
    MessageBox();
    ~MessageBox();
    MessageBox(TFT_eSPI* tft, String title, String message, MESSAGE_BOX_ICON icon, MESSAGE_BOX_BUTTONS buttons);

    DialogButtonType Touched(int x, int y);
};

