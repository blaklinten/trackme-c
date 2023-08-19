# TrackMe-C
This is a version of TrackMe written in C. 

# Dependencies
    - libgit2
    - mongoose
    - libmongoc
    - cmocka
# Building
Build the project with CMake:
    ```shell
    cd $PROJECT_ROOT
    mkdir build
    cd build/
    cmake ..
    make
    ```
# Testing
Tests are written with the `cmocka` testing library.
Test the project by running the `test_main` after successful compilation.
