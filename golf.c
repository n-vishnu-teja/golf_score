#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a new enum for scoring preferences
typedef enum {
    STROKE_PLAY,
    STABLEFORD
} ScoringPreference;

// Define structures
typedef struct Score {
    int hole_number;
    int score;
    int par;  // New: include par for each hole
    struct Score* next;
} Score;

typedef struct Rounds {
    char course_name[50];
    Score* scores;
    struct Rounds* next;
} Rounds;

typedef struct Player {
    char name[50];
    int handicap;
    Rounds* rounds;
    struct Player* next;
} Player;

// Function prototypes
Score* createScore(int hole_number, int score, int par);
Rounds* createRound(const char* course_name);
Player* createPlayer(const char* name, int handicap);
void addRound(Player* player, const char* course_name, int* scores, int* pars, ScoringPreference scoring_preference);
void displayAverageScore(Player* player);
void displayHandicap(Player* player);
void displayScoreTrends(Player* player);
void freeMemory(Player* players);
void displayInsightsTable(Player* player);

int main() {
    Player* players = NULL;

    while (1) {
        printf("\n1. Add Player\n");
        printf("2. Add Round\n");
        printf("3. Calculate Average Score\n");
        printf("4. Calculate Handicap\n");
        printf("5. View Insights\n");
        printf("0. Exit\n");

        int choice;
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                char name[50];
                int handicap;
                printf("Enter player name: ");
                scanf("%s", name);
                printf("Enter player handicap: ");
                scanf("%d", &handicap);
                Player* newPlayer = createPlayer(name, handicap);
                newPlayer->next = players;
                players = newPlayer;
                printf("Player %s added!\n", name);
                break;
            }

            case 2: {
                if (players == NULL) {
                    printf("No players available. Add a player first.\n");
                    break;
                }

                char name[50];
                printf("Enter player name: ");
                scanf("%s", name);

                Player* currentPlayer = players;
                while (currentPlayer != NULL && strcmp(currentPlayer->name, name) != 0) {
                    currentPlayer = currentPlayer->next;
                }

                if (currentPlayer != NULL) {
                    char course_name[50];
                    printf("Enter course name: ");
                    scanf("%s", course_name);

                    int scores[18];
                    int pars[18];  // New: array to store par for each hole
                    printf("Enter pars for 18 holes:\n");
                    for (int i = 0; i < 18; ++i) {
                        scanf("%d", &pars[i]);
                    }

                    printf("Enter scores for 18 holes:\n");
                    for (int i = 0; i < 18; ++i) {
                        scanf("%d", &scores[i]);
                    }

                    // Ask the user for scoring preference
                    int preference;
                    printf("Select scoring preference:\n");
                    printf("1. Stroke Play\n");
                    printf("2. Stableford\n");
                    scanf("%d", &preference);

                    ScoringPreference scoring_preference = (preference == 2) ? STABLEFORD : STROKE_PLAY;

                    addRound(currentPlayer, course_name, scores, pars, scoring_preference);
                    printf("Round added for %s at %s!\n", currentPlayer->name, course_name);
                } else {
                    printf("Player %s not found. Add the player first.\n", name);
                }
                break;
            }

            case 3: {
                if (players == NULL) {
                    printf("No players available. Add a player first.\n");
                    break;
                }

                char name[50];
                printf("Enter player name: ");
                scanf("%s", name);

                Player* currentPlayer = players;
                while (currentPlayer != NULL && strcmp(currentPlayer->name, name) != 0) {
                    currentPlayer = currentPlayer->next;
                }

                if (currentPlayer != NULL) {
                    displayAverageScore(currentPlayer);
                } else {
                    printf("Player %s not found.\n", name);
                }
                break;
            }

            case 4: {
                if (players == NULL) {
                    printf("No players available. Add a player first.\n");
                    break;
                }

                char name[50];
                printf("Enter player name: ");
                scanf("%s", name);

                Player* currentPlayer = players;
                while (currentPlayer != NULL && strcmp(currentPlayer->name, name) != 0) {
                    currentPlayer = currentPlayer->next;
                }

                if (currentPlayer != NULL) {
                    displayHandicap(currentPlayer);
                } else {
                    printf("Player %s not found.\n", name);
                }
                break;
            }

            case 5: {
                if (players == NULL) {
                    printf("No players available. Add a player first.\n");
                    break;
                }

                char name[50];
                printf("Enter player name: ");
                scanf("%s", name);

                Player* currentPlayer = players;
                while (currentPlayer != NULL && strcmp(currentPlayer->name, name) != 0) {
                    currentPlayer = currentPlayer->next;
                }

                if (currentPlayer != NULL) {
                    displayInsightsTable(currentPlayer);
                } else {
                    printf("Player %s not found.\n", name);
                }
                break;
            }

            case 0:
                freeMemory(players);
                printf("Exiting the program.\n");
                return 0;

            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }

    return 0;
}

Score* createScore(int hole_number, int score, int par) {
    Score* newScore = (Score*)malloc(sizeof(Score));
    newScore->hole_number = hole_number;
    newScore->score = score;
    newScore->par = par;  // New: set the par for the hole
    newScore->next = NULL;
    return newScore;
}

Rounds* createRound(const char* course_name) {
    Rounds* newRound = (Rounds*)malloc(sizeof(Rounds));
    strncpy(newRound->course_name, course_name, sizeof(newRound->course_name) - 1);
    newRound->scores = NULL;
    newRound->next = NULL;
    return newRound;
}

Player* createPlayer(const char* name, int handicap) {
    Player* newPlayer = (Player*)malloc(sizeof(Player));
    strncpy(newPlayer->name, name, sizeof(newPlayer->name) - 1);
    newPlayer->handicap = handicap;
    newPlayer->rounds = NULL;
    newPlayer->next = NULL;
    return newPlayer;
}

void addRound(Player* player, const char* course_name, int* scores, int* pars, ScoringPreference scoring_preference) {
    Rounds* newRound = createRound(course_name);
    for (int i = 0; i < 18; ++i) {
        Score* newScore = createScore(i + 1, scores[i], pars[i]);  // New: pass the par for the hole
        newScore->next = newRound->scores;
        newRound->scores = newScore;
    }
    newRound->next = player->rounds;
    player->rounds = newRound;

    // Handle scoring preference logic here
    if (scoring_preference == STABLEFORD) {
        int stableford_points = 0;
        for (int i = 0; i < 18; ++i) {
            int score = scores[i];
            int par = pars[i];

            if (score == 0) {
                // Hole not played
                stableford_points += 0;
            } else if (score == 1) {
                // Hole played in one stroke (Hole-in-One)
                stableford_points += 8;
            } else {
                // Calculate Stableford points based on score relative to par
                int relative_to_par = par - score;
                if (relative_to_par + 2 <= 0) {
                    stableford_points += 0; // Scored worse than par
                } else if (relative_to_par + 1 == 0) {
                    stableford_points += 1; // Scored bogey
                } else if (relative_to_par == 0) {
                    stableford_points += 2; // Scored par
                } else if (relative_to_par - 1 == 0) {
                    stableford_points += 3; // Scored birdie
                } else if (relative_to_par - 2 == 0) {
                    stableford_points += 4; // Scored eagle
                } else {
                    stableford_points += 5; // Scored better than eagle
                }
            }
        }

        printf("Stableford Points for %s at %s: %d\n", player->name, course_name, stableford_points);
    } else {
        // Stroke Play scoring logic
        int total_strokes = 0;
        for (int i = 0; i < 18; ++i) {
            total_strokes += scores[i];
        }

        printf("Total Strokes for %s at %s: %d\n", player->name, course_name, total_strokes);
    }
}

void displayAverageScore(Player* player) {
    if (player->rounds == NULL) {
        printf("No rounds played for %s.\n", player->name);
        return;
    }

    Rounds* currentRound = player->rounds;
    while (currentRound != NULL) {
        double total_score = 0;
        int total_holes = 0;

        Score* currentScore = currentRound->scores;
        while (currentScore != NULL) {
            total_score += currentScore->score;
            ++total_holes;
            currentScore = currentScore->next;
        }

        double average_score = total_score / total_holes;
        printf("Round: %s, Average Score: %.2f\n", currentRound->course_name, average_score);

        currentRound = currentRound->next;
    }
}

void displayHandicap(Player* player) {
    if (player->rounds == NULL) {
        printf("No rounds played for %s.\n", player->name);
        return;
    }

    Rounds* currentRound = player->rounds;
    while (currentRound != NULL) {
        double average_score = player->handicap;
        double total_handicap = player->handicap;

        Score* currentScore = currentRound->scores;
        while (currentScore != NULL) {
            average_score += currentScore->score - currentScore->par;
            ++total_handicap;
            currentScore = currentScore->next;
        }

        double handicap = average_score / total_handicap * 18;
        printf("Round: %s, Handicap: %.2f\n", currentRound->course_name, handicap);

        currentRound = currentRound->next;
    }
}

void displayScoreTrends(Player* player) {
    if (player->rounds == NULL) {
        printf("No rounds played for %s.\n", player->name);
        return;
    }

    printf("Score trends over time for %s:\n", player->name);

    Rounds* currentRound = player->rounds;
    int roundNumber = 1;
    while (currentRound != NULL) {
        printf("Round %d - %s: ", roundNumber, currentRound->course_name);

        Score* currentScore = currentRound->scores;
        while (currentScore != NULL) {
            printf("%d ", currentScore->score);
            currentScore = currentScore->next;
        }

        printf("\n");

        currentRound = currentRound->next;
        roundNumber++;
    }
}

void displayInsightsTable(Player* player) {
    if (player->rounds == NULL) {
        printf("No rounds played for %s.\n", player->name);
        return;
    }

    printf("\nInsights Table for %s:\n", player->name);
    printf("+------------------+------------------+------------------+------------------+\n");
    printf("| Course           | Round            | Average Score    | Handicap         |\n");
    printf("+------------------+------------------+------------------+------------------+\n");

    Rounds* currentRound = player->rounds;
    while (currentRound != NULL) {
        double total_score = 0;
        int total_holes = 0;

        Score* currentScore = currentRound->scores;
        while (currentScore != NULL) {
            total_score += currentScore->score;
            ++total_holes;
            currentScore = currentScore->next;
        }

        double average_score = total_score / total_holes;
        
        double average_handicap = player->handicap;
        double total_handicap = player->handicap;

        currentScore = currentRound->scores;
        while (currentScore != NULL) {
            average_handicap += currentScore->score - currentScore->par;
            ++total_handicap;
            currentScore = currentScore->next;
        }

        double handicap = average_handicap / total_handicap * 18;

        printf("| %-16s | %-16s | %-16.2f | %-16.2f |\n",
               currentRound->course_name, "Round", average_score, handicap);

        currentRound = currentRound->next;
    }

    printf("+------------------+------------------+------------------+------------------+\n");
}

void freeMemory(Player* players) {
    while (players != NULL) {
        Player* tempPlayer = players;
        players = players->next;

        Rounds* currentRound = tempPlayer->rounds;
        while (currentRound != NULL) {
            Rounds* tempRound = currentRound;
            currentRound = currentRound->next;

            Score* currentScore = tempRound->scores;
            while (currentScore != NULL) {
                Score* tempScore = currentScore;
                currentScore = currentScore->next;
                free(tempScore);
            }

            free(tempRound);
        }

        free(tempPlayer);
    }
}