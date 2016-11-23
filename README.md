# Overview
This is all the code that runs on numbat's embedded module computers

It contains a few key components:

- **Module-specific codebases** - these define the behaviour of different numbat modules. This is found in the `modules` directory.
- **eChronos** - an open-source Real Time Operating System that provides cool stuff like task scheduling & prioritisation. (See https://github.com/echronos/echronos). An echronos distribution is found in the `echronos` directory.
- **The TI software framework** - A bunch of driver libraries that make developing on our hardware easier, abstracting away lots of painful operations. This can be found in the `libs/ti` directory.

**To grab the source**:

    git clone https://github.com/bluesat/numbat-embedded.git
    cd numbat-embedded

# Getting up & running

First, make sure you have the `arm-none-eabi` GNU toolchain and `arm-none-eabi-gdb` for building and debugging the RTOS for ARM. These can be found in ubuntu/debian/arch package repositories.

If you decide to use a different ARM toolchain, you may have to modify the `.compiler_makedefs` file to suit your system.

In the root folder, run:

    make

to build all modules.

If you are having problems, check your prerequisites are being met.

If everything goes well, you will end up with a bunch of .elf files in the build directory in the repository root folder. These are ready to flash to the hardware!

# Running modules on real hardware

(insert brief openocd tutorial here)

# eChronos configuration

Each numbat module contains a `.prx` file, which is an eChronos configuration file. The `.prx` file is used for declaring OS primitives, like tasks, mutexes, semaphores, interrupts, etc. When a numbat module is built, a customized version of eChronos is constructed from this file.
For information as to how to use this file, see the 'Configuration Reference' section of the eChronos manual `docs/echronos/*`

# Adding new modules

Adding new numbat modules involves creating a new folder in the `modules` directory with at least one `.c` and `.prx` file, as well as modifying the top-level makefile.

To add a new module to the Makefile, open it up and copy one of the existing modules. You will need to change the `MODULE_NAME`, as well as the compilation steps. For each source file in your module, you will want to:

- Add an `.o` (object) target for each `.c` (source file). i.e:

        $(BUILD_DIR)/echronos_test.o: $(MODULE_DIR)/echronos_test.c $(MODULE_ECHRONOS)

- Add the object target to your final elf.

        $(BUILD_DIR)/$(MODULE_NAME).elf: \
            $(BUILD_DIR)/echronos_test.o \ # <-- Like this
            $(LIB_BUILD)/$(MODULE_NAME)-echronos.a

The eChronos & TI header files and sources will automagically be attached to your elf file, so you don't need to worry about that.

Finally, so that your new module actually gets looked at, you will need to add to to the `TARGETS` line as a new elf target:

    TARGETS=\
        $(BUILD_DIR)/blinky.elf \
        $(BUILD_DIR)/echronos_test.elf # <-- Like this

Hit make, and all compiling well you should get your shiny new module in `build`

# Help & Documentation

The following manuals are kept in this repository **in case of upstream changes**. We do **not** claim ownership over any of the contents of these manuals.:

* **eChronos manual**: `docs/echronos/*`. Note that this is for the kochab (feature set) armv7m (platform) echronos variant *only*, which is what we are using.

* **TI library user guide**: `docs/ti/*`. API documentation for the various TI libraries available to numbat modules.

* Note: **TI example code**: This can be found on TI's website inside their software package (TivaWare for EK-TM4C123GXL). Useful for figuring out how to use some peripherals. It is not distributed here for copyright reasons but mostly because it is huge.
