cmake_minimum_required(VERSION 2.9)
project(proga)

set (CMAKE_CXX_STANDARD 14)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set (CMAKE_CXX_FLAGS "-pthread")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin/${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE})

set(SOURCES
    src/auxiliaryObjects/Obj.cpp
    src/auxiliaryObjects/DataFrame.cpp
    src/auxiliaryObjects/UsefulMath.cpp
    src/algorithms/ConstPrediction.cpp
    src/algorithms/DecisionStump.cpp
    src/algorithms/KNN.cpp
    src/algorithms/LinearRegression.cpp
    src/LearningMachineLearning.cpp
)

add_executable(proga src/startMain.cpp ${SOURCES})
add_library(ml_lib_yo STATIC ${SOURCES})
