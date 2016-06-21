#include <iostream>
#include <cstdlib>
#include <fcntl.h>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

/*
 * 
 * Purpose: To create a program validates credit card transactions
 * over udp connection using hardwired host name to hopper server. The
 * program asks for input from the user - their name separated by a
 * space between the first and last name - accounts for a middle initial
 * or middle name. The expiration date must be entered in two digit
 * month slash four digit year xx/xxxx and it must be at least 04/2015
 * dates before are not accepted. The last valid date is 2029. The 
 * amount must be in xx.xx format - the amount can be up to a 17 digit
 * before the decimal point - to account for cards without limit. If
 * invalid data is entered the user is reprompted to enter a valid
 * data. The program does not perform the credit card checksum mod% 10
 * -- server will perform this check and will return invalid checksum if
 * the card number does not pass the checksum. Both American Express
 * type and regular 16 digit credit cards are excepted. The credit card
 * number is checked that it contains valid digits and that the clusters
 * if entered with spaces are the correct number of digits. The digit
 * succession is not checked but validated by the server using a
 * checksum algorithm.
 * 
 */

using namespace std;


//function prototypes
bool is_all_digit(const char *);

void card_verification(const char*, const char*, 
const char *, const char *);

bool date_validation(const char *);

bool amount_validation(const char *);


int main(int argc, char* argv[])
{
	char name[64];					//Contains a person's name or quit
	char credit_card [40]; 			//credit card numbber
	char card_err[21];				//used for strtok check of credit card
	char exp_date [10];				//holds input from expiration date
	int i;							//counter inner while loop
	char * ptr;						//ptr used in strtok
	char amount[20];				//amount input area
	int cnt;						//counter for spaces in name
	
	
	//flag to break out of the big while loop
	bool c_wrong = true;
	
	//welcome message
	cout << "Welcome to the Credit Card transaction verification utility:";
	cout << endl;
	
	//flag used to evaluate name if it is within the allowed askii range
	bool name_flag = false;
		
	//enless while loop which will run all of the verification code
	//until the user enters quit in the name field which immediatelly
	//will return 0 and normally end the program
	while(1)
	{
	//prompt and gather name
	cout << "Enter card holder name (or quit):  ";	
	cin.getline(name,64);
	
		//while there are 1 or 2 spaces in the name
		while (cnt !=1 || cnt != 2)
		{
			//reset the counter each time going through the loop
			cnt =0;
			
			//go through each char and compare against a spacce
			for (unsigned int z =0; z< strlen(name); z++)
			{
				//increment a counter if there is a space
				if(name[z] == ' ')
				{
					cnt++;		
											
				}
				
				//check if the name is only consistent of letters
				//using their askii values - the ranges represent
				//both upper and lower case letters
				//if letters are not in the range name is invalid				
				if((name[z] >= 65) && (name[z] <= 90))
				{ 
					name_flag = true;
				}
				else if (name[z] >= 95 && name[z] <= 122)
				{
					name_flag = true;
				}
				else
				{
					//if the letters do not fit in the conditions 
					//set flag to false
					name_flag = false;
				}
				
				//if there is a space in the name set flag back to true
				if (name[z] == 32)
					name_flag = true;
				
				
				//if name flag is false at this point break out of the
				//loop name will be evaluated as invalid	
				if (name_flag == false)
					break;	
				
			}
			
			//if quit was entered - end the program
			if (strcmp(name,"quit") == 0)
			{
				return 0;
			}
			
			//if name contains 1 or two spaces break out of the inner
			//loop and continue - name is correct at this point
			if((cnt == 1 || cnt == 2) && name_flag == true )
			{
				break;
			}
			else
			{
				cout << "Invalid name please enter your first and last name: ";
				cin.getline(name,64);
			}
		
		}
		
		//prompt and gather credit card
		cout << "Enter Credit Card number:  ";
		cin.getline(credit_card,40);
		
		//reset flag back to true for multiple runs of the while
		c_wrong = true;
		

		// process credit card and check if format is valid
		while (c_wrong ==true)
		{	
			//flag for exit the while loop		
			c_wrong = true;
			
			//segmented credit card num if there are spaces
			char * card_segment[6];
		
			//flag to hold is all digit outcome
			bool is_num;
			
			do
			{
				//check the length of the string right away	
				while(strlen(credit_card) <15)
				{
					cout << "Invalid credit card number! Please reenter number: ";
					cin.getline(credit_card,40);
				}
				
				//copy original string into card error string
				memcpy(card_err,credit_card,21);
				
				//set the flag to true at the top of the loop
				is_num = true;
			
				//process string with spaces using strtok
				ptr = strtok(card_err," ");
				card_segment[0] = ptr;
			
				//check if only digits are present in the string
				is_num = is_all_digit(card_segment[0]);
				
				//counter i
				i = 1;
				ptr = strtok(NULL," ");
				card_segment[i] = ptr;		
				while(ptr != NULL)
				{
					
					//check if everything is a digit
					is_num = is_num && is_all_digit(card_segment[i]);
					
					//if flag is false break out of the inner loop				
					if(is_num == false)
					{
						break;
					}
					
					
					//increment the counter	
					i++;
					
					//call to strtok to continue with parsing	
					ptr = strtok(NULL," ");
					card_segment[i] = ptr;		
				
				}
			
				
				//if the flag is false get prompt for new input
				if(is_num == false)
				{
					cout << "Invalid credit card number! Please reenter number: ";
					cin.getline(credit_card,40);
				}
				//if flag is true while will end
			}while(is_num !=true );
			
			
			//based on the iteration of I enter a switch statement		
			switch (i)
			{
				//if all card numbers entered without spaces
				case 1:
				{
					//case if credit card number is entered without spaces
					//test each character and make sure it is a digit
					//if not go to the default case and ask for input
						if(strlen(credit_card) == 16)
						{
							//if length is equal to 16 call to check for
							//digits and if only digits are found set the
							//flag off
							bool ret = is_all_digit(credit_card);
							
							if (ret == true)
							{
								//switch flag off
								c_wrong = false;
							}
							else
							{
								//if not all of the characters are digits
								//go to the default error case
								goto DEF;
							}
								
						}
						else if(strlen(credit_card) == 15)
						{
							//do the same if there are 15 characters entered
							//for american express card
							bool ret1 = is_all_digit(credit_card);
								
							if (ret1==true)
							{
								//set flag off	
								c_wrong = false;
							}
							else
							{
								//go to default case
								goto DEF;
							}
						}
						else
						{
							//go to default case
							goto DEF;
						}
							
							
					break;
				}
				//if amrican express with spaces
				case 3 :
				{
						//check that the length of the string is 17
						//if it is not go to default
						//if it is check each tokenized string's length
						//if it is 4 - 6 - 5 it is good -- token strings
						//have already been examined if they contain letters
						if(strlen(credit_card) == 17)
						{
							if((strlen(card_segment[0])== 4) &&
							   (strlen(card_segment[1]) ==6 ) &&
							   (strlen(card_segment[2])==5)   )
								{
									c_wrong = false;
								}
							else
							{
								goto DEF;
							}
						}
						else
						{
							goto DEF;
						}
						
						break;
				}
				//if all other cards with spaces
				case 4 :
				{
						//check that the length of the string is 19
						//if it is not go to default
						//if it is check each tokenized string's length
						//if it is 4 - 4 - 4 - 4 it is good -- token strings
						//have already been examined if they contain letters
						if(strlen(credit_card) == 19)
						{
							if((strlen(card_segment[0])== 4) &&
							   (strlen(card_segment[1]) ==4 ) &&
							   (strlen(card_segment[2])==4) && 
							   (strlen(card_segment[3]) == 4)     )
								{
									c_wrong = false;
								}
							else
							{
								goto DEF;
							}
						}
						else
						{
							goto DEF;
						}
						break;
				}
				DEF: default:
				{
					//default case - report error and have the user reenter
					cout << "Invalid credit card number! Please reenter number: ";
					cin.getline(credit_card,40);
					break;
				}
			}
			
		}

		//date flag to help evaluate if the date is in the limit
		bool flag_date = false;
		cout << "Enter expiration date: ";
		
		//get expiration date from the user
		cin.getline(exp_date,10);
		
		//if flag date is true the loop will end
		while (flag_date == false)
		{
			
			//call date validation function
			flag_date = date_validation(exp_date);
			
			if (flag_date == false)
			{
				cout << "Invalid expiration date! Please reenter date xx/xxxx: ";
					//get expiration date from the user
					cin.getline(exp_date,10);
			}
			
		}
		
		//flag for amount validation - original position false
		bool amount_validate = false;
		cout << "Enter amount: ";
		
		//gather user input
		cin.getline(amount,20);
		
		//while the amount validation is false keep executing while loop
		while(amount_validate == false)
		{
			//call to validate function
			amount_validate = amount_validation(amount);
			
			//if flag is false prompt user to reenter
			if(amount_validate == false)
			{
				//reprompt the user
				cout << "Invalid amount entered, Please reenter amount xx.xx: ";
				cin.getline(amount,20);
			}
				
		}
			
		
		
		//call to credit card verification
		//connects to server to check the card
		card_verification(name,credit_card,exp_date,amount);
	}
	
	//end program normally with code of 0	
	return 0;
}


/*
 * Function is all digit - responsible for checking strings for digits 
 * and if they are composed completely of digits, this function will
 * return true for a string completely composed of all digits
 * 
 * Function takes pointer to a c string
 */

bool is_all_digit(const char * str)
{
	//size is the length of the string
    size_t len = strlen(str);
    
    //flag will be flipped to true or false
	int flag;
	
	//step through each character and evaluate
	for (unsigned int j = 0; j < len; ++j)
	{
		
		flag = isdigit(str[j]);	
		
		//if a non digit character is found immediatly return false
		if (flag ==0)
		{
			//as soon as 1 non digit is found return fallse out of the
			//function
			return false;

		}
		
	}
	
	//else return true out of the function
		return true;
}


/*
 * Card verification function sends all of the input from the user to
 * hopper and receives a response. The arguments that are needed to be
 * passed in are person's name, credit card number, expiration date,
 * and the amount. The strings are combined into a colon deliminated
 * string and sent to the server looked up by host name.
 * 
 * Function does not return but displays the server respoce to the
 * screen using std out.
 */

void card_verification(const char * name, const char * ccard, 
const char * expdate, const char * amount)
{
	//message array
	char send_message [125];
	
	//send all of the strings to a single message array
	sprintf(send_message,"%s:%s:%s:%s",ccard,expdate,amount,name);
	
	//length of the message - in len
	size_t len = strlen(send_message);
	
	//size received
	int received =0;
	
	//address
	unsigned int addrlen;
	
	//socket descriptor
	int sock;
	
	//buffer to hold received data
	char receivebuff[256];
	
	//struct holds the address looked up by the host name
	struct addrinfo *res;
	
	
	struct sockaddr_in hopper;
	
	//holds input from ip_address and port
	string ip_address, port;
	
	//get input from console
	cout << "Enter Server IP Address: ";
	cin >> ip_address;
	
	cout << "Enter Server Port: ";
	cin >> port;
	
	//get hopper's specifics using adder info
	int net_info = getaddrinfo(ip_address.c_str(),NULL,NULL, &res);
	
	//error check if adderinfo failed
	if(net_info)
	{
		cerr << ip_address << ":" << gai_strerror(net_info) << endl;
		exit(EXIT_FAILURE);
	
	}
	
	//create the udp socket
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	
	//if socket failed
	if (sock < 0)
	{
		perror("Failed to create socket");
		exit(EXIT_FAILURE);
	}
	
	struct sockaddr_in *addr = (struct sockaddr_in *) res ->ai_addr;
	
	//construct the server sock
	memset(&hopper, 0, sizeof(hopper));
	hopper.sin_family = AF_INET;
	hopper.sin_addr = addr->sin_addr; 
	hopper.sin_port = htons(atoi(port.c_str()));
	
	//send message to server
	size_t my_mess = sendto(sock, send_message, len, 0, 
	(struct sockaddr*) &hopper, sizeof(hopper));
	
	//check bytes received
	if( my_mess != len)
	{
		perror("Mismatch in number of received bytes");
		exit(EXIT_FAILURE);
		
	}
	
	//Receive the message back from the server
	addrlen = sizeof(hopper);
	received = recvfrom(sock, receivebuff, 256, 0, 
	(struct sockaddr*) &hopper, &addrlen);
	
	//null terminate the received buffer array
	receivebuff[received] = '\0';
	
	//output the message
	cout << receivebuff << endl;
	
	
	
}


/* 
 * Function date validation which is responsible for checking if the 
 * date entered by the user is correct - it uses the askii values for
 * each one of the characters checking for - the lowest bound allowed
 * is the current month and year -- 04/2015 the card cannot be past
 * the expiration date.
 * 
 * Function returns a bool value of false if an error has been encountered.
 */

bool date_validation(const char * exp_date)
{
	
	//get the size of the string
	size_t expdate_len = strlen(exp_date);
	
	
	//check for date length to be 7	
	if(expdate_len != 7)
		return false;
	
	
	//pointer pointing to the year
	const char * year = exp_date + 3;
	

	//check for date / on 3rd position of string
	if(exp_date[2] != '/')
		return false;
	
	// check if the first position is bigger than 1 for month  		
	if(exp_date[0] > 49)
		return false;
	
	// check if the first position is smaller than 0 for month	
	if(exp_date[0] < 48)
		return false;
	
	// check if the second position is smaller than 0			
	if(exp_date[1] < 48)
		return false;
	
	//check if first position is equal to 1 and if the first
	//is larger than 2
	if(exp_date[0] == 49 && exp_date[1] > 50)
		return false;
	
	//check if the fourth position is bigger than 2 for thousand years
	if(exp_date[3] != 50)
		return false;
	
	//checck if the fifth position is greater than 0 for century
	if(exp_date[4] != 48)
		return false;
	
	//checck if 6th position is smaller than 1 for decade	
	if(exp_date[5] < 49)
		return false;
	
	//check if 6th position is greater than 2 - card is only valid for
	//10 years upper bound is 2025
	if(exp_date[5] > 50)
		return false;
	
	//check if the 6th position is less than zero
	if(exp_date[6] < 48)
		return false;
		
		
	if(exp_date[5] == 49 && exp_date[6] < 53)
		return false;
		
		
	if((strcmp(year,"2015"))== 0 && exp_date[0] ==48 && exp_date[1] < 52)
	{
		return false;
	}
	//if the check comes all the way down here return true
	return true;
}


/* 
 * Function amount validation is responcible for receiving a c string
 * which contains the amount inputed by the user making sure that the
 * amount entered is valid and within the specified standards xx.xx
 * 
 * If the amount fits within the correct format and contains only digits
 * a boolian value true is returned else false is returned at the first
 * character that is found that does not fit the specification. Period
 * must be entered at the 3rd digit to the end of the number (Number
 * with precision 2)
 */

bool amount_validation(const char * amount)
{
	
	//size is the length of the string
    size_t len = strlen(amount);
    
    //flag to hold outcome of isdigit test
    int is_digits;

	//check if the period is located in the correct place
	if(amount[len-3] != '.')
		return false;
	
	//step through each character and test if it is a digit
	for(unsigned int i =0; i< len; ++i)
	{
		// at the period position increment the counter past it
		if(i == (len-3))
		{
			i++;
		}
		
		is_digits = isdigit(amount[i]);

		//if a non digit is found return false
		if (is_digits == 0)
		{
			
			return false;
		}
	}
	
	return true;
}

