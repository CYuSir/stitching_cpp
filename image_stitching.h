#ifndef IMAGE_STITCHING_H
#define IMAGE_STITCHING_H

#include <opencv2/opencv.hpp>
#include <opencv2/stitching.hpp>

using namespace std;
using namespace cv;

class ImageStitching {
public:
    ImageStitching();
    Mat stitchImages(const vector<Mat>& images);
    Mat postProcess(Mat stitchedImg);

private:
    Ptr<Stitcher> stitcher;
    Stitcher::Mode mode = Stitcher::PANORAMA;
};

#endif // IMAGE_STITCHING_H
