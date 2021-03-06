#
# Makefile for radio_control susbsytem in fixedwing firmware
#
ifndef RADIO_CONTROL_SPEKTRUM_MODEL
RADIO_CONTROL_SPEKTRUM_MODEL=\"subsystems/radio_control/spektrum_dx7se.h\"
endif

ap.CFLAGS += -DRADIO_CONTROL -DRADIO_CONTROL_BIND_IMPL_FUNC=radio_control_spektrum_try_bind
ap.CFLAGS += -DRADIO_CONTROL_TYPE_H=\"subsystems/radio_control/spektrum.h\"
ifeq ($(BOARD), booz)
ap.CFLAGS += -DRADIO_CONTROL_SPEKTRUM_MODEL_H=$(RADIO_CONTROL_SPEKTRUM_MODEL)
endif
ifdef RADIO_CONTROL_LED
ap.CFLAGS += -DRADIO_CONTROL_LED=$(RADIO_CONTROL_LED)
endif
ap.CFLAGS += -DRADIO_CONTROL_SPEKTRUM_PRIMARY_PORT=$(RADIO_CONTROL_SPEKTRUM_PRIMARY_PORT)
ap.CFLAGS += -DOVERRIDE_$(RADIO_CONTROL_SPEKTRUM_PRIMARY_PORT)_IRQ_HANDLER -DUSE_TIM6_IRQ

ap.srcs += $(SRC_SUBSYSTEMS)/radio_control.c \
           $(SRC_SUBSYSTEMS)/radio_control/spektrum.c \
           $(SRC_ARCH)/subsystems/radio_control/spektrum_arch.c
