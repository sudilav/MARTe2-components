/**
 * @file OPCUAClientReadGTest.cpp
 * @brief Source file for class OPCUAClientReadGTest
 * @date Oct 28, 2019 TODO Verify the value and format of the date
 * @author lporzio TODO Verify the name and format of the author
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
 * the class OPCUAClientReadGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <limits.h>
#include "gtest/gtest.h"
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "OPCUAClientReadTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


TEST(OPCUAClientReadGTest,TestConstructor) {
    OPCUAClientReadTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(OPCUAClientReadGTest,Test_SetServiceRequest) {
    OPCUAClientReadTest test;
    ASSERT_TRUE(test.Test_SetServiceRequest());
}

TEST(OPCUAClientReadGTest,Test_GetExtensionObjectByteString) {
    OPCUAClientReadTest test;
    ASSERT_TRUE(test.Test_GetExtensionObjectByteString());
}

TEST(OPCUAClientReadGTest,Test_Read_Single) {
    OPCUAClientReadTest test;
    ASSERT_TRUE(test.Test_Read_Single());
}

TEST(OPCUAClientReadGTest,Test_Read_ExtensionObject) {
    OPCUAClientReadTest test;
    ASSERT_TRUE(test.Test_Read_ExtensionObject());
}

TEST(OPCUAClientReadGTest,Test_Read_ExtensionObject_Complex) {
    OPCUAClientReadTest test;
    ASSERT_TRUE(test.Test_Read_ExtensionObject_Complex());
}

TEST(OPCUAClientReadGTest,Test_Read_ExtensionObject_Array) {
    OPCUAClientReadTest test;
    ASSERT_TRUE(test.Test_Read_ExtensionObject_Array());
}

	