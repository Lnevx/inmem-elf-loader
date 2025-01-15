set(BUILD_TESTS_TARGET build-tests)
set(RUN_TESTS_TARGET run-tests)

add_custom_target(${BUILD_TESTS_TARGET})

# Setup checker (python) environment
set(CHECKER_VENV ${CMAKE_CURRENT_BINARY_DIR}/venv)
set(CHECKER_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_command(OUTPUT ${CHECKER_VENV}
    COMMAND python3 -m venv ${CMAKE_CURRENT_BINARY_DIR}/venv
        && . ${CMAKE_CURRENT_BINARY_DIR}/venv/bin/activate
        && pip install -r ${CMAKE_CURRENT_SOURCE_DIR}/requirements.txt

    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/requirements.txt
    COMMENT "Installing virtualenv for checker"
)
add_custom_target(checker-venv DEPENDS ${CHECKER_VENV})
add_dependencies(${BUILD_TESTS_TARGET} checker-venv)


function(run_test target_test target_file loader_file)
    add_test(
        NAME ${target_file}
        COMMAND ${CHECKER_VENV}/bin/python3
            ${CHECKER_SRC_DIR}/${target_test}.py TARGET=${target_file} LOADER=${loader_file}
    )
endfunction()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/c)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/cpp)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/go)
