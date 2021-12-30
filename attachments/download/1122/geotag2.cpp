
#include <exiv2/exiv2.hpp>
#include <iostream>
#include <cassert>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp> 


int main(int argc, char* const argv[])
try {

    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " file\n";
        return 1;
    }
    cv::Mat cvImage = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);
    // Create a new buffer for jpeg encoded image.
    std::vector<uchar> cvbuff;
    cv::imencode(".jpg",cvImage,cvbuff, std::vector<int>());

    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(&cvbuff[0], cvbuff.size());
    image->readMetadata();
    Exiv2::ExifData &exifData = image->exifData();
    exifData["Exif.Image.Model"] = "A long model name to make the Exif metadata grow";
    image->writeMetadata();
    
    // 1. Writing to disk
    // Create a new file for writing.
    FILE * file1 = ::fopen("mm2.jpg", "w");
    ::fclose(file1);
    // Now open using FileIo for writing.
    Exiv2::FileIo file("mm2.jpg");
    file.open();
    file.write(image->io());

    // 2. Writing to disk
    // This works but does not write metatags.
    // FILE * file = ::fopen("mm.jpg", "w"); 
    //::fwrite(&cvbuff[0], image->io().size(), 1, file); 
    //::fclose(file); 

    return 0;
}
catch (Exiv2::Error& e) {
    std::cout << "Caught Exiv2 exception '" << e.what() << "'\n";
    return -1;
}

