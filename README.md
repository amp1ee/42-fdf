# 42-fdf
Topographic wireframes 3D viewer. Reads data from map files. School42 / UNIT.ua project.

Install minilibx: https://achedeuzot.me/2014/12/20/installer-la-minilibx/

1. Download the sources:
- Linux:
```
  wget http://files.achedeuzot.me/42/mlx/mlx-2014-01-06.tgz
```
- MacOSX:
```
  wget http://files.achedeuzot.me/42/mlx/mlx-macosx-el-capitan-2015-11-05.tgz
```
2. Unpack and build the sources:
```
  tar -xvzf mlx-2014-01-06.tgz
  cd minilibx/
  make
```  
    *If make fails with an error like this one: "mlx_xpm.c:(.text+0x119): undefined reference to `strlcpy'"
    just replace 'strlcpy' with 'strncpy' in minilibx sources:
```
  sed -i 's/strlcpy/strncpy/g' mlx_xpm.c
```
Install:
```
git clone https://github.com/amp1ee/42-fdf
cd 42-fdf && make
```
Usage:
```
./fdf maps/42.fdf
```
