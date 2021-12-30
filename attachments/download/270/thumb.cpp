#include <exiv2/exiv2.hpp>

#include <iostream>
#include <iomanip>
#include <cassert>

int main(int argc, char* const argv[])
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " file\n";
        return 1;
    }

    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(argv[1]);
    assert(image.get() != 0);
    image->readMetadata();

    Exiv2::ExifData exifData = image->exifData();

    if (exifData.empty()) {
        std::cerr << "exif data is empty\n";
        exifData = Exiv2::ExifData();
    }

    bool isTiff = (image->mimeType() == "image/tiff");

    // ok, let's try to save the thumbnail...
    try {

        Exiv2::ExifThumb eThumb(exifData);

        if (isTiff) {
            eThumb.erase();

            Exiv2::ExifData::const_iterator pos = exifData.findKey(Exiv2::ExifKey("Exif.Image.NewSubfileType"));
            if (pos == exifData.end() || pos->count() != 1 || pos->toLong() != 0) {
                throw Exiv2::Error(1, "Exif.Image.NewSubfileType missing or not set as main image");
             }
             std::string subImage1("SubImage1");
             for (Exiv2::ExifData::iterator md = exifData.begin(); md != exifData.end();)
             {
                 if (md->groupName() == subImage1)
                     md = exifData.erase(md);
                 else
                     ++md;
             }
        }

        Exiv2::DataBuf buf(256);
        memset(buf.pData_, 0x88, buf.size_);

        if (isTiff) {
            Exiv2::ULongValue val;
            val.read("0");
            val.setDataArea(buf.pData_, buf.size_);
            exifData["Exif.SubImage1.JPEGInterchangeFormat"] = val;
            exifData["Exif.SubImage1.JPEGInterchangeFormatLength"] = uint32_t(buf.size_);
            exifData["Exif.SubImage1.Compression"] = uint16_t(6); // JPEG (old-style)
            exifData["Exif.SubImage1.NewSubfileType"] = uint32_t(1); // Thumbnail image
            std::cerr << "As you told me to, I am writing the TIFF thumbs...\n";

        } else {
            eThumb.setJpegThumbnail(buf.pData_, buf.size_);
            std::cerr << "As you told me to, I am writing the JPEG thumbs...\n";
        }

        image->setExifData(exifData);
        image->writeMetadata();
    }
    catch (Exiv2::Error& e) {
        std::cout << "Caught Exiv2 exception '" << e.what() << "'\n";
        return -1;
    }

    return 0;
}
