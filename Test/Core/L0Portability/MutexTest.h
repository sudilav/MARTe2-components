/**
 * @file MutexTest.h
 * @brief Header file for class MutexTest
 * @date 25/06/2015
 * @author Giuseppe Ferr�
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This header file contains the declaration of the class MutexTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MUTEXTEST_H_
#define MUTEXTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MutexSem.h"
#include "EventSem.h"
#include "Threads.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief A class to test the MutexSem functions.
 */
class MutexTest {
public:
    /**
     * @brief Constructor.
     * @details Creates the mutex handle.
     */
    MutexTest();

    /**
     * @brief Destructor.
     * @details Closes the mutex handle.
     */
    ~MutexTest();

    /**
     * @brief Tests the MutexSem::Lock function.
     *
     * @details This test spawns nOfThreads threads which compete for a shared resource variable.
     * Each of the threads will try to increment and decrement the shared variable as many times as possible during one second.
     * Each of the threads will call MutexSem::Lock before accessing the shared resource.
     *
     * @param[in] nOfThreads is the number of threads to launch.
     * @param[in] timeout the timeout value that will be used to call the lock function.
     * @return true if the calls to MutexSem::Lock returned true and if the shared resource was protected.
     */
    bool TestLock(int32 nOfThreads,
                  TimeoutType timeout);

    /**
     * @brief Tests the MutexSem::UnLock function.
     *
     * @details This test spawns nOfThreads threads which compete for a shared resource variable.
     * Each of the threads will try to increment and decrement the shared as many times as possible during one second.
     * Each of the threads will call MutexSem::UnLock before releasing the shared resource.
     *
     * @param[in] nOfThreads is the number of threads to launch.
     * @param[in] timeout the timeout value that will be used to call the lock function (this allows to demonstrate that
     * the UnLock is also working for semaphores which were locked with a finite timeout).
     * @return true if the calls to MutexSem::UnLock returned true and if the shared resource was protected.
     */
    bool TestUnLock(int32 nOfThreads,
                    TimeoutType timeout);

    /**
     * @brief Tests the MutexSem::FastLock function.
     *
     * @details The same strategy as in @see TestLock
     *
     * @param[in] nOfThreads is the number of threads to launch.
     * @param[in] timeout the timeout value that will be used to call the lock function.
     * @return true if the calls to MutexSem::Lock returned true and if the shared resource was protected.
     */
    bool TestFastLock(int32 nOfThreads,
                      TimeoutType timeout);

    /**
     * @brief Tests the MutexSem::FastUnLock function.
     *
     * @details The same strategy as in @see TestUnLock
     *
     * @param[in] nOfThreads is the number of threads to launch.
     * @param[in] timeout the timeout value that will be used to call the lock function (this allows to demonstrate that
     * the UnLock is also working for semaphores which were locked with a finite timeout).
     * @return true if the calls to MutexSem::UnLock returned true and if the shared resource was protected.
     */
    bool TestFastUnLock(int32 nOfThreads,
                        TimeoutType timeout);

    /**
     * @brief Tests the MutexSem::FastTryLock function.
     *
     * @details This test spawns nOfThreads threads which compete for a shared resource variable.
     * Each of the threads will try to increment and decrement the shared as many times as possible during one second.
     * Each of the threads will call MutexSem::FastTryLock before accessing the shared resource. If this call returns false
     * each thread will keep on calling MutexSem::FastTryLock until it returns true.
     *
     * @return true if the function behaves as expected when protecting a shared resource and if
     * it is demonstrated that a second consecutive call to MutexSem::FastTryLock returns false.
     */
    bool TestFastTryLock(int32 nOfThreads);

    /**
     * @brief Tests the MutexSem::Lock function and checks that the error code is correctly set.
     *
     * @return true if the error code is set to Debug when the call succeeds and to Timeout when
     * there is a timeout in the Lock.
     */
    bool TestLockErrorCode();

    /**
     * @brief Tests the MutexSem::FastLock function and checks that the error code is correctly set.
     *
     * @return true if the error code is set to Debug when the call succeeds and to Timeout when
     * there is a timeout in the Lock.
     */
    bool TestFastLockErrorCode();

    /**
     * @brief Tests the MutexSem::IsRecursive function.
     *
     * @return true if when the semaphore is created with recursive = true, IsRecursive = true
     * and when the semaphore is created with recursive = false, IsRecursive = false.
     */
    bool TestIsRecursive();

    /**
     * @brief Verifies if a semaphore that is set to non-recursive dead-locks as expected.
     *
     * @details a non-recursive semaphore does not allow two consecutive locks (i.e. without unlocking before)
     * from the same thread and goes into dead-lock.
     * Conversely a recursive semaphore allows two or more consecutive locks in the context of the same thread.
     *
     * @param[in] recursive true will create a recursive semaphore, false otherwise.
     *
     * @return true when recursive=false and the testing thread goes into dead-lock or when recursive=true and the
     * testing thread does not go dead-lock
     */
    bool TestRecursive(bool recursive);

    /**
     * @brief Tests the MutexSem copy constructor
     *
     * @details the MutexSem has a copy constructor which allow to copy and share the handle of the same semaphore.
     * This means that the semaphore implementation is still the same but is encapsulated by two different instances
     * of the MutexSem class.
     * Given this it should be possible to Lock a thread with MutexSem instance and unlock with another MutexSem instance
     * which was constructed from the first and which consequently shares the same handle.
     *
     * @return true if the a copy constructed semaphore does share the same handle (i.e. the pointer address do the
     * handle is the same) and is capable of correctly acting on this handle.
     */
    bool TestCopyConstructor();

private:

    /** Semaphore which is tested by the unit testing functions */
    MutexSem testMutex;

    /** Timeout to be used for the locking test*/
    TimeoutType testMutexTimeout;

    /** Shared variable that is protected by the unit testing functions */
    int32 sharedVariable;

    /** Number of running threads for a given test */
    int32 nOfExecutingThreads;

    /** This variable is set to true when the callback functions are expected to return  */
    bool stop;

    /** To be used by the threading callback functions to report an error*/
    bool failed;

    /** Helper semaphore to synchronize the launching of concurrent threads*/
    EventSem synchSem;

    /**
     * @brief Helper function to manage the threads involved in most of the MutexSem Tests.
     *
     * @details this method creates nOfThreads threads pointing at the functionToTest callback.
     * It waits one second and then informs the functionToTest callbacks that these should return.
     * This exchange of information is perform through the class variable stop=true. When all the threads
     * return, it sets the return value to !failed (which was set by the callback functions).
     *
     * @param[in] nOfThreads is the number of threads to launch.
     * @param[in] timeout the timeout value that will be used to call the lock function.
     * @param[in] functionToTest the function callback to be called by the threads.
     * @return the value returned by the functionToTest.
     */
    bool GenericMutexTestCaller(int32 nOfThreads,
                                TimeoutType timeout,
                                ThreadFunctionType functionToTest);

    /**
     * Allow the callback functions to access the private methods of the class
     */
    /**
     * @see TestLock
     */
    friend void TestLockCallback(MutexTest &mt);
    /**
     * @see TestUnLock
     */
    friend void TestUnLockCallback(MutexTest &mt);
    /**
     * @see TestFastLock
     */
    friend void TestFastLockCallback(MutexTest &mt);
    /**
     * @see TestFastTryLock
     */
    friend void TestFastTryLockCallback(MutexTest &mt);
    /**
     * @see TestFastUnLock
     */
    friend void TestFastUnLockCallback(MutexTest &mt);
    /**
     * @see TestLockErrorCode
     */
    friend void TestLockErrorCodeCallback(MutexTest &mt);
    /**
     * @see TestFastLockErrorCode
     */
    friend void TestFastLockErrorCodeCallback(MutexTest &mt);
    /**
     * @see TestRecursive
     */
    friend void TestRecursiveCallback(MutexTest &mt);
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MUTEXTEST_H_ */

