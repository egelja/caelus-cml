/*---------------------------------------------------------------------------*\
Copyright (C) 2012-2015 OpenFOAM Foundation
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

\*---------------------------------------------------------------------------*/

#include "functionObjectFile.hpp"
#include "Time.hpp"
#include "polyMesh.hpp"
#include "IFstream.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

const CML::word CML::functionObjectFile::outputPrefix = "postProcessing";
CML::label CML::functionObjectFile::addChars = 7;

// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

void CML::functionObjectFile::initStream(Ostream& os) const
{
    os.setf(ios_base::scientific, ios_base::floatfield);
    // os.precision(IOstream::defaultPrecision());
    os.width(charWidth());
}


CML::fileName CML::functionObjectFile::baseFileDir() const
{
    fileName baseDir = obr_.time().path();

    if (Pstream::parRun())
    {
        // Put in undecomposed case (Note: gives problems for
        // distributed data running)
        baseDir = baseDir/".."/outputPrefix;
    }
    else
    {
        baseDir = baseDir/outputPrefix;
    }

    // append mesh name if not default region
    if (isA<polyMesh>(obr_))
    {
        const polyMesh& mesh = refCast<const polyMesh>(obr_);
        if (mesh.name() != polyMesh::defaultRegion)
        {
            baseDir = baseDir/mesh.name();
        }
    }

    return baseDir;
}


CML::fileName CML::functionObjectFile::baseTimeDir() const
{
    return baseFileDir()/prefix_/obr_.time().timeName();
}


void CML::functionObjectFile::createFiles()
{
    if (Pstream::master())
    {
        const word startTimeName =
            obr_.time().timeName(obr_.time().startTime().value());

        label i = 0;
        forAllConstIter(wordHashSet, names_, iter)
        {
            if (!filePtrs_.set(i))
            {
                fileName outputDir(baseFileDir()/prefix_/startTimeName);

                mkDir(outputDir);

                word fName(iter.key());

                // check if file already exists
                IFstream is(outputDir/(fName + ".dat"));
                if (is.good())
                {
                    fName = fName + "_" + obr_.time().timeName();
                }

                filePtrs_.set(i, new OFstream(outputDir/(fName + ".dat")));

                initStream(filePtrs_[i]);

                writeFileHeader(i);

                i++;
            }
        }
    }
}


void CML::functionObjectFile::writeFileHeader(const label i)
{
    // do nothing
}


void CML::functionObjectFile::write()
{
    createFiles();
}


void CML::functionObjectFile::resetNames(const wordList& names)
{
    names_.clear();
    names_.insert(names);

    if (Pstream::master())
    {
        filePtrs_.clear();
        filePtrs_.setSize(names_.toc().size());

        createFiles();
    }
}


void CML::functionObjectFile::resetName(const word& name)
{
    names_.clear();
    names_.insert(name);

    if (Pstream::master())
    {
        filePtrs_.clear();
        filePtrs_.setSize(1);

        createFiles();
    }
}


CML::Omanip<int> CML::functionObjectFile::valueWidth(const label offset) const
{
    return setw(IOstream::defaultPrecision() + addChars + offset);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::functionObjectFile::functionObjectFile
(
    const objectRegistry& obr,
    const word& prefix
)
:
    obr_(obr),
    prefix_(prefix),
    names_(),
    filePtrs_()
{}


CML::functionObjectFile::functionObjectFile
(
    const objectRegistry& obr,
    const word& prefix,
    const word& name
)
:
    obr_(obr),
    prefix_(prefix),
    names_(),
    filePtrs_()
{
    names_.clear();
    names_.insert(name);

    if (Pstream::master())
    {
        filePtrs_.clear();
        filePtrs_.setSize(1);

        // cannot create files - need to access virtual function
    }
}


CML::functionObjectFile::functionObjectFile
(
    const objectRegistry& obr,
    const word& prefix,
    const wordList& names
)
:
    obr_(obr),
    prefix_(prefix),
    names_(names),
    filePtrs_()
{
    names_.clear();
    names_.insert(names);

    if (Pstream::master())
    {
        filePtrs_.clear();
        filePtrs_.setSize(names_.size());

        // cannot create files - need to access virtual function
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::functionObjectFile::~functionObjectFile()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const CML::wordHashSet& CML::functionObjectFile::names() const
{
    return names_;
}


CML::OFstream& CML::functionObjectFile::file()
{
    if (!Pstream::master())
    {
        FatalErrorInFunction
            << "Request for file() can only be done by the master process"
            << abort(FatalError);
    }

    if (filePtrs_.size() != 1)
    {
        WarningInFunction
            << "Requested single file, but multiple files are present"
            << endl;
    }

    if (!filePtrs_.set(0))
    {
        FatalErrorInFunction
            << "File pointer at index " << 0 << " not allocated"
            << abort(FatalError);
    }

    return filePtrs_[0];
}


CML::PtrList<CML::OFstream>& CML::functionObjectFile::files()
{
    if (!Pstream::master())
    {
        FatalErrorInFunction
            << "Request for files() can only be done by the master process"
            << abort(FatalError);
    }

    return filePtrs_;
}


CML::OFstream& CML::functionObjectFile::file(const label i)
{
    if (!Pstream::master())
    {
        FatalErrorInFunction
            << "Request for file(i) can only be done by the master process"
            << abort(FatalError);
    }

    if (!filePtrs_.set(i))
    {
        FatalErrorInFunction
            << "File pointer at index " << i << " not allocated"
            << abort(FatalError);
    }

    return filePtrs_[i];
}


CML::label CML::functionObjectFile::charWidth() const
{
    return IOstream::defaultPrecision() + addChars;
}


void CML::functionObjectFile::writeCommented
(
    Ostream& os,
    const string& str
) const
{
    os  << setw(1) << "#" << setw(1) << ' '
        << setw(charWidth() - 2) << str.c_str();
}


void CML::functionObjectFile::writeTabbed
(
    Ostream& os,
    const string& str
) const
{
    os  << tab << setw(charWidth()) << str.c_str();
}


void CML::functionObjectFile::writeHeader
(
    Ostream& os,
    const string& str
) const
{
    os  << setw(1) << "#" << setw(1) << ' '
        << setf(ios_base::left) << setw(charWidth() - 2) << str.c_str() << nl;
}


// ************************************************************************* //
