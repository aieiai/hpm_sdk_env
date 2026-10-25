# Copyright (c) 2021-2023 HPMicro
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.13)

set(CONFIG_CHERRYUSB 1)
set(CONFIG_USB_DEVICE 1)
set(CONFIG_USB_DEVICE_HID 1)
set(CONFIG_ECLIPSE_THREADX 1)

#set(CONFIG_ECLIPSE_THREADX_TIMER_RESOURCE_MTIMER 1) to use mchtmr as system's tick source
#set(CONFIG_ECLIPSE_THREADX_TIMER_RESOURCE_MTIMER 1)

find_package(hpm-sdk REQUIRED HINTS $ENV{HPM_SDK_BASE})
project(cherryusb_device_hid_keyboard_threadx)

sdk_inc(../../../config)
sdk_app_src(src/main.c)
sdk_app_src(src/hid_keyboard.c)

sdk_compile_definitions(-DUSE_NONVECTOR_MODE=1)
sdk_compile_definitions(-DDISABLE_IRQ_PREEMPTIVE=1)
if(TARGET app)
    target_compile_options(app PRIVATE "-Wextra") # "-Wignored-qualifiers"
endif()

sdk_ses_opt_debug_connection(J-Link)
sdk_ses_opt_debug_jlink_speed(50000)

if (CONFIG_SEGGER_RTT)
  sdk_ses_opt_lib_io_type(RTT)
endif()

generate_ide_projects()


