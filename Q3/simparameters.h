
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

#define USE_MIN_C 

/*******************************************************************************/

#define NUMBER_OF_STATIONS 10
#define MEAN_PACKET_DURATION 1      /* normalized packet Tx time */
//#define PACKET_ARRIVAL_RATE 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1     /* packets per Tx time 0.01, 0.15, 0.19, 0.2, 0.3, 0.35, 0.5,*/
#define PACKET_ARRIVAL_RATE 10 
#define RUNLENGTH 1E6
#define BLIPRATE 100000
#define MIN_C 3

/* Comma separated list of random seeds to run. */
//#define RANDOM_SEED_LIST 300, 678, 8874, 9876, 6909, 1573, 3450, 400050636, 400099173
#define RANDOM_SEED_LIST 300 
/*******************************************************************************/

#endif /* simparameters.h */






