#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
using namespace std;

cl::Kernel kernel;
cl::Context context;
cl::Device device;

void find_cl(bool debug) {
    cl::Platform platform;
    std::vector<cl::Platform> platforms;
    vector<cl::Device> devices;
    cl::Platform::get(&platforms);
    if (platforms.empty())
        throw cl::Error(1, "no opencl platform");
    std::cout << "Detected " << platforms.size() << " platforms : " << std::endl;
    for (int i = 0; i < platforms.size(); i++)
        std::cout << "  " << i + 1 << ": " << platforms[i].getInfo<CL_PLATFORM_NAME>() << std::endl;

    bool found = false;
    for (int i = 0; i < platforms.size(); i++) {
        platform = platforms[i];
        try {
            platform.getDevices(CL_DEVICE_TYPE_CPU, &devices);
        }
        catch (cl::Error& e) {
            if (e.err() != CL_DEVICE_NOT_FOUND)
                throw;
        }
        if (!devices.empty()) {
            found = true;
            break;
        }
    }
    if (!found) {
        throw cl::Error(1, "no CPU device available");
    }
    std::cout << "Found CPU platform: " << platform.getInfo<CL_PLATFORM_NAME>() << ", has devices:" << std::endl;
    for (int i = 0; i < devices.size(); i++) {
        std::cout << "  " << i + 1 << ": " << devices[i].getInfo<CL_DEVICE_NAME>() << std::endl;
    }
    device = devices[0];
    std::cout << "Use device: " << device.getInfo<CL_DEVICE_NAME>() << std::endl;
    context = cl::Context(device);




    std::ifstream input("E:/intel_opencl_bug/code.cl");
    std::string str(static_cast<std::stringstream const&>(std::stringstream() << input.rdbuf()).str());
    cl::Program program(context, str);



    try
    {
        if (debug)
            program.build("-g -s E:/intel_opencl_bug/code.cl");
        else
            program.build();
    }
    catch (cl::Error& e)
    {
        if (e.err() == CL_BUILD_PROGRAM_FAILURE)
        {
            // Check the build status
            cl_build_status status = program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(device);
            if (status == CL_BUILD_ERROR) {

                // Get the build log
                std::string name = device.getInfo<CL_DEVICE_NAME>();
                std::string buildlog = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device);
                std::cerr << "Build log for " << name << ":" << std::endl
                    << buildlog << std::endl;
            }
        }
        else
        {
            throw;
        }
        exit(1);
    }

    kernel = cl::Kernel(program, "test");
}

int main() {
    bool debug = true;
    find_cl(debug);
    const int N = 5;
    struct Obj
    {
        cl_int a[N];
    };
    Obj param1{ {1,12,123,1234,12345} };
    Obj param2{ {1,12,123,1234,12345} };
    Obj param3{ {1,12,123,1234,12345} };
    Obj param4{ {1,12,123,1234,12345} };
    Obj param5{ {1,12,123,1234,12345} };

    double output[N * 5];

    cl::Buffer output_b(context, CL_MEM_WRITE_ONLY, N * 5 * sizeof(double));

    kernel.setArg(0, output_b);
    kernel.setArg(1, param1);
    kernel.setArg(2, param2);
    kernel.setArg(3, param3);
    kernel.setArg(4, param4);
    kernel.setArg(5, param5);


    cl::CommandQueue queue(context, device);

    queue.enqueueNDRangeKernel(kernel, cl::NullRange, { size_t(1) }, { size_t(1) });
    queue.enqueueReadBuffer(output_b, false, 0, N * 5 * sizeof(double), output);
    queue.finish();

    for (int i = 0; i < N * 5; i++)
        std::cout << output[i] << std::endl;

    return 0;
}