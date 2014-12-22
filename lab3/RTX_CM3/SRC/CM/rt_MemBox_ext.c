/*----------------------------------------------------------------------------
 *      ECE254 Lab3
 *----------------------------------------------------------------------------
 *      Name:    RT_MEMBOX_ext.C
 *      Purpose: Interface functions for blocking 
 *               fixed memory block management system
 *      Rev.:    V4.60
 *----------------------------------------------------------------------------
 *      This code is extends the RealView Run-Time Library.
 *      Created by University of Waterloo ECE254 Lab Staff.
 *---------------------------------------------------------------------------*/
 
/*----------------------------------------------------------------------------
 *      Includes
 *---------------------------------------------------------------------------*/
 
#include "rt_TypeDef.h"
#include "RTX_Config.h"
#include "rt_System.h"
#include "rt_MemBox.h"
#include "rt_HAL_CM.h"
#include "rt_Task.h"       /* added in ECE254 lab3 keil_proc */ 
#include "rt_MemBox_ext.h" /* added in ECE254 lab3 keil_proc */   
#include "rt_List.h"

/* ECE254 Lab3 Comment: You may need to include more header files */

/*----------------------------------------------------------------------------
 *      Global Variables
 *---------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------
 *      Global Functions
 *---------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------
 *      Local Functions
 *---------------------------------------------------------------------------*/

static OS_TID rt_get_TID (void) {
  U32 tid;

  for (tid = 1; tid <= os_maxtaskrun; tid++) {
    if (os_active_TCB[tid-1] == NULL) {
      return ((OS_TID)tid);
    }
  }
  return (0);
}


/*==========================================================================*/
/*  The following are added for ECE254 Lab3 Proc Management Assignmet       */
/*==========================================================================*/

/*---------------- rt_alloc_box_s, task blocks when out of memory-----------*/

/**  
   @brief: Blocking memory allocation routine.
 */
void *rt_alloc_box_s (void *p_mpool) {
	
	void *ptr;
	P_TCB p_task;
	int task_id;
	ptr = rt_alloc_box (p_mpool);
	
	if(ptr == NULL)
	{
		task_id = rt_tsk_self();
		
		p_task = os_active_TCB[task_id-1];
		
		rt_put_prio(&os_dly, p_task);
		
//		rt_put_dly(p_task, 0xffff);
		
		rt_block(0xffff, 10);
		
		return NULL;
		
	}
	else
	{
		return ptr;
	}
		
	
}


/*----------- rt_free_box_s, pair with _s memory allocators ----------------*/
/**
 * @brief: free memory pointed by ptr, it will unblock a task that is waiting
 *         for memory.
 * @return: OS_R_OK on success and OS_R_NOK if ptr does not belong to gp_mpool 
 */
OS_RESULT rt_free_box_s (void *p_mpool, void *box) {
	/* Add your own code here. Change the following line accordingly */

	int freebox = rt_free_box(p_mpool, box);
	
	P_TCB headtask;
	
	if(freebox == 0)
	{
	//if(headtask != NULL)
	//{
		//rt_rmv_list (headtask);
		if((&os_dly)->p_lnk != NULL)
		{
		headtask = rt_get_first (&os_dly);
		headtask->ret_val = (U32)box;
		rt_dispatch(headtask);
		}
	//}
}
	
	else
		
	{
		return (OS_R_NOK);
	}
	return (OS_R_OK);
}
/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
