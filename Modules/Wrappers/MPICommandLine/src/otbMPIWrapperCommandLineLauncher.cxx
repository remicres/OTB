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
#include "otbMPIWrapperCommandLineLauncher.h"

// Single value parameter
//#include "otbWrapperChoiceParameter.h"
#include "otbWrapperDirectoryParameter.h"
#include "otbWrapperEmptyParameter.h"
#include "otbWrapperInputFilenameParameter.h"
#include "otbWrapperOutputFilenameParameter.h"
#include "otbWrapperInputImageParameter.h"
#include "otbWrapperInputVectorDataParameter.h"
#include "otbWrapperOutputVectorDataParameter.h"
#include "otbWrapperRadiusParameter.h"
#include "otbWrapperListViewParameter.h"
#include "otbWrapperRAMParameter.h"
#include "otbWrapperOutputProcessXMLParameter.h"
#include "otbWrapperAddProcessToWatchEvent.h"
#include "otbWrapperOutputImageParameter.h"

// List value parameter
#include "otbWrapperInputImageListParameter.h"
#include "otbWrapperInputVectorDataListParameter.h"
#include "otbWrapperInputFilenameListParameter.h"
#include "otbWrapperStringListParameter.h"

#include "otbWrapperTypes.h"
#include <itksys/RegularExpression.hxx>
#include <string>
#include <iostream>

#include "otbWrapperApplicationRegistry.h"

namespace otb
{
namespace Wrapper
{

MPICommandLineLauncher::MPICommandLineLauncher() :
  /*m_Expression(""),*/m_VExpression(), m_WatcherList(), m_ReportProgress(true), m_MaxKeySize(0)
{
  m_MyRank = 0;
  m_NProcs = 1;
  m_Application = NULL;
  m_Parser = CommandLineParser::New();
  m_LogOutput = itk::StdStreamLogOutput::New();
  m_LogOutput->SetStream(std::cout);

  // Add the callback to be added when a AddProcessToWatch event is invoked
  m_AddProcessCommand = AddProcessCommandType::New();
  m_AddProcessCommand->SetCallbackFunction(this, &MPICommandLineLauncher::LinkWatchers);
}

MPICommandLineLauncher::MPICommandLineLauncher(const char * exp) /*:
  m_Expression(exp)*/
{
  m_MyRank = 0;
  m_NProcs = 1;
  m_Application = NULL;
  m_Parser = CommandLineParser::New();
}

MPICommandLineLauncher::~MPICommandLineLauncher()
{
  this->DeleteWatcherList();
}

void MPICommandLineLauncher::DeleteWatcherList()
{
  for (unsigned int i = 0; i < m_WatcherList.size(); i++)
    {
    delete m_WatcherList[i];
    m_WatcherList[i] = NULL;
    }
  m_WatcherList.clear();
}

bool MPICommandLineLauncher::Load(const std::string & exp)
{
  /*m_Expression = exp;
  return this->Load(); */
  return false;
}

bool MPICommandLineLauncher::Load(const std::vector<std::string> &vexp)
{
  m_VExpression = vexp;
  return this->Load();
}


bool MPICommandLineLauncher::Load()
{
  // Add a space to clarify output logs
  std::cerr << std::endl;
  if (m_VExpression.empty())
    {
    itkExceptionMacro("No expression specified...");
    }

  if (this->CheckParametersPrefix() == false)
    {
    std::cerr << "ERROR: Parameter keys have to set using \"-\", not \"--\"" << std::endl;
    return false;
    }

  if (this->CheckUnicity() == false)
    {
    std::cerr << "ERROR: At least one key is not unique in the expression..." << std::endl;
    return false;
    }

  if (this->LoadPath() == false)
    {
    if (m_Parser->GetPathsAsString(m_VExpression).size() != 0)
      {
      std::cerr << "ERROR: At least one specified path within \"" << m_Parser->GetPathsAsString(m_VExpression)
                << "\" is invalid or doesn't exist..." << std::endl;
      return false;
      }
    }

  this->LoadApplication();

  return true;
}

bool MPICommandLineLauncher::Execute()
{
  if (this->BeforeExecute() == false)
    {
    return false;
    }

  if( m_Application->Execute() == 0 )
    {
    this->DisplayOutputParameters();
    return true;
    }
  else
    return false;
}

bool MPICommandLineLauncher::ExecuteAndWriteOutput()
{
    m_Application->SetMyRank(m_MyRank);
    m_Application->SetNProcs(m_NProcs);

  try
    {
    if (this->BeforeExecute() == false)
      {
      return false;
      MPI_Abort(MPI_COMM_WORLD, 1);
      }
    if( m_Application->ExecuteAndWriteOutput() == 0 )
      {
      this->DisplayOutputParameters();
      }
    else
    {
      return false;
      MPI_Abort(MPI_COMM_WORLD, 1);
    }
    }
  catch(std::exception& err)
    {
    std::ostringstream message;
    message << "The following error occurred during application execution : " << err.what() << std::endl;
    m_Application->GetLogger()->Write( itk::LoggerBase::FATAL, message.str() );
    MPI_Abort(MPI_COMM_WORLD, 1);
    return false;
    }
  catch(...)
    {
    m_Application->GetLogger()->Write( itk::LoggerBase::FATAL, "An unknown exception has been raised during application execution" );
    MPI_Abort(MPI_COMM_WORLD, 1);
    return false;
    }

  return true;
}

bool MPICommandLineLauncher::BeforeExecute()
{
  if (m_Application.IsNull())
    {
    std::cerr << "ERROR: No loaded application..." << std::endl;
    return false;
    }

  // Check if there's keys in the expression if the application takes
  // at least 1 mandatory parameter
  const std::vector<std::string> appKeyList = m_Application->GetParametersKeys(true);
  std::vector<std::string> keyList = m_Parser->GetKeyList( m_VExpression );

  if( appKeyList.size()!=0 && keyList.size()==0 )
    {
    std::cerr << "ERROR: Waiting for at least one parameter..." << std::endl;
    this->DisplayHelp();
    return false;
    }

  // if help is asked...
  if (m_Parser->IsAttributExists("-help", m_VExpression) == true)
    {
    this->DisplayHelp();
    return false;
    }
  //display OTB version
  if (m_Parser->IsAttributExists("-version", m_VExpression) == true)
    {
    std::cerr << "This is the "<<m_Application->GetName() << " application, version " << OTB_VERSION_STRING <<std::endl;
    return false;
    }

  // if we want to load test environnement
  if (m_Parser->IsAttributExists("-testenv", m_VExpression) == true)
    {
    this->LoadTestEnv();
    }

  // Check the key validity (ie. exist in the application parameters)

  std::string unknownKey;
  if (this->CheckKeyValidity(unknownKey) == false)
    {

    std::cerr << "ERROR: option -"<<unknownKey<<" does not exist in the application." << std::endl;
    this->DisplayHelp();

    return false;
    }
  try
    {
    if (this->LoadParameters() != OKPARAM)
      {
      std::cerr << "ERROR: Troubles loading parameter, please check your line argument..." << std::endl;
      // Force to reload the application, the LoadParameters can change wrong values
      this->LoadApplication();
      m_Application->Init();
      this->DisplayHelp();

      return false;
      }
    }
  catch (itk::ExceptionObject& err)
    {
    std::cerr << "ERROR: Troubles in parameter setting, please check your line argument..." << std::endl;
    std::cerr << err.GetDescription() << std::endl;
    // Force to reload the application, the LoadParameters can change wrong values
    this->LoadApplication();
    m_Application->Init();
    this->DisplayHelp();

    return false;
    }

  // Check for the progress report

  if (m_Parser->IsAttributExists("-progress", m_VExpression) == true)
    {
    std::vector<std::string> val;
    val = m_Parser->GetAttribut("-progress", m_VExpression);
    if (val.size() != 1)
      {
      std::cerr << "ERROR: Invalid progress argument, must be unique value..." << std::endl;
      return false;
      }
    if (val[0] == "1" || val[0] == "true")
      {
      m_ReportProgress = true;
      }
    else
      if (val[0] == "0" || val[0] == "false")
        {
        m_ReportProgress = false;
        }
      else
        {
        std::cerr << "ERROR: Invalid progress argument, must be 0, 1, false or true..." << std::endl;
        // Force to reload the application, the LoadParameters can change wrong values
        this->LoadApplication();
        this->DisplayHelp();

        return false;
        }
    }
  return true;
}

bool MPICommandLineLauncher::LoadPath()
{
  std::vector<std::string> pathList;
  // If users has set path...
  //if (m_Parser->GetPaths(pathList, m_Expression) == CommandLineParser::OK)
  if (m_Parser->GetPaths(pathList, m_VExpression) == CommandLineParser::OK)
    {
    for (unsigned i = 0; i < pathList.size(); i++)
      {
        ApplicationRegistry::AddApplicationPath(pathList[i]);
      }
    }
  else
    {
    return false;
    }

  return true;
}

void MPICommandLineLauncher::LoadApplication()
{
  // Look for the module name
  std::string moduleName;
  if (m_Parser->GetModuleName(moduleName, m_VExpression) != CommandLineParser::OK)
    {
    std::cerr << "ERROR: LoadApplication, no module found..." << std::endl;
    return;
    }

  // Instantiate the application using the factory
  Application::Pointer app = ApplicationRegistry::CreateApplication(moduleName);
  m_Application = MPIApplication::Pointer(static_cast<MPIApplication *>(app.GetPointer()));

  if (m_Application.IsNull())
    {
    std::cerr << "ERROR: Could not find application \"" << moduleName << "\"" << std::endl;

    const char * ITK_AUTOLOAD_PATH = itksys::SystemTools::GetEnv("ITK_AUTOLOAD_PATH");
    std::cerr << "ERROR: Module search path: " << (ITK_AUTOLOAD_PATH ? ITK_AUTOLOAD_PATH : "none (check ITK_AUTOLOAD_PATH)") << std::endl;

    std::vector<std::string> list = ApplicationRegistry::GetAvailableApplications();
    if (list.size() == 0)
      {
      std::cerr << "ERROR: Available modules : none." << std::endl;
      }
    else
      {
      std::cerr << "ERROR: Available modules :" << std::endl;
      for (std::vector<std::string>::const_iterator it = list.begin(); it != list.end(); ++it)
        {
        std::cerr << "\t" << *it << std::endl;
        }
      }
    }
  else
    {
    // Attach log output to the Application logger
    m_Application->GetLogger()->SetTimeStampFormat(itk::LoggerBase::HUMANREADABLE);
    m_Application->GetLogger()->AddLogOutput(m_LogOutput);

    // Add an observer to the AddedProcess event
    m_Application->AddObserver(otb::Wrapper::AddProcessToWatchEvent(), m_AddProcessCommand.GetPointer());

    }
}

MPICommandLineLauncher::ParamResultType MPICommandLineLauncher::LoadParameters()
{
  if (m_Application.IsNull())
    {
    itkExceptionMacro("No application loaded");
    }

  /* Check for inxml parameter. If exists Update all Parameters from xml and
   * check for user defined parameters for overriding those from XML
   */
  const char *inXMLKey =  "inxml";
  const char *attrib   = "-inxml";
  const bool paramInXMLExists(m_Parser->IsAttributExists(attrib, m_VExpression));
  if(paramInXMLExists)
    {
    std::vector<std::string> inXMLValues;
    inXMLValues = m_Parser->GetAttribut(attrib, m_VExpression);
    m_Application->SetParameterString(inXMLKey, inXMLValues[0]);
    m_Application->UpdateParameters();
    }

  const std::vector<std::string> appKeyList = m_Application->GetParametersKeys(true);

  // Loop over each parameter key declared in the application
  // FIRST PASS : set parameter values
  for (unsigned int i = 0; i < appKeyList.size(); i++)
    {
    const std::string paramKey(appKeyList[i]);
    std::vector<std::string> values;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    std::cout << " paramKey="<<paramKey << std::endl;

    otb::Wrapper::Parameter::Pointer param = m_Application->GetParameterByKey(paramKey);
//    std::cout << "got param" << std::endl;

    otb::Wrapper::ParameterType type = m_Application->GetParameterType(paramKey);
//    std::cout << "got type" << std::endl;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const bool paramExists(m_Parser->IsAttributExists(std::string("-").append(paramKey), m_VExpression));
//    std::cout << " paramExists="<<paramExists << std::endl;

    // if param is a Group, dont do anything, ParamGroup dont have values
    if (type != otb::Wrapper::ParameterType_Group)
      {
      // Get the attribute relative to this key as vector
      values = m_Parser->GetAttribut(std::string("-").append(paramKey), m_VExpression);

      // If the param does not exists in the cli, dont try to set a
      // value on it, an exception will be thrown later in this function
      if (paramExists)
        {
        // Check if there is a value associated to the attribute
        if ( values.empty() )
          {
          std::cerr << "ERROR: No value associated to the parameter : \"" << paramKey
                    << "\", invalid number of values " << values.size() << std::endl;
          return INVALIDNUMBEROFVALUE;
          }

        // Ensure that the parameter is enabled
        m_Application->EnableParameter(paramKey);
        if (type == otb::Wrapper::ParameterType_InputVectorDataList)
          {
          dynamic_cast<otb::Wrapper::InputVectorDataListParameter *> (param.GetPointer())->SetListFromFileName(values);
          }
        else
          if (type == otb::Wrapper::ParameterType_InputImageList)
            {
            dynamic_cast<otb::Wrapper::InputImageListParameter *> (param.GetPointer())->SetListFromFileName(values);
            }
          else
            if (type == otb::Wrapper::ParameterType_InputFilenameList)
              {
              dynamic_cast<otb::Wrapper::InputFilenameListParameter *> (param.GetPointer())->SetListFromFileName(values);
              }
            else
              if (type == otb::Wrapper::ParameterType_StringList)
                {
                dynamic_cast<otb::Wrapper::StringListParameter *> (param.GetPointer())->SetValue(values);
                }
              else
                if (type == otb::Wrapper::ParameterType_String)
                  {
                  dynamic_cast<otb::Wrapper::StringParameter *> (param.GetPointer())->SetValue(
                    m_Parser->GetAttributAsString(std::string("-").append(paramKey), m_VExpression) );
                  }
                else
                  if (type == otb::Wrapper::ParameterType_OutputImage)
                    {
                    m_Application->SetParameterString(paramKey, values[0]);
                    // Check if pixel type is given
                    if (values.size() == 2)
                      {
                        otb::Wrapper::ImagePixelType outPixType = otb::Wrapper::ImagePixelType_float;
                      if (values[1] == "uint8")
                        outPixType = otb::Wrapper::ImagePixelType_uint8;
                      else if (values[1] == "int16")
                        outPixType = otb::Wrapper::ImagePixelType_int16;
                      else if (values[1] == "uint16")
                        outPixType = otb::Wrapper::ImagePixelType_uint16;
                      else if (values[1] == "int32")
                        outPixType = otb::Wrapper::ImagePixelType_int32;
                      else if (values[1] == "uint32")
                        outPixType = otb::Wrapper::ImagePixelType_uint32;
                      else if (values[1] == "float")
                        outPixType = otb::Wrapper::ImagePixelType_float;
                      else if (values[1] == "double")
                        outPixType = otb::Wrapper::ImagePixelType_double;
                      else
                        {
                        return WRONGPARAMETERVALUE;
                        }
                      dynamic_cast<OutputImageParameter *> (param.GetPointer())->SetPixelType(outPixType);
                      }
                    else
                      if (values.size() != 1 && values.size() != 2)
                        {
                        std::cerr << "ERROR: Invalid number of value for: \"" << paramKey
                                  << "\", invalid number of values " << values.size() << std::endl;
                        return INVALIDNUMBEROFVALUE;
                        }
                    }
                  else
                    if (type == otb::Wrapper::ParameterType_ListView)
                      {
                      dynamic_cast<otb::Wrapper::ListViewParameter *> (param.GetPointer())->SetSelectedNames(values);
                      }
                    else
                      if(values.size() != 1)
                        {
                        // Handle space in filename. Only for input
                        // files or directories
                        if (type == otb::Wrapper::ParameterType_Directory         || type == otb::Wrapper::ParameterType_InputFilename ||
                            type == otb::Wrapper::ParameterType_ComplexInputImage || type == otb::Wrapper::ParameterType_InputImage ||
                            type == otb::Wrapper::ParameterType_InputVectorData   || type == otb::Wrapper::ParameterType_OutputVectorData )
                          {
                          for(unsigned int j=1; j<values.size(); j++)
                            {
                            values[0].append(" ");
                            values[0].append(values[j]);
                            }
                          }
                        else if (!param->GetAutomaticValue())
                          {
                          std::cerr << "ERROR: Invalid number of value for: \"" << paramKey << "\", must have 1 value, not  "
                                    << values.size() << std::endl;
                          return INVALIDNUMBEROFVALUE;
                          }
                        }
        // Single value parameter
        if (type == otb::Wrapper::ParameterType_Choice || type == otb::Wrapper::ParameterType_Float || type == otb::Wrapper::ParameterType_Int ||
            type == otb::Wrapper::ParameterType_Radius || type == otb::Wrapper::ParameterType_Directory || type == otb::Wrapper::ParameterType_InputFilename ||
            type == otb::Wrapper::ParameterType_InputFilenameList || type == otb::Wrapper::ParameterType_OutputFilename ||
            type == otb::Wrapper::ParameterType_ComplexInputImage || type == otb::Wrapper::ParameterType_InputImage ||
            type == otb::Wrapper::ParameterType_InputVectorData || type == otb::Wrapper::ParameterType_InputVectorDataList ||
            type == otb::Wrapper::ParameterType_OutputVectorData || type == otb::Wrapper::ParameterType_RAM ||
            type == otb::Wrapper::ParameterType_OutputProcessXML) // || type == ParameterType_InputProcessXML)
          {
          m_Application->SetParameterString(paramKey, values[0]);
          }
        else
          if (type == otb::Wrapper::ParameterType_Empty)
            {
            if (values[0] == "1" || values[0] == "true")
              {
              dynamic_cast<otb::Wrapper::EmptyParameter *> (param.GetPointer())->SetActive(true);
              }
            else
              if (values[0] == "0" || values[0] == "false")
                {
                dynamic_cast<otb::Wrapper::EmptyParameter *> (param.GetPointer())->SetActive(false);
                }
             else
              {
              std::cerr << "ERROR: Wrong parameter value: " << paramKey << std::endl;
              return WRONGPARAMETERVALUE;
              }
            }
        // Update the flag UserValue
        param->SetUserValue(true);
        // Call the DoUpdateParameter to update dependant params
        m_Application->UpdateParameters();
        }
      }
    }

  // SECOND PASS : check mandatory parameters
  for (unsigned int i = 0; i < appKeyList.size(); i++)
    {
    const std::string paramKey(appKeyList[i]);
    otb::Wrapper::Parameter::Pointer param = m_Application->GetParameterByKey(paramKey);
    otb::Wrapper::ParameterType type = m_Application->GetParameterType(paramKey);
    const bool paramExists(m_Parser->IsAttributExists(std::string("-").append(paramKey), m_VExpression));
    std::vector<std::string> values;

    // When a parameter is mandatory :
    // it must be set if :
    //  - The param is root
    //  - The param is not root and belonging to a Mandatory Group
    //    wich is activated
    bool mustBeSet = false;
    const bool hasValue = m_Application->HasValue(paramKey);

    //skip if mandatory parameters are missing because we have it already in XML
    if(!paramInXMLExists)
      {
      if( param->GetMandatory() == true && param->GetRole() != otb::Wrapper::Role_Output && type != otb::Wrapper::ParameterType_Group)
        {
        // check if the parameter is linked to a root parameter with a chain of active parameters
        if( param->IsRoot() || param->GetRoot()->IsRoot())
          {
          // the parameter is a root or inside a group at root level
          mustBeSet = true;
          }
        else
          {
            otb::Wrapper::Parameter* currentParam = param->GetRoot();
          while (!currentParam->IsRoot())
            {
            if (!currentParam->GetActive())
              {
              // the missing parameter is not on an active branch : we can ignore it
              break;
              }
            currentParam = currentParam->GetRoot();

            if (currentParam->IsRoot())
              {
              // the missing parameter is on an active branch : we need it
              mustBeSet = true;
              }
            }
          }
        }
      }

    if( mustBeSet )
      {
      if (!paramExists)
        {
        // If key doesn't exist and parameter hasn't default value set...
        if (!hasValue)
          {
          std::cerr << "ERROR: Missing mandatory parameter: " << paramKey << "  " << m_Application->HasValue(paramKey)
                    << std::endl;
          return MISSINGMANDATORYPARAMETER;
          }
        }
      else
        {
        values = m_Parser->GetAttribut(std::string("-").append(paramKey), m_VExpression);
        if (values.size() == 0 && !m_Application->HasValue(paramKey))
          {
          std::cerr << "ERROR: Missing mandatory parameter: " << paramKey << std::endl;
          return MISSINGPARAMETERVALUE;
          }
        }
      }
    // Check if non mandatory parameter have values
    else
      {
      if( paramExists )
        {
        values = m_Parser->GetAttribut(std::string("-").append(paramKey), m_VExpression);
        if (values.size() == 0)
          {
          std::cerr << "ERROR: Missing non-mandatory parameter: " << paramKey << std::endl;
          return MISSINGPARAMETERVALUE;
          }
        }
      }

    // Check output paths validity
    if (hasValue)
      {
      if (type == otb::Wrapper::ParameterType_OutputFilename)
        {
        std::string filename = m_Application->GetParameterString(paramKey);
        itksys::String path = itksys::SystemTools::GetFilenamePath(filename);
        if (!itksys::SystemTools::FileIsDirectory(path.c_str()))
          {
          std::cerr <<"ERROR: Directory doesn't exist : "<< path.c_str() << std::endl;
          return WRONGPARAMETERVALUE;
          }
        }
      }
    }

  return OKPARAM;
}

void MPICommandLineLauncher::LinkWatchers(itk::Object * itkNotUsed(caller), const itk::EventObject & event)
{
  // Report the progress only if asked
  if (m_ReportProgress)
    {

    if (typeid(otb::Wrapper::AddProcessToWatchEvent) == typeid( event ))
      {
      const otb::Wrapper::AddProcessToWatchEvent* eventToWatch = dynamic_cast<const otb::Wrapper::AddProcessToWatchEvent*> (&event);

      otb::StandardOneLineFilterWatcher * watch = new otb::StandardOneLineFilterWatcher(eventToWatch->GetProcess(),
                                                                              eventToWatch->GetProcessDescription());
      m_WatcherList.push_back(watch);
      }
    }
}

void MPICommandLineLauncher::DisplayHelp()
{
  std::cerr << "This is the "<<m_Application->GetName() << " application, version " << OTB_VERSION_STRING <<std::endl;

  std::cerr << m_Application->GetDescription() << std::endl;
  std::cerr<<std::endl;
  std::string link = "http://www.orfeo-toolbox.org/Applications/";
  link.append(m_Application->GetName());
  link.append(".html");

  std::cerr << "Complete documentation: " << link << std::endl;
  std::cerr<<std::endl;
  std::cerr << "Parameters: " << std::endl;

  const std::vector<std::string> appKeyList = m_Application->GetParametersKeys(true);
  const unsigned int nbOfParam = appKeyList.size();

  m_MaxKeySize = std::string("progress").size();
  for (unsigned int i = 0; i < nbOfParam; i++)
    {
    if (m_Application->GetParameterRole(appKeyList[i]) != otb::Wrapper::Role_Output)
      {
      if( m_MaxKeySize < appKeyList[i].size() )
        m_MaxKeySize = appKeyList[i].size();
      }
    }

  //// progress report parameter
  std::string bigKey = "progress";
  for(unsigned int i=0; i<m_MaxKeySize-std::string("progress").size(); i++)
    bigKey.append(" ");

  std::cerr << "        -"<<bigKey<<" <boolean>        Report progress " << std::endl;

  for (unsigned int i = 0; i < nbOfParam; i++)
    {
      otb::Wrapper::Parameter::Pointer param = m_Application->GetParameterByKey(appKeyList[i]);
      if (param->GetRole() != otb::Wrapper::Role_Output)
        {
        std::cerr << this->DisplayParameterHelp(param, appKeyList[i]);
        }
    }

  std::cerr<<std::endl;
  //std::string cl(m_Application->GetCLExample());

  std::cerr << "Examples: " << std::endl;
  std::cerr << m_Application->GetCLExample() << std::endl;

}


void MPICommandLineLauncher::LoadTestEnv()
{
  //Set seed for rand and itk mersenne twister
  //srand(1);
 // itk::Statistics::MersenneTwisterRandomVariateGenerator::GetInstance()->SetSeed(121212);
}


std::string MPICommandLineLauncher::DisplayParameterHelp(const otb::Wrapper::Parameter::Pointer & param, const std::string paramKey)
{
  // Display the type the type
    otb::Wrapper::ParameterType type = m_Application->GetParameterType(paramKey);

  // Discard Group parameters (they don't need a value)
  if (type == otb::Wrapper::ParameterType_Group)
    {
    return "";
    }

  std::ostringstream oss;

  // When a parameter is mandatory :
  // it must be set if :
  //  - The param is root
  //  - The param is not root and belonging to a Mandatory Group
  //    wich is activated
  bool isMissing = false;
  if (!m_Parser->IsAttributExists(std::string("-").append(paramKey), m_VExpression))
    {
    if (!m_Application->HasValue(paramKey))
      {
      if( param->GetMandatory() && param->GetRole() != otb::Wrapper::Role_Output )
        {
        if( param->IsRoot() || param->GetRoot()->IsRoot())
          {
          // the parameter is a root or inside a group at root level
          isMissing = true;
          }
        else
          {
            otb::Wrapper::Parameter* currentParam = param->GetRoot();
          while (!currentParam->IsRoot())
            {
            if (!currentParam->GetActive())
              {
              // the missing parameter is not on an active branch : we can ignore it
              break;
              }
            currentParam = currentParam->GetRoot();

            if (currentParam->IsRoot())
              {
              // the missing parameter is on an active branch : we need it
              isMissing = true;
              }
            }
          }
        }
      }
    }

  if( isMissing )
    {
    oss << "MISSING ";
    }
  else
    {
    oss << "        ";
    }

  std::string bigKey = paramKey;
  for(unsigned int i=0; i<m_MaxKeySize-paramKey.size(); i++)
    bigKey.append(" ");

  oss<< "-" << bigKey << " ";

  // Display the type the parameter
  if (type == otb::Wrapper::ParameterType_Radius || type == otb::Wrapper::ParameterType_Int || type == otb::Wrapper::ParameterType_RAM)
    {
    oss << "<int32>         ";
    }
  else if (type == otb::Wrapper::ParameterType_Empty )
    {
    oss << "<boolean>       ";
    }
  else if (type == otb::Wrapper::ParameterType_Float)
    {
    oss << "<float>         ";
    }
  else if (type == otb::Wrapper::ParameterType_InputFilename || type == otb::Wrapper::ParameterType_OutputFilename ||type == otb::Wrapper::ParameterType_Directory || type == otb::Wrapper::ParameterType_InputImage || type == otb::Wrapper::ParameterType_OutputProcessXML || type == otb::Wrapper::ParameterType_InputProcessXML ||
           type == otb::Wrapper::ParameterType_ComplexInputImage || type == otb::Wrapper::ParameterType_InputVectorData || type == otb::Wrapper::ParameterType_OutputVectorData ||
           type == otb::Wrapper::ParameterType_String || type == otb::Wrapper::ParameterType_Choice )
    {
    oss << "<string>        ";
    }
  else if (type == otb::Wrapper::ParameterType_OutputImage)
    {
    oss << "<string> [pixel]";
    }
  else if (type == otb::Wrapper::ParameterType_Choice || type == otb::Wrapper::ParameterType_ListView || otb::Wrapper::ParameterType_InputImageList ||
           type == otb::Wrapper::ParameterType_InputVectorDataList || type == otb::Wrapper::ParameterType_InputFilenameList ||
           type == otb::Wrapper::ParameterType_StringList )
    {
    oss << "<string list>   ";
    }
  else
    itkExceptionMacro("Not handled parameter type.");


  oss<< " " << param->GetName() << " ";

  if (type == otb::Wrapper::ParameterType_OutputImage)
    {
    oss << " [pixel=uint8/uint16/int16/uint32/int32/float/double]";
    oss << " (default value is float)";
    }


  if (type == otb::Wrapper::ParameterType_Choice)
    {
    std::vector<std::string> keys = dynamic_cast<ChoiceParameter*>(param.GetPointer())->GetChoiceKeys();
    std::vector<std::string> names = dynamic_cast<ChoiceParameter*>(param.GetPointer())->GetChoiceNames();

    oss << "[";
    for(unsigned int i=0; i<keys.size(); i++)
      {
      oss<<keys[i];
      if( i != keys.size()-1 )
        oss << "/";
      }

    oss << "]";
    }

  if(m_Application->IsMandatory(paramKey))
    {
    oss<<" (mandatory";
    }
  else
    {
    oss<<" (optional";

    if(m_Application->IsParameterEnabled(paramKey))
      {
      oss<<", on by default";
      }
    else
      {
      oss<<", off by default";
      }
    }

  if(m_Application->HasValue(paramKey))
    {
    oss<<", default value is "<<m_Application->GetParameterAsString(paramKey);
    }
  oss<<")";

  oss << std::endl;
  return oss.str();
}

bool MPICommandLineLauncher::CheckUnicity()
{
  bool res = true;
  // Extract expression keys
  //std::vector<std::string> keyList = m_Parser->GetKeyList(m_Expression);
  std::vector<std::string> keyList = m_Parser->GetKeyList(m_VExpression);

  // Check Unicity
  for (unsigned int i = 0; i < keyList.size(); i++)
    {
    std::vector<std::string> listTmp = keyList;
    const std::string keyRef = keyList[i];
    listTmp.erase(listTmp.begin() + i);
    for (unsigned int j = 0; j < listTmp.size(); j++)
      {
      if (keyRef == listTmp[j])
        {
        res = false;
        break;
        }
      }
    if (!res)
      break;
    }

  return res;
}

bool MPICommandLineLauncher::CheckParametersPrefix()
{
  // Check if the chain " --" appears in the args, could be a common mistake
  for (std::vector<std::string>::iterator it = m_VExpression.begin(); it != m_VExpression.end(); ++it)
    {
    if (it->find("--") != std::string::npos )
      {
      return false;
      }
    }
  return true;
}

bool MPICommandLineLauncher::CheckKeyValidity(std::string& refKey)
{
  bool res = true;
  // Extract expression keys
  std::vector<std::string> expKeyList = m_Parser->GetKeyList(m_VExpression);

  // Extract application keys
  std::vector<std::string> appKeyList = m_Application->GetParametersKeys(true);
  appKeyList.push_back("help");
  appKeyList.push_back("progress");
  appKeyList.push_back("testenv");
  appKeyList.push_back("version");

  // Check if each key in the expression exists in the application
  for (unsigned int i = 0; i < expKeyList.size(); i++)
    {
    refKey = expKeyList[i];
    bool keyExist = false;
    for (unsigned int j = 0; j < appKeyList.size(); j++)
      {
      if (refKey == appKeyList[j])
        {
        keyExist = true;
        break;
        }
      }
    if (keyExist == false)
      {
      res = false;
      break;
      }
    }

  return res;
}

void MPICommandLineLauncher::DisplayOutputParameters()
{
  std::vector< std::pair<std::string, std::string> > paramList;
  paramList = m_Application->GetOutputParametersSumUp();
  if( paramList.size() == 0 )
    return;

  std::ostringstream oss;
  for( unsigned int i=0; i<paramList.size(); i++)
    {
    oss << paramList[i].first;
    oss << ": ";
    oss << paramList[i].second;
    oss << std::endl;
    }


  if ( m_Parser->IsAttributExists("-testenv", m_VExpression) )
    {
    std::vector<std::string> val = m_Parser->GetAttribut("-testenv", m_VExpression);
    if( val.size() == 1 )
      {
      std::ofstream ofs(val[0].c_str());
      if (!ofs.is_open())
        {
        fprintf(stderr, "Error, can't open file");
        itkExceptionMacro( << "Error, can't open file "<<val[0]<<".");
        }
      ofs << oss.str();
      ofs.close();
      }
    }

  std::cout << "Output parameters value:" << std::endl;
  std::cout << oss.str() << std::endl;
}

}
}
