s,$[({]top_builddir[)}]/intl/libintl.a,-lintl,
s/\(-*\)no-undefined/\1allow-shlib-undefined/g
s/^flex++_LDFLAGS.*=/& -Wl,--major-image-version=2 -Wl,--minor-image-version=5 /
s/^flex++_OBJECTS.*=/& flex++-rc.o/
s/^flex_LDFLAGS.*=/& -Wl,--major-image-version=2 -Wl,--minor-image-version=5 /
s/^flex_OBJECTS.*=/& flex-rc.o/
/^.cc*.o:/i\
# Rule to make compiled resource (Windows)\
%-rc.o: %.rc\
	windres -i $< -o $@
s,/cygdrive/\([^/]*\)/,\1:/,g
s/ln -s/cp -fp/g
