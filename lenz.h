#ifndef LENZ_H
#define LENZ_H

#include <opencv2/core/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <iostream>
#include <sstream>


using namespace cv;

namespace cv{using std::vector;}

class lenz
{
private:
    ///settings
    size_t lineEndTypeThresh = 22; //crisp value to classify line; Higher than or equal this value is not narrow.
    size_t cornerIgnoreThresh = 70;   //threshold to ignore corners
           //resize for testing;
    size_t radius = 20;               //radius of ROI
    size_t noise = 100;               //max noise size
    const float resizeAt = 1;

    /// image variables for debugging
    Mat imgOrig;
    Mat imgGray;
    Mat imgBinary;
    Mat imgBinaryRaw;

    Mat imgMorph;
    Mat imgSegment;
    Mat imgSkeleton;
    Mat imgFoundEnds;
    Mat imgClassifiedEnds;


    long endPointsCount;

    std::vector< size_t > classResults {0, 0}; //stores general result of narrow and not narrow counts
    std::vector< Mat > imgEndAreas;
    std::vector< cv::Point > skelPoints;
	std::vector< cv::Point > endPoints;
    std::string fileName;


public:
    ///constructors
    lenz(std::string const &source, std::string const &result);

    ///Main Functions
    void setLineEndTypeThresh(long const &num);

    void setOrig(std::string const &source, std::string const &result);
    Mat getOrig();

    void setGray(Mat const &img);
    Mat getGray();

    void setBinary(Mat const &img);
    Mat getBinary();
    Mat getBinaryRaw();
    Mat getBinaryInv(Mat const &img);

    void setMorph(Mat const &img);
    Mat getMorph();

    void setSegment(Mat const &img);
    Mat getSegment();

    void setSkeleton(Mat const &img);
    Mat getSkeleton();

    void setEndPoints(Mat const &img);
    Mat getEndPointsImage();
    std::vector<cv::Point> getEndPoints();

    void setClassifyLineEnds(std::vector< cv::Point > const &p);
    Mat getClassifiedEnds();


    ///Sub-Functions
    Mat filter(Mat const &img, float kernel_length);
    Rect squareAroundCentroid(cv::Point p, int size, int width, int height) ;
    std::string to_string(long const &n);
    Mat getCircleROI(Mat const &img, cv::Point const &p);

    size_t getNarrowCount();
    size_t getNotNarrowCount();
    long getPointsCount();
    void saveImg(std::string const &path, Mat const &img);
    void setFileName(const std::string& s);
    std::string getFileName();
};



#endif //LENZ
