#!/usr/bin/env python

import sys, os, stat
from pathlib import Path

# Locate sphinx
try:
    import sphinx
except:
    print("Sphinx not found. Quitting.")
    sys.exit(1)

print(f"Action is '{sys.argv[1]}'.")
if sys.argv[1] == "build":
    print("Nothing to build. Quitting.")
    sys.exit(0)
elif sys.argv[1] == "install":
    pass
else:
    print(f"{sys.argv[1]} action not known. Quitting.")
    sys.exit(1)

# Remove the old installation
py_version = sys.version_info

sphinx_dir = Path(os.path.dirname(sphinx.__file__))
old_dest = sphinx_dir / "numfig.py"
old_destc = sphinx_dir / "numfig.pyc"

sphinx_ext_dir = sphinx_dir / "ext"
dest = sphinx_ext_dir / "numfig.py"
destc = (
    sphinx_ext_dir
    / "__pycache__"
    / f"numfig.cpython-{py_version.major}{py_version.minor}.pyc"
)

for f in [dest, old_dest, destc, old_destc]:
    try:
        os.remove(f)
    except:
        pass

# Assume we are in my directory
import shutil

print(f"Copying numfig.py to {dest}")
shutil.copyfile("numfig.py", dest)

# Compile to bytecode
import py_compile

print(f"Compiling {dest} to bytecode.")
py_compile.compile(dest)

# Fix perms
mode = stat.S_IRUSR | stat.S_IWUSR | stat.S_IRGRP | stat.S_IWGRP | stat.S_IROTH
print(f"Setting mode of {dest} to 664.")
os.chmod(dest, mode)
os.chmod(destc, mode)
