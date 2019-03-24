#include <gtest/gtest.h>
#include "../EPFMI.hpp"
#include "../EPComponent.hpp"
#include "test-config.h"
#include "../EnergyPlus/DataEnvironment.hh"


TEST( EPFMI, static_lib ) {

  auto comp = fmi2Instantiate("Building1", // instanceName
    fmi2ModelExchange, // fmuType
    "", // fmuGUID
    "", // fmuResourceLocation
    NULL, // functions
    true, // visible
    false); // loggingOn

  EPComponent * epcomp = static_cast<EPComponent*>(comp);

  epcomp->weatherFilePath = weather;
  epcomp->iddPath = idd;
  epcomp->idfInputPath = input;

  epcomp->variables.emplace(0,Variable("Attic,T", VariableType::INPUT));
  epcomp->variables.emplace(1,Variable("Core_ZN,T", VariableType::INPUT));
  epcomp->variables.emplace(2,Variable("Perimeter_ZN_1,T", VariableType::INPUT));
  epcomp->variables.emplace(3,Variable("Perimeter_ZN_2,T", VariableType::INPUT));
  epcomp->variables.emplace(4,Variable("Perimeter_ZN_3,T", VariableType::INPUT));
  epcomp->variables.emplace(5,Variable("Perimeter_ZN_4,T", VariableType::INPUT));

  epcomp->variables.emplace(6,Variable("Attic,QConSen_flow", VariableType::OUTPUT));
  epcomp->variables.emplace(7,Variable("Core_ZN,QConSen_flow", VariableType::OUTPUT));
  epcomp->variables.emplace(8,Variable("Perimeter_ZN_1,QConSen_flow", VariableType::OUTPUT));
  epcomp->variables.emplace(9,Variable("Perimeter_ZN_2,QConSen_flow", VariableType::OUTPUT));
  epcomp->variables.emplace(10,Variable("Perimeter_ZN_3,QConSen_flow", VariableType::OUTPUT));
  epcomp->variables.emplace(11,Variable("Perimeter_ZN_4,QConSen_flow", VariableType::OUTPUT));

  epcomp->variables.emplace(12,Variable("Attic,V", VariableType::PARAMETER));
  epcomp->variables.emplace(13,Variable("Core_ZN,V", VariableType::PARAMETER));
  epcomp->variables.emplace(14,Variable("Perimeter_ZN_1,V", VariableType::PARAMETER));
  epcomp->variables.emplace(15,Variable("Perimeter_ZN_2,V", VariableType::PARAMETER));
  epcomp->variables.emplace(16,Variable("Perimeter_ZN_3,V", VariableType::PARAMETER));
  epcomp->variables.emplace(17,Variable("Perimeter_ZN_4,V", VariableType::PARAMETER));

  epcomp->variables.emplace(18,Variable("Attic,AFlo", VariableType::PARAMETER));
  epcomp->variables.emplace(19,Variable("Core_ZN,AFlo", VariableType::PARAMETER));
  epcomp->variables.emplace(20,Variable("Perimeter_ZN_1,AFlo", VariableType::PARAMETER));
  epcomp->variables.emplace(21,Variable("Perimeter_ZN_2,AFlo", VariableType::PARAMETER));
  epcomp->variables.emplace(22,Variable("Perimeter_ZN_3,AFlo", VariableType::PARAMETER));
  epcomp->variables.emplace(23,Variable("Perimeter_ZN_4,AFlo", VariableType::PARAMETER));

  epcomp->variables.emplace(24,Variable("Attic,mSenFac", VariableType::PARAMETER));
  epcomp->variables.emplace(25,Variable("Core_ZN,mSenFac", VariableType::PARAMETER));
  epcomp->variables.emplace(26,Variable("Perimeter_ZN_1,mSenFac", VariableType::PARAMETER));
  epcomp->variables.emplace(27,Variable("Perimeter_ZN_2,mSenFac", VariableType::PARAMETER));
  epcomp->variables.emplace(28,Variable("Perimeter_ZN_3,mSenFac", VariableType::PARAMETER));
  epcomp->variables.emplace(29,Variable("Perimeter_ZN_4,mSenFac", VariableType::PARAMETER));

  double tStart = 0.0;
  double tEnd = 86400;

  double outputs[] = {0.0};
  const unsigned int outputRefs[] = {6}; // Attic,QConSen_flow
  double inputs[] = {21.0, 21.0,21.0, 21.0, 21.0, 21.0};
  const unsigned int inputRefs[] = {0, 1, 2, 3, 4, 5}; // Attic,T...
  const unsigned int paramRefs[] = {12}; // Attic,V
  double params[1];

  fmi2SetupExperiment(epcomp, // component
    false, // toleranceDefined
    0, // tolerance
    tStart, // startTime
    true, // stopTimeDefined
    tEnd); // stopTime

  fmi2EventInfo eventInfo;

  fmi2GetReal(epcomp, paramRefs, 1, params);
  double atticVolume = params[0]; // m^3

  std::cout << "atticVolume: " << atticVolume << std::endl;

  double atticTemp = 21.0;

  inputs[0] = atticTemp;
  fmi2SetReal(epcomp, inputRefs, 6, inputs);

  double time = tStart;
  fmi2SetTime(epcomp, time);

  while ( time < tEnd ) {
    fmi2NewDiscreteStates(epcomp, &eventInfo);

    double lastTime = time;
    time = eventInfo.nextEventTime;
    fmi2SetTime(epcomp, time);
    double dt = time - lastTime;

    inputs[0] = atticTemp;
    fmi2SetReal(epcomp, inputRefs, 6, inputs);

    // update atticTemp
    fmi2GetReal(epcomp, outputRefs, 1, outputs);

    double atticQFlow = outputs[0]; // J/s 
    std::cout << "atticQFlow: " << atticQFlow << std::endl;
    double densityAir = 1.276; // kg/m^3
    double heatCapacity = 1000.6; // J/kgK
    double tempDot = atticQFlow / ( atticVolume * densityAir * heatCapacity );

    atticTemp = atticTemp + (dt * tempDot);

    std::cout << "Current time: " << time << std::endl;
    std::cout << "dt: " << dt << std::endl;
    std::cout << "Attic Temp is: " << atticTemp << std::endl;
  }

  fmi2Terminate(epcomp);

  std::cout << "epfmi test is now complete" << std::endl;
}

