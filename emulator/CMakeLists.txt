
set(CMAKE_C_STANDARD 11)

# --- PDCurses Integration using Makefile ---

# Define path to the PDCurses submodule
set(PDCURSES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/PDCurses)
set(PDCURSES_WINCON_DIR ${PDCURSES_DIR}/wincon)

# Determine the correct make command and library name based on the compiler.
# For MinGW/GCC, you may need to set CMAKE_MAKE_PROGRAM to "mingw32-make".
if(MSVC)
    set(PDCURSES_MAKE_COMMAND nmake -f Makefile.vc)
    set(PDCURSES_LIB_NAME pdcurses.lib)
else()
    # Assumes a GCC-compatible make (like mingw32-make or make).
    set(PDCURSES_MAKE_COMMAND mingw32-make -f Makefile)
    set(PDCURSES_LIB_NAME pdcurses.a)
endif()

# Define the full path to the library that will be built
set(PDCURSES_LIBRARY ${PDCURSES_WINCON_DIR}/${PDCURSES_LIB_NAME})

# Add a custom target to build PDCurses using its Makefile.
# This command is executed in the 'wincon' directory.
add_custom_target(pdcurses_build ALL
    COMMAND ${PDCURSES_MAKE_COMMAND}
    WORKING_DIRECTORY ${PDCURSES_WINCON_DIR}
    BYPRODUCTS ${PDCURSES_LIBRARY}
    COMMENT "Building PDCurses library (${PDCURSES_LIB_NAME}) using its Makefile"
)

# Create an IMPORTED library target to represent the compiled PDCurses library.
# This allows us to treat it like a regular CMake target.
add_library(pdcurses STATIC IMPORTED GLOBAL)
set_target_properties(pdcurses PROPERTIES
    IMPORTED_LOCATION ${PDCURSES_LIBRARY}
    INTERFACE_INCLUDE_DIRECTORIES ${PDCURSES_DIR} # For curses.h
)

# Ensure PDCurses is built before any target that uses it.
add_dependencies(pdcurses pdcurses_build)

# --- End PDCurses Integration ---

add_executable(ToyTk80_emu
        main.c
        core.c
        display.c
        instruction.c
        instruction/logic_instruction.c
        instruction/ld_instruction.c
        instruction/math_instruction.c
        instruction/condition_instruction.c
        instruction/unimplement_instruction.c
        instruction/jump_instruction.c
        instruction/stack_instruction.c
        instruction/call_instruction.c
        # file_reader.c
)

# Link the PDCurses library to the emulator.
# This will also add the necessary include directory (${PDCURSES_DIR})
# because it's specified in the 'pdcurses' IMPORTED target.
target_link_libraries(ToyTk80_emu PRIVATE pdcurses)

target_compile_options(ToyTk80_emu
        PRIVATE -masm=intel -O3
)
target_compile_definitions(ToyTk80_emu
        PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
)
target_include_directories(ToyTk80_emu PRIVATE ../)
target_link_libraries(ToyTk80_emu PRIVATE ToyTk80_util)