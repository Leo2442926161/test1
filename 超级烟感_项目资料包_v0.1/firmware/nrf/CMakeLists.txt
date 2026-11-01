# nRF54L15 application skeleton. Not an EVT flash image.
cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(chaoji_smoke_nrf_skeleton LANGUAGES C)

# Host behavior, ICD v0.1.7. The host CLI is not a source.
set(FW_DIR ${CMAKE_CURRENT_LIST_DIR}/..)

target_sources(app PRIVATE
	src/main.c
	src/hook_session.c
	${FW_DIR}/src/fw_node.c
	${FW_DIR}/src/fw_ux.c
	${FW_DIR}/src/fw_status.c
)

target_include_directories(app PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/include
	${FW_DIR}/include
)
