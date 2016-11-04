
#include "lenz.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

//using namespace cv;
//std::string defaultPath = "C:\\Users\\jherenz\\OneDrive\\Thesis\\Project\\lei\\bin\\Debug\\Results\\";
std::string defaultPath = "Results\\";
std::string filename = "";



void process(std::string const &origPath){

    //std::cout << origPath;

    lenz ras(origPath, defaultPath);
    filename = ras.getFileName();

    std::cout << defaultPath + "orig_" + filename << "\n";


/// Grayscale
    ras.setGray(ras.getOrig());
    ras.saveImg(defaultPath + "gray_" + filename, ras.getGray() );

/// Binary
    ras.setBinary(ras.getGray());
    ras.saveImg(defaultPath + "bin_" + filename, ras.getBinary() );


/// Morph
    ras.setMorph(ras.getBinary());
    ras.saveImg(defaultPath + "morph_" + filename, ras.getMorph() );


/// Segmentation
    ras.setSegment(ras.getMorph());
    ras.saveImg(defaultPath + "seg_" + filename, ras.getSegment() );

/// Skeletonization
    ras.setSkeleton(ras.getSegment());
    ras.saveImg(defaultPath + "thin_" + filename, ras.getSkeleton() );

/// Identify endpoints
    ras.setEndPoints(ras.getSkeleton());
    ras.saveImg(defaultPath + "end_" + filename, ras.getEndPointsImage() );


/// Classify line end
    ras.setClassifyLineEnds(ras.getEndPoints() );
    //ras.saveImg(defaultPath + "binraw_" + filename, ras.getBinaryRaw() );
    //ras.saveImg(defaultPath + "bin_" + filename, ras.getBinary() );

    ras.saveImg(defaultPath + "result_" + filename, ras.getClassifiedEnds() );

    //std::cout << std::endl << "Done. Press any key to continue...";
    //getchar();

}

int main( int argc, char** argv )
{
    //heck passed args.

    if( argc < 2 ){
        std::cout << std::endl << "File Empty";
        getchar();
        return -1;
    }

    process(argv[1]);
    /*
    std::string a = "C:\\Users\\jherenz\\OneDrive\\Thesis\\Project\\lei\\bin\\Debug\\aa.jpeg";
    process(a);
    */
  waitKey(0);
  return 0;

}

