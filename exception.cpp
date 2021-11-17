/*
Exception has one private data member. The private data member string msg 
is an error message describing the nature of the exception thrown.

Invariants:
- none
*/

#include <string>
#include "exception.h"
using namespace std;

namespace surferwat_exception {
    Exception::Exception() {
        msg = "";
    }





    Exception::Exception(const string& newMsg) {
        msg = newMsg;
    }





    void Exception::setMsg(const string& newMsg) {
        msg = newMsg;
    }





    string Exception::what() {
        return msg;
    }
}