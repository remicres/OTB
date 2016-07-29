set(DOCUMENTATION "An MPI executable example")

otb_module(MPIToy
  DEPENDS
    OTBMPIImageIO
    OTBImageManipulation

  TEST_DEPENDS
  DESCRIPTION
    "MPI application example"
)
