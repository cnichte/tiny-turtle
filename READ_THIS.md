# Probleme

Failed to set target esp32c6: non zero exit code 2 Directory '/Users/cnichte/develop-software/01-active/embedded/test/hello_world/build' doesn't seem to be a CMake build directory. 

Refusing to automatically delete files in this directory. 

Delete the directory manually to 'clean' it. Adding "set-target"'s dependency "fullclean" to list of commands with default set of options. Executing action: fullclean

## Build

/opt/homebrew/bin/cmake

/opt/homebrew/bin/ninja

Adding "set-target"'s dependency "fullclean" to list of commands with default set of options.
Executing action: fullclean
Build directory '/Users/cnichte/develop-software/01-active/embedded/test/hello_world/build' not found. Nothing to clean.
Executing action: set-target
Set Target to: esp32c6, new sdkconfig will be created.
Running cmake in directory /Users/cnichte/develop-software/01-active/embedded/test/hello_world/build
Executing "cmake -G Ninja -DPYTHON_DEPS_CHECKED=1 -DPYTHON=/Users/cnichte/.espressif/python_env/idf6.0_py3.13_env/bin/python -DESP_PLATFORM=1 -DIDF_TARGET=esp32c6 -DCCACHE_ENABLE=0 /Users/cnichte/develop-software/01-active/embedded/test/hello_world"...
-- Found Git: /usr/bin/git (found version "2.39.5 (Apple Git-154)")

-- Minimal build - ON

-- The C compiler identification is GNU 14.2.0

-- The CXX compiler identification is GNU 14.2.0

-- The ASM compiler identification is GNU

-- Found assembler: /Users/cnichte/.espressif/tools/riscv32-esp-elf/esp-14.2.0_20241119/riscv32-esp-elf/bin/riscv32-esp-elf-gcc

-- Detecting C compiler ABI info

-- Detecting C compiler ABI info - done

-- Check for working C compiler: /Users/cnichte/.espressif/tools/riscv32-esp-elf/esp-14.2.0_20241119/riscv32-esp-elf/bin/riscv32-esp-elf-gcc - skipped

-- Detecting C compile features

-- Detecting C compile features - done

-- Detecting CXX compiler ABI info

-- Detecting CXX compiler ABI info - done

-- Check for working CXX compiler: /Users/cnichte/.espressif/tools/riscv32-esp-elf/esp-14.2.0_20241119/riscv32-esp-elf/bin/riscv32-esp-elf-g++ - skipped

-- Detecting CXX compile features

-- Detecting CXX compile features - done

-- git rev-parse returned 'fatal: not a git repository (or any of the parent directories): .git'
-- Could not use 'git describe' to determine PROJECT_VER.
-- Building ESP-IDF components for target esp32c6

CMake Error at /Users/cnichte/esp/esp-idf/tools/cmake/component.cmake:229 (message):
  CMake Warning (dev) at build_properties.temp.cmake:8:

    Syntax Warning in cmake code at column 106



    Argument not separated from preceding token by whitespace.

  Call Stack (most recent call first):

    /Users/cnichte/esp/esp-idf/tools/cmake/scripts/component_get_requirements.cmake:3 (include)

  This warning is for project developers.  Use -Wno-dev to suppress it.




  CMake Error at
  /Users/cnichte/esp/esp-idf/components/esp-tls/CMakeLists.txt:47:

    Parse error.  Expected "(", got right paren with text ")".

  Call Stack (most recent call first):

    /Users/cnichte/esp/esp-idf/tools/cmake/scripts/component_get_requirements.cmake:107 (include)
    /Users/cnichte/esp/esp-idf/tools/cmake/scripts/component_get_requirements.cmake:160 (__component_get_requirements)





Call Stack (most recent call first):
  /Users/cnichte/esp/esp-idf/tools/cmake/build.cmake:655 (__component_get_requirements)
  /Users/cnichte/esp/esp-idf/tools/cmake/project.cmake:736 (idf_build_process)
  CMakeLists.txt:8 (project)



-- Configuring incomplete, errors occurred!

cmake failed with exit code 1, output of the command is in the /Users/cnichte/develop-software/01-active/embedded/test/hello_world/build/log/idf_py_stderr_output_33491 and /Users/cnichte/develop-software/01-active/embedded/test/hello_world/build/log/idf_py_stdout_output_33491

Failed to set target esp32c6: non zero exit code 2

Adding "set-target"'s dependency "fullclean" to list of commands with default set of options.
Executing action: fullclean
Build directory '/Users/cnichte/develop-software/01-active/embedded/test/hello_world/build' not found. Nothing to clean.
Executing action: set-target
Set Target to: esp32c6, new sdkconfig will be created.
Running cmake in directory /Users/cnichte/develop-software/01-active/embedded/test/hello_world/build
Executing "cmake -G Ninja -DPYTHON_DEPS_CHECKED=1 -DPYTHON=/Users/cnichte/.espressif/python_env/idf6.0_py3.13_env/bin/python -DESP_PLATFORM=1 -DIDF_TARGET=esp32c6 -DCCACHE_ENABLE=0 /Users/cnichte/develop-software/01-active/embedded/test/hello_world"...
-- Found Git: /usr/bin/git (found version "2.39.5 (Apple Git-154)")
-- Minimal build - ON
-- The C compiler identification is GNU 14.2.0
-- The CXX compiler identification is GNU 14.2.0
-- The ASM compiler identification is GNU
-- Found assembler: /Users/cnichte/.espressif/tools/riscv32-esp-elf/esp-14.2.0_20241119/riscv32-esp-elf/bin/riscv32-esp-elf-gcc
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /Users/cnichte/.espressif/tools/riscv32-esp-elf/esp-14.2.0_20241119/riscv32-esp-elf/bin/riscv32-esp-elf-gcc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /Users/cnichte/.espressif/tools/riscv32-esp-elf/esp-14.2.0_20241119/riscv32-esp-elf/bin/riscv32-esp-elf-g++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- git rev-parse returned 'fatal: not a git repository (or any of the parent directories): .git'
-- Could not use 'git describe' to determine PROJECT_VER.
-- Building ESP-IDF components for target esp32c6
CMake Error at /Users/cnichte/esp/esp-idf/tools/cmake/component.cmake:229 (message):
  CMake Warning (dev) at build_properties.temp.cmake:8:

    Syntax Warning in cmake code at column 106



    Argument not separated from preceding token by whitespace.

  Call Stack (most recent call first):

    /Users/cnichte/esp/esp-idf/tools/cmake/scripts/component_get_requirements.cmake:3 (include)

  This warning is for project developers.  Use -Wno-dev to suppress it.



  CMake Error at
  /Users/cnichte/esp/esp-idf/components/esp-tls/CMakeLists.txt:47:

    Parse error.  Expected "(", got right paren with text ")".

  Call Stack (most recent call first):

    /Users/cnichte/esp/esp-idf/tools/cmake/scripts/component_get_requirements.cmake:107 (include)
    /Users/cnichte/esp/esp-idf/tools/cmake/scripts/component_get_requirements.cmake:160 (__component_get_requirements)





Call Stack (most recent call first):
  /Users/cnichte/esp/esp-idf/tools/cmake/build.cmake:655 (__component_get_requirements)
  /Users/cnichte/esp/esp-idf/tools/cmake/project.cmake:736 (idf_build_process)
  CMakeLists.txt:8 (project)


-- Configuring incomplete, errors occurred!
cmake failed with exit code 1, output of the command is in the /Users/cnichte/develop-software/01-active/embedded/test/hello_world/build/log/idf_py_stderr_output_33491 and /Users/cnichte/develop-software/01-active/embedded/test/hello_world/build/log/idf_py_stdout_output_33491
.
Directory '/Users/cnichte/develop-software/01-active/embedded/test/hello_world/build' doesn't seem to be a CMake build directory. Refusing to automatically delete files in this directory. Delete the directory manually to 'clean' it.

Adding "set-target"'s dependency "fullclean" to list of commands with default set of options.
Executing action: fullclean

Failed to set target esp32c6: non zero exit code 2

Directory '/Users/cnichte/develop-software/01-active/embedded/test/hello_world/build' doesn't seem to be a CMake build directory. Refusing to automatically delete files in this directory. Delete the directory manually to 'clean' it.
Adding "set-target"'s dependency "fullclean" to list of commands with default set of options.
Executing action: fullclean
.
Directory '/Users/cnichte/develop-software/01-active/embedded/test/hello_world/build' doesn't seem to be a CMake build directory. Refusing to automatically delete files in this directory. Delete the directory manually to 'clean' it.

Adding "set-target"'s dependency "fullclean" to list of commands with default set of options.
Executing action: fullclean

Failed to set target esp32c6: non zero exit code 2

Directory '/Users/cnichte/develop-software/01-active/embedded/test/hello_world/build' doesn't seem to be a CMake build directory. Refusing to automatically delete files in this directory. Delete the directory manually to 'clean' it.
Adding "set-target"'s dependency "fullclean" to list of commands with default set of options.
Executing action: fullclean
.
Directory '/Users/cnichte/develop-software/01-active/embedded/test/hello_world/build' doesn't seem to be a CMake build directory. Refusing to automatically delete files in this directory. Delete the directory manually to 'clean' it.

Adding "set-target"'s dependency "fullclean" to list of commands with default set of options.
Executing action: fullclean

Failed to set target esp32c6: non zero exit code 2

Directory '/Users/cnichte/develop-software/01-active/embedded/test/hello_world/build' doesn't seem to be a CMake build directory. Refusing to automatically delete files in this directory. Delete the directory manually to 'clean' it.
Adding "set-target"'s dependency "fullclean" to list of commands with default set of options.
Executing action: fullclean
.
[SDK Configuration Editor]
Executing action: confserver
Running cmake in directory /Users/cnichte/develop-software/01-active/embedded/test/hello_world/build
Executing "cmake -G Ninja -DPYTHON_DEPS_CHECKED=1 -DPYTHON=/Users/cnichte/.espressif/python_env/idf6.0_py3.13_env/bin/python -DESP_PLATFORM=1 -DSDKCONFIG='/Users/cnichte/develop-software/01-active/embedded/test/hello_world/sdkconfig' -DCCACHE_ENABLE=0 /Users/cnichte/develop-software/01-active/embedded/test/hello_world"...
-- Found Git: /usr/bin/git (found version "2.39.5 (Apple Git-154)")

-- Minimal build - ON

-- The C compiler identification is GNU 14.2.0

-- The CXX compiler identification is GNU 14.2.0

-- The ASM compiler identification is GNU

-- Found assembler: /Users/cnichte/.espressif/tools/xtensa-esp-elf/esp-14.2.0_20241119/xtensa-esp-elf/bin/xtensa-esp32-elf-gcc

-- Detecting C compiler ABI info

-- Detecting C compiler ABI info - done

-- Check for working C compiler: /Users/cnichte/.espressif/tools/xtensa-esp-elf/esp-14.2.0_20241119/xtensa-esp-elf/bin/xtensa-esp32-elf-gcc - skipped

-- Detecting C compile features

-- Detecting C compile features - done

-- Detecting CXX compiler ABI info

-- Detecting CXX compiler ABI info - done

-- Check for working CXX compiler: /Users/cnichte/.espressif/tools/xtensa-esp-elf/esp-14.2.0_20241119/xtensa-esp-elf/bin/xtensa-esp32-elf-g++ - skipped

-- Detecting CXX compile features

-- Detecting CXX compile features - done

-- git rev-parse returned 'fatal: not a git repository (or any of the parent directories): .git'
-- Could not use 'git describe' to determine PROJECT_VER.
-- Building ESP-IDF components for target esp32

---------------------------ERROR--------------------------

CMake Error at /Users/cnichte/esp/esp-idf/tools/cmake/component.cmake:229 (message):
  CMake Warning (dev) at build_properties.temp.cmake:8:

    Syntax Warning in cmake code at column 106



    Argument not separated from preceding token by whitespace.

  Call Stack (most recent call first):

    /Users/cnichte/esp/esp-idf/tools/cmake/scripts/component_get_requirements.cmake:3 (include)

  This warning is for project developers.  Use -Wno-dev to suppress it.




-----------------------END OF ERROR-----------------------
---------------------------ERROR--------------------------

  CMake Error at
  /Users/cnichte/esp/esp-idf/components/esp-tls/CMakeLists.txt:47:

    Parse error.  Expected "(", got right paren with text ")".

  Call Stack (most recent call first):

    /Users/cnichte/esp/esp-idf/tools/cmake/scripts/component_get_requirements.cmake:107 (include)
    /Users/cnichte/esp/esp-idf/tools/cmake/scripts/component_get_requirements.cmake:160 (__component_get_requirements)





Call Stack (most recent call first):
  /Users/cnichte/esp/esp-idf/tools/cmake/build.cmake:655 (__component_get_requirements)
  /Users/cnichte/esp/esp-idf/tools/cmake/project.cmake:736 (idf_build_process)
  CMakeLists.txt:8 (project)



-----------------------END OF ERROR-----------------------
-- Configuring incomplete, errors occurred!

---------------------------ERROR--------------------------

cmake failed with exit code 1, output of the command is in the /Users/cnichte/develop-software/01-active/embedded/test/hello_world/build/log/idf_py_stderr_output_33970 and /Users/cnichte/develop-software/01-active/embedded/test/hello_world/build/log/idf_py_stdout_output_33970

-----------------------END OF ERROR-----------------------
---------------------------ERROR--------------------------

SDK Configuration editor confserver process exited with code: 2
-----------------------END OF ERROR-----------------------