sim.ARCHDIR = $(ARCH)
sim.CFLAGS += -DSITL -DAP -DFBW -DRADIO_CONTROL -DINTER_MCU -DDOWNLINK -DDOWNLINK_TRANSPORT=XBeeTransport -DINFRARED -DRADIO_CONTROL_SETTINGS -DSIM_UART -DSIM_XBEE -DXBEE_UART=SimUart
sim.srcs = radio_control.c downlink.c xbee.c commands.c gps.c inter_mcu.c infrared.c $(SRC_FIXEDWING)/stabilization/stabilization_attitude.c $(SRC_FIXEDWING)/guidance/guidance_v.c nav.c estimator.c cam.c sys_time.c $(SRC_FIRMWARE)/main_fbw.c $(SRC_FIRMWARE)/main_ap.c rc_settings.c $(SRC_ARCH)/ppm_hw.c $(SRC_ARCH)/sim_gps.c $(SRC_ARCH)/sim_ir.c $(SRC_ARCH)/sim_ap.c  $(SRC_ARCH)/sim_uart.c
