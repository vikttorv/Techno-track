//Square Eq Slv v.1.0 Viktor

#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <math.h>

const int FAILURE_OF_SQUARE = -1;
const int FAILURE_OF_LINEAR = -2;
const int NO_ROOTS = 0;
const int ONE_ROOT = 1;
const int TWO_ROOTS = 2;
const int INFINITE_NUMBER_OF_ROOTS = 3;

int Solve_square(double a, double b, double c, double *root1, double *root2, double *D);
int Solve_linear(double b, double c, double *root1);

int main( )
{
    using namespace std;
	printf("Enter a, b, c:\n");
    double a = NAN, b = NAN, c = NAN, D = NAN;
    int code_scanf = 0xBEDA;
    do
    {
        if (code_scanf != 0xBEDA)
        {
            char rubbish;
            while ((rubbish = getchar( )) != '\n');
            printf("You enter incorrect coefficients. Try to enter they again.\n");
        }
        code_scanf = scanf("%lg %lg %lg", &a, &b, &c);
    }
    while (code_scanf != 3);

    double root1 = NAN, root2 = NAN;
    int num_roots = Solve_square(a, b, c, &root1, &root2, &D);
    switch(num_roots)
    {
        case NO_ROOTS:
            printf("Square equation hasn't roots.\n");
            break;
        case ONE_ROOT: 
            printf("Square equation has 1 root: %lg.\n", root1);
            break;
        case TWO_ROOTS:
            printf("Square equation has 2 roots: root1 = %lg; root2 = %lg.\n", root1, root2);
            break;
        case INFINITE_NUMBER_OF_ROOTS:
            printf("Each number from the set of real numbers is the solution.\n");
            break;
        case FAILURE_OF_SQUARE:
             printf("Strange D = %lg in Solve_square( ), where a = %lg, b = %lg, c = %lg.\n", D, a, b, c);
             break;
        case FAILURE_OF_LINEAR:
             printf("Strange b = %lg in Solve_linear.\n", b);
             break;
        default:
            printf("Strange num_root = %d in Solve_square(%lg, %lg, %lg, %p, %p).\n", num_roots, a, b, c, &root1, &root2);
            return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
/**
    \param[in] a - Coefficient before "x * x".
    \param[in] b - Coefficient before "x".
    \param[in] c - Constant of square equation.
    \param[out] root1 - Pointer on first root's block of memory.
    \param[out] root2 - Pointer on second root's block of memory.
    \param[out] D - Pointer on discriminant's block of memory.
    \return Number of roots.
    \throw FAILURE_OF_SQUARE - Strange discriminant.
    \throw FAILURE_OF_LINEAR - Strange b.
 */
int Solve_square(double a, double b, double c, double *root1, double *root2, double *D)
{
    assert(isfinite(a));
    assert(isfinite(b));
    assert(isfinite(b));
    assert(root1 != nullptr);
    assert(root2 != nullptr);
    assert(D != nullptr);
    if (a == 0)
    {
        return (Solve_linear(b, c, root1));
    }

    *D = b * b - 4 * a * c;
    if (*D < 0)
    {
        return NO_ROOTS;
    }
    else if (*D == 0)
    {
        *root1 = -b / (2 * a);
        return ONE_ROOT;   
    }
    else if (*D > 0)
    {
        *root1 = (-b + sqrt(*D)) / (2 * a);
        *root2 = (-b - sqrt(*D)) / (2 * a);
        return TWO_ROOTS;
    }
    else 
    {
        return FAILURE_OF_SQUARE;
    }
}

/**
    \param[in] b - Coefficient before "x". 
    \param[in] c - Constant of linear equation.
    \param[out] root1 - Pointer on first root's block of memory.
    \return Number of roots/
    \throw FAILURE_OF_LINEAR - Strange b.
 */
int Solve_linear(double b, double c, double *root1)
{
    assert(isfinite(b));
    assert(isfinite(c));
    assert(root1 != nullptr);
    if (b == 0)
    {
        return ((c == 0) ? 3 : 0);
    }
    else if (b != 0)
    {
        *root1 = -(c / b);
        return ONE_ROOT;
    }
    else 
    {
        return FAILURE_OF_LINEAR;
    }
}
