/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


// Software Guide : BeginCommandLineArgs
// INPUTS:  {ROISpot5.png}
// OUTPUTS: {BinaryThresholdImageFilterOutput.png}
// 150 180 0 255
// Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// \itkpiccaption[BinaryThresholdImageFilter transfer function]{Transfer function
// of the BinaryThresholdImageFilter.
// \label{fig:BinaryThresholdTransferFunction}}
// \parpic(8cm, 5cm)[r]{\includegraphics[width=7cm]{BinaryThresholdTransferFunction.eps}}
//
// This example illustrates the use of the binary threshold image filter.
// This filter is used to transform an image into a binary image by changing
// the pixel values according to the rule illustrated in
// Figure~\ref{fig:BinaryThresholdTransferFunction}. The user defines two
// thresholds---Upper and Lower---and two intensity values---Inside and
// Outside. For each pixel in the input image, the value of the pixel is
// compared with the lower and upper thresholds. If the pixel value is inside
// the range defined by $[Lower, Upper]$ the output pixel is assigned the
// InsideValue. Otherwise the output pixels are assigned to the OutsideValue.
// Thresholding is commonly applied as the last operation of a segmentation
// pipeline.
//
// \index{itk::Binary\-Threshold\-Image\-Filter!Instantiation}
// \index{itk::Binary\-Threshold\-Image\-Filter!Header}
//
// The first step required to use the \doxygen{itk}{BinaryThresholdImageFilter} is
// to include its header file.
//
// Software Guide : EndLatex

#include "itkUnaryFunctorImageFilter.h"
// Software Guide : BeginCodeSnippet
#include "itkBinaryThresholdImageFilter.h"
// Software Guide : EndCodeSnippet

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int main(int argc, char * argv[])
{
  if (argc < 7)
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImageFile outputImageFile ";
    std::cerr << " lowerThreshold upperThreshold ";
    std::cerr << " outsideValue insideValue   "  << std::endl;
    return EXIT_FAILURE;
    }

  //  Software Guide : BeginLatex
  //
  //  The next step is to decide which pixel types to use for the input and output
  //  images.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef  unsigned char InputPixelType;
  typedef  unsigned char OutputPixelType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The input and output image types are now defined using their respective
  //  pixel types and dimensions.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::Image<InputPixelType,  2> InputImageType;
  typedef otb::Image<OutputPixelType, 2> OutputImageType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The filter type can be instantiated using the input and output image
  //  types defined above.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::BinaryThresholdImageFilter<
      InputImageType, OutputImageType>  FilterType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  An \doxygen{otb}{ImageFileReader} class is also instantiated in order to read
  //  image data from a file. (See Section \ref{sec:IO} on page
  //  \pageref{sec:IO} for more information about reading
  //  and writing data.)
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::ImageFileReader<InputImageType> ReaderType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // An \doxygen{otb}{ImageFileWriter} is instantiated in order to write the output
  // image to a file.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::ImageFileWriter<InputImageType> WriterType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  Both the filter and the reader are created by invoking their \code{New()}
  //  methods and assigning the result to \doxygen{itk}{SmartPointer}s.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  ReaderType::Pointer reader = ReaderType::New();
  FilterType::Pointer filter = FilterType::New();
  // Software Guide : EndCodeSnippet

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(filter->GetOutput());
  reader->SetFileName(argv[1]);

  //  Software Guide : BeginLatex
  //
  //  The image obtained with the reader is passed as input to the
  //  BinaryThresholdImageFilter.
  //
  //  \index{itk::Binary\-Threshold\-Image\-Filter!SetInput()}
  //  \index{otb::FileImageReader!GetOutput()}
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  filter->SetInput(reader->GetOutput());
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The method \code{SetOutsideValue()} defines the intensity value to be
  //  assigned to those pixels whose intensities are outside the range defined
  //  by the lower and upper thresholds. The method \code{SetInsideValue()}
  //  defines the intensity value to be assigned to pixels with intensities
  //  falling inside the threshold range.
  //
  //  \index{itk::Binary\-Threshold\-Image\-Filter!SetOutsideValue()}
  //  \index{itk::Binary\-Threshold\-Image\-Filter!SetInsideValue()}
  //  \index{SetOutsideValue()!itk::Binary\-Threshold\-Image\-Filter}
  //  \index{SetInsideValue()!itk::Binary\-Threshold\-Image\-Filter}
  //
  //  Software Guide : EndLatex

  const OutputPixelType outsideValue = atoi(argv[5]);
  const OutputPixelType insideValue  = atoi(argv[6]);

  // Software Guide : BeginCodeSnippet
  filter->SetOutsideValue(outsideValue);
  filter->SetInsideValue(insideValue);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The methods \code{SetLowerThreshold()} and \code{SetUpperThreshold()}
  //  define the range of the input image intensities that will be transformed
  //  into the \code{InsideValue}. Note that the lower and upper thresholds are
  //  values of the type of the input image pixels, while the inside and
  //  outside values are of the type of the output image pixels.
  //
  //  Software Guide : EndLatex

  const InputPixelType lowerThreshold = atoi(argv[3]);
  const InputPixelType upperThreshold = atoi(argv[4]);

  // Software Guide : BeginCodeSnippet
  filter->SetLowerThreshold(lowerThreshold);
  filter->SetUpperThreshold(upperThreshold);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The execution of the filter is triggered by invoking the \code{Update()}
  //  method.   If the filter's output has been passed as input to subsequent
  //  filters, the \code{Update()} call on any posterior filters in the
  //  pipeline will indirectly trigger the update of this filter.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  filter->Update();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // \begin{figure}
  // \center
  // \includegraphics[width=0.44\textwidth]{ROISpot5.eps}
  // \includegraphics[width=0.44\textwidth]{BinaryThresholdImageFilterOutput.eps}
  // \itkcaption[BinaryThresholdImageFilter output]{Effect of the
  // BinaryThresholdImageFilter on a ROI of a Spot 5 image.}
  // \label{fig:BinaryThresholdImageFilterInputOutput}
  // \end{figure}
  //
  //  Figure \ref{fig:BinaryThresholdImageFilterInputOutput} illustrates the
  //  effect of this filter on a ROI of a Spot 5 image of an
  //  agricultural area. This
  //  figure shows the limitations of this filter for performing segmentation
  //  by itself. These limitations are particularly noticeable in noisy images
  //  and in images lacking spatial uniformity.
  //
  //  \relatedClasses
  //  \begin{itemize}
  //  \item \doxygen{itk}{ThresholdImageFilter}
  //  \end{itemize}
  //
  //  Software Guide : EndLatex

  writer->SetFileName(argv[2]);
  writer->Update();

  return EXIT_SUCCESS;
}
