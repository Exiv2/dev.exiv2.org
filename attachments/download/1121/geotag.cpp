
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
    //cv::cvtColor(cvImage, cvImage, CV_BGR2RGB);

    //Exiv2::DataBuf buf = Exiv2::readFile(argv[1]);
    //Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(buf.pData_, buf.size_);
    Exiv2::byte * buf = reinterpret_cast<Exiv2::byte *> (cvImage.ptr());
    //std::cout << static_cast<void *> (cvImage.ptr()) << std::endl;
    //std::cout << static_cast<void *> (buf) << std::endl;

    //Exiv2::MemIo memIo(buf, 3921722);
    //std::cout << "Image Type is:" << Exiv2::ImageFactory::getType(buf, 3921722) << std::endl;
    //std::cout << "Image Type is:" << Exiv2::ImageFactory::getType(argv[1]) << std::endl;


    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(buf, 3921722);
    image->readMetadata();
    Exiv2::ExifData &exifData = image->exifData();
    exifData["Exif.Image.Model"] = "A long model name to make the Exif metadata grow";
    image->writeMetadata();
    Exiv2::FileIo file(argv[1]);
    file.open();
    file.write(image->io());

    return 0;
}
catch (Exiv2::Error& e) {
    std::cout << "Caught Exiv2 exception '" << e.what() << "'\n";
    return -1;
}
