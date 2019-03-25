// error_class.h V.T.0.0
#pragma once
#include <iostream>
#include <assert.h>
#include <string.h>

namespace error_lib
{
    #define CREATE_EXEPTION(code, message, reason) My_exception(code,message,__FILE__,__PRETTY_FUNCTION__,__LINE__\
,reason)


    const int NAME_LEN = 200;
    const int MESSAGE_LEN = 600;
    const int BUF_LIMIT = 40; // Limit on numer of errors, that can be in buffer.
    My_exception buf[BUF_LIMIT] = {0};


    class My_exception
    {
        private:
            int code_;
            char message_[MESSAGE_LEN];
            char file_[NAME_LEN];
            char function_[NAME_LEN];
            int line_;
            My_exception* reason_;
        public:
        	My_exception(int code, const char* message, const char* file, const char* function,
        	             int line, My_exception* reason);
        	~My_exception(void);
        	My_exception(const My_exception &);
        	bool ok(void);
        	void dump(bool more = true);

            My_exception(void) = delete;
            My_exception & operator=(const My_exception &) = delete;

            void add()
            void what(void);
    };
}
