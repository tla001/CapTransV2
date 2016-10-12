/*
 * EncodeSaver2.h
 *
 *  Created on: Oct 12, 2016
 *      Author: tla001
 */

#ifndef ENCODESAVER2_H_
#define ENCODESAVER2_H_

class EncodeSaver2 {
public:
	EncodeSaver2();
	virtual ~EncodeSaver2();

	int initDevice();
	int closeDevice();
	int doEncode(unsigned char *buf);


private:

};

#endif /* ENCODESAVER2_H_ */
