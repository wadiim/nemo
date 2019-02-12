if(UNIX)
  set(OS unix)
elseif(WIN32)
  set(OS windows)
endif()

set(SOURCE_FILES
    ${OS}/color.cpp
    ${OS}/utils.cpp
    ${OS}/term.cpp
    utils.cpp
    term.cpp
    file.cpp
    nemo.cpp
    main.cpp
)

add_executable(nemo ${SOURCE_FILES})
