#include <dirent.h>

#include <filesystem>
#include <vector>

#include "image_stitching.h"

using namespace std;

int main(int argc, char **argv) {
    if (argc != 2) {
        cout << "Usage: ./ImageStitching <image_dir>" << endl;
        return 1;
    }

    string image_directory = argv[1];
    vector<string> image_files;

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(image_directory.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            string file_name = ent->d_name;
            if (file_name.size() > 4 && file_name.substr(file_name.size() - 4) == ".jpg") {
                string file_path = image_directory + "/" + file_name;
                image_files.push_back(file_path);
            }
        }
        closedir(dir);
    }

    vector<Mat> images;
    for (const auto &image_file : image_files) {
        Mat img = imread(image_file);
        if (!img.empty()) images.push_back(img);
    }

    ImageStitching stitcher;
    Mat stitchedImg = stitcher.stitchImages(images);
    Mat result = stitcher.postProcess(stitchedImg);

    imwrite("result.png", result);

    return 0;
}
