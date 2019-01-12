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
    None

Description
    NotImpleemnted function that can be switched off

Contributors/Copyright:
    2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "switchableNotImplemented.hpp"
#include "HashSet.hpp"
#include "dictionary.hpp"

namespace CML {

    void switchableNotImplemented(
        const word &methodName,
        const dictionary &dict
    ) {
        static HashSet<fileName> notImplementedWarningAlreadyIssued;

        word switchName="ignore_unimplemented_"+methodName;
        bool warn=dict.lookupOrDefault<bool>(switchName,false);

        if(warn) {
            fileName fullName=dict.name()+"_"+methodName;
            if(!notImplementedWarningAlreadyIssued.found(fullName)) {
                Info << endl;
                WarningInFunction
                    << "The method " << methodName << " isn't properly "
                    << " implemented (at least that is what the developer "
                    << " thinks. You chose to ignore this for "
                    << dict.name() << endl
                    << "The consequences are your responsibility "
                    << "(but if you're lucky everything will be OK)" << endl
                    << "This warning will only appear once"
                    << nl << endl;

                notImplementedWarningAlreadyIssued.insert(fullName);
            }
        } else {
            Info << endl << endl;
            Info << "It seems that the method " << methodName
                << " is not properly implemented (neither in the class or "
                << " any appropriate subclasses)." << endl
                << "If you think that it should work anyway add the entry" << nl
                << nl << switchName << " true;" << nl << endl
                << "to the dictionary " << dict.name() << " and we will "
                << "go on. Alternativly properly implement the method." << nl
                << nl << "Anyway: I'll go die now";
            Info << endl << endl;
            notImplemented(methodName);
        }
    }
}
