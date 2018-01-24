

#include <stdio.h>
#include <math.h>
#include <string.h>
#include "rlglue/RL_glue.h" /* Required for RL-Glue  */

extern int is_optimal_action_taken;

void saveResults(double* data, int dataSize, const char* filename);

int main(int argc, char *argv[]) {

	const reward_observation_action_terminal_t *rl_step_result = 0;

  	int k,i;
  	int numEpisodes = 1;
  	int maxStepsInEpisodes = 1000;
  	int numRuns = 2000;
  	//int isTerminal = 0;
  	double result[maxStepsInEpisodes];
	double optimal[maxStepsInEpisodes];
  	memset(result, 0, sizeof(result));
 	
       	for(i = 0; i < maxStepsInEpisodes; i++){
		result[i] = 0.0;
		optimal[i] = 0.0;
	}
		
  	printf("\nPrinting one dot for every run: %d total Runs to complete\n",numRuns);
  	for (k =0; k<numRuns; k++)
  	{
    		RL_init();

      		RL_start();
		result[0] += RL_return();
		//printf("returned\n");
      		/*RL_start sets steps to 1*/
      		for (i=1; i < maxStepsInEpisodes; i++ ) {
         		rl_step_result = RL_step();
	 		result[i] += RL_return();

			if(is_optimal_action_taken == 1)
				optimal[i]++;
      		}
		
		RL_cleanup();
    		printf(".");
    		fflush( stdout );
  	}
  
  	printf("\nDone\n");
  
  	/* average over runs */
  	for (i =0; i<maxStepsInEpisodes; i++){
    		result[i] = result[i]/(double)numRuns;
		result[i] = result[i]/(double)(i+1);
		optimal[i] = 100 * (optimal[i]/(double)(numRuns));
  	}
  
  	/* Save data to a file */
  	saveResults(result, maxStepsInEpisodes , "RL_EXP_OUT.dat");
	saveResults(optimal, maxStepsInEpisodes, "RL_OPT_ACT.dat");
  
  	return 0;
}

void saveResults(double* data, int dataSize, const char* filename) {
  FILE *dataFile;
  int i;
  dataFile = fopen(filename, "w");
  for(i = 0; i < dataSize; i++){
    fprintf(dataFile, "%lf\n", data[i]);
  }
  fclose(dataFile);
}
