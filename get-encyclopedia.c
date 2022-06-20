#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <errno.h>

const int SUCCESS = 0;
const int STAT_ERROR = -1;
const int OPEN_ERROR = -2;
const int READ_ERROR = -3;
const int STRANGE_FIRST_FOR = -4;
const int STRANGE_SECOND_FOR = -5;


/**
    \param[in] textfile - Way to the file, that has to be open.
    \param[out] size_text - Size of the text, that was found.
    \return In case of success returns pointer on first element in string with text. String is created dynamically. 
    \return In case of failure returns nullptr.
    \return Function change errno.
    \throw errno = SUCCESS - Program ended rightly.
    \throw errno = OPEN_ERROR - Error of function open( ).
    \throw errno = STAT_ERROR - Error of function stat( ).
    \throw errno = READ_ERROR - Error of function read( ).
 */
char* read_text(char* textfile, size_t* size_text);


/**
    \param[in] str - String with text to find number of strings in it.
    \return In case of success returns number of strings. 
    \return In case of failure returns -1.
    \return Function change errno.
    \throw errno = SUCCESS - Program ended rightly.
    \throw errno = STRANGE_FIRST_FOR - Error of cycle for ( ).
 */
int find_num_strings(char* str);


/**
    \param[in] str - String with text to create table of permutation.
    \param[out] num_strings - Pointer on part of memeory, to write number of strings at this part.
    \return In case of success returns pointer on first element in table of permutation. 
    \return Table of permutation is created dynamically.
    \return In case of failure returns nullptr.
    \return Function change errno.
    \throw errno = SUCCESS - Program ended rightly.
    \throw errno = STRANGE_SECOND_FOR - Error of cycle for ( ).
 */
char** create_table_permutation(char* str, int* num_strings);


/**
    \param[in] table_permutation - Array with pointers on strings in str.
    \param[in] num_strings - Number of strings in table of permutation.
    \param[in] backward - Do you want print it back?
    \return Returns code of error
    \throw SUCCESS - Program ended rightly.
 */
int print_str(char* table_permutation[ ], int num_strings, bool backward = false);


/**
    \param[in] arg1 - Pointer on first string for compare.
    \param[in] arg2 - Pointer on second string for compare.
    \return status of comparing two strings.
 */
int compare(const char** arg1, const char** arg2);


int main(int argc, char *argv[])
{
    if (argc >= 2)
    {

        size_t size_text = 0xBEDA;
        char* str = nullptr;
        str = read_text(argv[1], &size_text);


        int code_read_text = errno;
        switch(code_read_text)
        {
            case SUCCESS:
                break;
            case STAT_ERROR:
                fprintf(stderr, "Error: read_test(%s, %p): stat( ).\n", argv[1], &size_text);
                return EXIT_FAILURE;
            case OPEN_ERROR:
                fprintf(stderr, "You enter incorrect file name!!!\n");
                return EXIT_FAILURE;
            case READ_ERROR:
                fprintf(stderr, "Error: read_test(%s, %p): read( ).", argv[1], &size_text);
                return EXIT_FAILURE;
            default:
                fprintf(stderr, "Strange code_read_text = %d.\n", code_read_text);
                return EXIT_FAILURE;
        }


        int num_strings = 0xBEDA;
        char** table_permutation = create_table_permutation(str, &num_strings);
        

        int code_create_table_permutation = errno;
        switch(code_create_table_permutation)
        {
            case SUCCESS:
                break;
            case STRANGE_FIRST_FOR:
                fprintf(stderr, "Error: create_table_permutation(%p, %p): find_num_strings( ): in cycle for (...).\n",
                                                                                          argv[1], &size_text);
                return EXIT_FAILURE;
            case STRANGE_SECOND_FOR:
                fprintf(stderr, "Error: create_table_permutation(%p, %p): in cycle for (...).\n", str, &num_strings);
                return EXIT_FAILURE;
            default:
                fprintf(stderr, "Strange code_create_table_permutation = %d.\n", code_create_table_permutation);
                return EXIT_FAILURE;    
        }


        printf("\n\n\n");
        printf("Original text:\n");
        int code_print_str = print_str(table_permutation, num_strings);
        printf("\n\n\n");


        switch(code_print_str)
        {
            case SUCCESS:
                break;
            default:
                printf("Strange code_Print_str = %d \n", code_print_str);
                return EXIT_FAILURE;
        }
   
        int (*ptr_compare)(const char** , const char**) = &compare;
        qsort(table_permutation, num_strings, sizeof(char*), (int (*)(const void *, const void *))ptr_compare);


        printf("\n\n\n");
        printf("Direct order:\n");
        code_print_str = print_str(table_permutation, num_strings);
        printf("\n\n\n");
        switch(code_print_str)
        {
            case SUCCESS:
                break;
            default:
                printf("Strange code_Print_str = %d \n", code_print_str);
                return EXIT_FAILURE;
        }


        bool backward = true; //It's used to print table of permutation in the back order.
        printf("\n\n\n");
        printf("Reverse order:\n");
        code_print_str = print_str(table_permutation, num_strings, backward);
        switch(code_print_str)
        {
            case SUCCESS:
                break;
            default:
                printf("Strange code_Print_str = %d \n", code_print_str);
                return EXIT_FAILURE;
        }
        free(str);
        free(table_permutation);
        return EXIT_SUCCESS;
    }
    else
    {
        fprintf(stderr, "You should enter one argument. It's way to textfile!\n");
        return EXIT_FAILURE;
    }
}


char* read_text(char* textfile, size_t* size_text)
{
    assert(textfile);
    assert(size_text);


    int fd = 0xBEDA;
    if ((fd = open(textfile, O_RDONLY)) == -1)
    {
        errno = OPEN_ERROR;
        return nullptr;
    }
    struct stat stat_buffer = {0};
    if (stat(textfile, &stat_buffer) != 0)
    {
        errno = STAT_ERROR; 
        return nullptr; 
    }
    *size_text = stat_buffer.st_size + 2; // 2 block of memory are used to adding '\0' and '\n' at the end
                                          // of the file for easy work of function strrchr( ) in future.
    
    char* str = (char*)calloc(*size_text, sizeof(char));
    if (read(fd, str, *size_text - 2) == -1)
    {
        errno = READ_ERROR;
        return nullptr;
    }
    close(fd);

    if ((*size_text - 3) != '\n')
    {
        str[*size_text - 2] = '\n';
    }
    errno = SUCCESS;
    return str;
}


int find_num_strings(char* str)
{
    assert(str);


    int i = 0, num_strings = 0xBEDA;
    char* str_beginning = nullptr;
    for (str_beginning = strchr(str, '\n'); str_beginning; str_beginning = strchr(str_beginning + 1, '\n'), i++); 
    // Find number of strings.     
    if (str_beginning == nullptr)
    {   
        num_strings = i;
        errno = SUCCESS;
        return num_strings;
    }
    else
    {
        errno = STRANGE_FIRST_FOR;
        return -1;
    }
}


char** create_table_permutation(char* str, int* num_strings)
{
    assert(num_strings);
    assert(str);

    *num_strings = find_num_strings(str);
    if (errno != SUCCESS)
    { 
        return nullptr;
    }
    char** table_permutation = (char**)calloc((size_t)*num_strings, sizeof(char*));

    char* str_beginning = nullptr;
    int i = 1;
    table_permutation[0] = str;
    for (str_beginning = strchr(str, '\n'); str_beginning; str_beginning = strchr(str_beginning + 1, '\n'), i++)
    {
        *str_beginning = '\0';
        if (str_beginning + 1)
        {
            table_permutation[i] = str_beginning + 1;
        }
    }
    

    if (str_beginning == nullptr)
    {
        errno = SUCCESS;
        return table_permutation;
    }
    else
    {
        errno = STRANGE_SECOND_FOR;
        return nullptr;
    }      
}


int print_str(char* table_permutation[ ], int num_strings, bool backward)
{  
    assert(table_permutation != nullptr);
    assert(num_strings > 0);
    assert(num_strings != 0xBEDA);

    if (backward)
    {
        for (int i = num_strings - 1; i >= 0; i--)
        {
            printf("%s\n", table_permutation[i]);
        }
    }
    else
    {
        for (int i = 0; i < num_strings; i++)
            {
                printf("%s\n", table_permutation[i]);
            }    
    } 
    return SUCCESS;
}


int compare(const char** arg1, const char** arg2)
{
    return strcmp(*arg1, *arg2);
}
