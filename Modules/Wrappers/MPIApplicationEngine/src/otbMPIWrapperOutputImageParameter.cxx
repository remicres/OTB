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
#include "otbMPIWrapperOutputImageParameter.h"
#include "otbClampImageFilter.h"
#include "otbClampVectorImageFilter.h"

namespace otb
{
namespace Wrapper
{

MPIOutputImageParameter::MPIOutputImageParameter()
  : m_PixelType(ImagePixelType_float),
    m_DefaultPixelType(ImagePixelType_float),
    m_RAMValue(0)
{
  this->SetName("Output Image");
  this->SetKey("out");
}


MPIOutputImageParameter::~MPIOutputImageParameter()
{
}

std::string MPIOutputImageParameter::ConvertPixelTypeToString(ImagePixelType type)
{
  std::string ret;
  switch(type)
    {
    case ImagePixelType_uint8:
      {
      ret = "uint8";
      break;
      }
    case ImagePixelType_int16:
      {
      ret = "int16";
      break;
      }
    case ImagePixelType_uint16:
      {
      ret = "uint16";
      break;
      }
    case ImagePixelType_int32:
      {
      ret = "int32";
      break;
      }
    case ImagePixelType_uint32:
      {
      ret = "uint32";
      break;
      }
    case ImagePixelType_float:
      {
      ret = "float";
      break;
      }
    case ImagePixelType_double:
      {
      ret = "double";
      break;
      }
    }
  return ret;
}

void MPIOutputImageParameter::InitializeWriters()
{
  m_UInt8Writer = UInt8WriterType::New();
  m_UInt8Writer->SetMyRank(this->GetMyRank());
  m_UInt8Writer->SetNProcs(this->GetNProcs());

  m_Int16Writer = Int16WriterType::New();
  m_Int16Writer->SetMyRank(this->GetMyRank());
  m_Int16Writer->SetNProcs(this->GetNProcs());

  m_UInt16Writer = UInt16WriterType::New();
  m_Int16Writer->SetMyRank(this->GetMyRank());
  m_Int16Writer->SetNProcs(this->GetNProcs());

  m_Int32Writer = Int32WriterType::New();
  m_Int32Writer->SetMyRank(this->GetMyRank());
  m_Int32Writer->SetNProcs(this->GetNProcs());

  m_UInt32Writer = UInt32WriterType::New();
  m_UInt32Writer->SetMyRank(this->GetMyRank());
  m_UInt32Writer->SetNProcs(this->GetNProcs());

  m_FloatWriter = FloatWriterType::New();
  m_FloatWriter->SetMyRank(this->GetMyRank());
  m_FloatWriter->SetNProcs(this->GetNProcs());

  m_DoubleWriter = DoubleWriterType::New();
  m_DoubleWriter->SetMyRank(this->GetMyRank());
  m_DoubleWriter->SetNProcs(this->GetNProcs());


  m_VectorUInt8Writer = VectorUInt8WriterType::New();
  m_VectorUInt8Writer->SetMyRank(this->GetMyRank());
  m_VectorUInt8Writer->SetNProcs(this->GetNProcs());

  m_VectorInt16Writer = VectorInt16WriterType::New();
  m_VectorInt16Writer->SetMyRank(this->GetMyRank());
  m_VectorInt16Writer->SetNProcs(this->GetNProcs());

  m_VectorUInt16Writer = VectorUInt16WriterType::New();
  m_VectorUInt16Writer->SetMyRank(this->GetMyRank());
  m_VectorUInt16Writer->SetNProcs(this->GetNProcs());

  m_VectorInt32Writer = VectorInt32WriterType::New();
  m_VectorInt32Writer->SetMyRank(this->GetMyRank());
  m_VectorInt32Writer->SetNProcs(this->GetNProcs());

  m_VectorUInt32Writer = VectorUInt32WriterType::New();
  m_VectorUInt32Writer->SetMyRank(this->GetMyRank());
  m_VectorUInt32Writer->SetNProcs(this->GetNProcs());

  m_VectorFloatWriter = VectorFloatWriterType::New();
  m_VectorFloatWriter->SetMyRank(this->GetMyRank());
  m_VectorFloatWriter->SetNProcs(this->GetNProcs());

  m_VectorDoubleWriter = VectorDoubleWriterType::New();
  m_VectorDoubleWriter->SetMyRank(this->GetMyRank());
  m_VectorDoubleWriter->SetNProcs(this->GetNProcs());


  m_RGBUInt8Writer = RGBUInt8WriterType::New();
  m_RGBUInt8Writer->SetMyRank(this->GetMyRank());
  m_RGBUInt8Writer->SetNProcs(this->GetNProcs());

  m_RGBAUInt8Writer = RGBAUInt8WriterType::New();
  m_RGBAUInt8Writer->SetMyRank(this->GetMyRank());
  m_RGBAUInt8Writer->SetNProcs(this->GetNProcs());
}


#define otbClampAndWriteImageMacro(InputImageType, OutputImageType, writer)         \
  {                                                                                 \
    typedef otb::ClampImageFilter<InputImageType, OutputImageType> ClampFilterType; \
    typename ClampFilterType::Pointer clampFilter = ClampFilterType::New();         \
    clampFilter->SetInput( dynamic_cast<InputImageType*>(m_Image.GetPointer()) );   \
    writer->SetFileName( this->GetFileName() );                                     \
    writer->SetInput(clampFilter->GetOutput());                                     \
    writer->SetAutomaticAdaptativeStreaming(m_RAMValue);                            \
    writer->Update();                                                               \
  }

#define otbClampAndWriteVectorImageMacro(InputImageType, OutputImageType, writer)         \
  {                                                                                       \
    typedef otb::ClampVectorImageFilter<InputImageType, OutputImageType> ClampFilterType; \
    typename ClampFilterType::Pointer clampFilter = ClampFilterType::New();               \
    clampFilter->SetInput( dynamic_cast<InputImageType*>(m_Image.GetPointer()) );         \
    writer->SetFileName(this->GetFileName() );                                            \
    writer->SetInput(clampFilter->GetOutput());                                           \
    writer->SetAutomaticAdaptativeStreaming(m_RAMValue);                                  \
    writer->Update();                                                                     \
  }


template <class TInputImageType>
void
MPIOutputImageParameter::SwitchImageWrite()
{
  switch(m_PixelType )
    {
    case ImagePixelType_uint8:
    {
    otbClampAndWriteImageMacro(TInputImageType, UInt8ImageType, m_UInt8Writer);
    break;
    }
    case ImagePixelType_int16:
    {
    otbClampAndWriteImageMacro(TInputImageType, Int16ImageType, m_Int16Writer);
    break;
    }
    case ImagePixelType_uint16:
    {
    otbClampAndWriteImageMacro(TInputImageType, UInt16ImageType, m_UInt16Writer);
    break;
    }
    case ImagePixelType_int32:
    {
    otbClampAndWriteImageMacro(TInputImageType, Int32ImageType, m_Int32Writer);
    break;
    }
    case ImagePixelType_uint32:
    {
    otbClampAndWriteImageMacro(TInputImageType, UInt32ImageType, m_UInt32Writer);
    break;
    }
    case ImagePixelType_float:
    {
    otbClampAndWriteImageMacro(TInputImageType, FloatImageType, m_FloatWriter);
    break;
    }
    case ImagePixelType_double:
    {
    otbClampAndWriteImageMacro(TInputImageType, DoubleImageType, m_DoubleWriter);
    break;
    }
    }
}


template <class TInputVectorImageType>
void
MPIOutputImageParameter::SwitchVectorImageWrite()
  {
  switch(m_PixelType )
    {
    case ImagePixelType_uint8:
    {
    otbClampAndWriteVectorImageMacro(TInputVectorImageType, UInt8VectorImageType, m_VectorUInt8Writer);
    break;
    }
    case ImagePixelType_int16:
    {
    otbClampAndWriteVectorImageMacro(TInputVectorImageType, Int16VectorImageType, m_VectorInt16Writer);
    break;
    }
    case ImagePixelType_uint16:
    {
    otbClampAndWriteVectorImageMacro(TInputVectorImageType, UInt16VectorImageType, m_VectorUInt16Writer);
    break;
    }
    case ImagePixelType_int32:
    {
    otbClampAndWriteVectorImageMacro(TInputVectorImageType, Int32VectorImageType, m_VectorInt32Writer);
    break;
    }
    case ImagePixelType_uint32:
    {
    otbClampAndWriteVectorImageMacro(TInputVectorImageType, UInt32VectorImageType, m_VectorUInt32Writer);
    break;
    }
    case ImagePixelType_float:
    {
    otbClampAndWriteVectorImageMacro(TInputVectorImageType, FloatVectorImageType, m_VectorFloatWriter);
    break;
    }
    case ImagePixelType_double:
    {
    otbClampAndWriteVectorImageMacro(TInputVectorImageType, DoubleVectorImageType, m_VectorDoubleWriter);
    break;
    }
    }
  }


template <class TInputRGBAImageType>
void
MPIOutputImageParameter::SwitchRGBAImageWrite()
  {
  if( m_PixelType == ImagePixelType_uint8 )
    {
    m_RGBAUInt8Writer->SetFileName( this->GetFileName() );
    m_RGBAUInt8Writer->SetInput(dynamic_cast<UInt8RGBAImageType*>(m_Image.GetPointer()) );
    m_RGBAUInt8Writer->SetAutomaticAdaptativeStreaming(m_RAMValue);
    m_RGBAUInt8Writer->Update();
    }
   else
     itkExceptionMacro("Unknown PixelType for RGBA Image. Only uint8 is supported.");
  }

template <class TInputRGBImageType>
void
MPIOutputImageParameter::SwitchRGBImageWrite()
  {
   if( m_PixelType == ImagePixelType_uint8 )
    {
    m_RGBUInt8Writer->SetFileName( this->GetFileName() );
    m_RGBUInt8Writer->SetInput(dynamic_cast<UInt8RGBImageType*>(m_Image.GetPointer()) );
    m_RGBUInt8Writer->SetAutomaticAdaptativeStreaming(m_RAMValue);
    m_RGBUInt8Writer->Update();
    }
   else
     itkExceptionMacro("Unknown PixelType for RGB Image. Only uint8 is supported.");
  }

void
MPIOutputImageParameter::Write()
{
  m_Image->UpdateOutputInformation();

  if (dynamic_cast<UInt8ImageType*>(m_Image.GetPointer()))
    {
    SwitchImageWrite<UInt8ImageType>();
    }
  else if (dynamic_cast<Int16ImageType*>(m_Image.GetPointer()))
    {
    SwitchImageWrite<Int16ImageType>();
    }
  else if (dynamic_cast<UInt16ImageType*>(m_Image.GetPointer()))
    {
    SwitchImageWrite<UInt16ImageType>();
    }
  else if (dynamic_cast<Int32ImageType*>(m_Image.GetPointer()))
    {
    SwitchImageWrite<Int32ImageType>();
    }
  else if (dynamic_cast<UInt32ImageType*>(m_Image.GetPointer()))
    {
    SwitchImageWrite<UInt32ImageType>();
    }
  else if (dynamic_cast<FloatImageType*>(m_Image.GetPointer()))
    {
    SwitchImageWrite<FloatImageType>();
    }
  else if (dynamic_cast<DoubleImageType*>(m_Image.GetPointer()))
    {
    SwitchImageWrite<DoubleImageType>();
    }
  else if (dynamic_cast<UInt8VectorImageType*>(m_Image.GetPointer()))
    {
    SwitchVectorImageWrite<UInt8VectorImageType>();
    }
  else if (dynamic_cast<Int16VectorImageType*>(m_Image.GetPointer()))
    {
    SwitchVectorImageWrite<Int16VectorImageType>();
    }
  else if (dynamic_cast<UInt16VectorImageType*>(m_Image.GetPointer()))
    {
    SwitchVectorImageWrite<UInt16VectorImageType>();
    }
  else if (dynamic_cast<Int32VectorImageType*>(m_Image.GetPointer()))
    {
    SwitchVectorImageWrite<Int32VectorImageType>();
    }
  else if (dynamic_cast<UInt32VectorImageType*>(m_Image.GetPointer()))
    {
    SwitchVectorImageWrite<UInt32VectorImageType>();
    }
  else if (dynamic_cast<FloatVectorImageType*>(m_Image.GetPointer()))
    {
    SwitchVectorImageWrite<FloatVectorImageType>();
    }
  else if (dynamic_cast<DoubleVectorImageType*>(m_Image.GetPointer()))
    {
    SwitchVectorImageWrite<DoubleVectorImageType>();
    }
  else if (dynamic_cast<UInt8RGBImageType*>(m_Image.GetPointer()))
    {
    SwitchRGBImageWrite<UInt8RGBImageType>();
    }
  else if (dynamic_cast<UInt8RGBAImageType*>(m_Image.GetPointer()))
    {
    SwitchRGBAImageWrite<UInt8RGBAImageType>();
    }
  else
    {
    itkExceptionMacro("Unknown image type");
    }
  }


itk::ProcessObject*
MPIOutputImageParameter::GetWriter()
{
  int type = 0;
  // 0 : image
  // 1 : VectorImage
  // 2 : RGBAImage
  // 3 : RGBImage
  itk::ProcessObject* writer = 0;
  if (dynamic_cast<UInt8VectorImageType*> (m_Image.GetPointer())
      || dynamic_cast<Int16VectorImageType*> (m_Image.GetPointer())
      || dynamic_cast<UInt16VectorImageType*> (m_Image.GetPointer())
      || dynamic_cast<Int32VectorImageType*> (m_Image.GetPointer())
      || dynamic_cast<UInt32VectorImageType*> (m_Image.GetPointer())
      || dynamic_cast<FloatVectorImageType*> (m_Image.GetPointer())
      || dynamic_cast<DoubleVectorImageType*> (m_Image.GetPointer()))
    {
    type = 1;
    }
  else if (dynamic_cast<UInt8RGBAImageType*> (m_Image.GetPointer()))
    {
    type = 2;
    writer = m_RGBAUInt8Writer;
    itkWarningMacro("UInt8RGBAImageType will be saved in UInt8 format.");
    return writer;
    }
  else if (dynamic_cast<UInt8RGBImageType*> (m_Image.GetPointer()))
    {
    type = 3;
    writer = m_RGBUInt8Writer;
    itkWarningMacro("UInt8RGBImageType will be saved in UInt8 format.");
    return writer;
    }
  
  switch (GetPixelType())
    {
    case ImagePixelType_uint8:
      {
      switch(type)
        {
        case 0:
          writer = m_UInt8Writer;
          break;
        case 1:
          writer = m_VectorUInt8Writer;
          break;
        case 2:
          writer = m_RGBAUInt8Writer;
          break;
        default:
          writer = m_RGBUInt8Writer;
          break;
        }
      break;
      }
    case ImagePixelType_int16:
      {
      if (type == 1)
        writer = m_VectorInt16Writer;
      else
        if (type == 0) writer = m_Int16Writer;
      break;
      }
    case ImagePixelType_uint16:
      {
      if (type == 1)
        writer = m_VectorUInt16Writer;
      else
        if (type == 0) writer = m_UInt16Writer;
      break;
      }
    case ImagePixelType_int32:
      {
      if (type == 1)
        writer = m_VectorInt32Writer;
      else
        if (type == 0) writer = m_Int32Writer;
      break;
      }
    case ImagePixelType_uint32:
      {
      if (type == 1)
        writer = m_VectorUInt32Writer;
      else
        if (type == 0) writer = m_UInt32Writer;
      break;
      }
    case ImagePixelType_float:
      {
      if (type == 1)
        writer = m_VectorFloatWriter;
      else
        if (type == 0) writer = m_FloatWriter;
      break;
      }
    case ImagePixelType_double:
      {
      if (type == 1)
        writer = m_VectorDoubleWriter;
      else
        if (type == 0) writer = m_DoubleWriter;
      break;
      }
    }
  if (0 == writer)
    {
    itkExceptionMacro("Unknown Writer type.");
    }

  return writer;
}

MPIOutputImageParameter::ImageBaseType*
MPIOutputImageParameter::GetValue( )
{
  return m_Image;
}

void
MPIOutputImageParameter::SetValue(ImageBaseType* image)
{
  m_Image = image;
  SetActive(true);
}

bool
MPIOutputImageParameter::HasValue() const
{
  std::string filename(this->GetFileName());
  return !filename.empty();
}

}
}
