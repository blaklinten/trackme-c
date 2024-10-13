# TrackMe-C
This is a version of TrackMe written in C. 

## Dependencies
    - libmongoc
    - cmocka
## Building
Build the project with CMake:
```
cd $PROJECT_ROOT
mkdir build
cd build/
cmake ..
make
```
## Testing
Tests are written with the `cmocka` testing library.
Test the project by running the `test_main` after successful compilation.
To enable logging `export TRACKME_LOG_LEVEL` with values 1, 2 or 3 to have SUCCESS, ERROR, or INFO level.

## Utils
Use `./util/serve_web.sh` to start the web server.

```bash
cd ./util
./serve_web.sh
```

Use `ctrl + c` to reload, i.e. terminate the current process and reload from a freshly baked binary.
Use `q` to exit the infinite loop and terminate the serve script.
