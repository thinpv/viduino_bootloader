#ifndef __ARM32_TYPES_H__
#define __ARM32_TYPES_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

typedef signed char				s8_t;
typedef unsigned char			u8_t;

typedef signed short			s16_t;
typedef unsigned short			u16_t;

typedef signed int				s32_t;
typedef unsigned int			u32_t;

typedef signed long long		s64_t;
typedef unsigned long long		u64_t;

typedef signed char				s8;
typedef unsigned char			u8;

typedef signed short			s16;
typedef unsigned short			u16;

typedef signed int				s32;
typedef unsigned int			u32;

typedef signed long long		s64;
typedef unsigned long long		u64;

typedef signed long long		intmax_t;
typedef unsigned long long		uintmax_t;

typedef signed int				ptrdiff_t;
typedef signed int				intptr_t;
typedef unsigned int 			uintptr_t;

typedef unsigned int			size_t;
typedef signed int				ssize_t;

typedef signed long long		loff_t;

typedef signed int				bool_t;

typedef signed int				register_t;
typedef unsigned int			irq_flags_t;

typedef unsigned int			virtual_addr_t;
typedef unsigned int			virtual_size_t;
typedef unsigned int			physical_addr_t;
typedef unsigned int			physical_size_t;

#ifndef FALSE
#      define  FALSE   (0)
#endif

#ifndef TRUE
#      define  TRUE    (1)
#endif

#ifndef NULL
#      define  NULL 0
#endif

typedef struct {
	volatile long counter;
} atomic_t;

typedef struct {
	volatile long lock;
} spinlock_t;

#ifdef __cplusplus
}
#endif

#endif /* __ARM32_TYPES_H__ */
