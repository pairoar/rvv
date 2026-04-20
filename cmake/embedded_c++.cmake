if (USE_EMBEDDED_C++)
    target_compile_options(cp_cxx_options INTERFACE
        $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions -fno-rtti>
    )
endif()