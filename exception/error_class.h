// error_class.h V.T.0.0
#pragma once
#include <iostream>
#include <assert.h>
#include <string.h>

namespace error_lib
{

    const int NAME_LEN = 200;
    const int MESSAGE_LEN = 600;
    const int BUF_LIMIT = 40; // Limit on numer of errors, that can be in buffer.

    #define CHECK if(!(My_exception::ok( )))\
    {\
        std::cout << "Error of " << __PRETTY_FUNCTION__ << " method:\n";\
        My_exception::dump( );\
        exit(EXIT_FAILURE);\
    }

    #define CHECK_EXTERN(exception) if(!((exception).ok( )))\
    {\
        std::cout << "Error of " << __PRETTY_FUNCTION__ << " method:\n";\
        (exception).dump( );\
        exit(EXIT_FAILURE);\
    }
// в класссе сделать буфер и в этом буфере сделать предыдущий класс. Переопределяется new. 
// И старый exception в новом сидит. Расширение прошлой задачи.
// catch(const &CE e)
// throw CE("   ", new CE(e)); 
    class My_exception
    {
        private:
            int code_;
            char message_[MESSAGE_LEN + 1];
            char file_[NAME_LEN + 1];
            char function_[NAME_LEN + 1];
            int line_;
            My_exception* reason_;

            void swap_and_kill(My_exception &);
        public:
        	My_exception(int code, const char* message, const char* file, const char* function,
        	             int line, My_exception* reason);
        	My_exception(void);
        	~My_exception(void);
        	My_exception(const My_exception &);
        	bool ok(void);// имена параметров надо писать.Так как может не понять чиаттель.
        	void dump(bool more = true);
            My_exception & operator=(My_exception &&);

            My_exception & operator=(const My_exception &) = delete;

            void what(void);
    };

};

void* operator new[](std::size_t size, int code, const char* message, const char* file,
                     const char* function, int line, error_lib::My_exception* reason);

    #define CREATE_EXCEPTION(code,message,reason) (error_lib::My_exception*)new(code,message,__FILE__,__PRETTY_FUNCTION__,\
__LINE__,reason)char[sizeof(error_lib::My_exception)]
// перевод в тип bool и int
