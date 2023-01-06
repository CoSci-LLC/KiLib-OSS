#"KILIB_BUILD_TESTS": "OFF",
#"KILIB_BUILD_IPO": "ON",
#"KILIB_LOG_LEVEL": "0",
#"KILIB_STRICT_BUILD": "OFF",

# Function that sets the appropriate flags / options above for the given target
function(SetKiLibOptions target)
    # Turn on C++17
    set_property(TARGET ${target} PROPERTY CXX_STANDARD 17)
    set_property(TARGET ${target} PROPERTY CXX_EXTENSIONS OFF)
    set_property(TARGET ${target} PROPERTY CXX_STANDARD_REQUIRED ON)

    set(BUILD_SHARED_LIBS OFF)

    _SetKiLibWarnings(${target})
endfunction()

# Function that sets warning flags
function(_SetKiLibWarnings target)
    if (KILIB_COMPILER_IS_GNU_LIKE) 
        target_compile_options(${target} PRIVATE -Wall -Wextra)
    else()
        target_compile_options(${target} PRIVATE /W4)
    endif()

    # If KILIB_STRICT_BUILD then turn on warnings as errors
    if (KILIB_STRICT_BUILD)
        if (KILIB_COMPILER_IS_GNU_LIKE) 
            target_compile_options(${target} PRIVATE -Werror)
        else()
            target_compile_options(${target} PRIVATE /WX)
        endif()
    endif()
endfunction()
