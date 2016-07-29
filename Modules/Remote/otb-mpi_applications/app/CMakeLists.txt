set(OTBMPIApplications_LINK_LIBS
  ${OTBChangeDetection_LIBRARIES}
  ${OTBTextures_LIBRARIES}
)

otb_create_mpi_application(
  NAME           MultivariateAlterationDetector
  SOURCES        otbMultivariateAlterationDetector.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_mpi_application(
  NAME           HaralickTextureExtraction
  SOURCES        otbHaralickTextureExtraction.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_mpi_application(
  NAME           SFSTextureExtraction
  SOURCES        otbSFSTextureExtraction.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_mpi_application(
  NAME           GridBasedImageResampling
  SOURCES        otbGridBasedImageResampling.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_mpi_application(
  NAME           OrthoRectification
  SOURCES        otbOrthoRectification.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_mpi_application(
  NAME           RigidTransformResample
  SOURCES        otbRigidTransformResample.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_mpi_application(
  NAME           Superimpose
  SOURCES        otbSuperimpose.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_mpi_application(
  NAME           BundleToPerfectSensor
  SOURCES        otbBundleToPerfectSensor.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_mpi_application(
  NAME           Pansharpening
  SOURCES        otbPansharpening.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_mpi_application(
  NAME           ClassificationMapRegularization
  SOURCES        otbClassificationMapRegularization.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_mpi_application(
  NAME           PredictRegression
  SOURCES        otbPredictRegression.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_mpi_application(
  NAME           ImageClassifier
  SOURCES        otbImageClassifier.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_mpi_application(
  NAME           EdgeExtraction
  SOURCES        otbEdgeExtraction.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_mpi_application(
  NAME           Despeckle
  SOURCES        otbDespeckle.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_mpi_application(
  NAME           Smoothing
  SOURCES        otbSmoothing.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_mpi_application(
  NAME           BandMath
  SOURCES        otbBandMath.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_mpi_application(
  NAME           BandMathX
  SOURCES        otbBandMathX.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_mpi_application(
  NAME           LocalStatisticExtraction
  SOURCES        otbLocalStatisticExtraction.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_mpi_application(
  NAME           BinaryMorphologicalOperation
  SOURCES        otbBinaryMorphologicalOperation.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_mpi_application(
  NAME           GrayScaleMorphologicalOperation
  SOURCES        otbGrayScaleMorphologicalOperation.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_mpi_application(
  NAME           MeanShiftSmoothing
  SOURCES        otbMeanShiftSmoothing.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_mpi_application(
  NAME           LSMSSegmentation
  SOURCES        otbLSMSSegmentation.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_mpi_application(
  NAME           Convert
  SOURCES        otbConvert.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})
  
otb_create_mpi_application(
  NAME           RadiometricIndices
  SOURCES        otbRadiometricIndices.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})
  