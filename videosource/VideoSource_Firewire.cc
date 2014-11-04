#include "VideoSource.h"
#include <gvars3/instances.h>
#include <cvd/utility.h>
#include <dc1394/dc1394.h>
#include <cstring>

using namespace std;
using namespace CVD;
using namespace GVars3;


static dc1394_t* context = dc1394_new();
static dc1394error_t err;
static dc1394camera_t* camera;
static dc1394video_frame_t* frame;

VideoSource::VideoSource()
{
    mirSize=ImageRef(640,480);

    dc1394camera_list_t* list;
    err = dc1394_camera_enumerate (context, &list);
    camera = dc1394_camera_new (context, list->ids[0].guid);
    dc1394_camera_free_list (list);
    err = dc1394_video_set_mode(camera, DC1394_VIDEO_MODE_640x480_MONO8);
    err = dc1394_video_set_framerate(camera, DC1394_FRAMERATE_30);

    err = dc1394_feature_set_mode(camera, DC1394_FEATURE_SHUTTER, DC1394_FEATURE_MODE_AUTO);

    err = dc1394_feature_set_mode(camera, DC1394_FEATURE_BRIGHTNESS, DC1394_FEATURE_MODE_AUTO);

    err = dc1394_feature_set_power(camera, DC1394_FEATURE_EXPOSURE, DC1394_ON);
    err = dc1394_feature_set_mode(camera, DC1394_FEATURE_EXPOSURE, DC1394_FEATURE_MODE_AUTO);

    err = dc1394_feature_set_power(camera, DC1394_FEATURE_GAIN, DC1394_ON);
    err = dc1394_feature_set_mode(camera, DC1394_FEATURE_GAIN, DC1394_FEATURE_MODE_AUTO);

    err = dc1394_capture_setup (camera, 10, DC1394_CAPTURE_FLAGS_DEFAULT);
    err = dc1394_video_set_transmission (camera, DC1394_ON);
    if (err != DC1394_SUCCESS) dc1394_capture_stop(camera);
};

void VideoSource::GetAndFillFrameBWandRGB(CVD::Image<CVD::byte> &imBW, CVD::Image<CVD::Rgb<CVD::byte> > &imRGB)
{
    imRGB.resize(mirSize);
    imBW.resize(mirSize);
    dc1394_capture_dequeue(camera, DC1394_CAPTURE_POLICY_WAIT, &frame);
    std::memcpy(imBW.data(), frame->image, frame->image_bytes);
    copy(imBW, imRGB);
    dc1394_capture_enqueue(camera, frame);
}

ImageRef VideoSource::Size()
{
    return mirSize;
}
