set(GOARCH_TRANSLATE_x86_64 amd64)
set(GOARCH_TRANSLATE_i386 386)

function(build_test target_test enable_pie)
    set(TARGET ${target_test}-go-${enable_pie})
    set(SRC_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${target_test}.go)
    set(TARGET_FILE ${CMAKE_CURRENT_BINARY_DIR}/${TARGET})

    if (${enable_pie} STREQUAL no-pie)
        add_custom_command(
            OUTPUT ${TARGET_FILE}
            DEPENDS ${SRC_FILE}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMAND env GOOS=linux GOARCH=${GOARCH_TRANSLATE_${ARCH}}
                CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER}
                go build -o ${TARGET_FILE} ${SRC_FILE}
        )
    else()
        add_custom_command(
            OUTPUT ${TARGET_FILE}
            DEPENDS ${SRC_FILE}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMAND env GOOS=linux GOARCH=${GOARCH_TRANSLATE_${ARCH}} CGO_ENABLED=1
                CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER}
                go build -o ${TARGET_FILE}
                -ldflags "-linkmode external -s -w -extldflags -static-pie"
                -buildmode=pie -tags static_build ${SRC_FILE}
        )
    endif()

    add_custom_target(${TARGET} DEPENDS ${TARGET_FILE})
    add_dependencies(${BUILD_TESTS_TARGET} ${TARGET})

    run_test(${target_test} ${TARGET} $<TARGET_FILE:loader>)
endfunction()


build_test(test-echo no-pie)
build_test(test-echo pie)
