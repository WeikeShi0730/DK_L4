
/*
 * Simulation of the ALOHA Protocol
 * 
 * Copyright (C) 2014 Terence D. Todd Hamilton, Ontario, CANADA
 * todd@mcmaster.ca
 * 
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/*******************************************************************************/

#ifndef _SIMPARAMETERS_H_
#define _SIMPARAMETERS_H_

//#define ASSERT_OFF
#define SMALL_TIME 0.001
//#define FAST_RUN
//#define D_Arrival
//#define D_duration
//#define SIM_TIME_END 26400
#define MEAN_PACKET_DURATION 1 //not use

/*******************************************************************************/

#ifdef FAST_RUN


#define NUMBER_OF_STATIONS 5*1.000,  5*1.000,  5*1.000,  5*1.000
#define PACKET_ARRIVAL_RATE  0.15, 0.2, 0.25, 0.3, 0.35, 0.4   /* packets per Tx time */
#define MEAN_BACKOFF_DURATION 5*1.000,  5*1.000,  5*1.000,  5*1.000  /* in units of packet transmit time  Tx */
#define MINI_SLOT_M 10*1.000,  10*1.000,  10*1.000,  10*1.000 
#define MINI_SLOT_DURATION 1*1,  1*2,  1*3,  1*4 //same as MEAN_PACKET_DURATION   
#define MEAN_DATA_PACKET_DURATION 5*1.000,  5*1.000,  5*1.000,  5*1.000
#define RUNLENGTH 1E4
#define BLIPRATE 100000


/* Comma separated list of random seeds to run. */
#define RANDOM_SEED_LIST 400050636, 400099173, 345

#else

#define NUMBER_OF_STATIONS 5*1,  5*2,  5*3,  5*4,  5*5
#define PACKET_ARRIVAL_RATE 0.01, 0.05, 0.1, 0.13, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4   /* packets per Tx time */
#define MEAN_BACKOFF_DURATION 5*1.000,  5*1.000,  5*1.000,  5*1.000,  5*1.000  /* in units of packet transmit time  Tx */
#define MINI_SLOT_M 10*1.000,  10*1.000,  10*1.000,  10*1.000,  10 *1.000 
#define MINI_SLOT_DURATION 1*1,  1*1,  1*1,  1*1,  1*1 //same as MEAN_PACKET_DURATION   
#define MEAN_DATA_PACKET_DURATION 5*1.000,  5*1.000,  5*1.000,  5*1.000,  5*1.000
#define RUNLENGTH 1E5
#define BLIPRATE 100000

/* Comma separated list of random seeds to run. */
#define RANDOM_SEED_LIST 400050636, 341
//#define RANDOM_SEED_LIST 400050636, 400099173, 345

#endif //FAST_RUN

/*******************************************************************************/

#endif /* simparameters.h */






