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
#ifndef __otbMPIWrapperCommandLineLauncher_h
#define __otbMPIWrapperCommandLineLauncher_h


#include "otbMPIWrapperApplication.h"
#include "otbWrapperParameter.h"
#include "itksys/SystemTools.hxx"
#include "otbWrapperCommandLineParser.h"
#include "otbWrapperChoiceParameter.h"
#include "itkStdStreamLogOutput.h"
#include "otbStandardOneLineFilterWatcher.h"
#include "itkCommand.h"

#include <vector>

#include "otbWrapperCommandLineLauncher.h"

namespace otb
{
namespace Wrapper
{

/** \class MPICommandLineLauncher
 *  \brief This class check the validity of a command line application.
 *
 * To be valid, the expression must be as follow:
 * ModuleName (ModulePath) --attribut1_Key attribut1_Value --attribut2_Key
 * attribut2_Value
 * After the attribute key, if the user give several values (expression
 * without \"--\" separated by space), it will automatically be
 * interpreted as a list.
 */

class ITK_ABI_EXPORT MPICommandLineLauncher : public CommandLineLauncher
{
public:
  /** Standard class typedefs. */
  typedef MPICommandLineLauncher        Self;
  typedef CommandLineLauncher           Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(MPICommandLineLauncher, itk::Object);

  /** Command Member */
  typedef itk::MemberCommand< Self >        AddProcessCommandType;

  /** Load the application in several steps :
   * - Load the paths
   * - Load the application using the ApplicationRegistry
   */
  bool Load();

  /** same as Load method but set the expression before. */
  bool Load( const std::string & exp );

  //        /** same as Load method but set the expression before. */
  bool Load(const std::vector<std::string> &vexp);

  /** Launch the process, using the Execute application method
   * The method will check if the user asked for help (looking at
   * --help key) before loading parameter and launching process.
   **/
  bool Execute();

  /** Launch the process, using the ExecuteAndWriteOutput application
   * method
   * The method will check if the user asked for help (looking at
   * --help key) before loading parameter and launching process.
   */
  bool ExecuteAndWriteOutput();

  /** Gather the code process to do before Execute and
   * ExecuteAndWriteOutput
   * Returns false if a problem occurs, true otherwise.
   */
  bool BeforeExecute();

  /** Create and display the help of the application */
  void DisplayHelp();

  /** Performs specific action for testing environment */
  void LoadTestEnv();

  /** MPI params accessors */
  itkSetMacro(MyRank, int);
  itkGetMacro(MyRank, int);
  itkSetMacro(NProcs, int);
  itkGetMacro(NProcs, int);

protected:
  /** Constructor */
  MPICommandLineLauncher();

  MPICommandLineLauncher(const char * exp);

  /** Destructor */
  virtual ~MPICommandLineLauncher();

  /** Load the executable path. It looks for the key --modulePath,
   * extract and interpret as path the following strings.
   */
  bool LoadPath();

  /** Load the application calling the CreateApplication method of the
   * ApplicationRegistry classes.
   * Pay attention, the executable paths have to be loaded or set in
   * the environment before calling the function.
   */
  void LoadApplication();

  /** Parse the user expression, extract the key and the associated
   * string and set it as value of each corresonding application
   *  parameter.
   */
  MPICommandLineLauncher::ParamResultType LoadParameters();
  //        MPICommandLineLauncher::ParamResultType LoadParameters()
  //        {
  //          return Superclass::LoadParameters();
  //        }

  /** Create and display the help of the application */
  std::string DisplayParameterHelp( const otb::Wrapper::Parameter::Pointer & param,
      const std::string paramKey );

  /** Check if each key is unique in the expression. */
  bool CheckUnicity();

  /** Check if option contains at least one "-". */
  bool CheckParametersPrefix();

  /** Check if each key is valid for the application. */
  bool CheckKeyValidity(std::string& key);

  /** Display the output parameter (Role == RoleOutput) */
  void DisplayOutputParameters();

  /** Load the watchers for internal progress and writeing progress report. */
  void LinkWatchers(itk::Object * caller, const itk::EventObject & event);

  /** Clear watcher list, deleting its pointers. */
  void DeleteWatcherList();

private:

  MPICommandLineLauncher(const MPICommandLineLauncher &); //purposely not implemented
  void operator =(const MPICommandLineLauncher&); //purposely not implemented

  std::string                       m_Path;

  MPIApplication::Pointer              m_Application;
  //std::string                       m_Expression;
  std::vector<std::string>          m_VExpression;
  CommandLineParser::Pointer        m_Parser;

  WatcherListType                   m_WatcherList;

  itk::StdStreamLogOutput::Pointer  m_LogOutput;

  AddProcessCommandType::Pointer    m_AddProcessCommand;
  bool                              m_ReportProgress;

  unsigned int m_MaxKeySize;

  int m_MyRank;
  int m_NProcs;

}; //end class

} // end namespace Wrapper
} //end namespace gtb

#endif
