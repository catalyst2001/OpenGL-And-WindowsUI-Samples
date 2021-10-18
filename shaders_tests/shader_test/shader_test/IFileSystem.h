#pragma once
#include <stdio.h>
#include <stdint.h>

/**
* Filesystem file interface
*/
class IFile
{
public:
	/**
	* @brief End of file check
	* @returns True if the offset is at the end of the file, false otherwise
	*/
	virtual const char *FileName() = 0;
	virtual bool EndOfFile() = 0;

	/**
	* @brief Get an error while working with a file
	* @returns Error code. NULL if no error, otherwise any other number.
	*/
	virtual int Error() = 0;

	virtual int Seek(int offset, int pos) = 0;
	virtual int Tell() = 0;
	virtual int Size() = 0;
	virtual int Close() = 0;

	/**
	* @brief Read bytes from file
	* @param Destination buffer
	* @param Number of bytes for reading. NULL to read the string to terminal zero
	* @returns Number of readed bytes
	*/
	virtual int ReadBytes(void *buffer, int nbytes) = 0;

	/**
	* @brief Reading string from file
	* @param Destination buffer
	* @returns Destination buffer start address
	*/
	inline char *ReadString(char *buffer) { 
		ReadBytes(buffer, NULL);
		return buffer;
	}

	/**
	* @brief Writing bytes to file
	* @param Source buffer
	* @param Number of bytes for writing. NULL to write the string to terminal zero
	* @returns Number of writed bytes.
	*/
	virtual int WriteBytes(void *buffer, int nbytes) = 0;

	/**
	* @brief Writing string to file
	* @param Source buffer
	* @returns Number of writed bytes.
	*/
	inline int WriteString(char *string) { return WriteBytes(string, NULL); }

	inline int WriteByte(char byteval) { return WriteBytes(&byteval, sizeof(byteval)); };
	inline char ReadByte() {
		char rbyte;
		ReadBytes(&rbyte, sizeof(rbyte));
		return rbyte;
	}

	inline int WriteShort(short shortval) { return WriteBytes(&shortval, sizeof(shortval)); }
	inline short ReadShort() {
		short shortv;
		ReadBytes(&shortv, sizeof(shortv));
		return shortv;
	}

	inline int WriteInt(int intval) { return WriteBytes(&intval, sizeof(intval)); }
	inline int ReadInt() {
		int n;
		ReadBytes(&n, sizeof(n));
		return n;
	}

	inline int WriteFloat(float flval) { return WriteBytes(&flval, sizeof(flval)); }
	inline float ReadFloat() {
		float floatval;
		ReadBytes(&floatval, sizeof(floatval));
		return floatval;
	}
};

#define FS_MAX_PATH 260
struct FileInfo_s
{
	char filename[FS_MAX_PATH];
	uint32_t size_in_disk;
	uint64_t creation_date;
};

class IDirectory
{
public:
	/**
	* @prief Get current directory name
	* @returns Current directory name
	*/
	virtual const char *DirectoryName() = 0;

	/**
	* @prief Find first file in this diectory
	* @param Pointer to file info structure 
	* @param Search file pattern. If NULL, find all contained files
	* @returns true if first file exists, false if the directory contains no files
	*/
	virtual int FirstFile(FileInfo_s *fileinfo, const char *searchpattern) = 0;

	/**
	* @prief Find next file in this directory
	* @param Pointer to file info structure
	* @returns Return 1 if next file is exists, otherwise 0
	*/
	virtual int NextFile(FileInfo_s *fileinfo) = 0;

	/**
	* @prief Remove this directory extended
	* @param True, if you need to delete all selected subdirectories and files, otherwise the function will fail if the directory to be removed is not empty 
	* @param The template by which to delete files. If this parameter is NULL, all files will be deleted
	* @param The template by which to delete directories. If this parameter is NULL, all files will be deleted
	* @returns 1 if the operation completed successfully, otherwise 0
	*/
	virtual int RemoveEx(bool delchilds, const char *fpattern, const char *dpattern) = 0;

	/**
	* @prief Remove all contents in current directory
	* @returns 1 if the operation completed successfully, otherwise 0
	*/
	virtual int ClearDirectory() = 0;

	/**
	* @prief Remove current directory
	* @returns 1 if the operation completed successfully, otherwise 0
	*/
	virtual int Remove() = 0;

	/**
	* @prief Close current directory
	* @returns 1 if the operation completed successfully, otherwise 0
	* @note Call LastError() for get error, if Close() was returned 0
	*/
	virtual int Close() = 0;

	/**
	* @prief Get last error in current directory
	* @returns Last error code
	*/
	virtual int LastError() = 0;
};

/**
* File system file modes
*/
#define FSF_OPEN_EXIST (1 << 0)     /* Open exists file */
#define FSF_OPEN_CREATE (1 << 1)    /* Create new file. If file exists, file will be rewrited */
#define FSF_OPEN_MEMORY (1 << 2)    /* Open virtual file in memory */
#define FSF_OPEN_LOCKED (1 << 3)    /* Lock created file */
#define FSF_WRITE (1 << 4)          /* File open in write mode */
#define FSF_READ (1 << 5)           /* File open in reading mode */
#define FSF_LOAD_TO_MEMORY (1 << 6) /* Load opened file to memory */

/**
* Direct engine filesystem interface
*/
class IFileSystem
{
public:
	/**
	* @brief Returns the last error that occurred when calling functions of IFileSystem interface
	* @returns Error code
	*/
	virtual int LastError() = 0;

	/**
	* @brief Creates a physical or virtual file for writing or reading, opens application memory and shared resources in memory, or opens system devices to send IOCTL requests
	* 
	*/
	virtual IFile *OpenFileEx(const char *filename, int flags) = 0;
	virtual IDirectory *OpenDirectory(const char *directory) = 0;
};