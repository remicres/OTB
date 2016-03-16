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

#include "otbMPIWrapperApplication.h"
#include "otbWrapperChoiceParameter.h"
#include "otbWrapperListViewParameter.h"
#include "otbWrapperDirectoryParameter.h"
#include "otbWrapperEmptyParameter.h"
#include "otbWrapperInputFilenameParameter.h"
#include "otbWrapperInputFilenameListParameter.h"
#include "otbWrapperOutputFilenameParameter.h"
#include "otbWrapperOutputProcessXMLParameter.h"
#include "otbWrapperInputVectorDataParameter.h"
#include "otbWrapperInputVectorDataListParameter.h"
#include "otbWrapperOutputVectorDataParameter.h"
#include "otbWrapperRadiusParameter.h"
#include "otbWrapperStringListParameter.h"
#include "otbWrapperInputImageListParameter.h"
#include "otbWrapperInputProcessXMLParameter.h"
#include "otbWrapperRAMParameter.h"


#include "otbWrapperAddProcessToWatchEvent.h"

#include "otbMacro.h"
#include "otbWrapperTypes.h"
#include <exception>
#include "itkMacro.h"

namespace otb
{
namespace Wrapper
{

MPIApplication::MPIApplication() : m_MyRank(0), m_NProcs(1)
{
}

MPIApplication::~MPIApplication()
{
}

int MPIApplication::ExecuteAndWriteOutput()
{

  /* Init. MPI session */

  int initialized, finalized;
  bool already_initialized = false;

  MPI_Initialized(&initialized);
  if (!initialized)
    {
    // MPI needs to be initialized
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &m_NProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &m_MyRank);
    }
  else
    {
    // MPI is already initialized
    already_initialized = true;
    }

  /* Perform the work in parallel */

  int status = this->Execute();

  if (status == 0)
    {
    std::vector<std::string> paramList = GetParametersKeys(true);
    // First Get the value of the available memory to use with the
    // writer if a RAMParameter is set
    bool useRAM = false;
    unsigned int ram = 0;
    for (std::vector<std::string>::const_iterator it = paramList.begin();
        it != paramList.end();
        ++it)
      {
      std::string key = *it;

      if (GetParameterType(key) == ParameterType_RAM
          && IsParameterEnabled(key))
        {
        Parameter* param = GetParameterByKey(key);
        RAMParameter* ramParam = dynamic_cast<RAMParameter*>(param);
        if(ramParam!=NULL)
          {
          ram = ramParam->GetValue();
          useRAM = true;
          }
        }
      }

    for (std::vector<std::string>::const_iterator it = paramList.begin();
        it != paramList.end();
        ++it)
      {
      std::string key = *it;
      if (GetParameterType(key) == ParameterType_OutputImage
          && IsParameterEnabled(key) && HasValue(key) )
        {
        Parameter* param = GetParameterByKey(key);
        MPIOutputImageParameter* outputParam = dynamic_cast<MPIOutputImageParameter*>(param);
        outputParam->SetMyRank(m_MyRank);
        outputParam->SetNProcs(m_NProcs);
        if(outputParam!=NULL)
          {
          outputParam->InitializeWriters();
          if (useRAM)
            {
            outputParam->SetRAMValue(ram);
            }
          std::ostringstream progressId;
          progressId << "Writing " << outputParam->GetFileName() << "...";
          AddProcess(outputParam->GetWriter(), progressId.str());
          outputParam->Write();
          }
        }
      else if (GetParameterType(key) == ParameterType_OutputVectorData
          && IsParameterEnabled(key) && HasValue(key) )
        {
        Parameter* param = GetParameterByKey(key);
        OutputVectorDataParameter* outputParam = dynamic_cast<OutputVectorDataParameter*>(param);
        if(outputParam!=NULL)
          {
          outputParam->InitializeWriters();
          std::ostringstream progressId;
          progressId << "Writing " << outputParam->GetFileName() << "...";
          AddProcess(outputParam->GetWriter(), progressId.str());
          outputParam->Write();
          }
        }
      else if (GetParameterType(key) == ParameterType_ComplexOutputImage
          && IsParameterEnabled(key) && HasValue(key) )
        {
        Parameter* param = GetParameterByKey(key);
        ComplexOutputImageParameter* outputParam = dynamic_cast<ComplexOutputImageParameter*>(param);

        if(outputParam!=NULL)
          {
          outputParam->InitializeWriters();
          if (useRAM)
            {
            outputParam->SetRAMValue(ram);
            }
          std::ostringstream progressId;
          progressId << "Writing " << outputParam->GetFileName() << "...";
          AddProcess(outputParam->GetWriter(), progressId.str());
          outputParam->Write();
          }
        }

      //        //xml writer parameter
      //        else if (m_HaveOutXML && GetParameterType(key) == ParameterType_OutputProcessXML
      //                 && IsParameterEnabled(key) && HasValue(key) )
      //          {
      //          Parameter* param = GetParameterByKey(key);
      //          OutputProcessXMLParameter* outXMLParam = dynamic_cast<OutputProcessXMLParameter*>(param);
      //          if(outXMLParam!=NULL)
      //            {
      //            outXMLParam->Write(this);
      //            }
      //          }
      }
    }

  AfterExecuteAndWriteOutputs();

  /* Terminate MPI session */

  MPI_Finalized(&finalized);
  if (!finalized  && !already_initialized)
    MPI_Finalize();

  return status;
}

void MPIApplication::AfterExecuteAndWriteOutputs()
{}


}
}
