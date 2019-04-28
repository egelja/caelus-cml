/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
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

\*---------------------------------------------------------------------------*/

#include "memInfo.hpp"

// Undefine DebugInfo, because we don't use it here and it collides with a
// macro in windows.h
#undef DebugInfo

#include <windows.h>
#include <stdio.h>
#include <psapi.h>

// Define type for using the function GetProcessMemoryInfo
typedef BOOL (*GetProcessMemoryInfoType)
(
    HANDLE,
    PPROCESS_MEMORY_COUNTERS,
    DWORD
);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::memInfo::memInfo()
:
    peak_(-1),
    size_(-1),
    rss_(-1),
	swap_(0),
    GetProcessMemoryInfo_(nullptr)
{
  GetProcessMemoryInfo_ =
        (void *)(
        GetProcAddress(LoadLibrary("Psapi.dll"), "GetProcessMemoryInfo")
        );

  update();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::memInfo::~memInfo()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

const CML::memInfo& CML::memInfo::update()
{
    HANDLE hProcess = nullptr;
    PROCESS_MEMORY_COUNTERS pmc;

    // Reset to invalid values first
    peak_ = size_ = rss_ = -1;
	swap_ = 0;

    hProcess = OpenProcess(  PROCESS_QUERY_INFORMATION |
                             PROCESS_VM_READ,
                             FALSE, 
                             pid() );
    if (nullptr != hProcess)
    {
        if(GetProcessMemoryInfo_!= nullptr)
        {
            if
            (
                (GetProcessMemoryInfoType(GetProcessMemoryInfo_))
                (
                    hProcess,
                    &pmc,
                    sizeof(pmc)
                )
            )
            {
                peak_ = int(pmc.PeakWorkingSetSize*0.0009765625);
                size_ = int(pmc.WorkingSetSize*0.0009765625);
                rss_ = int(pmc.WorkingSetSize*0.0009765625);
            }
            else
            {
                WarningInFunction
                  << "GetProcessMemoryInfo function was unable to "
                  << "retrieve information." << endl;
            }
        }
        else
        {
            WarningInFunction
              << "Unable to access the GetProcessMemoryInfo function." << endl;
        }

        CloseHandle( hProcess );
    }

    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    swap_ = peak_ - int(memInfo.ullTotalPhys*0.0009765625);

    return *this;
}


bool CML::memInfo::valid() const
{
    return peak_ != -1;
}


bool CML::memInfo::swapping() const
{
	if (swap_ > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

CML::Istream& CML::operator>>(Istream& is, memInfo& m)
{
    is.readBegin("memInfo");

    is  >> m.peak_ >> m.size_ >> m.rss_;

    is.readEnd("memInfo");

    // Check state of Istream
    is.check
    (
        "CML::Istream& CML::operator>>(CML::Istream&, CML::memInfo&)"
    );

    return is;
}


CML::Ostream& CML::operator<<(Ostream& os, const memInfo& m)
{
    os  << token::BEGIN_LIST
        << m.peak_ << token::SPACE << m.size_ << token::SPACE << m.rss_
        << token::END_LIST;

    // Check state of Ostream
    os.check
    (
        "CML::Ostream& CML::operator<<(CML::Ostream&, "
        "const CML::memInfo&)"
    );

    return os;
}
