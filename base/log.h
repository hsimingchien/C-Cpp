#ifndef _C_CPP_BASE_LOG_H
#define _C_CPP_BASE_LOG_H

#define __LOG_DEBUG__
#define __LOG_NORMAL__
#define __LOG_WARNING__
#define __LOG_ERROR__



#ifdef __LOG_DEBUG__

#define LOG_DEBUG(format,...) \
printf("[\033[1;34mLOG_DEBUG\033[0m] FILE: " __FILE__ ", LINE: %d: " format "\n", __LINE__, ##__VA_ARGS__)

#else

#define LOG_DEBUG(format,...)

#endif


#ifdef __LOG_NORMAL__

#define LOG_NORMAL(format,...) \
printf("[\033[0;34mLOG_NOTICE\033[0m] FILE: " __FILE__ ", LINE: %d: " format "\n", __LINE__, ##__VA_ARGS__)

#else

#define LOG_NORMAL(format,...)

#endif


#ifdef __LOG_WARNING__

#define LOG_WARNING(format,...) \
printf("[\033[1;31mLOG_ERROR\033[0m] FILE: " __FILE__ ", LINE: %d: " format "\n", __LINE__, ##__VA_ARGS__)

#else

#define LOG_WARNING(format,...)

#endif


#ifdef __LOG_ERROR__

#define LOG_ERROR(format,...) \
printf("[\033[0;31mLOG_CRITICAL\033[0m] FILE: " __FILE__ ", LINE: %d: " format "\n", __LINE__, ##__VA_ARGS__)

#else

#define LOG_ERROR(format,...)

#endif




#endif //_C_CPP_BASE_LOG_H

