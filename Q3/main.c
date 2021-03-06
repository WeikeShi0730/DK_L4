
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

int main(void)
{
  /* Get the list of random number generator seeds defined in simparameters.h */
  unsigned random_seed;
  unsigned RANDOM_SEEDS[] = {RANDOM_SEED_LIST, 0};
  unsigned int NUMBER_OF_STATIONS_LIST[] = {NUMBER_OF_STATIONS};
  double PACKET_ARRIVAL_RATE_LIST[] = {PACKET_ARRIVAL_RATE};
  int size_rand_seed = (sizeof(RANDOM_SEEDS) / sizeof(unsigned)) - 1;

  Simulation_Run_Ptr simulation_run;
  Simulation_Run_Data data;
  Simulation_Run_Data for_avg_acc;

  // create a csv file
  FILE *fp;
  char data_set_name[] = "./Q3.csv";
  //file IO

  fp = fopen(data_set_name, "w");
  //cell/element name/type

  fprintf(fp, ("Number of Stations, "));
  fprintf(fp, ("Mean Packet Duration, "));
  fprintf(fp, ("Packet Arrival Rate, "));
  fprintf(fp, ("Attempts, "));
  fprintf(fp, ("G, "));
  fprintf(fp, ("Number of collision, "));
  fprintf(fp, ("Throughput, "));
  fprintf(fp, ("theo_throughput, "));
  fprintf(fp, ("Mean Delay, "));
  fprintf(fp, ("sim time, "));

  fprintf(fp, "\n");
  fclose(fp);
  for (int l = 0; l < (sizeof(NUMBER_OF_STATIONS_LIST) / sizeof(int)); l++)
  {
    for (int k = 0; k < (sizeof(PACKET_ARRIVAL_RATE_LIST) / sizeof(double)); k++)
    {
      /* Initialize various average simulation_run variables. */
      for_avg_acc.arrival_count = 0;
      for_avg_acc.blip_counter = 0;
      for_avg_acc.number_of_packets_processed = 0;
      for_avg_acc.number_of_collisions = 0;
      for_avg_acc.accumulated_delay = 0;
      for_avg_acc.init_time = 0;
      for_avg_acc.end_time = 0;
      for_avg_acc.tpt = 0;
      for_avg_acc.g = 0;
      for_avg_acc.sim_time = 0;
      for_avg_acc.attempt_count = 0;
      
      int j = 0;
      /* Do a new simulation_run for each random number generator seed. */
      while ((random_seed = RANDOM_SEEDS[j++]) != 0)
      {
        /* Set the random generator seed. */
        random_generator_initialize(random_seed);

        /* Create a new simulation_run. This gives a clock and
          eventlist. Clock time is set to zero. */
        simulation_run = (Simulation_Run_Ptr)simulation_run_new();

        /* Add our data definitions to the simulation_run. */
        simulation_run_set_data(simulation_run, (void *)&data);

        /* Initialize various simulation_run variables. */
        data.attempt_count = 0;
        data.arrival_count = 0;
        data.sim_time = 0;
        data.blip_counter = 0;
        data.number_of_packets_processed = 0;
        data.number_of_collisions = 0;
        data.accumulated_delay = 0;
        data.arrival_rate = PACKET_ARRIVAL_RATE_LIST[k];
        data.number_of_stations = NUMBER_OF_STATIONS_LIST[l];
        data.init_time = 0;
        data.end_time = 0;
        data.tpt = 0;
        data.g = 0;
        data.random_seed = random_seed;

        /* Create and initalize the stations. */
        data.stations = (Station_Ptr)xcalloc((unsigned int)NUMBER_OF_STATIONS_LIST[l],
                                             sizeof(Station));

        assert(NUMBER_OF_STATIONS_LIST[l] != 1);
        /* Initialize the stations. */
        for (int i = 0; i < NUMBER_OF_STATIONS_LIST[l]; i++)
        {
          (data.stations + i)->id = i;
          (data.stations + i)->buffer = fifoqueue_new();
          (data.stations + i)->packet_count = 0;
          (data.stations + i)->accumulated_delay = 0.0;
          (data.stations + i)->mean_delay = 0;
        }

        /* Create and initialize the channel. */
        data.channel = channel_new();

        /* Schedule initial packet arrival. */
        schedule_packet_arrival_event(simulation_run,
                                      simulation_run_get_time(simulation_run) +
                                          exponential_generator((double)1 / PACKET_ARRIVAL_RATE_LIST[k]));

        /* Execute events until we are finished. */
        while (data.number_of_packets_processed < RUNLENGTH)
        {
          simulation_run_execute_event(simulation_run);
        }
        data.sim_time = data.end_time - data.init_time;
        data.tpt = (double)data.number_of_packets_processed / (data.sim_time);
        //data.g = (double)(data.arrival_count + data.number_of_collisions) / (data.sim_time);
        data.g = (double)(data.attempt_count) / (data.end_time - data.init_time);

        for_avg_acc.sim_time += data.sim_time;
        for_avg_acc.arrival_count += data.arrival_count;
        for_avg_acc.number_of_packets_processed += data.number_of_packets_processed;
        for_avg_acc.number_of_collisions += data.number_of_collisions;
        for_avg_acc.accumulated_delay += data.accumulated_delay;
        for_avg_acc.tpt += data.tpt;
        for_avg_acc.g += data.g;
        for_avg_acc.attempt_count += data.attempt_count;

        printf("seed = %d \n", data.random_seed);
        printf("start time = %f\n", data.init_time);
        printf("end time = %f\n", data.end_time);
        printf("attempt_count = %d \n", data.attempt_count);
        printf("collisions = %d \n", data.number_of_collisions);
        printf("Throughput = %f \n", data.tpt);
        printf("G = %f \n\n", data.g);
        printf("sim_time = %f \n\n", data.end_time);
        /* Print out some results. */
        output_results(simulation_run);

        /* Clean up memory. */
        cleanup(simulation_run);
      }
      for_avg_acc.sim_time /= size_rand_seed;
      for_avg_acc.arrival_count /= size_rand_seed;
      for_avg_acc.number_of_packets_processed /= size_rand_seed;
      for_avg_acc.number_of_collisions /= size_rand_seed;
      for_avg_acc.accumulated_delay /= size_rand_seed;
      for_avg_acc.init_time /= size_rand_seed;
      for_avg_acc.end_time /= size_rand_seed;
      for_avg_acc.tpt /= size_rand_seed;
      for_avg_acc.g /= size_rand_seed;
      for_avg_acc.attempt_count /= size_rand_seed;
      fp = fopen(data_set_name, "a");
      //cell/element name/type

      //fprintf(fp, ("Number of Stations"));
      fprintf(fp, "%d, ", NUMBER_OF_STATIONS_LIST[l]);

      //fprintf(fp, ("Mean Packet Duration"));
      fprintf(fp, "%d, ", MEAN_PACKET_DURATION);

      //fprintf(fp, ("Packet Arrival Rate"));
      fprintf(fp, "%f, ", PACKET_ARRIVAL_RATE_LIST[k]);
      
      //fprintf(fp, ("Attempts, "));
      fprintf(fp, "%d, ", for_avg_acc.attempt_count);

      //fprintf(fp, ("G"));
      fprintf(fp, "%f, ", for_avg_acc.g);

      //fprintf(fp, ("Number of collision"));
      fprintf(fp, "%d, ", for_avg_acc.number_of_collisions);

      //fprintf(fp, ("Throughput, "));
      fprintf(fp, "%f, ", for_avg_acc.tpt);

      //fprintf(fp, ("theo_throughput, "));
      fprintf(fp, "%f, ", for_avg_acc.g * exp(-2 * for_avg_acc.g));

      //fprintf(fp, ("Mean Delay, "));
      fprintf(fp, "%f, ", (double)for_avg_acc.accumulated_delay / for_avg_acc.number_of_packets_processed);

      //fprintf(fp, ("sim time, "));
      fprintf(fp, "%f, ", for_avg_acc.sim_time);

      fprintf(fp, "\n");
      fclose(fp);

      printf("\n");
      printf("Random Seed = %d \n", random_seed);
      printf("Number of Stations = %d \n", NUMBER_OF_STATIONS_LIST[l]);
      printf("Packet Arrival Rate = %f \n", PACKET_ARRIVAL_RATE_LIST[k]);
      printf("G = %f \n", for_avg_acc.g);
      printf("Attempts = %d \n", for_avg_acc.attempt_count);
      printf("Packet duration = %d \n", MEAN_PACKET_DURATION);
      printf("Throughput = %f \n", for_avg_acc.tpt);
      printf("theo_throughput= %f \n", for_avg_acc.g * exp(-2 * for_avg_acc.g));
      printf("number of arrivals = %ld \n", for_avg_acc.arrival_count);
      printf("number of collision = %d \n", for_avg_acc.number_of_collisions);
      printf("Mean Delay = %f \n", (double)for_avg_acc.accumulated_delay / for_avg_acc.number_of_packets_processed);
      printf("\n");
    }
  }
  return 0;
}
