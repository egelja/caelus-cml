Follow this to compile the entire documentation, which includes separate files for theory, validation and tutorials guides:
1. Install sphinx site-packages (http://www.sphinx-doc.org/en/stable/install.html)
2. Install third party extensions located in Caelus/doc/packages: to do this from within each extension directory (eg. sphinx_numfig*/), type 
-----------------------------
python setup.py install
-----------------------------
3. In the doc directory, run the wrapper documentation to generate the html files for entire documentation. 

