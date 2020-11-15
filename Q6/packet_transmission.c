
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

#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "trace.h"
#include "output.h"
#include "channel.h"
#include "packet_transmission.h"
#include "packet_arrival.h"

//#include "packet_duration.h"
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
  packet_duration = data->mini_slot_duration;
  current_slot_start_time = current_slot_end_time - packet_duration - SMALL_TIME * 2; 

    //TRACE(printf("start event current_slot_end_time = %f\n", current_slot_end_time););
//
  //printf("start data->reserve_mode : %d \n",data->reserve_mode);
  //printf("start data->expect_end_data_packet_duration : %f \n",data->expect_end_data_packet_duration);

  if (data->reserve_mode == 1)
  {

      if (now <= current_slot_end_time && now > current_slot_start_time + SMALL_TIME)
      {
        TRACE(printf("start event : outside slot current_slot_end_time = %f\n", current_slot_end_time););
        schedule_transmission_start_event(simulation_run, current_slot_end_time + SMALL_TIME, (void *) this_packet);

      }
      else if (now <= current_slot_start_time + SMALL_TIME && now >= current_slot_start_time)
      {
        TRACE(printf("start event : within slot current_slot_end_time = %f\n", current_slot_end_time););
        TRACE(printf("this_packet->service_time %f\n", this_packet->service_time););

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
        schedule_transmission_end_event(simulation_run,simulation_run_get_time(simulation_run) + this_packet->service_time, (void *) this_packet);
      }
      else
      {
        TRACE(printf("Unknown case \n"););
      }
  }
  else
  {
    backoff_duration = 2.0*uniform_generator() * data->packet_backoff_duration;
    TRACE(printf("backoff into non reserve mode"););
    //printf("now = %f\n", now);
    //printf("num_of_slots = %d\n", num_of_slots);

    //back_time = data->expect_end_data_packet_duration + num_of_slots * (data->mini_slot_duration + 2 * SMALL_TIME);
    //printf("back_time = %f\n", back_time);
    //
    //schedule_transmission_start_event(simulation_run, back_time + SMALL_TIME, (void *) this_packet);
    schedule_transmission_start_event(simulation_run, data->expect_end_data_packet_duration + SMALL_TIME, (void *) this_packet);
    
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

Packet_Ptr deep_copy_packet(Packet_Ptr RHS)
{
  Packet_Ptr deep_copy;
  deep_copy = (Packet_Ptr) xmalloc(sizeof(Packet));
  deep_copy-> arrive_time = RHS->arrive_time;
  deep_copy-> service_time = RHS->service_time;
  deep_copy-> X = RHS->X;
  deep_copy-> status = RHS->status;
  deep_copy-> collision_count = RHS->collision_count;
  deep_copy-> station_id = RHS->station_id;
    
  return deep_copy;

}

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

  if (data->reserve_mode == 0)
  {
    schedule_transmission_start_event(simulation_run, data->expect_end_data_packet_duration + SMALL_TIME, (void *) this_packet);

  }
  //printf("data->reserve_mode : %d \n",data->reserve_mode);

  /* Check if the packet was successful. */
  if(get_channel_state(channel) == SUCCESS) {

    /* Transmission was a success. The channel is now IDLE. */
    set_channel_state(channel, IDLE);
    
    TRACE(printf("Success in mini slot.\n"););
    //printf("Success in mini slot.\n");

    /* Collect statistics. */
    //TODO data->number_of_packets_processed++;

    //TODO (data->stations+this_packet->station_id)->packet_count++;
    // TODO (data->stations+this_packet->station_id)->accumulated_delay += now - this_packet->arrive_time;

    data->number_of_collisions += this_packet->collision_count;
    // TODO data->accumulated_delay += now - this_packet->arrive_time;

    //output_blip_to_screen(simulation_run);

    /* This packet is done. */

    Packet_Ptr deep_copy_put_into_data_fifo;
    deep_copy_put_into_data_fifo = deep_copy_packet(this_packet);

    //put mini slot successful packet into data_fifo
    fifoqueue_put(data->data_fifo, (void *) deep_copy_put_into_data_fifo);
    //calc expect_end_data_packet_duration

    Packet_Ptr tmp;
    //tmp = fifoqueue_see_front(data->data_fifo);
    //printf("data_fifo tmp->arrive_time: %f \n",tmp->arrive_time);
//
    //tmp = fifoqueue_see_front(buffer);
    //printf("buffer fifoqueue_size %d\n",fifoqueue_size(buffer));
    //printf("buffer tmp->arrive_time: %f \n",tmp->arrive_time);

    //printf(" \n");
    free((void*) fifoqueue_get(buffer));

    data->expect_end_data_packet_duration += deep_copy_put_into_data_fifo->X;
    //printf("end data->expect_end_data_packet_duration : %f \n",data->expect_end_data_packet_duration);

    /* See if there is another packet at this station. If so, enable
       it for transmission. We will transmit immediately. */
    if(fifoqueue_size(buffer) > 0) {
      next_packet = fifoqueue_see_front(buffer);

      schedule_transmission_start_event(simulation_run, data->current_slot_end_time+SMALL_TIME, (void*) next_packet);
    }
  } else {

    /* The transmission was unsuccessful. Clean up the channel state,
       backoff, and try again. */

    this_packet->collision_count++;
    this_packet->status = WAITING;

    TRACE(printf("Collision. Collision count = %i\n", this_packet->collision_count););
    //printf("Collision. Collision count = %i\n", this_packet->collision_count);

    /* If the collision is over, free up the channel. */
    if(get_transmitting_stn_count(channel) == 0) {
      set_channel_state(channel, IDLE);
    }

    backoff_duration = 2.0*uniform_generator() * data->packet_backoff_duration;
    num_of_slots = round(backoff_duration / data->mini_slot_duration); 

    TRACE(printf("num_of_slots = %d\n", num_of_slots););
    //printf("now = %f\n", now);
    //printf("num_of_slots = %d\n", num_of_slots);

        back_time = data->current_slot_end_time + num_of_slots * (data->mini_slot_duration + 2 * SMALL_TIME);
    //printf("back_time = %f\n", back_time);
        schedule_transmission_start_event(simulation_run, back_time, (void *) this_packet);

    }

}

long int
schedule_end_data_packet_event(Simulation_Run_Ptr simulation_run, Time event_time, void * packet)
{
  Event event;

  event.description = "data_packet";
  event.function = end_data_packet_event;
  event.attachment = packet;

  return simulation_run_schedule_event(simulation_run, event, event_time);
}

void
end_data_packet_event(Simulation_Run_Ptr simulation_run, void * packet)
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
  //buffer = (data->stations+this_packet->station_id)->buffer;


#ifndef ASSERT_OFF
    assert(data->reserve_mode == 0);
    assert(get_channel_state(channel) == IDLE); 
#endif

    /* Collect statistics. */
    data->number_of_packets_processed++;

    (data->stations+this_packet->station_id)->packet_count++;
    (data->stations+this_packet->station_id)->accumulated_delay += now - this_packet->arrive_time;

    data->number_of_collisions += this_packet->collision_count;
    data->accumulated_delay += now - this_packet->arrive_time;

    output_blip_to_screen(simulation_run);

    //printf("number_of_packets_processed : %d \n",data->number_of_packets_processed);

    /* This packet is done. */
    free((void*) fifoqueue_get(data->data_fifo));

    if(fifoqueue_size(data->data_fifo) > 0) {
      next_packet = fifoqueue_see_front(data->data_fifo);

      schedule_end_data_packet_event(simulation_run, now + next_packet->X, next_packet);
    }
    else
    {
      //printf("at %f switch back to reserve mode\n", now);
      data->reserve_mode = 1;
#ifndef ASSERT_OFF
      //printf("at %f expect_end_data_packet_duration: %f \n", now,data->expect_end_data_packet_duration);
      //assert(now == data->expect_end_data_packet_duration);
#endif
      data->expect_end_data_packet_duration = 0;

      schedule_slot_event(simulation_run, now);
    }
}

