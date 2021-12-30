//
// XMP encoding test program
// -------------------------
// Cf. http://dev.exiv2.org/boards/3/topics/1622
//
// Produce a XMP sidecar file with all metadata from an image.
//

#include <stdio.h>

#include <exiv2/exif.hpp>
#include <exiv2/image.hpp>
#include <exiv2/convert.hpp>

using namespace Exiv2;

// Read XMP packet from image file
// -------------------------------

bool readpacket(std::string& packet, const char* iname)
{
    // Open image file
    Image::AutoPtr image = ImageFactory::open(iname);
    if( ! image->good() ) {
        fprintf(stderr, "Invalid image %s\n", iname);
        return false;
    }

    // Load metadata sets
    image->readMetadata();

    // Re-encode Exif metadata into XMP schema
    copyExifToXmp(image->exifData(), image->xmpData());

    // Serialize XMP dataset into a string
    int state = XmpParser::encode(packet, image->xmpData());
    if( ! state ) {
        fprintf(stderr, "XMP serialized Ok\n");
        return true;
    }
    else {
        fprintf(stderr, "XMP serialize error %d\n", state);
        return false;
    }
}

// Save XMP packet to sidecar file
// -------------------------------
// (Just to keep a visible trace of the encoded packet)

bool savepacket(const std::string& packet, const char* iname)
{
    // Sidecar filename
    std::string sname(iname);
    sname += ".xmp";

    FILE* file = fopen(sname.c_str(), "w");
    if( ! file ) {
        perror(sname.c_str());
        return false;
    }
    else {
        fprintf(file, "%s\n", packet.c_str());
        fclose(file);

        fprintf(stderr, "XMP saved Ok\n");
        return true;
    }
}

// Test program
// ------------

int main(int argc, char* argv[])
{
    // One arg !
    if( argc < 2 ) {
        fprintf(stderr, "Usage: %s <image-filename>\n", argv[0]);
        return 1;
    }

    // Process
    std::string packet;
    if( readpacket(packet, argv[1]) ) {
        savepacket(packet, argv[1]);
        return 0;
    }
    else return 1;
}

