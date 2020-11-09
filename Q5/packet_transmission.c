
/*
 * Simulation_Run of the ALOHA Protocol
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

#include <stdio.h>
#include <math.h>
#include "trace.h"
#include "output.h"
#include "channel.h"
#include "packet_transmission.h"

#include "packet_duration.h"
/*******************************************************************************/

long int
schedule_transmission_start_event(Simulation_Run_Ptr simulation_run,
				  Time event_time,
				  void * packet) 
{
  Event event;

  event.description = "Start Of Packet";
  event.function = transmission_start_event;
  event.attachment = packet;

  return simulation_run_schedule_event(simulation_run, event, event_time);
}

/*******************************************************************************/

void
transmission_start_event(Simulation_Run_Ptr simulation_run, void * ptr)
{
  Packet_Ptr this_packet;
  Simulation_Run_Data_Ptr data;
  Channel_Ptr channel;
  Time backoff_duration, now;
  double current_slot_end_time;
  double current_slot_start_time;
  double packet_duration;

  now = simulation_run_get_time(simulation_run);
  this_packet = (Packet_Ptr) ptr;
  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);
  channel = data->channel;
  current_slot_end_time = data->current_slot_end_time; 
  packet_duration = get_packet_duration();
  current_slot_start_time = current_slot_end_time - get_packet_duration() - SMALL_TIME * 2; 

    TRACE(printf("start event current_slot_end_time = %f\n", current_slot_end_time););

  if (now <= current_slot_end_time && now > current_slot_start_time + SMALL_TIME)
  {

    schedule_transmission_start_event(simulation_run, current_slot_end_time + uniform_generator()/100000.0, (void *) this_packet);

  }
  else if (now <= current_slot_start_time + SMALL_TIME && now >= current_slot_start_time)
  {

    /* This packet is starting to transmit. */
    increment_transmitting_stn_count(channel);
    this_packet->status = TRANSMITTING;

    if(get_channel_state(channel) != IDLE) {
    /* The channel is now colliding. */
    set_channel_state(channel, COLLISION);
    } else {
    /* The channel is successful, for now. */
    set_channel_state(channel, SUCCESS);
    }

    /* Schedule the end of packet transmission event. */
    schedule_transmission_end_event(simulation_run,
                  simulation_run_get_time(simulation_run) + 
                  this_packet->service_time,
                  (void *) this_packet);
  }
  else
  {
    TRACE(printf("Unknown case \n"););
  }
}

/*******************************************************************************/

long int
schedule_transmission_end_event(Simulation_Run_Ptr simulation_run,
				Time event_time,
				void * packet)
{
  Event event;

  event.description = "End of Packet";
  event.function = transmission_end_event;
  event.attachment = packet;

  return simulation_run_schedule_event(simulation_run, event, event_time);
}

/*******************************************************************************/

void
transmission_end_event(Simulation_Run_Ptr simulation_run, void * packet)
{
  Packet_Ptr this_packet, next_packet;
  Buffer_Ptr buffer;
  Time backoff_duration, now;
  Simulation_Run_Data_Ptr data;
  Channel_Ptr channel;
  int num_of_slots;
  double back_time;

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);
  channel = data->channel;

  now = simulation_run_get_time(simulation_run);
  data->sim_time = now;

  this_packet = (Packet_Ptr) packet;
  buffer = (data->stations+this_packet->station_id)->buffer;

  /* This station has stopped transmitting. */
  decrement_transmitting_stn_count(channel);

    TRACE(printf("end event current_slot_end_time = %f\n", data->current_slot_end_time););
  /* Check if the packet was successful. */
  if(get_channel_state(channel) == SUCCESS) {

    /* Transmission was a success. The channel is now IDLE. */
    set_channel_state(channel, IDLE);
    
    data->current_slot_end_time = now + SMALL_TIME;
    TRACE(printf("Success.\n"););

    /* Collect statistics. */
    data->number_of_packets_processed++;

    (data->stations+this_packet->station_id)->packet_count++;
    (data->stations+this_packet->station_id)->accumulated_delay +=
      now - this_packet->arrive_time;

    data->number_of_collisions += this_packet->collision_count;
    data->accumulated_delay += now - this_packet->arrive_time;

    //output_blip_to_screen(simulation_run);

    /* This packet is done. */
    free((void*) fifoqueue_get(buffer));

    /* See if there is another packet at this station. If so, enable
       it for transmission. We will transmit immediately. */
    if(fifoqueue_size(buffer) > 0) {
      next_packet = fifoqueue_see_front(buffer);

      schedule_transmission_start_event(simulation_run, now, (void*) next_packet);
    }

  } else {

    /* The transmission was unsuccessful. Clean up the channel state,
       backoff, and try again. */

    this_packet->collision_count++;
    this_packet->status = WAITING;

    TRACE(printf("Collision. Collision count = %i\n",
		 this_packet->collision_count););

    /* If the collision is over, free up the channel. */
    if(get_transmitting_stn_count(channel) == 0) {
      set_channel_state(channel, IDLE);
    }

    backoff_duration = 2.0*uniform_generator() * data->packet_backoff_duration;

    num_of_slots = round(backoff_duration / get_packet_duration());

    //because it is the last one, we already update it
   if(get_transmitting_stn_count(channel) == 0)
    {
        back_time = data->current_slot_end_time + num_of_slots * (get_packet_duration() + 2 * SMALL_TIME);
        schedule_transmission_start_event(simulation_run, back_time, (void *) this_packet);

    }
    else
    {
        back_time = data->current_slot_end_time + (num_of_slots + 1) * (get_packet_duration() + 2 * SMALL_TIME);
        schedule_transmission_start_event(simulation_run, back_time, (void *) this_packet);

    }
  }

}



