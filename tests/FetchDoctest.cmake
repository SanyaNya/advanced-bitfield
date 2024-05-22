
include(FetchContent)

# Fetch doctest
FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG "v2.4.11")

# make the dependencies available to the build system
FetchContent_MakeAvailable(doctest)
