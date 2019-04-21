// "Vector_func.cpp": V.T.0.0
#include "class_vector.h"

namespace my_lib
{
    #define CHECK if(!(Vector<Type>::ok( )))\
    {\
        std::cout << "Error of " << __PRETTY_FUNCTION__ << " method:\n";\
        Vector<Type>::dump( );\
        exit(EXIT_FAILURE);\
    }

    #define CHECK_EXTERN(vect) if(!((vect).ok( )))\
    {\
        std::cout << "Error of " << __PRETTY_FUNCTION__ << " method:\n";\
        (vect).dump( );\
        exit(EXIT_FAILURE);\
    }

    template <typename Type>
    Vector<Type>::Vector(void):
        data_ (new Type[4]),
        capacity_ (2),
        size_(0)
    {
    	data_[0] = data_[capacity_ + 1] = CANARY_VAL<Type>;
    }

    template <typename Type>
    Vector<Type>::Vector(VECTOR_SIZE size)
    {
    	if (size < 0)
    	{
    		capacity_ = 2;
    		size_ = 0;
    		errno = NOT_POSITIVE_LENGTH;
    	}
        else if (size == 0)
        {
            capacity_ = 2;
            size_ = 0;
        }
        else
        {
    	    capacity_ = size_ = size;
        }  
    	data_ = new Type[capacity_ + 2];
    	data_[0] = data_[capacity_ + 1] = CANARY_VAL<Type>;
    }

    template <typename Type>
    Vector<Type>::~Vector(void)
    {
    	delete [ ] data_;
    	data_ = nullptr;
    }
    template <typename Type>
    bool Vector<Type>::ok(void) const
    {
    	if (!data_ || size_ < 0  || capacity_ < size_)
    	{
    		return false;
    	}
        return data_[0] == CANARY_VAL<Type> && data_[capacity_ + 1] == CANARY_VAL<Type>;
    }

    template <typename Type>
    void Vector<Type>::dump(void) const
    {
        if (data_ == nullptr)
        {
            std::cout << "data_ == nullptr!!!\n";
            return; 
        }
        
        std::cout << "capacity == " << capacity_ << std::endl;
        std::cout << "size == " << size_ << std::endl;
        std::cout << "data_ == " << data_ << std::endl;
        if (size_ < 0)
        {
        	std::cout << "size < 0!!!\n";
        	return;
        }
        if (size_ > capacity_)
        {
        	std::cout << "size > capacity!!!\n";
        }
        if (capacity_ < 0)
        {
        	std::cout << "capacity < 0!!!\n";
        }

        std::cout << "Data from 0 to capacity without canaries:\n";
        int new_str = 0;
        for (VECTOR_SIZE ind = 1; ind <= capacity_; ind++)
        {   
            if (new_str == NEW_STR)
            {
                new_str = 0;
                std::cout << std::endl << ' ';
            }
            if (ind == 1)
            {
            	std::cout << "[";
            }
            if (ind == capacity_)
            {
                std::cout << data_[ind] << "]\n";
            }
            else if (ind == size_)
            {
            	std::cout << data_[ind] << "]\nEnd of real information, additional memory:\n[";
            }
            else
            {
                std::cout << data_[ind] << ", ";
            }
            new_str++;
        }
        std::cout << "data_[0] == " << data_[0] << "; expected: " << CANARY_VAL<Type> << std::endl;
        std::cout << "data_[" << capacity_ + 1 << "] == " << data_[capacity_ + 1] << "; expected: " 
                  << CANARY_VAL<Type> << std::endl;
        return;
    }

    template <typename Type>
    Vector<Type>::Vector(const Vector & vect)
    {
        CHECK_EXTERN(vect)
        size_ = vect.size_;
        capacity_ = vect.capacity_;
        data_ = new Type[capacity_ + 2];
        std::cout << "size_ = " << size_ << std::endl;
        for (int ind = 1; ind <= size_; ind++)
        {
            data_[ind] = (vect.data_)[ind];         
        }
        data_[0] = data_[capacity_ + 1] = CANARY_VAL<Type>;
        CHECK
    }

    template <typename Type>
    Type Vector<Type>::at(VECTOR_SIZE ind) const
    {
        CHECK
        assert(ind < size_ && ind >= 0);
        return data_[ind + 1];
    }

    template <typename Type>
    Type & Vector<Type>::operator[](VECTOR_SIZE ind)
    {
        CHECK
        assert(ind < size_ && ind >= 0);
        return data_[ind + 1];
    }
   
    template <typename Type>
    Type & Vector<Type>::front(void)
    {
    	CHECK
    	return data_[1];
    }

    template <typename Type>
    Type & Vector<Type>::back(void)
    {
    	CHECK
    	return data_[size_];
    }

    template <typename Type>
    void Vector<Type>::push_back(const Type & elem)
    {
    	CHECK
    	if (size_ == capacity_)
    	{
    	    Type *new_data = new Type[capacity_ * 2 + 2];
    		for (int ind = 1; ind <= size_; ind++)
    		{
    			new_data[ind] = data_[ind];
    		}
    		delete [ ] data_;
    		data_ = new_data;
    		capacity_ *= 2;
    		data_[0] = data_[capacity_ + 1] = CANARY_VAL<Type>;
    	}

        data_[++size_] = elem;
        CHECK
        return;
    }

    template <typename Type>
    void Vector<Type>::pop_back(void)
    {
        CHECK
        if (size_ == 0)
        {
        	errno = VECTOR_EMPTY;
        	return;
        }

        --size_;
        CHECK
        return;
    }

    template <typename Type>
    void Vector<Type>::clear(void)
    {
        CHECK
        delete [ ] data_;
        data_ = new Type[4];
        capacity_ = 2;
        size_ = 0;
    	data_[0] = data_[capacity_ + 1] = CANARY_VAL<Type>;        
        CHECK
        return;
    }

    template <typename Type>
    bool Vector<Type>::empty(void) const
    {
        CHECK
        return size_ == 0;
    }

    template <typename Type>
    VECTOR_SIZE Vector<Type>::size(void) const
    {
    	CHECK
    	return size_;
    }

    template <typename Type>
    VECTOR_SIZE Vector<Type>::capacity(void) const
    {
    	CHECK
    	return capacity_;
    }

    template <typename Type>
    void Vector<Type>::swap(Vector<Type> & vect)
    {
        Type* tmp_data = data_;
        data_ = vect.data_;
        vect.data_ = tmp_data;

        VECTOR_SIZE tmp_size = size_;
        size_ = vect.size_;
        vect.size_ = tmp_size;

        VECTOR_SIZE tmp_capacity = capacity_;
        capacity_ = vect.capacity_;
        vect.capacity_ = tmp_capacity;
        return;
    }

    template <typename Type>
    Vector<Type> & Vector<Type>::operator=(const Vector & vect)
    {
    	CHECK
        CHECK_EXTERN(vect)

        if (this == &vect)
        {
            return *this;
        }

        Vector<Type> copy(vect);
        swap(copy);

        CHECK
        return *this;
    }

}
