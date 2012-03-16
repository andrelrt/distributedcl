/* ============================================================

Copyright (c) 2009 Advanced Micro Devices, Inc.  All rights reserved.
 
Redistribution and use of this material is permitted under the following 
conditions:
 
Redistributions must retain the above copyright notice and all terms of this 
license.
 
In no event shall anyone redistributing or accessing or using this material 
commence or participate in any arbitration or legal action relating to this 
material against Advanced Micro Devices, Inc. or any copyright holders or 
contributors. The foregoing shall survive any expiration or termination of 
this license or any agreement or access or use related to this material. 

ANY BREACH OF ANY TERM OF THIS LICENSE SHALL RESULT IN THE IMMEDIATE REVOCATION 
OF ALL RIGHTS TO REDISTRIBUTE, ACCESS OR USE THIS MATERIAL.

THIS MATERIAL IS PROVIDED BY ADVANCED MICRO DEVICES, INC. AND ANY COPYRIGHT 
HOLDERS AND CONTRIBUTORS "AS IS" IN ITS CURRENT CONDITION AND WITHOUT ANY 
REPRESENTATIONS, GUARANTEE, OR WARRANTY OF ANY KIND OR IN ANY WAY RELATED TO 
SUPPORT, INDEMNITY, ERROR FREE OR UNINTERRUPTED OPERA TION, OR THAT IT IS FREE 
FROM DEFECTS OR VIRUSES.  ALL OBLIGATIONS ARE HEREBY DISCLAIMED - WHETHER 
EXPRESS, IMPLIED, OR STATUTORY - INCLUDING, BUT NOT LIMITED TO, ANY IMPLIED 
WARRANTIES OF TITLE, MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, 
ACCURACY, COMPLETENESS, OPERABILITY, QUALITY OF SERVICE, OR NON-INFRINGEMENT. 
IN NO EVENT SHALL ADVANCED MICRO DEVICES, INC. OR ANY COPYRIGHT HOLDERS OR 
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, PUNITIVE,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, REVENUE, DATA, OR PROFITS; OR 
BUSINESS INTERRUPTION) HOWEVER CAUSED OR BASED ON ANY THEORY OF LIABILITY 
ARISING IN ANY WAY RELATED TO THIS MATERIAL, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE. THE ENTIRE AND AGGREGATE LIABILITY OF ADVANCED MICRO DEVICES, 
INC. AND ANY COPYRIGHT HOLDERS AND CONTRIBUTORS SHALL NOT EXCEED TEN DOLLARS 
(US $10.00). ANYONE REDISTRIBUTING OR ACCESSING OR USING THIS MATERIAL ACCEPTS 
THIS ALLOCATION OF RISK AND AGREES TO RELEASE ADVANCED MICRO DEVICES, INC. AND 
ANY COPYRIGHT HOLDERS AND CONTRIBUTORS FROM ANY AND ALL LIABILITIES, 
OBLIGATIONS, CLAIMS, OR DEMANDS IN EXCESS OF TEN DOLLARS (US $10.00). THE 
FOREGOING ARE ESSENTIAL TERMS OF THIS LICENSE AND, IF ANY OF THESE TERMS ARE 
CONSTRUED AS UNENFORCEABLE, FAIL IN ESSENTIAL PURPOSE, OR BECOME VOID OR 
DETRIMENTAL TO ADVANCED MICRO DEVICES, INC. OR ANY COPYRIGHT HOLDERS OR 
CONTRIBUTORS FOR ANY REASON, THEN ALL RIGHTS TO REDISTRIBUTE, ACCESS OR USE 
THIS MATERIAL SHALL TERMINATE IMMEDIATELY. MOREOVER, THE FOREGOING SHALL 
SURVIVE ANY EXPIRATION OR TERMINATION OF THIS LICENSE OR ANY AGREEMENT OR 
ACCESS OR USE RELATED TO THIS MATERIAL.

NOTICE IS HEREBY PROVIDED, AND BY REDISTRIBUTING OR ACCESSING OR USING THIS 
MATERIAL SUCH NOTICE IS ACKNOWLEDGED, THAT THIS MATERIAL MAY BE SUBJECT TO 
RESTRICTIONS UNDER THE LAWS AND REGULATIONS OF THE UNITED STATES OR OTHER 
COUNTRIES, WHICH INCLUDE BUT ARE NOT LIMITED TO, U.S. EXPORT CONTROL LAWS SUCH 
AS THE EXPORT ADMINISTRATION REGULATIONS AND NATIONAL SECURITY CONTROLS AS 
DEFINED THEREUNDER, AS WELL AS STATE DEPARTMENT CONTROLS UNDER THE U.S. 
MUNITIONS LIST. THIS MATERIAL MAY NOT BE USED, RELEASED, TRANSFERRED, IMPORTED,
EXPORTED AND/OR RE-EXPORTED IN ANY MANNER PROHIBITED UNDER ANY APPLICABLE LAWS, 
INCLUDING U.S. EXPORT CONTROL LAWS REGARDING SPECIFICALLY DESIGNATED PERSONS, 
COUNTRIES AND NATIONALS OF COUNTRIES SUBJECT TO NATIONAL SECURITY CONTROLS. 
MOREOVER, THE FOREGOING SHALL SURVIVE ANY EXPIRATION OR TERMINATION OF ANY 
LICENSE OR AGREEMENT OR ACCESS OR USE RELATED TO THIS MATERIAL.

NOTICE REGARDING THE U.S. GOVERNMENT AND DOD AGENCIES: This material is 
provided with "RESTRICTED RIGHTS" and/or "LIMITED RIGHTS" as applicable to 
computer software and technical data, respectively. Use, duplication, 
distribution or disclosure by the U.S. Government and/or DOD agencies is 
subject to the full extent of restrictions in all applicable regulations, 
including those found at FAR52.227 and DFARS252.227 et seq. and any successor 
regulations thereof. Use of this material by the U.S. Government and/or DOD 
agencies is acknowledgment of the proprietary rights of any copyright holders 
and contributors, including those of Advanced Micro Devices, Inc., as well as 
the provisions of FAR52.227-14 through 23 regarding privately developed and/or 
commercial computer software.

This license forms the entire agreement regarding the subject matter hereof and 
supersedes all proposals and prior discussions and writings between the parties 
with respect thereto. This license does not affect any ownership, rights, title,
or interest in, or relating to, this material. No terms of this license can be 
modified or waived, and no breach of this license can be excused, unless done 
so in a writing signed by all affected parties. Each term of this license is 
separately enforceable. If any term of this license is determined to be or 
becomes unenforceable or illegal, such term shall be reformed to the minimum 
extent necessary in order for this license to remain in effect in accordance 
with its terms as modified by such reformation. This license shall be governed 
by and construed in accordance with the laws of the State of Texas without 
regard to rules on conflicts of law of any state or jurisdiction or the United 
Nations Convention on the International Sale of Goods. All disputes arising out 
of this license shall be subject to the jurisdiction of the federal and state 
courts in Austin, Texas, and all defenses are hereby waived concerning personal 
jurisdiction and venue of these courts.

============================================================ */


#include "MatrixMulDouble.hpp"

int 
MatrixMulDouble::setupMatrixMulDouble()
{
    /* allocate and init memory used by host  inputA[widthA][heightA]*/
    cl_uint inputSizeBytes0 = widthA * heightA * sizeof(cl_double);

    inputA = (cl_double*) malloc(inputSizeBytes0);
    if(inputA == NULL)    
    { 
        sampleCommon->error("Failed to allocate host memory. (inputA)");
        return SDK_FAILURE;
    }

    /* allocate and init memory used by host inputB[widthB][heightB]*/
    cl_uint inputSizeBytes1 = widthB * heightB * sizeof(cl_double);

    inputB = (cl_double*) malloc(inputSizeBytes1);
    if(inputB == NULL)    
    { 
        sampleCommon->error("Failed to allocate host memory. (inputB)");
        return SDK_FAILURE;
    }
    
    /* random initialisation of input */
    sampleCommon->fillRandom<cl_double>(inputA, widthA, heightA, 0, 10);
    sampleCommon->fillRandom<cl_double>(inputB, widthB, heightB, 0, 10);

    /* allocate memory for output[widthB][heightA] */
    cl_uint outputSizeBytes = heightA * widthB * sizeof(cl_double);

    output = (cl_double*) malloc(outputSizeBytes);
    if(output == NULL)    
    { 
        sampleCommon->error("Failed to allocate host memory. (output)");
        return SDK_FAILURE;
    }

    /* allocate memory for output[widthB][heightA] of reference implemenation*/
    if(verify)
    {
        refOutput = (cl_double*) malloc(outputSizeBytes);
        if(refOutput == NULL)
        { 
            sampleCommon->error("Failed to allocate host memory. (verificationOutput)");
            return SDK_FAILURE;
        }
        memset(refOutput, 0, outputSizeBytes);
    }
    /* 
     * Unless quiet mode has been enabled, print the INPUT arrays
     */
    if(!quiet) 
    {
        sampleCommon->printArray<cl_double>(
            "inputA", 
            inputA, 
            widthA, 
            1);
        sampleCommon->printArray<cl_double>(
            "inputB", 
            inputB, 
            widthB, 
            1);

    }

    return SDK_SUCCESS;
}

int 
MatrixMulDouble::genBinaryImage()
{
    cl_int err = CL_SUCCESS;

    /*
     * Have a look at the available platforms and pick either
     * the AMD one if available or a reasonable default.
     */
    err = cl::Platform::get(&platforms);
    if(!sampleCommon->checkVal(err, 
        CL_SUCCESS,
        "Platform::get() failed."))
    {
        return SDK_FAILURE;
    }

    std::vector<cl::Platform>::iterator i = platforms.begin();

    cl_context_properties cps[5] = 
    { 
        CL_CONTEXT_PLATFORM, 
        (cl_context_properties)(*i)(), 
        CL_CONTEXT_OFFLINE_DEVICES_AMD,
        (cl_context_properties)1,
        0
    };

    context = cl::Context(CL_DEVICE_TYPE_ALL, cps, NULL, NULL, &err);
    if(!sampleCommon->checkVal(err, 
        CL_SUCCESS,
        "Context::Context() failed."))
    {
        return SDK_FAILURE;
    }


    devices = context.getInfo<CL_CONTEXT_DEVICES>();
    if(!sampleCommon->checkVal(err, 
        CL_SUCCESS,
        "Context::getInfo() failed."))
    {
        return SDK_FAILURE;
    }

    std::cout << "Platform :" << (*i).getInfo<CL_PLATFORM_VENDOR>().c_str() << "\n";
    size_t numDevices = devices.size();

    if (numDevices == 0) 
    {
        std::cerr << "No device available\n";
        return SDK_FAILURE;
    }

    /* create a CL program using the kernel source */
    streamsdk::SDKFile kernelFile;
    std::string kernelPath = sampleCommon->getPath();
    kernelPath.append("MatrixMulDouble_Kernels.cl");
    if(!kernelFile.open(kernelPath.c_str()))
    {
        std::cout << "Failed to load kernel file : " << kernelPath << std::endl;
        return SDK_FAILURE;
    }

    cl::Program::Sources programSource(1, 
        std::make_pair(kernelFile.source().data(), 
        kernelFile.source().size()));
    
    program = cl::Program(context, programSource, &err);
    if(!sampleCommon->checkVal(err, 
        CL_SUCCESS,
        "Program::program() failed."))
    {
        return SDK_FAILURE;
    }

    program.build(devices);

    std::cout << "Number of devices found : " << numDevices << "\n\n";

    std::vector<size_t> binarySizes = program.getInfo<CL_PROGRAM_BINARY_SIZES>(&err);
    if(!sampleCommon->checkVal(err, 
        CL_SUCCESS,
        "Program::getInfo(CL_PROGRAM_BINARY_SIZES) failed."))
    {
        return SDK_FAILURE;
    }

    std::vector<char*> binaries = program.getInfo<CL_PROGRAM_BINARIES>(&err);
    if(!sampleCommon->checkVal(err, 
        CL_SUCCESS,
        "Program::getInfo(CL_PROGRAM_BINARIES) failed."))
    {
        return SDK_FAILURE;
    }   

    /* dump out each binary into its own separate file. */
    std::vector<cl::Device>::iterator deviceIndx = devices.begin();
    for(size_t i = 0; i < numDevices; i++)
    {
        char fileName[100];
        sprintf(fileName, "%s.%d", dumpBinary.c_str(), (int)i);
        if(binarySizes[i] != 0)
        {
            std::string deviceName = (*deviceIndx).getInfo<CL_DEVICE_NAME>(&err);
            if(!sampleCommon->checkVal(err,
                                       CL_SUCCESS,
                                       "Device::getInfo(CL_DEVICE_NAME) failed."))
            {
                return SDK_FAILURE;
            }

            printf( "%s binary kernel: %s\n", deviceName.c_str(), fileName);
            streamsdk::SDKFile BinaryFile;
            if(!BinaryFile.writeBinaryToFile(fileName, 
                                             binaries[i], 
                                             binarySizes[i]))
            {
                std::cout << "Failed to load kernel file : " << fileName << std::endl;
                return SDK_FAILURE;
            }
        }
        else
        {
            printf("Skipping %s since there is no binary data to write!\n",
                    fileName);
        }
    }

    return SDK_SUCCESS;
}



int
MatrixMulDouble::setupCL(void)
{
    cl_int status = 0;
    cl_device_type dType;
    
    if(deviceType.compare("cpu") == 0)
        dType = CL_DEVICE_TYPE_CPU;
    else //deviceType = "gpu" 
        dType = CL_DEVICE_TYPE_GPU;

    /*
     * Have a look at the available platforms and pick either
     * the AMD one if available or a reasonable default.
     */
    status = cl::Platform::get(&platforms);
    if(!sampleCommon->checkVal(status, 
        CL_SUCCESS,
        "Platform::get() failed."))
        return SDK_FAILURE;

    std::vector<cl::Platform>::iterator i = platforms.begin();

    cl_context_properties cps[3] = 
    { 
        CL_CONTEXT_PLATFORM, 
        (cl_context_properties)(*i)(),
        0 
    };


    if(NULL == (*i)())
    {
        sampleCommon->error("NULL platform found so Exiting Application.");
        return SDK_FAILURE;
    }

    context = cl::Context(dType, cps, NULL, NULL, &status);
    if(!sampleCommon->checkVal(status, 
        CL_SUCCESS,
        "Context::Context() failed."))
        return SDK_FAILURE;

    devices = context.getInfo<CL_CONTEXT_DEVICES>();
    if(!sampleCommon->checkVal(status, CL_SUCCESS, "Context::getInfo() failed."))
        return SDK_FAILURE;

    std::cout << "Platform :" << (*i).getInfo<CL_PLATFORM_VENDOR>().c_str() << "\n";
    int deviceCount = (int)devices.size();
    int j = 0;
    for (std::vector<cl::Device>::iterator i = devices.begin(); i != devices.end(); ++i, ++j)
    {
        std::cout << "Device " << j << " : ";
        std::string deviceName = (*i).getInfo<CL_DEVICE_NAME>();
        std::cout << deviceName.c_str() << "\n";
    }
    std::cout << "\n";

    if (deviceCount == 0) 
    {
        std::cerr << "No device available\n";
        return SDK_FAILURE;
    }

    if(!sampleCommon->validateDeviceId(deviceId, deviceCount))
    {
        sampleCommon->error("sampleCommon::validateDeviceId() failed");
        return SDK_FAILURE;
    }

    std::string extensions = devices[deviceId].getInfo<CL_DEVICE_EXTENSIONS>();
    if(!strstr(extensions.c_str(), "cl_amd_fp64") || 
       !strstr(extensions.c_str(), "cl_khr_fp64") )
    {
        sampleCommon->error("Device does not support double precision extension!");
        exit(SDK_SUCCESS);
    }



    cl_uint localMemType;
    /* Get device specific information */
    status = devices[deviceId].getInfo<cl_uint>(
             CL_DEVICE_LOCAL_MEM_TYPE,
            &localMemType);
    if(!sampleCommon->checkVal(
            status,
            CL_SUCCESS, 
            "Device::getInfo("
            "CL_DEVICE_LOCAL_MEM_TYPE) failed."))
        return SDK_FAILURE;

    /* If scratchpad is available then update the flag */
    if(localMemType == CL_LOCAL)
        lds = true;

    /* Get Device specific Information */
    status = devices[deviceId].getInfo<size_t>(
              CL_DEVICE_MAX_WORK_GROUP_SIZE, 
              &maxWorkGroupSize);

    if(!sampleCommon->checkVal(
            status,
            CL_SUCCESS, 
            "Device::getInfo(CL_DEVICE_MAX_WORK_GROUP_SIZE) failed."))
        return SDK_FAILURE;
    
    status = devices[deviceId].getInfo<cl_uint>(
             CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,
             &maxDimensions);

    if(!sampleCommon->checkVal(
                status,
                CL_SUCCESS, 
                "Device::getInfo(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS) failed."))
        return SDK_FAILURE;


    maxWorkItemSizes = (size_t*)malloc(maxDimensions * sizeof(size_t));
    
    std::vector<size_t> workItems = devices[deviceId].getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>();

    for(cl_uint i = 0; i < maxDimensions; ++i)
        maxWorkItemSizes[i] = workItems[i];

    status = devices[deviceId].getInfo<cl_ulong>(
             CL_DEVICE_LOCAL_MEM_SIZE,
             &totalLocalMemory);

    if(!sampleCommon->checkVal(
                status,
                CL_SUCCESS, 
                "Device::getInfo(CL_DEVICE_LOCAL_MEM_SIZES) failed."))
        return SDK_FAILURE;


    commandQueue = cl::CommandQueue(context, devices[deviceId], 0, &status);
    if(!sampleCommon->checkVal(
        status, 
        CL_SUCCESS,
        "CommandQueue::CommandQueue() failed."))
        return SDK_FAILURE;

    /* Create buffer for matrix A */
    inputBufA = cl::Buffer(
                    context, 
                    CL_MEM_READ_ONLY, 
                    sizeof(cl_double) * widthA * heightA, 
                    NULL, 
                    &status);

    if(!sampleCommon->checkVal(
            status,
            CL_SUCCESS,
            "cl::Buffer failed. (inputBufA)"))
        return SDK_FAILURE;

    /* Write data to buffer */
    status = commandQueue.enqueueWriteBuffer(
             inputBufA, 
             CL_TRUE,
             0,
             sizeof(cl_double) * widthA * heightA,
             inputA,
             NULL,
             NULL);

    if(!sampleCommon->checkVal(
            status,
            CL_SUCCESS,
            "CommandQueue::enqueueWriteBuffer failed. (inputBufA)"))
        return SDK_FAILURE;

    /* Create buffer for matrix B */
    inputBufB = cl::Buffer(
                    context,
                    CL_MEM_READ_ONLY,
                    sizeof(cl_double) * widthB * heightB,
                    NULL,
                    &status);

    if(!sampleCommon->checkVal(
            status,
            CL_SUCCESS,
            "cl::Buffer failed. (inputBufB)"))
        return SDK_FAILURE;

    /* Write data to buffer */
    status = commandQueue.enqueueWriteBuffer(
             inputBufB, 
             CL_TRUE,
             0,
             sizeof(cl_double) * widthB * heightB,
             inputB,
             NULL,
             NULL);

    if(!sampleCommon->checkVal(
            status,
            CL_SUCCESS,
            "CommandQueue::enqueueWriteBuffer failed. (inputBufB)"))
        return SDK_FAILURE;


    outputBuf = cl::Buffer(
                    context,
                    CL_MEM_WRITE_ONLY,
                    sizeof(cl_double) * heightA * widthB,
                    NULL,
                    &status);
    if(!sampleCommon->checkVal(
            status,
            CL_SUCCESS,
            "cl::Buffer failed. (outputBuf)"))
        return SDK_FAILURE;


    device.push_back(devices[deviceId]);

    /* create a CL program using the kernel source */
    streamsdk::SDKFile kernelFile;
    std::string kernelPath = sampleCommon->getPath();

    if(isLoadBinaryEnabled())
    {
        kernelPath.append(loadBinary.c_str());
        if(!kernelFile.readBinaryFromFile(kernelPath.c_str()))
        {
            std::cout << "Failed to load kernel file : " << kernelPath << std::endl;
            return SDK_FAILURE;
        }
        cl::Program::Binaries programBinary(1,std::make_pair(
                                              (const void*)kernelFile.source().data(), 
                                              kernelFile.source().size()));
        
        program = cl::Program(context, device, programBinary, NULL, &status);
        if(!sampleCommon->checkVal(
            status,
            CL_SUCCESS,
            "Program::Program(Binary) failed."))
            return SDK_FAILURE;

    }
    else
    {
        kernelPath.append("MatrixMulDouble_Kernels.cl");
        if(!kernelFile.open(kernelPath.c_str()))
        {
            std::cout << "Failed to load kernel file : " << kernelPath << std::endl;
            return SDK_FAILURE;
        }
        cl::Program::Sources programSource(
                                1,
                                std::make_pair(kernelFile.source().data(), 
                                kernelFile.source().size()));
        
        program = cl::Program(context, programSource, &status);

        if(!sampleCommon->checkVal(
            status,
            CL_SUCCESS,
            "Program::Program(Source) failed."))
            return SDK_FAILURE;
    }

    status = program.build(device);

    if(status != CL_SUCCESS)
    {
        if(status == CL_BUILD_PROGRAM_FAILURE)
        {
            std::string str = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[deviceId]);

            std::cout << " \n\t\t\tBUILD LOG\n";
            std::cout << " ************************************************\n";
            std::cout << str << std::endl;
            std::cout << " ************************************************\n";
        }
    }

    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "Program::build() failed."))
        return SDK_FAILURE;
    
    /* Create kernel */
    /* If local memory is present then use the specific kernel */
    if(lds)
        kernel = cl::Kernel(program, "mmmKernel_local", &status);
    else
        kernel = cl::Kernel(program, "mmmKernel", &status);

    if(!sampleCommon->checkVal(
            status,
            CL_SUCCESS,
            "cl::Kernel failed."))
        return SDK_FAILURE;

     return SDK_SUCCESS;
}


int 
MatrixMulDouble::runCLKernels(void)
{
    cl_int   status;
    cl::Event events[2];

    /* 
     * Kernel runs over complete output matrix with blocks of blockSize x blockSize 
     * running concurrently
     */
    size_t globalThreads[2]= {widthB / 4, heightA / 4};
    size_t localThreads[2] = {blockSize, blockSize};

    status =  kernel.getWorkGroupInfo<cl_ulong>(
                devices[deviceId],
                CL_KERNEL_LOCAL_MEM_SIZE,
                &usedLocalMemory);

    if(!sampleCommon->checkVal(
                status,
                CL_SUCCESS,
                "Kernel::getWorkGroupInfo(CL_KERNEL_LOCAL_MEM_SIZE) failed"
                ".(usedLocalMemory)"))
        return SDK_FAILURE;

    availableLocalMemory = totalLocalMemory - usedLocalMemory;
    neededLocalMemory    = 2 * blockSize * blockSize * sizeof(cl_double); 
    if(neededLocalMemory > availableLocalMemory)
    {
        std::cout << "Unsupported: Insufficient local memory on device." << std::endl;
        return SDK_SUCCESS;
    }

    /* Check group size against group size returned by kernel */
    kernelWorkGroupSize = kernel.getWorkGroupInfo<CL_KERNEL_WORK_GROUP_SIZE>(devices[deviceId], &status);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "Kernel::getWorkGroupInfo()  failed."))
        return SDK_FAILURE;

    if((cl_uint)(localThreads[0] * localThreads[1]) > kernelWorkGroupSize)
    {
       if(kernelWorkGroupSize >= 64)
        {
            blockSize = 8; 
            localThreads[0] = blockSize;
            localThreads[1] = blockSize;
        }
        else if(kernelWorkGroupSize >= 32)
        {
            blockSize = 4; 
            localThreads[0] = blockSize;
            localThreads[1] = blockSize;
        }
        else
        {
            std::cout << "Out of Resources!" << std::endl;
            std::cout << "Group Size specified : " << localThreads[0] * localThreads[1] << std::endl;
            std::cout << "Max Group Size supported on the kernel : " 
                      << kernelWorkGroupSize<<std::endl;
            return SDK_FAILURE;
        }
    }

    if(localThreads[0] > maxWorkItemSizes[0] ||
       localThreads[1] > maxWorkItemSizes[1] ||
       localThreads[0]*localThreads[1] > maxWorkGroupSize)
    {
        std::cout << "Unsupported: Device does not support requested number of work items." << std::endl;
        return SDK_FAILURE;
    }

    /*** Set appropriate arguments to the kernel ***/
    
    /* output array as the 1st argument : stores product of inputA and inputB */
    status = kernel.setArg(0, inputBufA);
    if(!sampleCommon->checkVal(
            status,
            CL_SUCCESS,
            "cl::setArg failed. (inputBufA)"))
        return SDK_FAILURE;

    status = kernel.setArg(1, inputBufB);
    if(!sampleCommon->checkVal(
            status,
            CL_SUCCESS,
            "cl::setArg failed. (inputBufB)"))
        return SDK_FAILURE;

    status = kernel.setArg(2, outputBuf);
    if(!sampleCommon->checkVal(
            status,
            CL_SUCCESS,
            "cl::setArg failed. (outputBuf)"))
        return SDK_FAILURE;


    /* widthA of the inputA matrix as 4th argument - widthA */
    status = kernel.setArg(3, widthA);
    if(!sampleCommon->checkVal(
            status,
            CL_SUCCESS,
            "cl::setArg failed. (widthA)"))
        return SDK_FAILURE;

    /* Set local memory argument if Scratchpad is available */
    if(lds)
    {
        status = kernel.setArg(
                    4, 
                    (blockSize * 4) * (blockSize * 4) * sizeof(cl_double),
                    NULL);
        if(!sampleCommon->checkVal(
                status,
                CL_SUCCESS,
                "cl::setArg failed. (local memory)"))
            return SDK_FAILURE;
    }
    else
    {
        status = kernel.setArg(4, sizeof(cl_int), &widthB);

        if(!sampleCommon->checkVal(
                status,
                CL_SUCCESS,
                "cl::setArg failed. (widthB)"))
            return SDK_FAILURE;
    }
    /* 
    * Enqueue a kernel run call.
    */
    /* Each thread calculates 2 gaussian numbers */
    cl::NDRange gThreads(globalThreads[0], globalThreads[1]);
    cl::NDRange lThreads(localThreads[0], localThreads[1]);

    status = commandQueue.enqueueNDRangeKernel(kernel,
                                               cl::NullRange,
                                               gThreads,
                                               lThreads,
                                               0, 
                                               &events[0]);

    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS, 
        "CommandQueue::enqueueNDRangeKernel() failed."))
        return SDK_FAILURE;

    status = events[0].wait();
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "Event::wait() failed. (event[0])"))
        return SDK_FAILURE;

    /* Enqueue readBuffer*/
    status = commandQueue.enqueueReadBuffer(
                outputBuf,
                CL_FALSE,
                0,
                heightA * widthB * sizeof(cl_double),
                output,
                NULL,
                &events[1]);
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "CommandQueue::enqueueReadBuffer failed. (outputBuf)"))
        return SDK_FAILURE;


    /* Wait for the read buffer to finish execution */
    status = events[1].wait();
    if(!sampleCommon->checkVal(
        status,
        CL_SUCCESS,
        "Event::wait() failed. (event[1])"))
        return SDK_FAILURE;

    return SDK_SUCCESS;
}

/*
 * This is a naive O(N^3) CPU implementatio of matrix multiplication
 */
void 
MatrixMulDouble::MatrixMulDoubleCPUReference(
    cl_double * output,
    cl_double * inputA,
    cl_double * inputB,
    const cl_uint y,
    const cl_uint x,
    const cl_uint z)
{
    for(cl_uint i=0; i < y; i++)
    {
        for(cl_uint j=0; j < z; j++)
        {
            for(cl_uint k=0; k < x; k++)
            {
                output[i*z + j] += (inputA[i*x + k]*inputB[k*z + j]);
            }
        }
    }
}

int 
MatrixMulDouble::initialize()
{
    /* Call base class Initialize to get default configuration */
    if(!this->SDKSample::initialize())
        return SDK_FAILURE;

    /* add an option for getting blockSize from commandline */
    streamsdk::Option* xParam = new streamsdk::Option;
    if(!xParam)
    {
        sampleCommon->error("Memory Allocation error.\n");
        return SDK_FAILURE;
    }

    xParam->_sVersion = "x";
    xParam->_lVersion = "heightA";
    xParam->_description = "height of matrix A";
    xParam->_type     = streamsdk::CA_ARG_INT;
    xParam->_value    = &n;

    sampleArgs->AddOption(xParam);
    delete xParam;

    streamsdk::Option* yParam = new streamsdk::Option;
    if(!yParam)
    {
        sampleCommon->error("Memory Allocation error.\n");
        return SDK_FAILURE;
    }

    yParam->_sVersion = "y";
    yParam->_lVersion = "widthA";
    yParam->_description = "width of matrix A and Height of matrix B";
    yParam->_type     = streamsdk::CA_ARG_INT;
    yParam->_value    = &m;

    sampleArgs->AddOption(yParam);
    delete yParam;

    streamsdk::Option* zParam = new streamsdk::Option;
    if(!zParam)
    {
        sampleCommon->error("Memory Allocation error.\n");
        return SDK_FAILURE;
    }

    zParam->_sVersion = "z";
    zParam->_lVersion = "widthB";
    zParam->_description = "width of matrix B";
    zParam->_type     = streamsdk::CA_ARG_INT;
    zParam->_value    = &k;

    sampleArgs->AddOption(zParam);
    delete zParam;

    streamsdk::Option* num_iterations = new streamsdk::Option;
    if(!num_iterations)
    {
        sampleCommon->error("Memory allocation error.\n");
        return SDK_FAILURE;
    }

    num_iterations->_sVersion = "i";
    num_iterations->_lVersion = "iterations";
    num_iterations->_description = "Number of iterations for kernel execution";
    num_iterations->_type = streamsdk::CA_ARG_INT;
    num_iterations->_value = &iterations;

    sampleArgs->AddOption(num_iterations);
    delete num_iterations;

    return SDK_SUCCESS;
}

int 
MatrixMulDouble::setup()
{  
    /* Make sure the dimensions are multiples of blockSize */
    const int vectorSize = 4;
    if(n % (blockSize * vectorSize) != 0)
    {
        n = (n / (blockSize * vectorSize) + 1) * (blockSize * vectorSize);
    }

    if(m % blockSize != 0)
    {
        m = (m / blockSize + 1) * blockSize;
    }

    if(k % (blockSize * vectorSize) != 0)
    {
       k = (k / (blockSize * vectorSize) + 1) * (blockSize * vectorSize);
    }

    widthA  = m;
    heightA = n;
    
    widthB  = k;
    heightB = m;

    if(setupMatrixMulDouble() != SDK_SUCCESS)
        return SDK_FAILURE;

    int timer = sampleCommon->createTimer();
    sampleCommon->resetTimer(timer);
    sampleCommon->startTimer(timer);

    if(setupCL()!= SDK_SUCCESS)
        return SDK_FAILURE;

    sampleCommon->stopTimer(timer);
    setupTime = (cl_double)sampleCommon->readTimer(timer);

    return SDK_SUCCESS;
}


int 
MatrixMulDouble::run()
{
    int timer = sampleCommon->createTimer();
    sampleCommon->resetTimer(timer);
    sampleCommon->startTimer(timer);   

    std::cout << "Executing kernel for " << iterations 
              << " iterations" << std::endl;
    std::cout << "-------------------------------------------" << std::endl;

    for(int i = 0; i < iterations; i++)
    {
        /* Arguments are set and execution call is enqueued on command buffer */
        int kernelRun = runCLKernels();
        if(kernelRun != SDK_SUCCESS)
            return kernelRun;
    }

    sampleCommon->stopTimer(timer);
    kernelTime = (double)(sampleCommon->readTimer(timer)) / iterations;

    if(!quiet)
        sampleCommon->printArray<cl_double>("Output", output, widthB, 1);

    return SDK_SUCCESS;
}

int 
MatrixMulDouble::verifyResults()
{
    if(verify)
    {
        /* reference implementation */
        MatrixMulDoubleCPUReference(refOutput, inputA, inputB, heightA, widthA,  widthB);

        /* compare the results and see if they match */
        if(sampleCommon->compare(output, refOutput, heightA * widthB))
        {
            std::cout<<"Passed!\n";
            return SDK_SUCCESS;
        }
        else
        {
            std::cout<<"Failed\n";
            return SDK_FAILURE;
        }
    }

	return SDK_SUCCESS;
}

void 
MatrixMulDouble::printStats()
{
    std::string strArray[4] = {"MatrixA", "MatrixB", "Time(sec)", "KernelTime(sec)"};
    std::string stats[4];

    totalTime = setupTime + kernelTime;

    stats[0]  = sampleCommon->toString(heightA, std::dec)
                +"x"+sampleCommon->toString(widthA, std::dec);
    stats[1]  = sampleCommon->toString(heightB, std::dec)
                +"x"+sampleCommon->toString(widthB, std::dec);
    stats[2]  = sampleCommon->toString(totalTime, std::dec);
	stats[3]  = sampleCommon->toString(kernelTime, std::dec);
    
    this->SDKSample::printStats(strArray, stats, 4);
}

int 
MatrixMulDouble::cleanup()
{
    /* release program resources (input memory etc.) */
    if(inputA) 
        free(inputA);

    if(inputB) 
        free(inputB);

    if(output)
        free(output);

    if(refOutput) 
        free(refOutput);

    if(maxWorkItemSizes)
        free(maxWorkItemSizes);

    return SDK_SUCCESS;
}

int 
main(int argc, char * argv[])
{
    MatrixMulDouble clMatrixMulDouble("OpenCL Matrix Multiplication for Double");

    clMatrixMulDouble.initialize();

    if(!clMatrixMulDouble.parseCommandLine(argc, argv))
        return SDK_FAILURE;
    
    if(clMatrixMulDouble.isDumpBinaryEnabled())
    {
        return clMatrixMulDouble.genBinaryImage();
    }
    else
    {
        if(clMatrixMulDouble.setup() == SDK_FAILURE)
            return SDK_FAILURE;
        
        if(clMatrixMulDouble.run() == SDK_FAILURE)
            return SDK_FAILURE;

        if(clMatrixMulDouble.verifyResults() == SDK_FAILURE)
            return SDK_FAILURE;

        if(clMatrixMulDouble.cleanup() == SDK_FAILURE)
            return SDK_FAILURE;

        clMatrixMulDouble.printStats();
    }

    return SDK_SUCCESS;
}
