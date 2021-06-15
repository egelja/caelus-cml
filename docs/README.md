# Building documentation

Follow this guide to compile the entire documentation, which includes separate files for theory, validation and tutorials guides:

1. Install [Sphinx](http://www.sphinx-doc.org/en/stable/install.html). If you have setup CPL, then you will have already completed this step and can just run `workon cpl`. Otherwise, run `pip install -r requirements.txt`.
2. Install third party extensions located in `./packages`. In each package directory (i.e. `sphinx_numfig*`), run `python setup.py install`.
3. Now, in the `docs` directory, run either `make html` or `.\make.bat html` to build the documentation. The completed docs will be output in `Guides/`.
