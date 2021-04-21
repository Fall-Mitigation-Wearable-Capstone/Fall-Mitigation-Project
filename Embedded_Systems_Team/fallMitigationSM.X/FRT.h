#ifndef FRT_H
#define FRT_H


/**
Function: FRT_Init
param: none
return: none
brief: intializes timer 5 and its interrupt
**/
void FRT_Init(void);

/**
Function: FRT_GetMicroSeconds
param: none
return: unsigned int
brief: returns the current count for microseconds passed since initialization
**/
unsigned int FRT_GetMicroSeconds(void);

/**
Function: FRT_GetMilliSeconds
param: none
return: unsigned int
brief: returns the current count for milliseconds passed since initialization
**/
unsigned int FRT_GetMilliSeconds(void);


#endif 