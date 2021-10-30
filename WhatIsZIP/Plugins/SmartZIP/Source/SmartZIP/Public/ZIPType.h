#pragma once
#include "ZIPType.generated.h"

// These are the result codes:
#define SMART_ZIP_OK         0x00000000     // nb. the pseudo-code zr-recent is never returned,
#define SMART_ZIP_RECENT     0x00000001     // but can be passed to FormatZipMessage.
// The following come from general system stuff (e.g. files not openable)
#define SMART_ZIP_GENMASK    0x0000FF00
#define SMART_ZIP_NODUPH     0x00000100     // couldn't duplicate the handle
#define SMART_ZIP_NOFILE     0x00000200     // couldn't create/open the file
#define SMART_ZIP_NOALLOC    0x00000300     // failed to allocate some resource
#define SMART_ZIP_WRITE      0x00000400     // a general error writing to the file
#define SMART_ZIP_NOTFOUND   0x00000500     // couldn't find that file in the zip
#define SMART_ZIP_MORE       0x00000600     // there's still more data to be unzipped
#define SMART_ZIP_CORRUPT    0x00000700     // the zipfile is corrupt or not a zipfile
#define SMART_ZIP_READ       0x00000800     // a general error reading the file
// The following come from mistakes on the part of the caller
#define SMART_ZIP_CALLERMASK 0x00FF0000
#define SMART_ZIP_ARGS       0x00010000     // general mistake with the arguments
#define SMART_ZIP_NOTMMAP    0x00020000     // tried to ZipGetMemory, but that only works on mmap zipfiles, which yours wasn't
#define SMART_ZIP_MEMSIZE    0x00030000     // the memory size is too small
#define SMART_ZIP_FAILED     0x00040000     // the thing was already failed when you called this function
#define SMART_ZIP_ENDED      0x00050000     // the zip creation has already been closed
#define SMART_ZIP_MISSIZE    0x00060000     // the indicated input file size turned out mistaken
#define SMART_ZIP_PARTIALUNZ 0x00070000     // the file had already been partially unzipped
#define SMART_ZIP_ZMODE      0x00080000     // tried to mix creating/opening a zip 
// The following come from bugs within the zip library itself
#define SMART_ZIP_BUGMASK    0xFF000000
#define SMART_ZIP_NOTINITED  0x01000000     // initialisation didn't work
#define SMART_ZIP_SEEK       0x02000000     // trying to seek in an unseekable file
#define SMART_ZIP_NOCHANGE   0x04000000     // changed its mind on storage, but not allowed
#define SMART_ZIP_FLATE      0x05000000     // an internal error in the de/inflation code

namespace SmartZIP
{
	void CollectErrorInformation(unsigned long Code);
}

UENUM(BlueprintType)
enum class EZipType :uint8
{
	E_ZIP,
	E_UNZIP,
};

DECLARE_DYNAMIC_DELEGATE_FourParams(FOnZipFileCallbackFunction,EZipType,InType,int32,InTotal,int32 ,InProgress,const FString,InFileName);