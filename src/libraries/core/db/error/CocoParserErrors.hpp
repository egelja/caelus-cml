/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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
    CML::CocoParserErrors

Description
    Templated class to shadow the error handling for Coco/R parsers

\*---------------------------------------------------------------------------*/

#ifndef CocoParserErrors_H
#define CocoParserErrors_H

#include "error.hpp"
#include "wchar.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                      Class CocoParserErrors Declaration
\*---------------------------------------------------------------------------*/

template<class BaseClass, class StringClass=std::wstring>
class CocoParserErrors
:
    public BaseClass
{
    // Private data

        //- The name issued in warnings and errors
        word   name_;

public:

    // Constructors

        //- Construct with given name
        CocoParserErrors(const word& name)
        :
            BaseClass(),
            name_(name)
        {}


    //- Destructor
    virtual ~CocoParserErrors()
    {}


    // Member functions

        //- Return the name issued for warnings
        virtual const word& name() const
        {
            return name_;
        }

        //- Return the name issued for warnings
        virtual word& name()
        {
            return name_;
        }


    // Error Handling

        //- Handle a general warning 'msg'
        virtual void Warning(const StringClass& msg)
        {
            WarningInFunction
                << msg << endl;
        }

        //- Handle a general warning 'msg'
        virtual void Warning(int line, int col, const StringClass& msg)
        {
            WarningInFunction
                <<"line " << line << " col " << col << ": "
                << msg << endl;
        }

        //- Handle general error 'msg' (eg, a semantic error)
        virtual void Error(int line, int col, const StringClass& msg)
        {
            FatalErrorInFunction
                << "line " << line << " col " << col <<": " << msg << endl
                << exit(FatalError);
        }

        //- Handle general error 'msg' (eg, a semantic error)
        virtual void Error(const StringClass& msg)
        {
            FatalErrorInFunction
                << msg << endl
                << exit(FatalError);
        }

        //- Handle a general exception 'msg'
        virtual void Exception(const StringClass& msg)
        {
            this->Error(msg);
        }

};



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
