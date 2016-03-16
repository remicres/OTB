set(OTBMPIApplicationEngine_SRC
  otbMPIWrapperOutputImageParameter.cxx
  otbMPIWrapperApplication.cxx
  )
add_library(OTBMPIApplicationEngine ${OTBMPIApplicationEngine_SRC})
target_link_libraries(OTBMPIApplicationEngine ${OTBApplicationEngine_LIBRARIES} ${OTBMPIImageIO_LIBRARIES})

otb_module_target(OTBMPIApplicationEngine)
