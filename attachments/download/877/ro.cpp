// Demonstrates a problem with exiv2 0.25 that allows read-only files to be
// overwritten

#include <iostream>
#include "exiv2/image.hpp"
#include "exiv2/exif.hpp"

int main(void)
{
	Exiv2::Image::AutoPtr Image;
	Image = Exiv2::ImageFactory::open("ro.jpg");
	try {
		Image->writeMetadata();
	} catch (Exiv2::Error e) {
		std::cout << e << "\n";
		return 1;
	}
	return 0;
}
