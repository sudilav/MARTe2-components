/**
 * @file SingleThreadServiceGTest.cpp
 * @brief Source file for class SingleThreadServiceGTest
 * @date 19/09/2016
 * @author Andre Neto
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

 * @details This source file contains the definition of all the methods for
 * the class MessageGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/


#include <SingleThreadServiceTest.h>
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(SingleThreadServiceGTest,TestConstructor) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestDefaultConstructor());
}

TEST(SingleThreadServiceGTest,TestDefaultConstructor_Template) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestDefaultConstructor_Template());
}

TEST(SingleThreadServiceGTest,TestInitialise) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestInitialise());
}

TEST(SingleThreadServiceGTest,TestInitialise_False) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False());
}

TEST(SingleThreadServiceGTest,TestStart) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestStart());
}

TEST(SingleThreadServiceGTest,TestStart_False) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestStart_False());
}

TEST(SingleThreadServiceGTest,TestStart_Restart) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestStart_Restart());
}

TEST(SingleThreadServiceGTest,TestStop) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestStop());
}

TEST(SingleThreadServiceGTest,TestStop_Kill) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestStop_Kill());
}


TEST(SingleThreadServiceGTest,TestSetTimeout) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestSetTimeout());
}

TEST(SingleThreadServiceGTest,TestGetTimeout) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestGetTimeout());
}

TEST(SingleThreadServiceGTest, TestGetPriorityClass) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestGetPriorityClass());
}

TEST(SingleThreadServiceGTest, TestGetPriorityLevel) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestGetPriorityLevel());
}

TEST(SingleThreadServiceGTest, TestGetCPUMask) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestGetCPUMask());
}

TEST(SingleThreadServiceGTest, TestSetPriorityClass) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestSetPriorityClass());
}

TEST(SingleThreadServiceGTest, TestSetPriorityLevel) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestSetPriorityLevel());
}

TEST(SingleThreadServiceGTest, TestSetCPUMask) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestSetCPUMask());
}

TEST(SingleThreadServiceGTest, TestSetPriorityClass_Start) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestSetPriorityClass_Start());
}

TEST(SingleThreadServiceGTest, TestSetPriorityLevel_Start) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestSetPriorityLevel_Start());
}

TEST(SingleThreadServiceGTest, TestSetCPUMask_Start) {
    SingleThreadServiceTest target;
    ASSERT_TRUE(target.TestSetCPUMask_Start());
}