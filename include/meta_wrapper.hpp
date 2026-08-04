#if defined(__cplusplus)
    #if __cplusplus <= 202302L
        #include "qlibs-mp.hpp"
    #else 
        #include <meta>
        namespace mp = std;
    #endif
#else
#error "broski"
#endif

