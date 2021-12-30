// ***************************************************************** -*- C++ -*-
// xmpsample.cpp, $Rev: 2286 $
// Sample/test for high level XMP classes. See also addmoddel.cpp

#include <exiv2/exiv2.hpp>

#include <string>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <cmath>

bool isEqual(float a, float b)
{
    double d = std::fabs(a - b);
    return d < 0.00001;
}

int main(int argc, char* const argv[])
try 
{
  if (argc != 2) 
  {
    std::cout << "Usage: " << argv[0] << " file\n";
    return 1;
  }

  std::string testFile = argv[1];

  //Open File and write some XMP
  Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(testFile);
  assert(image.get() != 0);

  Exiv2::XmpData xmpDataW;
  Exiv2::XmpProperties::registerNs("R&S/", "RS");
  xmpDataW["Xmp.RS.SerialNumber"] = "12345678";
  xmpDataW["Xmp.RS.SoftwareVersion"] = "9.9.9.9";
  
  image->setXmpData(xmpDataW);
  image->writeMetadata();
  image.release();

  Exiv2::Image::AutoPtr imageOpenSameFileAgain = Exiv2::ImageFactory::open(testFile);
  assert(imageOpenSameFileAgain.get() != 0);
  imageOpenSameFileAgain->readMetadata();

  Exiv2::XmpData &xmpDataR = imageOpenSameFileAgain->xmpData();
  if (xmpDataR.empty()) 
  {
    std::string error(argv[1]);
    error += ": No XMP data found in the file";
    throw Exiv2::Error(1, error);
  }
  else
  {
    Exiv2::XmpData::const_iterator pos = xmpDataR.findKey(Exiv2::XmpKey("Xmp.RS.SoftwareVersion"));
    if (pos == xmpDataR.end()) 
    {
      std::string error(argv[1]);
      error += ": SoftwareVersion not found in XMP data of the file";
      throw Exiv2::Error(1, error);
    }
    
    Exiv2::Value::AutoPtr v = pos->getValue();
    assert(v->ok());
    std::string sXMPValue = v->toString();
    std::cout << sXMPValue << "\n";
  }

  return 0;
}
catch (Exiv2::AnyError& e) 
{
    std::cout << "Caught Exiv2 exception '" << e << "'\n";
    return -1;
}
