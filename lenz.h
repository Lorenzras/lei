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
    long lineEndTypeThresh = 29; //crisp value to classify line;

    Mat imgOrig;
    Mat imgGray;
    Mat imgBinary;
    Mat imgBinaryRaw;

    Mat imgMorph;
    Mat imgSegment;
    Mat imgSkeleton;
    Mat imgFoundEnds;
    Mat imgClassifiedEnds;

    std::vector< Mat > imgEndAreas;
    std::vector< cv::Point > skelPoints;
	std::vector< cv::Point > endPoints;


    //std::string defaultPath = "C:\\Users\\jherenz\\OneDrive\\Thesis\\Project\\lei\\bin\\Debug\\Results\\";
    //std::string filename = "a.jpeg";

public:
    ///constructors
    lenz(std::string const &source, std::string const &result);

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

    void setClassifyLineEnds(Mat const &img, std::vector< cv::Point > const &p);
    Mat getClassifiedEnds();

    Rect squareAroundCentroid(cv::Point p, int size, int width, int height) ;
    std::string to_string(long const &n);
    Mat getCircleROI(Mat const &img, cv::Point const &p);

    void saveImg(std::string const &path, Mat const &img);
};



#endif //LENZ
