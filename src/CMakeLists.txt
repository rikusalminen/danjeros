add_executable(${CMAKE_PROJECT_NAME}
    arch/x86_64/boot/multiboot.s
    arch/x86_64/boot/start.s
    arch/x86_64/isr.s
    arch/x86_64/interrupt.c
    arch/x86/pic.c
    kernel/main.c
    )

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
    LINK_DEPENDS "${LINKER_SCRIPT}"
    LINK_FLAGS "-Map ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.map"
    LINKER_LANGUAGE ASM-ATT
    )
