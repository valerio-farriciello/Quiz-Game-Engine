//NAME: Valerio Farriciello

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct quiz_st {
    char* question;
    char* answer;
    struct quiz_st* next;
};

typedef struct quiz_st quiz;

/*Generates a random value between 0 and max_range -1.*/
int generate_random_value(int max_range){
    time_t seconds;
    seconds = time(NULL);
    /*the reason why I call rand function twice is to make it as more randomic as possible, because I've noticed that if I call the function twice
    and I have  (seconds + rand()) % max_range instead of random twice, it tents to give me 2 odd or 2 even values.*/
    return (rand() + seconds + rand()) % max_range;
}

/*Takes an array of integers and inserts all the values from 0 to n in a random position. 
I use this function in order to call the questions and the letters of the answer in a different order*/
void mix_indeces(int length, int array_to_randomize[]){
    int remaining_values = length;
    int rand_value;
    //assign all the indexes to -1
    for(int i = 0; i < length; i++){
        array_to_randomize[i] = -1;
    }
            
    //remaining value stores the length of the word
    while(remaining_values > 0){
        //generates a random value
        rand_value =  generate_random_value(length);
        //boolean: assuming that the index hasn't been used yet
        int not_used = 1;
        //checking
        for(int i = 0; i < length; i++){
            //if the index of the word is already in the array_to_randomize then set not_used to false
            if(array_to_randomize[i] == rand_value){
                not_used = 0;
                break;
            }
        }
        //if the flag is set to true it will assign the index to be displayed to the NEXT index of the array
        if(not_used){
            array_to_randomize[length - remaining_values] = rand_value;
            //if the index has been assigned to a position it will reduce the number of remaining values to assign
            remaining_values--;
        }
    }
}



int get_linked_list_from_file(char* file_name_provided, quiz* const head){
    int total_questions = 0;
    FILE *file;
    int i;
    //building the file path
    char file_name[30] = "./";
    strcat(file_name, file_name_provided);
    strcat(file_name, ".txt");

    /*this is just for readabily purposes*/
    quiz* temporary_quiz = head;


    if ((file = fopen(file_name, "r")) == NULL) {
        printf("ERROR: THE FILE %s DOESN'T EXIST OR IT CAN'T BE OPEN", file_name);
        return -1;
    }

    //loop until end of the file
    while (!feof(file)){
        total_questions++;
        //allocate space for both question and answer pointers
        (*temporary_quiz).question = malloc(sizeof(char) * 100);
        (*temporary_quiz).answer = malloc(sizeof(char) * 30);

        //reads the whole line INCLUDING \n if it's not the end of the file
        char* question_and_answer = malloc(sizeof(char) * 130);
        fgets(question_and_answer, 129, file);

        /*if it's not the end of the file "remove" \n 
        E.g.
        What is the capital of Italy? Rome\n\0
        
        after this operation is going to be:
        What is the capital of Italy? Rome\0\0
        */
        if(!feof(file)){
            question_and_answer[strlen(question_and_answer) - 2] = '\0';
        }

    	//boolean
        int do_question = 1;
        int index_for_answer = 0;

        //now it needs to split the question part and the answer part
        for(i = 0; i <= strlen(question_and_answer); i++){
            if(do_question){
                (*temporary_quiz).question[i] = question_and_answer[i];
                //copy the question until '?' is reached
                if(question_and_answer[i] == '?'){
                    i++;
                    //once '?' has been added, adds '\0' in order to terminate the string
                    (*temporary_quiz).question[i] = '\0';
                    //do_question is set to false and it will start assigning characters to the answer until the end including '\0'
                    do_question = 0; 
                }
            }
            else{
                //for the answer I'm using a different index
                (*temporary_quiz).answer[index_for_answer] = question_and_answer[i];
                index_for_answer++;
            }
        }

        //before continuing with the next quiz the answer is converted to uppercase
        for(i = 0; i < strlen((*temporary_quiz).answer); i++){
            if((*temporary_quiz).answer[i] >= 97 && (*temporary_quiz).answer[i] <= 122){
                (*temporary_quiz).answer[i] -= 32;
        }
    }


        //allocating space for a new struct
        quiz* connection_quiz = malloc(sizeof(quiz));
        //the current struct.next is pointing to the new struct
        (*temporary_quiz).next = connection_quiz;
        
        //moving the temporary_quiz pointer in order to assign value to the new struct 
        temporary_quiz = connection_quiz;
     }

    fclose(file);
    return total_questions;
}

void display_hint(char* answer_to_display, int level){
    int i;

    //used in level 4
    int indexes_to_display[2];

    //used in level 5
    int randomized_word_indexes[strlen(answer_to_display)];

    //used in level 6
    int rand_value;


    switch(level){
        case 1:
            printf("?\n");
            break;

        case 2:
            for(i = 0; i < strlen(answer_to_display); i++){
                    printf("_ ");
            }
            printf("\n");
            break;

        case 3:
            printf("%c ", answer_to_display[0]);
            for(i = 1; i < strlen(answer_to_display)-1; i++){
                printf("_ ");
            }
            printf("%c\n", answer_to_display[i]);
            break;

        case 4:
            indexes_to_display[0] = generate_random_value(strlen(answer_to_display));
            indexes_to_display[1] = generate_random_value(strlen(answer_to_display));

            for(i = 0; i < strlen(answer_to_display); i++){
                if(indexes_to_display[0] == i){
                    printf("%c ", answer_to_display[i]);
                }
                else if(indexes_to_display[1] == i){
                    printf("%c ", answer_to_display[i]);
                }
                else printf("_ ");
            }
            printf("\n");
            break;
        case 5:
            //generating an array with the word length but mixed indeces
            mix_indeces(strlen(answer_to_display), randomized_word_indexes);
            for(i = 0; i < strlen(answer_to_display); i++){
                //displays the word in "order" but using the actual index of the randomized_word_index[]
                printf("%c ", answer_to_display[randomized_word_indexes[i]]);
            }
            break;

            case 6:
                //it will generates a number between 1 and 5 and the function is going to be called again
                rand_value = generate_random_value(5) + 1;
                display_hint(answer_to_display, rand_value);
                break;

    }

}

int get_word_from_user(char word[]){
    char temp_array[256];
    int index = 0;
    /*The reason why I'm creating a new temporary array of 256 characters is that this will (almost) prevent any buffer overflow risk*/
    fgets(temp_array, 255, stdin);

    //Once I get the word into the temporary array, I will store the first characters until the 20th or less.
    while((temp_array[index] != '\n') && (index < 20)){
        word[index] = temp_array[index];
        index++;
    }
    
    word[index] = '\0';
    return strlen(word);
}

// It will retrieve the quiz from a given index from the actual order of the 'linked list'
quiz* get_quiz(int index, quiz* iterator){
    int current_index = 0;
    while((*iterator).next != NULL){
        if(current_index == index){
            return iterator;
        }
        current_index++;
        iterator = (*iterator).next;
    }
}

//validates user answer
int is_correct(char* answer, char* user_answer){
    //check the length first
    if(strlen(answer) != strlen(user_answer)) return 0;

    //converting the user answer to uppercase (the correct answer is already uppercase)
    for(int i = 0; i < strlen(user_answer); i++){
        if(user_answer[i] >= 97 && user_answer[i] <= 122){
            user_answer[i] -= 32;
        }
    }

    //comparing
    for(int i = 0; i < strlen(answer); i++){
        if(answer[i] != user_answer[i]){
            return 0;
        }
    }

    return 1;
}

void save_in_a_file(char* file_name, int number_of_questions, int level, int correct_answers){
    FILE* file;

   file = fopen ("./quiz_history.txt","w");
   if(file == NULL) return;
    
    fprintf(file, "NAME OF QUIZ FILE : %s\nNUMBER OF QUESTIONS ANSWERED CORRECTLY: %d\nTOTAL NUMBER OF QUESTIONS: %d\nLEVEL : %d",
    file_name, correct_answers, number_of_questions, level);
    
   fclose (file);
}

int main(void){
    //allocating space for the first quiz structure
    quiz* head = malloc(sizeof(quiz));
    int length_of_answer_from_user;
    char answer_from_user[21];
    int number_of_questions;
    char file_name_provided[21];
    //getting the file name
    while(1){
        printf("\nName of the file (without .txt):\n");
        length_of_answer_from_user = get_word_from_user(file_name_provided);
        number_of_questions = get_linked_list_from_file(file_name_provided, head);
        if(number_of_questions != -1) break;

    }
    //an array that will set the order in which the questions will be asked
    int order_of_questions[number_of_questions];
    mix_indeces(number_of_questions, order_of_questions);

    //not initialized because it will just be pointing to an already created quiz struct once get_quiz function will be called.
    quiz* current_quiz;
    int level = -1;
    int correct_answers = 0;
    //an array that will store in order the indeces of the wrong answers
    int index_wrong_answer[number_of_questions];

    printf("WELCOME! YOU HAVE %d QUESTIONS TO ANSWER:\n", number_of_questions);

    //getting the level
    while(1){
        printf("Select the level (1 to 6):\n");
        length_of_answer_from_user = get_word_from_user(answer_from_user);
        /*Since I'm using the same function to get the values from the user and since the already declared array is made of 'char', I need
        to convert the first character (since I only need a number) from digit to actual number*/
        answer_from_user[0] -= '0'; 
        if((length_of_answer_from_user == 1) && (answer_from_user[0] >= 1 && answer_from_user[0] <= 6)){
            level = answer_from_user[0];
            break;
        }
    }
    printf("\n\n");
    //starting the game
    for(int i = 0; i < number_of_questions; i++){
        //getting the quiz to ask based on the random order previously generated
        current_quiz =  get_quiz(order_of_questions[i], head);
        printf("%s\n", (*current_quiz).question);
        //displays the hint based on the level
        display_hint((*current_quiz).answer, level);
        printf("\nInput a word (max 20)\n");
        length_of_answer_from_user = get_word_from_user(answer_from_user);
        //checks if the word is correct, if not it will be adding the index of the current quiz to the array that will be (eventually) used later
        if(is_correct((*current_quiz).answer, answer_from_user)){
            correct_answers++;
        }
        else{
            index_wrong_answer[i - correct_answers] = order_of_questions[i];
        }
        printf("\n\nYOU HAVE ANSWERED %d QUESTION/S\nCURRENT SCORE %d/%d\n\n", (i+1), correct_answers, number_of_questions);   
    }
    
    save_in_a_file(file_name_provided, number_of_questions, level, correct_answers);

    if(correct_answers < number_of_questions){
        printf("Do you wish to see the correct answer for the questions that you answered wrong? y/[n]");
        length_of_answer_from_user = get_word_from_user(answer_from_user);
        if((length_of_answer_from_user == 1) && (answer_from_user[0] == 'y' || answer_from_user[0] == 'Y')){
            int j;
            for(j = 0; j < (number_of_questions - correct_answers); j++){
                current_quiz = get_quiz(index_wrong_answer[j], head);
                printf("------------------------------------------------------------\n");
                printf("%s\n", (*current_quiz).question);
                printf("%s\n", (*current_quiz).answer);

            }
        }

    }
    else{
        printf("CONGRATULATIONS! ALL YOUR ANSWERS ARE CORRECT!");
    }
    
    return 0;
}