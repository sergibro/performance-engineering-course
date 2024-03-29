#include <iostream>
#include <thread>
void threadFunction(int * q)
{
    std::cout<<"In Thread :  "" : q = "<< *q << std::endl;
    std::chrono::milliseconds t( 1000 );
    std::this_thread::sleep_for( t );
    *q = 113;
    std::cout<<"In Thread :  "" : q = "<< *q  << std::endl;
}
void startThread()
{
    int x = 10;
    std::cout<<"Main Thread :  "" : x = "<< x <<std::endl;
    std::thread t(threadFunction,&x);
    t.join();
    std::cout<<"Main Thread :  "" : x = "<< x <<std::endl;
}
int main()
{
    startThread();
    std::chrono::milliseconds t( 2000 );
    std::this_thread::sleep_for( t );
    return 0;
}
