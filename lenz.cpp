

#include "lenz.h"
#include "zhangsuen.h"


lenz::lenz(std::string const &source, std::string const &result){
    setOrig(source, result);
}

void lenz::setLineEndTypeThresh(long const &num){
    lineEndTypeThresh = num;
}
void lenz::setOrig(std::string const &source, std::string const &result)
{

    setFileName(source);
    std::cout << "Image path is: " << source << std::endl;
    std::cout << "Filename is: " << getFileName() << std::endl;

    imgOrig = imread(source, CV_LOAD_IMAGE_COLOR);



    cv::resize(imgOrig, imgOrig, cv::Size(), resizeAt, resizeAt);

}
Mat lenz::getOrig(){
    return imgOrig;
}

/// grayscale
void lenz::setGray(Mat const &img){

    imgGray = img.clone();

    std::cout << "Converting to grayscale..." << std::endl;
    cvtColor(imgGray, imgGray, COLOR_RGB2GRAY);
    //GaussianBlur( imgGray, imgGray, Size( 3, 3 ), 0, 0 );
    imgGray = filter(imgGray, 6);
}

Mat lenz::getGray(){
    return imgGray;
}

Mat lenz::filter(Mat const &img, float kernel_length){
    Mat dst;
    dst.create(imgGray.size(), CV_8U);
    bilateralFilter(imgGray, dst, kernel_length, kernel_length*2, kernel_length*2);
    return dst.clone();
}

///Binary
void lenz::setBinary(Mat const &img){
    //imgBinaryRaw = img.clone();
    imgBinary = img.clone();;
    std::cout << "Generating local adaptive threshold..." << std::endl;
    adaptiveThreshold(imgBinary, imgBinary, 255, ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, 11, 7);

}
Mat lenz::getBinary(){
    return imgBinary;
}
Mat lenz::getBinaryRaw(){
    return imgBinaryRaw;
}
Mat lenz::getBinaryInv(Mat const &img){
    Mat res = img.clone();
    threshold(res, res, 0, 255, CV_THRESH_BINARY_INV);
    return res;
}



/// Morph
void lenz::setMorph(Mat const &img){
    int morph_size = 2;

    imgMorph = img.clone();



    cv::Mat structure_elem = cv::getStructuringElement(
                 2, cv::Size(morph_size, morph_size));

    //erode(imgMorph, imgMorph, structure_elem);
    dilate( imgMorph, imgMorph, structure_elem);
    cv::morphologyEx(imgMorph, imgMorph, cv::MORPH_CLOSE, structure_elem);
    cv::morphologyEx(imgMorph, imgMorph, cv::MORPH_OPEN, structure_elem);
}
Mat lenz::getMorph(){
    return imgMorph;
}

/// Segmentation (Cleaning)
void lenz::setSegment(Mat const &img){
  //  imgSegment = img.clone();
    Mat drawing;
    vector< vector <cv::Point> > contours;
    vector<Vec4i> heirachy;
    size_t area = 0;

    cv::findContours(img, contours, heirachy, RETR_CCOMP, CV_CHAIN_APPROX_TC89_KCOS, cv::Point(0, 0));
    drawing = Mat::zeros(img.size(), CV_8UC3);
    for (size_t idx = 0; idx < contours.size(); idx++) {
        area = contourArea(contours[idx]);
        if (area > noise) {
			if(heirachy[idx][3] == -1)
                cv::drawContours(drawing, contours, idx, cv::Scalar(255, 255, 255), CV_FILLED, 8, heirachy);
                std::cout << "Found line: " << area << std::endl;
        }else{
            //if(area!=0) std::cout << "Size of removed noise: " << area << std::endl;
        }
    }

    imgSegment = drawing.clone();
}
Mat lenz::getSegment(){
    return imgSegment;
}

/// Skeletonization
void lenz::setSkeleton(Mat const &img){
    imgSkeleton = img.clone();
    cvtColor(imgSkeleton, imgSkeleton, COLOR_RGB2GRAY);
    imgSkeleton = getBinaryInv(imgSkeleton);
    //thinning(imgSkeleton);
    thin(imgSkeleton, true, true, true);



    imgSkeleton = getBinaryInv(imgSkeleton);
    cv::findNonZero(imgSkeleton, skelPoints);



}
Mat lenz::getSkeleton(){
    return imgSkeleton;
}

/// Set endpoint
void lenz::setEndPoints(Mat const &img){
    Mat res = img.clone();
    //threshold(res, res, 0, 255, CV_THRESH_BINARY_INV);
    std::vector<cv::Point> shuuten;
    std::vector<cv::Point> n = { cv::Point(-1,1), cv::Point(0,1), cv::Point(1,1),
                            cv::Point(-1,0), cv::Point(0, 0), cv::Point(1,0),
                            cv::Point(-1,-1), cv::Point(0,-1), cv::Point(1,-1) };


    //walk through each skeleton point to test number of neighbors. A point with only 1 neighbor is considered an endpoint
    //possible bottle neck, convert ko to sa pointers.
    for (size_t i = 0; i < skelPoints.size(); i++) {
        int nCount = 0;
        for (size_t j = 0; j < n.size(); j++) {
            if (res.at<uchar>(skelPoints[i].y + n[j].y,
                skelPoints[i].x + n[j].x) == 255) {
                nCount++;
            }
        }

        if (nCount <= 2) {
            shuuten.push_back(cv::Point(skelPoints[i].x, skelPoints[i].y));
        }

        //res.at<uchar>(skelPoints[i].y, skelPoints[i].x); //tangnang to, dapat pala y,x sa halip x,y
    }

    /*shuuten o tamesu*/
    //draw the end points to original image
    imgFoundEnds = imgOrig.clone();
    cvtColor(res, res, COLOR_GRAY2BGR);
    for (size_t i = 0; i < shuuten.size(); i++) {
        circle(imgFoundEnds, cv::Point(shuuten[i].x, shuuten[i].y),	10.0, Scalar(255, 0, 0),1,8);
    }

    endPoints = shuuten;
}
Mat lenz::getEndPointsImage(){
    return imgFoundEnds;
}
std::vector<cv::Point> lenz::getEndPoints(){
    return endPoints;
}


void putString(Mat &img, std::string const &s, cv::Point const &p){
    String text = s;
    int fontFace = FONT_HERSHEY_SCRIPT_SIMPLEX;
    double fontScale = 0.5;
    int thickness = 1;
    cv::Point textOrg = p;
    //Mat img(600, 800, CV_8UC3, Scalar::all(0));

    int baseline=0;

    baseline += thickness;


    putText(img, text, textOrg, fontFace, fontScale,
            Scalar::all(0), thickness, 8);
}

Rect lenz::squareAroundCentroid(cv::Point p, int size, int width, int height) {
    cv::Point a, b;
    Rect res;
    int rad = size / 2;
    int x1 = p.x - rad-1,
        y1 = p.y + rad-1,
        x2 = p.x + rad,
        y2 = p.y - rad-1;

    x1 = (x1 < 0) ? 0 : x1;
    y1 = (y1 < 0) ? 0 : y1;
    x2 = (x2 < 0) ? 0 : x2;
    y2 = (y2 < 0) ? 0 : y2;


    // Check if rectangles points exceeds the image boundaries
    a = cv::Point(	(x1 > width) ? width : x1,
                    (y1 > height) ? height : y1);


    b = cv::Point((x2 > width) ? width : x2,
                    (y2 > height) ? height : y2 );

    return Rect(a, b);
}



Mat lenz::getCircleROI(Mat const &img, cv::Point const &p){

    //get the Rect containing the circle:

    //Rect r(cen.x-radius, cen.y-radius, radius*2,radius*2);
    Rect r;
    r = squareAroundCentroid(p, radius, img.cols-1, img.rows-1);

    // obtain the image ROI:
    Mat roi(img, r);

    threshold(roi, roi, 200, 255, CV_THRESH_BINARY_INV);

    // make a black mask, same size:
    Mat mask(roi.size(), roi.type(), Scalar::all(0));

    // with a white, filled circle in it:
    circle(mask, cv::Point(radius/2,radius/2), radius/2, Scalar::all(255), -1);

    // combine roi & mask:
    Mat circle_roi = roi & mask;

    return circle_roi;
}

void lenz::setClassifyLineEnds(std::vector< cv::Point > const &p){
    Mat res = filter(imgGray,11);

    //blur( imgGray, res, Size( 3, 3 ) ); //filter(imgGray,11);

    Mat croppedImage;
    Rect ROI;
    cv::Point center;
    size_t ROIarea;
    imgClassifiedEnds = imgOrig.clone();

    //threshold(res, res, 0, 255, CV_THRESH_BINARY_INV);
    for (size_t i = 0; i < p.size(); i++) {
        center = cv::Point(p[i].x, p[i].y);

        //ROI = squareAroundCentroid(center, 20, res.cols-1, res.rows-1);
        //croppedImage = res(ROI);

        croppedImage = getCircleROI(res, center);


        //imwrite("Results\\"   + to_string(i) + "_circroi_" + fileName, croppedImage);
        imgEndAreas.push_back(croppedImage);
        //if(cv::countNonZero(croppedImage) > lineEndTypeThresh) {
        ROIarea = cv::countNonZero(croppedImage);

        std::cout << "Area of index " << to_string(i) <<" at point (" << p[i].x << ", " << p[i].y << ") is : " << ROIarea << std::endl;
        std::cout << "Crisp Threshold is " << lineEndTypeThresh <<std::endl;
        circle(
               imgClassifiedEnds,
               center,
               10.0,
               (ROIarea >= lineEndTypeThresh) ? Scalar(0, 0, 255) :  Scalar(0, 255, 0),
               2,
               8);
        //}
        //endAreas.push_back (cv::countNonZero(croppedImage));
        putString(imgClassifiedEnds, to_string(i) + "_" + to_string(ROIarea), cv::Point(p[i].x, p[i].y));

        //std::cout << path ;


    }
}
Mat lenz::getClassifiedEnds(){
    return imgClassifiedEnds;
}
size_t lenz::getNarrowCount(){
    return classResults[0];
}
size_t lenz::getNotNarrowCount(){
    return classResults[1];
}

void lenz::saveImg(std::string const &path, Mat const &img){

    imwrite(path, img);
}

std::string lenz::to_string(long const &n){
    std::ostringstream ss;
    ss << n;
    return ss.str();
}


void lenz::setFileName(const std::string& s) {

   char sep = '/';
#ifdef _WIN32
   sep = '\\';
#endif
    std::cout << "Passed path:" << s << std::endl;
    size_t i = s.rfind(sep, s.length());
    if (i != std::string::npos) {
        fileName = (s.substr(i+1, s.length() - i));
   }else{
    fileName = s;
    }
}
std::string lenz::getFileName(){return fileName;}
