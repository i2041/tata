/*
 * task.h
 *
 *  Created on: 27 ian. 2016
 *      Author: uidg6243
 */

#ifndef LIBRARY_TASK_H_
#define LIBRARY_TASK_H_
#include "CiocaneLipit.h"
#include "commontypes.h"
#include "interrupts.h"
#include "TLC5947.h"
#include "ads1115.h"
void task33ms();
void task100ms();
void task500ms();
void task1s();
uint16 tmpValue;

#endif /* LIBRARY_TASK_H_ */
