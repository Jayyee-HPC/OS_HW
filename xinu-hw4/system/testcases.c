/**
 * @file testcases.c
 * @provides testcases
 *
 * Embedded XINU, Copyright (C) 2007, 2019.  All rights reserved.
 */

#include <xinu.h>

void print_lockent(int);

/**
 * Testcase functions.  These functions are used as workloads on other
 * cores during various testcases.
 */
void core_print(void)
{
    uint cpuid;
    cpuid = getcpuid();
    while (1){
        if(0 == cpuid){
            kprintf("Hello World from core %d\r\n", cpuid);

        }else if(1 == cpuid){
            kprintf("Core %d say good morning \r\n", cpuid);

        }else if(2 == cpuid){
            kprintf("Nice to see you from core %d\r\n", cpuid);

        }else if(3 == cpuid){
            kprintf("Have a nice core %d day \r\n", cpuid);

        }
    }
        //kprintf("Hello World from core %d\r\n", cpuid);
}

void core_acquire(spinlock_t lock)
{
    lock_acquire(lock);
    while (1)
        ;
}

/**
 * testcases - called after initialization completes to test things.
 */

void testcases(void)
{
    int c,status;
    spinlock_t testlock;

    kprintf("===TEST BEGIN===\r\n");

    c = kgetc();
    switch (c)
    {
    case '0':
        // 1 spinlock, create and acquire.
        // Tests to make sure lock field is being set.
        // Expected output is that lock field is set to SPINLOCK_LOCKED
        testlock = lock_create();

        lock_acquire(testlock);
        print_lockent(testlock);

		lock_release(testlock);
        lock_free(testlock);
        break;

    case '1':
        // 1 spinlock, acquire and release
        // Tests to make sure lock field is being set correctly.
        // Expected output is that lock field is set to SPINLOCK_UNLOCKED.
        testlock = lock_create();

        lock_acquire(testlock);
        lock_release(testlock);
        print_lockent(testlock);

        lock_free(testlock);
        break;

    case '2':
        // Invalid spinlock test case.
        // An invalid number is sent to lock_acquire.
        // This will make sure students do error checking.
        // Expected output is SYSERR
        status = lock_acquire(-1);
        kprintf("status: %s\r\n", (status == SYSERR) ? "SYSERR" : "OK");
        break;

    case '3':
        // Invalid spinlock test case.
        // An invalid number is sent to lock_release.
        // Expected output is SYSERR.
        status = lock_release(-1);
        kprintf("status: %s\r\n", (status == SYSERR) ? "SYSERR" : "OK");
        break;

    case '4':
        // Acquire lock on another core.
        // This tests that the core field is being set.
        // Expected output is that lock field is locked and core field is 1.
        // TODO: Write this testcase.
        
        testlock = lock_create();
        unparkcore(1, (void *)core_acquire, (void *)testlock);
        //lock_acquire(testlock);
        print_lockent(testlock);
        
        break;

    case '5':
        // Core competition test case.
        // Core 0 will acquire the lock, then unpark the other cores
        // which will try to also acquire the lock.
        //
        // The other cores should not be able to acquire the lock.. 
        // if they do then the lock is not working properly.
        //
        // Expected output is that the core field should be 0.

        // TODO: Write this testcase.
        testlock = lock_create();

        lock_acquire(testlock);
        unparkcore(1, (void *)core_acquire, (void *)testlock);
        print_lockent(testlock);
        lock_release(testlock);
        lock_free(testlock);
        break;
		
	case '6':
        // All cores competition test case.
        // Core 0 will create the lock, then unpark the other cores
        // which will try to acquire the lock.
  
        // Expected output is that the core field should be 1, 2, or 3.

        // TODO: Write this testcase.
        testlock = lock_create();

        
        unparkcore(1, (void *)core_acquire, (void *)testlock);
		unparkcore(2, (void *)core_acquire, (void *)testlock);
		unparkcore(3, (void *)core_acquire, (void *)testlock);
        print_lockent(testlock);
        //lock_release(testlock);
        //lock_free(testlock);
        break;	

	case '7':
        // Illegle acquire test case.
        // Unpark the other cores
        // which will try to acquire locks without lock_create.
  
        // Expected output is that the core field should be -1.

        // TODO: Write this testcase.
        
        unparkcore(1, (void *)core_acquire, (void *)2);
		unparkcore(2, (void *)core_acquire, (void *)3);
		unparkcore(3, (void *)core_acquire, (void *)4);
        print_lockent(2);
		print_lockent(3);
		print_lockent(4);
        //lock_release(testlock);
        //lock_free(testlock);
        break;	
    default:
        // Default testcase.
        // Output is non-deterministic.
        //
        // Unpark the cores and set them to print.
        // If output is coherent, then the serial lock is working.

        unparkcore(1, (void *)core_print, NULL);
        unparkcore(2, (void *)core_print, NULL);
        unparkcore(3, (void *)core_print, NULL);
    }


    kprintf("\r\n===TEST END===\r\n");
    return;
}

void print_lockent(int index)
{
    struct lockent *lentry;
    lentry = &locktab[index];

    kprintf("%d->state: %s\r\n", index,
            (lentry->state ==
             SPINLOCK_FREE) ? "SPINLOCK_FREE" : "SPINLOCK_USED");
    kprintf("%d->lock:  %s\r\n", index,
            (lentry->lock ==
             SPINLOCK_UNLOCKED) ? "SPINLOCK_UNLOCKED" :
            "SPINLOCK_LOCKED");
    kprintf("%d->core:  %d\r\n", index, lentry->core);
    kprintf("\r\n");
}