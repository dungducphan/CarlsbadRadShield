if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_definitions(-DDEBUG)
endif()

find_package(Geant4 REQUIRED ui_all vis_all multithreaded)
find_package(ROOT REQUIRED Core)

include(${Geant4_USE_FILE})
include(${ROOT_USE_FILE})

file(GLOB sources src/*.cc)
file(GLOB headers inc/*.hh)
file(GLOB stlFiles geometryFiles/HDC3_Fe1_HDC12/*.stl)

file(GLOB MACROS "*.mac")

file(COPY ${MACROS} DESTINATION ${PROJECT_BINARY_DIR}/G4Sim)
file(COPY ${EXT_DATA} DESTINATION ${PROJECT_BINARY_DIR}/G4Sim)
file(COPY ${stlFiles} DESTINATION ${PROJECT_BINARY_DIR}/G4Sim)

include_directories(${PROJECT_SOURCE_DIR}/G4Sim/inc)
add_executable(CarlsbadRadShield sim.cc ${sources} ${headers})
target_link_libraries(CarlsbadRadShield ${Geant4_LIBRARIES})
target_link_libraries(CarlsbadRadShield ${ROOT_LIBRARIES})
