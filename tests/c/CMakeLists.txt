enable_language(C)
set(CMAKE_C_STANDARD 11)

function(build_test target_test enable_pie)
    set(TARGET ${target_test}-c-${enable_pie})

    add_executable(${TARGET} EXCLUDE_FROM_ALL
        ${CMAKE_CURRENT_SOURCE_DIR}/${target_test}.c
    )
    target_compile_options(${TARGET} PRIVATE
        -Wall -Wextra
    )
    target_link_options(${TARGET} PRIVATE
        $<IF:$<STREQUAL:${enable_pie},pie>,-static-pie,-static>
    )
    add_dependencies(${BUILD_TESTS_TARGET} ${TARGET})

    run_test(${target_test} ${TARGET} $<TARGET_FILE:loader>)
endfunction()

build_test(test-echo no-pie)
build_test(test-echo pie)
