@echo off

echo ----------Input File----------

set /p input=
echo %input%.exe

echo ----------Compling----------
g++ -finput-charset=UTF-8 -fexec-charset=gbk %input%.cpp -o %input%.exe -lglfw3 -lgdi32 -lopengl32 -lglew32 -lglu32
echo ----------Running----------
%input%.exe

echo ----------End----------
pause