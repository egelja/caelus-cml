/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright (C) 1991-2008 OpenCFD Ltd.
-------------------------------------------------------------------------------
License
    This file is based on CAELUS.

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
    CML::DelayedExpressionResult

Description

SourceFiles
    DelayedExpressionResultI.hpp
    DelayedExpressionResult.cpp
    DelayedExpressionResultIO.cpp

Contributors/Copyright:
    2012-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef DelayedExpressionResult_H
#define DelayedExpressionResult_H

#include "ExpressionResult.hpp"
#include "Tuple2.hpp"

#include "exprString.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class DelayedExpressionResult Declaration
\*---------------------------------------------------------------------------*/

class DelayedExpressionResult
:
    public ExpressionResult
{
    // Private data

    word name_;

    exprString startupValueExpression_;

    //- the result that is going to be set
    ExpressionResult settingResult_;

    typedef Tuple2<scalar,ExpressionResult> ValueAtTime;

    //- the old results
    DLList<ValueAtTime> storedValues_;

    //- how often should the values be stored
    scalar storeInterval_;

    //- how big is the delay
    scalar delay_;

protected:

    inline ExpressionResult &toBeSet();

public:

     //- Runtime type information
    TypeName("DelayedExpressionResult");

   // Constructors

    //- Construct null
    DelayedExpressionResult();

    //- Construct as copy
    DelayedExpressionResult(const DelayedExpressionResult&);

    //- Construct from a dictionary
    DelayedExpressionResult(const dictionary&);

    // Destructor

    virtual ~DelayedExpressionResult();

    void operator=(const DelayedExpressionResult&);

    void operator=(const ExpressionResult&);

    virtual autoPtr<ExpressionResult> clone() const {
        return autoPtr<ExpressionResult>(
            new DelayedExpressionResult(*this)
        );
    }

    const word name() const { return name_; }

    const exprString &startupValueExpression() const
        {
            return startupValueExpression_;
        }

    //- update the read-value

    //- returns true if there was a valid value.
    //- this does not do the work of setReadValue because we have no
    //- access to the Parser
    bool updateReadValue(const scalar &time);

    //- setting the readValue with a calculated value
    void setReadValue(const ExpressionResult &val);

    //- add a stored value
    void storeValue(const scalar &time);

    friend Ostream & operator<<(Ostream &,const DelayedExpressionResult &);

    friend Istream & operator>>(Istream &,DelayedExpressionResult &);

    friend bool operator!=(
        const DelayedExpressionResult &,const DelayedExpressionResult &
    );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "DelayedExpressionResultI.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
