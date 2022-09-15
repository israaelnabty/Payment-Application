/* Included Libraries */
#include"app.h"
#include"../Card/card.h"
#include"../Terminal/terminal.h"
#include"../Server/server.h"
#include<stdio.h>

/* Functions Implementations */
void appStart(void)
{
	ST_cardData_t userData;
	EN_cardError_t errorNumCard;

	ST_terminalData_t terminalData;
	EN_terminalError_t errorNumTerm;

	ST_transaction_t transactionData = { 0 }; /* Initialized with zeros for no warning messages */
	EN_transState_t stateNumServer;

	/* Terminal Initializations for max amount limit */
	printf("\nInitializing the Terminal");
	errorNumTerm = INVALID_MAX_AMOUNT;
	while (errorNumTerm != TERMINAL_OK)
	{
		errorNumTerm = setMaxAmount(&terminalData);
		printErrorNumTerm(errorNumTerm);
	}

	/* Starting Payment Application */
	printf("\n\n********Starting Payment Application********");
	for (; ; ) /* For the program to be running to accept more than one transaction sequentially */
	{
		/******************** CARD Module Stage ********************/
		printf("\n\n(1)Starting the Card Module");
		/* Step (1): Get Card Holder Name */
		errorNumCard = WRONG_NAME;
		/* Wait till a correct name is entered */
		while (errorNumCard != CARD_OK) {
			errorNumCard = getCardHolderName(&userData);
			printErrorNumCard(errorNumCard);
		}
		printf("\n");

		/* Step (2): Get Card Expiry Date */
		errorNumCard = WRONG_EXP_DATE;
		/* Wait till a correct date format is entered */
		while (errorNumCard != CARD_OK) {
			errorNumCard = getCardExpiryDate(&userData);
			printErrorNumCard(errorNumCard);
		}
		printf("\n");

		/* Step (3): Get Card PAN */
		errorNumCard = WRONG_PAN;
		/* Wait till a correct PAN format is entered */
		while (errorNumCard != CARD_OK) {
			errorNumCard = getCardPAN(&userData);
			printErrorNumCard(errorNumCard);
		}

		/************************ TERMINAL Module Stage ************************/
		printf("\n\n(2)Starting the Terminal Module");
		/* Step (1): All Card Data are obtained in the variable userData */
		printf("\n**Card Module Data Obtained Successfully!");
		printf("\n");

		/* Step (2): Checking the Validity of Card Number (throught luhn number check) */
		errorNumTerm = INVALID_CARD;
		errorNumTerm = isValidCardPAN(&userData);
		printErrorNumTerm(errorNumTerm);
		/* If the card number is valid, proceed,
		   if otherwise, end the application */
		if (errorNumTerm == TERMINAL_OK)
		{
			printf("\n");
			
			/* Step (3): Get Transaction Date Automatically from PC */
			errorNumTerm = WRONG_DATE;
			/* Wait till a correct Transaction Date format is obtained */
			while (errorNumTerm != TERMINAL_OK) {
				errorNumTerm = getTransactionDate(&terminalData);
				printErrorNumTerm(errorNumTerm);
			}
			printf("\n");

			/* Step (4): Check if Card is expired */
			errorNumTerm = EXPIRED_CARD;
			errorNumTerm = isCardExpired(&userData, &terminalData);
			printErrorNumTerm(errorNumTerm);
			/* If the card is not expired, proceed taking more information,
			   if otherwise, end the application */
			if (errorNumTerm == TERMINAL_OK)
			{
				printf("\n");

				/* Step (5): Get the transaction amount */
				errorNumTerm = INVALID_AMOUNT;
				/* Wait till a correct Transaction amount is obtained */
				while (errorNumTerm != TERMINAL_OK)
				{
					errorNumTerm = getTransactionAmount(&terminalData);
					printErrorNumTerm(errorNumTerm);
				}
				printf("\n");

				/* Step (6): Check if the entered amount is not exceeding the terminal max amount */
				errorNumTerm = EXCEED_MAX_AMOUNT;
				errorNumTerm = isBelowMaxAmount(&terminalData);
				printErrorNumTerm(errorNumTerm);
				/* If the amount is not exceeding the terminal max amount, proceed to the server module,
				   if otherwise, end the application */
				if (errorNumTerm == TERMINAL_OK)
				{

					/******************* SERVER Module Stage *******************/
					printf("\n\n(3)Starting the Server Module");
					/* Saving both card data and terminal data as in the current-transaction data */
					printf("\n***Receiving the Card and Terminal Data ...");
					transactionData.cardHolderData = userData;
					transactionData.terminalData = terminalData;
					/* Receive all data and make all server checks, and return the appropriate server state */
					/* Firstly, checking if the account is valid or fraud */
					/* Secondly, checking if the account is blocked or running */
					/* Thirdly, check if the amount required is available in the balance, and update the balance */
					/* Fourthly, trying to save the transaction */
					printf("\n");
					stateNumServer = recieveTransactionData(&transactionData);
					printStateNumServer(stateNumServer);
				}
			}
		}
	}	
}