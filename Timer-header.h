//used for: parallel coding execution, for late code execution, for repeated code execution (means calling func within interval)
//like sending mail after every particular interval
//multiple threading helps in continuous logging of keyboard
//before C++11 there was no support of thread in C++, as we didn't have std support of thread in C++ for Windows that were independent of platform we had to use create thread function from Windows API which is good due to difficult management of thread.
#ifndef TIMER-HEADER_H
#define TIME-HEADER_h

#include<thread>
#include<chrono>
class Timer
{
    std::thread Thread //this is our thread for sync execution, it create var thread of type std::thread from the thread library; we will use this var if v want a sync code execution without blocking the main thread
    bool Alive=false; //state of timer whether it's running or not
    long CallNumber=-1L; //this will determine how may times this var will be used to determine we would like to call a certain function
    long repeat_count=-1L; //amt of time a certain function has already be called
    std::chrono::milliseconds interval=std::chrono::milliseconds(0); //this one represent the interval b/w func calls and default value is zero
    std::function<void(void)>funct=nullptr; //new type in C++11 contain func similar to ptr in function, nullptr means it doesn't point to any function at that moment
    //method
    void SleepAndRun()
    {
        std::this thread::sleep for(interval); //this thread indicate the current thread which created a timer obj
        if(Alive)
            Function()(); //1st () calls the Function while the 2nd one call the function that Function returns
    }
    void ThreadFunc() //this is passed to the thread
    {
        if(CallNumber==Infinite)
            while(Alive)
                SleepAndRun();
        else
            while(repeat_count--)
                SleepAndRun();
    }
    public: 
        static const long Infinite=-1L; // we will use it to state that the func will be called an infinite amt of time so timer would work until we stop it manually
        Timer(){} //use inorder to create timer as global variable

        Timer(const std::function<void(void)>&f):funct (f){} //this constructor will accept a ptr to a function

        Timer(const std::function<void(void)>&f, const unsigned long &i, const long repeat=Time::Infinite):funct (f), interval(std::chrono::milliseconds(i)), CallNumber(repeat){}

        void Start (bool Async=true)
        {
            if(IsAlive())
                return;
            Alive=true;
            repeat_count=CallNumber;
            if(Async)
                Thread=std::thread(ThreadFunc, this);
            else
                this->ThreadFunc();
        }

        //starting timer manually
        void Stop()
        {
            Alive=false;
            Thread.join();
        }

        void SetFunction(const std::function<void(void)>&f) //set function which will be executed, could use when we create a timer within an empty constructor and then we can set value of variables ourself
        {
            funct =f;
        }

        bool IsAlive() const{return Alive;} //it is use to figure out that timer is running or not it get the var alive and based on alive it set true or false
        void RepeatCount(const long r) //it sets no of repeaters
        {
            if(Alive)
                return;
            CallNumber=r;
        }

        long GetLeftCount() const {return repeat_count;} //how many iterations are left

        long RepeatCount() const{return CallNumber;} //get total no of repeatations

        void SetInterval(const unsigned long &i)
        {
            if(Alive)
                return;
            interval=std::chrono::milliseconds(i);
        }
        unsigned long Interval() const {return interval.count();}  //count take the no from the interval
        const std::function<void(void)> &Function() const
        {
            return funct; //this is the 2nd function that is return by Function()()
        }
};
#endif // TIMER
