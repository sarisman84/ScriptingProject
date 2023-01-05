mkdir Bin\
copy Dependencies\dll\* Bin\
call "Premake/premake5" --file=Source/Scriptkurs/premake5.lua vs2022
pause