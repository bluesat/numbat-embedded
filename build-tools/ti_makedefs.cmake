set(CMAKE_VERBOSE_MAKEFILE 1)
set(CPU "-mcpu=cortex-m4 -mstructure-size-boundary=8")
set(FPU "-mfpu=fpv4-sp-d16 -mfloat-abi=hard")

set(CMAKE_ASM_FLAGS "-mthumb ${CPU} ${FPU} -mlittle-endian -MD -Dgcc")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS
        "-mthumb \
        ${CPU} \
        ${FPU} \
        -mlittle-endian \
        -fno-exceptions \
        -Wall \
        -pedantic \
        -DPART_${PART} \
        -DTARGET_IS_TM4C123_RB1 \
        -Dgcc"
)
# -std=c99?
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS
        "-mthumb \
        ${CPU} \
        ${FPU} \
        -O0 \
        -mlittle-endian \
        -fno-exceptions \
        -fno-unwind-tables \
        -fno-rtti \
        -frepo \
        -Wall \
        -pedantic \
        -DPART_${PART} \
        -DTARGET_IS_TM4C123_RB1 \
        -ffreestanding \
        -mtpcs-frame \
        -mtpcs-leaf-frame \
        -flto \
        -Dgcc"
)
# -std=c++11

#set(CMAKE_STATIC_LINKER_FLAGS "--gc-sections")
set(CMAKE_EXE_LINKER_FLAGS
        " -Wl,--emit-relocs \
		  -Wl,-print-gc-sections \
		  -nostdlib \
		 ${CPU}				  \
		 ${FPU}				  \
		 -O0 \
		 -mlittle-endian     \
		 -fno-exceptions     \
		 -fno-unwind-tables  \
		 -ffreestanding \
		 -fno-rtti		     \
		 -frepo				  \
          -flto \
		 -mthumb \
		 -Wall				   \
		 -pedantic		     \
		 -g \
		 -mtpcs-frame \
		 -mtpcs-leaf-frame \
		 -DPART_${PART}		\
		  -T new.ld       \
          -Wl,--print-memory-usage"
)
#        "--gc-sections")

set(LIBGCC ``)
