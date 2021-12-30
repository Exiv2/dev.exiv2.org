// ***************************************************************** -*- C++ -*-
// memIo.cpp, $Rev$
// Sample program to print the Exif metadata of an image

#include <exiv2/exiv2.hpp>
#include <iostream>
#include <cassert>

int main(int argc, char* const argv[])
try {

    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " file\n";
        return 1;
    }

    Exiv2::DataBuf buf = Exiv2::readFile(argv[1]);
    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(buf.pData_, buf.size_);
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
