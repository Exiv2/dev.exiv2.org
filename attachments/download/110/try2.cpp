#include "image.hpp"
#include "exif.hpp"
#include <iostream>
#include <cassert>
#include <string>
#include "futils.hpp"
#include <iconv.h>

std::string printUcs2(const std::string& value);

int main(int argc, char* const argv[])
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " file\n";
        return 1;
    }

    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(argv[1]);
    assert(image.get() != 0);
    image->readMetadata();

    Exiv2::ExifData &exifData = image->exifData();

    Exiv2::ExifData::const_iterator md = exifData.findKey(
        Exiv2::ExifKey("Exif.Photo.UserComment"));
    if (md == exifData.end()) {
        std::cout << "No Exif comment found in the image.\n";
    }
    else {
        std::string val = md->toString();
        std::string::size_type start = val.find(' ');
        if (start != std::string::npos) {
            val = val.substr(start + 1);
        }
        std::cout << "md->toString()\n"
                  << "--------------\n"
                  << "size = " << val.size() << "\n"
                  << printUcs2(val) << "\n\n";

        val = md->print(&exifData);
        std::cout << "md->print(&exifData)\n"
                  << "--------------------\n"
                  << "size = " << val.size() << "\n"
                  << printUcs2(val) << "\n\n";

        const Exiv2::CommentValue& cv = dynamic_cast<const Exiv2::CommentValue&>(md->value());
        val = cv.comment();
        std::cout << "Exiv2::CommentValue::comment()\n"
                  << "------------------------------\n"
                  << "size = " << val.size() << "\n"
                  << printUcs2(val) << "\n\n";
    }
    return 0;
}

std::string printUcs2(const std::string& value)
{
    bool go = true;
    iconv_t cd = (iconv_t)(-1);
    cd = iconv_open("UTF-8", "UCS-2BE");
    if (cd == (iconv_t)(-1)) {
        std::cerr << "iconv_open: " << Exiv2::strError() << "\n";
        go = false;
    }
    std::string ret;
    if (go) {
        Exiv2::DataBuf ob(value.size() * 2);
        char* outptr = reinterpret_cast<char*>(ob.pData_);
        const char* outbuf = outptr;
        size_t outbytesleft = ob.size_;
        EXV_ICONV_CONST char* inbuf = const_cast<EXV_ICONV_CONST char*>(value.data());
        size_t inbytesleft = value.size();

        std::cerr << std::dec << "inbytesleft = " << inbytesleft << ", "
                  << "outbytesleft = " << outbytesleft << "\n";

        size_t rc = iconv(cd,
                          &inbuf,
                          &inbytesleft,
                          &outptr,
                          &outbytesleft);
        if (rc == size_t(-1)) {
            std::cerr << "iconv: "
                      << Exiv2::strError()
                      << " inbytesleft = " << inbytesleft << "\n";
            go = false;
        }
        if (go) {
            if (outptr > outbuf && *(outptr-1) == '\0') outptr--;
            ret = std::string(outbuf, outptr-outbuf);
        }
    }
    if (cd != (iconv_t)(-1)) {
        iconv_close(cd);
    }
    return ret;
} // printUcs2
