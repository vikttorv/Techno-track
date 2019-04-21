// error_class.cpp V.T.0.0
#include "error_class.h"
namespace error_lib
{
    My_exception buf[BUF_LIMIT] = { };
    int buf_ind = 0;
};

void* operator new[](std::size_t size, int code, const char* message, const char* file,
                     const char* function, int line, error_lib::My_exception* reason)
{
    assert(message);
    assert(file);
    assert(function);
    assert(line > 0);
    using error_lib::buf;
    using error_lib::buf_ind;
    using error_lib::BUF_LIMIT;

    buf[buf_ind] = error_lib::My_exception(code, message, file, function, line, reason);
    if (buf_ind == BUF_LIMIT - 1)
    {
        buf_ind = 0;
        return &buf[BUF_LIMIT - 1];
    }
    buf_ind++;

    CHECK_EXTERN(buf[buf_ind - 1])
    return (void*)&buf[buf_ind - 1];
}

namespace error_lib
{   
    My_exception::My_exception(int code, const char* message, const char* file, const char* function, int line,
                 My_exception* reason):
        code_ (code),    
        line_ (line),
        reason_ (reason)
    {
        assert(message);
        assert(file);
        assert(line >= 1);
        assert(function);

        strncpy(message_, message, MESSAGE_LEN); 
        message_[MESSAGE_LEN] = '\0'; 
        strncpy(file_, file, NAME_LEN);
        file_[NAME_LEN] = '\0';
        strncpy(function_, function, NAME_LEN);
        function_[NAME_LEN] = '\0';
        return;
    }


    My_exception::~My_exception(void)
    {

    }

    My_exception::My_exception(void):
    code_ (0),
    line_ (0),
    reason_ (nullptr)
    {
        for (int ind = 0; ind <= MESSAGE_LEN; ind++)
        {
            message_[ind] = '\0';
            if (ind <= NAME_LEN)
            {
                function_[ind] = '\0';
                file_[ind] = '\0';
            }
        }
    }

    My_exception::My_exception(const My_exception & exception):
        code_ (exception.code_),
        line_ (exception.line_),
        reason_ (exception.reason_)
    {
        strcpy(message_, exception.message_);
        strcpy(file_, exception.file_);
        strcpy(function_, exception.function_);
        return;
    }

    void My_exception::dump(bool more)
    {
    	std::cout << "Code of error = " << code_ << std::endl;
        std::cout << "Message is:\n" << message_ << std::endl;
        std::cout << "In file:  " << file_ << std::endl;
        std::cout << "In function:  " << function_ << std::endl;
        std::cout << "In line:  " << line_ << std::endl;
        if (more)
        {
            std::cout << "Pointer on reason = " << reason_ << std::endl;
        }
        return;
    }


    bool My_exception::ok(void)
    {
    	return message_ && file_ && function_ && line_ > 0;
    }

// that справа
    My_exception & My_exception::operator=(My_exception && exception)
    {
    	CHECK_EXTERN(exception)
    	swap_and_kill(exception);
    	CHECK
    	return *this;
    }

    void My_exception::swap_and_kill(My_exception & exception)
    {
    	CHECK_EXTERN(exception)

    	code_ = exception.code_;
    	line_ = exception.line_;
        reason_ = exception.reason_;
        strncpy(message_, exception.message_, strlen(exception.message_) + 1);
        strncpy(file_, exception.file_, strlen(exception.file_) + 1);
        strncpy(function_, exception.function_, strlen(exception.function_) + 1);   
        CHECK     
    }

    void My_exception::what(void)
    {
    	CHECK
    	dump(false);
        
        if (reason_)
        {
        	std::cout << "\n\nThe reason is:\n";
        	reason_ -> what( );
        }

    	CHECK
        return;
    }

};
