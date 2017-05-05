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

Contributors/Copyright:
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

namespace CML {


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class T>
void StackExpressionResult::pushInternal(ExpressionResult &atEnd)
{
    Dbug << "pushInternal(ExpressionResult &atEnd) "
            << pTraits<T>::typeName << endl;

    Field<T> oldValue(getResult<T>(false));
    autoPtr<Field<T> > pNewValue(new Field<T>(oldValue.size()+1));
    Field<T> &newValue=pNewValue();

    forAll(oldValue,i) {
        newValue[i]=oldValue[i];
    }
    newValue[oldValue.size()]=atEnd.getResult<T>(true)()[0];

    Dbug << "New value:" << newValue << endl;

    this->setResult(
        pNewValue.ptr(),
        this->isPoint()
    );
}

template<class T>
ExpressionResult StackExpressionResult::popInternal()
{
    Field<T> oldValue(getResult<T>(false));
    autoPtr<Field<T> > pNewValue(new Field<T>(oldValue.size()-1));
    Field<T> &newValue=pNewValue();

    forAll(newValue,i) {
        newValue[i]=oldValue[i];
    }
    Field<T> result(1,oldValue[newValue.size()]);

    this->setResult(
        pNewValue.ptr(),
        this->isPoint()
    );

    return ExpressionResult(result);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

}

// ************************************************************************* //
