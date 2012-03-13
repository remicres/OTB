MESSAGE(STATUS "Importing TinyXML...")
FIND_PACKAGE(TinyXML)

IF(TINYXML_FOUND)
  OPTION(OTB_USE_EXTERNAL_TINYXML "Use external TinyXML library." ON)
ELSE(TINYXML_FOUND)
  OPTION(OTB_USE_EXTERNAL_TINYXML "Use external TinyXML library." OFF)
  SET(TINYXML_LIBRARIES otbtinyXML)
ENDIF(TINYXML_FOUND)
MARK_AS_ADVANCED(OTB_USE_EXTERNAL_TINYXML)
