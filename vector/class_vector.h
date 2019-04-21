// "class_Vector.h": V.T.0.0
#pragma once
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <cstdlib>
#include <fstream>


namespace my_lib
{
    typedef long long VECTOR_SIZE;
    
    template <typename Type>
    const Type CANARY_VAL = {};

    const int NEW_STR = 10;
    enum errors
    {
    	NOT_POSITIVE_LENGTH = -100,
    	INCORRECT_INDEX,
    	VECTOR_EMPTY
    };

    template <typename Type>
    class Vector
    {
        private:
            Type* data_;
            VECTOR_SIZE size_;
            VECTOR_SIZE capacity_;
        public:
            Vector(void);
            explicit Vector(const VECTOR_SIZE cap);
            ~Vector(void);
            Vector(const Vector<Type> & vect);
            bool ok(void) const;
            void dump(void) const;

            /**
                \brief Get element from ind position. You can't change element. Check bounds.
                \param[in] ind - Ind to get element from it. 
                \return Element that lie on this index.
            */
            Type at(const VECTOR_SIZE ind) const;

            /**
                \brief Get element from ind position. You can change element. Check bounds.
                \param[in] ind - Ind to get or change element from it. 
                \return Link on element that lie on this index.
            */
           Type & operator[](const VECTOR_SIZE ind);

            /**
                \brief To get the first element. 
                \return Link on element that lie on the first index.
            */            
            Type & front(void);

            /**
                \brief To get the last element. 
                \return Link on element that lie on the last index.
            */            
            Type & back(void);

            /**
                \brief Check vector. Is it empty? 
                \return True if vector is empty and false if it isn't empty.
            */     
            bool empty(void) const;

            /**
                \brief Return size of vector. 
                \return Size of vector.
            */     
            VECTOR_SIZE size(void) const;

            /**
                \brief Return capacity of vector. 
                \return Capacity of vector.
            */     
            VECTOR_SIZE capacity(void) const;

            /**
                \brief Add new element in stack. Increase bounds on 1 position. 
                \param[in] elem - Elem to push in stack.
            */     
            void push_back(const Type & elem);

            /**
                \brief Pop the last elemnts from stack. Decrease bounds on one position. 
            */  
            void pop_back(void);

            /**
                \brief Clear stack.  
            */              
            void clear(void);

            /**
                \brief Swap two vectors superficially. 
                \param[in] vect - Vector to swap with it.
            */          
            void swap(Vector<Type> & vect);

            Vector & operator=(const Vector<Type> & vect);
    };


    enum Constants 
    {
        START_BOOL_LEN = 3,
        BOOL_CANARY = 66,
        ONE_BYTE = 8,
        DEFAULT_LEN = 2,
    };

    enum Errors
    {
        SUCCESS = 0,
        POP_ERROR = 1
    };

    const char DEFAULT_COFFICIENT = 2;

    // Index in this array means bit that turn on. For example, 0 - 10000000, 1 - 01000000, ... , 7 - 00000001. 
    const unsigned char MASK_BUF[ONE_BYTE] = {128, 64, 32, 16, 8, 4, 2, 1};

    class Proxy_bool
    {
        private:
            // Pointer on appropriate byte in bool array
            unsigned char* byte_ptr_;
            // Number of bit in bool array.
            char bit_num_; 
        public:
            Proxy_bool(unsigned char* byte_num, char bit_num_);

            ~Proxy_bool(void);

            Proxy_bool(const Proxy_bool & pr_bool);

            Proxy_bool & operator=(const Proxy_bool & pr_bool);

            Proxy_bool(void) = delete;
            
            Proxy_bool & operator!(void);

            operator bool(void) const;

    };



    template <>
    class Vector<bool>
    {
        private:
            unsigned char* data_;
            VECTOR_SIZE size_;
            VECTOR_SIZE capacity_;

            void realloc(void);

            void print_bool_vector(void) const;
        public:
            Vector(void);

            explicit Vector(const VECTOR_SIZE cap);

            ~Vector(void);

            Vector(const Vector<bool> & vect);

            bool ok(void) const;

            void dump(void) const;

            // Numeration of elements is similiar usual array numeration.
            bool at(const VECTOR_SIZE ind) const;

            Proxy_bool operator[](VECTOR_SIZE ind) const;

            Proxy_bool front(void) const;
          
            Proxy_bool back(void) const;
 
            bool empty(void) const;
  
            VECTOR_SIZE size(void) const;

            VECTOR_SIZE capacity(void) const;

            void push_back(const bool bit);

            /**
                \brief Pop element from the end of vector. 
                \return SUCCESS == 0 if success, else POP_ERROR == 1.
            */               
            int pop_back(void);

            void clear(void);


    };

} // End of namespace my_lib.



#include "vector_func.cpp"
#include "bool_vector.cpp"
#include "proxy_bool.cpp"

