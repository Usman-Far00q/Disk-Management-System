#include<iostream>
#include<string>
using namespace std;

struct block // structure (that wil be used as a link list) that will keep record of the memory location(s) of char memory[64] which have been alloted to a particular file
{
	char *next_mem_block;
	block *next;
};

struct File_Node    // basic structure that will represent our File (it will store all relevent information associated with the file)
{
	char File [26];
	char Extn [6];
    block *start; 
    block *end;
    int fSize;
    File_Node *next; 
};

//HASH FUNCTION to generate hash value
//hash value is generated by adding up all the ASCII values of characters in the 'name string' 
//And multiplying each ASCII value by 32 ^ (place value of that ASCII value)
//And taking the mod of the sum with 13(size of the hash table)  
unsigned int hash_function(string name)
{
	unsigned long int hash_val = 0; 
	
	for(int i = 0 ; i < name.length() ; i++ )
	{
		//" <<5 " means shifting a binary 5 places to the left same as multiplying the binary by 2 ^ 5 = 32
		//LINE # 31 can  also be written as: hash_val = (hash_value * 32) + name [i]
		hash_val = (hash_val << 5) + name[i]; 
	}
	
	return( hash_val % 13 );
}

void allocate_mem_to_file (File_Node *file_ptr , char memory[] , int size)
{
	block *block_pointer;
	int no_of_available_mem_blocks = 0;

	for(int i = 0 ; i < 64 ; i++)
	{
		if(memory[i] == 'E')
		{
			memory[i] = 'O';
			no_of_available_mem_blocks++;
			
			//when we are allocating the first ever block of memory to our file
			if(no_of_available_mem_blocks == 1) 
			{
				block_pointer = new block;
				
				//(memory+i) stores the address of the required value
				block_pointer -> next_mem_block = (memory + i);
				
				file_ptr -> start = block_pointer;
				block_pointer->next = NULL;
			}
			
			else
			{
				block_pointer -> next = new block;
				block_pointer = block_pointer -> next;
				block_pointer -> next_mem_block = memory + i;
			}
		}
		
		if(no_of_available_mem_blocks == size)
		{
			//To save the End index 
			file_ptr -> end = block_pointer;
			block_pointer -> next = NULL;
			return;
		}
	}
}

//To create a new file and place it acordingly in the hash_table
void create_file_node(string name , File_Node **ptr , int size , char memory[])
{
	int k = 0;
	File_Node *ptr1 = new File_Node;
	ptr1 -> fSize = size;
	allocate_mem_to_file(ptr1 , memory , size);
	int i;
	
	//For saving the name (without extention)
	for(i = 0 ; i < name.length() ; i++)
	{
		if(name[i] == '.')
		{
			break;
		}
		
		else
		{
			ptr1 -> File[i] = name[i];
		}
	}
	
	ptr1 -> File[i] = '\0';
	i = i + 1;
	
	//For saving the extention
	int j = 0;
	for(j = 0 ; i < name.length() ; i++)
	{
		ptr1 -> Extn[j] = name[i];
		j++;
	}
	ptr1 -> Extn[j] = '\0';
	string tempname = ptr1 -> Extn;
	
	//Generating the hash_value
	k = hash_function(name);
	
	if(ptr[k] == NULL) // this condition is true if this is the first ever file (in memory) being created against a particular hash_value
	{
		ptr1 -> next = NULL; 
		ptr[k] = ptr1;
		return;
	}
	
	File_Node *ptr2;
	
	for(ptr2 = ptr[k] ; ptr2 -> next != NULL ; ptr2 = ptr2 -> next) // ptr2 will be pointing to the very last file that has been
	{}                                                                 //"mapped against" the same hash value as that of our newly created file
	ptr1 -> next = NULL;
	ptr2 -> next = ptr1;		//appending the newly created file at the very end of the files "mapped against" the same hash_value
	
}

bool search_file_alredy_exits (string name , File_Node **ptr , File_Node **ptrfound)	//Search Function
{
	//Using the key (name) a hash value is generated which gives the index of the hash table
	int i = 0;
	i = hash_function(name);
	string tempname;
	
	//This loop searches the given value by comparing it with the elements of the link list present at that particular index (hash value)
	for(File_Node *ptr1 = ptr[i] ; ptr1 != NULL ; ptr1 = ptr1 -> next)
	{
		tempname = ptr1 -> File;
		tempname = tempname + ".";
		tempname = tempname + ptr1 -> Extn;
		if(tempname == name)
		{
			*ptrfound = ptr1;
			return(true);
		}
	}
	
	//If the file is not found the function returns "false"
	return(false);
	
}

void delete_file(string name , File_Node **ptr , File_Node *ptrfound)
{
	string tempname;
	int index;
	
	
	//Search Function is called to check if the file (to be deleted) is already present or not
	if(search_file_alredy_exits (name , ptr , &ptrfound))
	{
		
		block *block_ptr = ptrfound -> start;
		//the hash_function gives the hash value which is basically the index of the file to be deleted
		index = hash_function(name);	
		File_Node *prev_ptr = ptr[index];
		tempname = ptr[index]-> File;
		tempname = tempname + ".";
		tempname = tempname + ptr[index]-> Extn;
		if(tempname == name)
		{	
			block_ptr = (ptr[index] -> start);
			cout << "The file exists" << endl << "Delete Successfull" << endl;	
			//Loop to change the memory blocks (occupied by the file to be deleted) from 'Occupied' to 'Empty'
			for(int d = 0 ; d < ptr[index] -> fSize ; d++) 
			{
				*(block_ptr -> next_mem_block) = 'E';
				block_ptr = block_ptr -> next;
			}
			ptr[index] = ptr[index]->next;
			return;
		}
		for(File_Node *ptr1 = ptr[index]->next ; ptr1!= NULL ; ptr1 = ptr1 -> next)
		{	
			//the name of the file to be deleted is stored in tempname along with extension	
			tempname = ptr1 -> File;
			tempname = tempname + ".";
			tempname = tempname + ptr1 -> Extn;
			if(tempname == name)
			{
				block_ptr = ((ptr1) -> start);
				cout << "The file exists" << endl << "Delete Successfull" << endl;
				
				//Loop to change the memory blocks (occupied by the file to be deleted) from 'Occupied' to 'Empty'
				for(int d = 0 ; d < (ptr1) -> fSize ; d++) 
				{
					*(block_ptr -> next_mem_block) = 'E';
					block_ptr = block_ptr -> next;
				}
				
				//If the first element is deleted , its next element takes its place (whether it is another block or NULL)	
				if(ptr[index] == ptrfound)
					ptr[index] = ptrfound -> next;
			
				prev_ptr -> next = ptr1 -> next;
				delete ptr1;
				return;
			}
			prev_ptr = prev_ptr -> next;
		}
		
		return;
	}
	
	
	cout << "The file does not exist (!!!! Delete FAILED !!!!)" << endl;
}

bool check_if_memory_available(int size , char memory[])
{
	int no_of_available_mem_blocks = 0;
	for(int i = 0 ; i < 64 ; i++)
	{
		if(memory[i] == 'E')
		{
			no_of_available_mem_blocks ++;
		}
		
		if(no_of_available_mem_blocks == size)
		{
			return(true);
		}
	}
	
	return(false);
}

//To print the hash table
//Special characters have been used to give a graphical appearance to the hash table
void display_hash_tbl(File_Node **ptr)
{
	cout << "Displaying Hash Table " <<endl;
	cout << endl << "Hash Table" << endl << "________" << endl;
	
	for(int index = 0 ; index < 13 ; index++)
	{
		if(index < 10)
		cout << "|___" << index << "___|" << "-->";
		
		else
		cout << "|__" << index<< "___|" << "-->";
		
		for(File_Node *ptr1 = ptr[index] ; ptr1 != NULL ; ptr1 = ptr1 -> next)
		{
			cout << ptr1 -> File << "." << ptr1 -> Extn << " ---> ";
		}
		cout << "NULL" << endl;
	}
}

//Checks that the extention must be present
bool extention_check(string name)
{
	for(int i = 0 ; i < name.length() ; i++)
	{
		if(name[i] == '.')
		{
			return(false);
		}	
	}
	
	//If the extension is not present the function returns "true"
	return(true);
}

//Checks that the size of the extention must be less than 6
bool extention_size(string name)
{
	int count = 0;
	for(int i = 0 ; i < name.length() ; i++)
	{
		if(name[i] == '.')
		{
			for(int j = i + 1 ; j < name.length() ; j++)
			{
				++ count;
			}
			break;
		}	
	}

	if(count >= 6)
	{
		return(true);
	}
	
	//If the size is correct the function returns  "false"
	return(false);
}



//								**********************************		MAIN FUNCTION		*************************************
int main()
{
	system("color 0E"); 
	int size = 0;
	File_Node *ptrfound;	//used for deleting a file node
	
	File_Node **ptr = (new File_Node *[13]);	//Array of 13 File_Node*
	
	//Creating the hash table (Array of head pointers)
	for(int k = 0 ; k < 13 ; k++)
	{
		ptr[k] = NULL;
	}	
	
	char memory[64];	//Memory Array	
	
	//Declaring all the spaces as empty in the memory
	int i;
	for(i = 0 ; i < 64 ; i++)
	{
		memory[i] = 'E';
	}
	
	
	string name;
	bool wrong_choice = false;
	
	char a = 0;		//Loop controlling variable
	while(a != 5)	//Main Menu loop
	{
		//MAIN MENU
		wrong_choice = false;
		cout << "Press 0 - to Create a New File" << endl;
		cout << "Press 1 - to Search For a File " << endl;
		cout << "Press 2 - to Delete a File" << endl;
		cout << "Press 3 - to View Memory " <<  endl;
		cout << "Press 4 - to Display Hash Table" << endl;
		cout << "Press 5 - to Exit" << endl;
		cin>>a;
		
		if(a != 48 && a != 49 && a != 50 && a != 51 && a != 52 && a != 53)
		{
			wrong_choice = true;
			cout << "You entered a wrong choice (reverting back to main menu)"  << endl;
		}
		
		if(wrong_choice == false)
		{
			system("cls");
			
			//Since the variable 'a' is character type 
			//Hence the integer values are converted / compared by their respective ASCII values (0 = 48 , 1 = 49 etc) 
			if(a == 48)		//Create A New File
			{
				cout << "***** Create a file *****" << endl;
				
				//A point has been created for "goto" command which returns the compiler (from LINE # 364 and LINE # 371) to LINE # 353
				point1:
					
				cout << "Enter the name of the file you want to create (including extention)" << endl;
				cin.ignore();
				getline(cin,name);
				
				//Check if the extention is present in the name
				if(extention_check(name))
				{
					cout << "The extention is not present" << endl;
					system("pause");
					goto point1;	//Goes back to line 353
				}
				
				//Check if the name & extention are in the given size range (Limit for name = 25 and Limit for extention = 5)
				while(name.length() > 25 || extention_size(name))
				{
					cout << "NAME + EXTENTION is too large \nNAME + EXTENTION needs to be of max 25 + 5 characters!!!!!....\n Please try again" << endl;
					goto point1;	//Goes back to line 353
				}
				
				//Check that the file names must not be repeated
				if(search_file_alredy_exits (name , ptr , &ptrfound))
				{
					cout << "The file already exists" << endl;
				}
				
				//After passing all the checks user is prompted to enter the size of the file to be created
				else
				{
					cout << "Enter the size of the file" << endl;
					cin >> size;
					
					if(size == 0)
					{
						cout << "Size of a file cannot be zero" <<  endl;
					}
					
					else if(check_if_memory_available (size , memory))
					{
						create_file_node (name , ptr , size , memory);
						cout << "File Created" << endl;	
					}
					
					else
					{
						cout << "Cannot create file (no more space left in memory)" << endl;
					}
				}
				
			}
			
			if(a == 49)		//To Search A File
			{
				cout << "***** Search a file *****" << endl;
				cout << "Enter the name of the file to search for (including extension)" << endl;
				cin.ignore();
				getline(cin , name);
				
				if(search_file_alredy_exits (name , ptr , &ptrfound))
				{
					cout << "The file exists " << endl;
					
					cout << "File name " << ptrfound -> File << endl;
					cout << "File extension "<< ptrfound -> Extn << endl;
					cout << "File size "<< ptrfound -> fSize << endl;
					
					cout << "The file occupies memory index(s) : ";
					block *block_ptr = ptrfound -> start;
					
					//Loop to print the memory indexes occupied by this file
					for(int c = 0 ; c < (ptrfound -> fSize) ; c++)
					{
						cout << block_ptr -> next_mem_block - memory<<" ";
						block_ptr = block_ptr -> next;
					}	
					cout << endl;
				}
			
				else
				cout << "The file does not exist" << endl;	
			}
			
			if(a == 50)		//To Delete A File
			{
				cout << "***** Delete a file *****" << endl;
				cout << "Enter name of the file (Including Extension) to Delete" << endl;
				cin.ignore();
				getline(cin , name);
				delete_file(name , ptr , ptrfound);
			}
			
			
			if(a == 51)		//To View Memory 
			{
				cout << "***** View memory *****" << endl;
				for(int j = 0 ; j < 64 ; j ++)
				{
					//After printing values 8 indexes shift to next line
					if(!(j % 8))
					{
						cout << endl << memory[j] << "  ";
					}
					
					else
					{
						cout << memory[j] << "  ";
					}
				}
				cout << endl << endl;
			}
			
			if(a == 52)		//To Display Hash Table
			{
				display_hash_tbl(ptr);
			}
			
			if(a == 53)		//To Exit The Program
			{
				system("cls");					
				cout << "Thank You" << endl;
				return(0);
			}	
		}
		
		system("pause");
		system("cls");			
	}
	
	return 0;
}
