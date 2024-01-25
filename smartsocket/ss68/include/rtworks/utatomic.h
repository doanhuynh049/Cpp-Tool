/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tkrnl/dev/68/src/rtworks/utatomic.h#1 $
 */

#if !defined(_RTWORKS_UTATOMIC_H_) && !defined(T_GENERATE_PROTOTYPES)
#define  _RTWORKS_UTATOMIC_H_

/*
 * What's the most bytes we can move around atomically, without any special
 * instructions? So far I have not encountered any platform where a move of
 * T_PTR_SIZE is NOT atomic (which almost has to be the case if the instruction
 * set supports ANY kind of atomic operations), so a bunch of our code relies
 * on this assumption. Some architectures can move MORE than T_PTR_SIZE,
 * however... Usually these are the ones where we're using a smaller pointer
 * than the hardware supports (except that Power/PARISC are funny; see note
 * below).
 */
#if defined(T_ASM_IA32X) || defined(T_ASM_IA64) || defined(T_ASM_SPARC) || defined(T_ASM_ALPHA) || defined(T_ASM_MIPS)
# define T_MOVE_ATOMIC 8
#endif

#if defined(T_TANDEM_OSS)
/*
 * I'm not sure what instruction set, or even what hardware, is used by
 * tandem_oss, so for now it has no ASM macro, but it does do 64-bit moves
 * atomically.
 */
# define T_MOVE_ATOMIC 8
#endif

/*
 * Power/PARISC are funny, at least on the compilers I tested... a 64-bit move
 * is atomic if we compile in 64-bit mode, but not in 32-bit mode. IA-32X is
 * always 64-bit safe, but it is doubtful we will distinguish between i86
 * packages running on pure IA32 and i86 packages running on IA-32X, so
 * we'll assume IA32 is only safe for 32-bits, unless compiling 64-bit which
 * requires IA-32X hardware. Anyway, we'll use sizeof(T_PTR) as the size we can
 * move atomically, since this is 8 in 64-bit mode and 4 otherwise, which is
 * what we want. - MW 2005-09-27
 */
#if defined(T_ASM_IA32) || defined(T_ASM_POWER) || defined(T_ASM_PARISC)
# define T_MOVE_ATOMIC T_PTR_SIZE
#endif

#ifndef T_MOVE_ATOMIC
/* Define it as something on unrecognized platforms */
# define T_MOVE_ATOMIC 0
#endif

/*
 * Define memory barriers for particular platforms. These are slow enough to
 * begin with, so if we can inline them and save a function call, so much the
 * better.
 * NOTE: For other platforms, we leave the function call, because it reduces
 * the risk of compiler re-ordering (we had a really nasty bug caused by gcc
 * reordering instructions!). - MW 2005-10-13
 */
#if defined(T_I86_LINUX)
# ifdef T_ASM_IA32_X64
#  define T_MSYNC() asm volatile("lock orq $0,(%%rsp)"::)
# else
#  define T_MSYNC() asm volatile("lock orl $0,(%%esp)"::)
# endif
#endif


#if 0 /* defined(T_ASM_IA32) */
/* TODO */

# define T_ASM_ATOMIC
# define T_PURE_ATOMICS

# if !defined(T_MSYNC)
#  define T_MSYNC() TutAsmAtomicMemCommit()
# endif
# define T_ATOMIC_GET_RM(c,m) TutAsmAtomicRead(c)
# define T_ATOMIC_GET_FM(c,m) TutAsmAtomicRead(c)
# define T_ATOMIC_SET_RM(c,v,m) TutAsmAtomicWrite(c,v)
# define T_ATOMIC_SET_FM(c,v,m) TutAsmAtomicWrite(c,v)
# define T_ATOMIC_SWAP_RM(c,v,m) TutAsmAtomicSwap(c,v)
# define T_ATOMIC_SWAP_FM(c,v,m) TutAsmAtomicSwap(c,v)
# define T_ATOMIC_COND_SWAP_RM(c,v,t,m) TutAsmAtomicCondSwap(c,v,t)
# define T_ATOMIC_COND_SWAP_FM(c,v,t,m) TutAsmAtomicCondSwap(c,v,t)
# define T_ATOMIC_ADD_RM(c,v,m) TutAsmAtomicAdd(c,v)
# define T_ATOMIC_ADD_FM(c,v,m) TutAsmAtomicAdd(c,v)
# define T_ATOMIC_INC_RM(c,m) TutAsmAtomicInc(c)
# define T_ATOMIC_INC_FM(c,m) TutAsmAtomicInc(c)
# define T_ATOMIC_DEC_TEST_RM(c,m) TutAsmAtomicDecAndTest(c)
# define T_ATOMIC_DEC_TEST_FM(c,m) TutAsmAtomicDecAndTest(c)

#else

# if defined(T_OS_WINDOWS)
/*
 * Windows has KeMemoryBarrier function, but it doesn't seem to be accessible,
 * so we use an implicit one from an Interlocked function - this isn't entirely
 * different from what we do on i86 anyway, since the [msl]fence instructions
 * aren't necessarily supported.
 * NOTE: If we have an assembly version, we prefer to use that, since it may be
 * leaner. This is down here because - even though it is inlined - it  does not
 * save us the function call.
 */
#  define T_MSYNC() { long foo; InterlockedExchange(&foo, 0); }
# endif

# if defined(T_I86_LINUX) || defined(T_OS_WINDOWS)
/* i86_linux and Windows have pure atomics w/o assembly atomics */
#  define T_PURE_ATOMICS
# endif

# if !defined(T_MSYNC)
#  define T_MSYNC() TutAtomicMemCommit()
#  define T_IMPURE_MSYNC
# endif

# define T_ATOMIC_GET_RM(c,m) TutAtomicRead(c,m)
# define T_ATOMIC_GET_FM(c,m) TutAtomicReadFast(c,m)
# define T_ATOMIC_SET_RM(c,v,m) TutAtomicWrite(c,v,m)
# define T_ATOMIC_SET_FM(c,v,m) TutAtomicWriteFast(c,v,m)
# define T_ATOMIC_SWAP_RM(c,v,m) TutAtomicSwap(c,v,m)
# define T_ATOMIC_SWAP_FM(c,v,m) TutAtomicSwapFast(c,v,m)
# define T_ATOMIC_COND_SWAP_RM(c,v,t,m) TutAtomicCondSwap(c,v,t,m)
# define T_ATOMIC_COND_SWAP_FM(c,v,t,m) TutAtomicCondSwapFast(c,v,t,m)
# define T_ATOMIC_ADD_RM(c,v,m) TutAtomicAdd(c,v,m)
# define T_ATOMIC_ADD_FM(c,v,m) TutAtomicAddFast(c,v,m)
# define T_ATOMIC_INC_RM(c,m) TutAtomicIncr(c,m)
# define T_ATOMIC_INC_FM(c,m) TutAtomicIncrFast(c,m)
# define T_ATOMIC_DEC_TEST_RM(c,m) TutAtomicDecrAndTest(c,m)
# define T_ATOMIC_DEC_TEST_FM(c,m) TutAtomicDecrAndTestFast(c,m)

#endif

/*
 * There is a penalty for read-write atomic instructions, so if a move
 * instruction is in fact atomic, it is faster than a special "atomic"
 * instruction. However, we can only use moves if we also have pure atomics,
 * otherwise we don't protect againt a get/set while in a lock-based atomic.
 * So, if we have atomic moves and pure atomics, then re-alias get/set as
 * moves.
 * NOTE: This does not apply to reads, which can be done even without pure
 * atomics. The reason is because atomic moves mean that the update is still
 * atomic; it's just the calculation-plus-update that (potentially) is not. In
 * other words, if we do a read while doing an atomic that modifies the value,
 * we will merely get the old value rather than waiting for the new one, which
 * is OK. This is important because it means reads can be non-blocking, even if
 * writes are not.
 */
#if T_MOVE_ATOMIC >= T_PTR_SIZE /* Should ALWAYS be true */
# undef T_ATOMIC_GET_RM
# undef T_ATOMIC_GET_FM
# define T_ATOMIC_GET_RM(c,m) (*(c))
# define T_ATOMIC_GET_FM(c,m) (*(c))

# if defined(T_PURE_ATOMICS)
#  undef T_ATOMIC_SET_RM
#  undef T_ATOMIC_SET_FM
#  define T_ATOMIC_SET_RM(c,v,m) ((*(c))=(v))
#  define T_ATOMIC_SET_FM(c,v,m) ((*(c))=(v))
# endif

/*
 * Customer builds are OK if T_MOVE_ATOMIC is 0; this just means that the
 * T_ATOMIC_GET_{RM|FM} macro calls to TutAtomicRead[Fast] won't inline.
 * MW 2006-06-13
 */
#elif defined(T_RTWORKS_SRC)
/*
 * As per earlier comments, there are several places (atomics, fast mutexes,
 * possibly other places) where we assume that moves are atomic and code
 * accordingly. If this is not the case, throw an error so that we know to fix
 * stuff. - MW 2006-01-31
 */
# error Non-atomic read! You need to fix things!
#endif

/* Aliases for SWAP */
#define T_ATOMIC_SET_OLD_RM(c,v,m) T_ATOMIC_SWAP_RM(c,v,m)
#define T_ATOMIC_SET_OLD_FM(c,v,m) T_ATOMIC_SWAP_FM(c,v,m)
/* Wrapper for SUB */
#define T_ATOMIC_SUB_RM(c,v,m) T_ATOMIC_ADD_RM(c,-(v),m)
#define T_ATOMIC_SUB_FM(c,v,m) T_ATOMIC_ADD_FM(c,-(v),m)


/*
 * Wrapper macros for some variants of Add/Sub/Inc/Dec that return the old/new
 * values. Implementing these in assembly would be equivalent to what these
 * wrapper macros do, so we're adding convenience without losing anything to
 * speak of. Plus, these all use TutAtomicAdd internally, so we don't need to
 * add any actual functions.
 */
#define T_ATOMIC_ADD_OLD_RM(c,v,m) T_ATOMIC_ADD_RM(c,v,m)
#define T_ATOMIC_ADD_OLD_FM(c,v,m) T_ATOMIC_ADD_FM(c,v,m)
#define T_ATOMIC_SUB_OLD_RM(c,v,m) T_ATOMIC_SUB_RM(c,v,m)
#define T_ATOMIC_SUB_OLD_FM(c,v,m) T_ATOMIC_SUB_FM(c,v,m)
#define T_ATOMIC_INC_OLD_RM(c,m) T_ATOMIC_ADD_RM(c,1,m)
#define T_ATOMIC_INC_OLD_FM(c,m) T_ATOMIC_ADD_FM(c,1,m)
#define T_ATOMIC_DEC_OLD_RM(c,m) T_ATOMIC_SUB_RM(c,1,m)
#define T_ATOMIC_DEC_OLD_FM(c,m) T_ATOMIC_SUB_FM(c,1,m)

#define T_ATOMIC_ADD_NEW_RM(c,v,m) (T_ATOMIC_ADD_RM(c,v,m) + (v))
#define T_ATOMIC_ADD_NEW_FM(c,v,m) (T_ATOMIC_ADD_FM(c,v,m) + (v))
#define T_ATOMIC_SUB_NEW_RM(c,v,m) (T_ATOMIC_ADD_RM(c,-(v),m) - (v))
#define T_ATOMIC_SUB_NEW_FM(c,v,m) (T_ATOMIC_ADD_FM(c,-(v),m) - (v))
#define T_ATOMIC_INC_NEW_RM(c,m) T_ATOMIC_ADD_RM(c,1,m) + 1)
#define T_ATOMIC_INC_NEW_FM(c,m) T_ATOMIC_ADD_FM(c,1,m) + 1)
#define T_ATOMIC_DEC_NEW_RM(c,m) T_ATOMIC_SUB_RM(c,1,m) - 1)
#define T_ATOMIC_DEC_NEW_FM(c,m) T_ATOMIC_SUB_FM(c,1,m) - 1)

#endif /* !defined(_RTWORKS_UTATOMIC_H_) && !defined(T_GENERATE_PROTOTYPES) */
