#include <exiv2/exiv2.hpp>
#include <iostream>
#include <iomanip>
#include <cassert>
#define EXIF_ORIENTATION_TAG	274
#define EXIF_CAMERA_MODEL_TAG	272
#define EXIF_DATETIME_TAG		306
extern "C" // start
{
#include "common_header.h"
#include "lip_image_processing.h"
#include "lip_image_processing_internal.h"
}
void exiv2(const char *jpg_filename, IMAGE *jpg_image)
{
	Exiv2::ExifData::const_iterator exif_data_ptr, end;
	Exiv2::Image::AutoPtr image;
	
    image = Exiv2::ImageFactory::open(jpg_filename);

    if(!image.get()) 
    {
		image_lib_program_error(__FILE__, __LINE__, __FUNC__,
			"can't read <%s>", jpg_filename);
    }
	
    image->readMetadata();
    Exiv2::ExifData &exifData = image->exifData();

    if(exifData.empty()) 
    {
		image_lib_program_warning(__FILE__, __LINE__, __FUNC__,
			"exifData is empty <%s>", jpg_filename);
		return;
    }

    end = exifData.end();

    for(exif_data_ptr = exifData.begin(); exif_data_ptr != end; exif_data_ptr++) 
    {
		switch(exif_data_ptr->tag())
		{
			case EXIF_CAMERA_MODEL_TAG:
			case EXIF_ORIENTATION_TAG:
			case EXIF_DATETIME_TAG:
				std::cout 
				<< std::setw(44) 
				<< std::setfill(' ') 
				<< std::left
				<< exif_data_ptr->key() 
				<< " "
				<< std::dec 
				<< exif_data_ptr->value()
				<< "\n";
			break;
			
			default:
			break;
		}
    }

    for(exif_data_ptr = exifData.begin(); exif_data_ptr != end; exif_data_ptr++) 
    {
		switch(exif_data_ptr->tag())
		{
			case EXIF_CAMERA_MODEL_TAG:
				// !!! the following line is wrong !!!
				//strncpy(jpg_image->CameraModel, (char *)(exif_data_ptr->value()), CAMERA_MODEL_STRING_LENGTH);
			break;
			
			case EXIF_ORIENTATION_TAG:
				// !!! the following line is wrong !!!
				//jpg_image->orientation = (int)(exif_data_ptr->value());
			break;
			
			case EXIF_DATETIME_TAG:
				// !!! the following line is wrong !!!
				//strncpy(jpg_image->DateTime, (char *)(exif_data_ptr->value()), DATETIME_STRING_LENGTH);
			break;
			
			default:
			break;
		}
    }
}

