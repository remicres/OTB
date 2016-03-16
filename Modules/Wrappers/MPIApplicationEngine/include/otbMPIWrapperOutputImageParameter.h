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
#ifndef __otbMPIWrapperOutputImageParameter_h
#define __otbMPIWrapperOutputImageParameter_h

//#include "otbVectorImage.h"
#include "itkImageBase.h"
#include "otbWrapperParameter.h"
#include "otbSimpleParallelTiffWriter.h"

namespace otb
{
namespace Wrapper
{
/** \class OutputImageParameter
 *  \brief This class represents a OutputImage parameter
 *
 * \ingroup OTBApplicationEngine
 */

class ITK_ABI_EXPORT MPIOutputImageParameter : public Parameter
{
public:
  /** Standard class typedef */
  typedef MPIOutputImageParameter          Self;
  typedef Parameter                     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef itk::ImageBase<2> ImageBaseType;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(MPIOutputImageParameter, Parameter);

  /** Set the value */
  itkSetObjectMacro(Image, ImageBaseType);

  /** Get the value */
  itkGetObjectMacro(Image, ImageBaseType);

  /** Return any value */
  void SetValue(ImageBaseType* image);

  /** Return any value */
  ImageBaseType* GetValue( void );

  /** Set/Get PixelType to be used when saving */
  itkSetMacro(PixelType, ImagePixelType);
  itkGetMacro(PixelType, ImagePixelType);

  /** Set/Get DefaultPixelType  */
  itkSetMacro(DefaultPixelType, ImagePixelType);
  itkGetMacro(DefaultPixelType, ImagePixelType);

  /** Set/Get available RAM value */
  itkSetMacro(RAMValue, unsigned int);
  itkGetMacro(RAMValue, unsigned int);

  /** MPI parameters accessors */
  itkGetMacro(MyRank, int);
  itkGetMacro(NProcs, int);
  itkSetMacro(MyRank, int);
  itkSetMacro(NProcs, int);

  /** Implement the reset method (replace pixel type by default type) */
  virtual void Reset()
  {
    m_PixelType = m_DefaultPixelType;
  }

  /** Static method to convert pixel type into string */
  static std::string ConvertPixelTypeToString(ImagePixelType type);

  /** Return true if a filename is set */
  bool HasValue() const;

  void SetFileName (const char* filename)
  {
    m_FileName = filename;
    SetActive(true);
  }
  void SetFileName (const std::string& filename)
  {
    this->SetFileName(filename.c_str());
  }

  itkGetStringMacro(FileName);

  void Write();

  itk::ProcessObject* GetWriter();

  void InitializeWriters();

protected:
  /** Constructor */
  MPIOutputImageParameter();
  /** Destructor */
  virtual ~MPIOutputImageParameter();

  template <class TInputImageType>
    void SwitchImageWrite();

  template <class TInputVectorImageType>
    void SwitchVectorImageWrite();

  template <class TInputVectorImageType>
    void SwitchRGBImageWrite();

  template <class TInputVectorImageType>
    void SwitchRGBAImageWrite();

  //FloatVectorImageType::Pointer m_Image;
  ImageBaseType::Pointer m_Image;
  std::string            m_FileName;
  ImagePixelType         m_PixelType;
  ImagePixelType         m_DefaultPixelType;

  typedef otb::SimpleParallelTiffWriter<UInt8ImageType>  UInt8WriterType;
  typedef otb::SimpleParallelTiffWriter<Int16ImageType>  Int16WriterType;
  typedef otb::SimpleParallelTiffWriter<UInt16ImageType> UInt16WriterType;
  typedef otb::SimpleParallelTiffWriter<Int32ImageType>  Int32WriterType;
  typedef otb::SimpleParallelTiffWriter<UInt32ImageType> UInt32WriterType;
  typedef otb::SimpleParallelTiffWriter<FloatImageType>  FloatWriterType;
  typedef otb::SimpleParallelTiffWriter<DoubleImageType> DoubleWriterType;

  typedef otb::SimpleParallelTiffWriter<UInt8VectorImageType>  VectorUInt8WriterType;
  typedef otb::SimpleParallelTiffWriter<Int16VectorImageType>  VectorInt16WriterType;
  typedef otb::SimpleParallelTiffWriter<UInt16VectorImageType> VectorUInt16WriterType;
  typedef otb::SimpleParallelTiffWriter<Int32VectorImageType>  VectorInt32WriterType;
  typedef otb::SimpleParallelTiffWriter<UInt32VectorImageType> VectorUInt32WriterType;
  typedef otb::SimpleParallelTiffWriter<FloatVectorImageType>  VectorFloatWriterType;
  typedef otb::SimpleParallelTiffWriter<DoubleVectorImageType> VectorDoubleWriterType;

  typedef otb::SimpleParallelTiffWriter<UInt8RGBAImageType>  RGBAUInt8WriterType;
  typedef otb::SimpleParallelTiffWriter<UInt8RGBImageType>   RGBUInt8WriterType;

  UInt8WriterType::Pointer  m_UInt8Writer;
  Int16WriterType::Pointer  m_Int16Writer;
  UInt16WriterType::Pointer m_UInt16Writer;
  Int32WriterType::Pointer  m_Int32Writer;
  UInt32WriterType::Pointer m_UInt32Writer;
  FloatWriterType::Pointer  m_FloatWriter;
  DoubleWriterType::Pointer m_DoubleWriter;

  VectorUInt8WriterType::Pointer  m_VectorUInt8Writer;
  VectorInt16WriterType::Pointer  m_VectorInt16Writer;
  VectorUInt16WriterType::Pointer m_VectorUInt16Writer;
  VectorInt32WriterType::Pointer  m_VectorInt32Writer;
  VectorUInt32WriterType::Pointer m_VectorUInt32Writer;
  VectorFloatWriterType::Pointer  m_VectorFloatWriter;
  VectorDoubleWriterType::Pointer m_VectorDoubleWriter;

  RGBUInt8WriterType::Pointer   m_RGBUInt8Writer;
  RGBAUInt8WriterType::Pointer  m_RGBAUInt8Writer;

private:
  MPIOutputImageParameter(const Parameter &); //purposely not implemented
  void operator =(const Parameter&); //purposely not implemented

  unsigned int                  m_RAMValue;

  int m_MyRank;
  int m_NProcs;

}; // End class OutputImage Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
