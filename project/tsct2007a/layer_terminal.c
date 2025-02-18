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
static ITUBackground* sColorTestBackground;



bool TerminalOnEnter(ITUWidget* widget, char* param)
{
    bGloAdminStatus = true;
    
    sInputTextBox = ituSceneFindWidget(&theScene, "InputTextBox");
    assert(sInputTextBox);

    sInfoTextBox = ituSceneFindWidget(&theScene, "InfoTextBox");
    assert(sInfoTextBox);

    sColorTestBackground = ituSceneFindWidget(&theScene, "ColorTestBackground");
    assert(sColorTestBackground);

    bindTextBox(sInputTextBox, sInfoTextBox, sColorTestBackground);
    hookKeyboard(commandInput);

    return true;
}

bool TerminalOnLeave(ITUWidget* widget, char* param)
{
    return true;
}

