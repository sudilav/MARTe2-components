/**
 * @file MemoryMapSynchNMutexOutputBroker.h
 * @brief   Specialized MemoryMapOutputBroker with output-related synch features.
 * @date 
 * @author
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

 * @details     Specialization of the MemoryMapOutputBroker, which leverages ISynchronisableOutput
 *              interface, implemented by the DataSource, to synchronise outputs.
 *              Before starting with the copy the Synchronise is called on outputs (SynchroniseOutput)
 *              while the DataSource is expected to mutually exclude memory access, until the broker signals
 *              using the TerminateOutputCopy its ending.
 */

#ifndef DATASOURCES_PROFINET_MEMMAPSYNCHNMUTEXOUTPUTBROKER_H_
#define DATASOURCES_PROFINET_MEMMAPSYNCHNMUTEXOUTPUTBROKER_H_

#include "MemoryMapOutputBroker.h"
#include "ISynchronisableOutput.h"

namespace MARTe {

    class MemoryMapSynchNMutexOutputBroker : public MemoryMapOutputBroker {

        public:
            CLASS_REGISTER_DECLARATION()

            MemoryMapSynchNMutexOutputBroker();
            virtual ~MemoryMapSynchNMutexOutputBroker();
            virtual bool Execute();

    };

}

#endif /* DATASOURCES_PROFINET_MEMMAPSYNCHNMUTEXOUTPUTBROKER_H_ */