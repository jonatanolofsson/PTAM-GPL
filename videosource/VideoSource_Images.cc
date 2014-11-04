#include "VideoSource.h"
//~ #include <gvars3/instances.h>
#include <cvd/utility.h>
#include <cvd/image_io.h>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;
using namespace CVD;
//~ using namespace GVars3;

VideoSource::VideoSource()
{
    mirSize=ImageRef(640,480);
};

void VideoSource::GetAndFillFrameBWandRGB(CVD::Image<CVD::byte> &imBW, CVD::Image<CVD::Rgb<CVD::byte> > &imRGB)
{
    static int counter = 0;
    imRGB.resize(mirSize);
    imBW.resize(mirSize);
    stringstream isource; isource << dir << setfill('0') << setw(3) << counter++ << ".bmp";
    //~ std::string source = isource.str();
    imBW = img_load(isource.str());
    copy(imBW, imRGB);
}

ImageRef VideoSource::Size()
{
    return mirSize;
}
