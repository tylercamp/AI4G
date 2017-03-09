#ifndef _ORE_FILE_IO_H_
#define _ORE_FILE_IO_H_

#include <ORE/Types.h>
#include <ORE/Namespaces.h>

#include <string>
#include <list>

#include "Diagnostics.h"

#define FILEIO_NAMESPACE_NAME FileIO

#define FILEIO_NAMESPACE_START namespace FILEIO_NAMESPACE_NAME {
#define FILEIO_NAMESPACE_END }

#define FILEIO_INTERNAL_NAMESPACE_NAME Internal
#define FILEIO_INTERNAL_NAMESPACE_START namespace FILEIO_INTERNAL_NAMESPACE_NAME {
#define FILEIO_INTERNAL_NAMESPACE_END }

//	WinAPI defines this as DeleteFileA/DeleteFileW; we use the name for our own stuff, so
//		get rid of it
#ifdef DeleteFile
#undef DeleteFile
#endif
#ifdef CopyFile
#undef CopyFile
#endif
#ifdef CreateDirectory
#undef CreateDirectory
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ORE_NAMESPACE_START
FILEIO_NAMESPACE_START/* Directory Management Functionality */

//	* SetActiveDirectory
//
// Sets the directory that is used as the relative path for
//	ALL file operations within this library (unless an absolute
//	path is specified.)
//
// Returns:
//	void;
//
// Reports to:
//
// - Warning diagnostic stream: "Unable to set active directory to $path$ - path length too long. Maximum length is $length$."
//
//					 Condition: If the path length is longer than the operating
//									system permits.
//
void SetActiveDirectory (t_String pathName);

//	* GetActiveDirectory
//
// Retrieves the current directory that is used as a
//	relative path. This is an absolute path.
//
// Returns:
//	The current active directory, as indicated by the active directory stack.
//
// Reports to:
//
// - No diagnostic streams.
//
const t_String GetActiveDirectory ();

void ResetActiveDirectory ();

//	* GetApplicationDirectory
//
// Retrieves the absolute path to the running application that
//	this function is called from.
//
// Returns:
//	const t_String - The directory to the running application, including the trailing slash.
//
// Reports to:
//
// - Warning diagnostic stream: "Unable to successfully retrieve application directory - path exceeds maximum length allowed by the operating system. (Apparently?)"
//
//					 Condition: If the operating system reports a failure while retrieving the application's directory. This should
//									not occur.
//
const t_String GetApplicationDirectory ();

//	Converts a path to being in relation to the active directory, if it's not
//		an absolute path.
const t_String TranslatePath (t_String path);

//	Only available in debug builds; ensures that correct formatting is within the given path,
//		and generates a warning if the given path doesn't conform. If formatting is not
//		resolved before the Release build, code may not operate as expected.

#ifdef ORE_DEBUG
#	define FormatPath(x) (ore::FileIO::Internal::FormatPathDebug(x))
#else
#	define FormatPath(x) (x)
#endif

//	Combines the two paths with proper formatting. firstPart must always be a directory,
//		and lastPart can be either a directory or a file.
const t_String CombinePaths (t_String firstPart, t_String lastPart);

bool PathIsAbsolute (t_String path);

bool PathIsValid (t_String path);

//	It's up to the user to free the object returned by this. TODO refactor
bool GetEntriesInDirectory (t_String directory, t_StringSet * stringSet);

//
bool EntryIsFolder (t_String entry);

//
bool EntryIsFile (t_String entry);

/* File Interaction Functionality */

/* Format:
 (..., ..., "Filter Name 1", "filter.ext", "Filter Name 2", "filter2.ext", ...)

 NOTE: These variadic arguments require C-strings, not t_Strings
 */
/*
 t_StringSet * DisplayOpenMultiFileDialogue (t_String defaultValue, t_UInteger filterCount, ...);
 t_String DisplayOpenSingleFileDialogue (t_String defaultValue, t_UInteger filterCount, ...);
 t_String DisplaySaveFileDialogue (t_String defaultValue, t_UInteger extensionCount, ...);
 */

//	* FileExists
//
// Returns:
//	bool - Whether or not the given file exists.
//
// Reports to:
//
// - Error diagnostic stream: "Unable to access directory $directory$ for detecting $file$ - don't have the required credentials."
//
//				   Condition: If the operating system reports that the application does not have the
//								credentials required to access the given directory/file. This check
//								is not guaranteed to be reported on all operating systems.
//
// - Error diagnostic stream: "Unable to access drive $drivelabel$ for checking file $filename$."
//
//				   Condition: If the file could not be checked due to a missing drive.
//
bool FileExists (t_String fileName);

bool DirectoryExists (t_String directoryName);

bool EntryExists (t_String entryName);

//	* GetFileContents
//
// Retrieves the contents of the given file and stores it in an
//	allocated array. The size of the data extracted (in bytes) is
//	stored in the optional integer pointer. It is up to the user
//	of the function to free the allocated memory.
//
// Returns:
//	t_Byte * - The allocated buffer containing the contents of the file; NULL on error.
//	- OR -
//	t_UInteger - The amount of data that was written to 'buffer'.
//
// Reports to:
//
// - Error diagnostic stream: "Unable to access file $file$."
//
//				   Condition: If the given file does not exist.
//
//							  May also be reported in the case of a permissions conflict
//								on an OS that does not provide information on such a conflict.
//								(Could not access the file because of permissions, but the
//								OS doesn't tell us anything about permissions, so it's
//								interpreted as a nonexistant file.)
//
// - Error diagnostic stream: "File $file$ could not be accessed - don't have the required credentials."
//
//				   Condition: If the file exists, but could not be accessed due to permissions conflicts.
//
// - Error diagnostic stream: "Unable to read all contents of file $file$."
//
//				   Condition: If the amount of bytes read from the file does not match the detected
//								size of the file.
//
// - Error diagnostic stream: "Unable to access drive $drivelabel$ for reading file $filename$."
//
//				   Condition: If the file could not be read due to a missing drive.
//
t_Byte * GetFileContents (t_String fileName, t_UInteger extraBytes = 0, t_UInteger * fileSize = NULL);
//	TODO: Document (writes to buffer, sets bufferSize to the size set
t_UInteger GetFileContents (t_String fileName, t_Byte * buffer, t_UInteger bufferSize);

//	* GetFileSize
//
// Attempts to retrieve the size of the given file.
//
// Returns:
//	t_UInteger - Size of the file; -1 on error.
//
// Reports to:
//
// - Error diagnostic stream: "Unable to access file $file$."
//
//				   Condition: If the given file does not exist.
//
//							  May also be signaled in the case of permissions conflicts - see GetFileContents
//								for more information.
//
// - Error diagnostic stream: "Unable to access drive $drivelabel$ for reading file $filename$."
//
//				   Condition: If the file could not be read due to a missing drive.
//
t_UInteger GetFileSize (t_String fileName);

//	* WriteFile
//
// Writes the contents of the given buffer to a given file. This operation completely
//	overwrites the contents of the file, if it already exists. If the file does not
//	exist, it is created. If the given path does not exist, it is created.
//
// Returns:
//	bool - Whether or not the operation was successful.
//
// Reports to:
//
// - Error diagnostic stream: "Unable to access path $path$ for writing file $filename$."
//
//				   Condition: If the path could not be accessed due to permissions conflicts.
//
// - Error diagnostic stream: "Unable to access drive $drivelabel$ for writing file $filename$."
//
//				   Condition: If the file could not be created due to a missing drive.
//
bool WriteFile (t_String fileName, t_Byte * fileContents, t_UInteger fileSize);

INLINE bool WriteFile (t_String fileName, const t_String & fileContents);

bool CreateDirectory (t_String directoryName);

bool CopyFile (t_String sourceFileName, t_String newFileName, bool overwritePrevious = true);

//	* DeleteFile
//
// Unlinks the given file from the directory.
//
// Returns:
//	bool - Whether or not the operation was successful.
//
// Reports to:
//
// - Error diagnostic stream: "Unable to access path $path$ for unlinking file $filename$."
//
//				   Condition: If the path could not be accessed due to permissions conflicts.
//
// - Error diagnostic stream: "Unable to access drive $drivelabel$ for unlinking file $filename$."
//
//				   Condition: If the file could not be unlinked due to a missing drive.
//
bool DeleteFile (t_String fileName);

bool DeleteDirectory (t_String directoryName);

/* Directory Group Functionality (Don't use this directly) */

struct DirectoryGroup
{
    bool FindFileInDirectory (t_String directory, t_String fileName, t_String * fileLocationOut);
    t_String FindFileInDirectoryGroup (t_String fileName);
    void AddPath (t_String path);
    void Clear ();

private:
    std::list <t_String> _directories;
};

DirectoryGroup * CreateDirectoryGroup ();

//	Adds the given path (which is translated to an absolute path (if it isn't already)
//		by using the current active directory) to the directory group
void AddPathToDirectoryGroup (DirectoryGroup * directoryGroup, t_String path);

//
void ClearDirectoryGroup (DirectoryGroup * directoryGroup);

//
void DeleteDirectoryGroup (DirectoryGroup * directoryGroup);

//
t_String FindFileInDirectoryGroup (DirectoryGroup * directoryGroup, t_String fileName);

//	Might eventually want to write a general-purpose Stream system, since it looks
//		like the stream read/write API is meant to allow different types of streams,
//		other than File streams

//	File operations were split into reading/writing segments in order to allow detection
//		of things like trying to write to a reading stream and vice-versa. It was simplest
//		to split them into two separate classes and simply do comparisons between the type
//		of stream and the type that was necessary, but this check can also be done in
//		a single class, and would avoid this API clutter. But combining them is
//		something to be done in the future.

//	TODO: Make stream object that allows custom access styles (i.e. text instead of binary,
//			enable use of appended writing)
//	TODO: Make a commonly-used stream opening mechanism (for internal use); currently,
//			there's a lot of copy/pasted code in the OpenStream methods that could
//			simply be left to a method that has the stream type as a parameter
struct FileStream
{
    enum e_StreamType
    {
        READ_ONLY = 1,
        WRITE_ONLY = 2,
        //	Not implemented (Gotta' love diamond inheritance; this functionality is not
        //		currently imperative, so taking the time to restructure this isn't
        //		worth it right now)
        READ_WRITE = 4
    };

    FileStream ();
    virtual ~FileStream ();

    virtual void OpenStream (t_String fileName) = 0;
    virtual e_StreamType GetStreamType () = 0;
    virtual t_String GetStreamTypeName () = 0;
    t_UInteger GetSize ();

    t_UInteger GetCursorPosition ();
    void SetCursorPosition (t_UInteger position);

    void * GetFileHandle ();

    bool IsGood ();
    bool IsOpen ();

protected:
    FILE * _fileHandle;
};

FileStream * OpenStream (t_String fileName, FileStream::e_StreamType streamType);
//	Finalizes changes (if any were made), deletes the object and closes the stream
void CloseStream (FileStream * stream);

t_UInteger GetStreamSize (FileStream * stream);

void StreamSetCursorPosition (FileStream * stream, t_UInteger position);
t_UInteger StreamGetCursorPosition (FileStream * stream);

bool StreamIsWrite (FileStream * stream);
bool StreamIsRead (FileStream * stream);

template <typename T>
void StreamWrite (FileStream * stream, const T& data);

void StreamWriteBuffer (FileStream * stream, t_Byte * buffer, t_UInteger count);

template <typename T>
T StreamRead (FileStream * stream);

t_Byte * StreamReadBuffer (FileStream * stream, t_UInteger count);

bool StreamIsEOF (FileStream * stream);

//	Enables easy reading/writing when only one file stream is currently in use
void SetActiveStream (FileStream * stream);
FileStream * GetActiveStream ();

void SetCursorPosition (t_UInteger position);
t_UInteger GetCursorPosition ();

template <typename T>
void Write (const T& data);

void WriteBuffer (t_Byte * buffer, t_UInteger count);

template <typename T>
T Read ();

t_Byte * ReadBuffer (t_UInteger count);

bool IsEOF ();

//	These are not to be used directly; use the FormatPath macro instead.
FILEIO_INTERNAL_NAMESPACE_START

//	Formats the path and outputs a warning if the path did not conform to standards
t_String FormatPathDebug (t_String path);

//	Simply formats the given path
t_String FormatPathForce (t_String path);

FILEIO_INTERNAL_NAMESPACE_END
FILEIO_NAMESPACE_END

FILEIO_NAMESPACE_START

//	Inlined function variations for speed
bool WriteFile (t_String file, const t_String & fileContents)
{
    return WriteFile (file, (t_Byte *)fileContents.c_str (), fileContents.size ()+1);
}

struct ReadFileStream
: public FileStream
{
    virtual e_StreamType GetStreamType ();
    virtual t_String GetStreamTypeName ();

    virtual void OpenStream (t_String fileName);

    //	The memory returned by this will have to be deleted by the user.
    t_Byte * ReadData (t_UInteger count);
    //	Includes the symbol itself
    t_Byte * ReadToSymbol (char symbol);

    bool IsEOF ();

    template <typename T>
    T ReadVariable ();
};

struct WriteFileStream
: public FileStream
{
    virtual e_StreamType GetStreamType ();
    virtual t_String GetStreamTypeName ();

    //	If 'append' is set to false, the file will be formatted upon opening
    virtual void OpenStream (t_String fileName);

    void WriteData (t_Byte * data, t_UInteger size);

    template <typename T>
    void WriteVariable (const T& variable);
};

/*
 struct MultiFileStream
 : public ReadFileStream, public WriteFileStream
 {
 e_StreamType GetStreamType ();
 t_String GetStreamTypeName ();

 void OpenStream (t_String fileName);
 };
 */

template <typename T>
T ReadFileStream::ReadVariable ()
{
    T returnValue;
    t_Byte * data = ReadData (sizeof(T));
    returnValue = *((T *)data);
    //	TODO: Replace deletion with library
    //	TODO: Re-implement with a more efficient system (allocating memory every time isn't
    //			very efficient)
    delete [] data;
    return returnValue;
}

template <>
inline t_String ReadFileStream::ReadVariable <t_String> ()
{
    t_String returnValue;
    t_Byte * data = ReadToSymbol ('\0');
    returnValue = (char *)data;
    delete [] data;
    return returnValue;
}

template <typename T>
void WriteFileStream::WriteVariable (const T& variable)
{
    WriteData ((t_Byte *)&variable, sizeof(T));
}

template <>
inline void WriteFileStream::WriteVariable <t_String> (const t_String& variable)
{
    WriteData ((t_Byte *)C_STRING (variable), variable.size () + 1);
}

template <typename T>
void StreamWrite (FileStream * stream, const T& data)
{
    if (!StreamIsWrite (stream))
    {
        Diagnostics::Warning (true)
        .Append ("FileIO.h | ore::FileIO::StreamWrite<T> - Attempted to write to a stream that has not been configured for writing.")
        .Append ("\nStream configuration: ").Append (stream->GetStreamTypeName ())
        .Display ();
    }

    ((WriteFileStream *)stream)->WriteVariable <T> (data);
}

template <typename T>
T StreamRead (FileStream * stream)
{
    if (!StreamIsRead (stream))
    {
        Diagnostics::Warning (true)
        .Append ("FileIO.h | ore::FileIO::StreamRead<T> - Attempted to read from a stream that has not been configured for reading.")
        .Append ("\nStream configuration: ").Append (stream->GetStreamTypeName ())
        .Display ();
    }

    return ((ReadFileStream *)stream)->ReadVariable <T> ();
}

template <typename T>
void Write (const T& data)
{
    StreamWrite (GetActiveStream (), data);
}

template <typename T>
T Read ()
{
    return StreamRead <T> (GetActiveStream ());
}

FILEIO_NAMESPACE_END
ORE_NAMESPACE_END

#endif
