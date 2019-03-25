// error_class.cpp V.T.0.0
#include "error_class.h"

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
        strncpy(file_, file, NAME_LEN);
        strncpy(function_, function, NAME_LEN);
        return;
    }


    My_exception::~My_exception(void)
    {

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

    void My_exception::what(void)
    {
    	if (!ok( ))
    	{
    		dump( );
    		exit(-1);
    	}
    	dump(false);
        
        if (reason_)
        {
        	std::cout << "\n\nThe reason is:\n";
        	(*reason_).what( );
        }

        return;
    }
}
