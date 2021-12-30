// ***************************************************************** -*- C++ -*-
// *****************************************************************************

// included header files
#include "exif.hpp"
#include <iostream>
#include <iomanip>

using namespace Exiv2;

// *****************************************************************************
// Main
int main(int argc, char* const argv[])
try {
    if( argc != 2 ){
        std::cerr << "Usage: test file.jpg" << std::endl;
        return 1;
    }

    Exiv2::ExifData exifData;
    exifData.read(argv[1]);

    ExifKey exifKey("Exif.Image.ImageDescription");

    Value::AutoPtr value = Value::create(asciiString);
    value->read("I don't really know");

    ExifData::iterator iter = exifData.findKey(exifKey);
    if (iter != exifData.end()) {
        iter->setValue(value.get());
    }
    else {
        exifData.add(exifKey, value.get());
    }

    exifData.write(argv[1]);

    return 0;
}
catch (Exiv2::Error& e) {
    std::cout << "Caught Exiv2 exception '" << e << "'\n";
    return -1;
}
