cmake_minimum_required (VERSION 2.8)

add_executable(otbMPIToy otbMPIToy.cxx)
target_link_libraries(otbMPIToy ${OTBMPIImageIO_LIBRARIES})


