#include <iostream>
#include "error_class.h"

void first_level(void);
void second_level(void);
void third_level(void);


int main( )
{
    try
    {
    	first_level( );
    }
    catch (error_lib::My_exception* exception)
    {
    	exception -> what( );
    }

    return 0;
}

void first_level(void)
{
    std::cout << "First level!!!\n";
    try
    {
        second_level( );            	
    }
    catch (error_lib::My_exception* exception2)
    {
        static error_lib::My_exception* exception1 = CREATE_EXCEPTION(3,
        "Студент сегодня получил нуль баллов за задание.\n", exception2);
        throw exception1;
    }
}

void second_level(void)
{
    std::cout << "Second level!!!\n";
    try
    {
        third_level( );            	
    }
    catch (error_lib::My_exception* exception3)
    {
        error_lib::My_exception* exception2 = CREATE_EXCEPTION(2,
        "Потому что он плохо подготовился.\n", exception3);
        throw exception2;
    }

}

void third_level(void)
{
    std::cout << "Third level!!!\n";
    error_lib::My_exception* exception3 = CREATE_EXCEPTION(1, "Не знал формулу Байеса.\n", nullptr);
    throw exception3;
}
