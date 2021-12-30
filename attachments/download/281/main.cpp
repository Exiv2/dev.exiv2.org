/*
 * main.cpp
 *
 *  Created on: Sep 15, 2011
 *      Author: sasha
 */

#include "image.hpp"
#include "exif.hpp"
#include <math.h>
#include <stdio.h>
#include <iostream>


int main(int argc, char **argv)
{
		std::string filename;
	if (argc == 1)
	{
		std::cout << "usage: input filename\n";
		return -1;
	}
	else
	{
		filename = argv[1];
	}
	std::cout << "start program\n",fflush(0);

	Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filename);
	Exiv2::ExifData exifData;
		double latitude = 50.236;
		double longitude = -43.236;
			char scratchBufLatitude[100];
			long int degLatitude, minLatitude, secLatitude;
			char scratchBufLongitude[100];
			long int degLongitude, minLongitude, secLongitude;

				if ( exifData.findKey(Exiv2::ExifKey("Exif.GPSInfo.GPSVersionID")) == exifData.end())
				{
					Exiv2::Value::AutoPtr value = Exiv2::Value::create(Exiv2::unsignedByte);
					value->read("2 0 0 0");
					exifData.add(Exiv2::ExifKey("Exif.GPSInfo.GPSVersionID"), value.get());
					exifData["Exif.GPSInfo.GPSMapDatum"] = "WGS-84";
				}

					exifData["Exif.GPSInfo.GPSLatitudeRef"] = (latitude < 0 ) ? "S" : "N";
							degLatitude = (int)floor(fabs(latitude));
							minLatitude = (int)floor((fabs(latitude) - degLatitude)*60);
							secLatitude = (int)floor(((fabs(latitude) - degLatitude)*60 - minLatitude )*60);
						snprintf(scratchBufLatitude, 100, "%ld/1 %ld/1 %ld/1", degLatitude, minLatitude, secLatitude);
					exifData["Exif.GPSInfo.GPSLatitude"] = scratchBufLatitude;

					exifData["Exif.GPSInfo.GPSLongitudeRef"] = (longitude < 0 ) ? "W" : "E";
							degLongitude = (int)floor(fabs(longitude));
							minLongitude = (int)floor((fabs(longitude) - degLongitude)*60);
							secLongitude = (int)floor(((fabs(longitude) - degLongitude)*60 - minLongitude )*60);
						snprintf(scratchBufLongitude, 100, "%ld/1 %ld/1 %ld/1", degLongitude, minLongitude, secLongitude);
					exifData["Exif.GPSInfo.GPSLongitude"] = scratchBufLongitude;

				image->setExifData(exifData);
				image->writeMetadata();

				Exiv2::ExifMetadata::iterator it = exifData.findKey(Exiv2::ExifKey("Exif.GPSInfo.GPSVersionID"));
				if ( it != exifData.end())
				{
					std::ostringstream out;
					out << exifData.findKey(Exiv2::ExifKey("Exif.GPSInfo.GPSLatitudeRef"))->value().toString() << " ";
						Exiv2::ExifMetadata::iterator itL = exifData.findKey(Exiv2::ExifKey("Exif.GPSInfo.GPSLatitude"));
					out << itL->value().toFloat(0) << "deg " << itL->value().toFloat(1) << "' " << itL->value().toFloat(2) << "\"";

					std::cout << "writed Latitude: " << out.str() << "\n", fflush(0);
				}
				else
				{
					std::cout << "GPS tag is empty\n", fflush(0);
				}
		std::cout << "all OK\n", fflush(0);
	return 0;
}
