/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

  =========================================================================*/
#ifndef __otbMPIWrapperApplication_h
#define __otbMPIWrapperApplication_h

#include <string>
#include "otbWrapperApplication.h"
#include "otbWrapperTypes.h"
#include "otbWrapperTags.h"
#include "otbWrapperParameterGroup.h"

#include "itkLogger.h"
#include "otbWrapperMacros.h"
#include "otbWrapperInputImageParameter.h"
#include "otbMPIWrapperOutputImageParameter.h"
#include "otbWrapperComplexInputImageParameter.h"
#include "otbWrapperComplexOutputImageParameter.h"
#include "otbWrapperDocExampleStructure.h"
#include "itkMersenneTwisterRandomVariateGenerator.h"
#include "mpi.h"

namespace otb
{
namespace Wrapper
{

/** \class Application
 *  \brief This class represent an application
 *  TODO
 *
 *
 * \ingroup OTBApplicationEngine
 */
class ITK_ABI_EXPORT MPIApplication: public otb::Wrapper::Application
{
public:
  /** Standard class typedefs. */
  typedef MPIApplication                 Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** RTTI support */
  itkTypeMacro(MPIApplication, itk::Object);

  /** Run the application, then writes all the output to disk
   * if they have an associated filename.
   * This is a helper function for wrappers without pipeline support.
   *
   * Returns 0 on success, or a non-null integer on error
   */
  int ExecuteAndWriteOutput();

  /* This method will be called after the
   * ExecuteAndWriteOutput() call to allow for cleanup. Default
   * implementation does nothing */
  virtual void AfterExecuteAndWriteOutputs();

  /* MPI Parameters accessors */
  itkGetMacro(MyRank, int);
  itkGetMacro(NProcs, int);
  itkSetMacro(MyRank, int);
  itkSetMacro(NProcs, int);

protected:
  /** Constructor */
  MPIApplication();

  /** Destructor */
  virtual ~MPIApplication();

private:

  MPIApplication(const MPIApplication &); //purposely not implemented
  void operator =(const MPIApplication&); //purposely not implemented

  int m_MyRank;
  int m_NProcs;

}; //end class

} // end namespace Wrapper
} //end namespace otb

#endif // __otbMPIWrapperApplication_h_
