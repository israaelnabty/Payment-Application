/* Included Libraries */
#include"server.h"
#include<stdio.h>
#include<string.h>

/* Global Variables used in the application */
/* Declaring and Initializing 10 Clients Accounts Database */
ST_accountsDB_t clientAccDatabase[255] = { {20000.5f, RUNNING, "44833264445785283"}, 
										   {100.4f, RUNNING, "547652155025728315"},   
										   {30000.0f, RUNNING, "6370816755825740"},  
										   {4000.5f, RUNNING, "7264212051684410"},
										   {19000.0f, RUNNING, "183748386545885661"},
										   {100000.0f, BLOCKED, "00208275617413107"},
										   {3000000.0f, RUNNING, "6348042063635536"},
										   {50000.0f, RUNNING, "560816144343438738"},
										   {400.0f, RUNNING, "8743173132206634239"},
										   {1000000.0f, BLOCKED, "8571720627885408"}
};
/* Declaring Transactions Database and initializing all its members with zeros */
ST_transaction_t transDatabase[255] = { {{{0}}} }; /* However, Global Variables are zeros by default */

/* A variable that hold the currently founf client index in the accounts database */
uint8_t clientIndexFound;
/* A variable that holds the current transaction sequence number */
uint32_t currentTransactionSeqNum = 0;


/* Functions Implementations */
EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t* accountReference)
{
	printf("\n***Checking Account Validity ...");
	/* Check if the PAN exits in any account in the accounts database */
	for (uint8_t index = 0; index <= 254; index++)
	{
		if (!strcmp((clientAccDatabase[index]).primaryAccountNumber, cardData->primaryAccountNumber))
		{
			/* Saving a copy of the client account of the current transaction to a container pointed to 
			   by the pointer "accountReference" */
			*accountReference = clientAccDatabase[index];
			/* Also save its index for later use */
			clientIndexFound = index;
			/* Return OK */
			return SERVER_OK;
		}
	}
	/* If not found, return that it is not found */
	return ACCOUNT_NOT_FOUND;
}

EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountReference)
{
	printf("\n***Checking Account State ...");
	/* Checking the state of the given account if blocked or running */
	if (accountReference->state == BLOCKED)
	{
		return BLOCKED_ACCOUNT;
	}
	else /* If RUNNING */
	{
		return SERVER_OK;
	}
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData)
{
	printf("\n***Checking Client Balance Availability ...");
	/* Checking if the transaction amount exceeds the client balance */
	if ((termData->transAmount) > (clientAccDatabase[clientIndexFound]).balance)
	{
		return LOW_BALANCE;
	}
	else
	{
		return SERVER_OK;
	}
}

EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData)
{
	printf("\n***Checking Saving Transaction Process for transaction sequence #%d ...", transactionSequenceNumber);
	/* Searching for the specified transaction sequence in the transaction database,
	   and if found, save that transaction into a structure of transaction type, then return OK */
	for (uint8_t index = 0; index <= 254; index++)
	{
		if ((transDatabase[index].transactionSequenceNumber) == transactionSequenceNumber)
		{
			*transData = transDatabase[index];
			return SERVER_OK;
		}
	}
	/* If not found in the transaction database, return that it is not found */
	return TRANSACTION_NOT_FOUND;
}

EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	printf("\n***Saving Transaction Process ...");
	/* Assigning a sequence transaction number to the current transaction,
	   and it is an incremented value based on the previous transaction sequence number */
	currentTransactionSeqNum++;
	transData->transactionSequenceNumber = currentTransactionSeqNum;
	/* Printing the sequence number after increment */
	printf("\nCurrent Transaction Sequence Number: %d", currentTransactionSeqNum);

	/* Check if the transaction is not the first one to be saved */
	if ((transData->transactionSequenceNumber) > 1)
	{
		/* Searching the transaction database for the location of the sequence just preceding the current one,
		   and save the current transaction in the following index to it, then return OK */
		for (uint8_t index = 0; index <= 253; index++)
		{
			if ((transDatabase[index]).transactionSequenceNumber == ((transData->transactionSequenceNumber) - 1))
			{
				transDatabase[index + 1] = *transData;
			}
		}
	}
	else if ((transData->transactionSequenceNumber) == 1) /* For the first transaction only */
	{
		/* save it in the first location in the database */
		transDatabase[0] = *transData;
	}

	/* Check if the current transaction is saved successfully, and return "OK" if saving succeeded */
	ST_transaction_t transDataFound; /* To be used in the return of the getTransactions only */
	if (getTransaction(currentTransactionSeqNum, &transDataFound) == SERVER_OK)
	{
		return SERVER_OK;
	}
	else /* If the transaction can't be saved, for any reason (ex: dropped connection), return "saving failed" */
	{
		return SAVING_FAILED;
	}
}

EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	printf("\n***Making Some Checking at the Bank ...");
	EN_serverError_t errorServ;
	ST_accountsDB_t accountRef;

	/* Checking the validity of the account and get its reference if found,
	   and if it is not valid, update the transaction state, then return "FRAUD_CARD" */
	printf("\n");
	errorServ = ACCOUNT_NOT_FOUND;
	errorServ = isValidAccount(&(transData->cardHolderData), &accountRef);
	printErrorNumServer(errorServ);
	if (errorServ == ACCOUNT_NOT_FOUND)
	{
		transData->transState = FRAUD_CARD;
		return FRAUD_CARD;
	}

	/* Checking if the account is blocked,
	   and if it is blocked, update the transaction state, then return "DECLINED_STOLEN_CARD" */
	printf("\n");
	errorServ = BLOCKED_ACCOUNT;
	errorServ = isBlockedAccount(&accountRef);
	printErrorNumServer(errorServ);
	if (errorServ == BLOCKED_ACCOUNT)
	{
		transData->transState = DECLINED_STOLEN_CARD;
		return DECLINED_STOLEN_CARD;
	}

	/* Checking the availability of the transaction amount, and if it is not available
	   update the transaction state, then return "declined" */
	printf("\n");
	errorServ = LOW_BALANCE;
	errorServ = isAmountAvailable(&(transData->terminalData));
	printErrorNumServer(errorServ);
	if (errorServ == LOW_BALANCE)
	{
		transData->transState = DECLINED_INSUFFECIENT_FUND;
		return DECLINED_INSUFFECIENT_FUND;
	}

	/* In case that no "account/amount" error is found, update client balance in the database,
	   then update the transaction state as "approved" */
	printf("\n");
	printf("\nAvailable balance before transaction : % f", clientAccDatabase[clientIndexFound].balance);
	clientAccDatabase[clientIndexFound].balance =
		(clientAccDatabase[clientIndexFound]).balance - (transData->terminalData).transAmount;
	transData->transState = APPROVED;
	printf("\n Transaction Process Has Succeeded!");
	/* Printing the remaining balance after transaction */
	printf("\nAvailable balance becomes : % f", clientAccDatabase[clientIndexFound].balance);

	/* Saving the current transaction in the transaction database, and return "server error" if failed */
	printf("\n");
	errorServ = SAVING_FAILED;
	errorServ = saveTransaction(transData);
	printErrorNumServer(errorServ);
	if (errorServ == SAVING_FAILED)
	{
		return INTERNAL_SERVER_ERROR;
	}
	/* In case that transaction is processed and saving succeeds, return "approved" */
	return APPROVED;
}

/* Aiding Functions */
void printStateNumAccount(EN_accountState_t statNumAcc)
{
	/* Printing the state number with its corresponding text */
	switch (statNumAcc)
	{
	case 0:
		printf("\n RUNNING");
		break;
	case 1:
		printf("\n BLOCKED");
		break;
	}
}

void printStateNumServer(EN_transState_t statNumServ)
{
	/* Printing the state number with its corresponding text */
	switch (statNumServ)
	{
	case 0:
		printf("\n APPROVED");
		break;
	case 1:
		printf("\n DECLINED_INSUFFECIENT_FUND");
		break;
	case 2:
		printf("\n DECLINED_STOLEN_CARD");
		break;
	case 3:
		printf("\n FRAUD_CARD");
		break;
	case 4:
		printf("\n INTERNAL_SERVER_ERROR");
		break;
	}
}

void printErrorNumServer(EN_serverError_t errNumServ)
{
	/* Printing the error number with its corresponding text */
	switch (errNumServ)
	{
	case 0:
		printf("\n SERVER_OK");
		break;
	case 1:
		printf("\n LOW_BALANCE");
		break;
	case 2:
		printf("\n ACCOUNT_NOT_FOUND");
		break;
	case 3:
		printf("\n BLOCKED_ACCOUNT");
		break;
	case 4:
		printf("\n SAVING_FAILED");
		break;
	case 5:
		printf("\n TRANSACTION_NOT_FOUND");
		break;
	}
}