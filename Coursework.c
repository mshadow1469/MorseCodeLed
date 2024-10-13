#include <stdio.h>
#include "pico/stdlib.h"
#include <time.h>
#include <string.h>
#include "Includes/seven_segment.h"

#define BUTTON_PIN 16   // Defines the button on gpio pin 16
// 2 dimensional char array containg the dot and dash values required to produce morse code letters (A-Z)
char morse_code[26][5] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.." };
// Single dimensional char array containg letters (A-Z)
char alphabet[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
// char morse array set to empty string
char morse[5] = "";
// dot character defined as char
char dot[] = ".";
// dash character defined as char
char dash[] = "-";
// proccessLetters, welcome, clearMorseArray, getMorseArray,getMorseLength, method defined
char processLetters();
void welcome();
void clearMorseArray();
int getMorseLength();
void output_letter(char morse[5]);
void seven_segment_show(unsigned int number);
int count = 0;
// Count variable defined and set to 0
int main() {
    stdio_init_all();
    //  Initialises all stdio types
    gpio_init(BUTTON_PIN);
    // Initializes button's GPIO pin
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    // Sets gpio direction
    gpio_pull_down(BUTTON_PIN); //pull down the button pin towards ground
    clock_t start_t, end_t, unpressed_t, total_unpressed_t , start_unpressed_t, end_unpressed_t;
    // Defines time values when button is pressed and when it's released
    double total_t;
    int count = 0;
    welcome();
    // Calls welcome method to display welcome message and light up seven segment

    while (true) {
        bool pressed = gpio_get(BUTTON_PIN);
        // Boolean variable defined and set to the state of the button pin (true if button pressed)
        bool held;
        // Boolean variable held defined
        if (pressed){
            // If condition if button is pressed
            seven_segment_off();
            // Seven segment display is set to off
            start_t = clock(); // the time is set when the button is pressed
            held = true; //  Held variable is set to true because button has been pressed
            while(held){
                seven_segment_show(26); // A dash is displayed whilst the button is pressed
                held = gpio_get(BUTTON_PIN); //  The variable held is set to the current status of the button pin
                // While loop exits when held is false and button is released
            }
            seven_segment_off(); // Seven segment display turns off when button is released
            end_t = clock(); // Timer when button is released( time at when button is released)
            start_unpressed_t = clock(); // Timer set when button released (time after button is pressed)
            
            total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000; // Time calculated in mili seconds by dividing by (clocks per second * 1000)
            // Total time is calculated as double value (total time button is pressed)
            if (total_t < 250.000000){
                // If the total time is less then 250 ms the press is registered as a dot
                strcat(morse, dot); // dot value is added to morse array
                printf("%s \n", morse); // Array outputted to console
            } else if (total_t >= 250.000000 && total_t < 700.000000){
                // If the total time is less then 700 ms and greater then 250 msthe press is registered as a dash
                strcat(morse, dash); // Dash value added to morse array
                printf("%s \n", morse); // Array outputted to console
            } else{
                // If the total time is greater then 700 ms then press is registered as neither dot or dash
                seven_segment_show(27);
                sleep_ms(300);
                seven_segment_off();
                printf("This is an error input \n");
                // An 8 is displayed on the seven segment led for 300 ms
            }

        }
            
        end_unpressed_t = clock(); // Timer after button has been released

        total_unpressed_t = (double)(end_unpressed_t - start_unpressed_t) / CLOCKS_PER_SEC * 1000; // Time calculated in mili seconds by dividing by (clocks per second * 1000)
        // Total time button has not been pressed is calculated as double value
        // If the button has not been pressed 400 ms after releasing and the morse array is not empty
        if(total_unpressed_t > 400 && morse[0] != '\0'){
            // processLetters method is called and value is outputted
            if (processLetters() == '!'){
                printf("%s", "Invalid morse code \n");
            } else {
                printf("%c \n", processLetters());
            }
            // After proccessLetters method called, the morse array is cleared
            clearMorseArray();
        }
        sleep_ms(20); // Time delay of 20 ms before while loop end
        
    }

}

char processLetters(){
    for(int j = 0; j < 26; j++){
        // For loop to loop through 26 letters of alphabet
        if(strcmp(morse, morse_code[j]) == 0){
            // Morse array compared against the morse code values in the morse_code array at index j(if both arrays same then 0 returned)
            seven_segment_show(j);
            sleep_ms(500);
            seven_segment_off();
            // Alphabet letter at index j displayed to seven segment display for 500 ms
            return alphabet[j]; // Value in alphabet array at index j return (corresponding letter returned)
        }
    }
    return '!'; // If the morse code does'nt match any letters
}

void welcome(){
    // Welcome method displays welcome message to terminal and line to seven segment display for 1 second
    printf("%s\n", "WELCOME!");
    seven_segment_init();
    seven_segment_show(26);
    sleep_ms(1000);
    seven_segment_off();
}

int getMorseLength(){
    // Used to find the length of morse array, if value in array at count not null then count is incremented
    int count = 0;
    for(count = 0; count < 5; count++){
        if(morse[count] == '\0'){
            break;
        }
    }
    return count;
}

void clearMorseArray(){
    // Used to clear the morse array by setting each position in the array to null 
    int i = 0;
    for(i = 0; i < getMorseLength(); i++){
        morse[i] = '\0';
    }
    count = 0;
}