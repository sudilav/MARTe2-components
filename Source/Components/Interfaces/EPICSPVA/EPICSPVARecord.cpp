/**
 * @file EPICSPVARecord.cpp
 * @brief Source file for class EPICSPVARecord
 * @date 12/06/2018
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
 * the class EPICSPVARecord (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "pv/pvData.h"
#include "pv/pvDatabase.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "EPICSPVAHelper.h"
#include "EPICSPVARecord.h"
#include "EPICSPVAStructureDataI.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
//The PVRecord has some protected members... that need to be exposed
class epicsShareClass MARTe2PVARecord: public epics::pvDatabase::PVRecord {
public:
    //POINTER_DEFINITIONS(MARTe2PVARecord);
    MARTe2PVARecord(std::string const & recordName, epics::pvData::PVStructurePtr const & pvStructure) :
            epics::pvDatabase::PVRecord(recordName, pvStructure) {
    }

    void initPvt() {
        initPVRecord();
    }
};

}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

EPICSPVARecord::EPICSPVARecord() :
        Object() {

}

EPICSPVARecord::~EPICSPVARecord() {

}

bool EPICSPVARecord::InitEPICSArrays(StructuredDataI &pvaDB) {
    bool ok = true;
    uint32 nChildren = pvaDB.GetNumberOfChildren();
    uint32 n;
    for (n = 0u; (n < nChildren) && (ok); n++) {
        if (pvaDB.MoveToChild(n)) {
            ok = InitEPICSArrays(pvaDB);
            if (ok) {
                ok = pvaDB.MoveToAncestor(1u);
            }
        }
        else {
            const char8 * const name = pvaDB.GetChildName(n);
            AnyType type = pvaDB.GetType(name);
            bool isArray = (type.GetNumberOfDimensions() > 0u);
            if (isArray) {
                isArray = (type.GetNumberOfElements(0u) > 1u);
            }
            if (isArray) {
                uint32 numberOfElements = type.GetNumberOfElements(0u);
                if (type.GetTypeDescriptor() == UnsignedInteger8Bit) {
                    InitArray<uint8>(pvaDB, name, numberOfElements);
                }
                else if (type.GetTypeDescriptor() == UnsignedInteger16Bit) {
                    InitArray<uint16>(pvaDB, name, numberOfElements);
                }
                else if (type.GetTypeDescriptor() == UnsignedInteger32Bit) {
                    InitArray<uint32>(pvaDB, name, numberOfElements);
                }
                else if (type.GetTypeDescriptor() == UnsignedInteger64Bit) {
                    InitArray<uint64>(pvaDB, name, numberOfElements);
                }
                else if (type.GetTypeDescriptor() == SignedInteger8Bit) {
                    InitArray<int8>(pvaDB, name, numberOfElements);
                }
                else if (type.GetTypeDescriptor() == SignedInteger16Bit) {
                    InitArray<int16>(pvaDB, name, numberOfElements);
                }
                else if (type.GetTypeDescriptor() == SignedInteger32Bit) {
                    InitArray<int32>(pvaDB, name, numberOfElements);
                }
                else if (type.GetTypeDescriptor() == SignedInteger64Bit) {
                    InitArray<int64>(pvaDB, name, numberOfElements);
                }
                else if (type.GetTypeDescriptor() == Float32Bit) {
                    InitArray<float32>(pvaDB, name, numberOfElements);
                }
                else if (type.GetTypeDescriptor() == Float64Bit) {
                    InitArray<float64>(pvaDB, name, numberOfElements);
                }
                else if ((type.GetTypeDescriptor().type == CArray) || (type.GetTypeDescriptor().type == BT_CCString)
                        || (type.GetTypeDescriptor().type == PCString) || (type.GetTypeDescriptor().type == SString)) {
                    Vector<StreamString> vec(numberOfElements);
                    uint32 n;
                    for (n = 0u; n < numberOfElements; n++) {
                        vec[n] = "";
                    }
                    ok = pvaDB.Write(name, vec);

                }
                else {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported type");
                }
            }
        }
    }
    return ok;
}

bool EPICSPVARecord::InitEPICSStructureArray(epics::pvData::PVStructurePtr pvaStruct) {
    uint32 i;
    uint32 nOfChildren = cdb.GetNumberOfChildren();
    bool ok = true;

    for (i = 0u; (i < nOfChildren) && (ok); i++) {
        ok = cdb.MoveToChild(i);
        StreamString typeStr;
        if (ok) {
            ok = cdb.Read("Type", typeStr);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "A Type shall be defined for every node");
            }
        }
        if (ok) {
            TypeDescriptor typeDesc = TypeDescriptor::GetTypeDescriptorFromTypeName(typeStr.Buffer());
            if (typeDesc == InvalidType) {
                const ClassRegistryItem *cri = ClassRegistryDatabase::Instance()->Find(typeStr.Buffer());
                const Introspection *intro = NULL_PTR(const Introspection *);
                ok = (cri != NULL_PTR(const ClassRegistryItem *));
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Type %s is not registered", typeStr.Buffer());
                }
                if (ok) {
                    intro = cri->GetIntrospection();
                    ok = (intro != NULL_PTR(const Introspection *));
                    if (!ok) {
                        REPORT_ERROR(ErrorManagement::ParametersError, "Type %s has no introspection", typeStr.Buffer());
                    }
                }
                if (ok) {
                    epics::pvData::PVStructureArrayPtr pvStructArr = std::dynamic_pointer_cast < epics::pvData::PVStructureArray
                            > (pvaStruct->getSubField(cdb.GetName()));
                    bool isRootArray = (pvStructArr ? true : false);
                    if (isRootArray) {
                        uint32 totalElements = GetArrayNumberOfElements(cdb);
                        pvStructArr->setCapacity(totalElements);
                        pvStructArr->append(totalElements);
                        pvStructArr->setCapacityMutable(false);
                    }
                    ok = EPICSPVAHelper::InitPVStructureArrays(intro, pvaStruct, cdb.GetName());
                }
            }
        }
        if (ok) {
            ok = cdb.MoveToAncestor(1u);
        }
    }
    return ok;
}

uint32 EPICSPVARecord::GetArrayNumberOfElements(StructuredDataI &cdb) {
    bool ok = true;
    uint32 totalElements = 0u;
    uint8 numberOfDimensions = 0u;
    uint32 *numberOfElements = NULL_PTR(uint32 *);
    AnyType arrayDescription = cdb.GetType("NumberOfElements");
    if (arrayDescription.GetDataPointer() == NULL_PTR(void *)) {
        numberOfElements = new uint32[1u];
        numberOfElements[0u] = 1u;
    }
    else {
        numberOfDimensions = arrayDescription.GetNumberOfElements(0u);
        if (numberOfDimensions < 2u) {
            numberOfElements = new uint32[1u];
            Vector<uint32> readVector(numberOfElements, numberOfDimensions);
            if (!cdb.Read("NumberOfElements", readVector)) {
                uint32 nElementsRead = 1u;
                ok = cdb.Read("NumberOfElements", nElementsRead);
                if (ok) {
                    if (nElementsRead > 1u) {
                        numberOfDimensions = 1u;
                    }
                    numberOfElements[0u] = nElementsRead;
                }
            }
            if (numberOfDimensions == 1u) {
                if (numberOfElements[0u] == 1u) {
                    numberOfDimensions = 0u;
                }
            }
        }
        else {
            numberOfElements = new uint32[numberOfDimensions];
            Vector<uint32> readVector(numberOfElements, numberOfDimensions);
            ok = cdb.Read("NumberOfElements", readVector);
        }
    }
    if (ok) {
        uint32 nd;
        for (nd = 0u; nd < numberOfDimensions; nd++) {
            totalElements += numberOfElements[nd]; //[1][1][1] has size 3x
        }
        if (totalElements == 0u) {
            totalElements = numberOfElements[0U];
        }
        if (totalElements == 0u) {
            totalElements = 1u;
        }
    }
    if (numberOfElements != NULL_PTR(uint32 *)) {
        delete[] numberOfElements;
    }
    return totalElements;
}

bool EPICSPVARecord::GetEPICSStructure(epics::pvData::FieldBuilderPtr &fieldBuilder) {
    uint32 i;
    uint32 nOfChildren = cdb.GetNumberOfChildren();
    bool ok = true;

    for (i = 0u; (i < nOfChildren) && (ok); i++) {
        uint32 totalElements = 0u;
        ok = cdb.MoveToChild(i);
        StreamString typeStr;
        if (ok) {
            ok = cdb.Read("Type", typeStr);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "A Type shall be defined for every node");
            }
        }
        if (ok) {
            totalElements = GetArrayNumberOfElements(cdb);
        }
        TypeDescriptor typeDesc = TypeDescriptor::GetTypeDescriptorFromTypeName(typeStr.Buffer());
        if (typeDesc == InvalidType) {
            const ClassRegistryItem *cri = ClassRegistryDatabase::Instance()->Find(typeStr.Buffer());
            const Introspection *intro = NULL_PTR(const Introspection *);
            ok = (cri != NULL_PTR(const ClassRegistryItem *));
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Type %s is not registered", typeStr.Buffer());
            }
            if (ok) {
                intro = cri->GetIntrospection();
                ok = (intro != NULL_PTR(const Introspection *));
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Type %s has no introspection", typeStr.Buffer());
                }
            }
            if (ok) {
                epics::pvData::StructureConstPtr structPtr = EPICSPVAHelper::GetStructure(intro, typeStr.Buffer());
                ok = (structPtr ? true : false);
                if (ok) {
                    if (totalElements > 1u) {
                        fieldBuilder = fieldBuilder->addNestedStructureArray(cdb.GetName());
                        fieldBuilder = fieldBuilder->setId(typeStr.Buffer());
                    }
                    fieldBuilder = fieldBuilder->add(cdb.GetName(), structPtr);
                    if (totalElements > 1u) {
                        fieldBuilder = fieldBuilder->endNested();
                    }
                }
            }
        }
        else {
            epics::pvData::ScalarType epicsType;
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
            else if ((typeDesc.type == CArray) || (typeDesc.type == BT_CCString) || (typeDesc.type == PCString)
                    || (typeDesc.type == SString)) {
                epicsType = epics::pvData::pvString;
            }
            else {
                ok = false;
                REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported type %s", typeStr.Buffer());
            }
            if (ok) {
                if (totalElements > 1u) {
                    fieldBuilder = fieldBuilder->addBoundedArray(cdb.GetName(), epicsType, totalElements);
                }
                else {
                    fieldBuilder = fieldBuilder->add(cdb.GetName(), epicsType);
                }
            }
        }
        if (ok) {
            ok = cdb.MoveToAncestor(1u);
        }

    }
    return ok;
}

bool EPICSPVARecord::Initialise(StructuredDataI &data) {
    bool ok = Object::Initialise(data);
    if (ok) {
        ok = data.Copy(cdb);
    }
    if (ok) {
        recordName = "";
        if (!cdb.Read("Alias", recordName)) {
            recordName = GetName();
        }
        REPORT_ERROR(ErrorManagement::Information, "Going to use the following record name: [%s]", recordName.Buffer());
        ok = cdb.MoveRelative("Structure");
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "No Structure defined");
        }
    }
    return ok;
}

epics::pvDatabase::PVRecordPtr EPICSPVARecord::CreatePVRecord() {
    epics::pvData::FieldCreatePtr fieldCreate = epics::pvData::getFieldCreate();
    epics::pvData::FieldBuilderPtr fieldBuilder = fieldCreate->createFieldBuilder();
    epics::pvData::PVStructurePtr pvStructure;
    std::shared_ptr<MARTe2PVARecord> pvRecordWrapper;
    bool ok = GetEPICSStructure(fieldBuilder);

    if (ok) {
        epics::pvData::StructureConstPtr topStructure = fieldBuilder->createStructure();
        pvStructure = epics::pvData::getPVDataCreate()->createPVStructure(topStructure);
        ok = InitEPICSStructureArray(pvStructure);

        if (ok) {
            pvRecordWrapper = std::shared_ptr < MARTe2PVARecord > (new MARTe2PVARecord(recordName.Buffer(), pvStructure));
            pvRecordWrapper->initPvt();
        }
        if (ok) {
            EPICSPVAStructureDataI cdb;
            cdb.SetStructure(pvStructure);
            ok = cdb.MoveToRoot();
            if (ok) {
                ok = InitEPICSArrays(cdb);
            }
        }
    }
    return pvRecordWrapper;

}

void EPICSPVARecord::GetRecordName(StreamString &recName) {
    recName = recordName;
}

CLASS_REGISTER(EPICSPVARecord, "1.0")
}
