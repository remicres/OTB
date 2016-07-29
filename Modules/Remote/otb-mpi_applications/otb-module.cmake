set(DOCUMENTATION "A bunch of otb applications in their MPI flavor")

otb_module(OTBMPIApplications 
  DEPENDS
    OTBMPIApplicationEngine
    OTBAppChangeDetection
    OTBAppTextures
    OTBAppProjection
    OTBAppFusion
    OTBAppClassification
    OTBAppEdge
    OTBAppFiltering
    OTBAppMathParser
    OTBAppMathParserX
    OTBAppMoments
    OTBAppMorphology
	OTBAppSegmentation
	OTBAppImageUtils
	OTBAppIndices

  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
