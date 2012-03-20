project(libsunai CXX)

find_package(OpenCV 2 REQUIRED)
find_package(CURL REQUIRED)

# Output directories
set(SLIB_OUTPUT "../../../../lib")
set(DLIB_OUTPUT "../../../../bin")
set(EXEC_OUTPUT "../../../../bin")

# Library includes
include_directories(BEFORE SYSTEM "../../../include")

#include_directories(BEFORE SYSTEM "../../../3rdparty/opencv/include")
#include_directories(BEFORE SYSTEM "../../../3rdparty/curl/include")
include_directories(BEFORE SYSTEM ${OpenCV_INCLUDE_DIRS} ${CURL_INCLUDE_DIR})

# Sources path
set(LIB_SRC_PATH "../../../src/")
set(SAMPLES_SRC_PATH "../../../samples/")

# Add core files
list(APPEND LIB_FILES ${LIB_SRC_PATH}LibSUNAI.cpp)

# Add ArtDatabase files
list(APPEND LIB_FILES ${LIB_SRC_PATH}ArtDatabase/LabelDictionary.cpp ${LIB_SRC_PATH}ArtDatabase/ArtDatabase.cpp)

#Add Object dectection files
list(APPEND LIB_FILES ${LIB_SRC_PATH}ObjectDetection/DetectedObject.cpp ${LIB_SRC_PATH}ObjectDetection/ObjectDetector.cpp ${LIB_SRC_PATH}ObjectDetection/FaceObjectDetector.cpp )

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${EXEC_OUTPUT})

# Define the library path
add_library(libsunai SHARED ${LIB_FILES})

# Define the executable programs
add_executable(dbExplorer ${SAMPLES_SRC_PATH}DBExplorer/DBExplorer.cpp)

# Assign output files
set_target_properties(libsunai PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${SLIB_OUTPUT})
set_target_properties(libsunai PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${DLIB_OUTPUT})
set_target_properties(dbExplorer PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXEC_OUTPUT})
#set(RUNTIME_OUTPUT_DIRECTORY ${EXEC_OUTPUT})

target_link_libraries(dbExplorer libsunai ${OpenCV_LIBS} ${CURL_LIBRARY})
