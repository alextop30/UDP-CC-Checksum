# UDP-CC-Checksum
Credit Card Checksum using UDP -- client only

Program creates checksum verification for credit card which applies to both AMEX and Standard 16 digit cards

Program will take all of the information with prompts from standard in and will use Datagram or UDP to send it to the server to validate the number, the program will only apply limited checks to the number before it sends it to the server for complete verification. The server is to be set up separately, this was part of an assignment for class and I do not have the server code. If you wish to design a server yourself below is an example run which will show what the server is to report back. The program worked successfully and the highest grade was given for this assignment.


Example Run:

Welcome to the Credit Card transaction verification utility:
Enter card holder name (or quit): John Doe
Enter CC number: 3724 531039 23000
Enter expiration: 10/2016
Enter amount: 12.00
Wed Nov 14 10:34:05 CST 2012: American Express: 3715 199624 21012 is valid: authorization code: 198806 for $12.00
Enter card holder name (or quit): Susan Maloney
Enter CC number: 6011 1234 4321 1234
Enter expiration: 12/16
... expiration date invalid, try again: 12/2014
Enter amount: 1234.56
Wed Nov 14 10:35:45 CST 2012: Discover: 6011 1234 4321 1234 is valid: authorization code: 386489 for $1234.56
