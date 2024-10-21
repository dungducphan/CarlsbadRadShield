find_package(ROOT REQUIRED Core)
include(${ROOT_USE_FILE})

add_executable(BeamAna BeamAna.C)
target_link_libraries(BeamAna ${ROOT_LIBRARIES})

add_executable(MeatWall MeatWall.C)
target_link_libraries(MeatWall ${ROOT_LIBRARIES})

add_executable(HDC18 HDC18.C)
target_link_libraries(HDC18 ${ROOT_LIBRARIES})

add_executable(HDC20 HDC20.C)
target_link_libraries(HDC20 ${ROOT_LIBRARIES})