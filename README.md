# otbclimpi branch
## Description
This is a fork of the OTB used to experiment the parallel approach.
Implementation of parallel pipelines is done thanks to the otb::wrapper::MPIApplication class.
This class is similar to the original otb::Application class.
The new application wrapper implements an MPI based layer to manage essential MPI world parameters (rank, size, ...) to allow the development of parallel pipelines.

## Code example
The **otb-mpitoy** module is a very simple example of an MPI-ized OTB pipeline.
A pipeline composed by a source (image file reader), a filter (resample image filter), and a parallel mapper (parallel geotiff file writer), is implemented.
The example shows explicitely the accessors of the parallel file writer for MPI rank and world size.

Once built, the application can be runned on multiple processing nodes with mpirun.
The following command will deploy the pipeline on two MPI processes :
`mpirun -np 2 otbMPIToy inputImage.tif resampledImage.tif`


## Parallel pipelines
A few pipelines are implemented in their parallel variant. They can be found in Modules/Remote/otb-mpi_applications

