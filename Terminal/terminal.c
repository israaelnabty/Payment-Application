/* Included Libraries */
#include"terminal.h"
#include"../Useful Folders/usefulFunctions.h"
#include<stdio.h>
#include<string.h>
#include<time.h>

/* Functions Implementations */
EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{   
	printf("\n**Obtaining the Current Date Automatically ...");
	/* Get the Current Date from PC */
	uint8_t DatePC[26] = { 0 }; /* Initialized with zeros for no warning messages */
	                            /* The Result String Format Example: "Thu Aug 18 11:58:06 2022\n" + NULL */
	time_t currentTimeResult = time(NULL);
	/* Putting the obtainted time in a readable string format */
	ctime_s(DatePC, sizeof DatePC, &currentTimeResult);
	printf("\n Date from PC is: %s", DatePC);

	/* Extracting the Parts of interest only, and Saving them in the Desired Format "DD/MM/YYYY" */
	 /* Saving Days */
	termData->transactionDate[0] = DatePC[8];
	termData->transactionDate[1] = DatePC[9];
	/* Saving Slashes */
	termData->transactionDate[2] = '/';
	termData->transactionDate[5] = '/';
	/* Saving Year */
	termData->transactionDate[6] = DatePC[20];
	termData->transactionDate[7] = DatePC[21];
	termData->transactionDate[8] = DatePC[22];
	termData->transactionDate[9] = DatePC[23];
	termData->transactionDate[10] = '\0'; /* End of string */
	/* Comparing Text Months to get them in ASCII format of corresponding numbers */
	uint8_t monthCurrent[4] = { 0 }; /* Initialized with zeros for no warning messages */
	monthCurrent[0] = DatePC[4];
	monthCurrent[1] = DatePC[5];
	monthCurrent[2] = DatePC[6];
	monthCurrent[3] = '\0'; /* End of string */
	if (!strcmp(monthCurrent, "Jan"))
	{
		termData->transactionDate[3] = '0'; termData->transactionDate[4] = '1';
	}
	else if (!strcmp(monthCurrent, "Feb"))
	{
		termData->transactionDate[3] = '0'; termData->transactionDate[4] = '2';
	}
	else if (!strcmp(monthCurrent, "Mar"))
	{
		termData->transactionDate[3] = '0'; termData->transactionDate[4] = '3';
	}
	else if (!strcmp(monthCurrent, "Apr"))
	{
		termData->transactionDate[3] = '0'; termData->transactionDate[4] = '4';
	}
	else if (!strcmp(monthCurrent, "May"))
	{
		termData->transactionDate[3] = '0'; termData->transactionDate[4] = '5';
	}
	else if (!strcmp(monthCurrent, "Jun"))
	{
		termData->transactionDate[3] = '0'; termData->transactionDate[4] = '6';
	}
	else if (!strcmp(monthCurrent, "Jul"))
	{
		termData->transactionDate[3] = '0'; termData->transactionDate[4] = '7';
	}
	else if (!strcmp(monthCurrent, "Aug"))
	{
		termData->transactionDate[3] = '0'; termData->transactionDate[4] = '8';
	}
	else if (!strcmp(monthCurrent, "Sep"))
	{
		termData->transactionDate[3] = '0'; termData->transactionDate[4] = '9';
	}
	else if (!strcmp(monthCurrent, "Oct"))
	{
		termData->transactionDate[3] = '1'; termData->transactionDate[4] = '0';
	}
	else if (!strcmp(monthCurrent, "Nov"))
	{
		termData->transactionDate[3] = '1'; termData->transactionDate[4] = '1';
	}
	else if (!strcmp(monthCurrent, "Dec"))
	{
		termData->transactionDate[3] = '1'; termData->transactionDate[4] = '2';
	}
	printf("\n Date in the required format is: %s", termData->transactionDate);

	/* (Optional) Checking if the Entered Transaction Date Format is Correct,
	   and Return the Suitable Error Message */
	if ((termData->transactionDate[0] == '\0') ||  /* If the entry is NULL */
		(termData->transactionDate[10] != '\0') || /* If the last character is not NULL (i.e. extra entry) */
		(termData->transactionDate[2] != '/') ||   /* Check the presence of "forward slash" */
		(termData->transactionDate[5] != '/') ||   /* Check the presence of "forward slash" */
		((termData->transactionDate[0] < '0') || (termData->transactionDate[0] > '9')) ||
		((termData->transactionDate[1] < '0') || (termData->transactionDate[1] > '9')) ||
		((termData->transactionDate[3] < '0') || (termData->transactionDate[3] > '9')) ||
		((termData->transactionDate[4] < '0') || (termData->transactionDate[4] > '9')) ||
		((termData->transactionDate[6] < '0') || (termData->transactionDate[6] > '9')) ||
		((termData->transactionDate[7] < '0') || (termData->transactionDate[7] > '9')) ||
		((termData->transactionDate[8] < '0') || (termData->transactionDate[8] > '9')) ||
		((termData->transactionDate[9] < '0') || (termData->transactionDate[9] > '9')))
	{
		return WRONG_DATE;
	}
	return TERMINAL_OK;
}

EN_terminalError_t getTransactionDate_Entry(ST_terminalData_t* termData)
{
	/* Asking for the transaction date and storing it into terminal data */
	printf("\n**Please, enter the Transaction Date (DD/MM/YYYY): ");
	readString(termData->transactionDate, 11);

	/* Checking if the Entered Transaction Date Format is Correct,
	   and Return the Suitable Error Message */
	if ((termData->transactionDate[0] == '\0') ||  /* If the entry is NULL */
		(termData->transactionDate[10] != '\0') || /* If the last character is not NULL (i.e. extra entry) */
		(termData->transactionDate[2] != '/') ||   /* Check the presence of "forward slash" */
		(termData->transactionDate[5] != '/') ||   /* Check the presence of "forward slash" */
		((termData->transactionDate[0] < '0') || (termData->transactionDate[0] > '9')) ||
		((termData->transactionDate[1] < '0') || (termData->transactionDate[1] > '9')) ||
		((termData->transactionDate[3] < '0') || (termData->transactionDate[3] > '9')) ||
		((termData->transactionDate[4] < '0') || (termData->transactionDate[4] > '9')) ||
		((termData->transactionDate[6] < '0') || (termData->transactionDate[6] > '9')) ||
		((termData->transactionDate[7] < '0') || (termData->transactionDate[7] > '9')) ||
		((termData->transactionDate[8] < '0') || (termData->transactionDate[8] > '9')) ||
		((termData->transactionDate[9] < '0') || (termData->transactionDate[9] > '9')))
	{
		return WRONG_DATE;
	}
	/* Check the validity of the entered month (should be in the ramge [1, 12]) */
	/* Subtracting 48 from the ASCII of numbers converts them into actual values */
	uint8_t monthEntry = ((termData->transactionDate[3]) - 48) * 10 + (termData->transactionDate[4]) - 48;
	if ((monthEntry < 1) && (monthEntry > 12))
	{
		return WRONG_DATE;
	}
	/* Check the validity of the entered days (should be in the range [1, 31], and depending on the month) */
	uint8_t dayEntry = ((termData->transactionDate[0]) - 48) * 10 + (termData->transactionDate[1]) - 48;
	if ((dayEntry < 1) || (dayEntry > 31))
	{
		return WRONG_DATE;
	}
	if (((dayEntry == 30) || (dayEntry == 31)) && (monthEntry == 2))
	{
		return WRONG_DATE;
	}
	if ((dayEntry == 31) &&
		((monthEntry == 4) || (monthEntry == 6)) || (monthEntry == 9) || (monthEntry == 11))
	{
		return WRONG_DATE;
	}
	/* If months and days are correctly entered, return OK */
	return TERMINAL_OK;
}


EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData)
{
	printf("\n**Checking the Card Expiration Date ...");
	uint8_t cardExpMon, transMon;
	uint16_t cardExpYear, transYear;

	/* Storing the card expiry date (MM/YY) as actual numbers */
	cardExpMon = ((cardData->cardExpirationDate[0]) - 48) * 10 + ((cardData->cardExpirationDate[1]) - 48);
	cardExpYear = 2000 +
		((cardData->cardExpirationDate[3]) - 48) * 10 + ((cardData->cardExpirationDate[4]) - 48);

	/* Storing the transaction date (DD/MM/YYYY) as actual numbers */
	transMon = ((termData->transactionDate[3]) - 48) * 10 + ((termData->transactionDate[4]) - 48);
	transYear = ((termData->transactionDate[6]) - 48) * 1000 + ((termData->transactionDate[7]) - 48) * 100 +
		((termData->transactionDate[8]) - 48) * 10 + ((termData->transactionDate[9]) - 48);

	/* Comparing both dates and check if the card is expired */
	if (transYear <= cardExpYear)
	{
		if (transMon <= cardExpMon)
		{
			return TERMINAL_OK;
		}
	}
	/* If Card is expired */
	return EXPIRED_CARD;
}

EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData)
{
	printf("\n**Checking the Card Validity ...");
	uint8_t numPAN[19] = { 0 }; /* Initialized with zeros for no warning messages */
	uint8_t numCount = 0, checkSum = 0;
	/* Storing the PAN number digits as actual numbers, not ASCII */
	for (uint8_t index = 0; cardData->primaryAccountNumber[index] != '\0'; index++)
	{
		numPAN[index] = (cardData->primaryAccountNumber[index]) - 48;
		/* Count each number found */
		numCount++;
	}
	/* Moving from the rightmost digit, whose index is (numCount - 1) */   
	/* Starting from the secont digit rightmost (numCount - 2) and move by 2 locations, not one */
	for (int8_t index = numCount - 2; index >= 0; index = index - 2)
	{
		/* Multiplyby 2 */
		numPAN[index] = numPAN[index] * 2;

		/* Sum the number digits if they are two digits */
		if (numPAN[index] > 9)
		{   /* (numPAN[index] % 10) gets the unit digit */
			/* Adding with "one" that is the number ten in the digit */
			numPAN[index] = (numPAN[index] / 10) + (numPAN[index] % 10);
		}
	}
	/* Adding all digits of the resulting number */
	for (uint8_t index = 0; index <= (numCount - 1); index++)
	{
		checkSum = checkSum + numPAN[index];
	}
	/* Check if their sum is divisible by 10, then it is a luhn number
	   if otherwise, the card is invalid */
	if ((checkSum % 10) == 0)
	{
		return TERMINAL_OK;
	}
	else
	{
		return INVALID_CARD;
	}
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
	/* Asking for the transaction amount and storing it into terminal data */
	printf("\n**Please, enter the Transaction Amount: ");
	scanf_s("%f", &(termData->transAmount));
	/* Receive the new line after using scanf to avoid problems in other functions */
	uint8_t c = (uint8_t)getchar();

	/* Check if the entered amount is reasonable */
	if (termData->transAmount <= 0.0f) /* a float literal is double by default */
	{
		return INVALID_AMOUNT;
	}
	else
	{
		return TERMINAL_OK;
	}
}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData)
{
	float maxAmount;
	/* Asking for the transaction amount and storing it into terminal data */
	printf("\n**Please, set the Transaction Maximum Amount: ");
	scanf_s("%f", &maxAmount);
	/* Receive the new line after using scanf to avoid problems in other functions */
	uint8_t c = (uint8_t)getchar(); 

	/* Check if the entered max amount is reasonable */
	if (maxAmount <= 0.0f) /* a float literal is double by default */
	{
		return INVALID_MAX_AMOUNT;
	}
	else
	{
		termData->maxTransAmount = maxAmount;
		return TERMINAL_OK;
	}
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
	printf("\n**Checking the Maximum Amount Limit ...");
	/* Comparing the transaction amount if it is below the max allowed terminal amount */
	if ((termData->transAmount) > (termData->maxTransAmount))
	{
		return EXCEED_MAX_AMOUNT;
	}
	else
	{
		return TERMINAL_OK;
	}
}


/* Aiding Functions */
void printErrorNumTerm(EN_terminalError_t errNumTerm)
{
	/* Printing the error number with its corresponding text */
	switch (errNumTerm)
	{
	case 0:
		printf("\n TERMINAL_OK");
		break;
	case 1:
		printf("\n WRONG_DATE");
		break;
	case 2:
		printf("\n EXPIRED_CARD");
		break;
	case 3:
		printf("\n INVALID_CARD");
		break;
	case 4:
		printf("\n INVALID_AMOUNT");
		break;
	case 5:
		printf("\n EXCEED_MAX_AMOUNT");
		break;
	case 6:
		printf("\n INVALID_MAX_AMOUNT");
		break;
	}
}