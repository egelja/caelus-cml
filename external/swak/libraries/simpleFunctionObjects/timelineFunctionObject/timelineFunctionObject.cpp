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

Contributors/Copyright:
    2008-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "timelineFunctionObject.hpp"
#include "addToRunTimeSelectionTable.hpp"

#include "polyMesh.hpp"
#include "IOmanip.hpp"
#include "Time.hpp"

#include "OFstream.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(timelineFunctionObject, 0);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<>
const char* NamedEnum<CML::timelineFunctionObject::outputFileMode,3>::names[]=
{
    "caelus",
    "raw",
    "csv"
};
const NamedEnum<timelineFunctionObject::outputFileMode,3> timelineFunctionObject::outputFileModeNames_;

timelineFunctionObject::timelineFunctionObject
(
    const word &name,
    const Time& t,
    const dictionary& dict
)
:
    simpleDataFunctionObject(name,t,dict),
    factor_(
        dict.lookupOrDefault<scalar>("factor",1)
    ),
    outputFileMode_(
        outputFileModeNames_[dict.lookupOrDefault<word>("outputFileMode","caelus")]
    )
{
    switch(outputFileMode_) {
        case ofmCaelus:
            fileExtension_="";
            separator_=" ";
            break;
        case ofmRaw:
            fileExtension_=".raw";
            separator_=" ";
            break;
        case ofmCsv:
            fileExtension_=".csv";
            separator_=",";
            break;
        default:
            fileExtension_=".unknownFileMode";
            separator_="?";
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void timelineFunctionObject::flush()
{
    forAllIter(HashPtrTable<OFstream>, filePtrs_, iter)
    {
        (*iter()).flush();
    }
}

void timelineFunctionObject::closeAllFiles()
{
    forAllIter(HashPtrTable<OFstream>, filePtrs_, iter)
    {
        delete filePtrs_.remove(iter);
    }
}

bool timelineFunctionObject::start()
{
    simpleDataFunctionObject::start();

    if (Pstream::master())
    {
        wordList names(fileNames());

        // Check if any fieldNames have been removed. If so close
        // the file.

        forAllIter(HashPtrTable<OFstream>, filePtrs_, iter)
        {
            if (findIndex(names, iter.key()) == -1)
            {
                // Field has been removed. Close file
                delete filePtrs_.remove(iter);
            }
        }

        // Open new files for new fields. Keep existing files.

        filePtrs_.resize(2*names.size());

        forAll(names,fileI)
        {
            const word& fldName = names[fileI];

            // Check if added field. If so open a stream for it.

            if (!filePtrs_.found(fldName))
            {
                fileName theDir=dataDir();

                OFstream* sPtr = new OFstream(theDir/fldName+fileExtension_);

                filePtrs_.insert(fldName, sPtr);

                OFstream &s=*sPtr;

                if(
                    outputFileMode_==ofmRaw
                    ||
                    outputFileMode_==ofmCaelus
                ) {
                    s << '#';
                }

                writeTime(fldName,word("Time"));
                writeData(fldName,this->columnNames());

                endData(fldName);
            }
        }
    }

    return true;
}

void timelineFunctionObject::endData(const word &name) {
    (*filePtrs_[name]) << endl;
}


} // namespace CML

// ************************************************************************* //
