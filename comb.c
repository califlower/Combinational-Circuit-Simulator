#include<stdio.h>
#include<stdlib.h>
#include<math.h>

struct node
{
	char varname;
	int val;
	int isoutput;
    	struct node *next;
}
*head;

/******
Gets the value of a character
***/

int get(char in)
{
    struct node *iter;
	iter=head;
	while (iter!=NULL)
	{
		if (iter->varname==in)
		{
			return iter->val;
		}

		iter=iter->next;
	}
	return 0;
	
}


/*****
A function used for storing things easily in a linked list
Handles collisions and stuff. Makes life easy. 
Has a output flag that determines if I should print it
*****/

void set(char in,int val,int output)
{
	struct node *toInsert=(struct node *)malloc(sizeof(struct node));
	struct node *iter;
	
	toInsert->varname=in;
	toInsert->val=val;
	
	iter=head;

	if (iter==NULL)
	{
		toInsert->isoutput=output;
		head=toInsert;
		head->next=NULL;
		return;
	}
	else
	{
		while (iter!=NULL)
		{
			if (iter->varname==in)
			{
				iter->val=val;
				return;
			}
			if (iter->next==NULL)
			{
				toInsert->isoutput=output;
				iter->next=toInsert;
				toInsert->next=NULL;
				return;
			}
			iter=iter->next;
		}	
	}
	
	
	
}


/*****************
Deletes the linked linked list 
so that it doesn't cause an issue if someone decides to 
run a zillion inputs or something weird
******************/

void deleteList()
{
   
   struct node* iter = head;
   struct node* next;
 
   while (iter != NULL) 
   {
       next = iter->next;
       free(iter);
       iter = next;
   }
   
   head = NULL;
}


/*******************************
Prints the linked list. Will only print stuff with an output flag of 1
The reason for the flag is because otherwise, it's impossible to know what I should print
out dynamicaly
******************************/

void print()
{
	struct node *iter;
	iter=head;
	while (iter!=NULL)
	{
		if (iter->isoutput==1)
		{
		printf("%d", iter->val);	
		}
		
		iter=iter->next;
	}
	printf("\n");
}


void readInputFile (char circuittext[],FILE *yargh)
{
	
		/*************
		Opens the circuit description file
		**************/
		
		FILE *circuit = fopen(circuittext,"r");
						
		// A generic string that holds the value of FSCAN		
		char command[100]; 
		
		//A String that holds the inputs that come after inputVar. Holds them one at a time
		char inputStore[100]; 
	
		//The amount of initial variables after the word INPUTVAR
		int numAmount=0;
		
		//The amount of outputs
		int outputNumAmount=0; 
		
		//Jumps past INPUTVAR
		fscanf(circuit, "%s", command); 
		
		//FSCAN of the number of inputs after INPUTVAR
		fscanf(circuit, "%d", &numAmount); 

		//Sets the amount of outputs to the amount of inputs? Literally don't remember why I did this.
		outputNumAmount=numAmount;
		
		//A temporary character for output numbers
		char tempOut[10]; 
		 
		
		/*****************************************
		Runs inputs
		*****************************************/
		int q;
		for (q=0;q<numAmount;q++)
		{
			//Reads the variable in INPUTVAR
			fscanf(circuit, "%s", command); 
			
			//Reads the matching number from the inputs
			fscanf(yargh, "%s", inputStore);
			
			//Sets it in linked list with bit flag 0
			//Bit flag zero means ITS NOT AN OUTPUT
			set(command[0],atoi(inputStore),0);
	
		}
		

		
		//Gets to OUTPUTVAR
		fscanf(circuit, "%s", command);
		
		//STORES THE AMOUNT OF OUTPUT VARS
		fscanf(circuit, "%s", tempOut);
		
		//CONVERTS THE OUTPUTVAR to an Integer
		int tempOut2=atoi(tempOut);
		
		
		
		/*****************************************
		Runs outputs
		*****************************************/
		
		for (tempOut2;tempOut2!=0;tempOut2--)
		{
			fscanf(circuit, "%s", command); //reads the initial variables from outputs			
			set(command[0],0,1);
		}
		
		
		/************************************************************
		This while loop is clever
		The FSCAN in the while loop itself is designed to read the commands 
		OR,AND,MULTIPLEXER,NOT etc
		Inside the loop are basic if statements. Each If statement will trigger depending on the command
		***************************************************************/
		while (fscanf(circuit, "%s", command)!=EOF)
		{
			if (strcmp("AND",command)==0)
			{		
				char var1;
				char var2;
				char out;
				
				
				fscanf(circuit, "%s", command);
				var1=command[0];
				fscanf(circuit, "%s", command);
				var2=command[0];
				fscanf(circuit, "%s", command);
				out=command[0];

				
				if (get(var1)==1 && get(var2)==1)
				{
					set(out,1,0);
				}
				else
				{
					set(out,0,0);
				}
				
				
			}
			else if (strcmp("OR",command)==0)
			{
				char var1;
				char var2;			
				char out;
			
				
				fscanf(circuit, "%s", command);
				var1=command[0];
				fscanf(circuit, "%s", command);
				var2=command[0];
				fscanf(circuit, "%s", command);
				out=command[0];

				int var1val=get(var1);
				int var2val=get(var2);
				
				if (var1val==1 || var2val==1)
				{
					set(out,1,0);
				}
				else
				{
					set(out,0,0);
				}
							
			}
			else if (strcmp("NOT",command)==0)
			{
				char var1;//variable char
				int varval1;//value
				fscanf(circuit, "%s", command);//variable
				var1=command[0];
				varval1=get(var1);
				
				fscanf(circuit, "%s", command);//output
				
				if (varval1==1)
				{

					set(command[0],0,0);
				}
				else
				{
					set(command[0],1,0);
				}
							
			}
			
			else if (strcmp("MULTIPLEXER",command)==0)
			{
				
				
				int loopvalue=0;		//Stores the value of how many inputs there are. Also determines how many times to loop to scan inputs
				char loopypoopy[10];		//The character representation of above
				int amtoutputs=0;		//The outputs/selectors. Determined by taking the log-base2 of the amount of inputs to get 2^x=outputs
				int inputarray[100];		//An array that stores the input values as integers 1 or 0
				int biSelect[10];		//Used to store the selector value as a binary array
				int decimalSelect=0;		//Stores the converted binary array as a decimal number

				
				fscanf(circuit, "%s", loopypoopy);	//Scans the input number and stores it
				loopvalue=atoi(loopypoopy);		//Converts the loop char to int
				
				double temp1=log(loopvalue);		//Takes log of loopvalue
				double temp2=log(2);			//Takes log of base 2
				amtoutputs=temp1/temp2;			//Math trick to do log base 2
				
				
				/*
				Gets inputs and converts them to the proper numbers. Puts in array inputarray
				*/
				
				int i;
				for (i=0;i<loopvalue;i++)
				{
					fscanf(circuit, "%s", command);
					
					if (command[0]=='1' || command[0]=='0')
					{
						inputarray[i]=atoi(command);
					}
					else
					{
						inputarray[i]=get(command[0]);
						
					}
					
				}
				
				
				/*
				Gets selector and stores in array biSelect
				*/
				
				int y;
				for (y=0; y<amtoutputs;y++)
				{
					fscanf(circuit, "%s", command);
					
					int x=get(command[0]);
					biSelect[y]=x;
				
					
				}
		
		
				/*
				Gets binary array biSelect and converts it to decimal
				by just seeing what value is 1 and raising it to that power
				*/
				
				int z;
				for (z=0;z<amtoutputs;z++)
				{
					if (biSelect[z]==1)
					{
						decimalSelect+=pow(2,(amtoutputs-1)-z);
					}
				
				}
				//Converts the  decimal number to a gray code by bit shifting and xoring
				decimalSelect=(decimalSelect>>1)^decimalSelect;
				
				//Jumps to the output variable
				fscanf(circuit, "%s", command);
				
				//stores the output char, with the correct array location selected by decimalSelect
				set(command[0],inputarray[decimalSelect],0);
			
							
			}
			
		}
		
		fclose(circuit);
		
		print();
		deleteList();
	
	
}


void function(char circuittext[],char inputtext[])
{
	
		FILE *inputs = fopen(inputtext,"r");
		
		int lines=0;
		int ch=0;
		while(!feof(inputs))
		{
			ch = fgetc(inputs);
			if(ch == '\n')
			{
			lines++;
			}
		}
		fclose(inputs);
		
		FILE *in = fopen(inputtext,"r");
	
		int e;
		for (e=0;e<lines;e++)
		{
			readInputFile(circuittext,in);
		}
	
}

int main(int argc, char** argv)
{	
	function(argv[1],argv[2]);
		
}
