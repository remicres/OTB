WRAP_INCLUDE("itkPolyLineParametricPath.h" POINTER)
WRAP_CLASS("itk::PathSource" POINTER)
  FOREACH(d ${WRAP_ITK_DIMS})
    WRAP_TEMPLATE("PLPP${d}" "itk::PolyLineParametricPath< ${d} >")
  ENDFOREACH(d)
END_WRAP_CLASS()
