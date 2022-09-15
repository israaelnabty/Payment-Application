/* Included Libraries */
#include"../Application/app.h"

int main(void)
{
	/* appStart */
	appStart();

	/*
		Max Transaction Amount = 15000
		Card Holder Name : Israa Ahmed Abdulmohsen

		**Test Cases :
		1) Valid Card and Account, Available Balance(= 20000) :
		PAN         : 44833264445785283
		Expiry Date : 08/26
		TransAmount : 10000 < MaxTransAmount(15000)

		2) Valid Card and Account, Available Balance(= 20000) :
		PAN         : 44833264445785283
		Expiry Date : 08/26
		TransAmount : 20000 > MaxTransAmount(15000) !!!!!!

		3) Valid Card and Account :
		PAN         : 547652155025728315, with Balance(= 100.5)
		Expiry Date : 08/26
		TransAmount : 1000 > Balance(100.5) !!!!!!

		4) Valid Card and Account, Available Balance(= 20000) :
		PAN         : 44833264445785283
		Expiry Date : 08/20 !!!!!!
		TransAmount : 10000 < MaxTransAmount(15000)

		5) Valid Card and Account, but Blocked !!!!, and Available Balance(= 100000) :
		PAN         : 00208275617413107
		Expiry Date : 08 / 26
		TransAmount : 10000 < MaxTransAmount(15000)

		6) Invalid Card PAN(failed in luhn check) :
		PAN         : 44833264445785289
	*/

	return 0;
}