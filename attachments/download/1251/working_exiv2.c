extern "C"
{
#include "common_header.h"
#include "lip_image_processing.h"
#include "lip_image_processing_internal.h"
}
void exiv2(const char *jpg_filename, IMAGE *jpg_image)
{
	char data[256];
	Exiv2::ExifData::const_iterator exif_data_ptr, begin, end;
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
			"Exif data is empty in file <%s>", 
			strip_path_from_filename(jpg_filename));
		return;
    }

	begin	= exifData.begin();
    end		= exifData.end();

    for(exif_data_ptr = begin; exif_data_ptr != end; exif_data_ptr++) 
    {
        strncpy(data, exifData[exif_data_ptr->key()].toString().c_str(), 255);

		switch(exif_data_ptr->tag())
		{
			case EXIF_CAMERA_MODEL_TAG:
				strncpy(jpg_image->CameraModel, data, CAMERA_MODEL_STRING_LENGTH);
			break;
			
			case EXIF_ORIENTATION_TAG:
				jpg_image->orientation = atoi(data);
			break;
			
			case EXIF_DATETIME_TAG:
				strncpy(jpg_image->DateTime, data, DATETIME_STRING_LENGTH);
			break;
			
			default:
			break;
		}
    }
	
	printf
	(
		"%s\n\tcamera=\"%s\" datetime=\"%s\" orientation=%d\n", 
		jpg_filename, 
		jpg_image->CameraModel, jpg_image->DateTime, 
		jpg_image->orientation
	);
}

