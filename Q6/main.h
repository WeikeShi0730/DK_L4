
/*
 * 
 * Simulation_Run of the ALOHA Protocol
 * 
 * Copyright (C) 2014 Terence D. Todd
 * Hamilton, Ontario, CANADA
 * todd@mcmaster.ca
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 * 
 */

/**********************************************************************/

#ifndef _MAIN_H_
#define _MAIN_H_

/**********************************************************************/

#include "simlib.h"
#include "simparameters.h"
#include "channel.h"

/**********************************************************************/

typedef double Time;
typedef Fifoqueue_Ptr Buffer_Ptr;

/**********************************************************************/

typedef struct _station_
{
  int id;
  Buffer_Ptr buffer;
  long int packet_count;
  double accumulated_delay;
  double mean_delay;
} Station, *Station_Ptr;

/**********************************************************************/

typedef enum
{
  WAITING,
  TRANSMITTING
} Packet_Status;

typedef struct _packet_
{
  double arrive_time;
  double service_time;
  double X;
  int station_id;
  Packet_Status status;
  int collision_count;
} Packet, *Packet_Ptr;

typedef struct _simulation_run_data_
{
  Station_Ptr stations;
  Channel_Ptr channel;
  long int blip_counter;
  long int arrival_count;
  long int packets_processed;
  long int number_of_packets_processed;
  long int number_of_collisions;
  double accumulated_delay;
  unsigned random_seed;
  double current_slot_end_time;
  double sim_time;
  double throughput;
  double mean_delay;
  double G_per_X;

  int cnt_slot;
  int reserve_mode;
  int num_mini_slot;
  double mini_slot_duration;
  double mean_data_packet_duration;

  Fifoqueue_Ptr data_fifo;
  double expect_end_data_packet_duration;

  double arrival_rate;
  double packet_backoff_duration;
  int number_of_stations;
} Simulation_Run_Data, *Simulation_Run_Data_Ptr;

/**********************************************************************/

/*
 * Function prototypes
 */

int main(void);

/**********************************************************************/

#endif /* main.h */
