
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "rlglue/Environment_common.h" /* Required for RL-Glue interface*/
#include "utils.h"



#define MAX_STEP_COUNT 1000

static gsl_vector* local_observation;
static reward_observation_terminal_t this_reward_observation;
static const int nStatesSimpleEnv = 1;
static double mu[10] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
static double sigma[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

static int stepcount = 0;

static int optimal_action = 0;

extern int is_optimal_action_taken = 0;

void env_init() 
{
	local_observation = gsl_vector_calloc(1);
	
	this_reward_observation.observation=local_observation;
	this_reward_observation.reward=0;
	this_reward_observation.terminal=0;

	return;
}

const observation_t* env_start()
{
  	gsl_vector_set(local_observation,0,0);
	
	for(int i=0; i<10; i++){
		double val = (double)randInRange(3);
		double frac = rand_un();
		val+=frac;
		if(randInRange(2)==1)
			val*=-1;
		mu[i] = val;
	}
	
	int maxindex = 0;
	for(int i = 0; i <10; i++){
		if(mu[i] > mu[maxindex])
			maxindex = i;
	}
	optimal_action = maxindex;


  	return this_reward_observation.observation;
}

const reward_observation_terminal_t *env_step(const action_t *this_action)
{
  	int episode_over = 0;
  	int act = (int)gsl_vector_get(this_action, 0);
  	double the_reward =  randn(mu[act], sigma[act]); /* rewards drawn from 0,1 gaussian */
  	//printf("%d-%f ", act, the_reward);
  	//int atp1 = gsl_vector_get(this_action,0); /* how to extact action */

  	int stp1 =randInRange(nStatesSimpleEnv); /* state transitions are uniform random */

	for(int i=0; i<10; i++){
		double val = (double)randInRange(3);
		double frac = rand_un();
		val+=frac;
		if(randInRange(2)==1)
			val*=-1;
		mu[i] = val;
	}
	
	int maxindex = 0;
	for(int i = 0; i <10; i++){
		if(mu[i] > mu[maxindex])
			maxindex = i;
	}
	optimal_action = maxindex;

  
  	if(++stepcount >= MAX_STEP_COUNT)
	  	episode_over = 1;
	
	if(act == optimal_action)
		is_optimal_action_taken = 1;
	else
		is_optimal_action_taken = 0;

  	gsl_vector_set(local_observation, 0, stp1);
  	this_reward_observation.reward = the_reward;
  	this_reward_observation.terminal = episode_over;
  
  return &this_reward_observation;
}


void env_cleanup()
{
  gsl_vector_free(local_observation);
}

const char* env_message(const char* inMessage) 
{
  if(strcmp(inMessage,"what is your name?")==0)
  return "my name is skeleton_environment!";
  
  /* else */
  return "I don't know how to respond to your message";
}
