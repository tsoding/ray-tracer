find_package(PythonInterp)

set(STYLE_FILTER)

# disable unwanted filters
set(STYLE_FILTER ${STYLE_FILTER}-legal/copyright,)
set(STYLE_FILTER ${STYLE_FILTER}-readability/todo,)

function(add_cpp_lint_target TARGET_NAME SOURCES_LIST)
  if (NOT PYTHONINTERP_FOUND)
    return()
  endif()

  list(REMOVE_DUPLICATES SOURCES_LIST)
  list(SORT SOURCES_LIST)

  add_custom_target(${TARGET_NAME}
    COMMAND "${CMAKE_COMMAND}" -E chdir
            "${CMAKE_CURRENT_SOURCE_DIR}"
            "${PYTHON_EXECUTABLE}"
            "${CMAKE_SOURCE_DIR}/misc/cpplint.py"
            "--filter=${STYLE_FILTER}"
            "--counting=detailed"
            "--extensions=cpp,hpp,h"
            "--linelength=250"
            ${SOURCES_LIST}
    DEPENDS ${SOURCES_LIST}
    COMMENT "Linting ${TARGET_NAME}"
    VERBATIM)
endfunction()
