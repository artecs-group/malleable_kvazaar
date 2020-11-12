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

#include "global.h"
#include "stats.h"

#include <errno.h> // ETIMEDOUT
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#include "threads.h"
#include "threadqueue.h"


struct threadqueue_queue_t {
    pthread_mutex_t lock;

    /**
     * \brief Job available condition variable
     *
     * Signalled when there is a new job to do.
     */
    pthread_cond_t job_available;

    /**
     * \brief Job done condition variable
     *
     * Signalled when a job has been completed.
     */
    pthread_cond_t job_done;

    /**
     * Array containing spawned threads
     */
    pthread_t *threads;

    /**
     * \brief Number of threads spawned
     */
    int thread_count;

    /**
     * \brief Number of threads running
     */
    int thread_running_count;

    /**
     * \brief If true, threads should stop ASAP.
     */
    bool stop;

    /**
     * \brief Pointer to the first ready job
     */
    threadqueue_job_t *first;

    /**
     * \brief Pointer to the last ready job
     */
    threadqueue_job_t *last;

    /**
     * \brief Number of active threads
     */
    int thread_active;

    /**
     * \brief Maximum Number of active threads
     */
    int thread_max_active;
};


#define PTHREAD_LOCK(l)							\
    if (pthread_mutex_lock((l)) != 0) {					\
	fprintf(stderr, "pthread_mutex_lock(%s) failed!\n", #l);	\
	assert(0);							\
    }

#define PTHREAD_UNLOCK(l)						\
    if (pthread_mutex_unlock((l)) != 0) {				\
	fprintf(stderr, "pthread_mutex_unlock(%s) failed!\n", #l);	\
	assert(0);							\
    }


/**
 * \brief Initialize the statistical module.
 *
 * \return stats_t on success, 0 on failure
 */
stats_t * kvz_stats_init(threadqueue_queue_t* threadqueue)
{
    stats_t * stats = (stats_t*) MALLOC(stats_t, 1);
    if(!stats) {
	goto failed;
    }

    if (pthread_mutex_init(&stats->lock, NULL) != 0) {
	fprintf(stderr, "pthread_mutex_init failed!\n");
	goto failed;
    }

    char* txt = getenv ("NUM_FRAMES_PER_BATCH");
    stats->batchSize = (txt==NULL) ? 24 : atoi (txt);

    stats->framesDone = 0;
    stats->nThs = 0;
    
    KVZ_GET_TIME(&stats->encoding_start_real_time);
    
    return stats;
    
failed:
    kvz_stats_free(stats);
    return NULL;
}


/**
 * \brief Free allocated resources for statistical module, and prints overall information
 *
 * \return 1 on success, 0 on failure
 */
void kvz_stats_free(stats_t *stats)
{
    if (stats == NULL) return;

    if (pthread_mutex_destroy(&stats->lock) != 0) {
	fprintf(stderr, "pthread_mutex_destroy failed!\n");
    }

    /* if(stats->framesDone % stats->batchSize != 0)	 */
    /* 	helper_printBatchInfo(stats); */
    
    fprintf (stdout, "END\n");
    fflush (stdout);
    
    helper_printProcessInfo(stats);
    
    FREE_POINTER(stats);
}


void stats_start_frame (stats_t *stats, KVZ_CLOCK_T timestamp){
    if(stats->framesDone==0 || stats->framesDone % stats->batchSize==0)
    	stats->encoding_start_last_batch = timestamp;
}


void stats_end_frame (stats_t *stats, KVZ_CLOCK_T timestamp){
    stats->framesDone++;
    const int frames_done = stats->framesDone;
    
    if( (frames_done >= stats->batchSize) && (frames_done%stats->batchSize == 0) )
	helper_printBatchInfo (stats, timestamp);
}


void stats_start_encoding (stats_t *stats, KVZ_CLOCK_T timestamp){
    stats->encoding_start_real_time = timestamp;
}

void stats_end_encoding (stats_t *stats, KVZ_CLOCK_T timestamp){
    stats->encoding_end_real_time = timestamp;
}
   

/*@ ------ @*/

int helper_readNumThreads(stats_t *stats){

    if(stats->framesDone!=0 && stats->framesDone % stats->batchSize != 0)
        return stats->nThs;

    int nThs=-1;
    int ret;
    if((ret=fscanf(stdin, "nThs:%d%*c", &nThs))!=1){
	perror ("Error reading N. threads. Wrong format\n");
	return stats->nThs;
    } else{
      stats->nThs = nThs;
      return nThs;
    }
}


int helper_readQP(){
    int QP=-1;
    int ret=0;

    while((ret=fscanf(stdin, "nThs:%d\n", &QP))!=1){
	perror ("Error reading QP value. Wrong format\n");
    }
    
    return QP;
}

void helper_printBatchInfo(stats_t *stats, KVZ_CLOCK_T timestamp){


    long long time =
	KVZ_CLOCK_T_NS_AS_LONG_LONG(timestamp) - 
	KVZ_CLOCK_T_NS_AS_LONG_LONG(stats->encoding_start_last_batch);

  
    int nFrames = (stats->framesDone % stats->batchSize);
    if(nFrames == 0) nFrames=stats->batchSize;

    long double fps = (nFrames * 1.0) / (time*1e-9);
    
    fprintf (stdout, "FPS:%Lf\n", fps);
    fflush (stdout);
}



void helper_printProcessInfo(stats_t *stats){
    long long time =
	KVZ_CLOCK_T_NS_AS_LONG_LONG(stats->encoding_end_real_time) - 
	KVZ_CLOCK_T_NS_AS_LONG_LONG(stats->encoding_start_real_time);
    
    long double fps = (stats->framesDone * 1.0) / (time*1e-9);
    
    fprintf (stdout, "Overall_FPS:%Lf\n", fps);
    fflush (stdout);
}
