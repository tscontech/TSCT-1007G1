#include <assert.h>
#include "scene.h"
#include "keyboardCommand.h"

/* widgets:
TerminalLayer
TerminalBackground
InputTextBox
InfoTextBox
*/

static ITUTextBox* sInputTextBox;
static ITUTextBox* sInfoTextBox;



bool TerminalOnEnter(ITUWidget* widget, char* param)
{
    sInputTextBox = ituSceneFindWidget(&theScene, "InputTextBox");
    assert(sTriggerBtn);

    sInfoTextBox = ituSceneFindWidget(&theScene, "InfoTextBox");
    assert(sTriggerBtn);

    bindTextBox(sInputTextBox, sInfoTextBox);
    hookKeyboard(commandInput);

    return true;
}

bool TerminalOnLeave(ITUWidget* widget, char* param)
{
    return true;
}

