/**
 * @file RealTimeApplicationConfigurationBuilder.h
 * @brief Header file for class RealTimeApplicationConfigurationBuilder
 * @date 23/06/2016
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

 * @details This header file contains the declaration of the class RealTimeApplicationConfigurationBuilder
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REALTIMEAPPLICATIONCONFIGURATIONBUILDER_H_
#define REALTIMEAPPLICATIONCONFIGURATIONBUILDER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief Helper class which expands and parses a RealTimeApplication configuration database.
 * @details This class takes a RealTimeApplication ConfigurationDatabase as defined by the
 * application user and expands into a fully defined and consistent ConfigurationDatabase that
 * can be used by the MARTe L5GAMs classes to build an application.
 *
 * In particular this class takes cares of: expanding all the nested introspection in both the
 * GAMs (i.e. Functions) and DataSources; completing the missing details in any of the signals
 * and verifying if all the GAM and DataSource signals definition is consistent.
 *
 * The expanded information is stored in two ConfigurationDatabase instances. One for the Functions and
 * another for the DataSources.
 *
 * The schema used below to define the pre and post condition is
 * &NAME = VALUE
 * Where VALUE can be a node {} or can be a string representing a property
 * The & can be replaced by:
 *   an empty character, i.e. NAME = VALUE, => the NAME shall exist
 *   +, i.e. NAME+ = VALUE, => the NAME may exist
 *   *, i.e. NAME* = VALUE, => a list with at least one NAME shall exist. If NAME=NUMBER then the NAME must be a number.
 *   +*, i.e. NAME+* = VALUE, the NAME may exist. If it exists the * rules apply.
 *   |, i.e NAME|NAME = VALUE, one and only one of the specified NAME shall exist.
 *   +|, i.e NAME|NAME = VALUE, the NAME may exist. If it exists the | rules apply.
 */
class DLL_API RealTimeApplicationConfigurationBuilder {
public:

    /**
     * @brief Default constructor. Sets the RealTimeApplication
     * @param[in] realTimeApplicationIn the RealTimeApplication associated to this RealTimeApplicationConfigurationBuilder.
     * @param[in] defaultDataSourceNameIn default DataSource name to be used when the DataSource in not defined in any of the signals.
     * @post
     *   realTimeApplication = realTimeApplicationIn
     *   defaultDataSourceName = defaultDataSourceNameIn
     */
    RealTimeApplicationConfigurationBuilder(ReferenceT<ReferenceContainer> realTimeApplicationIn,
                                            const char8 *defaultDataSourceNameIn);

    /**
     * @brief Adds all the GAM signals to the Functions database and adds all the DataSource signals to the DataSource database.
     * @details Looks for all the GAM instances and calls GAM::AddSignals. The GAM name is stored as a qualified name (i.e. each recursion into
     * the configuration tree is separated by a point and added to the GAM name). The same action is performed for each DataSource instance.
     *
     * Each GAM/DataSource that is added, is stored under the Functions/Data nodes and identified with an unique number.
     * @return true if all the GAM::AddSignals and all the DataSource::AddSignals were successful.
     * @pre
     *   functionsDatabase = ""
     *   dataSourcesDatabase = ""
     * @post
     *   functionsDatabase =
     *     Functions = {
     *       *NUMBER = {
     *         QualifiedName = "x.y.GAMNAME"
     *         Signals = {
     *            InputSignals|OutputSignals = {
     *               NAME*={
     *                 +Alias = "Path.In.Data.Source (Otherwise SignalName = NAME)"
     *                 +DataSource = "QualifiedName of the DataSource"
     *                 +Type = BasicType|StructuredType
     *                 +NumberOfDimensions = 0|1|2
     *                 +NumberOfElements = NUMBER>0
     *                 +Ranges = {{min_idx:max_idx} {min_idx:max_idx} ...} (min_idx<=max_idx indexes may not overlap)
     *                 +TimeCyclesSamples = {x y}, where x is the number o samples for each cycle and y the number of cycles covered by the data
     *                                      {1,1} => same as cycle time, {1,>1} => over-sampling, {>1,1) => sub-sampling, {-1,1} => asynchronous (i.e.
     *                                      not related to cycle time), {N,N>1} => N history samples, {-1,N} => history of asynchronous data
     *               }
     *            }
     *         }
     *       }
     *    }
     *    dataSourcesDatabase =
     *      Data = {
     *        *NUMBER = {
     *          QualifiedName = "x.y.DATASOURCENAME"
     *          Signals = {
     *            +*NAME = {
     *              +Type = BasicType|StructuredType
     *              +NumberOfDimensions = 0|1|2
     *              +NumberOfElements = NUMBER>0
     *            }
     *          }
     *        }
     *      }
     */
    bool InitialiseSignalsDatabase();

    /**
     * @brief Flattens the Functions and DataSource databases.
     * @details All the introspection elements are expanded and the signals are flatten, so that nested structured
     * signals are stored with a QualifiedName which includes the full path to the signal. After this stage the
     * DataSource for all the Function signals must be known. If it not specified the defaultDataSource is assumed.
     * @return true if all the signals can be successfully flatten and if the DataSource is known for all the GAM signals.
     * @pre
     *   InitialiseSignalsDatabase()
     * @post
     *   functionsDatabase =
     *     Functions = {
     *       *NUMBER = {
     *         QualifiedName = "x.y.GAMNAME"
     *         Signals = {
     *            InputSignals|OutputSignals = {
     *               *NUMBER = {
     *                 QualifiedName = "QualifiedName of the signal"
     *                 DataSource = "QualifiedName of the DataSource"
     *                 +Alias = "Path.In.Data.Source (Otherwise SignalName = NAME)"
     *                 +Type = BasicType
     *                 +NumberOfDimensions = 0|1|2
     *                 +NumberOfElements = NUMBER>0
     *                 +Ranges = {{min_idx:max_idx} {min_idx:max_idx} ...} (min_idx<=max_idx indexes may not overlap)
     *                 +TimeCyclesSamples = {x y}
     *               }
     *            }
     *         }
     *       }
     *    }
     *    dataSourcesDatabase =
     *      Data = {
     *        *NUMBER = {
     *          QualifiedName = "x.y.DATASOURCENAME"
     *          Signals = {
     *            +*NUMBER ={
     *              QualifiedName = "QualifiedName of the signal"
     *              +Type = BasicType
     *              +NumberOfDimensions = 0|1|2
     *              +NumberOfElements = NUMBER>0
     *            }
     *          }
     *        }
     *      }
     */
    bool FlattenSignalsDatabases();

    /**
     * @brief Merges all the signals from the Functions into the corresponding DataSource.
     * @details For each signal (where the Type is defined) in every function, merge the signal to the corresponding DataSource.
     * If the signal does not exist in the DataSource it will be created.
     * Every signal property will be copied from the function signal definition into the DataSource. If the signal property already exists in
     *  the DataSource, the two properties must be consistent.
     * @return true if all the signals from the Functions can be merged into the corresponding DataSource.
     * @pre
     *   FlattenSignalsDatabases()
     * @post
     *   functionsDatabase =
     *     Functions = {
     *       *NUMBER = {
     *         QualifiedName = "x.y.GAMNAME"
     *         Signals = {
     *            InputSignals|OutputSignals = {
     *               *NUMBER = {
     *                 QualifiedName = "QualifiedName of the signal"
     *                 DataSource = "QualifiedName of the DataSource"
     *                 +Alias = "Path.In.Data.Source (Otherwise SignalName = NAME)"
     *                 +Type = BasicType
     *                 +NumberOfDimensions = 0|1|2
     *                 +NumberOfElements = NUMBER>0
     *                 +Ranges = {{min_idx:max_idx} {min_idx:max_idx} ...} (min_idx<=max_idx indexes may not overlap)
     *                 +TimeCyclesSamples = {x y}
     *               }
     *            }
     *         }
     *       }
     *    }
     *    dataSourcesDatabase =
     *      Data = {
     *        *NUMBER = {
     *          QualifiedName = "x.y.DATASOURCENAME"
     *          Signals = {
     *            *NUMBER = {
     *              QualifiedName = "QualifiedName of the signal"
     *              Type = BasicType
     *              +NumberOfDimensions = 0|1|2
     *              +NumberOfElements = NUMBER>0
     *            }
     *          }
     *        }
     *      }
     */
    bool ResolveDataSources();

    /**
     * @brief Verify that for every signal in every DataSource, the Type is defined.
     * @details Add the default values for NumberOfElements=+1 and NumberOfDimensions=+0 if needed.
     * @return true if the Type is defined for all the signals in all the DataSource elements.
     * @pre
     *   ResolveDataSources()
     * @post
     *   functionsDatabase =
     *     Functions = {
     *       *NUMBER = {
     *         QualifiedName = "x.y.GAMNAME"
     *         Signals = {
     *            InputSignals|OutputSignals = {
     *               *NUMBER ={
     *                 QualifiedName = "QualifiedName of the signal"
     *                 DataSource = "QualifiedName of the DataSource"
     *                 +Alias = "Path.In.Data.Source (Otherwise SignalName = NAME)"
     *                 +Type = BasicType
     *                 +NumberOfDimensions = 0|1|2
     *                 +NumberOfElements = NUMBER>0
     *                 +Ranges = {{min_idx:max_idx} {min_idx:max_idx} ...} (min_idx<=max_idx indexes may not overlap)
     *                 +TimeCyclesSamples = {x y}
     *               }
     *            }
     *         }
     *       }
     *    }
     *    dataSourcesDatabase =
     *      Data = {
     *       *NUMBER = {
     *          QualifiedName = "x.y.DATASOURCENAME"
     *          Signals = {
     *            *NUMBER = {
     *              QualifiedName = "QualifiedName of the signal"
     *              Type = BasicType
     *              NumberOfDimensions = 0|1|2
     *              NumberOfElements = NUMBER>0
     *              ByteSize = NUMBER > 0
     *            }
     *          }
     *        }
     *      }
     */
    bool VerifyDataSourcesSignals();

    /**
     * @brief Final merge of signals from the DataSources to the Functions.
     * @details For every signal in every Function merges it with the corresponding DataSource signal.
     * If the signal type was not originally defined in the Function it will get defined by the DataSource.
     * If incompatibilities are found in the signal definition in the DataSource or in the Function an error will be returned.
     * If the signal name is a subset of a nested structure (e.g. A.B in a structure that has A.B.C and A.B.D defined) all the subset will be copied (both
     * A.B.C and A.B.D in the previous example).
     * @return true if all the signals can be successfully merged.
     * @pre
     *   VerifyDataSourcesSignals()
     * @post
     *   functionsDatabase =
     *     Functions = {
     *       *NUMBER = {
     *         QualifiedName = "x.y.GAMNAME"
     *         Signals = {
     *            InputSignals|OutputSignals = {
     *               *NUMBER = {
     *                 QualifiedName = "QualifiedName of the signal"
     *                 DataSource = "QualifiedName of the DataSource"
     *                 +Alias = "Path.In.Data.Source (Otherwise SignalName = NAME)"
     *                 Type = BasicType
     *                 NumberOfDimensions = 0|1|2
     *                 NumberOfElements = NUMBER>0
     *                 +Ranges = {{min_idx:max_idx} {min_idx:max_idx} ...} (min_idx<=max_idx indexes may not overlap)
     *                 +TimeCyclesSamples = {x y}
     *               }
     *            }
     *         }
     *       }
     *    }
     *    dataSourcesDatabase =
     *      Data = {
     *        *NUMBER = {
     *          QualifiedName = "x.y.DATASOURCENAME"
     *          Signals = {
     *            *NUMBER = {
     *              QualifiedName = "QualifiedName of the signal"
     *              Type = BasicType
     *              NumberOfDimensions = 0|1|2
     *              NumberOfElements = NUMBER>0
     *            }
     *          }
     *        }
     *      }
     */
    bool ResolveFunctionSignals();

    /**
     * @brief Verifies that for every signal in every function all the properties are defined.
     * @return true if the Type, the NumberOfDimensions and the NumberOfElements are defined for every signal in every Function.
     * @pre
     *   ResolveFunctionSignals()
     * @post
     *   functionsDatabase =
     *     Functions = {
     *       *NUMBER = {
     *         QualifiedName = "x.y.GAMNAME"
     *         Signals = {
     *            InputSignals|OutputSignals = {
     *               *NUMBER = {
     *                 QualifiedName = "QualifiedName of the signal"
     *                 DataSource = "QualifiedName of the DataSource"
     *                 +Alias = "Path.In.Data.Source (Otherwise SignalName = NAME)"
     *                 Type = BasicType
     *                 NumberOfDimensions = 0|1|2
     *                 NumberOfElements = NUMBER>0
     *                 +Ranges = {{min_idx:max_idx} {min_idx:max_idx} ...} (min_idx<=max_idx indexes may not overlap)
     *                 +TimeCyclesSamples = {x y}
     *               }
     *            }
     *         }
     *       }
     *    }
     *    dataSourcesDatabase =
     *      Data = {
     *        *NUMBER = {
     *          QualifiedName = "x.y.DATASOURCENAME"
     *          Signals = {
     *            *NUMBER = {
     *              QualifiedName = "QualifiedName of the signal"
     *              Type = BasicType
     *              NumberOfDimensions = 0|1|2
     *              NumberOfElements = NUMBER>0
     *            }
     *          }
     *        }
     *      }
     */
    bool VerifyFunctionSignals();

    /**
     * @brief Declare all the states where a given Function is executed.
     * @details Add to the configuration Tree a States node holding all the states where a Function is executed.
     * @return true if for all the functions, all the states can be successfully added.
     * @pre
     *   VerifyFunctionSignals()
     * @post
     *   functionsDatabase =
     *     Functions = {
     *       *NUMBER = {
     *         QualifiedName = "x.y.GAMNAME"
     *         Signals = {
     *            InputSignals|OutputSignals = {
     *               *NUMBER = {
     *                 QualifiedName = "QualifiedName of the signal"
     *                 DataSource = "QualifiedName of the DataSource"
     *                 +Alias = "Path.In.Data.Source (Otherwise SignalName = NAME)"
     *                 Type = BasicType
     *                 NumberOfDimensions = 0|1|2
     *                 NumberOfElements = NUMBER>0
     *                 +Ranges = {{min_idx:max_idx} {min_idx:max_idx} ...} (min_idx<=max_idx indexes may not overlap)
     *                 +TimeCyclesSamples = {x y}
     *               }
     *            }
     *         }
     *         States = {
     *           +*StateNameN = "ThreadNameN"
     *         }
     *       }
     *    }
     *    dataSourcesDatabase =
     *      Data = {
     *        *NUMBER = {
     *          QualifiedName = "x.y.DATASOURCENAME"
     *          Signals = {
     *            *NUMBER = {
     *              QualifiedName = "QualifiedName of the signal"
     *              Type = BasicType
     *              NumberOfDimensions = 0|1|2
     *              NumberOfElements = NUMBER>0
     *            }
     *          }
     *        }
     *      }
     */
    bool ResolveStates();

    /**
     * @brief For every signal in every DataSource set the Consumer and Producer Functions.
     * @return true if for every signal in every DataSource the Consumer and Producer Functions can be successfully added.
     * @pre
     *   ResolveStates()
     * @post
     *   functionsDatabase =
     *     Functions = {
     *       *NUMBER = {
     *         QualifiedName = "x.y.GAMNAME"
     *         Signals = {
     *            InputSignals|OutputSignals = {
     *              *NUMBER = {
     *                 QualifiedName = "QualifiedName of the signal"
     *                 DataSource = "QualifiedName of the DataSource"
     *                 +Alias = "Path.In.Data.Source (Otherwise SignalName = NAME)"
     *                 Type = BasicType
     *                 NumberOfDimensions = 0|1|2
     *                 NumberOfElements = NUMBER>0
     *                 +Ranges = {{min_idx:max_idx} {min_idx:max_idx} ...} (min_idx<=max_idx indexes may not overlap) (max_idx<NumberOfElements)
     *                 +TimeCyclesSamples = {x y}
     *               }
     *            }
     *         }
     *         States = {
     *           +*StateNameN = "ThreadNameN"
     *         }
     *       }
     *    }
     *    dataSourcesDatabase =
     *      Data = {
     *        *NUMBER = {
     *          QualifiedName = "x.y.DATASOURCENAME"
     *          Signals = {
     *            *NUMBER = {
     *              QualifiedName = "QualifiedName of the signal"
     *              Type = BasicType
     *              NumberOfDimensions = 0|1|2
     *              NumberOfElements = NUMBER>0
     *              +States = {
     *                *StateN = {
     *                  +Consumers = { "FunctionX" ... "FunctionZ" }
     *                  +Producers = { "FunctionX" ... "FunctionZ" }
     *                }
     *              }
     *            }
     *          }
     *        }
     *      }
     */
    bool ResolveConsumersAndProducers();

    /**
     * @brief For every signal in every Function compute the memory size and the memory offset (if Ranges are defined).
     * @details The memory size is given by sizeof(Type) multiplied by all the Ranges. Illegal Ranges definitions (see post condition).
     * are trapped at this stage.
     * If the Ranges are defined, the memory offset is also computed and stored directly in bytes.
     * @return true if all the Ranges definitions are correctly defined and if the memory size can be successfully computed.
     * @pre
     *   ResolveConsumersAndProducers()
     * @post
     *   functionsDatabase =
     *     Functions = {
     *       *NUMBER = {
     *         QualifiedName = "x.y.GAMNAME"
     *         Signals = {
     *            InputSignals|OutputSignals = {
     *               *NUMBER = {
     *                 QualifiedName = "QualifiedName of the signal"
     *                 DataSource = "QualifiedName of the DataSource"
     *                 +Alias = "Path.In.Data.Source (Otherwise SignalName = NAME)"
     *                 Type = BasicType
     *                 NumberOfDimensions = 0|1|2
     *                 NumberOfElements = NUMBER>0
     *                 +Ranges = {{min_idx:max_idx} {min_idx:max_idx} ...} (min_idx<=max_idx indexes may not overlap) (max_idx<NumberOfElements)
     *                 ByteSize = NUMBER > 0
     *                 +ByteOffset = { { min_idx_bytes range_bytes } { min_idx_bytes range_bytes } ...}
     *                 +TimeCyclesSamples = {x y}
     *               }
     *            }
     *         }
     *         States = {
     *           +*StateNameN = "ThreadNameN"
     *         }
     *       }
     *    }
     *    dataSourcesDatabase =
     *      Data = {
     *        *NUMBER = {
     *          QualifiedName = "x.y.DATASOURCENAME"
     *          Signals = {
     *            *NUMBER = {
     *              QualifiedName = "QualifiedName of the signal"
     *              Type = BasicType
     *              NumberOfDimensions = 0|1|2
     *              NumberOfElements = NUMBER>0
     *              +States = {
     *                *StateN = {
     *                  +Consumers = { "FunctionX" ... "FunctionZ" }
     *                  +Producers = { "FunctionX" ... "FunctionZ" }
     *                }
     *              }
     *            }
     *          }
     *        }
     *      }
     */
    bool ResolveFunctionSignalsMemorySize();

    /**
     * @brief For every Function compute the memory size and the memory offset for each DataSource which interfaces to the Function.
     * @details Computes the amount of memory grouped by DataSource and includes any signal specific ByteOffset information.
     * @return true if the total amount of memory can be successfully computed.
     * @pre
     *   ResolveFunctionSignalsMemorySize()
     * @post
     *   functionsDatabase =
     *     Functions = {
     *       *NUMBER = {
     *         QualifiedName = "x.y.GAMNAME"
     *         Signals = {
     *            InputSignals|OutputSignals = {
     *               *NUMBER  ={
     *                 QualifiedName = "QualifiedName of the signal"
     *                 DataSource = "QualifiedName of the DataSource"
     *                 +Alias = "Path.In.Data.Source (Otherwise SignalName = NAME)"
     *                 Type = BasicType
     *                 NumberOfDimensions = 0|1|2
     *                 NumberOfElements = NUMBER>0
     *                 +Ranges = {{min_idx:max_idx} {min_idx:max_idx} ...} (min_idx<=max_idx indexes may not overlap) (max_idx<NumberOfElements)
     *                 ByteSize = NUMBER>0
     *                 +ByteOffset = { { min_idx_bytes range_bytes } { min_idx_bytes range_bytes } ...}
     *                 +TimeCyclesSamples = {x y}
     *               }
     *            }
     *         }
     *         States = {
     *           +*StateNameN = "ThreadNameN"
     *         }
     *         Memory = {
     *           InputSignals|OutputSignals = {
     *             +*NUMBER = {
     *               DataSource = "QualifiedName of the DataSource"
     *               ByteSize = NUMBER>0
     *               Signals = {
     *                 *NUMBER = {
     *                   QualifiedName = "QualifiedName of the Signal"
     *                   +ByteOffset = { { min_idx_bytes range_bytes } { min_idx_bytes range_bytes } ... }
     *                   +TimeCyclesSamples = {x y}
     *                 }
     *               }
     *             }
     *           }
     *         }
     *       }
     *    }
     *    dataSourcesDatabase =
     *      Data = {
     *        *NUMBER = {
     *          QualifiedName = "x.y.DATASOURCENAME"
     *          Signals = {
     *            *NUMBER = {
     *              QualifiedName = "QualifiedName of the signal"
     *              Type = BasicType
     *              NumberOfDimensions = 0|1|2
     *              NumberOfElements = NUMBER>0
     *              +States = {
     *                *StateN = {
     *                  +Consumers = { "FunctionX" ... "FunctionZ" }
     *                  +Producers = { "FunctionX" ... "FunctionZ" }
     *                }
     *              }
     *            }
     *          }
     *        }
     *      }
     */
    bool ResolveFunctionsMemory();

    /**
     * @brief For every DataSource add the memory information about each Function interacts with the DataSource.
     * @return true if the memory information can be successfully added to each DataSource definition.
     * @pre
     *   ResolveFunctionsMemory()
     * @post
     *   functionsDatabase =
     *     Functions = {
     *       *NUMBER = {
     *         QualifiedName = "x.y.GAMNAME"
     *         Signals = {
     *            InputSignals|OutputSignals = {
     *               *NUMBER = {
     *                 QualifiedName = "QualifiedName of the signal"
     *                 DataSource = "QualifiedName of the DataSource"
     *                 +Alias = "Path.In.Data.Source (Otherwise SignalName = NAME)"
     *                 Type = BasicType
     *                 NumberOfDimensions = 0|1|2
     *                 NumberOfElements = NUMBER>0
     *                 +Ranges = {{min_idx:max_idx} {min_idx:max_idx} ...} (min_idx<=max_idx indexes may not overlap) (max_idx<NumberOfElements)
     *                 ByteSize = NUMBER>0
     *                 ByteOffset = { { min_idx_bytes range_bytes } { min_idx_bytes range_bytes } ...}
     *                 +TimeCyclesSamples = {x y}
     *               }
     *            }
     *         }
     *         States = {
     *           +*StateNameN = "ThreadNameN"
     *         }
     *         Memory = {
     *           InputSignals|OutputSignals = {
     *             +*NUMBER = {
     *               DataSource = "QualifiedName of the DataSource"
     *               ByteSize = NUMBER>0
     *               Signals = {
     *                 *NUMBER = {
     *                   QualifiedName = "QualifiedName of the Signal"
     *                   +ByteOffset = { { min_idx_bytes range_bytes } { min_idx_bytes range_bytes } ... }
     *                 }
     *               }
     *             }
     *           }
     *         }
     *       }
     *    }
     *    dataSourcesDatabase =
     *      Data = {
     *        *NUMBER = {
     *          QualifiedName = "x.y.DATASOURCENAME"
     *          Signals = {
     *            *NUMBER={
     *              QualifiedName = "QualifiedName of the signal"
     *              Type = BasicType
     *              NumberOfDimensions = 0|1|2
     *              NumberOfElements = NUMBER>0
     *              +States = {
     *                *StateN = {
     *                  +Consumers = { "FunctionX" ... "FunctionZ" }
     *                  +Producers = { "FunctionX" ... "FunctionZ" }
     *                }
     *              }
     *              Functions+ = {
     *                *NUMBER = {
     *                  QualifiedName = "QualifiedName of the Function"
     *                  ByteSize = NUMBER>0
     *                  Signals = {
     *                    *NUMBER = {
     *                      QualifiedName = "QualifiedName of the Signal"
     *                      +ByteOffset = { { min_idx_bytes range_bytes } { min_idx_bytes range_bytes } ... }
     *                      +TimeCyclesSamples = {x y}
     *                    }
     *                 }
     *               }
     *             }
     *          }
     *        }
     *     }
     */
    bool AllocateFunctionsMemory();

    /**
     * @brief For each DataSource call DataSourceI::SetConfiguredDatabase.
     * @details Calls DataSourceI::SetConfiguredDatabase on each DataSource under Data, passing the Signals{} and Functions{} branches.
     * @return true if DataSourceI::SetConfiguredDatabase returns true for all DataSources.
     */
    bool PostConfigureDataSources();

    /**
     * @brief Copies the Function and DataSource databases.
     * @param[out] functionsDatabaseOut where to copy the Functions database into.
     * @param[out] dataSourcesDatabaseOut where to copy the DataSource database into.
     * @return true if both ConfigurationDatabase::Copy are successful.
     */
    bool Copy(ConfigurationDatabase &functionsDatabaseOut,
              ConfigurationDatabase &dataSourcesDatabaseOut);

private:
    /**
     * ConfigurationDatabase with all the Functions information.
     */
    ConfigurationDatabase functionsDatabase;

    /**
     * ConfigurationDatabase with all the DataSource information.
     */
    ConfigurationDatabase dataSourcesDatabase;

    /**
     * The default DataSource name to be used if this is not defined in any of the signals.
     */
    StreamString defaultDataSourceName;

    /**
     * The RealTimeApplication associated to this RealTimeApplicationConfigurationBuilder.
     */
    ReferenceT<ReferenceContainer> realTimeApplication;

    /**
     * @brief Flattens all the nested structures in a Database.
     * @details All the introspection elements are expanded and the signals are flatten, so that nested structured
     * signals are stored with a QualifiedName which includes the full path to the signal.
     *
     * For each signal a node with a consecutive number will be created and the fully qualified signal name (with its properties added to it).
     * The original signal definition will then be deleted.
     *
     * @param[in] signalDatabase the database to flatten.
     * @param[in] signalDirection if signalDatabase = functionsDatabase this can be either "InputSignals" or "OutputSignals",
     * otherwise it shall have the value "Signals".
     * @return true if FlattenSignal returns true for all signals in the functionsDatabase and in the dataSourcesDatabase tree.
     */
    bool FlattenSignalsDatabase(ConfigurationDatabase &signalDatabase,
                                const char8 * const signalDirection);

    /**
     * @brief Flattens a signal from the \a signalDatabase and stores it in the \a resolvedSignal database.
     * @details This function flattens the signal with name \a signalName. If the signal is part of a nested node (i.e. a signal
     * with a namespace), this recursively calls itself until it founds a signal which is final. If the signal is a registered type
     * then this function will call SignalIntrospectionToStructuredData which is recursive.
     *
     * @param[in] signalDatabase the database where to read the signal from (it must be pointing at the correct entry signal).
     * @param[in] signalName the name of the signal to be flatten.
     * @param[out] resolvedSignal the flatten signal will be written in this database.
     * @param[out] signalNumber this value is incremented for each signal that is added.
     * @param[in] forceWriteDataSource true if this signal belongs to a GAM. In this case and at this point the DataSource must be known.
     * If it is not known and if defaultDataSourceName exists then DataSource will be assumed to be defaultDataSourceName.
     * @return true if \a signalName can be successfully flatten and all of its members added to a new signal node in \a resolvedSignal.
     */
    bool FlattenSignal(ConfigurationDatabase &signalDatabase,
                       const char8 * const signalName,
                       ConfigurationDatabase &resolvedSignal,
                       uint32 &signalNumber,
                       bool forceWriteDataSource);

    /**
     * @brief Recursively flattens a nested structure into a list of signals.
     * @details For each signal a node with a consecutive number will be created and the
     * fully qualified signal name (with its properties added to it).
     * @param[in] typeName the type of the signal as registered in the ClassRegistryDatabase
     * @param[in] signalName the signal name that will prefix the fully qualified signal name
     * @param[in] alias where can this signal be found in the DataSource. If this parameter is defined, the qualified alias will be prefixed by this value.
     * @param[in] dataSourceName the qualified name of the DataSource where this signal can be found.
     * @param[in] ranges if this parameter is defined, the same ranges will be applied to all the nested signals.
     * @param[in] timeCyclesSamples if this parameter is defined, the same TimeCyclesSamples definition will be applied to all the nested signals.
     * @param[in] data target database where to write the flatten nested signals.
     * @param[out] signalNumber is incremented every time a new signal is added to \a data.
     * @return true if the typeName exists in the ClassRegistryDatabase and if all the signal properties can be successfully written to \a data.
     */
    bool SignalIntrospectionToStructuredData(const char8 * const typeName,
                                             const char8 * const signalName,
                                             const char8 * const alias,
                                             const char8 * const dataSourceName,
                                             AnyType ranges,
                                             AnyType timeCyclesSamples,
                                             StructuredDataI & data,
                                             uint32 &signalNumber);

    /**
     * @brief Adds a signal from \a gamName into \a dataSourceName.
     * @details It assumes that the functionsDatabase and the dataSourcesDatabase are pointing at the correct signal already. If the signal
     *  already exits in the DataSource the definitions of the signal in the GAM and the one already existent in the DataSource will be merged.
     *  If incompatibilities are found an error will be returned.
     * @param[in] gamName name of the GAM where the signal is going to be copied from.
     * @param[in] dataSourceName name of the DataSource where the signal is going to be copied to.
     * @return true if the signal from the GAM \a gamName can be successfully added to the DataSource with name \a dataSourceName. It will
     *  return false if the signal already exists in the DataSource and incompatibilities between definitions are found.
     */
    bool AddSignalToDataSource(StreamString gamName,
                               StreamString dataSourceName);

    /**
     * @brief Merge signals from the Functions to the corresponding DataSources.
     * @details For each signal (where the Type is defined!) in each function call AddSignalToDataSource.
     * @param[in] signalDirection can be either InputSignals or OutputSignals
     * @return true if all the calls to AddSignalToDataSource are successful.
     */
    bool ResolveDataSources(const char8 * const signalDirection);

    /**
     * @brief @see ResolveFunctionSignals()
     * @param[in] signalDirection can be either InputSignals or OutputSignals.
     * @return @see ResolveFunctionSignals()
     */
    bool ResolveFunctionSignals(const char8 * const signalDirection);

    /**
     * @brief Recursively adds a signal with an unknown type in the Function definition
     * @details If the type is not known in the Function definition this is obtained from the DataSource.
     * @param[in] signalName the name of the signal to be resolved.
     * @param[in] functionName the name of the Function to which this signal belongs to.
     * @param[in] dataSourceName the name of the DataSource to which this signal belongs to.
     * @param[in] numberOfFunctionSignals the number of signals that currently exist in the Function with name \a functionName.
     * @return true if the signal can be successfully resolved (which might include createing the signal).
     */
    bool ResolveFunctionSignal(const char8 * const signalName,
                               const char8 * const functionName,
                               const char8 * const dataSourceName,
                               uint32 numberOfFunctionSignals);
    /**
     * @brief Final merge of signals from the DataSources to the Functions.
     * @details For every signal in every Function merge with the corresponding DataSource signal.
     * If the signal type was not originally defined in the Function it will get defined by the DataSource.
     * If incompatibilities are found in the signal definition in the DataSource or in the Function an error will be returned.
     * @param[in] signalDirection can be either InputSignals or OutputSignals
     * @return true if all the signals can be successfully merged.
     */
    bool VerifyFunctionSignals(const char8 * const signalDirection);

    /**
     * @brief @see ResolveConsumersAndProducers()
     * @param[in] consumers true if the function is being called to add Consumers, false if the function is being called to add Producers.
     * @return @see ResolveConsumersAndProducers()
     */
    bool ResolveConsumersAndProducers(bool consumers);

    /**
     * @brief @see ResolveFunctionSignalsMemorySize()
     * @param[in] signalDirection can be either InputSignals or OutputSignals
     * @return @see ResolveFunctionSignalsMemorySize()
     */
    bool ResolveFunctionSignalsMemorySize(const char *signalDirection);

    /**
     * @brief @see ResolveFunctionsMemory()
     * @param[in] signalDirection can be either InputSignals or OutputSignals
     * @return @see ResolveFunctionsMemory()
     */
    bool ResolveFunctionsMemory(const char *signalDirection);

    /**
     * @brief @see AllocateFunctionsMemory()
     * @param[in] signalDirection can be either InputSignals or OutputSignals
     * @return @see AllocateFunctionsMemory()
     */
    bool AllocateFunctionsMemory(const char *signalDirection);

    /**
     * @brief Finds a signal with the name \a signalName in \a database.
     * @details Looks for a signal with a QualifiedName=signalName and leaves the database
     * pointing at node where the signal is found.
     * @param[in] signalName the name of the signal to be found.
     * @param[in] database the database where to look for the signal.
     * @return true if the signal is found.
     */
    bool FindSignalName(StreamString signalName,
                        ConfigurationDatabase &database);
    /**
     * @brief Find the unique number associated to the DataSource with name = \a dataSourceName.
     * @param[in] dataSourceName the fully qualified name of the DataSource to search.
     * @param[out] dataSourceNumber the number associated to the DataSource with name = \a dataSourceName.
     * @return true iff a DataSource with name = \a dataSourceName exists.
     */
    bool FindDataSourceNumber(StreamString dataSourceName,
                              StreamString &dataSourceNumber);
    /**
     * @brief Find the unique number associated to the Function with name = \a functionName.
     * @param[in] functionName the fully qualified name of the Function to search.
     * @param[out] functionNumber the number associated to the Function with name = \a functionName.
     * @return true iff a Function with name = \a functionName exists.
     */
    bool FindFunctionNumber(StreamString functionName,
                            StreamString &functionNumber);
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMEAPPLICATIONCONFIGURATIONBUILDER_H_ */

