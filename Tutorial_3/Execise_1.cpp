#include	<iostream>
#include	<opencv2/core/core.hpp>
#include	<opencv2/highgui/highgui.hpp>
#include	"Supp.h"

using namespace std;
using namespace cv;

// Piecewise Linear Transform function
double piecewise_function(double intensity, double r1, double s1, double r2, double s2) {
    if (0 <= intensity && intensity <= r1) {
        return (s1 / r1) * intensity;
    }
    else if (r1 < intensity && intensity <= r2) {
        return ((s2 - s1) / (r2 - r1)) * (intensity - r1) + s1;
    }
    else {
        return ((255 - s2) / (255 - r2)) * (intensity - r2) + s2;
    }
}

// Perform piecewise linear transform (intensity transform)
void piecewise_linear_transform(Mat src, double r1, double s1, double r2, double s2, Mat &dst) {
	// Copy src image to dst image
    dst = src.clone();

	// Change intensity value of each pixel on dst image
    for (int y = 0; y < src.rows; y++) {
        for (int x = 0; x < src.cols; x++) {
            int intensity = src.at<uchar>(y, x);
            dst.at<uchar>(y, x) = (int)piecewise_function(intensity, r1, s1, r2, s2);
        }
    }
}

Mat drawHistogram(Mat imgs, Mat mask, Scalar color) {
	// Below is the step to generate a histogram (frequency count) of the input image intensity within the mask
	// adapted from the above site.

	/// Establish the number of bins to count frequencies of intensities of 0 to 255
	int histSize = 256;

	/// Set the ranges for counting, i.e. intensity outside of this range (0-255) is not counted
	float range[] = { 0, 256 };
	const float* histRange = { range };

	bool uniform = true; bool accumulate = false;
	Mat		histogram;

	// Compute the histograms: its input parameters are
	/////////// imgs: the input image or images but we are using only 1 channel image here.
	// 2nd input: number of images in imgs
	// 3rd input: list of channels in each image for the computation, the first channel is 0
	////////// 4th input: optional mask; if it is empty, no effect; else collect info from area under mask
	////////// 5th: output matrix of histogram
	// 6th: Histogram dimensionality, 1D histogram, 2D histogram, ...
	// 7th: Array of histogram sizes in each dimension
	// 8th: Array of the dims arrays of the histogram bin boundaries in each dimension
	// 9th: Flag indicating whether the histogram is uniform or not, i.e. uniform have one
	//      range in each dimension. Else it has multiple ranges with no count on out of range intensity.
	// 10th: Accumulation flag. If it is set, the histogram is not cleared in the beginning 

	// Consult OpenCV docs for comprehensive usage
	calcHist(&imgs, 1, 0, mask, histogram, 1, &histSize, &histRange, uniform, accumulate);
	/*	cout << "Histogram values:";
		for (int i = 0; i < 256; i++) { // This part is important for you to access the histogram value at each bin
			if (i % 16 == 0) cout << "\n";
			cout << histogram.at<float>(i) << ' ';
		}
		cout << "\n\n";*/

		// Draw the histograms in an image of size 400X512
	int		hist_w = 512; int hist_h = 400;
	int		bin_w = cvRound((double)hist_w / histSize);

	// create image to display histogram later
	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

	/// Normalize the height to the range [ 0, histImage.rows ]
	normalize(histogram, histogram, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	/// Draw histogram, each point is specified by its column (X) and then row (Y) number.
	for (int i = 1; i < histSize; i++) // draw line from one point(x,y) to the next
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(histogram.at<float>(i - 1))),
			Point(bin_w * (i), hist_h - cvRound(histogram.at<float>(i))),
			color, 2, 8, 0);
	return histImage;
}


int main() {
	Mat		image, dst;

	image = imread("Inputs/Images/Yeast.png", IMREAD_GRAYSCALE);

	// Piecewise point
	int r1 = 140, s1 = 0; // first point
	int r2 = 165, s2 = 255; // second point

	// Perform piecewise linear transform
    piecewise_linear_transform(image, r1, s1, r2, s2, dst);

	// Draw Histogram
	Mat histImage_original = drawHistogram(image, Mat(), Scalar(250, 250, 250));
	Mat histImage_dst = drawHistogram(dst, Mat(), Scalar(250, 250, 250));

	imshow("original.jpg", image);
	imshow("dst.jpg", dst);
	imshow("original_hist.jpg", histImage_original);
    imshow("dst_hist.jpg", histImage_dst);
	
	waitKey(0);

	destroyAllWindows();
	return 0;

}