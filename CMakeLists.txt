cmake_minimum_required(VERSION 3.16)

project(HashTable)

add_executable(ht-test)

set_target_properties(
	ht-test
	PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Bin/$<0:>"
		ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Lib/$<0:>"
		LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Lib/$<0:>"
		C_STANDARD 11
		C_STANDARD_REQUIRED ON
		C_EXTENSIONS OFF
)

if(MSVC)
	target_compile_options(
		ht-test
			PRIVATE
				/W4 /wd4996 / $<$<CONFIG:RELEASE>: /WX /O2>
	)
else()
	target_compile_options(
		ht-test
			PRIVATE
				-Wall -Wextra -pedantic $<$<CONFIG:RELEASE>: -Werror -O3>
	)
endif()

add_subdirectory(Source)
