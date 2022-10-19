#ifndef _TYPES_BASE_H
#define _TYPES_BASE_H

#ifdef __cplusplus
extern "C" {
#endif

/**************in order to compliant MDK******************/
#if defined(__CC_ARM)
#define __ASM __asm		  /*!< asm keyword for ARM Compiler        */
#define __INLINE __inline /*!< inline keyword for ARM Compiler     */

#elif defined(__ICCARM__)
#define __ASM __asm		/*!< asm keyword for IAR Compiler        */
#define __INLINE inline /*!< inline keyword for IAR Compiler. Only avaiable in High optimization mode! */

#elif defined(__GNUC__)
#define __ASM __asm		/*!< asm keyword for GNU Compiler        */
#define __INLINE inline /*!< inline keyword for GNU Compiler     */

#elif defined(__TASKING__)
#define __ASM __asm		/*!< asm keyword for TASKING Compiler    */
#define __INLINE inline /*!< inline keyword for TASKING Compiler */

#elif defined(_MSC_VER)
#define __ASM __asm		/*!< asm keyword for GNU Compiler        */
#define __INLINE inline /*!< inline keyword for GNU Compiler     */

#elif defined(__C51__)
#define __ASM __asm		/*!< asm keyword for GNU Compiler        */
#define __INLINE inline /*!< inline keyword for GNU Compiler     */

#endif

#if  defined ( __GNUC__ )
  #ifndef __weak
    #define __weak   __attribute__((weak))
  #endif /* __weak */
  #ifndef __packed
    #define __packed __attribute__((__packed__))
  #endif /* __packed */
#endif /* __GNUC__ */


/* Macro to get variable aligned on 4-bytes, for __ICCARM__ the directive "#pragma data_alignment=4" must be used instead */
#if defined   (__GNUC__)        /* GNU Compiler */
  #ifndef __ALIGN_END
    #define __ALIGN_END    __attribute__ ((aligned (4)))
  #endif /* __ALIGN_END */
  #ifndef __ALIGN_BEGIN  
    #define __ALIGN_BEGIN
  #endif /* __ALIGN_BEGIN */
#else
  #ifndef __ALIGN_END
    #define __ALIGN_END
  #endif /* __ALIGN_END */
  #ifndef __ALIGN_BEGIN      
    #if defined   (__CC_ARM)      /* ARM Compiler */
      #define __ALIGN_BEGIN    __align(4)  
    #elif defined (__ICCARM__)    /* IAR Compiler */
      #define __ALIGN_BEGIN 
    #endif /* __CC_ARM */
  #endif /* __ALIGN_BEGIN */
#endif /* __GNUC__ */

/** 
  * @brief  __NOINLINE definition
  */ 
#if defined ( __CC_ARM   ) || defined   (  __GNUC__  )
/* ARM & GNUCompiler 
   ---------------- 
*/
#define __NOINLINE __attribute__ ( (noinline) )

#elif defined ( __ICCARM__ )
/* ICCARM Compiler
   ---------------
*/
#define __NOINLINE _Pragma("optimize = no_inline")

#endif

/************************************************/

// #include <stdint.h> //uint32_t...
// #include <stdio.h> //size_t NULL
// #include <string.h>
// #include <stdbool.h> //bool
// #include <limits.h> //limit c value type range 

typedef byte uint8_t;
typedef word uint16_t;
typedef dword uint32_t;
typedef word size_t;

#ifndef TRUE //old, unrecommend to use
#define TRUE 1
#endif

#ifndef FALSE //old, unrecommend to use
#define FALSE 0
#endif

#ifndef __cplusplus

typedef byte bool;

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#endif

// #ifndef NULL
// #define NULL		((void *)0)
// #endif

//declare data storage in rom
#define IN_ROM code
// #define IN_ROM const 

#ifdef __cplusplus
}
#endif

#endif
