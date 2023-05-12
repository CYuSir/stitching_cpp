#include "image_stitching.h"

ImageStitching::ImageStitching() { stitcher = Stitcher::create(mode); }

Mat ImageStitching::stitchImages(const vector<Mat>& images) {
    Mat stitchedImg;
    Stitcher::Status status = stitcher->stitch(images, stitchedImg);
    if (status != Stitcher::OK) {
        cout << "Images could not be stitched!" << endl;
    }
    return stitchedImg;
}

Mat ImageStitching::postProcess(Mat stitchedImg) {
    copyMakeBorder(stitchedImg, stitchedImg, 10, 10, 10, 10, BORDER_CONSTANT, Scalar(0, 0, 0));
    Mat gray, thresh_img;
    cvtColor(stitchedImg, gray, COLOR_BGR2GRAY);
    threshold(gray, thresh_img, 0, 255, THRESH_BINARY);

    vector<vector<Point>> contours;
    findContours(thresh_img.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    int max_idx = -1;
    double max_area = 0.0;
    for (size_t i = 0; i < contours.size(); i++) {
        double area = contourArea(contours[i]);
        if (area > max_area) {
            max_area = area;
            max_idx = i;
        }
    }

    Rect bounding_rect = boundingRect(contours[max_idx]);
    Mat mask = Mat::zeros(thresh_img.size(), CV_8U);
    rectangle(mask, bounding_rect, Scalar(255), FILLED);

    Mat minRectangle = mask.clone();
    Mat sub = mask.clone();
    while (countNonZero(sub) > 0) {
        erode(minRectangle, minRectangle, Mat());
        subtract(minRectangle, thresh_img, sub);
    }

    findContours(minRectangle.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    max_idx = -1;
    max_area = 0.0;
    for (size_t i = 0; i < contours.size(); i++) {
        double area = contourArea(contours[i]);
        if (area > max_area) {
            max_area = area;
            max_idx = i;
        }
    }

    bounding_rect = boundingRect(contours[max_idx]);
    stitchedImg = stitchedImg(bounding_rect);
    return stitchedImg;
}
