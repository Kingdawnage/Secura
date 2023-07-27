cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(wxWidgets CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)

set(SRCS "App.cpp" "Cryptograph.cpp" "Decrypt_Panel.cpp" "Encrypt_Panel.cpp" "MainFrame.cpp")

if(APPLE)
	add_executable(main MACOSX_BUNDLE ${SRCS})

	set_target_properties(main PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)
else()
	add_executable(main WIN32 ${SRCS})
endif()

target_link_libraries(main PRIVATE wx::core wx::base) 
target_link_libraries(main PRIVATE OpenSSL::SSL OpenSSL::Crypto)