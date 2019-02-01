#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include "stack2.cpp"
#include "proc_consts.h"
#include <ctype.h>


const int MAX_COMMAND = 300;
long long LABELS[MAX_LABELS] = {0}; // array of labels to jump on these labels.
double LABELS1[MAX_LABELS] = {0};
double label_now = 0;


double command_position = 0; // position to add it in LABELS.
bool first_comp = true;



enum errors 
{
    PUSH_RESTR_FAILURE = 1, 
    PUSH_RAM_NOT_A_DIGIT, 
    INCORRECT_ARG_PUSH_RAM, 
    NO_ARG_PUSH,
    NOT_DOUBLE_PUSH, 
    PUSH_INCORRECT_REGISTER, 
    PUSH_FAILURE, 
    POP_RESTR_FAILURE, 
    POP_RAM_NOT_A_DIGIT,
    INCORRECT_ARG_POP_RAM, 
    INCORRECT_ARG_POP_NO_RAM, 
    POP_FAILURE, 
    SQRT_RAM_NOT_A_DIGIT, 
    NO_ARG_SQRT, 
    NOT_DOUBLE_SQRT,
    INCORRECT_ARG_SQRT_RAM,
    SQRT_NEGATIVE_VALUE,
    SQRT_INCORRECT_REGISTER,
    SQRT_RESTR_FAILURE, 
    SQRT_FAILURE,
    JMP_FAILURE, 
    JA_FAILURE, 
    JB_FAILURE,
    JC_FAILURE, 
    JAE_FAILURE, 
    JBE_FAILURE, 
    JNE_FAILURE,
    INCORRECT_ARG_JMP, 
    INCORRECT_ARG_JA, 
    INCORRECT_ARG_JB, 
    INCORRECT_ARG_JC, 
    INCORRECT_ARG_JAE, 
    INCORRECT_ARG_JBE,
    INCORRECT_ARG_JNE, 
    INCORRECT_ASSEMB, 
    ERROR_NAME, 
    FWRITE_ERROR, 
    WHAT_REGSTR_ERROR, 
    NOT_CREATE_STACK,
    NOT_CREATE_COMMAND, 
    INCORRECT_LABEL, 
    CALL_FAILURE,
    INCORRECT_CALL_LABEL, 
    NOT_CREATE_RET_STACK, 
    FALSE_CALLRET_LOGIC
}; 


int compile_text(char* program_way);
int write_binary_code(struct stack* stk);
int what_command(struct stack* stk, char* command, FILE* program);
int command_push(struct stack* stk, char* command);
int command_pop(struct stack* stk, char* command);
int command_add(struct stack* stk);
int command_mul(struct stack* stk);
int command_sub(struct stack* stk);
int command_div(struct stack* stk);
int command_out(struct stack* stk);
int command_sqrt(struct stack* stk, char* command);
int command_jmp(struct stack* stk, char* command);
int command_ja(struct stack* stk, char* command);
int command_jb(struct stack* stk, char* command);
int command_jc(struct stack* stk, char* command);
int command_jae(struct stack* stk, char* command);
int command_jbe(struct stack* stk, char* command);
int command_jne(struct stack* stk, char* command);
int command_call(struct stack* stk, char* command);
int command_ret(struct stack* stk);
int command_get(struct stack* stk);
int command_label(struct stack* stk, char* command);
int command_end(struct stack* stk);
int error_func(int code_comp_test);
int what_register(struct stack* stk, char symbol);
int poison_labels(void); // set all labels as units to find more mistakes.
int increase_pos(int times);
int delete_comments(FILE *program);


int main(int argc, char* argv[ ])
{
	if (argc <= 1)
	{
        printf("Too few arguments for this function!!!\n");        
        return EXIT_FAILURE;
	}
    poison_labels( );

    // first compilation
    int code_comp_text = compile_text(argv[1]);
    first_comp = false;
    code_comp_text = compile_text(argv[1]);
    return error_func(code_comp_text);
}


int poison_labels(void)
{
    for (int ind = 0; ind < MAX_LABELS; ind++)
    {
        LABELS[ind] = -1;
    }
    

    return SUCCESS;
}


int increase_pos(double times)
{
    
    command_position += times;
    return SUCCESS;
}


int compile_text(char* program_way)
{
    assert(program_way != nullptr);

    FILE* program = fopen(program_way, "r");
    if (program == nullptr)
    {
    	return ERROR_NAME;
    }


    struct stack* stk = stack_get(START_LEN);
    if (!stk)
    {
        return NOT_CREATE_STACK;
    }
    char* command = (char*)calloc(1, MAX_COMMAND);
    if (!command)
    {
        return NOT_CREATE_COMMAND;
    }
    char code_scanf = EOF;
    int code_what_program = 0xBEDA;


    while ((code_scanf = fscanf(program, "%s", command)) != EOF && code_scanf)
    {
        code_what_program = what_command(stk, command, program);
        if (code_what_program)
        {
        	free(command);
            fclose(program);
            return code_what_program;
        }
    }
    

    free(command);
    fclose(program);
    if (first_comp)
    {
        free(stk -> data);
        free(stk);
        command_position = 0;
        return SUCCESS;
    }
    else
    {
        return write_binary_code(stk);
    }
} 


int write_binary_code(struct stack* stk)
{
    assert(stk != nullptr);


    FILE* proc_file = fopen("proc_code.h", "wb");
    size_t num_elem = 0;
    num_elem = fwrite(stk -> data, sizeof(double), stk -> count, proc_file);
    if (num_elem != stk -> count)
    {
        return FWRITE_ERROR;
    }


    free(stk -> data);
    free(stk);
    fclose(proc_file);
    return SUCCESS;
}


int what_command(struct stack* stk, char* command, FILE* program)
{
    assert(stk != nullptr);
    assert(command != nullptr);
    assert(program != nullptr);
    

    if (!strcmp(command, "push"))
    {
        int code_scanf = fscanf(program, "%s", command);
        if (code_scanf && code_scanf != EOF);
        {
            return command_push(stk, command);
        }


        return PUSH_FAILURE;
    }

    else if (!strcmp(command, "pop"))
    {
        int code_scanf = fscanf(program, "%s", command);
        if (code_scanf && code_scanf != EOF);
        {
            return command_pop(stk, command);
        }


        return POP_FAILURE;
    }

    else if (!strcmp(command, "sqrt"))
    {
        int code_scanf = fscanf(program, "%s", command);
        if (code_scanf && code_scanf != EOF);
        {
            return command_sqrt(stk, command);
        }


        return SQRT_FAILURE;
    }

    else if (!strcmp(command, "add"))
    {
        return command_add(stk);
    }

    else if (!strcmp(command, "mul"))
    {
        return command_mul(stk);
    }

    else if (!strcmp(command, "sub"))
    {
        return command_sub(stk);
    }

    else if (!strcmp(command, "div"))
    {
        return command_div(stk);
    }

    else if (!strcmp(command, "out"))
    {
        return command_out(stk);
    }

    else if (!strcmp(command, "jmp"))
    {
        int code_scanf = fscanf(program, "%s", command);
        if (code_scanf && code_scanf != EOF);
        {
            return command_jmp(stk, command);
        }


        return JMP_FAILURE;
    }

    else if (!strcmp(command, "ja"))
    {
        int code_scanf = fscanf(program, "%s", command);
        if (code_scanf && code_scanf != EOF);
        {
            return command_ja(stk, command);
        }


        return JA_FAILURE;
    }

    else if (!strcmp(command, "jb"))
    {
        int code_scanf = fscanf(program, "%s", command);
        if (code_scanf && code_scanf != EOF);
        {
            return command_jb(stk, command);
        }


        return JB_FAILURE;
    }

    else if (!strcmp(command, "jc"))
    {
        int code_scanf = fscanf(program, "%s", command);
        if (code_scanf && code_scanf != EOF);
        {
            return command_jc(stk, command);
        }


        return JC_FAILURE;
    }

    else if (!strcmp(command, "jae"))
    {
        int code_scanf = fscanf(program, "%s", command);
        if (code_scanf && code_scanf != EOF);
        {
            return command_jae(stk, command);
        }


        return JAE_FAILURE;
    }

    else if (!strcmp(command, "jbe"))
    {
        int code_scanf = fscanf(program, "%s", command);
        if (code_scanf && code_scanf != EOF);
        {
            return command_jbe(stk, command);
        }


        return JBE_FAILURE;
    }

    else if (!strcmp(command, "jne"))
    {
        int code_scanf = fscanf(program, "%s", command);
        if (code_scanf && code_scanf != EOF);
        {
            return command_jne(stk, command);
        }


        return JNE_FAILURE;
    }

    else if (!strcmp(command, "call"))
    {
        int code_scanf = fscanf(program, "%s", command);
        if (code_scanf && code_scanf != EOF);
        {
            return command_call(stk, command);
        }


        return CALL_FAILURE;
    }

    else if (!strcmp(command, "ret"))
    {
        return command_ret(stk);
    }

    else if (!strcmp(command, "get"))
    {
        return command_get(stk);
    }

    else if (!strcmp(command, "end"))
    {
        return command_end(stk);
    }

    else if (command[0] == ':')
    {
        return command_label(stk, command);
    }

    else if (command[0] == '/' && command[1] == '/')
    {
        return delete_comments(program);
    }

    else
    {
        return INCORRECT_ASSEMB;
    }
}


int command_push(struct stack* stk, char* command)
{
    assert(stk != nullptr);
    assert(command != nullptr);


    if (command[0] == '[')
    // push from RAM.
    {
        stack_push(stk, (double)PUSH_RAM);

        if (command[1] != 'r')
        {
            if (strlen(command) < 3)
            {
                return INCORRECT_ARG_PUSH_RAM;
            }


            int operative_ind = 0;
            int ind = 0;


            for (ind = 1; command[ind] != ']'; ind++)
            {
                if (isdigit(command[ind]))
                {
                    operative_ind = operative_ind * 10 + command[ind] - '0';
                }
                else 
                {
                    return PUSH_RAM_NOT_A_DIGIT;
                }
            }
            stack_push(stk, (double)ELEM);
            stack_push(stk, (double)operative_ind);
            increase_pos(3.);
            return SUCCESS;
        }

        else if (command[1] == 'r' && command[3] == 'x' && command[4] == ']')
        {
            stack_push(stk, (double)REGSTR);
            int code_what_regstr = 0xBEDA; 
            if ((code_what_regstr = what_register(stk, command[2])) == SUCCESS)
            {
                increase_pos(3.);
                return SUCCESS;
            }

            else
            {
                return PUSH_RESTR_FAILURE;
            }
        }

        else 
        {
            return PUSH_INCORRECT_REGISTER;
        }
    }

    
    else
    {
    	stack_push(stk, (double)PUSH);


    	if (command[0] != 'r')
    	{
            if (strlen(command) < 1)
            {
                return NO_ARG_PUSH;
            }


            double value = -666;
            value = atof(command);
            if (value == 0 && command[0] != '0')
            {
                return NOT_DOUBLE_PUSH;
            }
            stack_push(stk, (double)ELEM);
            stack_push(stk, value);
            increase_pos(3.);
            return SUCCESS;
        }
        else
        {
            if (strlen(command) != 3)
            {
            // 3 is strlen of register
                return PUSH_INCORRECT_REGISTER;
            }


            stack_push(stk, (double)REGSTR);
            int code_what_regstr = 0xBEDA; 
            if ((code_what_regstr = what_register(stk, command[1])) == SUCCESS)
            {
            increase_pos(3.);
            	return SUCCESS;
            }
            else
            {
            	return PUSH_RESTR_FAILURE;
            }
        }
    }
}


int command_pop(struct stack* stk, char* command)
{
    assert(stk != nullptr);
    assert(command != nullptr);

    if (command[0] == '[')
    // pop in RAM.
    {
        stack_push(stk, (double)POP_RAM);


        if (command[1] != 'r')
        {


            if (strlen(command) < 3)
            {
                return INCORRECT_ARG_POP_RAM;
            }    


            int operative_ind = 0;
            int ind = 0;


            for (ind = 1; command[ind] != ']'; ind++)
            {
                if (isdigit(command[ind]))
                {
                    operative_ind = operative_ind * 10 + command[ind] - '0';
                }
                else 
                {
                    return POP_RAM_NOT_A_DIGIT;
                }
            }
            stack_push(stk, (double)ELEM);
            stack_push(stk, (double)operative_ind);
            increase_pos(3.);
            return SUCCESS;
        }
        else if (command[1] == 'r' && command[3] == 'x' && command[4] == ']')
        {
            stack_push(stk, (double)REGSTR);
            int code_what_regstr = 0xBEDA; 
            if ((code_what_regstr = what_register(stk, command[2])) == SUCCESS)
            {
                increase_pos(3.);
                return SUCCESS;
            }

            else
            {
                return POP_RESTR_FAILURE;
            }
        }

        else 
        {
            return POP_RESTR_FAILURE;
        }
    }


    else
    {
        if (strlen(command) != 3)
        // 3 is strlen of register.
        {
            return INCORRECT_ARG_POP_NO_RAM;
        }


    	stack_push(stk, (double)POP_REGSTR);
        int code_what_regstr = 0xBEDA;   
        if ((code_what_regstr = what_register(stk, command[1])) == SUCCESS)
        {
            increase_pos(2.);
            return SUCCESS;
        }

        else
        {
            return POP_RESTR_FAILURE;
        }       
    }
} 


int command_add(struct stack* stk)
{
    assert(stk != nullptr);
    stack_push(stk, (double)ADD);
    increase_pos(1.);
    return SUCCESS;
}


int command_mul(struct stack* stk)
{
    assert(stk != nullptr);
    stack_push(stk, (double)MUL);
    increase_pos(1.);
    return SUCCESS;
}


int command_sub(struct stack* stk)
{
    assert(stk != nullptr);    
    stack_push(stk, (double)SUB);
    increase_pos(1.);
    return SUCCESS;
}


int command_div(struct stack* stk)
{
    assert(stk != nullptr);    
    stack_push(stk, (double)DIV);
    increase_pos(1.);
    return SUCCESS;
}


int command_out(struct stack* stk)
{
    assert(stk != nullptr);    
    stack_push(stk, (double)OUT);
    increase_pos(1.);
    return SUCCESS;
}


int command_sqrt(struct stack* stk, char* command)
{
    assert(stk != nullptr);
    assert(command != nullptr);


    if (command[0] == '[')
    // push from RAM.
    {
        stack_push(stk, (double)SQRT_RAM);


        if (strlen(command) < 3)
        {
            return INCORRECT_ARG_SQRT_RAM;
        }


        int operative_ind = 0;
        int ind = 0;


        for (ind = 1; command[ind] != ']'; ind++)
        {
            if (isdigit(command[ind]))
            {
                operative_ind = operative_ind * 10 + command[ind] - '0';
            }
            else 
            {
                return SQRT_RAM_NOT_A_DIGIT;
            }
        }
        stack_push(stk, (double)operative_ind);
        increase_pos(2.);
        return SUCCESS;
    }
    else
    {
        stack_push(stk, (double)SQRT);


        if (command[0] != 'r')
        {
            if (strlen(command) < 1)
            {
                return NO_ARG_SQRT;
            }


            double value = -666;
            value = atof(command);
            if (value == 0 && command[0] != '0')
            {
                return NOT_DOUBLE_SQRT;
            }
            if (value < 0)
            {
                return SQRT_NEGATIVE_VALUE;
            }


            stack_push(stk, (double)ELEM);
            stack_push(stk, value);
            increase_pos(3.);
            return SUCCESS;
        }
        else
        {
            if (strlen(command) != 3)
            {
            // 3 is strlen of register
                return SQRT_INCORRECT_REGISTER;
            }
            stack_push(stk, (double)REGSTR);


            int code_what_regstr = 0xBEDA; 
            if ((code_what_regstr = what_register(stk, command[1])) == SUCCESS)
            {
                increase_pos(3.);
                return SUCCESS;
            }
            else
            {
                return SQRT_RESTR_FAILURE;
            }
        }
    }
}


int command_jmp(struct stack* stk, char* command)
{
    assert(stk != nullptr);
    assert(command != nullptr);


    increase_pos(2.);
    if (first_comp)
    {
        return SUCCESS;
    }

    stack_push(stk, (double)JMP);
    if (command[0] != ':')
    {
        return INCORRECT_ARG_JMP;
    }


    int label = 0;
    for (int ind = 1; command[ind] != '\0'; ind++)
    {
        if (isdigit(command[ind]))
        {
            label = label * 10 + command[ind] - '0';
        }   
        else
        {
            return INCORRECT_ARG_JMP;               
        }        
    }
    stack_push(stk, (double)LABELS[label]);
    return SUCCESS;
}


int command_ja(struct stack* stk, char* command)
{
    assert(stk != nullptr);
    assert(command != nullptr);


    increase_pos(2.);
    if (first_comp)
    {
        return SUCCESS;
    }


    stack_push(stk, (double)JA);
    if (command[0] != ':')
    {
        return INCORRECT_ARG_JA;
    }


    int label = 0;
    for (int ind = 1; command[ind] != '\0'; ind++)
    {
        if (isdigit(command[ind]))
        {
            label = label * 10 + command[ind] - '0';
        }   
        else
        {
            return INCORRECT_ARG_JA;               
        }        
    }
    stack_push(stk, (double)LABELS[label]);
    return SUCCESS;
}


int command_jb(struct stack* stk, char* command)
{
    assert(stk != nullptr);
    assert(command != nullptr);


    increase_pos(2.);
    if (first_comp)
    {
        return SUCCESS;
    }


    stack_push(stk, (double)JB);
    if (command[0] != ':')
    {
        return INCORRECT_ARG_JB;
    }


    int label = 0;
    for (int ind = 1; command[ind] != '\0'; ind++)
    {
        if (isdigit(command[ind]))
        {
            label = label * 10 + command[ind] - '0';
        }   
        else
        {
            return INCORRECT_ARG_JB;               
        }        
    }
    stack_push(stk, (double)LABELS[label]);
    return SUCCESS;
}


int command_jc(struct stack* stk, char* command)
{
    assert(stk != nullptr);
    assert(command != nullptr);


    increase_pos(2.);
    if (first_comp)
    {
        return SUCCESS;
    }


    stack_push(stk, (double)JC);
    if (command[0] != ':')
    {
        return INCORRECT_ARG_JC;
    }


    int label = 0;
    for (int ind = 1; command[ind] != '\0'; ind++)
    {
        if (isdigit(command[ind]))
        {
            label = label * 10 + command[ind] - '0';
        }   
        else
        {
            return INCORRECT_ARG_JC;               
        }        
    }
    stack_push(stk, (double)LABELS[label]);
    return SUCCESS;
}


int command_jae(struct stack* stk, char* command)
{
    assert(stk != nullptr);
    assert(command != nullptr);


    increase_pos(2.);
    if (first_comp)
    {
        return SUCCESS;
    }


    stack_push(stk, (double)JAE);
    if (command[0] != ':')
    {
        return INCORRECT_ARG_JAE;
    }


    int label = 0;
    for (int ind = 1; command[ind] != '\0'; ind++)
    {
        if (isdigit(command[ind]))
        {
            label = label * 10 + command[ind] - '0';
        }   
        else
        {
            return INCORRECT_ARG_JAE;               
        }        
    }
    stack_push(stk, (double)LABELS[label]);
    return SUCCESS;
}


int command_jbe(struct stack* stk, char* command)
{
    assert(stk != nullptr);
    assert(command != nullptr);


    increase_pos(2.);
    if (first_comp)
    {
        return SUCCESS;
    }



    stack_push(stk, (double)JBE);
    if (command[0] != ':')
    {
        return INCORRECT_ARG_JBE;
    }


    int label = 0;
    for (int ind = 1; command[ind] != '\0'; ind++)
    {
        if (isdigit(command[ind]))
        {
            label = label * 10 + command[ind] - '0';
        }   
        else
        {
            return INCORRECT_ARG_JBE;               
        }        
    }
    stack_push(stk, (double)LABELS[label]);
    return SUCCESS;
}


int command_jne(struct stack* stk, char* command)
{
    assert(stk != nullptr);
    assert(command != nullptr);


    increase_pos(2.);
    if (first_comp)
    {
        return SUCCESS;
    }


    stack_push(stk, (double)JNE);
    if (command[0] != ':')
    {
        return INCORRECT_ARG_JNE;
    }


    int label = 0;
    for (int ind = 1; command[ind] != '\0'; ind++)
    {
        if (isdigit(command[ind]))
        {
            label = label * 10 + command[ind] - '0';
        }   
        else
        {
            return INCORRECT_ARG_JNE;               
        }        
    }
    stack_push(stk, (double)LABELS[label]);
    return SUCCESS;
}
   

int command_call(struct stack* stk, char* command)
// call function.
{
    assert(stk != nullptr);
    assert(command != nullptr);
    

    int code_jmp = 0;
    if ((code_jmp = command_jmp(stk, command)) != SUCCESS)
    {
        return code_jmp;
    }

    int label = 0;
    for (int ind = 1; command[ind] != '\0'; ind++)
    {
        if (isdigit(command[ind]))
        {
            label = label * 10 + command[ind] - '0';
        }   
        else
        {
            return INCORRECT_CALL_LABEL;               
        }        
    }
    LABELS1[label] = command_position;
    return SUCCESS;
}


int command_ret(struct stack* stk)
{
    assert(stk != nullptr);

    increase_pos(2.);
    if (first_comp)
    {
        return SUCCESS;
    }


    stack_push(stk, (double)JMP);


    double position = label_now;
    if (errno != SUCCESS)
    {
        return FALSE_CALLRET_LOGIC;
    }
    stack_push(stk, position);
    return SUCCESS;
}


int command_get(struct stack* stk)
// get number from user and push it in stack.
{
    assert(stk != nullptr);
    stack_push(stk, (double)GET);
    increase_pos(1.);
    return SUCCESS;
}


int command_label(struct stack* stk, char* command)
{
    if (first_comp)
    {
        int label = 0;
        for (int ind = 1; command[ind] != '\0'; ind++)
        {
            if (isdigit(command[ind]))
            {
                label = label * 10 + command[ind] - '0';
            }   
            else
            {
                return INCORRECT_LABEL;               
            }        
        }  

        LABELS[label] = command_position;
        return SUCCESS;
    }
    else
    {
        int label = 0;
        for (int ind = 1; command[ind] != '\0'; ind++)
        {
            if (isdigit(command[ind]))
            {
                label = label * 10 + command[ind] - '0';
            }   
            else
            {
                return INCORRECT_LABEL;               
            }        
        } 

        label_now = LABELS1[label]; 
        return SUCCESS;
    }
} 


int command_end(struct stack* stk)
{
    assert(stk != nullptr);
    increase_pos(1.);
    stack_push(stk, (double)END);
    return SUCCESS;
}


int delete_comments(FILE *program)
{
    assert(program != nullptr);
    char ch = '\0';
    while ((ch = getc(program)) != '\n');
    return SUCCESS;
}


int what_register(struct stack* stk, char symbol)
{
    assert(stk != nullptr);


    switch(symbol)
    {
        case 'a':
            stack_push(stk, (double)RAX);
            break;

        case 'b':
            stack_push(stk, (double)RBX);
            break;

        case 'c':
            stack_push(stk, (double)RCX);
            break;

        case 'd':
            stack_push(stk, (double)RDX);
            break;

        case 'e':
            stack_push(stk, (double)REX);
            break;

        case 'f':
            stack_push(stk, (double)RFX);
            break;

        case 'g':
            stack_push(stk, (double)RGX);
            break;

        case 'h':
            stack_push(stk, (double)RHX);
            break;

        case 'i':
            stack_push(stk, (double)RIX);
            break;
                
        case 'p':
            stack_push(stk, (double)RPX);
            break;

        default:
            return WHAT_REGSTR_ERROR;
    }
    

    return SUCCESS;
}


int error_func(int code_comp_test)
{
    switch(code_comp_test)
    {
        case SUCCESS:
            return EXIT_SUCCESS;


         //push errors   
        case PUSH_RESTR_FAILURE:
            printf("Strange register for push!!!\n");
            return EXIT_FAILURE;
        case PUSH_RAM_NOT_A_DIGIT:
            printf("The argument of push [ind] not a positive integer!!!\n"); 
            return EXIT_FAILURE;
        case INCORRECT_ARG_PUSH_RAM:
            printf("You enter incorrect construction 'push[elem]'!!!\n");
            return EXIT_FAILURE;
        case NO_ARG_PUSH:
            printf("You don't enter argument for push!!!\n");
            return EXIT_FAILURE;
        case NOT_DOUBLE_PUSH:
            printf("Argument of command push isn't digit!!!\n");
            return EXIT_FAILURE;
        case PUSH_INCORRECT_REGISTER:
            printf("Register in construction 'push register' is incorrect!!!\n");
            return EXIT_FAILURE;
        case PUSH_FAILURE:
            printf("Strange argument for push!!!\n");
            return EXIT_FAILURE;


        // pop errors    
        case POP_RESTR_FAILURE:
            printf("Strange register for pop!!!\n");
            return EXIT_FAILURE;
        case POP_RAM_NOT_A_DIGIT:
            printf("The argument of pop [ind] not a positive integer!!!\n"); 
            return EXIT_FAILURE;
        case INCORRECT_ARG_POP_RAM:
            printf("You enter incorrect construction 'pop[elem]'!!!\n");
            return EXIT_FAILURE;
        case INCORRECT_ARG_POP_NO_RAM:
            printf("You enter incorrect construction 'pop register'!!!\n");
            return EXIT_FAILURE;
        case POP_FAILURE:
            printf("Strange argument for pop!!!\n");
            return EXIT_FAILURE;


        // sqrt errors
        case SQRT_RAM_NOT_A_DIGIT:
            printf("The argument of sqrt [ind] not a positive number!!!\n"); 
            return EXIT_FAILURE;
        case NO_ARG_SQRT:
            printf("You don't enter argument for push!!!\n");
            return EXIT_FAILURE;
        case NOT_DOUBLE_SQRT:
            printf("Argument of command push isn't digit!!!\n");
            return EXIT_FAILURE;
        case INCORRECT_ARG_SQRT_RAM:
            printf("You enter incorrect construction 'sqrt [ind]'!!!\n");
            return EXIT_FAILURE;
        case SQRT_NEGATIVE_VALUE:
            printf("Number in construction 'sqrt number' has to be positive!!!\n");
            return EXIT_FAILURE;
        case SQRT_INCORRECT_REGISTER:
            printf("Register in construction 'sqrt register' is incorrect!!!\n");
            return EXIT_FAILURE;
        case SQRT_RESTR_FAILURE:
            printf("Strange register for sqrt!!!\n");
            return EXIT_FAILURE;
        case SQRT_FAILURE:
            printf("Strange argument for sqrt!!!\n");
            return EXIT_FAILURE;


        //jump errors
        case JMP_FAILURE:
            printf("Strange argument for jmp!!!\n");
            return EXIT_FAILURE;
        case JA_FAILURE:
            printf("Strange argument for ja!!!\n");
            return EXIT_FAILURE;
        case JB_FAILURE:
            printf("Strange argument for jb!!!\n");
            return EXIT_FAILURE;
        case JC_FAILURE:
            printf("Strange argument for jc!!!\n");
            return EXIT_FAILURE;
        case JAE_FAILURE:
            printf("Strange argument for jae!!!\n");
            return EXIT_FAILURE;
        case JBE_FAILURE:
            printf("Strange argument for jbe!!!\n");
            return EXIT_FAILURE;
        case JNE_FAILURE:
            printf("Strange argument for jne!!!\n");
            return EXIT_FAILURE;


        case INCORRECT_ARG_JMP:
            printf("Incorrect argument for jmp!!!\n");
            return EXIT_FAILURE;
        case INCORRECT_ARG_JA:
            printf("Incorrect argument for ja!!!\n");
            return EXIT_FAILURE;
        case INCORRECT_ARG_JB:
            printf("Incorrect argument for jb!!!\n");
            return EXIT_FAILURE;
        case INCORRECT_ARG_JC:
            printf("Incorrect argument for jc!!!\n");
            return EXIT_FAILURE;
        case INCORRECT_ARG_JAE:
            printf("Incorrect argument for jae!!!\n");
            return EXIT_FAILURE;
        case INCORRECT_ARG_JBE:
            printf("Incorrect argument for jbe!!!\n");
            return EXIT_FAILURE;
        case INCORRECT_ARG_JNE:
            printf("Incorrect argument for jne!!!\n");
            return EXIT_FAILURE; 
       

        // other errors    
        case INCORRECT_ASSEMB:
            printf("You write incorrect accembler command!!!\n");
            return EXIT_FAILURE;      
        case ERROR_NAME:
            printf("Incorrect name of file!!!\n");
            return EXIT_FAILURE; 
        case FWRITE_ERROR:
            printf("Error of writing in binary file!!!\n");
            return EXIT_FAILURE;
        case WHAT_REGSTR_ERROR:
            printf("You enter incorrect name of register!!!\n");
            return EXIT_FAILURE;
        case NOT_CREATE_STACK:
            printf("Not create stack!!!\n");
            return EXIT_FAILURE;
        case NOT_CREATE_COMMAND:
            printf("Error of calloc memory for command!!!\n");
            return EXIT_FAILURE;
        case INCORRECT_LABEL:
            printf("You enter incorrect label in construction ':label' !!!\n");
            return EXIT_FAILURE;
        case CALL_FAILURE:
            printf("Incorrect argument for call!!!\n");
            return EXIT_FAILURE; 
        case INCORRECT_CALL_LABEL:
            printf("Incorrect call label!!!\n");
            return EXIT_FAILURE;
        case NOT_CREATE_RET_STACK:
            printf("Ret stack didn't create!!!\n");
            return EXIT_FAILURE;
        case FALSE_CALLRET_LOGIC:
            printf("Logic of commands call and ret is incorrect!!!\n");
            return EXIT_FAILURE;            
        default:
            printf("Strange error!!!\n");
            return EXIT_FAILURE;
    }
}
