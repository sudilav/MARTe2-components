/**
 * @file TypeConversion.cpp
 * @brief Source file for class TypeConversion
 * @date 08/11/2015
 * @author Andre' Neto
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
 * the class TypeConversion (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#define DLL_API

#include "TypeConversion.h"
#include "AnyType.h"
#include "BitSetToInteger.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
extern bool StringToFloatGeneric(const char8 * const input,
                                 float32 * const number,
                                 const uint32 destBitSize);

/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
extern bool StringToIntegerGeneric(const char8* const source,
                                   uint8 * const dest,
                                   const uint32 destBitSize,
                                   const bool isSigned);

/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
extern bool IntegerToFloatGeneric(const uint8 * const source,
                                  const uint32 sourceBitSize,
                                  float32 * const dest,
                                  const uint32 destBitSize,
                                  const bool isSigned);

/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
extern bool FloatToIntegerGeneric(const float32 * const source,
                                  const uint32 sourceBitSize,
                                  uint8 * const dest,
                                  const uint32 destBitSize,
                                  const bool isSigned);

/**
 * @brief Saturate the number in input to the maximum or minimum 32-bit float number.
 * @param[in] isPositive specifies if the number must be set to the maximum 32-bit float (true) or to the minimum (false).
 * @param[in] numberOfBits specifies the bit-size of the number in input.
 * @param[in, out] number is the float number to be set.
 */
/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
template<typename FloatType>
static bool MinMaxFloat(const bool isPositive,
                        const uint8 numberOfBits,
                        FloatType &number) {

    bool ret = false;
    if (isPositive) {
        if (numberOfBits == 4u) {
            REPORT_ERROR(ErrorManagement::Warning, "FloatToFloat: Saturation to maximum float");
            uint32 maxFloat32Mask = 0x7f7fffffu;
            ret = MemoryOperationsHelper::Copy(&number, &maxFloat32Mask, 4u);
        }
    }
    else {
        if (numberOfBits == 4u) {
            REPORT_ERROR(ErrorManagement::Warning, "FloatToFloat: Saturation to minimum float");
            uint32 maxFloat32Mask = 0xff7fffffu;
            ret = MemoryOperationsHelper::Copy(&number, &maxFloat32Mask, 4u);
        }
    }
    return ret;
}

/**
 * @brief Performs the conversion from a float number to another float number.
 * @param[in] source is the float number to be converted.
 * @param[out] destination is the converted float number in output.
 * @return true if the conversion succeeds, false otherwise.
 * @post
 *   If the size of destination type is minor than the source type one, and the conversion returns a nan or inf number,
 *   the output will be saturated at the maximum or minimum depending on the sign of the source number and a warning will
 *   be generated.
 */
/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
template<typename FloatType1, typename FloatType2>
static bool FloatToFloat(const FloatType1 source,
                         FloatType2 &destination) {

    destination = static_cast<FloatType2>(0.0);

    bool isSourceNan = isNaN(source);
    bool isSourceInf = isInf(source);
    bool ret = ((!isSourceNan) && (!isSourceInf));
    if (ret) {
        FloatType2 test = static_cast<FloatType2>(source);

        bool isTestNan = isNaN(test);
        bool isTestInf = isInf(test);
        if ((isTestNan) || (isTestInf)) {
            if (sizeof(FloatType2) < sizeof(FloatType1)) {
                ret = MinMaxFloat((source > static_cast<FloatType2>(0.0)), static_cast<uint8>(sizeof(FloatType2)), destination);
            }
            else {
                ret = false;
            }
        }
        else {

            destination = test;
        }
    }

    return ret;
}

/**
 * @brief Performs the conversion from integer to any type.
 * @param[out] destination is the any type in output.
 * @param[in] source is the any type representing an integer number.
 * @return true if the conversion succeeds, false otherwise.
 * @pre
 *   source.GetTypeDescriptor() == SignedInteger ||
 *   source.GetTypeDescriptor() == UnsignedInteger;
 */
/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
static bool IntegerToType(const AnyType &destination,
                          const AnyType &source) {

    void* destinationPointer = destination.GetDataPointer();
    const TypeDescriptor destinationDescriptor = destination.GetTypeDescriptor();
    void* sourcePointer = source.GetDataPointer();
    const TypeDescriptor sourceDescriptor = source.GetTypeDescriptor();

    bool ret = false;

    bool isSourceSignedInteger = (sourceDescriptor.type == SignedInteger);
    bool isSourceUnsignedInteger = (sourceDescriptor.type == UnsignedInteger);

    if ((isSourceSignedInteger) || (isSourceUnsignedInteger)) {

        if (destinationDescriptor.type == SString) {
            StreamString tempString;
            ret = tempString.PrintFormatted("%d", &source);
            if (ret) {
                uint32 stringLength = static_cast<uint32>(tempString.Size());
                ret = (reinterpret_cast<StreamString*>(destinationPointer))->Write(tempString.Buffer(), stringLength);
            }
        }
        if (destinationDescriptor.type == CArray) {
            StreamString tempString;
            ret = tempString.PrintFormatted("%d", &source);
            if (ret) {
                uint32 stringLength = static_cast<uint32>(tempString.Size());
                uint32 arraySize = destination.GetByteSize();
                if (stringLength >= arraySize) {
                    REPORT_ERROR(ErrorManagement::Warning, "IntegerToType: The input is too long for the output buffer.");
                    ret = StringHelper::CopyN(reinterpret_cast<char8 *>(destinationPointer), tempString.Buffer(), arraySize);
                    uint32 lastCharIndex = arraySize - 1u;
                    reinterpret_cast<char8 *>(destinationPointer)[lastCharIndex] = '\0';
                }
                else {
                    ret = StringHelper::Copy(reinterpret_cast<char8 *>(destinationPointer), tempString.Buffer());
                }
            }
        }
        if (destinationDescriptor.type == CCString) {
            StreamString tempString;
            ret = tempString.PrintFormatted("%d", &source);
            if (ret) {
                // in this case the data pointer is the const char*
                if (destination.GetNumberOfDimensions() == 0u) {
                    ret = StringHelper::Copy(reinterpret_cast<char8 *>(destinationPointer), tempString.Buffer());
                }
                // in this case the data pointer points to the const char *
                else {
                    ret = StringHelper::Copy(*reinterpret_cast<char8 **>(destinationPointer), tempString.Buffer());
                }
            }
        }
        if (destinationDescriptor.type == SignedInteger) {
            if ((sourceDescriptor.type == SignedInteger)) {
                uint8* destinationInput = reinterpret_cast<uint8*>(destinationPointer);
                uint8* sourceInput = reinterpret_cast<uint8*>(sourcePointer);
                uint8 destShift = static_cast<uint8>(destination.GetBitAddress());
                uint8 sourceShift = static_cast<uint8>(source.GetBitAddress());
                ret = BitSetToBitSet(destinationInput, destShift, static_cast<uint8>(destinationDescriptor.numberOfBits), true, sourceInput, sourceShift,
                                     static_cast<uint8>(sourceDescriptor.numberOfBits), true);

            }
            if (sourceDescriptor.type == UnsignedInteger) {
                uint8* destinationInput = reinterpret_cast<uint8*>(destinationPointer);
                uint8* sourceInput = reinterpret_cast<uint8*>(sourcePointer);
                uint8 destShift = static_cast<uint8>(destination.GetBitAddress());
                uint8 sourceShift = static_cast<uint8>(source.GetBitAddress());
                ret = BitSetToBitSet(destinationInput, destShift, static_cast<uint8>(destinationDescriptor.numberOfBits), true, sourceInput, sourceShift,
                                     static_cast<uint8>(sourceDescriptor.numberOfBits), false);
            }
        }
        if (destinationDescriptor.type == UnsignedInteger) {
            if ((sourceDescriptor.type == SignedInteger)) {
                uint8* destinationInput = reinterpret_cast<uint8*>(destinationPointer);
                uint8* sourceInput = reinterpret_cast<uint8*>(sourcePointer);
                uint8 destShift = static_cast<uint8>(destination.GetBitAddress());
                uint8 sourceShift = static_cast<uint8>(source.GetBitAddress());
                ret = BitSetToBitSet(destinationInput, destShift, static_cast<uint8>(destinationDescriptor.numberOfBits), false, sourceInput, sourceShift,
                                     static_cast<uint8>(sourceDescriptor.numberOfBits), true);
            }
            if (sourceDescriptor.type == UnsignedInteger) {
                uint8* destinationInput = reinterpret_cast<uint8*>(destinationPointer);
                uint8* sourceInput = reinterpret_cast<uint8*>(sourcePointer);
                uint8 destShift = static_cast<uint8>(destination.GetBitAddress());
                uint8 sourceShift = static_cast<uint8>(source.GetBitAddress());
                ret = BitSetToBitSet(destinationInput, destShift, static_cast<uint8>(destinationDescriptor.numberOfBits), false, sourceInput, sourceShift,
                                     static_cast<uint8>(sourceDescriptor.numberOfBits), false);
            }
        }

        if (destinationDescriptor.type == Float) {
            if ((sourceDescriptor.type == SignedInteger)) {
                uint8* sourceInput = reinterpret_cast<uint8*>(sourcePointer);
                float32* destinationInput = reinterpret_cast<float32*>(destinationPointer);
                ret = IntegerToFloatGeneric(sourceInput, source.GetBitSize(), destinationInput, destination.GetBitSize(), true);
            }
            if (sourceDescriptor.type == UnsignedInteger) {
                uint8* sourceInput = reinterpret_cast<uint8*>(sourcePointer);
                float32* destinationInput = reinterpret_cast<float32*>(destinationPointer);
                ret = IntegerToFloatGeneric(sourceInput, source.GetBitSize(), destinationInput, destination.GetBitSize(), false);
            }
        }
    }
    return ret;

}

/**
 * @brief Performs the conversion from float to any type.
 * @param[out] destination is the any type in output.
 * @param[in] source is the any type representing a float number.
 * @return true if the conversion succeeds, false otherwise.
 * @pre
 *   source.GetTypeDescriptor() == Float;
 */
/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
static bool FloatToType(const AnyType &destination,
                        const AnyType &source) {

    void* destinationPointer = destination.GetDataPointer();
    const TypeDescriptor destinationDescriptor = destination.GetTypeDescriptor();
    void* sourcePointer = source.GetDataPointer();
    const TypeDescriptor sourceDescriptor = source.GetTypeDescriptor();

    bool ret = false;

    if (sourceDescriptor.type == Float) {

        if (destinationDescriptor.type == SString) {
            StreamString tempString;
            ret = tempString.PrintFormatted("%E", &source);
            if (ret) {
                uint32 stringLength = static_cast<uint32>(tempString.Size());
                ret = (reinterpret_cast<StreamString*>(destinationPointer))->Write(tempString.Buffer(), stringLength);
            }
        }
        if (destinationDescriptor.type == CArray) {
            StreamString tempString;
            ret = tempString.PrintFormatted("%E", &source);
            if (ret) {
                uint32 stringLength = static_cast<uint32>(tempString.Size());
                uint32 arraySize = destination.GetByteSize();
                if (stringLength >= arraySize) {
                    REPORT_ERROR(ErrorManagement::Warning, "FloatToType: The input is too long for the output buffer.");
                    ret = StringHelper::CopyN(reinterpret_cast<char8 *>(destinationPointer), tempString.Buffer(), arraySize);
                    uint32 lastCharIndex = arraySize - 1u;
                    reinterpret_cast<char8 *>(destinationPointer)[lastCharIndex] = '\0';
                }
                else {
                    ret = StringHelper::Copy(reinterpret_cast<char8 *>(destinationPointer), tempString.Buffer());
                }
            }
        }
        if (destinationDescriptor.type == CCString) {
            StreamString tempString;
            ret = tempString.PrintFormatted("%E", &source);
            if (ret) {
                // in this case the data pointer is the const char*
                if (destination.GetNumberOfDimensions() == 0u) {
                    ret = StringHelper::Copy(reinterpret_cast<char8 *>(destinationPointer), tempString.Buffer());
                }
                // in this case the data pointer points to the const char *
                else {
                    ret = StringHelper::Copy(*reinterpret_cast<char8 **>(destinationPointer), tempString.Buffer());
                }
            }
        }
        if (destinationDescriptor.type == SignedInteger) {
            ret = FloatToIntegerGeneric(reinterpret_cast<float32*>(sourcePointer), static_cast<uint8>(sourceDescriptor.numberOfBits),
                                        reinterpret_cast<uint8*>(destinationPointer), destination.GetBitSize(), true);
        }
        if (destinationDescriptor.type == UnsignedInteger) {
            ret = FloatToIntegerGeneric(reinterpret_cast<float32*>(sourcePointer), static_cast<uint8>(sourceDescriptor.numberOfBits),
                                        reinterpret_cast<uint8*>(destinationPointer), destination.GetBitSize(), false);
        }
        if (destinationDescriptor.type == Float) {
            if (destination.GetBitSize() == 32u) {
                if (source.GetBitSize() == 64u) {
                    ret = FloatToFloat(*(reinterpret_cast<float64*>(sourcePointer)), *(reinterpret_cast<float32*>(destinationPointer)));
                }
            }
            if (destination.GetBitSize() == 64u) {
                if (source.GetBitSize() == 32u) {
                    ret = FloatToFloat(*(reinterpret_cast<float32*>(sourcePointer)), *(reinterpret_cast<float64*>(destinationPointer)));
                }
            }
        }
    }
    return ret;
}
/**
 * @brief Performs the conversion from string types (CCString, StreamString, CArray) to any type.
 * @param[out] destination is the any type in output.
 * @param[in] source is the any type representing a string.
 * @return true if the conversion succeeds, false otherwise.
 * @pre
 *   source.GetTypeDescriptor() == StreamString ||
 *   source.GetTypeDescriptor() == CCString ||
 *   source.GetTypeDescriptor() == CArray;
 */
/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
static bool StringToType(const AnyType &destination,
                         const AnyType &source) {

    void* destinationPointer = destination.GetDataPointer();
    const TypeDescriptor destinationDescriptor = destination.GetTypeDescriptor();
    void* sourcePointer = source.GetDataPointer();
    const TypeDescriptor sourceDescriptor = source.GetTypeDescriptor();

    bool ret = false;

    const char8* token = static_cast<const char8 *>(NULL);
    if (sourceDescriptor.type == CCString) {
        // in this case the data pointer is the const char*
        if (source.GetNumberOfDimensions() == 0u) {
            token = reinterpret_cast<const char8*>(sourcePointer);
        }
        // in this case the data pointer points to the const char *
        else {
            token = *reinterpret_cast<const char8**>(sourcePointer);
        }
    }
    if (sourceDescriptor.type == CArray) {
        token = reinterpret_cast<const char8*>(sourcePointer);
    }
    if ((sourceDescriptor.type == SString)) {
        token = (reinterpret_cast<StreamString*>(sourcePointer))->Buffer();
    }

    if (token != NULL) {
        uint32 tokenLength = StringHelper::Length(token);
        if (destinationDescriptor.type == SString) {
            StreamString* tempString = reinterpret_cast<StreamString*>(destinationPointer);
            ret = tempString->Write(token, tokenLength);
        }
        if (destinationDescriptor.type == CArray) {
            uint32 arraySize = destination.GetByteSize();
            if (tokenLength >= arraySize) {
                REPORT_ERROR(ErrorManagement::Warning, "StringToType: The input is too long for the output buffer.");
                ret = StringHelper::CopyN(reinterpret_cast<char8 *>(destinationPointer), token, arraySize);
                uint32 lastCharIndex = arraySize - 1u;
                reinterpret_cast<char8 *>(destinationPointer)[lastCharIndex] = '\0';
            }
            else {
                ret = StringHelper::Copy(reinterpret_cast<char8 *>(destinationPointer), token);
            }
        }
        if (destinationDescriptor.type == CCString) {
            // in this case the data pointer is the const char*
            if (destination.GetNumberOfDimensions() == 0u) {
                ret = StringHelper::Copy(reinterpret_cast<char8 *>(destinationPointer), token);
            }
            // in this case the data pointer points to the const char *
            else {
                ret = StringHelper::Copy(*reinterpret_cast<char8 **>(destinationPointer), token);
            }
        }
        if (destinationDescriptor.type == SignedInteger) {
            ret = StringToIntegerGeneric(token, reinterpret_cast<uint8*>(destinationPointer), destination.GetBitSize(), true);

        }
        if (destinationDescriptor.type == UnsignedInteger) {

            ret = StringToIntegerGeneric(token, reinterpret_cast<uint8*>(destinationPointer), destination.GetBitSize(), false);
        }
        if (destinationDescriptor.type == Float) {
            ret = StringToFloatGeneric(token, (reinterpret_cast<float32*>(destinationPointer)), destination.GetBitSize());
        }
    }
    return ret;
}

/**
 * @brief Performs the basic type conversion.
 * @param[out] destination is the converted type in output.
 * @param[in] source is the type to be converted.
 * @return true if the conversion succeeds, false otherwise.
 */
/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
static bool ScalarBasicTypeConvert(const AnyType &destination,
                                   const AnyType &source) {

    void* destinationPointer = destination.GetDataPointer();
    const TypeDescriptor destinationDescriptor = destination.GetTypeDescriptor();
    void* sourcePointer = source.GetDataPointer();
    const TypeDescriptor sourceDescriptor = source.GetTypeDescriptor();

    bool ret = false;

    if (sourceDescriptor == destinationDescriptor) {
        if (sourceDescriptor.type == SString) {
            StreamString *stringSource = reinterpret_cast<StreamString*>(sourcePointer);
            StreamString *stringDestination = reinterpret_cast<StreamString*>(destinationPointer);
            uint32 stringLength = static_cast<uint32>(stringSource->Size());
            ret = stringDestination->Write(stringSource->Buffer(), stringLength);
        }
        else if (sourceDescriptor.type == CCString) {
            if (source.GetNumberOfDimensions() == 0u) {
                ret = StringHelper::Copy(reinterpret_cast<char8 *>(destinationPointer), reinterpret_cast<const char8 *>(sourcePointer));
            }
            else {
                ret = StringHelper::Copy(*reinterpret_cast<char8 **>(destinationPointer), *reinterpret_cast<const char8 **>(sourcePointer));
            }
        }
        else {
            uint32 copySize = static_cast<uint32>(sourceDescriptor.numberOfBits) / 8u;
            ret = MemoryOperationsHelper::Copy(destinationPointer, sourcePointer, copySize);
        }
    }
    else {
        bool isSourceSignedInteger = (sourceDescriptor.type == SignedInteger);
        bool isSourceUnsignedInteger = (sourceDescriptor.type == UnsignedInteger);

        if ((isSourceSignedInteger) || (isSourceUnsignedInteger)) {
            ret = IntegerToType(destination, source);
        }
        else if (sourceDescriptor.type == Float) {
            ret = FloatToType(destination, source);
        }
        else {
            ret = StringToType(destination, source);
        }

    }

    return ret;
}

/**
 * @brief Performs the conversion from a vector to a vector.
 * @param[out] destination is the converted vector in output.
 * @param[in] source is the vector to be converted.
 * @return true if the conversion succeeds, false otherwise.
 * @pre
 *   source.GetNumberOfDimension() == 1 &&
 *   destination.GetNumberOfDimension() == 1 &&
 *   source.GetNumberOfElements([0:2]) == destination.GetNumberOfElements([0:2]);
 */
/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
static bool VectorBasicTypeConvert(const AnyType &destination,
                                   const AnyType &source) {
    uint32 numberOfElements = source.GetNumberOfElements(0u);

// Assume that the number of dimensions is equal
    bool ok = true;

    for (uint32 idx = 0u; (idx < numberOfElements); idx++) {
        uint32 sourceElementByteSize = static_cast<uint32>(source.GetByteSize());
        uint32 sourceIndex = idx * sourceElementByteSize;
        uint32 destinationElementByteSize = static_cast<uint32>(destination.GetByteSize());
        uint32 destinationIndex = idx * destinationElementByteSize;

        char8 *sourceArray = reinterpret_cast<char8 *>(source.GetDataPointer());

        bool isSourceCArray = (source.GetTypeDescriptor().type == CArray);
        bool isSourceStaticDeclared = source.IsStaticDeclared();

        // Consider the special case of matrix of characters
        if ((isSourceCArray) && (!isSourceStaticDeclared)) {
            sourceArray = reinterpret_cast<char8 **>(source.GetDataPointer())[idx];
            sourceIndex = 0u;
        }

        char8 *destinationArray = reinterpret_cast<char8 *>(destination.GetDataPointer());

        bool isDestinationCArray = (destination.GetTypeDescriptor().type == CArray);
        bool isDestinationStaticDeclared = destination.IsStaticDeclared();

        // Consider the special case of matrix of characters
        if ((isDestinationCArray) && (!isDestinationStaticDeclared)) {
            destinationArray = reinterpret_cast<char8 **>(destination.GetDataPointer())[idx];
            destinationIndex = 0u;
        }

        AnyType elementSource(source);
        elementSource.SetDataPointer(&sourceArray[sourceIndex]);
        AnyType elementDestination(destination);
        elementDestination.SetDataPointer(&destinationArray[destinationIndex]);

        if (!ScalarBasicTypeConvert(elementDestination, elementSource)) {
            ok = false;
        }

    }
    return ok;
}

/**
 * @brief Performs the conversion from a static matrix to a static matrix.
 * @param[out] destination is the converted matrix in output.
 * @param[in] source is the matrix to be converted.
 * @return true if the conversion succeeds, false otherwise.
 * @pre
 *   source.GetNumberOfDimension() == 2 &&
 *   destination.GetNumberOfDimension() == 2 &&
 *   source.GetNumberOfElements([0:2]) == destination.GetNumberOfElements([0:2]);
 */
/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
static bool StaticToStaticMatrix(const AnyType &destination,
                                 const AnyType &source) {

    uint32 numberOfRows = destination.GetNumberOfElements(1u);
    uint32 numberOfColumns = destination.GetNumberOfElements(0u);
    TypeDescriptor sourceDescriptor = source.GetTypeDescriptor();
    TypeDescriptor destinationDescriptor = destination.GetTypeDescriptor();
    void *sourcePointer = source.GetDataPointer();
    void *destinationPointer = destination.GetDataPointer();

    bool ok = true;
    for (uint32 r = 0u; (r < numberOfRows); r++) {

        char8* sourceArray = reinterpret_cast<char8 *>(sourcePointer);
        char8* destinationArray = reinterpret_cast<char8 *>(destinationPointer);

        uint32 sourceElementByteSize = static_cast<uint32>(source.GetByteSize());
        uint32 destinationElementByteSize = static_cast<uint32>(destination.GetByteSize());

        uint32 sourceRowIndex = ((r * numberOfColumns) * sourceElementByteSize);
        AnyType sourceRow(sourceDescriptor, source.GetBitAddress(), &sourceArray[sourceRowIndex]);
        sourceRow.SetNumberOfDimensions(1u);
        sourceRow.SetNumberOfElements(0u, numberOfColumns);
        sourceRow.SetStaticDeclared(true);

        uint32 destinationRowIndex = ((r * numberOfColumns) * destinationElementByteSize);
        AnyType destinationRow(destinationDescriptor, destination.GetBitAddress(), &destinationArray[destinationRowIndex]);
        destinationRow.SetNumberOfDimensions(1u);
        destinationRow.SetNumberOfElements(0u, numberOfColumns);
        destinationRow.SetStaticDeclared(true);

        if (!VectorBasicTypeConvert(destinationRow, sourceRow)) {
            ok = false;
        }
    }
    return ok;
}

/**
 * @brief Performs the conversion from a matrix allocated on heap to a static matrix.
 * @param[out] destination is the converted matrix in output.
 * @param[in] source is the matrix to be converted.
 * @return true if the conversion succeeds, false otherwise.
 * @pre
 *   source.GetNumberOfDimension() == 2 &&
 *   destination.GetNumberOfDimension() == 2 &&
 *   source.GetNumberOfElements([0:2]) == destination.GetNumberOfElements([0:2]);
 */
/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
static bool StaticToHeapMatrix(const AnyType &destination,
                               const AnyType &source) {
    uint32 numberOfRows = destination.GetNumberOfElements(1u);
    uint32 numberOfColumns = destination.GetNumberOfElements(0u);
    TypeDescriptor sourceDescriptor = source.GetTypeDescriptor();
    TypeDescriptor destinationDescriptor = destination.GetTypeDescriptor();
    void *sourcePointer = source.GetDataPointer();
    void *destinationPointer = destination.GetDataPointer();

    bool ok = true;
    for (uint32 r = 0u; (r < numberOfRows); r++) {

        char8* sourceArray = reinterpret_cast<char8 *>(sourcePointer);
        char8* destinationArray = reinterpret_cast<char8 **>(destinationPointer)[r];

        uint32 sourceElementByteSize = static_cast<uint32>(source.GetByteSize());

        uint32 sourceRowIndex = ((r * numberOfColumns) * sourceElementByteSize);
        AnyType sourceRow(sourceDescriptor, source.GetBitAddress(), &sourceArray[sourceRowIndex]);
        sourceRow.SetNumberOfDimensions(1u);
        sourceRow.SetNumberOfElements(0u, numberOfColumns);
        sourceRow.SetStaticDeclared(true);

        AnyType destinationRow(destinationDescriptor, destination.GetBitAddress(), destinationArray);
        destinationRow.SetNumberOfDimensions(1u);
        destinationRow.SetNumberOfElements(0u, numberOfColumns);
        destinationRow.SetStaticDeclared(false);

        if (!VectorBasicTypeConvert(destinationRow, sourceRow)) {
            ok = false;
        }
    }
    return ok;
}

/**
 * @brief Performs the conversion from a static matrix to a matrix allocated on heap.
 * @param[out] destination is the converted matrix in output.
 * @param[in] source is the matrix to be converted.
 * @return true if the conversion succeeds, false otherwise.
 * @pre
 *   source.GetNumberOfDimension() == 2 &&
 *   destination.GetNumberOfDimension() == 2 &&
 *   source.GetNumberOfElements([0:2]) == destination.GetNumberOfElements([0:2]);
 */
/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
static bool HeapToStaticMatrix(const AnyType &destination,
                               const AnyType &source) {
    uint32 numberOfRows = destination.GetNumberOfElements(1u);
    uint32 numberOfColumns = destination.GetNumberOfElements(0u);
    TypeDescriptor sourceDescriptor = source.GetTypeDescriptor();
    TypeDescriptor destinationDescriptor = destination.GetTypeDescriptor();
    void *sourcePointer = source.GetDataPointer();
    void *destinationPointer = destination.GetDataPointer();

    bool ok = true;
    for (uint32 r = 0u; (r < numberOfRows); r++) {
        char8* sourceArray = reinterpret_cast<char8 **>(sourcePointer)[r];
        char8* destinationArray = reinterpret_cast<char8 *>(destinationPointer);

        AnyType sourceRow(sourceDescriptor, source.GetBitAddress(), sourceArray);
        sourceRow.SetNumberOfDimensions(1u);
        sourceRow.SetNumberOfElements(0u, numberOfColumns);
        sourceRow.SetStaticDeclared(false);

        uint32 destinationElementByteSize = static_cast<uint32>(destination.GetByteSize());
        uint32 destinationRowIndex = r * numberOfColumns * destinationElementByteSize;
        AnyType destinationRow(destinationDescriptor, destination.GetBitAddress(), &destinationArray[destinationRowIndex]);
        destinationRow.SetNumberOfDimensions(1u);
        destinationRow.SetNumberOfElements(0u, numberOfColumns);
        destinationRow.SetStaticDeclared(true);

        if (!VectorBasicTypeConvert(destinationRow, sourceRow)) {
            ok = false;
        }
    }
    return ok;
}

/**
 * @brief Performs the conversion from a matrix allocated on heap to a matrix allocated on heap.
 * @param[out] destination is the converted matrix in output.
 * @param[in] source is the matrix to be converted.
 * @return true if the conversion succeeds, false otherwise.
 * @pre
 *   source.GetNumberOfDimension() == 2 &&
 *   destination.GetNumberOfDimension() == 2 &&
 *   source.GetNumberOfElements([0:2]) == destination.GetNumberOfElements([0:2]);
 */
/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
static bool HeapToHeapMatrix(const AnyType &destination,
                             const AnyType &source) {
    uint32 numberOfRows = destination.GetNumberOfElements(1u);
    uint32 numberOfColumns = destination.GetNumberOfElements(0u);
    TypeDescriptor sourceDescriptor = source.GetTypeDescriptor();
    TypeDescriptor destinationDescriptor = destination.GetTypeDescriptor();
    void *sourcePointer = source.GetDataPointer();
    void *destinationPointer = destination.GetDataPointer();

    bool ok = true;
    for (uint32 r = 0u; (r < numberOfRows); r++) {
        char8* sourceArray = reinterpret_cast<char8 **>(sourcePointer)[r];
        char8* destinationArray = reinterpret_cast<char8 **>(destinationPointer)[r];

        AnyType sourceRow(sourceDescriptor, source.GetBitAddress(), sourceArray);
        sourceRow.SetNumberOfDimensions(1u);
        sourceRow.SetNumberOfElements(0u, numberOfColumns);
        sourceRow.SetStaticDeclared(false);

        AnyType destinationRow(destinationDescriptor, destination.GetBitAddress(), destinationArray);
        destinationRow.SetNumberOfDimensions(1u);
        destinationRow.SetNumberOfElements(0u, numberOfColumns);
        destinationRow.SetStaticDeclared(false);

        if (!VectorBasicTypeConvert(destinationRow, sourceRow)) {
            ok = false;
        }
    }
    return ok;
}

/**
 * @brief Calls the correct matrix conversion function depending on the source and destination memory type (static or heap).
 * @param[out] destination is the converted matrix in output.
 * @param[in] source is the matrix to be converted.
 * @return true if the conversion succeeds, false otherwise.
 * @pre
 *   source.GetNumberOfDimension() == 1 &&
 *   destination.GetNumberOfDimension() == 1 &&
 *   source.GetNumberOfElements([0:2]) == destination.GetNumberOfElements([0:2]);
 */
/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
static bool MatrixBasicTypeConvert(const AnyType &destination,
                                   const AnyType &source) {

    bool isSourceStatic = source.IsStaticDeclared();
    bool isDestinationStatic = destination.IsStaticDeclared();

    bool ok = false;

    if ((isSourceStatic) && (isDestinationStatic)) {
        ok = StaticToStaticMatrix(destination, source);
    }

    if ((isSourceStatic) && (!isDestinationStatic)) {
        ok = StaticToHeapMatrix(destination, source);
    }

    if ((!isSourceStatic) && (isDestinationStatic)) {
        ok = HeapToStaticMatrix(destination, source);
    }

    if ((!isSourceStatic) && (!isDestinationStatic)) {
        ok = HeapToHeapMatrix(destination, source);
    }

    return ok;
}

}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

bool TypeConvert(const AnyType &destination,
                 const AnyType &source) {

    bool ok = true;
    if (static_cast<bool>(destination.GetTypeDescriptor().isConstant)) {
        ok = false;
    }
    if (ok) {

        //Source and destination dimensions must be the same
        ok = (destination.GetNumberOfDimensions() == source.GetNumberOfDimensions());
        //The number of elements in all dimensions must be the same
        for (uint32 i = 0u; ok && (i < 3u); i++) {
            ok = (destination.GetNumberOfElements(i) == source.GetNumberOfElements(i));
        }

        if (ok) {
            if (source.GetNumberOfDimensions() == 0u) {
                ok = ScalarBasicTypeConvert(destination, source);
            }
            if (source.GetNumberOfDimensions() == 1u) {
                ok = VectorBasicTypeConvert(destination, source);
            }
            if (source.GetNumberOfDimensions() == 2u) {
                ok = MatrixBasicTypeConvert(destination, source);
            }
        }
    }

    return ok;
}

}