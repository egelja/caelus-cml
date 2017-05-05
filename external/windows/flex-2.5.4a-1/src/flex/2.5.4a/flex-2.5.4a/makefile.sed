s/^OBJECTS.*=/& flex-rc.o/
/^.c.o:/i\
# Rule to make compiled resource (Windows)\
%-rc.o: %.rc\
	windres -i $< -o $@
s/CPPFLAGS =/CPPFLAGS +=/
