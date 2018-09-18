// Jack Anderson (jja54)

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

//global vars
int user_wins = 0;
int comp_wins = 0;
int round_num = 0;

// prints a message, reads a line of input, and chops off the newline.
// call it like
//    read_line("Type something in: ", buffer, sizeof(buffer));
void read_line(const char* message, char* buffer, int length)
{
	printf(message);
	fgets(buffer, length, stdin);
	if(strlen(buffer) != 0)
		buffer[strlen(buffer) - 1] = 0;
}

// sees if two strings are the same, ignoring case.
// call it like
//    if(streq_nocase(one, two))
int streq_nocase(const char* a, const char* b)
{
	for(; *a && *b; a++, b++)
		if(tolower(*a) != tolower(*b))
			return 0;

	return *a == 0 && *b == 0;
}

int random_range(int lo, int hi)
{
	return rand() % (hi - lo + 1) + lo;
}

int comp_pick()
{
	int comp_num = random_range(1, 3); 
	return comp_num;
}

void print_wins()
{
	printf("You: %d, Computer: %d\n", user_wins, comp_wins);
}

void play_game()
{
	char user_attack[100];
	int comp_attack = 0;
	int user_attack_int = 0;
	printf("Round %d! ", round_num);
	read_line("Choose your attack: ", user_attack, sizeof(user_attack));
	comp_attack = comp_pick();
	//arbitrarily assign numbers for game comparisons sake
	if (streq_nocase(user_attack, "rock")) {
		user_attack_int = 1;
		if (comp_attack == 1) //rock-rock tie
			printf("The computer chooses rock. It's a tie! ");
		else if (comp_attack == 2){ //rock-paper loss
			printf("The computer chooses paper. You lose this round! ");
			comp_wins++;
		}
		else if (comp_attack == 3){ //rock-scissors win
			printf("The computer chooses scissors. You win this round! ");
			user_wins++;
		}
		print_wins();
	}
	
	else if (streq_nocase(user_attack, "paper")) {
		user_attack_int = 2;
		if (comp_attack == 1){ //paper-rock win
			printf("The computer chooses rock. You win this round! ");
			user_wins++;
		}
		else if (comp_attack == 2) //paper-paper tie
			printf("The computer chooses paper. It's a tie! ");
		else if (comp_attack == 3){ //paper-scissors loss
			printf("The computer chooses scissors. You lose this round! ");
			comp_wins++;
		}
		print_wins();	
	}
		
	else if (streq_nocase(user_attack, "scissors")) {
		user_attack_int = 3;
		if (comp_attack == 1){ //scissors-rock loss
			printf("The computer chooses rock. You lose this round! ");
			comp_wins++;
		}
		else if (comp_attack == 2){ //scissors-paper win
			printf("The computer chooses paper. You win this round! ");
			user_wins++;
		}
		else if (comp_attack == 3) //scissors-scissors tie
			printf("The computer chooses scissors. It's a tie! ");
			
		print_wins();	
	}
	else {
		printf("You didn't enter rock, paper, or scissors. Restarting round, choose again:\n");
		round_num--;
	}	
}
int main()
{
	srand((unsigned int)time(NULL));
	while(1){
		comp_wins = 0;
		user_wins = 0;
		round_num = 0;
		while (comp_wins < 3 && user_wins < 3)
		{
			round_num++;
			play_game();
		}
	
		char answer[100];
		if (comp_wins == 3){
			read_line("The computer wins the tournament! Play again? ", answer, sizeof(answer));
			if (streq_nocase(answer, "no")) {
				break;
			}
			else if (streq_nocase(answer, "n")) {
				break;
			}
		}
		else if (user_wins == 3){
			read_line("You win the tournament! Play again? ", answer, sizeof(answer));
			if (streq_nocase(answer, "no")) {
				break;
			}
			else if (streq_nocase(answer, "n")) {
				break;
			}
		}
	}
	
	return 0;
}