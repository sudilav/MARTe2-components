/**
 * @file FastMath.h
 * @brief Header file for class FastMath
 * @date 26/06/2015
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

 * @details This header file contains the declaration of the class FastMath
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef FASTMATH_H_
#define 		FASTMATH_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include INCLUDE_FILE_ARCHITECTURE(ARCHITECTURE,FastMathA.h)

/**
 * @brief Performs mathematical operations using fast processor instructions.
 */
class FastMath {
public:

    /** e */
    static const double E;
    /** log2(e) */
    static const double LOG2E;
    /** log10(e) */
    static const double LOG10E;
    /** log(2) */
    static const double LN2;
    /** log(10) */
    static const double LN10;
    /** pi */
    static const double PI;
    /** pi/2 */
    static const double PI_2;
    /** pi/4 */
    static const double PI_4;
    /** 1/pi */
    static const double _1_PI;
    /** 2/pi */
    static const double _2_PI;
    /** 2/sqrt(pi)*/
    static const double _2_SQRTPI;
    /** sqrt(2) */
    static const double SQRT2;
    /** sqrt(1/2) */
    static const double SQRT1_2;

    /**
     * @brief Converts a float to an integer using a processor instruction.
     * @param[in] input the value to convert
     * @return the input as an integer
     */
    static inline int32 FloatToInt(float input);

    /**
     * @brief Computes the cosine of an angle using a processor instruction.
     * @param[in] angle the angle to compute
     * @return the cos(angle)
     */
    static inline float Cos(float angle);

    /**
     * @brief Computes the sine of an angle using a processor instruction.
     * @param[in] angle the angle to compute
     * @return the sin(angle)
     */
    static inline float Sin(float angle);
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

int32 FastMath::FloatToInt(float input) {
    return FastMathA::FloatToInt(input);
}


float FastMath::Cos(float angle) {
    return FastMathA::Cos(angle);
}

float FastMath::Sin(float angle) {
    return FastMathA::Sin(angle);
}


#endif /* FASTMATH_H_ */

