#ifndef STATS_H_
#define STATS_H_
/*****************************************************************************
 * This file is part of Kvazaar HEVC encoder.
 *
 * Copyright (C) 2013-2015 Tampere University of Technology and others (see
 * COPYING file).
 *
 * Kvazaar is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation; either version 2.1 of the License, or (at your
 * option) any later version.
 *
 * Kvazaar is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with Kvazaar.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/

/**
 * \ingroup Threading
 * \file
 * Container for statistical timing information.
 */

#include "global.h" // IWYU pragma: keep

#include "threads.h"

#include <pthread.h>

typedef struct stats_t {

    pthread_mutex_t lock;

    KVZ_CLOCK_T encoding_start_real_time;
    KVZ_CLOCK_T encoding_end_real_time;

    KVZ_CLOCK_T encoding_start_last_batch;

    
    int batchSize;
    int framesDone;

    int nThs;
} stats_t;

typedef struct threadqueue_queue_t threadqueue_queue_t;


stats_t * kvz_stats_init(threadqueue_queue_t* threadqueue);
void kvz_stats_free(stats_t *stats);

void stats_start_encoding (stats_t *stats, KVZ_CLOCK_T timestamp);
void stats_end_encoding (stats_t *stats, KVZ_CLOCK_T timestamp);

void stats_start_frame (stats_t *stats, KVZ_CLOCK_T timestamp);
void stats_end_frame (stats_t *stats, KVZ_CLOCK_T timestamp);

/*@ ------ @*/
int helper_readNumThreads(stats_t *stats);
int helper_readQP();

//Prints the information of the last batch encoded
void helper_printBatchInfo(stats_t *stats, KVZ_CLOCK_T endTimestamp);

//Prints the information of the whole encoding process
void helper_printProcessInfo(stats_t *stats);




#endif // STATS_H_
