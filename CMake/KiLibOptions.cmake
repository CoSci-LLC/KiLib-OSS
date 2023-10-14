# Function that sets the appropriate flags / options above for the given target
function(SetKiLibOptions target)
    # Turn on C++17
    set_property(TARGET ${target} PROPERTY CXX_STANDARD 20)
    set_property(TARGET ${target} PROPERTY CXX_EXTENSIONS OFF)
    set_property(TARGET ${target} PROPERTY CXX_STANDARD_REQUIRED ON)

    set(BUILD_SHARED_LIBS OFF)

    _SetKiLibWarnings(${target})
    _SetKiLibIPO(${target})
    _SetKiLibLogLevel(${target})
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

function(_SetKiLibIPO target)
    # If KILIB_BUILD_IPO is set then enable interprocedural optimization
    if (KILIB_BUILD_IPO)
        include(CheckIPOSupported)
        check_ipo_supported(RESULT KILIB_IPO_SUPPORTED OUTPUT KILIB_IPO_ERROR)
        if (KILIB_IPO_SUPPORTED)
            set_property(TARGET ${target} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
            message(STATUS "${target}: IPO is supported and enabled")
        else()
            message(WARNING "${target}: IPO is not supported: ${KILIB_IPO_ERROR}")
        endif()
    endif()
endfunction()

function(_SetKiLibLogLevel target)
    if (KILIB_LOG_LEVEL)
        target_compile_definitions(${target} PRIVATE SPDLOG_ACTIVE_LEVEL=${KILIB_LOG_LEVEL})
    endif()
endfunction()
