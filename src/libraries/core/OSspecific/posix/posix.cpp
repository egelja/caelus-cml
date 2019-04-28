/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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
    POSIX versions of the functions declared in OSspecific.H

\*---------------------------------------------------------------------------*/

#ifdef solarisGcc
# define _SYS_VNODE_H
#endif

#include "OSspecific.hpp"
#include "posix.hpp"
#include "caelusVersion.hpp"
#include "fileName.hpp"
#include "fileStat.hpp"
#include "timer.hpp"
#include "IFstream.hpp"
#include "DynamicList.hpp"

#include <fstream>
#include <cstdlib>
#include <cctype>

#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <pwd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netdb.h>
#include <dlfcn.h>
#ifndef darwin
#include <link.h>
#else
#endif


#include <netinet/in.h>

#ifdef USE_RANDOM
#   include <climits>
#   if INT_MAX    != 2147483647
#       error "INT_MAX    != 2147483647"
#       error "The random number generator may not work!"
#   endif
#endif

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


namespace CML
{
    defineTypeNameAndDebug(POSIX, 0);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

pid_t CML::pid()
{
    return ::getpid();
}


pid_t CML::ppid()
{
    return ::getppid();
}


pid_t CML::pgid()
{
    return ::getpgrp();
}


bool CML::env(const word& envName)
{
    return ::getenv(envName.c_str()) != nullptr;
}


CML::string CML::getEnv(const word& envName)
{
    char* env = ::getenv(envName.c_str());

    if (env)
    {
        return string(env);
    }
    else
    {
        // Return null-constructed string rather than string::null
        // to avoid cyclic dependencies in the construction of globals
        return string();
    }
}


bool CML::setEnv
(
    const word& envName,
    const std::string& value,
    const bool overwrite
)
{
    return setenv(envName.c_str(), value.c_str(), overwrite) == 0;
}


CML::string CML::hostName(bool full)
{
    char buf[128];
    ::gethostname(buf, sizeof(buf));

    // implementation as per hostname from net-tools
    if (full)
    {
        struct hostent *hp = ::gethostbyname(buf);
        if (hp)
        {
            return hp->h_name;
        }
    }

    return buf;
}


CML::string CML::domainName()
{
    char buf[128];
    ::gethostname(buf, sizeof(buf));

    // implementation as per hostname from net-tools
    struct hostent *hp = ::gethostbyname(buf);
    if (hp)
    {
        char *p = ::strchr(hp->h_name, '.');
        if (p)
        {
            ++p;
            return p;
        }
    }

    return string::null;
}


CML::string CML::userName()
{
    struct passwd* pw = ::getpwuid(::getuid());

    if (pw != nullptr)
    {
        return pw->pw_name;
    }
    else
    {
        return string::null;
    }
}


bool CML::isAdministrator()
{
    return (::geteuid() == 0);
}


// use $HOME environment variable or passwd info
CML::fileName CML::home()
{
    char* env = ::getenv("HOME");

    if (env != nullptr)
    {
        return fileName(env);
    }
    else
    {
        struct passwd* pw = ::getpwuid(getuid());

        if (pw != nullptr)
        {
            return pw->pw_dir;
        }
        else
        {
            return fileName::null;
        }
    }
}


CML::fileName CML::home(const string& userName)
{
    struct passwd* pw;

    if (userName.size())
    {
        pw = ::getpwnam(userName.c_str());
    }
    else
    {
        char* env = ::getenv("HOME");

        if (env != nullptr)
        {
            return fileName(env);
        }

        pw = ::getpwuid(::getuid());
    }

    if (pw != nullptr)
    {
        return pw->pw_dir;
    }
    else
    {
        return fileName::null;
    }
}


CML::fileName CML::cwd()
{
    char buf[256];
    if (::getcwd(buf, sizeof(buf)))
    {
        return buf;
    }
    else
    {
        FatalErrorInFunction
            << "Couldn't get the current working directory"
            << exit(FatalError);

        return fileName::null;
    }
}


bool CML::chDir(const fileName& dir)
{
    return ::chdir(dir.c_str()) == 0;
}


CML::fileNameList CML::findEtcFiles
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


CML::fileName CML::findEtcFile(const fileName& name, bool mandatory)
{
    fileNameList results(findEtcFiles(name, mandatory, true));

    if (results.size())
    {
        return results[0];
    }
    else
    {
        return fileName();
    }
}


bool CML::mkDir(const fileName& pathName, mode_t mode)
{
    // empty names are meaningless
    if (pathName.empty())
    {
        return false;
    }

    // Construct instance path directory if does not exist
    if (::mkdir(pathName.c_str(), mode) == 0)
    {
        // Directory made OK so return true
        return true;
    }
    else
    {
        switch (errno)
        {
            case EPERM:
            {
                FatalErrorInFunction
                    << "The filesystem containing " << pathName
                    << " does not support the creation of directories."
                    << exit(FatalError);

                return false;
            }

            case EEXIST:
            {
                // Directory already exists so simply return true
                return true;
            }

            case EFAULT:
            {
                FatalErrorInFunction
                    << "" << pathName
                    << " points outside your accessible address space."
                    << exit(FatalError);

                return false;
            }

            case EACCES:
            {
                FatalErrorInFunction
                    << "The parent directory does not allow write "
                       "permission to the process,"<< nl
                    << "or one of the directories in " << pathName
                    << " did not allow search (execute) permission."
                    << exit(FatalError);

                return false;
            }

            case ENAMETOOLONG:
            {
                FatalErrorInFunction
                    << "" << pathName << " is too long."
                    << exit(FatalError);

                return false;
            }

            case ENOENT:
            {
                // Part of the path does not exist so try to create it
                if (pathName.path().size() && mkDir(pathName.path(), mode))
                {
                    return mkDir(pathName, mode);
                }
                else
                {
                    FatalErrorInFunction
                        << "Couldn't create directory " << pathName
                        << exit(FatalError);

                    return false;
                }
            }

            case ENOTDIR:
            {
                FatalErrorInFunction
                    << "A component used as a directory in " << pathName
                    << " is not, in fact, a directory."
                    << exit(FatalError);

                return false;
            }

            case ENOMEM:
            {
                FatalErrorInFunction
                    << "Insufficient kernel memory was available to make "
                       "directory " << pathName << '.'
                    << exit(FatalError);

                return false;
            }

            case EROFS:
            {
                FatalErrorInFunction
                    << "" << pathName
                    << " refers to a file on a read-only filesystem."
                    << exit(FatalError);

                return false;
            }

            case ELOOP:
            {
                FatalErrorInFunction
                    << "Too many symbolic links were encountered in resolving "
                    << pathName << '.'
                    << exit(FatalError);

                return false;
            }

            case ENOSPC:
            {
                FatalErrorInFunction
                    << "The device containing " << pathName
                    << " has no room for the new directory or "
                    << "the user's disk quota is exhausted."
                    << exit(FatalError);

                return false;
            }

            default:
            {
                FatalErrorInFunction
                    << "Couldn't create directory " << pathName
                    << exit(FatalError);

                return false;
            }
        }
    }
}


// Set the file mode
bool CML::chMod(const fileName& name, const mode_t m)
{
    return ::chmod(name.c_str(), m) == 0;
}


// Return the file mode
mode_t CML::mode(const fileName& name)
{
    fileStat fileStatus(name);
    if (fileStatus.isValid())
    {
        return fileStatus.status().st_mode;
    }
    else
    {
        return 0;
    }
}


// Return the file type: FILE or DIRECTORY
CML::fileName::Type CML::type(const fileName& name)
{
    mode_t m = mode(name);

    if (S_ISREG(m))
    {
        return fileName::FILE;
    }
    else if (S_ISDIR(m))
    {
        return fileName::DIRECTORY;
    }
    else
    {
        return fileName::UNDEFINED;
    }
}


// Does the name exist in the filing system?
bool CML::exists(const fileName& name, const bool checkGzip)
{
    return mode(name) || isFile(name, checkGzip);
}


// Does the directory exist?
bool CML::isDir(const fileName& name)
{
    return S_ISDIR(mode(name));
}


// Does the file exist?
bool CML::isFile(const fileName& name, const bool checkGzip)
{
    return S_ISREG(mode(name)) || (checkGzip && S_ISREG(mode(name + ".gz")));
}


// Return size of file
off_t CML::fileSize(const fileName& name)
{
    fileStat fileStatus(name);
    if (fileStatus.isValid())
    {
        return fileStatus.status().st_size;
    }
    else
    {
        return -1;
    }
}


// Return time of last file modification
time_t CML::lastModified(const fileName& name)
{
    fileStat fileStatus(name);
    if (fileStatus.isValid())
    {
        return fileStatus.status().st_mtime;
    }
    else
    {
        return 0;
    }
}


// Read a directory and return the entries as a string list
CML::fileNameList CML::readDir
(
    const fileName& directory,
    const fileName::Type type,
    const bool filtergz
)
{
    // Initial filename list size
    // also used as increment if initial size found to be insufficient
    static const int maxNnames = 100;

    if (POSIX::debug)
    {
        Info<< "readDir(const fileName&, const fileType, const bool filtergz)"
            << " : reading directory " << directory << endl;
    }

    // Setup empty string list MAXTVALUES long
    fileNameList dirEntries(maxNnames);

    // Pointers to the directory entries
    DIR *source;
    struct dirent *list;

    // Temporary variables and counters
    label nEntries = 0;

    // Attempt to open directory and set the structure pointer
    if ((source = ::opendir(directory.c_str())) == nullptr)
    {
        dirEntries.setSize(0);

        if (POSIX::debug)
        {
            Info<< "readDir(const fileName&, const fileType, "
                   "const bool filtergz) : cannot open directory "
                << directory << endl;
        }
    }
    else
    {
        // Read and parse all the entries in the directory
        while ((list = ::readdir(source)) != nullptr)
        {
            fileName fName(list->d_name);

            // ignore files beginning with ., i.e. '.', '..' and '.*'
            if (fName.size() && fName[0] != '.')
            {
                word fExt = fName.ext();

                if
                (
                    (type == fileName::DIRECTORY)
                 ||
                    (
                        type == fileName::FILE
                     && fName[fName.size()-1] != '~'
                     && fExt != "bak"
                     && fExt != "BAK"
                     && fExt != "old"
                     && fExt != "save"
                    )
                )
                {
                    if ((directory/fName).type() == type)
                    {
                        if (nEntries >= dirEntries.size())
                        {
                            dirEntries.setSize(dirEntries.size() + maxNnames);
                        }

                        if (filtergz && fExt == "gz")
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

        // Reset the length of the entries list
        dirEntries.setSize(nEntries);

        ::closedir(source);
    }

    return dirEntries;
}


// Copy, recursively if necessary, the source to the destination
bool CML::cp(const fileName& src, const fileName& dest)
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
        std::ifstream srcStream(src.c_str());
        if (!srcStream)
        {
            return false;
        }

        std::ofstream destStream(destFile.c_str());
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

        // Make sure the destination directory exists.
        if (!isDir(destFile) && !mkDir(destFile))
        {
            return false;
        }

        // Copy files
        fileNameList contents = readDir(src, fileName::FILE, false);
        forAll(contents, i)
        {
            if (POSIX::debug)
            {
                Info<< "Copying : " << src/contents[i]
                    << " to " << destFile/contents[i] << endl;
            }

            // File to file.
            cp(src/contents[i], destFile/contents[i]);
        }

        // Copy sub directories.
        fileNameList subdirs = readDir(src, fileName::DIRECTORY);
        forAll(subdirs, i)
        {
            if (POSIX::debug)
            {
                Info<< "Copying : " << src/subdirs[i]
                    << " to " << destFile << endl;
            }

            // Dir to Dir.
            cp(src/subdirs[i], destFile);
        }
    }

    return true;
}


// Create a softlink. dst should not exist. Returns true if successful.
bool CML::ln(const fileName& src, const fileName& dst)
{
    if (POSIX::debug)
    {
        Info<< "Create softlink from : " << src << " to " << dst
            << endl;
    }

    if (exists(dst))
    {
        WarningInFunction
            << "destination " << dst << " already exists. Not linking."
            << endl;
        return false;
    }

    if (src.isAbsolute() && !exists(src))
    {
        WarningInFunction
            << "source " << src << " does not exist." << endl;
        return false;
    }

    if (::symlink(src.c_str(), dst.c_str()) == 0)
    {
        return true;
    }
    else
    {
        WarningInFunction
            << "symlink from " << src << " to " << dst << " failed." << endl;
        return false;
    }
}


// Rename srcFile dstFile
bool CML::mv(const fileName& src, const fileName& dst)
{
    if (POSIX::debug)
    {
        Info<< "Move : " << src << " to " << dst << endl;
    }

    if
    (
        dst.type() == fileName::DIRECTORY
     && src.type() != fileName::DIRECTORY
    )
    {
        const fileName dstName(dst/src.name());

        return ::rename(src.c_str(), dstName.c_str()) == 0;
    }
    else
    {
        return ::rename(src.c_str(), dst.c_str()) == 0;
    }
}


//- Rename to a corresponding backup file
//  If the backup file already exists, attempt with "01" .. "99" index
bool CML::mvBak(const fileName& src, const std::string& ext)
{
    if (POSIX::debug)
    {
        Info<< "mvBak : " << src << " to extension " << ext << endl;
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
                return ::rename(src.c_str(), dstName.c_str()) == 0;
            }

        }
    }

    // fall-through: nothing to do
    return false;
}


// Remove a file, returning true if successful otherwise false
bool CML::rm(const fileName& file)
{
    if (POSIX::debug)
    {
        Info<< "Removing : " << file << endl;
    }

    // Try returning plain file name; if not there, try with .gz
    if (remove(file.c_str()) == 0)
    {
        return true;
    }
    else
    {
        return ::remove(string(file + ".gz").c_str()) == 0;
    }
}


// Remove a dirctory and its contents
bool CML::rmDir(const fileName& directory)
{
    if (POSIX::debug)
    {
        Info<< "rmDir(const fileName&) : "
            << "removing directory " << directory << endl;
    }

    // Pointers to the directory entries
    DIR *source;
    struct dirent *list;

    // Attempt to open directory and set the structure pointer
    if ((source = ::opendir(directory.c_str())) == nullptr)
    {
        WarningInFunction
            << "cannot open directory " << directory << endl;

        return false;
    }
    else
    {
        // Read and parse all the entries in the directory
        while ((list = ::readdir(source)) != nullptr)
        {
            fileName fName(list->d_name);

            if (fName != "." && fName != "..")
            {
                fileName path = directory/fName;

                if (path.type() == fileName::DIRECTORY)
                {
                    if (!rmDir(path))
                    {
                        WarningInFunction
                            << "failed to remove directory " << fName
                            << " while removing directory " << directory
                            << endl;

                        ::closedir(source);

                        return false;
                    }
                }
                else
                {
                    if (!rm(path))
                    {
                        WarningInFunction
                            << "failed to remove file " << fName
                            << " while removing directory " << directory
                            << endl;

                        ::closedir(source);

                        return false;
                    }
                }
            }

        }

        if (!rm(directory))
        {
            WarningInFunction
                << "failed to remove directory " << directory << endl;

            ::closedir(source);

            return false;
        }

        ::closedir(source);

        return true;
    }
}


unsigned int CML::sleep(const unsigned int s)
{
    return ::sleep(s);
}


void CML::fdClose(const int fd)
{
    if (close(fd) != 0)
    {
        FatalErrorInFunction
            << "close error on " << fd << endl
            << abort(FatalError);
    }
}


bool CML::ping
(
    const string& destName,
    const label destPort,
    const label timeOut
)
{
    struct hostent *hostPtr;
    volatile int sockfd;
    struct sockaddr_in destAddr;      // will hold the destination addr
    u_int addr;

    if ((hostPtr = ::gethostbyname(destName.c_str())) == nullptr)
    {
        FatalErrorInFunction
            << "gethostbyname error " << h_errno << " for host " << destName
            << abort(FatalError);
    }

    // Get first of the SLL of addresses
    addr = (reinterpret_cast<struct in_addr*>(*(hostPtr->h_addr_list)))->s_addr;

    // Allocate socket
    sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        FatalErrorInFunction
            << "socket error"
            << abort(FatalError);
    }

    // Fill sockaddr_in structure with dest address and port
    memset(reinterpret_cast<char *>(&destAddr), '\0', sizeof(destAddr));
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(ushort(destPort));
    destAddr.sin_addr.s_addr = addr;


    timer myTimer(timeOut);

    if (timedOut(myTimer))
    {
        // Setjmp from timer jumps back to here
        fdClose(sockfd);
        return false;
    }

    if
    (
        ::connect
        (
            sockfd,
            reinterpret_cast<struct sockaddr*>(&destAddr),
            sizeof(struct sockaddr)
        ) != 0
    )
    {
        // Connection refused. Check if network was actually used or not.

        int connectErr = errno;

        fdClose(sockfd);

        if (connectErr == ECONNREFUSED)
        {
            return true;
        }
        //perror("connect");

        return false;
    }

    fdClose(sockfd);

    return true;
}


bool CML::ping(const string& hostname, const label timeOut)
{
    return ping(hostname, 222, timeOut) || ping(hostname, 22, timeOut);
}


int CML::system(const std::string& command)
{
    return ::system(command.c_str());
}


void* CML::dlOpen(const fileName& lib, const bool check)
{
    if (POSIX::debug)
    {
        std::cout<< "dlOpen(const fileName&)"
            << " : dlopen of " << lib << std::endl;
    }
    void* handle = ::dlopen(lib.c_str(), RTLD_LAZY|RTLD_GLOBAL);

#ifdef darwin
    if(!handle && lib.ext()=="so") {
        fileName lName=lib.lessExt()+".dylib";
        handle = 
            dlopen(lName.c_str(), RTLD_LAZY|RTLD_GLOBAL);
    }
#endif

    if (!handle && check)
    {
        WarningInFunction
            << "dlopen error : " << ::dlerror()
            << endl;
    }

    if (POSIX::debug)
    {
        std::cout
            << "dlOpen(const fileName&)"
            << " : dlopen of " << lib
            << " handle " << handle << std::endl;
    }

    return handle;
}


bool CML::dlClose(void* handle)
{
    if (POSIX::debug)
    {
        std::cout
            << "dlClose(void*)"
            << " : dlclose of handle " << handle << std::endl;
    }
    return ::dlclose(handle) == 0;
}


void* CML::dlSym(void* handle, const std::string& symbol)
{
    if (POSIX::debug)
    {
        std::cout
            << "dlSym(void*, const std::string&)"
            << " : dlsym of " << symbol << std::endl;
    }
    // clear any old errors - see manpage dlopen
    (void) ::dlerror();

    // get address of symbol
    void* fun = ::dlsym(handle, symbol.c_str());

    // find error (if any)
    char *error = ::dlerror();

    if (error)
    {
        WarningInFunction
            << "Cannot lookup symbol " << symbol << " : " << error
            << endl;
    }

    return fun;
}


bool CML::dlSymFound(void* handle, const std::string& symbol)
{
    if (handle && !symbol.empty())
    {
        if (POSIX::debug)
        {
            std::cout
                << "dlSymFound(void*, const std::string&)"
                << " : dlsym of " << symbol << std::endl;
        }

        // clear any old errors - see manpage dlopen
        (void) ::dlerror();

        // get address of symbol
        (void) ::dlsym(handle, symbol.c_str());

        // symbol can be found if there was no error
        return !::dlerror();
    }
    else
    {
        return false;
    }
}


static int collectLibsCallback
(
    struct dl_phdr_info *info,
    size_t size,
    void *data
)
{
#ifdef darwin
    WarningInFunction
        << "Not yet implemented for Mac OS X"
            << CML::endl;
#else
    CML::DynamicList<CML::fileName>* ptr =
        reinterpret_cast<CML::DynamicList<CML::fileName>*>(data);
    ptr->append(info->dlpi_name);
#endif
    return 0;
}


CML::fileNameList CML::dlLoaded()
{
    DynamicList<fileName> libs;
#ifdef darwin
    WarningInFunction
        << "Not yet implemented for Mac OS X"
            << endl;
#else
    dl_iterate_phdr(collectLibsCallback, &libs);
#endif
    if (POSIX::debug)
    {
        std::cout
            << "dlLoaded()"
            << " : determined loaded libraries :" << libs.size() << std::endl;
    }
    return libs;
}

std::string CML::toUnixPath(const std::string & path)
{
  return path;
}

// ************************************************************************* //
