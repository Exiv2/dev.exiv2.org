#!/usr/bin/env bash

# Usage: keepTags.sh <filename>

shopt -s extglob

if ! which exiv2 &>/dev/null; then
    echo "exiv2 is required"
    exit 1
fi

filename="$1"

# a list of all tags that will be kept
KEEP_TAGS=()
KEEP_TAGS+=("Exif.Image.Make")
KEEP_TAGS+=("Exif.Image.Model")
KEEP_TAGS+=("Exif.Photo.ExposureTime")
KEEP_TAGS+=("Exif.Photo.FNumber")
KEEP_TAGS+=("Exif.Photo.ISOSpeedRatings")
KEEP_TAGS+=("Exif.Photo.SensitivityType")
KEEP_TAGS+=("Exif.Photo.ExifVersion")

all_tags="$(exiv2 -p a pr "$filename" | awk '{print $1}')"
declare remove_flag
commands=()
while read -r tag; do
    remove_flag=true
    for keep in "${KEEP_TAGS[@]}"; do
        if [ "$keep" == $tag ]; then
            remove_flag=false
        fi
    done
    if $remove_flag; then
        commands+=("-Mdel $tag")
    fi
done <<< "$all_tags"

exiv2 "${commands[@]}" "$filename"
