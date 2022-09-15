/* Included Libraries */
#include"card.h"
#include"../Useful Folders/usefulFunctions.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

/* Functions Implementations */
EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
	/* Asking for the cardholder's name and storing it into card data */
	printf("\n*Please, enter the Card Holder's Name: ");
	readString(cardData->cardHolderName, 25);

	/* Counting the number of "non-NULL" characters in the Entered Card Name */
	uint8_t charactersIndex = 0; /* Starting from the first character Index */
	uint8_t nameCharCount = 0;   /* Intializing the characters counter with zero */
	while ((charactersIndex <= 24) && (cardData->cardHolderName[charactersIndex] != '\0'))
	{
		/* First check if the character is non-alphabetic (upper/lower cases) or a space */
		if ((cardData->cardHolderName[charactersIndex] < 'A') || (cardData->cardHolderName[charactersIndex] > 'Z'))
		{
			if ((cardData->cardHolderName[charactersIndex] < 'a') || (cardData->cardHolderName[charactersIndex] > 'z'))
			{
				if (cardData->cardHolderName[charactersIndex] != ' ') /* If not a space */
				{
					printf("\n The number of entered characters before ""error"" is %d", nameCharCount);
					return WRONG_NAME;
				}
			}
		}
		/* Count the character and Increment index only if the character is not null, and (alphabetic or space) */
		nameCharCount++;
		charactersIndex++;
	}
	printf("\n The number of entered alphabetical characters is %d", nameCharCount);
	/* Checking if the Entered Name Format is Correct,
	   and Return the Suitable Error Message */
	if ((nameCharCount < 20) || (nameCharCount > 24))
	{
		return WRONG_NAME;
	}
	else /* if (20 <= nameCharCount <= 24) */
	{
		return CARD_OK;
	}
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
	/* Asking for the card expiry date and storing it into card data */
	printf("\n*Please, enter the Card Expiry Date (MM/YY): ");
	readString(cardData->cardExpirationDate, 6);

	/* Checking if the Entered Expiry Date Format is Correct,
	   and Return the Suitable Error Message */
	if ((cardData->cardExpirationDate[0] == '\0') || /* If the entry is NULL */
		(cardData->cardExpirationDate[5] != '\0') || /* If the last character is not NULL (i.e. extra entry) */
		(cardData->cardExpirationDate[2] != '/') ||  /* Check the presence of "forward slash" */
		((cardData->cardExpirationDate[0] < '0') || (cardData->cardExpirationDate[0] > '9')) ||
		((cardData->cardExpirationDate[1] < '0') || (cardData->cardExpirationDate[1] > '9')) ||
		((cardData->cardExpirationDate[3] < '0') || (cardData->cardExpirationDate[3] > '9')) ||
		((cardData->cardExpirationDate[4] < '0') || (cardData->cardExpirationDate[4] > '9')))
	{
		return WRONG_EXP_DATE;
	}

	/* Check the validity of the entered month (should be in the ramge [1, 12]) */
	/* Subtracting 48 from the ASCII of numbers converts them into actual values */
	uint8_t monthEntry = ((cardData->cardExpirationDate[0]) - 48) * 10 + (cardData->cardExpirationDate[1]) - 48;
	if ((monthEntry >= 1) && (monthEntry <= 12))
	{
		return CARD_OK;
	}
	else
	{
		return WRONG_EXP_DATE;
	}
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
	/* Asking for the card PAN and storing it into card data */
	printf("\n*Please, enter the Primary Account Number (PAN) : ");
	readString(cardData->primaryAccountNumber, 20);

	/* Counting the correctly enterd characters in the requied format (numeric) */
	uint8_t numCount = 0;
	for (uint8_t index = 0; index <= 19; index++)
	{
		/* Check if the chractacter is not a NULL */
		if (cardData->primaryAccountNumber[index] != '\0')
		{
			/* Checking the format of each element if it is not numeric */
			if ((cardData->primaryAccountNumber[index] < '0') || (cardData->primaryAccountNumber[index] > '9'))
			{
				return WRONG_PAN;
			}
			/* Counting the element if it is numeric */
			numCount++;
		}
		else /* If it is NULL (the end of string) */
		{
			/* Exit the loop if we encounter a NULL */
			break;
		}
	}
	/* Check the counted number of elements if less than 16 or greater than 19 */
	printf("\n The number of digits in the entered PAN is %d", numCount);
	if ((numCount < 16) || (numCount > 19))
	{
		return WRONG_PAN;
	}
	else /* if (16 <= numCount <= 19) */
	{
		return CARD_OK;
	}
}

/* Aiding Functions */

void printErrorNumCard(EN_cardError_t errNumCard)
{
	/* Printing the error number with its corresponding text */
	switch (errNumCard)
	{
	case 0:
		printf("\n CARD_OK");
		break;
	case 1:
		printf("\n WRONG_NAME");
		break;
	case 2:
		printf("\n WRONG_EXP_DATE");
		break;
	case 3:
		printf("\n WRONG_PAN");
		break;
	}
}

 /* A function to generate random-numbers PAN following luhn number algorithm */
 /* This function is used to generate PAN numbers to be listed in the accounts database only */
void generatePAN(uint8_t numCount, uint8_t* PAN, uint8_t seedOffset)
{
	uint8_t checkPAN[20] = { 0 };
	uint8_t checkSum = 0;
	/* Generate (numCount - 1) random numbers between 0 and 9 */
	/* Change seed value with time using time(0) function */
	srand((unsigned int)time(0) + seedOffset); 
	 /* Note: This offset is to prevent similar seeds if we run the function in a loop */
	for (uint8_t index = 0; index <= (numCount - 2); index++)
	{
		PAN[index] = rand() % 9 + 0;
		checkPAN[index] = PAN[index];
	}
	/* Generate the rightmost control number */
	for (int8_t index = (numCount - 2); index >= 0; index = index - 2)
	{
		checkPAN[index] = checkPAN[index] * 2;
		/* Sum the number digits if they are two digits */
		if (checkPAN[index] > 9)
		{   /* (numPAN[index] % 10) gets the unit digit */
			/* Adding with "one" that is the number ten in the digit */
			checkPAN[index] = (checkPAN[index] / 10) + (checkPAN[index] % 10);
		}
	}
	/* Adding all digits of the resulting number */
	for (uint8_t index = 0; index <= (numCount - 2); index++)
	{
		checkSum = checkSum + checkPAN[index];
	}
	/* Calculate the last rightmost number in the PAN */
	PAN[numCount - 1] = (10 - (checkSum % 10)) % 10;
	/* Converting the PAN numbers into ASCII */
	for (uint8_t index = 0; index <= (numCount - 1); index++)
	{
		PAN[index] = PAN[index] + 48;
	}
	PAN[numCount] = '\0'; /* End of String */
}