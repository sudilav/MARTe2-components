/**
 * @file EPICSPVAHelper.cpp
 * @brief Source file for class EPICSPVAHelper
 * @date 20/11/2018
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
 * the class EPICSPVAHelper (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ConfigurationDatabase.h"
#include "EPICSPVAHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
const char8 * const EPICSPVAHelper::STRUCTURE_ARRAY_PARENT_ID = "_structureArrayParent";

template<typename T>
void EPICSPVAHelperInitArrayT(epics::pvData::PVScalarArrayPtr scalarArrayPtr, const uint32 &size) {
    epics::pvData::shared_vector<T> out(size);
    uint32 n;
    for (n = 0u; n < size; n++) {
        out[n] = static_cast<T>(0);
    }
    epics::pvData::shared_vector<const T> outF = freeze(out);
    scalarArrayPtr->putFrom<T>(outF);
}

template<>
void EPICSPVAHelperInitArrayT<std::string>(epics::pvData::PVScalarArrayPtr scalarArrayPtr, const uint32 &size) {
    epics::pvData::shared_vector<std::string> out(size);
    uint32 n;
    for (n = 0u; n < size; n++) {
        out[n] = "";
    }
    epics::pvData::shared_vector<const std::string> outF = freeze(out);
    scalarArrayPtr->putFrom<std::string>(outF);
}

uint32 EPICSPVAHelperGetNumberOfElements(const IntrospectionEntry &entry) {
    uint32 numberOfElements = 1u;
    uint8 nOfDimensions = entry.GetNumberOfDimensions();
    if (nOfDimensions > 0u) {
        numberOfElements = 0u;
        uint32 ne;
        for (ne = 0u; ne < nOfDimensions; ne++) {
            numberOfElements += entry.GetNumberOfElements(ne);
        }
    }
    return numberOfElements;
}

void EPICSPVAHelper::InitArray(epics::pvData::PVScalarArrayPtr pvScalarArr, const uint32 &numberOfElements) {
    bool ok = (pvScalarArr ? true : false);
    if (ok) {
        epics::pvData::ScalarType epicsType = pvScalarArr->getScalarArray()->getElementType();
        if (epicsType == epics::pvData::pvUByte) {
            EPICSPVAHelperInitArrayT<uint8>(pvScalarArr, numberOfElements);
        }
        else if (epicsType == epics::pvData::pvUShort) {
            EPICSPVAHelperInitArrayT<uint16>(pvScalarArr, numberOfElements);
        }
        else if (epicsType == epics::pvData::pvUInt) {
            EPICSPVAHelperInitArrayT<uint32>(pvScalarArr, numberOfElements);
        }
        else if (epicsType == epics::pvData::pvULong) {
            EPICSPVAHelperInitArrayT<unsigned long int>(pvScalarArr, numberOfElements);
        }
        else if (epicsType == epics::pvData::pvByte) {
            EPICSPVAHelperInitArrayT<int8>(pvScalarArr, numberOfElements);
        }
        else if (epicsType == epics::pvData::pvShort) {
            EPICSPVAHelperInitArrayT<int16>(pvScalarArr, numberOfElements);
        }
        else if (epicsType == epics::pvData::pvInt) {
            EPICSPVAHelperInitArrayT<int32>(pvScalarArr, numberOfElements);
        }
        else if (epicsType == epics::pvData::pvLong) {
            EPICSPVAHelperInitArrayT<long int>(pvScalarArr, numberOfElements);
        }
        else if (epicsType == epics::pvData::pvFloat) {
            EPICSPVAHelperInitArrayT<float32>(pvScalarArr, numberOfElements);
        }
        else if (epicsType == epics::pvData::pvDouble) {
            EPICSPVAHelperInitArrayT<float64>(pvScalarArr, numberOfElements);
        }
        else if (epicsType == epics::pvData::pvString) {
            EPICSPVAHelperInitArrayT<std::string>(pvScalarArr, numberOfElements);
        }
    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Type not supported");
    }
}

bool EPICSPVAHelper::GetType(TypeDescriptor typeDesc, epics::pvData::ScalarType &epicsType) {
    bool ok = true;
    if (typeDesc == UnsignedInteger8Bit) {
        epicsType = epics::pvData::pvUByte;
    }
    else if (typeDesc == UnsignedInteger16Bit) {
        epicsType = epics::pvData::pvUShort;
    }
    else if (typeDesc == UnsignedInteger32Bit) {
        epicsType = epics::pvData::pvUInt;
    }
    else if (typeDesc == UnsignedInteger64Bit) {
        epicsType = epics::pvData::pvULong;
    }
    else if (typeDesc == SignedInteger8Bit) {
        epicsType = epics::pvData::pvByte;
    }
    else if (typeDesc == SignedInteger16Bit) {
        epicsType = epics::pvData::pvShort;
    }
    else if (typeDesc == SignedInteger32Bit) {
        epicsType = epics::pvData::pvInt;
    }
    else if (typeDesc == SignedInteger64Bit) {
        epicsType = epics::pvData::pvLong;
    }
    else if (typeDesc == Float32Bit) {
        epicsType = epics::pvData::pvFloat;
    }
    else if (typeDesc == Float64Bit) {
        epicsType = epics::pvData::pvDouble;
    }
    else if ((typeDesc.type == CArray) || (typeDesc.type == BT_CCString) || (typeDesc.type == PCString) || (typeDesc.type == SString)) {
        epicsType = epics::pvData::pvString;
    }
    else {
        ok = false;
        REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Unsupported type");
    }
    return ok;
}

bool EPICSPVAHelper::GetType(const char8 * const memberTypeName, epics::pvData::ScalarType &epicsType) {
    TypeDescriptor typeDesc = TypeDescriptor::GetTypeDescriptorFromTypeName(memberTypeName);
    return GetType(typeDesc, epicsType);
}

epics::pvData::StructureConstPtr EPICSPVAHelperTransverseStructure(const Introspection *intro, const char8 * const typeName, bool addParentField = false) {
    epics::pvData::FieldCreatePtr fieldCreate = epics::pvData::getFieldCreate();
    epics::pvData::FieldBuilderPtr fieldBuilder = fieldCreate->createFieldBuilder();
    epics::pvData::StructureConstPtr topStructure;
    bool ok = (intro != NULL_PTR(const Introspection *));
    if (ok) {
        if (typeName != NULL_PTR(const char8 * const)) {
            fieldBuilder = fieldBuilder->setId(typeName);
        }
        uint32 numberOfMembers = intro->GetNumberOfMembers();
        uint32 i;
        //For each of the structure members...
        for (i = 0u; (i < numberOfMembers) && (ok); i++) {
            //lint -e{613} intro cannot be NULL as it is checked above.
            const IntrospectionEntry entry = intro->operator[](i);
            const char8 * const memberName = entry.GetMemberName();
            const char8 * const memberTypeName = entry.GetMemberTypeName();
            bool isStructured = entry.GetMemberTypeDescriptor().isStructuredData;
            uint32 numberOfElements = EPICSPVAHelperGetNumberOfElements(entry);
            if (isStructured) {
                const ClassRegistryItem *cri = ClassRegistryDatabase::Instance()->Find(memberTypeName);
                ok = (cri != NULL_PTR(const ClassRegistryItem *));
                if (ok) {
                    bool isArray = (numberOfElements > 1u);
                    if (isArray) {
                        fieldBuilder = fieldBuilder->addArray(memberName, EPICSPVAHelperTransverseStructure(cri->GetIntrospection(), memberTypeName));
                    }
                    else {
                        fieldBuilder = fieldBuilder->add(memberName, EPICSPVAHelperTransverseStructure(cri->GetIntrospection(), memberTypeName));
                    }
                }
                else {
                    REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Type %s is not registered", memberTypeName);
                }
            }
            else {
                epics::pvData::ScalarType epicsType;
                ok = EPICSPVAHelper::GetType(memberTypeName, epicsType);
                if (ok) {
                    if (numberOfElements > 1u) {
                        fieldBuilder = fieldBuilder->addBoundedArray(memberName, epicsType, numberOfElements);
                    }
                    else {
                        fieldBuilder = fieldBuilder->add(memberName, epicsType);
                    }
                }
            }
        }
    }
    if (ok) {
        if (addParentField) {
            if (sizeof(void *) == 8u) {
                fieldBuilder = fieldBuilder->add(EPICSPVAHelper::STRUCTURE_ARRAY_PARENT_ID, epics::pvData::pvULong);
            }
            else {
                fieldBuilder = fieldBuilder->add(EPICSPVAHelper::STRUCTURE_ARRAY_PARENT_ID, epics::pvData::pvUInt);
            }
        }
        topStructure = fieldBuilder->createStructure();
    }
    return topStructure;
}

bool EPICSPVAHelperTransverseStructureInit(const Introspection *intro, epics::pvData::PVStructurePtr pvStructPtr) {
    bool ok = (intro != NULL_PTR(const Introspection *));

    uint32 numberOfMembers = intro->GetNumberOfMembers();
    uint32 i;
    //For each of the structure members...
    for (i = 0u; (i < numberOfMembers) && (ok); i++) {
        //lint -e{613} intro cannot be NULL as it is checked above.
        const IntrospectionEntry entry = intro->operator[](i);
        const char8 * const memberName = entry.GetMemberName();
        const char8 * const memberTypeName = entry.GetMemberTypeName();
        uint32 numberOfElements = EPICSPVAHelperGetNumberOfElements(entry);
        bool isStructured = entry.GetMemberTypeDescriptor().isStructuredData;
        if (isStructured) {
            const ClassRegistryItem *cri = ClassRegistryDatabase::Instance()->Find(memberTypeName);
            const Introspection *memberIntro = cri->GetIntrospection();
            ok = (cri != NULL_PTR(const ClassRegistryItem *));
            if (ok) {
                ok = (memberIntro != NULL_PTR(const Introspection *));
            }
            if (ok) {
                if (numberOfElements > 1u) {
                    epics::pvData::PVStructureArrayPtr pvStructMemberArr = pvStructPtr->getSubField<epics::pvData::PVStructureArray>(memberName);
                    ok = (pvStructMemberArr ? true : false);
                    if (ok) {
                        ok = EPICSPVAHelper::ReplaceStructureArray(memberIntro, pvStructMemberArr, numberOfElements, memberTypeName);
                    }
                    else {
                        REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Member %s not found in PVStructurePtr or is not a PVStructureArrayPtr", memberName);
                    }
                }
                else {
                    epics::pvData::PVStructurePtr pvStructMember = pvStructPtr->getSubField<epics::pvData::PVStructure>(memberName);
                    ok = (pvStructMember ? true : false);
                    if (ok) {
                        ok = EPICSPVAHelperTransverseStructureInit(memberIntro, pvStructMember);
                    }
                    else {
                        REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Member %s not found in PVStructurePtr", memberName);
                    }
                }
            }
            else {
                REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Type %s is not registered", memberTypeName);
            }
        }
        else {
            if (numberOfElements > 1u) {
                epics::pvData::ScalarType epicsType;
                ok = EPICSPVAHelper::GetType(memberTypeName, epicsType);
                if (!ok) {
                    REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Member %s not found in PVScalarArrayPtr", memberName);
                }
                epics::pvData::PVScalarArrayPtr pvScalarArr = pvStructPtr->getSubField<epics::pvData::PVScalarArray>(memberName);
                if (ok) {
                    ok = (pvScalarArr ? true : false);
                }
                if (ok) {
                    EPICSPVAHelper::InitArray(pvScalarArr, numberOfElements);
                }
            }
        }
    }
    return ok;
}
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

void EPICSPVAHelper::REPORT_ERROR_PVA_STRUCT(ErrorManagement::ErrorType err, epics::pvData::StructureConstPtr pvaStruct) {
    StreamString debugStr;
    std::stringstream tmpStr;
    tmpStr << pvaStruct;
    REPORT_ERROR_STATIC(err, "%s", tmpStr.str().c_str());
}

epics::pvData::StructureConstPtr EPICSPVAHelper::GetStructure(const Introspection *intro, const char8 * const typeName) {
    epics::pvData::StructureConstPtr fullStructure = EPICSPVAHelperTransverseStructure(intro, typeName);
    REPORT_ERROR_PVA_STRUCT(ErrorManagement::Debug, fullStructure);
    return fullStructure;
}

bool EPICSPVAHelper::InitStructure(const Introspection *intro, epics::pvData::PVStructurePtr pvStructPtr) {
    bool ok = (pvStructPtr ? true : false);
    if (ok) {
        ok = EPICSPVAHelperTransverseStructureInit(intro, pvStructPtr);
        REPORT_ERROR_PVA_STRUCT(ErrorManagement::Debug, pvStructPtr->getStructure());
    }
    return ok;
}

bool EPICSPVAHelper::ReplaceStructureArray(const Introspection *memberIntro, epics::pvData::PVStructureArrayPtr pvStructMemberArr, uint32 numberOfElements, const char8 * const memberTypeName) {
    uint32 j;
    epics::pvData::PVStructureArray::svector arr(numberOfElements);
    uint32 arrSize = arr.size();
    bool ok = (pvStructMemberArr ? true : false);
    for (j = 0u; (j < arrSize) && (ok); j++) {
        arr[j] = epics::pvData::getPVDataCreate()->createPVStructure(EPICSPVAHelperTransverseStructure(memberIntro, memberTypeName, true));
        ok = EPICSPVAHelperTransverseStructureInit(memberIntro, arr[j]);
        if (ok) {
            epics::pvData::PVScalarPtr parentField = arr[j]->getSubField<epics::pvData::PVScalar>(EPICSPVAHelper::STRUCTURE_ARRAY_PARENT_ID);
            if (parentField ? true : false) {
                const epics::pvData::PVStructure * parent = pvStructMemberArr->getParent();
                if (parent != NULL_PTR(const epics::pvData::PVStructure *)) {
                    unsigned long int paddr64 = reinterpret_cast<unsigned long int>(parent);
                    parentField->putFrom<unsigned long int>(paddr64);
                }
                else {
                    parentField->putFrom<uint32>(0u);
                }
            }
        }
    }
    if (ok) {
        pvStructMemberArr->replace(freeze(arr));
    }
    return ok;
}
}

