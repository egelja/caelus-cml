/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
Copyright (C) 2011 OpenFOAM Foundation
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

Class
    CML::messageStream

Description
    Class to handle messaging in a simple, consistent stream-based
    manner.

    The messageStream class is globaly instantiated with a title string a
    given severity, which controls the program termination, and a number of
    errors before termination.  Errors, messages and other data are piped to
    the messageStream class in the standard manner.

Usage
    \code
        messageStream
            << "message1" << "message2" << CMLDataType << endl;
    \endcode


\*---------------------------------------------------------------------------*/

#ifndef messageStream_H
#define messageStream_H

#include "label.hpp"
#include "string.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class IOstream;
class Ostream;
class OSstream;
class OStringStream;
class dictionary;

/*---------------------------------------------------------------------------*\
                           Class messageStream Declaration
\*---------------------------------------------------------------------------*/

class messageStream
{

public:

    //- Severity flags
    enum errorSeverity
    {
        INFO,       // Debugging information in event of error
        WARNING,    // Warning of possible problem
        SERIOUS,    // A serious problem (data corruption?)
        FATAL       // Oh bugger!
    };


protected:

    // Private data

        string title_;
        errorSeverity severity_;
        int maxErrors_;
        int errorCount_;


public:

    // Debug switches

        static int level;


    // Constructors

        //- Construct from components
        messageStream
        (
            const string& title,
            errorSeverity,
            const int maxErrors = 0
        );


        //- Construct from dictionary
        messageStream(const dictionary&);


    // Member functions

        //- Return the title of this error type
        const string& title() const
        {
            return title_;
        }

        //- Return the maximum number of errors before program termination
        int maxErrors() const
        {
            return maxErrors_;
        }

        //- Return non-const access to the maximum number of errors before
        //  program termination to enable user to reset it
        int& maxErrors()
        {
            return maxErrors_;
        }

        //- Convert to OSstream
        //  Prints basic message and returns OSstream for further info.
        OSstream& operator()
        (
            const char* functionName,
            const char* sourceFileName,
            const int sourceFileLineNumber = 0
        );

        //- Convert to OSstream
        //  Prints basic message and returns OSstream for further info.
        OSstream& operator()
        (
            const string& functionName,
            const char* sourceFileName,
            const int sourceFileLineNumber = 0
        );

        //- Convert to OSstream
        //  Prints basic message and returns OSstream for further info.
        OSstream& operator()
        (
            const char* functionName,
            const char* sourceFileName,
            const int sourceFileLineNumber,
            const string& ioFileName,
            const label ioStartLineNumber = -1,
            const label ioEndLineNumber = -1
        );

        //- Convert to OSstream
        //  Prints basic message and returns OSstream for further info.
        OSstream& operator()
        (
            const char* functionName,
            const char* sourceFileName,
            const int sourceFileLineNumber,
            const IOstream&
        );

        //- Convert to OSstream
        //  Prints basic message and returns OSstream for further info.
        OSstream& operator()
        (
            const char* functionName,
            const char* sourceFileName,
            const int sourceFileLineNumber,
            const dictionary&
        );

        //- Convert to OSstream
        //  Use Info for default communicator, control output using bool flag
        OSstream& operator()(const bool output);

        //- Convert to OSstream for << operations
        operator OSstream&();

        //- Explicitly convert to OSstream for << operations
        OSstream& operator()()
        {
            return operator OSstream&();
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// Global error declarations: defined in messageStream.C

extern messageStream SeriousError;
extern messageStream Warning;
extern messageStream Info;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "OSstream.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// Convenience macros to add the file name and line number to the function name

// Compiler provided function name string:
//    for gcc-compatible compilers use __PRETTY_FUNCTION__
//    otherwise use the standard __func__
#ifdef __GNUC__
    #define FUNCTION_NAME __PRETTY_FUNCTION__
#else
    #define FUNCTION_NAME __func__
#endif


/**
 * \def SeriousErrorIn(functionName)
 * Report an error message using CML::SeriousError for functionName in
 * file __FILE__ at line __LINE__
*/
#define SeriousErrorIn(fn) \
    ::CML::SeriousError((fn), __FILE__, __LINE__)

//- Report an error message using CML::SeriousError
//  for FUNCTION_NAME in file __FILE__ at line __LINE__
#define SeriousErrorInFunction SeriousErrorIn(FUNCTION_NAME)


/**
 * \def SeriousIOErrorIn(functionName, ios)
 * Report an IO error message using CML::SeriousError for functionName in
 * file __FILE__ at line __LINE__
 * for a particular IOstream
*/
#define SeriousIOErrorIn(fn, ios) \
    ::CML::SeriousError((fn), __FILE__, __LINE__, ios)

//- Report an IO error message using CML::SeriousError
//  for FUNCTION_NAME in file __FILE__ at line __LINE__
//  for a particular IOstream
#define SeriousIOErrorInFunction(ios) SeriousIOErrorIn(FUNCTION_NAME, ios)


/**
 * \def WarningIn(functionName)
 * Report a warning using CML::Warning for functionName in
 * file __FILE__ at line __LINE__
*/
#define WarningIn(fn) \
    ::CML::Warning((fn), __FILE__, __LINE__)

//- Report a warning using CML::Warning
//  for FUNCTION_NAME in file __FILE__ at line __LINE__
#define WarningInFunction WarningIn(FUNCTION_NAME)


/**
 * \def IOWarningIn(functionName, ios)
 * Report an IO warning using CML::Warning for functionName in
 * file __FILE__ at line __LINE__
 * for a particular IOstream
*/
#define IOWarningIn(fn, ios) \
    ::CML::Warning((fn), __FILE__, __LINE__, (ios))

//- Report an IO warning using CML::Warning
//  for FUNCTION_NAME in file __FILE__ at line __LINE__
//  for a particular IOstream
#define IOWarningInFunction(ios) IOWarningIn(FUNCTION_NAME, ios)


/**
 * \def InfoIn(functionName)
 * Report a information message using CML::Info for functionName in
 * file __FILE__ at line __LINE__
*/
#define InfoIn(fn) \
    ::CML::Info((fn), __FILE__, __LINE__)

//- Report an information message using CML::Info
//  for FUNCTION_NAME in file __FILE__ at line __LINE__
#define InfoInFunction InfoIn(FUNCTION_NAME)


/**
 * \def IOInfoIn(functionName, ios)
 * Report an IO information message using CML::Info for functionName in
 * file __FILE__ at line __LINE__
 * for a particular IOstream
*/
#define IOInfoIn(fn, ios) \
    ::CML::Info((fn), __FILE__, __LINE__, (ios))

//- Report an IO information message using CML::Info
//  for FUNCTION_NAME in file __FILE__ at line __LINE__
//  for a particular IOstream
#define IOInfoInFunction(ios) IOInfoIn(FUNCTION_NAME, ios)


//- Report an information message using CML::Info
//  if the local debug switch is true
#define DebugInfo                                                              \
    if (debug) Info

//- Report an information message using CML::Info
//  for FUNCTION_NAME in file __FILE__ at line __LINE__
//  if the local debug switch is true
#define DebugIn(functionName)                                                  \
    if (debug) InfoIn(functionName)

//- Report an information message using CML::Info
//  for FUNCTION_NAME in file __FILE__ at line __LINE__
//  if the local debug switch is true
#define DebugInFunction                                                        \
    if (debug) InfoInFunction

//- Report a variable name and value
//  using CML::Pout in file __FILE__ at line __LINE__
#define DebugVar(var)                                                          \
{                                                                              \
    ::CML::string oldPrefix(::CML::Pout.prefix());                             \
    ::CML::Pout<< "["<< __FILE__ << ":" << __LINE__ << "] ";                   \
    ::CML::Pout.prefix() = oldPrefix + #var " ";                               \
    ::CML::Pout<< var << ::CML::endl;                                          \
    ::CML::Pout.prefix() = oldPrefix;                                          \
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
