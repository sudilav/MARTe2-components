/**
 * @file EPICSEventListener.h
 * @brief Header file for class EPICSEventListener
 * @date 21/03/2017
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

 * @details This header file contains the declaration of the class EPICSEventListener
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EPICSEVENTLISTENER_H_
#define EPICSEVENTLISTENER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <cadef.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "EmbeddedServiceMethodBinderI.h"
#include "MessageI.h"
#include "ReferenceContainer.h"
#include "SingleThreadService.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief TODO
 *
 * +PV_1 = {
 *   Class = EPICSEventListener
 *   PVName = "MyPV1" //Compulsory. Name of the PV
 *   PVType = int32 //Compulsory. Type of the PV (currently int32, float32 and string are supported)
 *   Destination = StateMachine //Compulsory. Destination of the message
 *   ModeValue = ValueIsFunction //Compulsory. What is the meaning of the PV value:
 *                                             ValueIsFunction means that the value will be used as the Message Function name.
 *                                             ValueIsParameter means that the value will be set in param1 of the function (which implies that the function must be defined)
 *                                             ValueIsIgnore means that the value is not important. A change in the value will trigger a message to the specified destination with the speficied Function
 *   Function = FunctionToCall //Function to be called. Compulsory if ModeValue=ValueIsParameter and FunctionMap is not specified. If ModeValue=ValueIsFunction, this parameter shall not be set.
 *   FunctionMap = {{"0", "START"}, {"1", "STOP"}, ...} //Compulsory if ModeValue=ValueIsParameter and Function is not specified. The PV value will be used to translate the PV value into a Function name.
 *   Timeout = 1 //Optional. Must be > 0. The ca_pend_event timeout in seconds. Default 1 second.
 *   CPUs = 0x1 //Optional, The affinity of the thread running the CA ca_pend_event wait.
 *   StackSize = 262144 //Optional. The stack size of the thread running the CA ca_pend_event wait.
 * }
 */
class EPICSEventListener: public ReferenceContainer, EmbeddedServiceMethodBinderI {
public:

    CLASS_REGISTER_DECLARATION()

    /**
     * @brief TODO
     */
EPICSEventListener    ();

    /**
     * @brief TODO
     */
    virtual ~EPICSEventListener();

    /**
     * @brief TODO.
     * @return TODO.
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief TODO
     */
    virtual ErrorManagement::ErrorType Execute(const ExecutionInfo & info);


    /**
     * @brief TODO
     */
    friend void EPICSEventListenerEventCallback(struct event_handler_args args);

private:
    /**
     * TODO
     */
    float32 timeout;

    /**
     * The EmbeddedThread where the ca_pend_event is executed.
     */
    SingleThreadService executor;

    /**
     * The CPU mask for the executor
     */
    uint32 cpuMask;

    /**
     * The stack size
     */
    uint32 stackSize;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EPICSEVENTLISTENER_H_ */

