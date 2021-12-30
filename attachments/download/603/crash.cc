// g++ -W -Wall -g `pkg-config --cflags --libs exiv2` -o crash crash.cc
#include <exiv2/easyaccess.hpp>
#include <exiv2/xmp.hpp>
#include <exiv2/error.hpp>
#include <exiv2/image.hpp>
#include <exiv2/exif.hpp>

#include <stdio.h>

int main()
{
  Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open("crash.xmp");
  if(image.get() == 0) return 0;

  image->readMetadata();

  Exiv2::XmpData                 &xmpData = image->xmpData();
  Exiv2::XmpData::const_iterator  pos     = xmpData.findKey(Exiv2::XmpKey("Xmp.darktable.multi_name"));
  if(pos != xmpData.end())
    printf("found %ld entries, should be 4\nelement 3: `%s'\n", pos->count(), pos->toString(3).c_str());
  else
    printf("not found\n");
  return 1;
}