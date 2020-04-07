set path=c:\tc\bin
::clean
del *.obj
::build
::tcc -I. app.cpp 
::tcc -I. dat.cpp
tcc -ml -I. install.cpp