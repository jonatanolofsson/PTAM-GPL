// Copyright 2010 Isis Innovation Limited
// This video source uses Point Grey's FlyCapture 2 API
// It is based on their example code
// Parameters are hard-coded here - adapt to your specific camera.

#include "VideoSource.h"
#include <gvars3/instances.h>
#include <cvd/utility.h>
#include <flycapture/FlyCapture2.h>

using namespace std;
using namespace CVD;
using namespace GVars3;
using namespace FlyCapture2;


static Camera cam;

static void PrintErrorAndExit( Error error )
{
    error.PrintErrorTrace();
    exit(0);
}

VideoSource::VideoSource()
{
    Error error;
    BusManager busMgr;
    unsigned int numCameras;
    error = busMgr.GetNumOfCameras(&numCameras);
    if (error != PGRERROR_OK) PrintErrorAndExit(error);

    printf( "Number of cameras detected: %u\n", numCameras );
    assert(numCameras > 0);
    PGRGuid guid;
    error = busMgr.GetCameraFromIndex(0, &guid);
    if (error != PGRERROR_OK) PrintErrorAndExit(error);

    error = cam.Connect(&guid);
    if (error != PGRERROR_OK) PrintErrorAndExit(error);

    CameraInfo camInfo;
    error = cam.GetCameraInfo(&camInfo);
    if (error != PGRERROR_OK) PrintErrorAndExit(error);

    printf(
        "\n*** CAMERA INFORMATION ***\n"
        "Serial number - %u\n"
        "Camera model - %s\n"
        "Camera vendor - %s\n"
        "Sensor - %s\n"
        "Resolution - %s\n"
        "Firmware version - %s\n"
        "Firmware build time - %s\n\n",
        camInfo.serialNumber,
        camInfo.modelName,
        camInfo.vendorName,
        camInfo.sensorInfo,
        camInfo.sensorResolution,
        camInfo.firmwareVersion,
        camInfo.firmwareBuildTime );

    // Start capturing images at 640x480xY8
    // You probably need to change this stuff for your camera
    mirSize=ImageRef(640,480);
    error = cam.SetVideoModeAndFrameRate(VIDEOMODE_640x480Y8, FRAMERATE_30);
    if (error != PGRERROR_OK) PrintErrorAndExit(error);

    FlyCapture2::Property p;

    memset (&p, 0, sizeof(p));
    p.type = FlyCapture2::BRIGHTNESS;
    //~ p.absControl = true;
    //~ p.absValue = 1.0f;
    p.onOff = false;
    p.autoManualMode = true;
    error = cam.SetProperty (&p);
    if (error != PGRERROR_OK) PrintErrorAndExit(error);
    //~
    memset (&p, 0, sizeof(p));
    p.type = FlyCapture2::SHUTTER;
    //~ p.absControl = true;
    //~ p.absValue = Shutter;
    p.onOff = true;
    p.autoManualMode = true;
    error = cam.SetProperty (&p);
    if (error != PGRERROR_OK) PrintErrorAndExit(error);
    //~
    memset (&p, 0, sizeof(p));
    p.type = FlyCapture2::GAIN;
    //~ p.absControl = true;
    //~ p.absValue = Gain;
    p.onOff = true;
    p.autoManualMode = true;
    //~ error = cam.SetProperty (&p);
    //~ if (error != PGRERROR_OK) PrintErrorAndExit(error);

    memset (&p, 0, sizeof(p));
    p.type = FlyCapture2::AUTO_EXPOSURE;
    p.onOff = true;
    p.autoManualMode = true;
    error = cam.SetProperty(&p);
    if (error != PGRERROR_OK) PrintErrorAndExit(error);

    error = cam.StartCapture();
    if (error != PGRERROR_OK) PrintErrorAndExit(error);
};

void VideoSource::GetAndFillFrameBWandRGB(CVD::Image<CVD::byte> &imBW, CVD::Image<CVD::Rgb<CVD::byte> > &imRGB)
{
    imRGB.resize(mirSize);
    imBW.resize(mirSize);
    static FlyCapture2::Image imCap;
    cam.RetrieveBuffer(&imCap);
    // Highly dubious double conversion; first PGR->RGB, then RGB->Y
    FlyCapture2::Image tempImage(480,640,0,(unsigned char*) imBW.data(),
        640*480,PIXEL_FORMAT_MONO8,NONE);
    imCap.Convert( PIXEL_FORMAT_MONO8, &tempImage );
    copy(imBW, imRGB);
}

ImageRef VideoSource::Size()
{
    return mirSize;
}
