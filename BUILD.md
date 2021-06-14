# Building Caelus-CML

## Setup

### Build tools

To build Caelus-CML, you will GCC, Git, Python, and a text editor such as [Notepad++](https://notepad-plus-plus.org) or [VSCode](https://code.visualstudio.com/).

1.  Download and install either [Notepad++](https://notepad-plus-plus.org) or [VSCode](https://code.visualstudio.com/), whichever you prefer. If you already have a text editor, skip this step.
2.  Download the build tools and dependencies for your platform:

    1.  Linux: Just run the following command:

        ```sh
        $> sudo apt install git build-essential flex bison zlib1g-dev python
        ```

        Now continue to [Setup CPL](#setup-cpl).

    2.  MacOS: Clang comes with the developer tools package, just open a terminal and type `clang`, then click install. Alternatively, you could download XCode.

    3.  Windows:

        1.  Download and install [MSys2](https://www.msys2.org/). Select the default options in the installer. After the installer has finished, open MSys2 and run:

            ```sh
            $> pacman -Syu

            $> pacman -Su
            ```

            This is all detailed on the front page of the MSys2 site.

            <details><summary>Windows Terminal</summary>
            MSys2 adds 3 new shells, which, combined with the 2 shells Windows comes with, means that you have 5 shells on your computer. To help manage those, you may want to download [Windows Terminal](https://aka.ms/terminal).


            <details><summary>Sample Config</summary>
            For the MSys2 terminals, add this under `profiles.list` in `settings.json`:

            ```json
            {
                "commandline": "C:/msys64/msys2_shell.cmd -defterm -here -no-start -mingw64",
                "guid": "{17da3cac-b318-431e-8a3e-7fcdefe6d114}",
                "hidden": false,
                "icon": "C:/msys64/mingw64.ico",
                "name": "MINGW64 / MSYS2",
            },
            {
                "commandline": "C:/msys64/msys2_shell.cmd -defterm -here -no-start -mingw32",
                "guid": "{2d51fdc4-a03b-4efe-81bc-722b7f6f3820}",
                "icon": "C:/msys64/mingw32.ico",
                "name": "MINGW32 / MSYS2",
            },
            {
                "commandline": "C:/msys64/msys2_shell.cmd -defterm -here -no-start -msys",
                "guid": "{71160544-14d8-4194-af25-d05feeac7233}",
                "icon": "C:/msys64/msys2.ico",
                "name": "MSYS / MSYS2",
            }
            ```
            </details>
            </details>

        2.  Download and install the needed packages for building:

            ```sh
            $> pacman -S --needed base-devel mingw-w64-x86_64-toolchain

            $> pacman -S mingw-w64-x86_64-msmpi
            ```

        3.  Download and install Git. This can be done in one of 2 ways:
            1. Just add the Git package: `pacman -S git`. _Note: this is slower, since it requires a compatibility layer._
            2. Install inside MSys2 Proper (**recommended**):
               1. Download `winpty` for better use of certain applications in MSys2: `pacman -S winpty`.
               2. Follow the directions in [this guide](https://github.com/git-for-windows/git/wiki/Install-inside-MSYS2-proper).
               3. Close all MSys2 windows after the `git-extra` step, but **DO NOT** open any new ones.
               4. Open the `C:\msys64\etc\profile.d` folder, and delete the `git-prompt.sh` and `git-sdk.sh` files. These will change your prompt and make it into a Git-for-Windows development environment, and are therefore unneeded.
               5. Add `C:\msys64\cmd` to your PATH.
        4.  Download and install the version of MS-MPI from [here](https://docs.microsoft.com/en-us/message-passing-interface/microsoft-mpi-release-notes) matching the version of the [MSys2 package](https://packages.msys2.org/package/mingw-w64-x86_64-msmpi?repo=mingw64).
        5. Download and install [Python](https://python.org/). Make sure to check the box to add it to your PATH.
        6. Add the `C:\msys64\mingw64\bin` directory to your PATH.



