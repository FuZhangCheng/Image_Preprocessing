#include	<iostream>
#include	<opencv2/core/core.hpp>
#include	<opencv2/highgui/highgui.hpp>
#include    <cstdlib>
#include	"Supp.h"

using namespace std;
using namespace cv;

int main() {
    // Providing a seed value
    srand((unsigned)time(NULL));

    Mat i(25, 25, CV_8UC3);

    int const	noOfImagePerCol = 16, noOfImagePerRow = 16;
    Mat			largeWin, win[noOfImagePerRow * noOfImagePerCol], legend[noOfImagePerRow * noOfImagePerCol];

    createWindowPartition(i, largeWin, win, legend, noOfImagePerCol, noOfImagePerRow, 0, 0);

    for (int n = 0; n < 256; n++) {
        // Create an RNG object
        RNG rng(rand() % 100000);

        // Generate a random color
        Scalar color(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));

        // Create a random color image
        Mat img(25, 25, CV_8UC3, color);

        img.copyTo(win[n]);
    }

    imshow("Random color image", largeWin);

    waitKey(0);
    destroyAllWindows();
    return 0;

}
