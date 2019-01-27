/*
 * linetracking.h
 *
 *  Created on: 27 Jan 2019
 *      Author: utassyd
 */

#ifndef LINETRACKING_H_
#define LINETRACKING_H_

void vonalszamlalo(void);	//vonalfigyelo
uint32_t egyutanharomhossz(void);   //csak biztosan egy vonal esetén hívható meg
uint32_t egyutankettohossz(void);
uint32_t haromutanegyhossz(void);  //csak biztosan egy vonal esetén hívható meg
uint32_t kettoutanegyhossz(void);


#endif /* LINETRACKING_H_ */
