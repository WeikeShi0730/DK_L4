
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

#define SMALL_TIME 0.001
//#define FAST_RUN
//#define D_Arrival

/*******************************************************************************/

#ifdef FAST_RUN

#define NUMBER_OF_STATIONS 10
#define MEAN_PACKET_DURATION 1      /* normalized packet Tx time */
//#define PACKET_ARRIVAL_RATE 0.3, 0.31, 0.35, 0.5, 0.7, 0.9, 1.0, 2, 5, 8, 10  
#define PACKET_ARRIVAL_RATE 0.35, 0.4
#define MEAN_BACKOFF_DURATION 5   /* in units of packet transmit time, Tx */
#define RUNLENGTH 1E2
#define BLIPRATE 100000

/* Comma separated list of random seeds to run. */
#define RANDOM_SEED_LIST 400050636

#else
#define NUMBER_OF_STATIONS 5, 10
#define MEAN_PACKET_DURATION 1      /* normalized packet Tx time */
//#define PACKET_ARRIVAL_RATE 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1     /* packets per Tx time 0.01, 0.15, 0.19, 0.2, 0.3, 0.35, 0.5,*/
#define PACKET_ARRIVAL_RATE 0.01, 0.1, 0.15, 0.19, 0.2, 0.25, 0.3, 0.31, 0.35, 0.5, 0.7  
#define MEAN_BACKOFF_DURATION 5, 5   /* in units of packet transmit time, Tx */
#define RUNLENGTH 1E5
#define BLIPRATE 100000

/* Comma separated list of random seeds to run. */
#define RANDOM_SEED_LIST 400050636, 400099173, 345

#endif //FAST_RUN

/*******************************************************************************/

#endif /* simparameters.h */






