#include <exiv2/easyaccess.hpp>
#include <exiv2/image.hpp>
#include <exiv2/exif.hpp>

int main(int argc, char **argv){
	char *filename;
	Exiv2::Image::AutoPtr image;
	Exiv2::ExifData::const_iterator pos;

	if(argc < 2) return 0;
	filename = argv[1];

	image = Exiv2::ImageFactory::open(filename);
	if(image.get() == 0) return 0;
	image->readMetadata();

	Exiv2::ExifData &exifData = image->exifData();

	if ( (pos=Exiv2::focalLength(exifData)) != exifData.end() ){
		float focalLength = pos->toFloat();
	}

	return 1;
}