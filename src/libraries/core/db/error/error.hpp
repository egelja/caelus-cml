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
    CML::error

Description
    Class to handle errors and exceptions in a simple, consistent stream-based
    manner.

    The error class is globaly instantiated with a title string. Errors,
    messages and other data are piped to the messageStream class in the
    standard manner.  Manipulators are supplied for exit and abort which may
    terminate the program or throw an exception depending of if the exception
    handling has beed switched on (off by default).

Usage
    \code
        error << "message1" << "message2" << CMLDataType << exit(errNo);
        error << "message1" << "message2" << CMLDataType << abort();
    \endcode


\*---------------------------------------------------------------------------*/

#ifndef error_H
#define error_H

#include "messageStream.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators
class error;
Ostream& operator<<(Ostream&, const error&);


/*---------------------------------------------------------------------------*\
                            Class error Declaration
\*---------------------------------------------------------------------------*/

class error
:
    public std::exception,
    public messageStream
{

protected:

    // Protected data

        string functionName_;
        string sourceFileName_;
        label sourceFileLineNumber_;

        bool abort_;

        bool throwExceptions_;
        OStringStream* messageStreamPtr_;

public:

    // Constructors

        //- Construct from title string
        error(const string& title);

        //- Construct from dictionary
        error(const dictionary&);

        //- Construct as copy
        error(const error&);


    //- Destructor
    virtual ~error() throw();


    // Member functions

        string message() const;

        const string& functionName() const
        {
            return functionName_;
        }

        const string& sourceFileName() const
        {
            return sourceFileName_;
        }

        label sourceFileLineNumber() const
        {
            return sourceFileLineNumber_;
        }

        void throwExceptions()
        {
            throwExceptions_ = true;
        }

        void dontThrowExceptions()
        {
            throwExceptions_ = false;
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
        operator OSstream&();

        //- Explicitly convert to OSstream for << operations
        OSstream& operator()()
        {
            return operator OSstream&();
        }

        //- Create and return a dictionary
        operator dictionary() const;


        //- Helper function to print a stack
        static void printStack(Ostream&);

        //- Exit : can be called for any error to exit program.
        //  Prints stack before exiting.
        void exit(const int errNo = 1);

        //- Abort : used to stop code for fatal errors.
        //  Prints stack before exiting.
        void abort();


    // Ostream operator

        friend Ostream& operator<<(Ostream&, const error&);
};


// Forward declaration of friend functions and operators
class IOerror;
Ostream& operator<<(Ostream&, const IOerror&);


/*---------------------------------------------------------------------------*\
                           Class IOerror Declaration
\*---------------------------------------------------------------------------*/

//- Report an I/O error
class IOerror
:
    public error
{
    // Private data

        string ioFileName_;
        label ioStartLineNumber_;
        label ioEndLineNumber_;


public:

    // Constructors

        //- Construct from title string
        IOerror(const string& title);

        //- Construct from dictionary
        IOerror(const dictionary&);


    //- Destructor
    virtual ~IOerror() throw();


    // Member functions

        const string& ioFileName() const
        {
            return ioFileName_;
        }

        label ioStartLineNumber() const
        {
            return ioStartLineNumber_;
        }

        label ioEndLineNumber() const
        {
            return ioEndLineNumber_;
        }

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

        //- Print basic message and exit. Uses cerr if streams not constructed
        //  yet (at startup). Use in startup parsing instead of FatalError.
        static void SafeFatalIOError
        (
            const char* functionName,
            const char* sourceFileName,
            const int sourceFileLineNumber,
            const IOstream&,
            const string& msg
        );

        //- Create and return a dictionary
        operator dictionary() const;


        //- Exit : can be called for any error to exit program
        void exit(const int errNo = 1);

        //- Abort : used to stop code for fatal errors
        void abort();


    // Ostream operator

        friend Ostream& operator<<(Ostream&, const IOerror&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// Global error declarations: defined in error.cpp

extern error   FatalError;
extern IOerror FatalIOError;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// Convenience macros to add the file name and line number to the function name

/**
 * \def FatalErrorIn(functionName)
 * Report an error message using CML::FatalError for functionName in
 * file __FILE__ at line __LINE__
*/
#define FatalErrorIn(fn) \
    ::CML::FatalError((fn), __FILE__, __LINE__)

//- Report an error message using CML::FatalError
//  for FUNCTION_NAME in file __FILE__ at line __LINE__
#define FatalErrorInFunction FatalErrorIn(FUNCTION_NAME)


/**
 * \def FatalIOErrorIn(functionName, ios)
 * Report an error message using CML::FatalIOError for functionName in
 * file __FILE__ at line __LINE__
 * for a particular IOstream
*/
#define FatalIOErrorIn(fn, ios) \
    ::CML::FatalIOError((fn), __FILE__, __LINE__, (ios))

//- Report an error message using CML::FatalIOError
//  for FUNCTION_NAME in file __FILE__ at line __LINE__
//  for a particular IOstream
#define FatalIOErrorInFunction(ios) FatalIOErrorIn(FUNCTION_NAME, ios)


/**
 * \def SafeFatalIOErrorIn(functionName, ios, msg)
 * Report an error message using CML::FatalIOError (or cerr if FatalIOError
 * not yet constructed) for functionName in
 * file __FILE__ at line __LINE__
 * for a particular IOstream
*/
#define SafeFatalIOErrorIn(fn, ios, msg) \
    ::CML::IOerror::SafeFatalIOError((fn), __FILE__, __LINE__, (ios), (msg))

//- Report an error message using CML::FatalIOError
//  (or cerr if FatalIOError not yet constructed)
//  for functionName in file __FILE__ at line __LINE__
//  for a particular IOstream
#define SafeFatalIOErrorInFunction(ios, msg)                                   \
    SafeFatalIOErrorIn(FUNCTION_NAME, ios, msg)


/**
 * \def notImplemented(functionName)
 * Issue a FatalErrorIn for the functionName.
 * This is used for functions that are not currently implemented.
 * The functionName is printed and then abort is called.
 *
 * \note
 * This macro can be particularly useful when methods must be defined to
 * complete the interface of a derived class even if they should never be
 * called for this derived class.
*/
#define notImplemented(fn) \
    FatalErrorIn(fn) << "Not implemented" << ::CML::abort(FatalError);

//- Issue a FatalErrorIn for a function not currently implemented.
//  The FUNCTION_NAME is printed and then abort is called.
//
//  This macro can be particularly useful when methods must be defined to
//  complete the interface of a derived class even if they should never be
//  called for this derived class.
#define NotImplemented notImplemented(FUNCTION_NAME)


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "errorManip.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
