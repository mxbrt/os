# os

This is the tutorial-repo by Maximilian Breitenfeldt and Jakob Bode.

## make

Our Makefile supports the following commands:
* all - as typical build all
* clean - as typical clean up
* run - build the project an run it
* debug - as seen in the sample Makefile of this course
* gdb - as seen in the sample Makefile of this course

To run this project with the `run` command, you need to have the qemu binary
in the main directory named `qemu`.

We for example added a symlink to our qemu binarys build as described in the kvv
forums by leonard koenig:
```
ln -s ../qemu-build/arm-softmmu/qemu-system-arm build/qemu
```

To run our code you (hopefully!) only need to call
```
cd build && make run
```

If it's not working some environment variables may be missing:
```
LD_LIBRARY_PATH=/usr/local/lib:/import/sage-7.4/local/lib/
export LD_LIBRARY_PATH
PATH="$PATH:/home/mi/linnert/arm/bin/"
```

You may also source the script `build/andorra_env.sh` to initialize this environment.

Have fun!
