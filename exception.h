/*
Exception is a class for throwing and catching.

string what();
    post: returns the defined error message

void setMsg();
    post: the private data member string msg is assigned with 
    the value of the passed argument, const string& newMsg
*/

#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <string>
using namespace std;

namespace surferwat_exception {
    class Exception {
        public:
            Exception();
            Exception(const string& newMsg);
            ~Exception() {};
            string what();
            void setMsg(const string& newMsg);
        private:
            string msg;
    };
}

#endif 