#ifndef HELPER_H //HELPER=>std NAME
#define HELPER_H

#include <ctime> //we will use header to retrieve time
#include <string> //convert various type of string bcz we will use string inorder to fill out our log file so keystrokes on the keyboard will get converted into strings and then sending that off to log file
#include <sstream> //converting to string
#include <fstream> //for ostream

namespace Helper //our own name space
{
    template<class T> //generic type, because these type of function can accept anything
    std::string ToString(const T &); //prototype of function, we will use this function in order to convert anything to a string to fill our log files
    //ToString(5); 5 will get converted into string
    struct DateTime //helper struct use in order to represent date and time //struct= user defined type by default: public
    {
        DateTime()
        {
            time_t ms; //represent time located in ctime ms=variable
            time(&ms); //function from ctime library which takes system time and place into a variable that's why we are passing an address of ms
            struct tm *info=localtime(&ms); //local time will receive var address and then it will return a value
            D=info->tm_mday; //D=day and we will use info pointer to indirectly access the attribute tm from the struct tm
            m=info->tm_mon+1; //m=month
            y=1900+info->tm_year; //y=year
            M=info->tm_min; //minute
            H=info->tm_hour; //hour
            S=info->tm_sec; //seconds
        }

        DateTime(int D, int m, int y, int H, int M, int S): D(D), m(m), y(y), H(H), M(M), S(S){} //this will create a date time instance and user will provide date parameters
        DateTime(int D, int m, int y): D(D), m(m), y(y), H(0), M(0), S(0) {}  //this is when time is actually set to zero

        DateTime Now() const //to get current date time //will only read no modification in the class
        {
            return DateTime(); //this will return current date and time
        }
        int D, m, y, H, M, S;

        //format our date
        std::string GetDateString() const
        {
            return std::string(D<10 ? "0" : "")+ ToString(D)+
                   std::string(m<10 ? "0" : ".")+ToString(m)+"."+
                   ToString(y); //DD.mm.YYYY
        }
        //formate our time
        std::string GetTimeString(const std::string &sep=":") const //: is default separator for time
        {
            return std::string(H<10 ? "0" : "")+ToString(H)+sep+
                   std::string(M<10 ? "0" : "")+ToString(M)+sep+
                   std::string(S<10 ? sep : "")+ToString(S);  //HH:MM:SS
        }
        std::string GetDateTimeString(const std::string &sep=":") const
        {
            return GetDateString()+ "" +GetTimeString(sep);
        }
    };

    template <class T>
    std::string ToString(const T &e)
    {
        std::ostringstream s; //var s is type of ostringstream, we will place stuff in it //this will be able to receive any thing that support insertion operator
        s<<e; //the function will take any thing
        //you will only able to pass a type to this function that support an insertion operator << is an insertion operator
        return s.str();
    }
    //logs
    void WriteAppLog(const std::string &s)
    {
        std::ofstream file("AppLog.txt", std::ios::app); //app=append
        file<<"["<<Helper::DateTime().GetDateTimeString()<<"]"<<"\n"<<s<<std::endl<<"\n"; //dumping data into file
        file.close();
    }
}
#endif // HELPER_H
