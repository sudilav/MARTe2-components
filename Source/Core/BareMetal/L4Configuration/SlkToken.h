/**
 * @file SlkToken.h
 * @brief Header file for class SlkToken
 * @date 04/12/2015
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

 * @details This header file contains the declaration of the class SlkToken
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SLKTOKEN_H_
#define SLKTOKEN_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "StreamString.h"
#include "LexicalAnalyzer.h"
#include "StreamI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

class SlkToken{

public:
    SlkToken(StreamI &stream, const char8 *terminals, const char8 *separators);

    uint16 get();

    uint16 peek(uint16 position);

    const char8 *GetTokenData();

    uint32 GetLineNumber() const;

    uint16 GetTokenId() const;

private:
    uint32 tokenLineNumber;

    uint16 tokenId;

    StreamString tokenData;

    LexicalAnalyzer lexicalAnalyzer;
};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SLKTOKEN_H_ */

