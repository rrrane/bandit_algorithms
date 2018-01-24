
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <time.h>


#include "rlglue/Agent_common.h" /* Required for RL-Glue */

static gsl_vector* local_action;
static action_t* this_action;
static gsl_vector* last_observation;

static int numActions = 10;
static int numStates = 1;

static double action_rewards[10];
static int action_count[10];
static int previous_action;

void agent_init()
{
  //Allocate Memory
  local_action = gsl_vector_calloc(1);
  this_action = local_action;
  last_observation = gsl_vector_calloc(1);
}

const action_t *agent_start(const observation_t *this_observation) {
 	
  for(int i = 0; i < 10; i++)
  {
	  action_rewards[i] = 0.0;
	  action_count[i] = 0;
  }

  int stp1 = (int)gsl_vector_get(this_observation,0); /* how you convert observation to a int, if state is tabular */
  int atp1=randInRange(numActions);
	++action_count[atp1];
  previous_action = atp1;

  gsl_vector_set(local_action,0,atp1);
  
  gsl_vector_memcpy(last_observation,this_observation);/*save observation, might be useful on the next step */
  
  return this_action;
}

const action_t *agent_step(double reward, const observation_t *this_observation) {
  

  	int stp1 = (int)gsl_vector_get(this_observation,0);
  	int atp1 = randInRange(numActions);
  	int maxindex = 0;

	int randVar = randInRange(1000);



	action_rewards[previous_action] = action_rewards[previous_action] + (reward - action_rewards[previous_action])*0.1;

  	for(int i = 0; i < 10; i++)
	{
		if(action_rewards[i] > action_rewards[maxindex])
			maxindex = i;
	}
	
	int atp2 = maxindex;

	for(int i = 0; i < 10; i++)
	{
		if(action_rewards[i] == action_rewards[maxindex] && randInRange(2) == 0)
		{
			atp2 = i;
			break;
		}
	}
	
	//epsilon = 0.1
	if(randVar >=0 && randVar <= 99)
		atp2 = atp1;

	//printf("Random Action = %d RandVar = %d MaxAction = %d\n", atp1, randVar, atp2);
	//printf("[%d-%f] ", previous_action, action_rewards[previous_action]);
  	//update cumulative rewards
  	++action_count[atp2];
  	
  	//select new action

	previous_action = atp2;

  	/* might do some learning here */
  
  	gsl_vector_set(local_action,0,atp2);
  	gsl_vector_memcpy(last_observation,this_observation);
  
  	return this_action;
}


void agent_end(double reward) {
  /* final learning update at end of episode */
}

void agent_cleanup() {
  /* clean up mememory */
  gsl_vector_free(local_action);
  gsl_vector_free(last_observation);
}

const char* agent_message(const char* inMessage) {
  /* might be useful to get information from the agent */
  if(strcmp(inMessage,"what is your name?")==0)
  return "my name is skeleton_agent!";
  
  /* else */
  return "I don't know how to respond to your message";
}
