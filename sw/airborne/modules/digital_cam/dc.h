/*
 * Paparazzi $Id$
 *
 * Copyright (C) 2003-2008  Pascal Brisset, Antoine Drouin
 *
 * This file is part of paparazzi.
 *
 * paparazzi is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * paparazzi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with paparazzi; see the file COPYING.  If not, write to
 * the Free Software Foundation, 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */

/** \file dc.h
 *  \brief Digital Camera Control
 *
 * Provides the control of the shutter and the zoom of a digital camera
 * through standard binary IOs of the board.
 *
 * Configuration:
 *  Since the API of led.h is used, connected pins must be defined as led
 *  numbers (usually in the airframe file):
 *   <define name="DC_SHUTTER_LED" value="6"/>
 *   <define name="DC_ZOOM_LED" value="7"/>
 *  Related bank and pin must also be defined:
 *   <define name="LED_6_BANK" value="0"/>
 *   <define name="LED_6_PIN" value="2"/>
 *  The required initialization (dc_init()) and periodic (4Hz) process
 *  (dc_periodic()) are called if the DIGITAL_CAM flag is set:
 *   ap.CFLAGS += -DDIGITAL_CAM
 *
 * Usage (from the flight plan, the settings or any airborne code):
 *  - dc_Shutter(_) sets the DC_SHUTTER_LED pin output to 1 for 0.5s and sends
 *   a DC_SHOT message
 *  - dc_Zoom(_) sets the DC_ZOOM_LED pin output to 1 for 0.5s
 *  - dc_Periodic(s) activates a periodic call to dc_Shutter() every s seconds
 */

#ifndef DC_H
#define DC_H

#include "std.h"
#include "led.h"
#include "airframe.h"
#include "estimator.h"
#include "gps.h"


extern uint8_t dc_timer;

extern uint8_t dc_periodic_shutter;
/* In s. If non zero, period of automatic calls to dc_shutter() */
extern uint8_t dc_shutter_timer;
/* In s. Related counter */

extern uint8_t dc_utm_threshold;
/* In m. If non zero, automatic shots when greater than utm_north % 100 */

/* Picture Number starting from zero */
extern uint16_t dc_photo_nr;
extern uint8_t dc_shoot;

#ifndef DC_PUSH
#define DC_PUSH LED_ON
#endif

#ifndef DC_RELEASE
#define DC_RELEASE LED_OFF
#endif

#define SHUTTER_DELAY 2  /* 4Hz -> 0.5s */

uint8_t dc_shutter( void );

static inline uint8_t dc_zoom( void ) {
  dc_timer = SHUTTER_DELAY;
#ifdef DC_ZOOM_LED
  DC_PUSH(DC_ZOOM_LED);
#endif
  return 0;
}

#define dc_Shutter(_) ({ dc_shutter(); 0; })
#define dc_Zoom(_) ({ dc_zoom(); 0; })
#define dc_Periodic(s) ({ dc_periodic_shutter = s; dc_shutter_timer = s; 0; })

#ifndef DC_PERIODIC_SHUTTER
#define DC_PERIODIC_SHUTTER 0
#endif

#define dc_init() { /* initialized as leds */ dc_periodic_shutter = DC_PERIODIC_SHUTTER; DC_PUSH(DC_SHUTTER_LED);} /* Output */


#ifndef DC_GPS_TRIGGER_START
#define DC_GPS_TRIGGER_START 1
#endif
#ifndef DC_GPS_TRIGGER_STOP
#define DC_GPS_TRIGGER_STOP 3
#endif

void dc_send_shot_position(void);

static inline void dc_shoot_on_gps( void ) {
  static uint8_t gps_msg_counter = 0;

  if (dc_shoot > 0)
  {

    if (gps_msg_counter == 0)
    {
      DC_PUSH(DC_SHUTTER_LED);

      dc_send_shot_position();
    }
    else if (gps_msg_counter == DC_GPS_TRIGGER_START)
    {
      DC_RELEASE(DC_SHUTTER_LED);
    }

    gps_msg_counter++;
    if (gps_msg_counter >= DC_GPS_TRIGGER_STOP)
      gps_msg_counter = 0;
  }
}

/* 4Hz */
static inline void dc_periodic( void ) {
  if (dc_timer) {
    dc_timer--;
  } else {
    DC_RELEASE(DC_SHUTTER_LED);
#ifdef DC_ZOOM_LED
    DC_RELEASE(DC_ZOOM_LED);
#endif
  }

  if (dc_shoot > 0)
  {
    if (dc_periodic_shutter) {
      RunOnceEvery(2,
      {
        if (dc_shutter_timer) {
  	  dc_shutter_timer--;
        } else {
	  dc_shutter();
	  dc_shutter_timer = dc_periodic_shutter;
        }
      });
    }
  }
  else
  {
    dc_shutter_timer = 0;
  }
}

static inline void dc_shot_on_utm_north_close_to_100m_grid( void ) {
  if (dc_utm_threshold && !dc_timer) {
    uint32_t dist_to_100m_grid = (gps_utm_north / 100) % 100;
    if (dist_to_100m_grid < dc_utm_threshold || 100 - dist_to_100m_grid < dc_utm_threshold)
      dc_shutter();
  }
}

#endif // DC_H
