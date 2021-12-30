#!/usr/bin/env bash

TOKEEP="Exif.Image.Make|\
Exif.Image.Model|\
Exif.Photo.ExposureTime|\
Exif.Photo.FNumber|\
Exif.Photo.ISOSpeedRatings|\
Exif.Photo.DateTimeOriginal|\
Exif.Photo.ShutterSpeedValue|\
Exif.Photo.ApertureValue|\
Exif.Photo.Flash|\
Exif.Photo.FocalLength|\
Exif.Photo.ColorSpace|\
Exif.Photo.PixelXDimension|\
Exif.Photo.PixelYDimension|\
Exif.Photo.FocalLengthIn35mmFilm"

for i in "$@"; do
	exiv2 -Pkyv "$i" > "$i.cmd1.txt"
	egrep "$TOKEEP" "$i.cmd1.txt" > "$i.cmd2.txt"
	while read LINE; do BEG="set $LINE"; echo $BEG >> "$i.cmd3.txt"; done < "$i.cmd2.txt"
	exiv2 rm "$i"
	exiv2 -m "$i.cmd3.txt" "$i"

	#Clean Up
	rm "$i.cmd1.txt" "$i.cmd2.txt" "$i.cmd3.txt"
done

