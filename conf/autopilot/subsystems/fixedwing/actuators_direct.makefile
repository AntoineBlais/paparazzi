# for lisa_l

ap.CFLAGS += -DACTUATORS=\"servos_direct_hw.h\" -DSERVOS_DIRECT
ap.srcs += $(SRC_ARCH)/servos_direct_hw.c $(SRC_FIXEDWING)/actuators.c


# TODO TODO UGLY HACK: We re-use the booz actuators: Should become universal actuator code!!
# Carefull: paths might get broken with this silly rotorcraft/fixedwing mixup of directories

ifeq ($(ARCH), stm32)
ap.srcs    += firmwares/rotorcraft/actuators/arch/stm32/actuators_pwm_arch.c
ap.CFLAGS  += -Ifirmwares/rotorcraft/actuators/arch/stm32
endif
