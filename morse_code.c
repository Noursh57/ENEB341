#include <stdio.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <string.h>

#define BuzzerPin 3

// This data type structure is used to store morse code value of single character
struct MORSE{
    char word;
    char *code;
};

// This is the lookup table for Morse code signals
struct MORSE morseDict[]=
{
    {'A',"01"}, {'B',"1000"}, {'C',"1010"}, {'D',"100"}, {'E',"0"}, 
    {'F',"0010"}, {'G',"110"}, {'H',"0000"}, {'I',"00"}, {'J',"0111"}, 
    {'K',"101"}, {'L',"0100"}, {'M',"11"}, {'N',"10"}, {'O',"111"}, 
    {'P',"0110"}, {'Q',"1101"}, {'R',"010"}, {'S',"000"}, {'T',"1"},
	{'U',"001"}, {'V',"0001"}, {'W',"011"}, {'X',"1001"}, {'Y',"1011"}, 
    {'Z',"1100"},{'1',"01111"}, {'2',"00111"}, {'3',"00011"}, {'4',"00001"}, 
    {'5',"00000"},{'6',"10000"}, {'7',"11000"}, {'8',"11100"}, {'9',"11110"},
    {'0',"11111"},{'?',"001100"}, {'/',"10010"}, {',',"110011"}, {'.',"010101"},
    {';',"101010"},{'!',"101011"}, {'@',"011010"}, {':',"111000"}
};


// This function is a lookup function where we lookup the morse code for each character in string
char *lookup(char key,struct MORSE *dict,int length)
{
    for (int i=0;i<length;i++)
    {
        if(dict[i].word==key){
            return dict[i].code;
        }
    }  
    return  dict[0].code; 
}

// initialisation
void setup()
{
    if (wiringPiSetup() == -1)
    { 
        printf("setup wiringPi failed !");
        return;
    }
    pinMode(BuzzerPin,OUTPUT);
}

void on(){
    digitalWrite(BuzzerPin,HIGH); 	
}

void off(){
    digitalWrite(BuzzerPin,LOW);
}

void beep(int dt){
    on();
    delay(dt);
    off();
    delay(dt);
}

void morsecode(char *code){
    int pause = 250;
    char *point = NULL;
    int length = sizeof(morseDict)/sizeof(morseDict[0]);
    for (int i=0;i<strlen(code);i++)
    {
        point=lookup(code[i],morseDict,length);
        /* Iterate j less than length of point*/
        for (int j=0;j<;j++){

            /*  TODO:
                check here if j of point is equal to 0 then pass pause/2 in beep function 
                else if point[j] is 1 then pass pause in beep function
            */
            delay(pause);
        }
    }
}

// This function convert each single character in upper case
int toupper(int c)
{
    /* TODO:
      * Capitalize the character c in this function
      * Hint: refer to https://www.asciitable.com/ for acii value of each character
      * convert to capital letter
   */
}

// This function converts string to all caps
char *strupr(char *str)
{
    char *orign=str;
    for (; *str!='\0'; str++)
        *str = toupper(*str);
    return orign;
}

int main(){
     /* TODO:
        * call setup() function.
        * Declare a character array of length 3 to take input for "SOS".
        * Create an infinite while loop
        * Inside while loop write the following lines of code 
        * print message "Please input the messenger:"
        * delay by 100 millisecond
        * take string input from keyboard 
        * process the the string received from keyboard using strupr function e.g. inputString = strupr(inputString)
        * print the processed string
        * delay by 100 millisecond
        * pass the processed string into the morsecode function e.g. morsecode(inputString)
    */
    return 0;
}
