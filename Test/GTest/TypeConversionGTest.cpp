/**
 * @file TypeConversionGTest.cpp
 * @brief Source file for class TypeConversionGTest
 * @date 12/11/2015
 * @author Giuseppe Ferrò
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
 * the class TypeConversionGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "gtest/gtest.h"
#include "TypeConversionTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(TypeConversionGTest,TestCCStringToInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,int8> table[]={
            {"123",123, true},
            {"123s",123, true},
            {"a123s5",0, true},
            {"0xFF",-1, true},
            {"0xFF1",-1, true},
            {"0xABCD",0xab, true},
            {"0o12",10, true},
            {"0x7F",127, true},
            {"0x7F1",127, true},
            {"0x80",-128, true},
            {"0o377",-1, true},
            {"0o577",47, true},
            {"0b11111111",-1, true},
            {"0b111111111",-1, true},
            {"0b01111111",127, true},
            {"0b011111111",127, true},
            {"127",127, true},
            {"128",12, true},
            {"-128",-128, true},
            {"-129",-12, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(TypeConversionGTest,TestCCStringToUInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,uint8> table[]={
            {"123",123, true},
            {"123s1",123, true},
            {"a123s5",0, true},
            {"0xFF",255, true},
            {"0xFF1",255, true},
            {"0xABCD",0xab, true},
            {"0o12",10, true},
            {"0x7F",127, true},
            {"0x80",128, true},
            {"0o377",255, true},
            {"0o577",47, true},
            {"0b11111111",255, true},
            {"0b111111111",255, true},
            {"0b01111111",127, true},
            {"0b011111111",127, true},
            {"255",255, true},
            {"256",25, true},
            {"-1",0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestCCStringToInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,int16> table[]={
            {"123",123, true},
            {"123s1",123, true},
            {"a123s5",0, true},
            {"0xFFFF",-1, true},
            {"0xFFFF1",-1, true},
            {"0x7FFF",32767, true},
            {"0o177777",-1, true},
            {"0o77777",32767, true},
            {"0o277777",12287, true},
            {"0b1111111111111111",-1, true},
            {"0b11111111111111111",-1, true},
            {"0b0111111111111111",32767, true},
            {"0b01111111111111111",32767, true},
            {"32767",32767, true},
            {"32768",3276, true},
            {"-32768",-32768, true},
            {"-32769",-3276, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(TypeConversionGTest,TestCCStringToUInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,uint16> table[]={
            {"123",123, true},
            {"123s1",123, true},
            {"a123s5",0, true},
            {"0xFFFF",65535, true},
            {"0xFFFF1",65535, true},
            {"0x7FFF",32767, true},
            {"0o177777",65535, true},
            {"0o77777",32767, true},
            {"0o277777",12287, true},
            {"0b1111111111111111",65535, true},
            {"0b11111111111111111",65535, true},
            {"32767",32767, true},
            {"65536",6553, true},
            {"-1",0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestCCStringToInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,int32> table[]={
            {"123",123, true},
            {"123s1",123, true},
            {"a123s5",0, true},
            {"0xFFFFFFFF",-1, true},
            {"0xFFFFFFFF1",-1, true},
            {"0x7FFFFFFF",2147483647, true},
            {"0o37777777777",-1, true},
            {"0o17777777777",2147483647, true},
            {"0o47777777777",671088639, true},
            {"2147483647",2147483647, true},
            {"2147483648",214748364, true},
            {"0b11111111111111111111111111111111",-1, true},
            {"0b111111111111111111111111111111111",-1, true},
            {"0b01111111111111111111111111111111",2147483647, true},
            {"0b011111111111111111111111111111111",2147483647, true},
            {"-2147483648",-2147483648, true},
            {"-2147483649",-214748364, true},
            {0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestCCStringToUInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,uint32> table[]={
            {"123",123, true},
            {"123s1",123, true},
            {"a123s5",0, true},
            {"0xFFFFFFFF",4294967295, true},
            {"0xFFFFFFFF1",4294967295, true},
            {"0x7FFFFFFF",2147483647, true},
            {"0o37777777777",4294967295, true},
            {"0o17777777777",2147483647, true},
            {"0o47777777777",671088639, true},
            {"0b11111111111111111111111111111111",4294967295, true},
            {"0b111111111111111111111111111111111",4294967295, true},
            {"4294967295",4294967295, true},
            {"4294967296",429496729, true},
            {"-1",0, true},
            {0,0, 0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestCCStringToInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,int64> table[]={
            {"123",123, true},
            {"123s1",123, true},
            {"a123s5",0, true},
            {"0xFFFFFFFFFFFFFFFF",-1, true},
            {"0xFFFFFFFFFFFFFFFF1",-1, true},
            {"0x7FFFFFFFFFFFFFFF",9223372036854775807, true},
            {"0o1777777777777777777777",-1, true},
            {"0o777777777777777777777",9223372036854775807, true},
            {"0o2777777777777777777777",3458764513820540927, true},
            {"0b1111111111111111111111111111111111111111111111111111111111111111",-1, true},
            {"0b11111111111111111111111111111111111111111111111111111111111111111",-1, true},
            {"0b0111111111111111111111111111111111111111111111111111111111111111",9223372036854775807, true},
            {"0b01111111111111111111111111111111111111111111111111111111111111111",9223372036854775807, true},
            {"9223372036854775807",9223372036854775807, true},
            {"9223372036854775808",922337203685477580, true},
            {"-9223372036854775808",-9223372036854775808, true},
            {"-9223372036854775809",-922337203685477580, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestCCStringToUInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,uint64> table[]={
            {"123",123, true},
            {"123s1",123, true},
            {"a123s5",0, true},
            {"0xFFFFFFFFFFFFFFFF",18446744073709551615, true},
            {"0xFFFFFFFFFFFFFFFF1",18446744073709551615, true},
            {"0x7FFFFFFFFFFFFFFF",9223372036854775807, true},
            {"0o1777777777777777777777",18446744073709551615, true},
            {"0o777777777777777777777",9223372036854775807, true},
            {"0o2777777777777777777777",3458764513820540927, true},
            {"0b1111111111111111111111111111111111111111111111111111111111111111",18446744073709551615, true},
            {"0b11111111111111111111111111111111111111111111111111111111111111111",18446744073709551615, true},
            {"0b0111111111111111111111111111111111111111111111111111111111111111",9223372036854775807, true},
            {"0b01111111111111111111111111111111111111111111111111111111111111111",9223372036854775807, true},
            {"18446744073709551615",18446744073709551615, true},
            {"18446744073709551616",1844674407370955161, true},
            {"-1",0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestCCStringToFloat32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,float32> table[]={
            {"123.5",123.5, true},
            {"123.12345",123.12345, true},
            {"1E+2",100, true},
            {"10E+2",1000, true},
            {"12345.678E2",1234567.8, true},
            {"1E-2",0.01, true},
            {"0.1E-2",0.001, true},
            {"16E-2",0.16, true},
            // see the precision is not the same!
            {"12345.678E-2",123.456772, true},
            {"1E+20",1E+20, true},
            // this is overflow!
            {"123E+42",123, true},
            {"1E-25",1E-25, true},
            {"0.000000001",1E-9, true},
            //Overflow!
     //       {"340282346638528859811704183484516999999",340282346638528859811704183484516925440},
     //       {"340282346638528859811704183484516925440",340282346638528859811704183484516925440},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestCCStringToFloat64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,float64> table[]={
            {"123.5",123.5, true},
            {"123.12345",123.12345, true},
            {"1E+2",100, true},
            {"10E+2",1000, true},
            {"12345.678E2",1234567.8, true},
            {"1E-2",0.01, true},
            {"0.1E-2",0.001, true},
            {"16E-2",0.16, true},
            // see the precision now is the same!
            {"12345.678E-2",123.45678, true},
            {"1E+20",1E+20, true},
            {"123E+63",123E+63, true},
            {"123E+83",123E+83, true},
            // this is overflow!
            {"123E+510",123, true},
            {"1E-32",1E-32, true},
            {"0.000000001",1E-9, true},
            //Overflow!
     //       {"179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368",179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368},
     //       {"179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368",179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}



TEST(TypeConversionGTest,TestCCStringToString) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,String> table[]={
            {"Hello","Hello", true},
            {"123.12345","123.12345", true},
            {"","", true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(TypeConversionGTest,TestStringToString) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<String,String> table[]={
            {"Hello","Hello", true},
            {"123.12345","123.12345", true},
            {"","", true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestSStringToInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<String,int8> table[]={
            {"123",123, true},
            {"123s",123, true},
            {"a123s5",0, true},
            {"0xFF",-1, true},
            {"0xFF1",-1, true},
            {"0xABCD",0xab, true},
            {"0o12",10, true},
            {"0x7F",127, true},
            {"0x7F1",127, true},
            {"0x80",-128, true},
            {"0o377",-1, true},
            {"0o577",47, true},
            {"0b11111111",-1, true},
            {"0b111111111",-1, true},
            {"0b01111111",127, true},
            {"0b011111111",127, true},
            {"127",127, true},
            {"128",12, true},
            {"-128",-128, true},
            {"-129",-12, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(TypeConversionGTest,TestSStringToUInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<String,uint8> table[]={
            {"123",123, true},
            {"123s1",123, true},
            {"a123s5",0, true},
            {"0xFF",255, true},
            {"0xFF1",255, true},
            {"0xABCD",0xab, true},
            {"0o12",10, true},
            {"0x7F",127, true},
            {"0x80",128, true},
            {"0o377",255, true},
            {"0o577",47, true},
            {"0b11111111",255, true},
            {"0b111111111",255, true},
            {"0b01111111",127, true},
            {"0b011111111",127, true},
            {"255",255, true},
            {"256",25, true},
            {"-1",0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestSStringToInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<String,int16> table[]={
            {"123",123, true},
            {"123s1",123, true},
            {"a123s5",0, true},
            {"0xFFFF",-1, true},
            {"0xFFFF1",-1, true},
            {"0x7FFF",32767, true},
            {"0o177777",-1, true},
            {"0o77777",32767, true},
            {"0o277777",12287, true},
            {"0b1111111111111111",-1, true},
            {"0b11111111111111111",-1, true},
            {"0b0111111111111111",32767, true},
            {"0b01111111111111111",32767, true},
            {"32767",32767, true},
            {"32768",3276, true},
            {"-32768",-32768, true},
            {"-32769",-3276, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(TypeConversionGTest,TestSStringToUInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<String,uint16> table[]={
            {"123",123, true},
            {"123s1",123, true},
            {"a123s5",0, true},
            {"0xFFFF",65535, true},
            {"0xFFFF1",65535, true},
            {"0x7FFF",32767, true},
            {"0o177777",65535, true},
            {"0o77777",32767, true},
            {"0o277777",12287, true},
            {"0b1111111111111111",65535, true},
            {"0b11111111111111111",65535, true},
            {"32767",32767, true},
            {"65536",6553, true},
            {"-1",0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestSStringToInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<String,int32> table[]={
            {"123",123, true},
            {"123s1",123, true},
            {"a123s5",0, true},
            {"0xFFFFFFFF",-1, true},
            {"0xFFFFFFFF1",-1, true},
            {"0x7FFFFFFF",2147483647, true},
            {"0o37777777777",-1, true},
            {"0o17777777777",2147483647, true},
            {"0o47777777777",671088639, true},
            {"2147483647",2147483647, true},
            {"2147483648",214748364, true},
            {"0b11111111111111111111111111111111",-1, true},
            {"0b111111111111111111111111111111111",-1, true},
            {"0b01111111111111111111111111111111",2147483647, true},
            {"0b011111111111111111111111111111111",2147483647, true},
            {"-2147483648",-2147483648, true},
            {"-2147483649",-214748364, true},
            {0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestSStringToUInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<String,uint32> table[]={
            {"123",123, true},
            {"123s1",123, true},
            {"a123s5",0, true},
            {"0xFFFFFFFF",4294967295, true},
            {"0xFFFFFFFF1",4294967295, true},
            {"0x7FFFFFFF",2147483647, true},
            {"0o37777777777",4294967295, true},
            {"0o17777777777",2147483647, true},
            {"0o47777777777",671088639, true},
            {"0b11111111111111111111111111111111",4294967295, true},
            {"0b111111111111111111111111111111111",4294967295, true},
            {"4294967295",4294967295, true},
            {"4294967296",429496729, true},
            {"-1",0, true},
            {0,0, 0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestSStringToInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<String,int64> table[]={
            {"123",123, true},
            {"123s1",123, true},
            {"a123s5",0, true},
            {"0xFFFFFFFFFFFFFFFF",-1, true},
            {"0xFFFFFFFFFFFFFFFF1",-1, true},
            {"0x7FFFFFFFFFFFFFFF",9223372036854775807, true},
            {"0o1777777777777777777777",-1, true},
            {"0o777777777777777777777",9223372036854775807, true},
            {"0o2777777777777777777777",3458764513820540927, true},
            {"0b1111111111111111111111111111111111111111111111111111111111111111",-1, true},
            {"0b11111111111111111111111111111111111111111111111111111111111111111",-1, true},
            {"0b0111111111111111111111111111111111111111111111111111111111111111",9223372036854775807, true},
            {"0b01111111111111111111111111111111111111111111111111111111111111111",9223372036854775807, true},
            {"9223372036854775807",9223372036854775807, true},
            {"9223372036854775808",922337203685477580, true},
            {"-9223372036854775808",-9223372036854775808, true},
            {"-9223372036854775809",-922337203685477580, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestSStringToUInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<String,uint64> table[]={
            {"123",123, true},
            {"123s1",123, true},
            {"a123s5",0, true},
            {"0xFFFFFFFFFFFFFFFF",18446744073709551615, true},
            {"0xFFFFFFFFFFFFFFFF1",18446744073709551615, true},
            {"0x7FFFFFFFFFFFFFFF",9223372036854775807, true},
            {"0o1777777777777777777777",18446744073709551615, true},
            {"0o777777777777777777777",9223372036854775807, true},
            {"0o2777777777777777777777",3458764513820540927, true},
            {"0b1111111111111111111111111111111111111111111111111111111111111111",18446744073709551615, true},
            {"0b11111111111111111111111111111111111111111111111111111111111111111",18446744073709551615, true},
            {"0b0111111111111111111111111111111111111111111111111111111111111111",9223372036854775807, true},
            {"0b01111111111111111111111111111111111111111111111111111111111111111",9223372036854775807, true},
            {"18446744073709551615",18446744073709551615, true},
            {"18446744073709551616",1844674407370955161, true},
            {"-1",0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestSStringToFloat32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<String,float32> table[]={
            {"123.5",123.5, true},
            {"123.12345",123.12345, true},
            {"1E+2",100, true},
            {"10E+2",1000, true},
            {"12345.678E2",1234567.8, true},
            {"1E-2",0.01, true},
            {"0.1E-2",0.001, true},
            {"16E-2",0.16, true},
            // see the precision is not the same!
            {"12345.678E-2",123.456772, true},
            {"1E+20",1E+20, true},
            // this is overflow!
            {"123E+42",123, true},
            {"1E-25",1E-25, true},
            {"0.000000001",1E-9, true},
            //Overflow!
     //       {"340282346638528859811704183484516999999",340282346638528859811704183484516925440},
     //       {"340282346638528859811704183484516925440",340282346638528859811704183484516925440},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestSStringToFloat64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<String,float64> table[]={
            {"123.5",123.5, true},
            {"123.12345",123.12345, true},
            {"1E+2",100, true},
            {"10E+2",1000, true},
            {"12345.678E2",1234567.8, true},
            {"1E-2",0.01, true},
            {"0.1E-2",0.001, true},
            {"16E-2",0.16, true},
            // see the precision now is the same!
            {"12345.678E-2",123.45678, true},
            {"1E+20",1E+20, true},
            {"123E+63",123E+63, true},
            {"123E+83",123E+83, true},
            // this is overflow!
            {"123E+510",123, true},
            {"1E-32",1E-32, true},
            {"0.000000001",1E-9, true},
            //Overflow!
     //       {"179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368",179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368},
     //       {"179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368",179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

///////////////////////////////////////////////////////////////////////////////////////////////

TEST(TypeConversionGTest,TestInt8ToInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int8,int8> table[]={
            {1,1, true},
            {-1,-1,true},
            {127,127,true},
            {-128, -128, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}



TEST(TypeConversionGTest,TestInt8ToUInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int8,uint8> table[]={
            {1,1, true},
            {-1,0,true},
            {127,127,true},
            {-128, 0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestInt8ToInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int8,int16> table[]={
            {1,1, true},
            {-1,-1,true},
            {127,127,true},
            {-128, -128, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestInt8ToUInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int8,uint16> table[]={
            {1,1, true},
            {-1,0,true},
            {127,127,true},
            {-128, 0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(TypeConversionGTest,TestInt8ToInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int8,int32> table[]={
            {1,1, true},
            {-1,-1,true},
            {127,127,true},
            {-128, -128, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestInt8ToUInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int8,uint32> table[]={
            {1,1, true},
            {-1,0,true},
            {127,127,true},
            {-128, 0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestInt8ToInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int8,int64> table[]={
            {1,1, true},
            {-1,-1,true},
            {127,127,true},
            {-128, -128, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestInt8ToUInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int8,uint64> table[]={
            {1,1, true},
            {-1,0,true},
            {127,127,true},
            {-128, 0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

//////////////////////////////////////////////////////////////////////////////////////////




TEST(TypeConversionGTest,TestUInt8ToInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint8,int8> table[]={
            {1,1, true},
            {128,127,true},
            {255,127,true},
            {127,127,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}



TEST(TypeConversionGTest,TestUInt8ToUInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint8,uint8> table[]={
            {1,1, true},
            {255,255,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestUInt8ToInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint8,int16> table[]={
            {1,1, true},
            {255,255,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestUInt8ToUInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint8,uint16> table[]={
            {1,1, true},
            {255,255,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(TypeConversionGTest,TestUInt8ToInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint8,int32> table[]={
            {1,1, true},
            {255,255,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestUInt8ToUInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint8,uint32> table[]={
            {1,1, true},
            {255,255,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestUInt8ToInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint8,int64> table[]={
            {1,1, true},
            {255,255,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestUInt8ToUInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint8,uint64> table[]={
            {1,1, true},
            {255,255,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

//////////////////////////////////////////////////////////////



TEST(TypeConversionGTest,TestInt16ToInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int16,int8> table[]={
            {1,1, true},
            {-1,-1,true},
            {127,127,true},
            {128, 127, true},
            {-128, -128, true},
            {-129, -128, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}



TEST(TypeConversionGTest,TestInt16ToUInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int16,uint8> table[]={
            {1,1, true},
            {-1,0,true},
            {255,255,true},
            {256, 255, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestInt16ToInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int16,int16> table[]={
            {1,1, true},
            {-1,-1,true},
            {32767,32767,true},
            {-32768, -32768, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestInt16ToUInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int16,uint16> table[]={
            {1,1, true},
            {32767,32767,true},
            {-1, 0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(TypeConversionGTest,TestInt16ToInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int16,int32> table[]={
            {1,1, true},
            {-1,-1,true},
            {32767,32767,true},
            {-32768, -32768, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestInt16ToUInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int16,uint32> table[]={
            {1,1, true},
            {32767,32767,true},
            {-1, 0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestInt16ToInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int16,int64> table[]={
            {1,1, true},
            {-1,-1,true},
            {32767,32767,true},
            {-32768, -32768, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestInt16ToUInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int16,uint64> table[]={
            {1,1, true},
            {32767,32767,true},
            {-1, 0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

//////////////////////////////////////////////////////////////////////////////


TEST(TypeConversionGTest,TestUInt16ToInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint16,int8> table[]={
            {1,1, true},
            {127,127,true},
            {128,127,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}



TEST(TypeConversionGTest,TestUInt16ToUInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint16,uint8> table[]={
            {1,1, true},
            {256,255,true},
            {255,255,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestUInt16ToInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint16,int16> table[]={
            {1,1, true},
            {32768,32767,true},
            {32767, 32767, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestUInt16ToUInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint16,uint16> table[]={
            {1,1, true},
            {65535,65535,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(TypeConversionGTest,TestUInt16ToInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint16,int32> table[]={
            {1,1, true},
            {65535,65535,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestUInt16ToUInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint16,uint32> table[]={
            {1,1, true},
            {65535,65535,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestUInt16ToInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint16,int64> table[]={
            {1,1, true},
            {65535,65535,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestUInt16ToUInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint16,uint64> table[]={
            {1,1, true},
            {65535,65535,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(TypeConversionGTest,TestInt32ToInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int32,int8> table[]={
            {1,1, true},
            {-1,-1,true},
            {-128, -128, true},
            {127,127,true},
            {128,127,true},
            {-129,-128, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}



TEST(TypeConversionGTest,TestInt32ToUInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int32,uint8> table[]={
            {1,1, true},
            {-1,0,true},
            {255,255,true},
            {256, 255, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestInt32ToInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int32,int16> table[]={
            {1,1, true},
            {-1,-1,true},
            {32767,32767,true},
            {-32768, -32768, true},
            {32768,32767,true},
            {-32769, -32768, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestInt32ToUInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int32,uint16> table[]={
            {1,1, true},
            {65535,65535,true},
            {65536,65535,true},
            {-1, 0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(TypeConversionGTest,TestInt32ToInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int32,int32> table[]={
            {1,1, true},
            {-1,-1,true},
            {2147483647,2147483647,true},
            {-2147483648, -2147483648, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestInt32ToUInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int32,uint32> table[]={
            {1,1, true},
            {2147483647,2147483647,true},
            {-1, 0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestInt32ToInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int32,int64> table[]={
            {1,1, true},
            {-1,-1,true},
            {2147483647,2147483647,true},
            {-2147483648, -2147483648, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestInt32ToUInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int32,uint64> table[]={
            {1,1, true},
            {2147483647,2147483647,true},
            {-1, 0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


///////////////////////////////////////////////////////////////////////////////////////


TEST(TypeConversionGTest,TestUInt32ToInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint32,int8> table[]={
            {1,1, true},
            {127,127,true},
            {128,127,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}



TEST(TypeConversionGTest,TestUInt32ToUInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint32,uint8> table[]={
            {1,1, true},
            {256,255,true},
            {255,255,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestUInt32ToInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint32,int16> table[]={
            {1,1, true},
            {32768,32767,true},
            {32767, 32767, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestUInt32ToUInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint32,uint16> table[]={
            {1,1, true},
            {65535,65535,true},
            {65536,65535,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(TypeConversionGTest,TestUInt32ToInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint32,int32> table[]={
            {1,1, true},
            {2147483647,2147483647,true},
            {2147483648,2147483647,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestUInt32ToUInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint32,uint32> table[]={
            {1,1, true},
            {4294967295,4294967295,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestUInt32ToInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint32,int64> table[]={
            {1,1, true},
            {4294967295,4294967295,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestUInt32ToUInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint32,uint64> table[]={
            {1,1, true},
            {4294967295,4294967295,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


////////////////////////////////////////////////////////////////////////////////////



TEST(TypeConversionGTest,TestInt64ToInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int64,int8> table[]={
            {1,1, true},
            {-1,-1,true},
            {-128, -128, true},
            {127,127,true},
            {128,127,true},
            {-129,-128, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}



TEST(TypeConversionGTest,TestInt64ToUInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int64,uint8> table[]={
            {1,1, true},
            {-1,0,true},
            {255,255,true},
            {256, 255, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestInt64ToInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int64,int16> table[]={
            {1,1, true},
            {-1,-1,true},
            {32767,32767,true},
            {-32768, -32768, true},
            {32768,32767,true},
            {-32769, -32768, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestInt64ToUInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int64,uint16> table[]={
            {1,1, true},
            {65535,65535,true},
            {65536,65535,true},
            {-1, 0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(TypeConversionGTest,TestInt64ToInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int64,int32> table[]={
            {1,1, true},
            {-1,-1,true},
            {2147483647,2147483647,true},
            {-2147483648, -2147483648, true},
            {2147483648,2147483647,true},
            {-2147483649, -2147483648, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestInt64ToUInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int64,uint32> table[]={
            {1,1, true},
            {4294967295,4294967295,true},
            {4294967296,4294967295,true},
            {-1, 0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestInt64ToInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int64,int64> table[]={
            {1,1, true},
            {-1,-1,true},
            {9223372036854775807,9223372036854775807,true},
            {-9223372036854775808, -9223372036854775808, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestInt64ToUInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int64,uint64> table[]={
            {1,1, true},
            {9223372036854775807,9223372036854775807,true},
            {-1, 0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


///////////////////////////////////////////////////////////////////////////////////////


TEST(TypeConversionGTest,TestUInt64ToInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint64,int8> table[]={
            {1,1, true},
            {127,127,true},
            {128,127,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}



TEST(TypeConversionGTest,TestUInt64ToUInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint64,uint8> table[]={
            {1,1, true},
            {256,255,true},
            {255,255,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestUInt64ToInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint64,int16> table[]={
            {1,1, true},
            {32768,32767,true},
            {32767, 32767, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestUInt64ToUInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint64,uint16> table[]={
            {1,1, true},
            {65535,65535,true},
            {65536,65535,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(TypeConversionGTest,TestUInt64ToInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint64,int32> table[]={
            {1,1, true},
            {2147483647,2147483647,true},
            {2147483648,2147483647,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestUInt64ToUInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint64,uint32> table[]={
            {1,1, true},
            {4294967295,4294967295,true},
            {4294967296,4294967295,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestUInt64ToInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint64,int64> table[]={
            {1,1, true},
            {9223372036854775807,9223372036854775807,true},
            {9223372036854775808,9223372036854775807,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestUInt64ToUInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint64,uint64> table[]={
            {1,1, true},
            {18446744073709551615,18446744073709551615,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


///////////////////////////////////////////////////////////////////////////////////////////

TEST(TypeConversionGTest,TestInt8ToFloat32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int8,float32> table[]={
            {1,1, true},
            {-1,-1,true},
            {-128,-128,true},
            {127,127,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestInt8ToFloat64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int8,float64> table[]={
            {1,1, true},
            {-1,-1,true},
            {-128,-128,true},
            {127,127,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


///////////////////////////////////////////////////////////////////////////////////


TEST(TypeConversionGTest,TestUInt8ToFloat32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint8,float32> table[]={
            {1,1, true},
            {255,255,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestUInt8ToFloat64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint8,float64> table[]={
            {1,1, true},
            {255,255,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

/////////////////////////////////////////////////////////////////

TEST(TypeConversionGTest,TestInt16ToFloat32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int16,float32> table[]={
            {1,1, true},
            {-1,-1,true},
            {32767,32767,true},
            {-32768,-32768,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestInt16ToFloat64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int16,float64> table[]={
            {1,1, true},
            {-1,-1,true},
            {32767,32767,true},
            {-32768,-32768,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


///////////////////////////////////////////////////////////////////////////////////


TEST(TypeConversionGTest,TestUInt16ToFloat32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint16,float32> table[]={
            {1,1, true},
            {65535,65535,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestUInt16ToFloat64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint16,float64> table[]={
            {1,1, true},
            {65535,65535,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

/////////////////////////////////////////////////////////////////

TEST(TypeConversionGTest,TestInt32ToFloat32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int32,float32> table[]={
            {1,1, true},
            {-1,-1,true},
            {2147483647,2147483647,true},
            {-2147483648,-2147483648,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestInt32ToFloat64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int32,float64> table[]={
            {1,1, true},
            {-1,-1,true},
            {2147483647,2147483647,true},
            {-2147483648,-2147483648,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


///////////////////////////////////////////////////////////////////////////////////


TEST(TypeConversionGTest,TestUInt32ToFloat32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint32,float32> table[]={
            {1,1, true},
            {4294967295,4294967295,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestUInt32ToFloat64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint32,float64> table[]={
            {1,1, true},
            {4294967295,4294967295,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

/////////////////////////////////////////////////////////////////


TEST(TypeConversionGTest,TestInt64ToFloat32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int64,float32> table[]={
            {1,1, true},
            {-1,-1,true},
            {9223372036854775807,9223372036854775807,true},
            {-9223372036854775808,-9223372036854775808.0,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestInt64ToFloat64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int64,float64> table[]={
            {1,1, true},
            {-1,-1,true},
            {9223372036854775807,9223372036854775807,true},
            {-9223372036854775808,-9223372036854775808.0,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


///////////////////////////////////////////////////////////////////////////////////


TEST(TypeConversionGTest,TestUInt64ToFloat32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint64,float32> table[]={
            {1,1, true},
            {18446744073709551615,18446744073709551615,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestUInt64ToFloat64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint64,float64> table[]={
            {1,1, true},
            {18446744073709551615,18446744073709551615,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

/////////////////////////////////////////////////////////////////


TEST(TypeConversionGTest,TestFloat32ToInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float32,int8> table[]={
            {1.9,2, true},
            {126.5, 126, true},
            {126.51, 127, true},
            {127.999, 127, true},
            {127.49, 127, true},
            {127.51, 127, true},
            {128.0, 127, true},
            {-1.0, -1, true},
            {-127.5,-127,true},
            {-127.51, -128, true},
            {-128.9, -128, true},
            {-129.0, -128, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestFloat32ToUInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float32,uint8> table[]={
            {1.9,2, true},
            {255.0, 255, true},
            {254.499, 254, true},
            {254.5001, 255, true},
            {255.99, 255, true},
            {256.0, 255, true},
            {-1E-9, 0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

/////////////////////////////////////////////////////////////////


TEST(TypeConversionGTest,TestFloat32ToInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float32,int16> table[]={
            {1.9,2, true},
            {32766.5, 32766, true},
            {32766.51, 32767, true},
            {32767.999, 32767, true},
            {32767.49, 32767, true},
            {32767.51, 32767, true},
            {32768.0, 32767, true},
            {-1.0, -1, true},
            {-32767.5,-32767,true},
            {-32767.51, -32768, true},
            {-32768.9, -32768, true},
            {-32769.0, -32768, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestFloat32ToUInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float32,uint16> table[]={
            {1.9,2, true},
            {65535.0, 65535, true},
            {65534.499, 65534, true},
            {65534.505, 65535, true},
            {65535.99, 65535, true},
            {65536.0, 65535, true},
            {-1E-9, 0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

/////////////////////////////////////////////////////////////////


TEST(TypeConversionGTest,TestFloat32ToInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float32,int32> table[]={
            {1.9,2, true},
            // precision lost!!
            {214748364.0f, 214748368, true},
            // precision lost!!
            {2147483646.5f, 2147483647, true},
            {-1.0, -1, true},
            {-2147483647.5,-2147483648,true},
            {-2147483649.0, -2147483648, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestFloat32ToUInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float32,uint32> table[]={
            {1.9,2, true},
            //loss of precision !
            {4294967295.0, 4294967295, true},
            {4294967294.499, 4294967295, true},
            {4294967294.505, 4294967295, true},
            {4294967295.99, 4294967295, true},
            {4294967296.0,4294967295, true},
            {65536.0, 65536, true},
            {-1E-9, 0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

/////////////////////////////////////////////////////////////////


TEST(TypeConversionGTest,TestFloat32ToInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float32,int64> table[]={
            {1.9,2, true},
            {99223372036854775809.0f, 9223372036854775807, true},
            {99223372036854775807.0f, 9223372036854775807, true},
            // precision lost!!
            {9223372036854775807.0f, 9223372036854775807, true},
            // precision lost!!
            {9223372036854775806.5f, 9223372036854775807, true},
            {-1.0, -1, true},
            // precision lost!!
            {-9223372036854775807.0,-9223372036854775808,true},
            // precision lost!!
            {-9223372036854775807.5,-9223372036854775808,true},
            {-9223372036854775808.0, -9223372036854775808, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(TypeConversionGTest,TestFloat32ToUInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float32,uint64> table[]={
            {1.9,2, true},
            {18446744073709551617.0, 18446744073709551615, true},
            //loss of precision !
            {18446744073709551615.0, 18446744073709551615, true},
            {18446744073709551614.499, 18446744073709551615, true},
            {18446744073709551613.9, 18446744073709551615, true},
            {-1E-9, 0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

/////////////////////////////////////////////////////////////////


TEST(TypeConversionGTest,TestFloat64ToInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float64,int8> table[]={
            {1.9,2, true},
            {126.5, 126, true},
            {126.51, 127, true},
            {127.999, 127, true},
            {127.49, 127, true},
            {127.51, 127, true},
            {128.0, 127, true},
            {-1.0, -1, true},
            {-127.5,-127,true},
            {-127.51, -128, true},
            {-128.9, -128, true},
            {-129.0, -128, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestFloat64ToUInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float64,uint8> table[]={
            {1.9,2, true},
            {255.0, 255, true},
            {254.499, 254, true},
            {254.5001, 255, true},
            {255.99, 255, true},
            {256.0, 255, true},
            {-1E-9, 0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

/////////////////////////////////////////////////////////////////


TEST(TypeConversionGTest,TestFloat64ToInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float64,int16> table[]={
            {1.9,2, true},
            {32766.5, 32766, true},
            {32766.51, 32767, true},
            {32767.999, 32767, true},
            {32767.49, 32767, true},
            {32767.51, 32767, true},
            {32768.0, 32767, true},
            {-1.0, -1, true},
            {-32767.5,-32767,true},
            {-32767.51, -32768, true},
            {-32768.9, -32768, true},
            {-32769.0, -32768, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestFloat64ToUInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float64,uint16> table[]={
            {1.9,2, true},
            {65535.0, 65535, true},
            {65534.499, 65534, true},
            {65534.505, 65535, true},
            {65535.99, 65535, true},
            {65536.0, 65535, true},
            {-1E-9, 0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

/////////////////////////////////////////////////////////////////


TEST(TypeConversionGTest,TestFloat64ToInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float64,int32> table[]={
            {1.9,2, true},
            {2147483646.5, 2147483646, true},
            {-1.0, -1, true},
            {-2147483647.5,-2147483647,true},
            {-2147483649.0, -2147483648, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestFloat64ToUInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float64,uint32> table[]={
            {1.9,2, true},
            //loss of precision !
            {4294967295.0, 4294967295, true},
            {4294967294.499, 4294967294, true},
            {4294967294.505, 4294967295, true},
            {4294967295.99, 4294967295, true},
            {4294967296.0,4294967295, true},
            {65536.0, 65536, true},
            {-1E-9, 0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

/////////////////////////////////////////////////////////////////


TEST(TypeConversionGTest,TestFloat64ToInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float64,int64> table[]={
            {1.9,2, true},
            {99223372036854775809.0f, 9223372036854775807, true},
            {99223372036854775807.0f, 9223372036854775807, true},
            // precision lost!!
            {9223372036854775807.0f, 9223372036854775807, true},
            // precision lost!!
            {9223372036854775806.5f, 9223372036854775807, true},
            {-1.0, -1, true},
            // precision lost!!
            {-9223372036854775807.0,-9223372036854775808,true},
            // precision lost!!
            {-9223372036854775807.5,-9223372036854775808,true},
            {-9223372036854775808.0, -9223372036854775808, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(TypeConversionGTest,TestFloat64ToUInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float64,uint64> table[]={
            {1.9,2, true},
            {18446744073709551617.0, 18446744073709551615, true},
            //loss of precision !
            {18446744073709551615.0, 18446744073709551615, true},
            {18446744073709551614.499, 18446744073709551615, true},
            {18446744073709551613.9, 18446744073709551615, true},
            {-1E-9, 0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


/////////////////////////////////////////////////////////////////



TEST(TypeConversionGTest,TestFloat32ToFloat32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float32,float32> table[]={
            {1.9,1.9, true},
            {-1.9, -1.9, true},
            {340282346638528859811704183484516925440.000000,340282346638528859811704183484516925440.000000, true},
            {-340282346638528859811704183484516925440.000000,-340282346638528859811704183484516925440.000000, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(TypeConversionGTest,TestFloat32ToFloat64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float32,float64> table[]={
            {1.5,1.5, true},
            {-1.5, -1.5, true},
            {340282346638528859811704183484516925440.000000,340282346638528859811704183484516925440.000000, true},
            {-340282346638528859811704183484516925440.000000,-340282346638528859811704183484516925440.000000, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(TypeConversionGTest,TestFloat64ToFloat64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float64,float64> table[]={
            {1.9,1.9, true},
            {-1.9, -1.9, true},
            {179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.000000,179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.000000, true},
            {-179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.000000,-179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.000000, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestFloat64ToFloat32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float64,float32> table[]={
            {1.5,1.5, true},
            {-1.5, -1.5, true},
            {340282346638528859811704183484516925440.000000,340282346638528859811704183484516925440.000000, true},
            {-340282346638528859811704183484516925440.000000,-340282346638528859811704183484516925440.000000, true},

            // Saturation
            {340282346638528859811704183484516925480.000000,340282346638528859811704183484516925440.000000, true},
            {-340282346638528859811704183484516925480.000000,-340282346638528859811704183484516925440.000000, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}



//////////////////////////////////////////////////////////////////////////////////////////

TEST(TypeConversionGTest,TestInt8ToSString) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int8,String> table[]={
            {0,"0",true},
            {123, "123", true},
            {-1,"-1", true},
            {127,"127", true},
            {-128,"-128", true},
            {255,"-1",true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}



TEST(TypeConversionGTest,TestUInt8ToSString) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint8,String> table[]={
            {123,"123", true},
            {255,"255", true},
            {127,"127", true},
            {-1,"255",true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(TypeConversionGTest,TestInt16ToSString) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int16,String> table[]={
            {0,"0",true},
            {123, "123", true},
            {-1,"-1", true},
            {32767,"32767", true},
            {-32768,"-32768", true},
            {65535,"-1", true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestUInt16ToSString) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint16,String> table[]={
            {123,"123", true},
            {65535,"65535", true},
            {65536,"0", true},
            {32767,"32767", true},
            {-1,"65535",true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestInt32ToSString) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int32,String> table[]={
            {0,"0",true},
            {123, "123", true},
            {-1,"-1", true},
            {2147483647,"2147483647", true},
            {2147483648,"-2147483648", true},
            {-2147483648,"-2147483648", true},
            {-2147483649,"2147483647", true},
            {4294967295,"-1", true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestUInt32ToSString) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint32,String> table[]={
            {123,"123", true},
            {4294967295,"4294967295", true},
            {4294967296,"0", true},
            {2147483647,"2147483647", true},
            {-1,"4294967295",true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}



TEST(TypeConversionGTest,TestInt64ToSString) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int64,String> table[]={
            {0,"0",true},
            {123, "123", true},
            {-1,"-1", true},
            {9223372036854775807,"9223372036854775807", true},
            {9223372036854775808,"-9223372036854775808", true},
            {-9223372036854775808,"-9223372036854775808", true},
            {-9223372036854775809,"9223372036854775807", true},
            {18446744073709551615,"-1", true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestUInt64ToSString) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint64,String> table[]={
            {123,"123", true},
            {18446744073709551615,"18446744073709551615", true},
            {18446744073709551616,"0", true},
            {9223372036854775808,"9223372036854775808", true},
            {-1,"18446744073709551615",true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(TypeConversionGTest,float32ToSString) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float32,String> table[]={
            {123.5,"123.5000", true},
            {123.12345,"123.1234", true},
            {123.123456,"123.1235", true},
            {100,"100.0000", true},
            {10E+2,"1.000000E+3", true},
            {12345.678E+2,"1.234568E+6", true},
            {1E-2,"10.00000E-3", true},
            {0.1E-2,"1.000000E-3", true},
            {16E-2,"160.0000E-3", true},
            {1E+20,"100.0000E+18", true},
            {1E-7,"100.0000E-9", true},
            {1E-9,"1.000000E-9", true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestFloat64ToSString) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float64,String> table[]={
            {123.5,"123.500000000000", true},
            {123.12345,"123.123450000000", true},
            {123.123456,"123.123456000000", true},
            {100,"100.000000000000", true},
            {10E+2,"1.00000000000000E+3", true},
            {12345.678E+2,"1.23456780000000E+6", true},
            {1E-2,"10.0000000000000E-3", true},
            {0.1E-2,"1.00000000000000E-3", true},
            {16E-2,"160.000000000000E-3", true},
            {123E+63,"123.000000000000E+63", true},
            {123E+83,"12.3000000000000E+84", true},
            {1E-7,"100.000000000000E-9", true},
            {1E-9,"1.00000000000000E-9", true},
            {1E-25,"100.000000000000E-27", true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}





TEST(TypeConversionGTest,TestInt32ToInt32Vector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int32[3],int32[3]> table[]={
            {{1,2,3},{1,2,3}, true},
            {0,0,0}
    };

    const uint32 cols=3;
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertVector(table,cols));
}









