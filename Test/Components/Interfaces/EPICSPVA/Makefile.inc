#############################################################
#
# Copyright 2015 F4E | European Joint Undertaking for ITER 
#  and the Development of Fusion Energy ('Fusion for Energy')
# 
# Licensed under the EUPL, Version 1.1 or - as soon they 
# will be approved by the European Commission - subsequent  
# versions of the EUPL (the "Licence"); 
# You may not use this work except in compliance with the 
# Licence. 
# You may obtain a copy of the Licence at: 
#  
# http://ec.europa.eu/idabc/eupl
#
# Unless required by applicable law or agreed to in 
# writing, software distributed under the Licence is 
# distributed on an "AS IS" basis, 
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either 
# express or implied. 
# See the Licence for the specific language governing 
# permissions and limitations under the Licence. 
#
# $Id: Makefile.inc 3 2012-01-15 16:26:07Z aneto $
#
#############################################################

OBJSX +=  EPICSPVStructureDataITest.x
		
PACKAGE=Components/Interfaces
ROOT_DIR=../../../..
MAKEDEFAULTDIR=$(MARTe2_DIR)/MakeDefaults

include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

ifdef CODAC_ROOT
PVDATA = $(EPICS_BASE)
PVACCESS = $(EPICS_BASE)
PVACLIENT = $(EPICS_BASE)
PVCOMMON = $(EPICS_BASE)
PVDATABASE = $(EPICS_BASE)
endif

INCLUDES += -I$(PVDATA)/include/
INCLUDES += -I$(PVACCESS)/include/
INCLUDES += -I$(PVCOMMON)/include/
INCLUDES += -I$(PVDATABASE)/include/
INCLUDES += -I$(EPICS_BASE)/include/
INCLUDES += -I$(EPICS_BASE)/include/os/Linux/
INCLUDES += -I$(EPICS_BASE)/include/compiler/gcc/
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L0Types
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L1Portability
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L2Objects
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L3Streams
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L4Configuration
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L4Messages
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L4Logger
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L5GAMs
INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L1Portability
INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L3Services
INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L4Messages
INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L4StateMachine
INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L4LoggerService
INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L5GAMs
INCLUDES += -I$(MARTe2_DIR)/Source/Core/FileSystem/L1Portability

INCLUDES += -I../../../../Source/Components/Interfaces/EPICSPVA

all: $(OBJS) \
                $(BUILD_DIR)/EPICSPVATest$(LIBEXT)
	echo  $(OBJS)

include depends.$(TARGET)

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)
