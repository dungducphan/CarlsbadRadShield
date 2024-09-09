find_package(ROOT REQUIRED Core)
include(${ROOT_USE_FILE})

add_executable(BeamAna BeamAna.C)
target_link_libraries(BeamAna ${ROOT_LIBRARIES})