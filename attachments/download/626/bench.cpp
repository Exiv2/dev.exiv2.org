#include <omp.h>

#include <iostream>
#include <iomanip>
#include <cassert>
#include <vector>

#include "exiv2.hpp"

void check_exif(Exiv2::Exifdatum *datum, Exiv2::ExifKey expected)
{
    if (!datum) {
        std::cout << "Key not found! (" << expected.key() << ")\n";
    } else if (datum->key() != expected.key()) {
        std::cout << "Wrong key, expected " << expected.key() << " (" << expected.tag() << ") found " << datum->key() << " (" << datum->tag() << ")\n";
    }
}

void time_exif(Exiv2::Image::AutoPtr *image) {
    Exiv2::ExifData &exifData = (*image)->exifData();
    if (exifData.empty()) {
        std::cout << "No Exif data\n";
        return;
    }

    std::vector<Exiv2::ExifKey> keys;

    Exiv2::ExifData::const_iterator exif_end = exifData.end();
    for (Exiv2::ExifData::const_iterator i = exifData.begin(); i != exif_end; ++i) {
        std::string key = i->key();
        keys.push_back(Exiv2::ExifKey(key));
    }

    double start_time = omp_get_wtime();

    std::vector<Exiv2::ExifKey>::const_iterator keys_end = keys.end();
    for (std::vector<Exiv2::ExifKey>::const_iterator i = keys.begin(); i != keys_end; ++i) {
        Exiv2::Exifdatum datum = *exifData.findKey(*i);
        check_exif(&datum, *i);
    }

    double end_time = omp_get_wtime();

    // milliseconds
    double elapsed = 1000*(end_time - start_time);

    std::cout << "Exif:\tTags: " << keys.size() << "\tLookup time: " << elapsed << "ms\n";
}

void check_xmp(Exiv2::Xmpdatum *datum, Exiv2::XmpKey expected)
{
    if (!datum) {
        std::cout << "Key not found! (" << expected.key() << ")\n";
    } else if (datum->key() != expected.key()) {
        std::cout << "Wrong key, expected " << expected.key() << " found " << datum->key() << "\n";
    }
}

void time_xmp(Exiv2::Image::AutoPtr *image) {
    Exiv2::XmpData &xmpData = (*image)->xmpData();
    if (xmpData.empty()) {
        std::cout << "No Xmp data\n";
        return;
    }

    std::vector<Exiv2::XmpKey> keys;

    Exiv2::XmpData::const_iterator xmp_end = xmpData.end();
    for (Exiv2::XmpData::const_iterator i = xmpData.begin(); i != xmp_end; ++i) {
        std::string key = i->key();
        keys.push_back(Exiv2::XmpKey(key));
    }

    double start_time = omp_get_wtime();

    std::vector<Exiv2::XmpKey>::const_iterator keys_end = keys.end();
    for (std::vector<Exiv2::XmpKey>::const_iterator i = keys.begin(); i != keys_end; ++i) {
        Exiv2::Xmpdatum datum = *xmpData.findKey(*i);
        check_xmp(&datum, *i);
    }

    double end_time = omp_get_wtime();

    // milliseconds
    double elapsed = 1000*(end_time - start_time);

    std::cout << "Xmp:\tTags: " << keys.size() << "\tLookup time: " << elapsed << "ms\n";
}

void check_iptc(Exiv2::Iptcdatum *datum, Exiv2::IptcKey expected)
{
    if (!datum) {
        std::cout << "Key not found! (" << expected.key() << ")\n";
    } else if (datum->key() != expected.key()) {
        std::cout << "Wrong key, expected " << expected.key() << " (" << expected.tag() << ") found " << datum->key() << " (" << datum->tag() << ")\n";
    }
}

void time_iptc(Exiv2::Image::AutoPtr *image) {
    Exiv2::IptcData &iptcData = (*image)->iptcData();
    if (iptcData.empty()) {
        std::cout << "No Iptc data\n";
        return;
    }

    std::vector<Exiv2::IptcKey> keys;

    Exiv2::IptcData::const_iterator iptc_end = iptcData.end();
    for (Exiv2::IptcData::const_iterator i = iptcData.begin(); i != iptc_end; ++i) {
        std::string key = i->key();
        keys.push_back(Exiv2::IptcKey(key));
    }

    double start_time = omp_get_wtime();

    std::vector<Exiv2::IptcKey>::const_iterator keys_end = keys.end();
    for (std::vector<Exiv2::IptcKey>::const_iterator i = keys.begin(); i != keys_end; ++i) {
        Exiv2::Iptcdatum datum = *iptcData.findKey(*i);
        check_iptc(&datum, *i);
    }

    double end_time = omp_get_wtime();

    // milliseconds
    double elapsed = 1000*(end_time - start_time);

    std::cout << "Iptc:\tTags: " << keys.size() << "\tLookup time: " << elapsed << "ms\n";
}

int main(int argc, char* const argv[]) {
    try {
        if (argc != 2) {
            std::cout << "Usage: " << argv[0] << " file\n";
            return 1;
        }

        Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(argv[1]);
        assert(image.get() != 0);
        image->readMetadata();

        time_exif(&image);
        time_xmp(&image);
        time_iptc(&image);

        return 0;
    }
    catch (Exiv2::Error& e) {
        std::cout << "Caught Exiv2 exception '" << e.what() << "'\n";
        return -1;
    }
}
