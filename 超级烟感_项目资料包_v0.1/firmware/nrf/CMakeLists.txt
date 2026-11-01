# nRF54L15 skeleton. Host-sim sources are not on this target.
cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(chaoji_smoke_nrf_skeleton LANGUAGES C)

target_sources(app PRIVATE src/main.c)

# Not linked. firmware/MANIFEST.json hashes these for the host-sim package.
# Add only after HARDWARE_NEEDED.md is confirmed and the port compiles:
#   ${CMAKE_CURRENT_LIST_DIR}/../src/fw_node.c
#   ${CMAKE_CURRENT_LIST_DIR}/../src/fw_ux.c
#   ${CMAKE_CURRENT_LIST_DIR}/../src/fw_status.c
# Never add firmware/src/fw_sim.c.
