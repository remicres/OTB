set(DOCUMENTATION "This module contains classes that ease the implementation of
applications (see chapter 'How to write an application' of the SoftwareGuide).")

otb_module(OTBMPIApplicationEngine
  DEPENDS
    OTBApplicationEngine
    OTBMPIImageIO

  TEST_DEPENDS

  DESCRIPTION
    "${DOCUMENTATION}"
)
