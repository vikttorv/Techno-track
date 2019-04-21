// Exceptions for new.

namespace my_lib 
{
    #define BOOL_CHECK if(!(ok( )))\
    {\
        std::cout << "Error of " << __PRETTY_FUNCTION__ << " method:\n";\
        dump( );\
        exit(EXIT_FAILURE);\
    }


    inline VECTOR_SIZE get_length(VECTOR_SIZE cap)
    {
        return (cap % ONE_BYTE == 0) ? cap / ONE_BYTE + 2 : cap / ONE_BYTE + 3;
    }


    Vector<bool>::Vector(void):
        data_ (new unsigned char[START_BOOL_LEN]),
        capacity_ (ONE_BYTE),
        size_(0)
    {
    	data_[0] = data_[capacity_ / ONE_BYTE + 1] = BOOL_CANARY;
    	BOOL_CHECK
    }


    Vector<bool>::Vector(const VECTOR_SIZE cap):
        capacity_ (cap),
        size_ (0)
    {
    	// whole_length is length in bytes.
    	int whole_length = get_length(cap);
        data_ = new unsigned char[whole_length];
        data_[0] = data_[whole_length - 1] = BOOL_CANARY;
        for (int ind = 1; ind <= whole_length - 2; ind++)
        {
        	data_[ind] = 0;
        }
        BOOL_CHECK
    }


    Vector<bool>::~Vector(void) 
    {
        delete [] data_;
    }


    bool Vector<bool>::ok(void) const
    {
    	bool is_ok = capacity_ >= 0 && size_ >= 0 && data_ && capacity_ >= size_;
    	if (!is_ok)
    	{
    		return false;
    	}
    	int whole_length = get_length(capacity_);
    	is_ok = is_ok && data_[0] == BOOL_CANARY && data_[whole_length - 1] == BOOL_CANARY;
    	return is_ok; 
    }

 
    void Vector<bool>::print_bool_vector(void) const
    {
        std::cout << "Information of bool vector:\n";
        std::cout << "<";
        if (size_ == 0)
        {
            std::cout << ">";
        }
        for (int ind = 1; ind <= get_length(capacity_) - 2; ind++)
        {
            for (int ind1 = 0; ind1 < ONE_BYTE; ind1++)
            {          
                if ((data_[ind] & MASK_BUF[ind1]) == MASK_BUF[ind1])
                {
                    std::cout << 1; 
                }
                else
                {
                    std::cout << 0;
                }

                if (ind1 + 1 +(ind - 1) * ONE_BYTE == size_) 
                {
                    std::cout << ">";
                }
                if (ind1 + 1 + (ind - 1) * ONE_BYTE == capacity_)
                {
                    std::cout << "|";
                }                
            }
        }
        if (capacity_ == 0)
        {
            std::cout << "|";
        }   
    }


    void Vector<bool>::dump(void) const
    {
        std::cout << "capacity = " << capacity_ << std::endl;
        std::cout << "size = " << size_ << std::endl;
        std::cout << "data pointer = " << (int*)data_ << std::endl;
        if (!data_)
        {
        	return;
        }

        int whole_length = get_length(capacity_);
        std::cout << "First canary:" << data_[0] << std::endl;
        std::cout << "Second canary:" << data_[whole_length - 1] << std::endl;

        print_bool_vector( );
        std::cout << "\n\n\n";
    }


    Vector<bool>::Vector(const Vector<bool> & vect):
        capacity_(vect.capacity_),
        size_(vect.size_)
    {
    	int whole_length = get_length(capacity_);
    	data_ = new unsigned char[whole_length];
        for (int ind = 1; ind <= whole_length - 2; ind++)
        {
            data_[ind] = vect.data_[ind];
        }
        data_[0] = vect.data_[0];
        data_[whole_length - 1] = vect.data_[whole_length - 1];
        BOOL_CHECK
    }


    bool Vector<bool>::at(const VECTOR_SIZE ind) const
    {
        assert(ind < size_);
        assert(ind >= 0);
        BOOL_CHECK

    	int byte_position = (ind / ONE_BYTE) + 1;
    	int delta = ind - (byte_position - 1) * ONE_BYTE;
    	if ((data_[byte_position] & MASK_BUF[delta]) == MASK_BUF[delta])
    	{
    		return true;
    	}
        else
        {
        	return false;
        }
        BOOL_CHECK
    }


    Proxy_bool Vector<bool>::operator[](VECTOR_SIZE ind) const
    {
        BOOL_CHECK
        assert(ind < size_);
        assert(ind >= 0);

        int byte_position = (ind / ONE_BYTE) + 1;
        int delta = ind - (byte_position - 1) * ONE_BYTE;

        BOOL_CHECK
        return Proxy_bool(&data_[byte_position], delta);
    }

    Proxy_bool Vector<bool>::front(void) const
    {
        BOOL_CHECK
        return (*this)[0];
    }
    


    Proxy_bool Vector<bool>::back(void) const
    {
        BOOL_CHECK
        return (*this)[size_ - 1];
    }

    bool Vector<bool>::empty(void) const
    {
        BOOL_CHECK
        return size_ == 0;
    }
  
    VECTOR_SIZE Vector<bool>::size(void) const
    {
        return size_;
    }


    VECTOR_SIZE Vector<bool>::capacity(void) const
    {
        return capacity_;
    }

    void Vector<bool>::realloc(void)
    {
        BOOL_CHECK
        int new_length = DEFAULT_LEN + 2;
        if (capacity_ != 0)
        {
            new_length = get_length(capacity_ * DEFAULT_COFFICIENT);
        }

        int old_byte_length = get_length(capacity_) - 2;
        unsigned char* old_data = data_;
        data_ = new unsigned char[new_length];

        for (int ind = 1; ind <= old_byte_length; ind++)
        {
            data_[ind] = old_data[ind];
        }
        data_[0] = data_[new_length - 1] = BOOL_CANARY;
        if (!capacity_)
        {
            capacity_ = DEFAULT_LEN * ONE_BYTE;
        }
        else
        {
            capacity_ *= DEFAULT_COFFICIENT;
        }
        delete [] old_data;
        BOOL_CHECK    
        return;    
    }


    void Vector<bool>::push_back(const bool bit)
    {
        BOOL_CHECK

        
        // Reallocation memory.
        if (size_ == capacity_)
        {
            realloc( );
        }

        size_++;
        int byte_size = get_length(size_) - 2;
        int delta = size_ - (size_ / ONE_BYTE) * ONE_BYTE; 
        if (bit)
        {
            data_[byte_size] |= MASK_BUF[delta];
        }
        else
        {
            data_[byte_size] &= ~MASK_BUF[delta];
        }
        BOOL_CHECK
        return;
    }


    // Standart pop( ) was changed, because code of error is better than exception in this particular case.
    // No exception safety according to exception safety guarantees by David Abrahams.
    int Vector<bool>::pop_back(void)
    {
        BOOL_CHECK
        if (size_ == 0)
        {
            return POP_ERROR;
        }

        --size_;
        BOOL_CHECK
        return SUCCESS;
    }

     void Vector<bool>::clear(void)
     {
        size_ = 0;
        return;
     }
} // end of namespace my_lib
