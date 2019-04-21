

namespace my_lib {


    Proxy_bool::Proxy_bool(unsigned char* byte_ptr, char bit_num)
    	: byte_ptr_(byte_ptr),
    	  bit_num_(bit_num) {
    }


    Proxy_bool::~Proxy_bool(void) {
    }


    Proxy_bool::Proxy_bool(const Proxy_bool & pr_bool)
        : byte_ptr_(pr_bool.byte_ptr_),
          bit_num_(pr_bool.bit_num_) {
    }


    Proxy_bool & Proxy_bool::operator=(const Proxy_bool & pr_bool) {
      byte_ptr_ = pr_bool.byte_ptr_;
      bit_num_ = pr_bool.bit_num_;
      return *this;
    }


    Proxy_bool & Proxy_bool::operator!(void) {
      *byte_ptr_ ^= MASK_BUF[bit_num_];
      return *this;
    }


    Proxy_bool::operator bool(void) const {
      if ((*byte_ptr_ & MASK_BUF[bit_num_]) == MASK_BUF[bit_num_]) {
        return true;
      }
      else {
        return false;
      }
    }
} // End of namespace my_lib.
