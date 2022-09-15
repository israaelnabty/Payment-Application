/* card.h */

#ifndef CARD_H
#define CARD_H

/* Included Libraries */
#include"../Useful Folders/commonTypes.h"

/* Implementing typedefs */
typedef struct ST_cardData_t
{
	uint8_t cardHolderName[25];
	uint8_t primaryAccountNumber[20]; /* PAN */
	uint8_t cardExpirationDate[6];
}ST_cardData_t;

typedef enum EN_cardError_t
{
	CARD_OK, WRONG_NAME, WRONG_EXP_DATE, WRONG_PAN
}EN_cardError_t;

/* Functions Prototypes */
/* Main Functions */
EN_cardError_t getCardHolderName(ST_cardData_t* cardData);
EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData);
EN_cardError_t getCardPAN(ST_cardData_t* cardData);

/* Aiding Functions */

void printErrorNumCard(EN_cardError_t errNumCard);

/* A function to generate PAN following luhn number algorithm */
/* This function is used to generate PAN numbers to be listed in the accounts database only */
void generatePAN(uint8_t numCount, uint8_t* PAN, uint8_t seedOffset);

#endif /* End of card.h */