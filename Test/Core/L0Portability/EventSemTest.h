/**
 * @file EventSemTest.h
 * @brief Header file for class EventSemTest
 * @date 26/giu/2015
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

 * @details This header file contains the declaration of the class EventSemTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EVENTSEMTEST_H_
#define EVENTSEMTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "EventSem.h"
#include "MutexSem.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief A class to test the EventSem functions.
 */
class EventSemTest {
public:
    /**
     * The event semaphore object
     */
    EventSem eventSem;

    /**
     * @brief Constructor.
     * @details Creates the semaphore handle.
     */
    EventSemTest();

    /**
     * @brief Destructor.
     * @details Close the semaphore handle.
     */
    ~EventSemTest();

    /**
     * @brief Tests the EventSem::Create function.
     * @return true if the semaphore is successfully created which also implies that a non NULL handle is created.
     */
    bool TestCreate();

    /**
     * @brief Tests the EventSem::Close function.
     * @return true if the semaphore is successfully closed.
     */
    bool TestClose();

    /**
     * @brief Tests the EventSem copy constructor
     *
     * @details the EventSem has a copy constructor which allow to copy and share the handle of the same semaphore.
     * This means that the semaphore implementation is still the same but is encapsulated by two different instances
     * of the EventSem class.
     * Given this it should be possible to Lock a thread with EventSem instance and unlock with another MutexSem instance
     * which was constructed from the first and which consequently shares the same handle.
     *
     * @return true if the a copy constructed semaphore does share the same handle (i.e. the pointer address do the
     * handle is the same) and is capable of correctly acting on this handle.
     */
    bool TestCopyConstructor();

    /**
     * @brief Tests the EventSem::Wait function.
     * @details Cheks that the wait function fails after the timeout expire.
     * @param[in] timeout is the desired timeout.
     * @return true if successful, false otherwise.
     */
    bool TestWait(TimeoutType timeout);

    /**
     * @brief Tests the EventSem::Post and the EventSem::Reset function.
     * @details The first post should return true, the second should return false because the handle is still signalled.
     * After the reset the post should have success again.
     * @return true if successful, false otherwise.
     */
    bool TestPost();

    /**
     * @brief Test for an infinite timeout
     * @param[in] nOfThreads is the number of threads to launch.
     * @return True if the final value of sharedVariable == nOfThreads
     */
    bool WaitNoTimeoutTest(uint32 nOfThreads);

    /**
     * @brief Test for an finite timeout of 2 seconds
     * @param[in] nOfThreads is the number of threads to launch.
     * @return True if the final value of sharedVariable == nOfThreads
     */
    bool WaitTimeoutTestSuccess(uint32 nOfThreads);

    /**
     * @brief Test for a finite timeout of 2 seconds
     *
     * @details In this test the timeout of the semaphore will be very low with respect to the time of the test. So that it
     * is very likely that when the semaphore is posted some of the semaphores will have already timed-out and
     * changed the initial value of the sharedVariable (set to 0xABCD).
     *
     * @param[in] nOfThreads Number of threads that will change the sharedVariable value
     * @return True if when the semaphore is posted the sharedVariable is different from its initial value
     */
    bool WaitTimeoutTestFailure(uint32 nOfThreads);

    /**
     * @brief Checks that a semaphore, even after timing out still works.
     * @param[in] nOfThreads is the number of threads to launch.
     * @return Forces timeout by calling WaitTimeoutTestFailure and WaitNoTimeoutTest returns True afterwards.
     */
    bool WaitTimeoutTestFailureFollowedBySuccess(uint32 nOfThreads);

private:
    /**
     * Shared variable that will be incremented by each thread
     */
    uint32 sharedVariable;

    /**
     * Timeout for the tests with timeout
     */
    TimeoutType timeout;

    /**
     * Helper semaphore
     */
    MutexSem mutexSem;

    /**
     * @brief Implements the test logic.
     * @details The value of the sharedVariable is initialised to 0xABCD\n
     * The EventSem is reset and all the threads are created.\n
     * The threads will then wait on the event semaphore being allowed to proceed.\n
     * The Test function waits for some time before posting the semaphore. If the semaphore
     * is functioning as expected, the value of the sharedVariable should still be 0xABCD
     * before posting.
     *
     * @details The value of the sharedVariable is then set to zero and the semaphore is posted.\n
     * It is then expected that the sharedVariable is individually incremented by each thread.
     *
     * @param[in] nOfThreads the number of threads for the test
     * @return True if the final value of sharedVariable is equal to nOfThreads
     */
    bool MultiThreadedTestWait(uint32 nOfThreads);
    /**
     * Allow the callback functions to access the private methods of the class
     */
    /**
     * Callback function that is used by the thread spawned by the MultiThreadedTestWait.
     * @param[in] est the class instance under test
     */
    friend void EventSemTestWait(EventSemTest &est);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EVENTSEMTEST_H_ */

