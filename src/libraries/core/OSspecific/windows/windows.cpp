/*---------------------------------------------------------------------------*\
Copyright (C) 2011 Symscape
Copyright (C) 2018 Applied CCM Pty Ltd
-------------------------------------------------------------------------------
License
    This file is part of CAELUS.

    CAELUS is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CAELUS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with CAELUS.  If not, see <http://www.gnu.org/licenses/>.

Description
    MS Windows specific functions

\*---------------------------------------------------------------------------*/

#include "OSspecific.hpp"
#include "Windows.hpp"
#include "caelusVersion.hpp"
#include "fileName.hpp"
#include "fileStat.hpp"

// Undefine DebugInfo, because we don't need it and it collides with a macro
// in windows.h
#undef DebugInfo

#include <cassert>
#include <cstdlib>
#include <fstream>
#include <map>

// Windows system header files
#include <io.h> // _close
#include <windows.h>
#include <signal.h>



// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


namespace CML 
{

    defineTypeNameAndDebug(Windows, 0);

// Don't abort under Windows, causes abort dialog to
// popup. Instead just exit with exitCode.
static
void sigAbortHandler(int exitCode)
{
  ::exit(exitCode);
}


static
bool installAbortHandler()
{
  // If it didn't succeed there's not much we can do,
  // so don't check result.
  ::signal(SIGABRT, &sigAbortHandler);
  return true;
}


static bool const abortHandlerInstalled = installAbortHandler();


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


//- Get last Windows api error from GetLastError
std::string Windows::getLastError()
{
    // Based on an example at:
    // http://msdn2.microsoft.com/en-us/library/ms680582(VS.85).aspx

    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        LPTSTR(&lpMsgBuf),
        0, nullptr );

    lpDisplayBuf = LocalAlloc(LMEM_ZEROINIT, 
        (lstrlen(static_cast<LPCTSTR>(lpMsgBuf))+40)*sizeof(TCHAR)); 
    sprintf(static_cast<LPTSTR>(lpDisplayBuf),
            "Error %d: %s", int(dw), static_cast<LPCTSTR>(lpMsgBuf));

    const std::string errorMessage = static_cast<LPTSTR>(lpDisplayBuf);

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);

    return errorMessage;
}


//-Declared here to avoid polluting Windows.hpp with windows.h
namespace Windows
{
    //- Get Windows user name
    std::string getUserName();

    //- Remove quotes, if any, from std::string
    void removeQuotes(std::string & arg);

    //- Convert Windows directory slash (back-slash) to unix (forward-slash). 
    //- Windows is fine with unix like directory slashes.
    //- CML's file io (see src/CAELUS/db/IOstreams/Sstreams/OSwrite.C) 
    //- uses back-slash as escape character and continuation, 
    //- so not an option to have Windows file paths with back-slashes
    void toUnixSlash(string & arg);

    //- Auto create and then delete array when this goes out of scope
    template<class T>
    class AutoArray
    {
      T* const array_;

    public:
      AutoArray(const unsigned long arrayLength);
      ~AutoArray();

      //- Access array
      T* get();
    }; // class AutoArray


    //- Directory contents iterator
    class DirectoryIterator
    {
      WIN32_FIND_DATA findData_;
      HANDLE findHandle_;
      fileName nextName_;
      bool hasMore_;
      
    public:
      DirectoryIterator(const fileName & directory);
      ~DirectoryIterator();
      
      //- Initialization succeeded
      bool isValid() const;

      //- Has more?
      bool hasNext() const;
      
      //- Next item
      const fileName & next();
    }; // class DirectoryIterator
} // namespace Windows


inline
void Windows::removeQuotes(std::string & arg)
{
    std::size_t pos;

    while (std::string::npos != (pos = arg.find('"')))
    {
        arg.erase(pos, 1);
    }
}


inline
void Windows::toUnixSlash(string & arg)
{
    arg.replaceAll("\\", "/");

    const std::string UNC("//");

    // Preserve UNC i.e., \\machine-name\...
    if (0 == arg.find(UNC)) 
    {
        arg.replace(UNC, "\\\\");
    }
}


std::string Windows::getUserName()
{
    const DWORD bufferSize = 256;
    TCHAR buffer[bufferSize];
    DWORD actualBufferSize = bufferSize;
    std::string nameAsString;

    bool success = ::GetUserName(buffer, &actualBufferSize);

    if (success)
    {
        nameAsString = buffer;
    }
    else 
    {
        if (ERROR_INSUFFICIENT_BUFFER == ::GetLastError() &&
            32768 > actualBufferSize) 
        {
            AutoArray<TCHAR> actualBuffer(actualBufferSize);
            ::GetUserName(actualBuffer.get(), &actualBufferSize);
            nameAsString = actualBuffer.get();
        }
    }

    return nameAsString;
}


template<class T>
inline
Windows::AutoArray<T>::AutoArray(const unsigned long arrayLength)
    : array_(new T[arrayLength])
{}


template<class T>
inline
Windows::AutoArray<T>::~AutoArray()
{
    delete [] array_;
}


template<class T>
inline
T* Windows::AutoArray<T>::get()
{
    return array_;
}


inline
bool Windows::DirectoryIterator::isValid() const
{
    const bool valid = (INVALID_HANDLE_VALUE != findHandle_);
    return valid;
}

    
Windows::DirectoryIterator::DirectoryIterator(const fileName & directory)
{
    const fileName directoryContents = directory/"*";
    findHandle_ = ::FindFirstFile(directoryContents.c_str(), &findData_);
    hasMore_    = isValid();
}
        

Windows::DirectoryIterator::~DirectoryIterator()
{
    if (isValid()) 
    {
        ::FindClose(findHandle_);
    }
}


inline
bool Windows::DirectoryIterator::hasNext() const
{
    assert(isValid());

    return hasMore_;
}


inline
const fileName & Windows::DirectoryIterator::next()
{
    assert(hasNext());

    nextName_ = findData_.cFileName;
    hasMore_  = ::FindNextFile(findHandle_, &findData_);

    return nextName_;
}


PID_T pid()
{
#ifdef WIN32
    const DWORD processId = ::GetCurrentProcessId();
#elif WIN64
    const pid_t processId = (pid_t) ::GetCurrentProcessId();
#endif
    return processId;
}


PID_T ppid()
{
    // No equivalent under Windows.

    if (Windows::debug)
    {
        InfoInFunction
            << "ppid not supported under Windows" << endl;
    }

    return 0;
}


PID_T pgid()
{
    // No equivalent under Windows.

    if (Windows::debug)
    {
        InfoInFunction
            << "pgid not supported under Windows" << endl;
    }

    return 0;
}


bool env(const word& envName)
{
    const DWORD actualBufferSize = 
      ::GetEnvironmentVariable(envName.c_str(), nullptr, 0);

    const bool envExists = (0 < actualBufferSize);
    return envExists;
}


string getEnv(const word& envName)
{
    std::string envAsString;

    const DWORD actualBufferSize = 
      ::GetEnvironmentVariable(envName.c_str(), nullptr, 0);

    if (0 < actualBufferSize) 
    {
        Windows::AutoArray<TCHAR> actualBuffer(actualBufferSize);
        ::GetEnvironmentVariable(envName.c_str(),
                                 actualBuffer.get(),
                                 actualBufferSize);
        envAsString = actualBuffer.get();
        toUnixPath(envAsString);
    }

    return envAsString;
}


bool setEnv
(
    const word& envName,
    const std::string& value,
    const bool /*overwrite*/
)
{
    const bool success = 
      ::SetEnvironmentVariable(envName.c_str(), value.c_str());
    return success;
}


string hostName(const bool full)
{
    const DWORD bufferSize = MAX_COMPUTERNAME_LENGTH + 1;
    TCHAR buffer[bufferSize];
    DWORD actualBufferSize = bufferSize;

    const bool success = 
      ::GetComputerName(buffer, &actualBufferSize);
    const string computerName = success ? buffer : string::null;
    return computerName;
}


string domainName()
{
    // Could use ::gethostname and ::gethostbyname like POSIX.C, but would
    // then need to link against ws_32. Prefer to minimize dependencies.

    return string::null;
}


string userName()
{
    string nameAsString = getEnv("USERNAME");

    if (nameAsString.empty()) 
    {
        nameAsString = Windows::getUserName();
    }

    return nameAsString;
}


bool isAdministrator()
{
    // Not supported assume worst case
    return true;
}


fileName home()
{
    std::string homeDir = getEnv("HOME");

    if (homeDir.empty()) 
    {
        homeDir = getEnv("USERPROFILE");
    }

    return homeDir;
}


fileName home(const string& userName)
{
    return home();
}


fileName cwd()
{
    string currentDirectory;

    const DWORD actualBufferSize = 
      ::GetCurrentDirectory(0, nullptr);

    if (0 < actualBufferSize) 
    {
        Windows::AutoArray<TCHAR> actualBuffer(actualBufferSize);
        ::GetCurrentDirectory(actualBufferSize,
                              actualBuffer.get());   
        currentDirectory = actualBuffer.get();
        Windows::toUnixSlash(currentDirectory);
    }
    else 
    {
        FatalErrorInFunction
            << "Couldn't get the current working directory"
            << exit(FatalError);
    }

    return currentDirectory;
}


bool chDir(const fileName& dir)
{
    const bool success = ::SetCurrentDirectory(dir.c_str());
    return success; 
}


fileNameList findEtcFiles
(
    const fileName& name,
    bool mandatory,
    bool findFirst
)
{
    fileNameList results;

    // Search for user files in
    // * ~/.CAELUS/VERSION
    // * ~/.CAELUS
    //
    fileName searchDir = home()/".CAELUS";
    if (isDir(searchDir))
    {
        fileName fullName = searchDir/CAELUSversion/name;
        if (isFile(fullName))
        {
            results.append(fullName);
            if (findFirst)
            {
                return results;
            }
        }

        fullName = searchDir/name;
        if (isFile(fullName))
        {
            results.append(fullName);
            if (findFirst)
            {
                return results;
            }
        }
    }

    // Search for group (site) files in
    // * $CAELUS_PROJECT_SITE/VERSION
    // * $CAELUS_PROJECT_SITE
    //
    searchDir = getEnv("CAELUS_PROJECT_SITE");
    if (searchDir.size())
    {
        if (isDir(searchDir))
        {
            fileName fullName = searchDir/CAELUSversion/name;
            if (isFile(fullName))
            {
                results.append(fullName);
                if (findFirst)
                {
                    return results;
                }
            }

            fullName = searchDir/name;
            if (isFile(fullName))
            {
                results.append(fullName);
                if (findFirst)
                {
                    return results;
                }
            }
        }
    }
    else
    {
        // OR search for group (site) files in
        // * $CAELUS_PROJECT_INST_DIR/site/VERSION
        // * $CAELUS_PROJECT_INST_DIR/site
        //
        searchDir = getEnv("CAELUS_PROJECT_INST_DIR");
        if (isDir(searchDir))
        {
            fileName fullName = searchDir/"site"/CAELUSversion/name;
            if (isFile(fullName))
            {
                results.append(fullName);
                if (findFirst)
                {
                    return results;
                }
            }

            fullName = searchDir/"site"/name;
            if (isFile(fullName))
            {
                results.append(fullName);
                if (findFirst)
                {
                    return results;
                }
            }
        }
    }

    // Search for other (shipped) files in
    // * $CAELUS_PROJECT_DIR/etc
    //
    searchDir = getEnv("CAELUS_PROJECT_DIR");
    if (isDir(searchDir))
    {
        fileName fullName = searchDir/"etc"/name;
        if (isFile(fullName))
        {
            results.append(fullName);
            if (findFirst)
            {
                return results;
            }
        }
    }

    // Not found
    if (results.empty())
    {
        // Abort if the file is mandatory, otherwise return null
        if (mandatory)
        {
            std::cerr
                << "--> CAELUS FATAL ERROR in CML::findEtcFiles() :"
                   " could not find mandatory file\n    '"
                << name.c_str() << "'\n\n" << std::endl;
            ::exit(1);
        }
    }

    // Return list of matching paths or empty list if none found
    return results;
}


fileName findEtcFile(const fileName& name, bool mandatory)
{
    // Search most likely location first
    // Search installation files:
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~
    fileName searchDir = getEnv("CAELUS_PROJECT_DIR");
    if (isDir(searchDir))
    {
        // Check for shipped CAELUS file in $CAELUS_PROJECT_DIR/etc
        fileName fullName = searchDir/"etc"/name;
        if (isFile(fullName))
        {
            return fullName;
        }
    }

    // Search user files:
    // ~~~~~~~~~~~~~~~~~~
    searchDir = home()/".CAELUS";
    if (isDir(searchDir))
    {
        // Check for user file in ~/.CAELUS/VERSION
        fileName fullName = searchDir/CAELUSversion/name;
        if (isFile(fullName))
        {
            return fullName;
        }

        // Check for version-independent user file in ~/.CAELUS
        fullName = searchDir/name;
        if (isFile(fullName))
        {
            return fullName;
        }
    }


    // Search site files:
    // ~~~~~~~~~~~~~~~~~~
    searchDir = getEnv("CAELUS_PROJECT_INST_DIR");
    if (isDir(searchDir))
    {
        // Check for site file in $CAELUS_PROJECT_INST_DIR/site/VERSION
        fileName fullName = searchDir/"site"/CAELUSversion/name;
        if (isFile(fullName))
        {
            return fullName;
        }

        // Check for version-independent site file in $CAELUS_PROJECT_INST_DIR/site
        fullName = searchDir/"site"/name;
        if (isFile(fullName))
        {
            return fullName;
        }
    }

    // Not found
    // abort if the file is mandatory, otherwise return null
    if (mandatory)
    {
        cerr<< "--> CAELUS FATAL ERROR in CML::findEtcFile() :"
               " could not find mandatory file\n    '"
            << name.c_str() << "'\n\n" << std::endl;
        ::exit(1);
    }

    // Return null-constructed fileName rather than fileName::null
    // to avoid cyclic dependencies in the construction of globals
    return fileName();
}


bool mkDir(const fileName& pathName, const mode_t mode)
{
    if (pathName.empty())
    {
        return false;
    }

    bool success = ::CreateDirectory(pathName.c_str(), nullptr);
    if (success)
    {
        chMod(pathName, mode);
    }
    else 
    {
        const DWORD error = ::GetLastError();

        switch (error)
        {
            case ERROR_ALREADY_EXISTS:
            {
                success = true;
                break;
            }
            case ERROR_PATH_NOT_FOUND:
            {
                // Part of the path does not exist so try to create it
                const fileName& parentName = pathName.path();

                if (parentName.size() && mkDir(parentName, mode))
                {
                    success = mkDir(pathName, mode);
                }
                
                break;
            }  
        }

        if (!success) 
        {
            FatalErrorInFunction
              << "Couldn't create directory: " << pathName
              << " " << Windows::getLastError()
              << exit(FatalError);
        }
    }

    return success;
}


// Set the file mode
bool chMod(const fileName& name, const mode_t m)
{
    const int success = _chmod(name.c_str(), m);
    return success;
}


// Return the file mode
mode_t mode(const fileName& name)
{
    fileStat fileStatus(name);

    const mode_t m = fileStatus.isValid() ?
      fileStatus.status().st_mode : 0;
    return m;
}


// Return the file type: FILE or DIRECTORY
fileName::Type type(const fileName& name)
{
    fileName::Type fileType = fileName::UNDEFINED;
    const DWORD attrs = ::GetFileAttributes(name.c_str());

    if (attrs != INVALID_FILE_ATTRIBUTES) 
    {
        fileType = (attrs & FILE_ATTRIBUTE_DIRECTORY) ?
            fileName::DIRECTORY :
            fileName::FILE;
    }

    return fileType;
}


static
bool 
isGzFile(const fileName& name)
{
    std::string gzName(name);
    gzName += ".gz";
    const DWORD attrs = ::GetFileAttributes(gzName.c_str());
    const bool success = (attrs != INVALID_FILE_ATTRIBUTES);

    return success;
}


// Does the name exist in the filing system?
bool exists
(
    const fileName& name,
    const bool checkGzip
)
{
    const DWORD attrs = ::GetFileAttributes(name.c_str());
    const bool success = (attrs != INVALID_FILE_ATTRIBUTES) || 
                         (checkGzip && isGzFile(name));

    return success;
}


// Does the directory exist
bool isDir(const fileName& name)
{
    const DWORD attrs = ::GetFileAttributes(name.c_str());
    bool success = (attrs != INVALID_FILE_ATTRIBUTES) &&
                   (attrs & FILE_ATTRIBUTE_DIRECTORY);

    return success;
}


// Does the file exist
bool isFile
(
    const fileName& name,
    const bool checkGzip
)
{
    const DWORD attrs = ::GetFileAttributes(name.c_str());
    const bool success =
    (
        (attrs != INVALID_FILE_ATTRIBUTES) && 
        !(attrs & FILE_ATTRIBUTE_DIRECTORY)
    )
     ||
    (
        checkGzip && isGzFile(name)
    );

    return success;
}


// Return size of file
off_t fileSize(const fileName& name)
{
    fileStat fileStatus(name);

    const off_t fileSize = fileStatus.isValid() ?
      fileStatus.status().st_size : -1;
    return fileSize;
}


// Return time of last file modification
time_t lastModified(const fileName& name)
{
    fileStat fileStatus(name);

    const time_t modifiedTime = fileStatus.isValid() ?
      fileStatus.status().st_mtime : 0;
    return modifiedTime;
}


// Read a directory and return the entries as a string list
fileNameList readDir
(
    const fileName& directory,
    const fileName::Type type,
    const bool filtergz
)
{
    // Initial filename list size
    // also used as increment if initial size found to be insufficient
    const int maxNnames = 100;

    if (Windows::debug)
    {
        Info<< "readDir(const fileName&, const fileType, const bool filtergz)"
            << " : reading directory " << directory << endl;
    }

    // Setup empty string list MAXTVALUES long
    fileNameList dirEntries(maxNnames);

    // Temporary variables and counters
    label nEntries = 0;

    Windows::DirectoryIterator dirIt(directory);

    if (dirIt.isValid())
    {
        while (dirIt.hasNext())
        {
            const fileName & fName = dirIt.next();

            // ignore files begining with ., i.e. '.', '..' and '.*'
            if (fName.size() > 0 && fName[size_t(0)] != '.')
            {
                word fileNameExt = fName.ext();

                if
                (
                    (type == fileName::DIRECTORY)
                 ||
                    (
                        type == fileName::FILE
                        && fName[fName.size()-1] != '~'
                        && fileNameExt != "bak"
                        && fileNameExt != "BAK"
                        && fileNameExt != "old"
                        && fileNameExt != "save"
                    )
                )
                {
                    if ((directory/fName).type() == type)
                    {
                        if (nEntries >= dirEntries.size())
                        {
                            dirEntries.setSize(dirEntries.size() + maxNnames);
                        }

                        if (filtergz && fileNameExt == "gz")
                        {
                            dirEntries[nEntries++] = fName.lessExt();
                        }
                        else
                        {
                            dirEntries[nEntries++] = fName;
                        }
                    }
                }
            }
        }
    }
    else if (Windows::debug)
    {
        InfoInFunction
            << "cannot open directory " << directory << endl;
    }

    // Reset the length of the entries list
    dirEntries.setSize(nEntries);
    
    return dirEntries;
}


// Copy, recursively if necessary, the source top the destination
bool cp(const fileName& src, const fileName& dest)
{
    // Make sure source exists.
    if (!exists(src))
    {
        return false;
    }

    fileName destFile(dest);

    // Check type of source file.
    if (src.type() == fileName::FILE)
    {
        // If dest is a directory, create the destination file name.
        if (destFile.type() == fileName::DIRECTORY)
        {
            destFile = destFile/src.name();
        }

        // Make sure the destination directory exists.
        if (!isDir(destFile.path()) && !mkDir(destFile.path()))
        {
            return false;
        }

        // Open and check streams.
        // Use binary mode in case we read binary.
        // Causes Windows reading to fail if we don't.
        std::ifstream srcStream(src.c_str(), 
                                ios_base::in|ios_base::binary);
        if (!srcStream) 
        {
            return false;
        }

        // Use binary mode in case we write binary.
        // Causes Windows reading to fail if we don't.
        std::ofstream destStream(destFile.c_str(), 
                                 ios_base::out|ios_base::binary);
        if (!destStream)
        {
            return false;
        }

        // Copy character data.
        char ch;
        while (srcStream.get(ch))
        {
            destStream.put(ch);
        }

        // Final check.
        if (!srcStream.eof() || !destStream)
        {
            return false;
        }
    }
    else if (src.type() == fileName::DIRECTORY)
    {
        // If dest is a directory, create the destination file name.
        if (destFile.type() == fileName::DIRECTORY)
        {
            destFile = destFile/src.component(src.components().size() -1);
        }

        // Make sure the destination directory extists.
        if (!isDir(destFile) && !mkDir(destFile))
        {
            return false;
        }

        // Copy files
        fileNameList contents = readDir(src, fileName::FILE, false);
        forAll(contents, i)
        {
            if (Windows::debug)
            {
                InfoInFunction
                    << "Copying : " << src/contents[i] 
                    << " to " << destFile/contents[i] << endl;
            }

            // File to file.
            cp(src/contents[i], destFile/contents[i]);
        }

        // Copy sub directories.
        fileNameList subdirs = readDir(src, fileName::DIRECTORY);
        forAll(subdirs, i)
        {
            if (Windows::debug)
            {
                InfoInFunction
                    << "Copying : " << src/subdirs[i]
                    << " to " << destFile << endl;
            }

            // Dir to Dir.
            cp(src/subdirs[i], destFile);
        }
    }

    return true;
}


// Create a softlink. destFile should not exist. Returns true if successful.
bool ln(const fileName& src, const fileName& dest)
{
    // Seems that prior to Vista softlinking was poorly supported.
    // Vista does a better job, but requires administrator privileges.
    // Skip for now.

    if (Windows::debug)
    {
        InfoInFunction
            << "Windows does not support ln - softlinking" << endl;
    }

    return false;
}


// Rename srcFile destFile
bool mv(const fileName& srcFile, const fileName& destFile)
{
    if (Windows::debug)
    {
        Info<< "Move : " << srcFile << " to " << destFile << endl;
    }

    const fileName destName = 
      ((destFile.type() == fileName::DIRECTORY)
       && (srcFile.type() != fileName::DIRECTORY)) ?
      destFile/srcFile.name() :
      destFile;

    const bool success = 
      (0 == std::rename(srcFile.c_str(), destName.c_str()));

    return success;
}


//- Rename to a corresponding backup file
//  If the backup file already exists, attempt with "01" .. "99" index
bool mvBak(const fileName& src, const std::string& ext)
{
    if (Windows::debug)
    {
        InfoInFunction
            << "mvBak : " << src << " to extension " << ext << endl;
    }

    if (exists(src, false))
    {
        const int maxIndex = 99;
        char index[3];

        for (int n = 0; n <= maxIndex; n++)
        {
            fileName dstName(src + "." + ext);
            if (n)
            {
                sprintf(index, "%02d", n);
                dstName += index;
            }

            // avoid overwriting existing files, except for the last
            // possible index where we have no choice
            if (!exists(dstName, false) || n == maxIndex)
            {
                return (0 == std::rename(src.c_str(), dstName.c_str()));
            }

        }
    }

    // fall-through: nothing to do
    return false;
}


// Remove a file returning true if successful otherwise false
bool rm(const fileName& file)
{
    if (Windows::debug)
    {
        InfoInFunction
            << "Removing : " << file << endl;
    }

    bool success = (0 == std::remove(file.c_str()));

    // If deleting plain file name failed try with .gz
    if (!success) 
    {
        const std::string fileGz = file + ".gz";
        success = (0 == std::remove(fileGz.c_str()));
    }

    return success;
}


// Remove a dirctory and it's contents
bool rmDir(const fileName& directory)
{
    if (Windows::debug)
    {
        InfoInFunction
            << "rmdir(const fileName&) : "
            << "removing directory " << directory << endl;
    }

    bool success = true;

    // Need to destroy DirectorIterator prior to
    // removing directory otherwise fails on Windows XP
    {
      Windows::DirectoryIterator dirIt(directory);

      while (success && dirIt.hasNext())
      {
          const fileName & fName = dirIt.next(); 

          if (fName != "." && fName != "..")
          {
              fileName path = directory/fName;

              if (path.type() == fileName::DIRECTORY)
              {
                  success = rmDir(path);

                  if (!success)
                  {
                      WarningInFunction
                        << "failed to remove directory " << fName
                        << " while removing directory " << directory
                        << endl;
                  }
              }
              else
              {
                  success = rm(path);

                  if (!success)
                  {
                      WarningInFunction
                        << "failed to remove file " << fName
                        << " while removing directory " << directory
                        << endl;
                  }
              }
          }
      }
    }
        
    if (success) 
    {
        success = ::RemoveDirectory(directory.c_str());

        if (!success) 
        {
            WarningInFunction
                << "failed to remove directory " << directory << endl;
        }
    }

    return success;
}


//- Sleep for the specified number of seconds
unsigned int sleep(const unsigned int s)
{
    const DWORD milliseconds = s * 1000;

    ::Sleep(milliseconds);

    return 0;
}


void fdClose(const int fd)
{
    const int result = ::_close(fd);

    if (0 != result)
    {
        FatalErrorInFunction
            << "close error on " << fd << endl
            << abort(FatalError);
    }
}


//- Check if machine is up by pinging given port
bool ping
(
    const word& destName,
    const label destPort,
    const label timeOut
)
{
    // Appears that socket calls require administrator privileges.
    // Skip for now.

    if (Windows::debug)
    {
        InfoInFunction
            << "Windows does not support ping" << endl;
    }

    return false;
}


//- Check if machine is up by ping port 22 = ssh and 222 = rsh
bool ping(const word& hostname, const label timeOut)
{
    return ping(hostname, 222, timeOut) || ping(hostname, 22, timeOut);
}


int system(const std::string& command)
{
    return std::system(command.c_str());
}


// Explicitly track loaded libraries, rather than use
// EnumerateLoadedModules64 and have to link against Dbghelp.dll
// Details at http://msdn.microsoft.com/en-us/library/ms679316(v=vs.85).aspx
typedef std::map<void*, std::string> OfLoadedLibs;

static
OfLoadedLibs &
getLoadedLibs()
{
  static OfLoadedLibs loadedLibs;
  return loadedLibs;
}


//- Open shared library
void* dlOpen(const fileName& libName, const bool check)
{
    if (Windows::debug)
    {
        InfoInFunction
            << " : LoadLibrary of " << libName << endl;
    }

    const char* dllExt = ".dll";

    // Assume libName is of the form, lib<name>.so
    string winLibName(libName);
    winLibName.replace(".so", dllExt);
    void* handle = ::LoadLibrary(winLibName.c_str());

    if (nullptr == handle)
    {
        // Assumes libName = name
        winLibName = "lib";
        winLibName += libName;
        winLibName += dllExt;
      
        handle = ::LoadLibrary(winLibName.c_str());
    }

    if (nullptr != handle) 
    {
        getLoadedLibs()[handle] = libName;
    }
    else if (check)
    {
        WarningInFunction
            << "dlopen error : " << Windows::getLastError()
            << endl;
    }

    if (Windows::debug)
    {
        Info<< "dlOpen(const fileName&)"
            << " : LoadLibrary of " << libName
            << " handle " << handle << endl;
    }

    return handle;
}


//- Close shared library
bool dlClose(void* const handle)
{
    if (Windows::debug)
    {
        InfoInFunction
            << "FreeLibrary of handle " << handle << endl;
    }

    const bool success = 
      ::FreeLibrary(static_cast<HMODULE>(handle));
  
    if (success)
    {
	getLoadedLibs().erase(handle);
    }
    
    return success;
}


void* dlSym(void* handle, const std::string& symbol)
{
    if (Windows::debug)
    {
        InfoInFunction
            << "GetProcAddress of " << symbol << endl;
    }

    // get address of symbol
    void* fun = (void *)
    (
        ::GetProcAddress(static_cast<HMODULE>(handle), symbol.c_str())
    );

    if(fun == nullptr)
    {
        WarningInFunction
            << "Cannot lookup symbol " << symbol << " : "
            << Windows::getLastError()
            << endl;
    }

    return fun;
}


bool dlSymFound(void* handle, const std::string& symbol)
{
    if (handle && !symbol.empty())
    {
        if (Windows::debug)
        {
            InfoInFunction
                << "GetProcAddress of " << symbol << endl;
        }

       // get address of symbol
	void* fun = (void*) ::GetProcAddress(static_cast<HMODULE>(handle), symbol.c_str());

	return (nullptr != fun);
    }
    else
    {
        return false;
    }
}


fileNameList dlLoaded()
{
    fileNameList libs;
    OfLoadedLibs & loadedLibs = getLoadedLibs();

    for (OfLoadedLibs::const_iterator it = loadedLibs.begin();
         it != loadedLibs.end();
         ++it)
    {
        libs.append(it->second);
    }

    if (Windows::debug)
    {
        InfoInFunction
            << "determined loaded libraries :" << libs.size() << endl;
    }
    return libs;
}


std::string toUnixPath(const std::string & path)
{
    string unixPath(path);
    Windows::toUnixSlash(unixPath);
    Windows::removeQuotes(unixPath);

    return unixPath;
}

} // namespace CML
// ************************************************************************* //
