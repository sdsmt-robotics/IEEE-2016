echo Compile Python library
gcc -fPIC -shared -I/usr/include/python2.7/ -lpython2.7 -o gpio.so gpio.c
echo Move Python library into the module directory
mv gpio.so gpiolib/
