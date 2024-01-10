@echo off

set SHADER_DIR=Source
set TARGET_DIR=SPV

glslangValidator -V -S vert %SHADER_DIR%/Unlit.vert -o %TARGET_DIR%/Unlit_vert.spv
glslangValidator -V -S frag %SHADER_DIR%/Unlit.frag -o %TARGET_DIR%/Unlit_frag.spv

pause