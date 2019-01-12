/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright  held by original author
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
    timelineFunctionObject

Description
    Basis for function objects that generate timelines

SourceFiles
    timelineFunctionObject.cpp

Contributors/Copyright:
    2008-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef timelineFunctionObject_H
#define timelineFunctionObject_H

#include "simpleDataFunctionObject.hpp"
#include "NamedEnum.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class timelineFunctionObject Declaration
\*---------------------------------------------------------------------------*/

class timelineFunctionObject
:
    public simpleDataFunctionObject
{
    // Private Member Functions

    //- Disallow default bitwise copy construct
    timelineFunctionObject(const timelineFunctionObject&);

    //- Disallow default bitwise assignment
    void operator=(const timelineFunctionObject&);

    //- Factor to multiply results with (to account for symmetries etc)
    scalar factor_;

public:
    enum outputFileMode {
        ofmCaelus,
        ofmRaw,
        ofmCsv
    };

private:

    //- Current open files
    HashPtrTable<OFstream> filePtrs_;

protected:

    //- Name of the sub-dictionary of the case
    virtual word dirName()=0;

    //- Names of the files
    virtual wordList fileNames()=0;

    //- Text in the first line
    virtual stringList columnNames()=0;

    //- Factor to multiply results with (for symmetries etc)
    scalar factor() const { return factor_; }

    //- possible output-modes
    static const NamedEnum<outputFileMode,3> outputFileModeNames_;
    outputFileMode outputFileMode_;

    //- the file extension that fits the file-mode
    word fileExtension_;

    //- character that separates fields
    std::string separator_;

    //- write similar data items
    template<class T>
    void writeData(const word &name,const UList<T> &data);

    //- write the time
    template<class T>
    void writeTime(const word &name,const T &time);

    //- ends a dataset (usually by writing an endline)
    void endData(const word &name);

    //- close all files to enable a new slate
    void closeAllFiles();

public:

    //- Runtime type information
    TypeName("timelineFunctionObject");


    // Constructors

    //- Construct from components
    timelineFunctionObject
    (
        const word &name,
        const Time&,
        const dictionary&
    );


    // Member Functions

    //- start is called at the start of the time-loop
    virtual bool start();

    virtual void flush();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "IOmanip.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML {
/*---------------------------------------------------------------------------*\
                           Class timelineFunctionObject Declaration
\*---------------------------------------------------------------------------*/

template<>
inline void timelineFunctionObject::writeData(const word &name,const UList<string> &data) {
    Ostream &s=*filePtrs_[name];
    forAll(data,index) {
        string d(data[index]);
        d.replaceAll(" ","_");
        switch(outputFileMode_) {
            case ofmCaelus:
            case ofmRaw:
                s << setw
                    (
                        max(
                            label(IOstream::defaultPrecision() + 7),
                            label(d.size()+1)   // at least one space
                        )
                    ) << d.c_str();
                break;
            case ofmCsv:
                s << separator_.c_str() << d.c_str();
                break;
            default:
                FatalErrorInFunction
                    << "Don't know how to write files of type "
                        << outputFileModeNames_[outputFileMode_] << endl
                        << exit(FatalError);
        }
    }
}

template<>
inline void timelineFunctionObject::writeData(const word &name,const UList<scalar> &data) {
    Ostream &s=*filePtrs_[name];
    forAll(data,index) {
        switch(outputFileMode_) {
            case ofmCaelus:
            case ofmRaw:
                s << setw(IOstream::defaultPrecision() + 7) << data[index];
                break;
            case ofmCsv:
                s << separator_.c_str() << data[index];
                break;
            default:
                FatalErrorInFunction
                    << "Don't know how to write files of type "
                        << outputFileModeNames_[outputFileMode_] << endl
                        << exit(FatalError);
        }
    }
}

template<class T>
void timelineFunctionObject::writeData(const word &name,const UList<T> &data) {
    Ostream &s=*filePtrs_[name];
    forAll(data,index) {
        if(outputFileMode_==ofmCaelus) {
            s << setw(IOstream::defaultPrecision() + 7) << data[index];
        } else {
            for(label i=0;i<pTraits<T>::nComponents;i++) {
                switch(outputFileMode_) {
                    case ofmRaw:
                        s << setw(IOstream::defaultPrecision() + 7) << data[index][i];
                        break;
                    case ofmCsv:
                        s << separator_.c_str() << data[index][i];
                        break;
                    default:
                        FatalErrorInFunction
                            << "Don't know how to write files of type "
                                << outputFileModeNames_[outputFileMode_] << endl
                                << exit(FatalError);
                }
            }
        }
    }
}

template<class T>
void timelineFunctionObject::writeTime(const word &name,const T &time) {
    Ostream &s=*filePtrs_[name];
    switch(outputFileMode_) {
        case ofmCaelus:
        case ofmRaw:
            s << setw(IOstream::defaultPrecision() + 7) << time;
            break;
        case ofmCsv:
            s <<  time;
            break;
        default:
            FatalErrorInFunction
                    << "Don't know how to write files of type "
                        << outputFileModeNames_[outputFileMode_] << endl
                        << exit(FatalError);
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML



#endif

// ************************************************************************* //
