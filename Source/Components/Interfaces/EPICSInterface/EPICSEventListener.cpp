/**
 * @file EPICSEventListener.cpp
 * @brief Source file for class EPICSEventListener
 * @date 21/03/2017
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
 * the class EPICSEventListener (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "EPICSEventListener.h"
#include "EPICSPVEvent.h"
#include "Message.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/**
 * TODO
 */
static FastPollingMutexSem eventCallbackFastMux;
void EPICSEventListenerEventCallback(struct event_handler_args args) {
    eventCallbackFastMux.FastLock();
    EPICSEventListener *listener = static_cast<EPICSEventListener *>(args.usr);
    StreamString currentValue;
    if (listener != NULL_PTR(EPICSEventListener *)) {
        currentValue = static_cast<const char8 *>(args.dbr);
        bool found = false;
        uint32 j;
        for (j = 0u; (j < listener->Size()) && (!found); j++) {
            ReferenceT<EPICSPVEvent> pvEvent = listener->Get(j);
            StreamString pvName;
            if (pvEvent.IsValid()) {
                found = (pvEvent->GetPVChid() == args.chid);
                if (found) {
                    pvEvent->ValueChanged(currentValue);
                }
            }
        }
    }

    eventCallbackFastMux.FastUnLock();
}
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
EPICSEventListener::EPICSEventListener() :
        ReferenceContainer(), EmbeddedServiceMethodBinderI(), executor(*this) {
    timeout = 1.0F;
    cpuMask = 0xff;
    stackSize = THREADS_DEFAULT_STACKSIZE;
    eventCallbackFastMux.Create();
}

EPICSEventListener::~EPICSEventListener() {
    if (!executor.Stop()) {
        if (!executor.Stop()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not stop SingleThreadService.");
        }
    }
}

bool EPICSEventListener::Initialise(StructuredDataI & data) {
    bool ok = ReferenceContainer::Initialise(data);
    if (ok) {
        if (data.Read("Timeout", timeout)) {
            if (timeout == 0u) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Timeout shall be > 0");
            }
            else {
                REPORT_ERROR(ErrorManagement::Information, "Timeout set to %f s", timeout);
            }
        }
    }
    if (ok) {
        if (!data.Read("CPUs", cpuMask)) {
            REPORT_ERROR(ErrorManagement::Information, "No CPUs defined. Using default = %d", cpuMask);
        }
        if (!data.Read("StackSize", stackSize)) {
            REPORT_ERROR(ErrorManagement::Information, "No StackSize defined. Using default = %d", stackSize);
        }
        //This stack size should be sufficient?
        executor.SetStackSize(stackSize);
        executor.SetCPUMask(cpuMask);
        ok = (executor.Start() == ErrorManagement::NoError);
    }

    return ok;
}

ErrorManagement::ErrorType EPICSEventListener::Execute(const ExecutionInfo& info) {
    ErrorManagement::ErrorType err = ErrorManagement::NoError;
    if (info.GetStage() == ExecutionInfo::StartupStage) {
        if (ca_context_create(ca_enable_preemptive_callback) != ECA_NORMAL) {
            err = ErrorManagement::FatalError;
            REPORT_ERROR(err, "ca_enable_preemptive_callback failed");
        }
        if (err.ErrorsCleared()) {
            uint32 j;
            for (j = 0u; j < Size(); j++) {
                ReferenceT<EPICSPVEvent> pvEvent = Get(j);
                StreamString pvName;
                chid pvChid;
                if (pvEvent.IsValid()) {
                    pvEvent->GetPVName(pvName);
                    if (ca_create_channel(pvName.Buffer(), NULL, NULL, 20, &pvChid) != ECA_NORMAL) {
                        err = ErrorManagement::FatalError;
                        REPORT_ERROR(err, "ca_create_channel failed for PV with name %s", pvName.Buffer());
                    }
                }
                if (err.ErrorsCleared()) {
                    pvEvent->SetPVChid(pvChid);
                    evid ignore;
                    if (ca_create_subscription(DBR_STRING, 1u, pvChid, DBE_VALUE, EPICSEventListenerEventCallback, this, &ignore) != ECA_NORMAL) {
                        err = ErrorManagement::FatalError;
                        REPORT_ERROR(err, "ca_create_subscription failed for PV %s", pvName.Buffer());
                    }
                }
            }
        }
    }
    else if (info.GetStage() != ExecutionInfo::TerminationStage) {
        ca_pend_event(timeout);
    }
    else {
        ca_context_destroy();
    }
    return err;
}

CLASS_REGISTER(EPICSEventListener, "1.0")
}

