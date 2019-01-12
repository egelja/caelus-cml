/*---------------------------------------------------------------------------*\
Copyright (C) 2016-2018 OpenFOAM Foundation
Copyright (C) 2018 Applied CCM Pty Ltd
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Application
    caelusDictionary

Description
    Interrogates and manipulates dictionaries.

Usage
    \b caelusDictionary [OPTION] dictionary

      - \par -entry \<name\>
        Selects an entry

      - \par -keywords \<name\>
        Prints the keywords (of the selected entry or of the top level if
        no entry was selected

      - \par -add \<value\>
        Adds the entry (should not exist yet)

      - \par -set \<value\>
        Adds or replaces the entry

      - \par -merge \<value\>
        Merges the entry

      - \par -dict
        Set, add or merge entry from a dictionary

      - \par -remove
        Remove the selected entry

      - \par -diff \<dictionary\>
        Write differences with respect to the specified dictionary
        (or sub entry if -entry specified)

      - \par -expand
        Read the specified dictionary file, expand the macros etc. and write
        the resulting dictionary to standard output.

      - \par -includes
        List the \c #include and \c #includeIfPresent files to standard output

      - \par -disableFunctionEntries
        Do not expand macros or directives (#include etc)

    Example usage:
      - Change simulation to run for one timestep only:
        \verbatim
          caelusDictionary system/controlDict -entry stopAt -set writeNow
        \endverbatim

      - Change solver:
        \verbatim
           caelusDictionary system/fvSolution -entry solvers.p.solver -set PCG
        \endverbatim

      - Print bc type:
        \verbatim
           caelusDictionary 0/U -entry boundaryField.movingWall.type
        \endverbatim

      - Change bc parameter:
        \verbatim
           caelusDictionary 0/U -entry boundaryField.movingWall.value \
             -set "uniform (2 0 0)"
        \endverbatim

      - Change whole bc type:
        \verbatim
          caelusDictionary 0/U -entry boundaryField.movingWall \
            -set "{type uniformFixedValue; uniformValue (2 0 0);}"
        \endverbatim

      - Write the differences with respect to a template dictionary:
        \verbatim
          caelusDictionary 0/U -diff templates/closedVolume/0/U
        \endverbatim

      - Write the differences in boundaryField with respect to a
        template dictionary:
        \verbatim
          caelusDictionary 0/U -diff templates/closedVolume/0/U \
            -entry boundaryField
        \endverbatim

      - Change patch type:
        \verbatim
          caelusDictionary constant/polyMesh/boundary \
            -entry entry0.fixedWalls.type -set patch
        \endverbatim
        This uses special parsing of Lists which stores these in the
        dictionary with keyword 'entryDDD' where DDD is the position
        in the dictionary (after ignoring the FoamFile entry).

\*---------------------------------------------------------------------------*/

#include "argList.hpp"
#include "Time.hpp"
#include "IFstream.hpp"
#include "OFstream.hpp"
#include "includeEntry.hpp"
#include "Pair.hpp"

using namespace CML;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//- Converts old scope syntax to new syntax
word scope(const fileName& entryName)
{
    if (entryName.find(':') != string::npos)
    {
        wordList entryNames(entryName.components(':'));

        word entry(entryNames[0]);
        for (label i = 1; i < entryNames.size(); i++)
        {
            entry += word('.') + entryNames[i];
        }
        return entry;
    }
    else
    {
        return entryName;
    }
}


//- Extracts dict name and keyword
Pair<word> dictAndKeyword(const word& scopedName)
{
    string::size_type i = scopedName.find_last_of(".");
    if (i != string::npos)
    {
        return Pair<word>
        (
            scopedName.substr(0, i),
            scopedName.substr(i+1, string::npos)
        );
    }
    else
    {
        return Pair<word>("", scopedName);
    }
}


const dictionary& lookupScopedDict
(
    const dictionary& dict,
    const word& subDictName
)
{
    if (subDictName == "")
    {
        return dict;
    }
    else
    {
        const entry* entPtr = dict.lookupScopedEntryPtr
        (
            subDictName,
            false,
            false
        );
        if (!entPtr || !entPtr->isDict())
        {
            FatalIOErrorIn("Function",dict)
                << "keyword " << subDictName
                << " is undefined in dictionary "
                << dict.name() << " or is not a dictionary"
                << endl
                << "Valid keywords are " << dict.keys()
                << exit(FatalIOError);
        }
        return entPtr->dict();
    }
}


void remove(dictionary& dict, const dictionary& removeDict, fileName dictName)
{
    forAllConstIter(dictionary, removeDict, iter)
    {
        const entry* entPtr = dict.lookupEntryPtr
        (
            iter().keyword(),
            false,
            false
        );

        if (entPtr)
        {
            if (entPtr->isDict())
            {
                if (iter().isDict())
                {
                   // Store old dictionary name - this is the parent
                   fileName oldDictName = dictName;

                   // Update the dictionar name to include the child
                   dictName = dictName + "." + iter().keyword();

                    remove
                    (
                        const_cast<dictionary&>(entPtr->dict()),
                        iter().dict(),
                        dictName
                    );

                    // Check if dictionary is empty
                    if (!entPtr->dict().size())
                    {
                        dict.remove(iter().keyword());
                    }
                    // Either Child deleted because dictionar was empoty
                    // or still contains entries not found in template.
                    // Reset name to parent
                    dictName = oldDictName;
                }
            }
            else if (!iter().isDict())
            {
                if (*entPtr == iter())
                {
                    dict.remove(iter().keyword());
                }
                else
                {
                    Info<< "Dictionary:"<< dictName << endl;
                    Info<< "Dictionary entry " << iter().keyword() << " is different to template: "<< nl
                        << "Expected entry: "<< nl << iter() 
                        << "Actual entry  : "<< nl << *entPtr
                        << endl;
                    // Remove keyword from dictionary so we are left with only new entries
                    dict.remove(iter().keyword());
                }
            }
        }
        else
        {
             Info<< "Dictionary:"<< dictName << endl;
             Info<< "Template entry not found in dictionary: "<< nl 
                 << iter().keyword()<<nl
                 << "Expected entry:"<< nl
                 << iter()<<endl;
        }
    }
}


int main(int argc, char *argv[])
{
    #include "removeCaseOptions.hpp"

    argList::noBanner();
    argList::addNote("manipulates dictionaries");
    argList::validArgs.append("dictionary file");
    argList::addBoolOption("keywords", "list keywords");
    argList::addOption("entry", "name", "report/select the named entry");
    argList::addBoolOption
    (
        "value",
        "Print entry value"
    );
    argList::addOption
    (
        "set",
        "value",
        "Set entry value or add new entry"
    );
    argList::addOption
    (
        "add",
        "value",
        "Add a new entry"
    );
    argList::addOption
    (
        "merge",
        "value",
        "Merge entry"
    );
    argList::addBoolOption
    (
        "dict",
        "Set, add or merge entry from a dictionary."
    );
    argList::addBoolOption
    (
        "remove",
        "Remove the entry."
    );
    argList::addOption
    (
        "diff",
        "dict",
        "Write differences with respect to the specified dictionary"
    );
    argList::addBoolOption
    (
        "includes",
        "List the #include/#includeIfPresent files to standard output"
    );
    argList::addBoolOption
    (
        "expand",
        "Read the specified dictionary file, expand the macros etc. and write "
        "the resulting dictionary to standard output"
    );
    argList::addBoolOption
    (
        "disableFunctionEntries",
        "Disable expansion of dictionary directives - #include, etc"
    );

    argList args(argc, argv);

    const bool listIncludes = args.optionFound("includes");

    if (listIncludes)
    {
        CML::functionEntries::includeEntry::report = true;
    }

    if (args.optionFound("disableFunctionEntries"))
    {
        entry::disableFunctionEntries = true;
    }


    fileName dictFileName(args[1]);

    autoPtr<IFstream> dictFile(new IFstream(dictFileName));
    if (!dictFile().good())
    {
        FatalErrorIn("Function")
            << "Cannot open file " << dictFileName
            << exit(FatalError, 1);
    }

    // Read but preserve headers
    dictionary dict;
    dict.read(dictFile(), true);


    bool changed = false;

    if (listIncludes)
    {
        return 0;
    }
    else if (args.optionFound("expand"))
    {
        IOobject::writeBanner(Info)
            <<"//\n// " << dictFileName << "\n//\n";
        dict.write(Info, false);
        IOobject::writeDivider(Info);

        return 0;
    }


    // Second dictionary for -diff
    dictionary diffDict;
    fileName diffFileName;
    if (args.optionReadIfPresent("diff", diffFileName))
    {
        autoPtr<IFstream> diffFile(new IFstream(diffFileName));
        if (!diffFile().good())
        {
            FatalErrorIn("Function")
                << "Cannot open file " << diffFileName
                << exit(FatalError, 1);
        }

        // Read but preserve headers
        diffDict.read(diffFile(), true);
    }


    word entryName;
    if (args.optionReadIfPresent("entry", entryName))
    {
        word scopedName(scope(entryName));

        string newValue;
        if
        (
            args.optionReadIfPresent("set", newValue)
         || args.optionReadIfPresent("add", newValue)
         || args.optionReadIfPresent("merge", newValue)
        )
        {
            const bool overwrite = args.optionFound("set");
            const bool merge = args.optionFound("merge");

            Pair<word> dAk(dictAndKeyword(scopedName));
            const dictionary& d(lookupScopedDict(dict, dAk.first()));

            entry* ePtr = nullptr;

            if (args.optionFound("dict"))
            {
                const fileName fromDictFileName(newValue);
                autoPtr<IFstream> fromDictFile(new IFstream(fromDictFileName));
                if (!fromDictFile().good())
                {
                    FatalErrorInFunction
                        << "Cannot open file " << fromDictFileName
                        << exit(FatalError, 1);
                }

                dictionary fromDict(fromDictFile());

                const entry* fePtr
                (
                    fromDict.lookupScopedEntryPtr
                    (
                        scopedName,
                        false,
                        true            // Support wildcards
                    )
                );

                if (!fePtr)
                {
                    FatalErrorInFunction
                        << "Cannot find entry " << entryName
                        << " in file " << fromDictFileName
                        << exit(FatalError, 1);
                }

                ePtr = fePtr->clone().ptr();
            }
            else
            {
                IStringStream str(string(dAk.second()) + ' ' + newValue + ';');
                ePtr = entry::New(str).ptr();
            }

            if (overwrite)
            {
                const_cast<dictionary&>(d).set(ePtr);
            }
            else
            {
                const_cast<dictionary&>(d).add(ePtr, merge);
            }
            changed = true;

            // Print the changed entry
            // const entry* entPtr = dict.lookupScopedEntryPtr
            // (
            //     scopedName,
            //     false,
            //     true            // Support wildcards
            // );
            // if (entPtr)
            // {
            //     Info<< *entPtr;
            // }
        }
        else if (args.optionFound("remove"))
        {
            // Extract dictionary name and keyword
            Pair<word> dAk(dictAndKeyword(scopedName));

            const dictionary& d(lookupScopedDict(dict, dAk.first()));
            const_cast<dictionary&>(d).remove(dAk.second());
            changed = true;
        }
        else
        {
            // Optionally remove a second dictionary
            if (args.optionFound("diff"))
            {
                Pair<word> dAk(dictAndKeyword(scopedName));

                const dictionary& d(lookupScopedDict(dict, dAk.first()));
                const dictionary& d2(lookupScopedDict(diffDict, dAk.first()));

                const entry* ePtr =
                    d.lookupEntryPtr(dAk.second(), false, true);
                const entry* e2Ptr =
                    d2.lookupEntryPtr(dAk.second(), false, true);

                if (ePtr && e2Ptr)
                {
                    if (*ePtr == *e2Ptr)
                    {
                        const_cast<dictionary&>(d).remove(dAk.second());
                    }
                    else if (ePtr->isDict() && e2Ptr->isDict())
                    {
                        fileName dictName(dAk.first());
                        remove
                        (
                            const_cast<dictionary&>(ePtr->dict()),
                            e2Ptr->dict(),
                            dictName
                        );
                    }
                }
            }


            const entry* entPtr = dict.lookupScopedEntryPtr
            (
                scopedName,
                false,
                true            // Support wildcards
            );

            if (entPtr)
            {
                if (args.optionFound("keywords"))
                {
                    const dictionary& dict = entPtr->dict();
                    forAllConstIter(dictionary, dict, iter)
                    {
                        Info<< iter().keyword() << endl;
                    }
                }
                else
                {
                    if (args.optionFound("value"))
                    {
                        if (entPtr->isStream())
                        {
                            const tokenList& tokens = entPtr->stream();
                            forAll(tokens, i)
                            {
                                Info<< tokens[i];
                                if (i < tokens.size() - 1)
                                {
                                    Info<< token::SPACE;
                                }
                            }
                            Info<< endl;
                        }
                        else if (entPtr->isDict())
                        {
                            Info<< entPtr->dict();
                        }
                    }
                    else
                    {
                        Info<< *entPtr;
                    }
                }
            }
            else
            {
                FatalIOErrorIn("Function",dictFile)
                    << "Cannot find entry " << entryName
                    << exit(FatalIOError, 2);
            }
        }
    }
    else if (args.optionFound("keywords"))
    {
        forAllConstIter(dictionary, dict, iter)
        {
            Info<< iter().keyword() << endl;
        }
    }
    else if (args.optionFound("diff"))
    {
        fileName dictName;
        remove(dict, diffDict, dictName);
        if (dict.size())
        {
            Info<<"Addtional entries found in dictionary:"<<endl;
            dict.write(Info, false);
        }
    }
    else
    {
        dict.write(Info, false);
    }

    if (changed)
    {
        dictFile.clear();
        OFstream os(dictFileName);
        IOobject::writeBanner(os);
        dict.write(os, false);
        IOobject::writeEndDivider(os);
    }

    return 0;
}
