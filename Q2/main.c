
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

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "output.h"
#include "trace.h"
#include "simparameters.h"
#include "cleanup.h"
#include "packet_arrival.h"
#include "packet_transmission.h"
#include "main.h"

/*******************************************************************************/

int
main(void)
{
  /* Get the list of random number generator seeds defined in simparameters.h */
  unsigned random_seed;
  unsigned RANDOM_SEEDS[] = {RANDOM_SEED_LIST, 0};
  double MEAN_BACKOFF_DURATION_LIST[] = {MEAN_BACKOFF_DURATION};
  unsigned int NUMBER_OF_STATIONS_LIST[] = {NUMBER_OF_STATIONS};
  double PACKET_ARRIVAL_RATE_LIST[] = {PACKET_ARRIVAL_RATE};
  int size_rand_seed = (sizeof(RANDOM_SEEDS) / sizeof(unsigned)) - 1;

  Simulation_Run_Ptr simulation_run;
  Simulation_Run_Data data;
  Simulation_Run_Data for_avg_acc;

  // create a csv file
  FILE *fp;
  char data_set_name[] = "./Q2.csv";
  //file IO

  fp = fopen(data_set_name, "w");
  //cell/element name/type

  fprintf(fp, ("Number of Stations, "));
  fprintf(fp, ("Mean Packet Duration, "));
  fprintf(fp, ("Packet Arrival Rate, "));
  fprintf(fp, ("Mean Backoff duration, "));
  fprintf(fp, ("Throughput, "));
  fprintf(fp, ("Mean Delay, "));

  fprintf(fp, "\n");
  fclose(fp);
  for (int l = 0; l < (sizeof(MEAN_BACKOFF_DURATION_LIST) / sizeof(double)); l++)
  {
    for (int k = 0; k < (sizeof(PACKET_ARRIVAL_RATE_LIST) / sizeof(double)); k++)
    {
      int j = 0;
      /* Do a new simulation_run for each random number generator seed. */
      while ((random_seed = RANDOM_SEEDS[j++]) != 0) {

        /* Set the random generator seed. */
        random_generator_initialize(random_seed);

        /* Create a new simulation_run. This gives a clock and
          eventlist. Clock time is set to zero. */
        simulation_run = (Simulation_Run_Ptr) simulation_run_new();

        /* Add our data definitions to the simulation_run. */
        simulation_run_set_data(simulation_run, (void *) & data);

        /* Create and initalize the stations. */
        data.stations = (Station_Ptr) xcalloc((unsigned int) NUMBER_OF_STATIONS_LIST[l],
                sizeof(Station));

        /* Initialize various simulation_run variables. */
        data.arrival_count = 0;
        data.blip_counter = 0;
        data.number_of_packets_processed = 0;
        data.number_of_collisions = 0;
        data.accumulated_delay = 0;
        data.arrival_rate = PACKET_ARRIVAL_RATE_LIST[k];
        data.packet_backoff_duration = MEAN_BACKOFF_DURATION_LIST[l];
        data.number_of_stations = NUMBER_OF_STATIONS_LIST[l];
        data.init_time = 0;
        data.end_time = 0;
        data.random_seed = random_seed;
        
        /* Initialize the stations. */
        for(int i=0; i<NUMBER_OF_STATIONS_LIST[l]; i++) {
          (data.stations+i)->id = i;
          (data.stations+i)->buffer = fifoqueue_new();
          (data.stations+i)->packet_count = 0;
          (data.stations+i)->accumulated_delay = 0.0;
          (data.stations+i)->mean_delay = 0;
        }

        /* Create and initialize the channel. */
        data.channel = channel_new();

        /* Schedule initial packet arrival. */
        schedule_packet_arrival_event(simulation_run, 
            simulation_run_get_time(simulation_run) +
            exponential_generator((double) 1/PACKET_ARRIVAL_RATE_LIST[k]));

        /* Execute events until we are finished. */
        while(data.number_of_packets_processed < RUNLENGTH) {
          simulation_run_execute_event(simulation_run);
        }
        for_avg_acc.arrival_count += data.arrival_count;
        for_avg_acc.number_of_packets_processed += data.number_of_packets_processed;
        for_avg_acc.number_of_collisions += data.number_of_collisions;
        for_avg_acc.accumulated_delay += data.accumulated_delay;
        for_avg_acc.init_time += data.init_time;
        for_avg_acc.end_time += data.end_time;

        /* Print out some results. */
        //output_results(simulation_run);

        /* Clean up memory. */
        cleanup(simulation_run);
      }
      for_avg_acc.arrival_count /= size_rand_seed;
      for_avg_acc.number_of_packets_processed /= size_rand_seed;
      for_avg_acc.number_of_collisions /= size_rand_seed;
      for_avg_acc.accumulated_delay /= size_rand_seed;
      for_avg_acc.init_time /= size_rand_seed;
      for_avg_acc.end_time /= size_rand_seed;
      fp = fopen(data_set_name, "a");
      //cell/element name/type

      //fprintf(fp, ("Number of Stations"));
      fprintf(fp, "%d, ", NUMBER_OF_STATIONS_LIST[l]);

      //fprintf(fp, ("Mean Packet Duration"));
      fprintf(fp, "%d, ", MEAN_PACKET_DURATION);

      //fprintf(fp, ("Packet Arrival Rate"));
      fprintf(fp, "%f, ", PACKET_ARRIVAL_RATE_LIST[k]);

      //fprintf(fp, ("Mean Backoff duration"));
      fprintf(fp, "%f, ", MEAN_BACKOFF_DURATION_LIST[l]);
      
      //fprintf(fp, ("Throughput, "));
      fprintf(fp, "%f, ", (double)for_avg_acc.number_of_packets_processed / (for_avg_acc.end_time - for_avg_acc.init_time));

      //fprintf(fp, ("Mean Delay, "));
      fprintf(fp, "%f, ", (double)for_avg_acc.accumulated_delay / for_avg_acc.number_of_packets_processed);

      fprintf(fp, "\n");
      fclose(fp);

      printf("\n");
      printf("Random Seed = %d \n", random_seed);
      printf("Number of Stations = %d \n", NUMBER_OF_STATIONS_LIST[l]);
      printf("Packet Arrival Rate = %f \n", PACKET_ARRIVAL_RATE_LIST[k]);
      printf("Packet duration = %d \n", MEAN_PACKET_DURATION);
      printf("Mean Backoff duration = %f \n", MEAN_BACKOFF_DURATION_LIST[l]);
      printf("Throughput = %f \n", for_avg_acc.number_of_packets_processed / (for_avg_acc.end_time - for_avg_acc.init_time));
      printf("Mean Delay = %f \n", (double)for_avg_acc.accumulated_delay / for_avg_acc.number_of_packets_processed);
      printf("\n");
    }
  }
  return 0;
}












