/**
 * @file ObjectRegistryDatabase.cpp
 * @brief Source file for class ObjectRegistryDatabase
 * @date 18/02/2016
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
 * the class ObjectRegistryDatabase (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */
#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ErrorType.h"
#include "ObjectRegistryDatabase.h"
#include "ReferenceContainerFilterObjectName.h"
#include "ReferenceContainerFilterReferences.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

ObjectRegistryDatabase *ObjectRegistryDatabase::Instance() {

    static ObjectRegistryDatabase *instance = NULL_PTR(ObjectRegistryDatabase *);
    if (instance == NULL) {
        instance=new ObjectRegistryDatabase(); //dynamic_cast<ObjectRegistryDatabase*>(ObjectRegistryDatabase_BuildFn());
        GlobalObjectsDatabase::Instance()->Add(instance, NUMBER_OF_GLOBAL_OBJECTS - 3u);
    }
    return instance;
}

ObjectRegistryDatabase::ObjectRegistryDatabase() {
}

ObjectRegistryDatabase::~ObjectRegistryDatabase() {
}

bool ObjectRegistryDatabase::CleanUp() {
    bool ret;
    for (uint32 i = 0u; i < Size(); i++) {
        Reference toBeRemoved=Get(i);
        ret=ReferenceContainer::Delete(toBeRemoved);
    }
    return ret;
}

Reference ObjectRegistryDatabase::Find(const char8 * const path,
                                       const Reference current) {
    ReferenceT<ReferenceContainer> domain = current;
    bool isSearchDomain = current.IsValid();
    uint32 backSteps = 0u;
    if (isSearchDomain) {
        while (path[backSteps] == ':') {
            backSteps++;
        }
        uint32 stepsCounter = backSteps;
        // search the current remembering the path
        ReferenceContainerFilterReferences filterRef(1, ReferenceContainerFilterMode::PATH, current);
        ReferenceContainer resultPath;
        ReferenceContainer::Find(resultPath, filterRef);
        for (uint32 i = 0u; i < resultPath.Size(); i++) {
            Reference test = resultPath.Get(resultPath.Size() - i - 1u);
            if (stepsCounter == 0u) {
                break;
            }
            if (test.IsValid()) {
                if (test->GetName()[0] == '$') {
                    domain = test;
                    stepsCounter--;
                }
            }
        }

        if (stepsCounter > 0) {
            REPORT_ERROR(ErrorManagement::Warning, "Find: Too many back steps in the path. The searching will start from the root");
            isSearchDomain = false;
        }

    }
    // now search from the domain forward
    Reference ret;
    ReferenceContainerFilterObjectName filterName(1, ReferenceContainerFilterMode::RECURSIVE, &path[backSteps]);
    ReferenceContainer resultSingle;

    if (isSearchDomain) {
        if (domain.IsValid()) {
            domain->Find(resultSingle, filterName);
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "Find: Invalid domain");
        }
    }
    else {
        // search from the beginning
        ReferenceContainer::Find(resultSingle, filterName);
    }

    bool ok = (resultSingle.Size() > 0u);
    if (ok) {
        //Invalidate move to leafs
        ret = resultSingle.Get(resultSingle.Size() - 1u);
    }

    return ret;
}



const char8 * const ObjectRegistryDatabase::GetClassName() const {
    return "ObjectRegistryDatabase";
}

void *ObjectRegistryDatabase::operator new(osulong size) throw () {
    return GlobalObjectI::operator new(size);
}

}
