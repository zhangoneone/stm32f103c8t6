#ifndef __CC_H__
#define __CC_H__

//typedef unsigned    char    u8_t;     //基本数据类型设置
//typedef signed      char    s8_t;
//typedef unsigned    short   u16_t;
//typedef signed      short   s16_t;
//typedef unsigned    long    u32_t;
//typedef signed      long    s32_t;
//typedef u32_t               mem_ptr_t;

#ifndef BYTE_ORDER
#define BYTE_ORDER LITTLE_ENDIAN
#endif

#if defined(__arm__) && defined(__ARMCC_VERSION)         //以下主要设置不同编译器的结构体数据的对齐，lwIP需要
    //
    // Setup PACKing macros for KEIL/RVMDK Tools
    //
    #define PACK_STRUCT_BEGIN __packed
    #define PACK_STRUCT_STRUCT 
    #define PACK_STRUCT_END
    #define PACK_STRUCT_FIELD(x) x
#elif defined (__IAR_SYSTEMS_ICC__)
    //
    // Setup PACKing macros for IAR Tools
    //
    #define PACK_STRUCT_BEGIN
    #define PACK_STRUCT_STRUCT
    #define PACK_STRUCT_END
    #define PACK_STRUCT_FIELD(x) x
    #define PACK_STRUCT_USE_INCLUDES
#else
    //
    // Setup PACKing macros for GCC Tools
    //
    #define PACK_STRUCT_BEGIN
    #define PACK_STRUCT_STRUCT __attribute__ ((__packed__))
    #define PACK_STRUCT_END
    #define PACK_STRUCT_FIELD(x) x
#endif

#ifdef DEBUG
extern void xprintf_s(const char*	fmt,...);
#define vAssertCalled(char,int) xprintf_s("Error:%s,%d\r\n",char,int)
#define configASSERT(x) if((x)==0) vAssertCalled(__FILE__,__LINE__)
extern void __error__(char *pcFilename, unsigned long ulLine);
#define LWIP_PLATFORM_ASSERT(expr)    configASSERT(expr)  
#else
#define LWIP_PLATFORM_ASSERT(expr)
#endif

#endif /* __CC_H__ */