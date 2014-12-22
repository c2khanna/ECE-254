/*----------------------------------------------------------------------------
 *      ECE254 Lab3
 *----------------------------------------------------------------------------
 *      Name:    RT_TASK_ext.C
 *      Purpose: Interface functions for extended task management  
 *      Rev.:    V4.60
 *----------------------------------------------------------------------------
 *      This code extends the RealView Run-Time Library.
 *      Created by University of Waterloo ECE254 Lab Staff.
 *---------------------------------------------------------------------------*/

#include "rt_TypeDef.h"
#include "RTX_Config.h"
#include "rt_System.h"
#include "rt_Task.h"
#include "rt_List.h"
#include "rt_MemBox.h"
#include "rt_Robin.h"
#include "rt_HAL_CM.h"
#include "rt_Task_ext.h"

/*----------------------------------------------------------------------------
 *      Global Variables
 *---------------------------------------------------------------------------*/

P_TCB p_tcb;
/*----------------------------------------------------------------------------
 *      Local Functions
 *---------------------------------------------------------------------------*/
/*--------------------------- rt_tsk_count_get ------------------------------*/
/* added in ECE254 lab2 keil_rtx */
int rt_tsk_count_get (void) {
		int Counter = 0; 
	int n;
	for( n = 0; n < 10; n++)
	{
		if(os_active_TCB[n]!= NULL)
		{
			p_tcb = os_active_TCB[n];
			if (p_tcb ->state != 0)
			{
				Counter++; 
			}
		}

	}
	/* Add your own code here. Change the following line accordingly */
	return 0;

}

/*--------------------------- rt_tsk_get ------------------------------------*/
/* added in ECE254 lab3 keil_proc */
OS_RESULT rt_tsk_get(OS_TID task_id, RL_TASK_INFO *p_task_info) 
{
/*	int n;
	for( n = 0; n < 10; n++)
	{
		if(os_active_TCB[n]!= NULL){*/
		
	if(task_id == os_idle_TCB.task_id)
	{
		p_tcb = &os_idle_TCB;
	}
	else
	{
			p_tcb = os_active_TCB[task_id-1];
	}
			if(p_tcb->state <= 10) {
				
					p_task_info->task_id     = p_tcb->task_id;
					p_task_info->state       = p_tcb->state ;
					p_task_info->prio        = p_tcb->prio;
					p_task_info->ptask       = p_tcb->ptask;
				
					if(p_tcb->priv_stack != 0){
						
						if (p_tcb->state != 2){
							double a = ((int)p_tcb->stack - p_tcb->tsk_stack)/p_tcb->priv_stack;
						p_task_info->stack_usage = a; 
						}
						else {
							U16    stackPointerRunning;
							double a;
							stackPointerRunning = rt_get_PSP();
							 a =((int)p_tcb->stack - p_tcb->tsk_stack)/stackPointerRunning;
							p_task_info->stack_usage = a; 
							
							
						}
						
					}
					else {
						U16 size;
						
						size = (U16)os_stackinfo ;
						
						//p_task_info->stack_usage = ((int)p_tcb->stack - p_tcb->tsk_stack)/size;
						p_task_info->stack_usage =((int)p_tcb->stack + size - p_tcb->tsk_stack)*100/ size;
					}
						return OS_R_OK;
				}
				else {return OS_R_NOK;}
		
		}
		
	
	//return p_task_info;	
	

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
