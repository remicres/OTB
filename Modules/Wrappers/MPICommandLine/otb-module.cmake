set(DOCUMENTATION "This module contains classes that ease the implementation of MPI
applications (see chapter 'How to write an application' of the SoftwareGuide).")

otb_module(OTBMPICommandLine
  DEPENDS
    OTBCommandLine
    OTBCommon
    OTBITK
    OTBTinyXML
    OTBMPIApplicationEngine

  TEST_DEPENDS
    OTBTestKernel
    OTBAppImageUtils

  DESCRIPTION
    "${DOCUMENTATION}"
)
