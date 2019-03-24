#include "EPFMI_Export.hpp"
#include <cstddef>
#include <fmi2FunctionTypes.h>

extern "C" {

EPFMI_API fmi2Component fmi2Instantiate(fmi2String instanceName,
  fmi2Type fmuType,
  fmi2String fmuGUID,
  fmi2String fmuResourceLocation,
  const fmi2CallbackFunctions* functions,
  fmi2Boolean visible,
  fmi2Boolean loggingOn);

EPFMI_API fmi2Status fmi2SetupExperiment(fmi2Component c,
  fmi2Boolean toleranceDefined,
  fmi2Real tolerance,
  fmi2Real startTime,
  fmi2Boolean stopTimeDefined,
  fmi2Real stopTime);

EPFMI_API fmi2Status fmi2SetTime(fmi2Component c, fmi2Real time);

EPFMI_API fmi2Status fmi2SetReal(fmi2Component c,
  const fmi2ValueReference vr[],
  size_t nvr,
  const fmi2Real values[]);

EPFMI_API fmi2Status fmi2GetReal(fmi2Component c,
  const fmi2ValueReference vr[],
  size_t nvr,
  fmi2Real values[]);

EPFMI_API fmi2Status fmi2NewDiscreteStates(fmi2Component  c, fmi2EventInfo* fmi2eventInfo);

EPFMI_API fmi2Status fmi2Terminate(fmi2Component c);

}


