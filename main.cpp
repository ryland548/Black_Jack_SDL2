#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "./Code/card.cpp"
#include "./Code/hitButton.cpp"
#include "./Code/standButton.cpp"
#include "./Code/startButton.cpp"
#include "./Code/restartButton.cpp"
#include "./Code/scoreText.cpp"
#include "./Code/stateText.cpp"
#include "./Code/label.cpp"
#include "./Code/moneyText.cpp"
#include "./Code/betText.cpp"
#include "./Code/betButton.cpp"

void redistributeCards(std::vector<Card> &cards, SDL_Renderer* renderer, std::vector<Card> &playerCards, std::vector<Card> &computerCards) {
    for (int i = 1; i < 5; i += 1) {
        std::string suit = "";
        switch (i) {
            case 1:
                suit = "H";
                break;
            case 2:
                suit = "D";
                break;
            case 3:
                suit = "C";
                break;
            case 4:
                suit = "S";
                break;
        };
        Card aceCard = Card();
        aceCard.cardValue = 11;
        aceCard.suit = suit;
        aceCard.cardId = "A";
        Card kingCard = Card();
        kingCard.cardValue = 10;
        kingCard.suit = suit;
        kingCard.cardId = "K";
        Card queenCard = Card();
        queenCard.cardValue = 10;
        queenCard.suit = suit;
        queenCard.cardId = "Q";
        Card jackCard = Card();
        jackCard.cardValue = 10;
        jackCard.suit = suit;
        jackCard.cardId = "J";
        for (int j = 2; j < 11; j += 1) {
            Card numberCard = Card();
            numberCard.cardValue = j;
            numberCard.suit = suit;
            numberCard.cardId = std::to_string(j);
            cards.push_back(numberCard);
        }
        cards.push_back(aceCard);
        cards.push_back(kingCard);
        cards.push_back(queenCard);
        cards.push_back(jackCard);
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(cards.begin(), cards.end(), gen);
    for (int i = 0; i < 2; i += 1) {
        Card drawnCard = cards.back();
        drawnCard.x = 50 + (i * 80);
        drawnCard.y = 500;
        drawnCard.visible = false;
        cards.pop_back();
        std::uniform_int_distribution<> dist(1, 5);
        drawnCard.load(renderer, dist(gen));
        playerCards.push_back(drawnCard);
    }
    for (int i = 0; i < 2; i += 1) {
        Card drawnCard = cards.back();
        drawnCard.x = 50 + (i * drawnCard.gap);
        drawnCard.y = 100;
        drawnCard.visible = false;
        cards.pop_back();
        std::uniform_int_distribution<> dist(1, 5);
        drawnCard.load(renderer, dist(gen));
        computerCards.push_back(drawnCard);
    }
}

void playerWin(bool &gameOver, StateText &stateText, SDL_Renderer* renderer, int &betAmount, int &money) {
    gameOver = true;
    std::string text = "You Won.";
    stateText.changeText(text, renderer);
    money += betAmount * 2;
    betAmount = 0;
}

void dealerWin(bool &gameOver, StateText &stateText, SDL_Renderer* renderer, int &betAmount) {
    gameOver = true;
    std::string text = "Dealer Won.";
    stateText.changeText(text, renderer);
    betAmount = 0;
}

void tie(bool &gameOver, StateText &stateText, SDL_Renderer* renderer, int &betAmount, int &money) {
    gameOver = true;
    std::string text = "Push.";
    stateText.changeText(text, renderer);
    money += betAmount;
}

std::string calculateWin(int &playerScore, int &computerScore) {
    if (playerScore > computerScore) {
        return "P";
    } else if (playerScore == computerScore) {
        return "T";
    } else {
        return "D";
    }
}

void updateComputerScore(int &computerScore, std::vector<Card> &computerCards, bool &computerBust) {
    computerScore = 0;
    int aceCount = 0;
    for (Card &card: computerCards) {
        if (card.visible) {
            if (card.cardId[0] != 'A') {
                computerScore += card.cardValue;
            } else {
                aceCount += 1;
            }
        }
    }
    for (int i = 0; i < aceCount; i++) {
        if (computerScore + 11 <= 21) {
            computerScore += 11;
        } else {
            computerScore += 1;
        }
    }
    if (computerScore > 21) {
        computerBust = true;
    }
}

void updatePlayerScore(int &playerScore, std::vector<Card> &playerCards, bool &playerBust) {
    playerScore = 0;
    int aceCount = 0;
    for (Card &card: playerCards) {
        if (card.cardId[0] != 'A') {
            playerScore += card.cardValue;
        } else {
            aceCount += 1;
        }
    }
    for (int i = 0; i < aceCount; i++) {
        if (playerScore + 11 <= 21) {
            playerScore += 11;
        } else {
            playerScore += 1;
        }
    }
    if (playerScore > 21) {
        playerBust = true;
    }
}

void hitPlayer(std::vector<Card> &playerCards, std::vector<Card> &cards, SDL_Renderer* renderer, int &playerScore, bool &playerBust) {
    Card drawnCard = cards.back();
    drawnCard.x = playerCards.back().x + drawnCard.gap;
    drawnCard.y = 500;
    cards.pop_back();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 5);
    drawnCard.load(renderer, dist(gen));
    playerCards.push_back(drawnCard);
    updatePlayerScore(playerScore, playerCards, playerBust);
}

void hitComputer(std::vector<Card> &computerCards, std::vector<Card> &cards, SDL_Renderer* renderer, int &computerScore, bool &computerBust) {
    Card drawnCard = cards.back();
    drawnCard.x = computerCards.back().x + drawnCard.gap;
    drawnCard.y = 100;
    cards.pop_back();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 5);
    drawnCard.load(renderer, dist(gen));
    computerCards.push_back(drawnCard);
    updateComputerScore(computerScore, computerCards, computerBust);
}

void stand(int &turn, int &playerScore, std::vector<Card> &playerCards, bool &playerBust) {
    turn = 1;
    updatePlayerScore(playerScore, playerCards, playerBust);
}

int main() {
    int money = 100;
    int betAmount = 0;
    bool betConfirmed = false;
    bool openedCards = false;
    int turn = 0; // 0 = player 1 = computer
    int screenWidth = 1050;
    int screenHeight = 800;
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL Could not initalize. SDL Error: " << SDL_GetError() << '\n';
        return 1;
    }
    SDL_Window* window = SDL_CreateWindow(
        "Black Jack",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        screenWidth,
        screenHeight,
        SDL_WINDOW_SHOWN
    );
    if (window == nullptr) {
        std::cerr << "Window could not be created. SDL Error: " << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created. SDL Error: " << SDL_GetError() << '\n';
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    TTF_Init();
    std::vector<Card> cards;
    for (int i = 1; i < 5; i += 1) {
        std::string suit = "";
        switch (i) {
            case 1:
                suit = "H";
                break;
            case 2:
                suit = "D";
                break;
            case 3:
                suit = "C";
                break;
            case 4:
                suit = "S";
                break;
        };
        Card aceCard = Card();
        aceCard.cardValue = 11;
        aceCard.suit = suit;
        aceCard.cardId = "A";
        Card kingCard = Card();
        kingCard.cardValue = 10;
        kingCard.suit = suit;
        kingCard.cardId = "K";
        Card queenCard = Card();
        queenCard.cardValue = 10;
        queenCard.suit = suit;
        queenCard.cardId = "Q";
        Card jackCard = Card();
        jackCard.cardValue = 10;
        jackCard.suit = suit;
        jackCard.cardId = "J";
        for (int j = 2; j < 11; j += 1) {
            Card numberCard = Card();
            numberCard.cardValue = j;
            numberCard.suit = suit;
            numberCard.cardId = std::to_string(j);
            cards.push_back(numberCard);
        }
        cards.push_back(aceCard);
        cards.push_back(kingCard);
        cards.push_back(queenCard);
        cards.push_back(jackCard);
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(cards.begin(), cards.end(), gen);
    std::vector<Card> playerCards;
    std::vector<Card> computerCards;
    for (int i = 0; i < 2; i += 1) {
        Card drawnCard = cards.back();
        drawnCard.x = 50 + (i * 80);
        drawnCard.y = 500;
        drawnCard.visible = false;
        cards.pop_back();
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(1, 5);
        drawnCard.load(renderer, dist(gen));
        playerCards.push_back(drawnCard);
    }
    for (int i = 0; i < 2; i += 1) {
        Card drawnCard = cards.back();
        drawnCard.x = 50 + (i * drawnCard.gap);
        drawnCard.y = 100;
        drawnCard.visible = false;
        cards.pop_back();
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(1, 5);
        drawnCard.load(renderer, dist(gen));
        computerCards.push_back(drawnCard);
    }
    HitButton hitButton = HitButton();
    if (!hitButton.load(renderer)) {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        TTF_Quit();
    }
    StandButton standButton = StandButton();
    if (!standButton.load(renderer)) {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        TTF_Quit();
    }
    StartButton startButton = StartButton();
    if (!startButton.load(renderer)) {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        TTF_Quit();
    }
    RestartButton restartButton = RestartButton();
    if (!restartButton.load(renderer)) {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        TTF_Quit();
    }
    ScoreText playerScoreText = ScoreText();
    if (!playerScoreText.load(renderer)) {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        TTF_Quit();
    }
    ScoreText computerScoreText = ScoreText();
    computerScoreText.y = 130;
    if (!computerScoreText.load(renderer)) {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        TTF_Quit();
    }
    MoneyText moneyText = MoneyText();
    if (!moneyText.load(renderer)) {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        TTF_Quit();
    }
    BetText betText = BetText();
    if (!betText.load(renderer)) {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        TTF_Quit();
    }
    StateText stateText = StateText();
    if (!stateText.load(renderer)) {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        TTF_Quit();
    }
    LabelText label = LabelText();
    if (!label.load(renderer)) {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        TTF_Quit();
    }
    BetButton betButton1 = BetButton();
    betButton1.buttonLabel = "$1";
    betButton1.y = 670;
    betButton1.x = 555;
    if (!betButton1.load(renderer)) {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        TTF_Quit();
    }
    BetButton betButton10 = BetButton();
    betButton10.buttonLabel = "$10";
    betButton10.y = 600;
    betButton10.x = 555;
    if (!betButton10.load(renderer)) {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        TTF_Quit();
    }
    BetButton betButton100 = BetButton();
    betButton100.buttonLabel = "$100";
    betButton100.y = 670;
    if (!betButton100.load(renderer)) {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        TTF_Quit();
    }
    BetButton betButton1000 = BetButton();
    betButton1000.buttonLabel = "$1000";
    betButton1000.y = 600;
    if (!betButton1000.load(renderer)) {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        TTF_Quit();
    }
    BetButton betButton10000 = BetButton();
    betButton10000.buttonLabel = "$10000";
    betButton10000.y = 530;
    if (!betButton10000.load(renderer)) {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        TTF_Quit();
    }
    BetButton betButtonRestart = BetButton();
    betButtonRestart.buttonLabel = "Delete";
    if (!betButtonRestart.load(renderer)) {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        TTF_Quit();
    }
    bool gameOver = false;
    int playerScore = 0;
    bool playerBust = false;
    bool computerBust = false;
    int computerScore = 0;
    bool running = true;
    SDL_Event event;
    int mouseX = 0;
    int mouseY = 0;
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Mixer Audio could not be created. SDL Error: " << Mix_GetError() << '\n';
    }
    Mix_Music* bgm = Mix_LoadMUS("./Assets/Audio/jazz.mp3");
    if (!bgm) {
        std::cerr << "Failed to load music: " << Mix_GetError() << std::endl;
    }
    Mix_PlayMusic(bgm, -1);
    while (running) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                running = false;
                break;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN && betConfirmed) {
                if (hitButton.isClicked(mouseX, mouseY) && turn == 0 && !gameOver) {
                    hitPlayer(playerCards, cards, renderer, playerScore, playerBust);
                    if (playerBust) {
                        stand(turn, playerScore, playerCards, playerBust);
                    }
                }
                if (standButton.isClicked(mouseX, mouseY) && turn == 0 && !gameOver) {
                    stand(turn, playerScore, playerCards, playerBust);
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN && !betConfirmed) {
                if (betAmount <= money) {
                    if (betAmount != 0) {
                        if (startButton.isClicked(mouseX, mouseY) && turn == 0 && !gameOver) {
                            betConfirmed = true;
                            money -= betAmount;
                            std::string text = " ";
                            stateText.changeText(text, renderer);
                        }
                    } else {
                        std::string text = "Must Bet";
                        stateText.changeText(text, renderer);
                    }
                } else {
                    std::string text = "Must be lower";
                    stateText.changeText(text, renderer);
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN && gameOver) {
                if (restartButton.isClicked(mouseX, mouseY) && gameOver) {
                    std::string text = "Make a Bet: ";
                    stateText.changeText(text, renderer);
                    gameOver = false;
                    betConfirmed = false;
                    betAmount = 0;
                    turn = 0;
                    openedCards = false;
                    playerCards.clear();
                    computerCards.clear();
                    redistributeCards(cards, renderer, playerCards, computerCards);
                    playerScore = 0;
                    computerScore = 0;
                    playerBust = false;
                    computerBust = false;
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN && !betConfirmed) {
                if (betButton1.isClicked(mouseX, mouseY) && turn == 0) {
                    betAmount += 1;
                }
                if (betButton10.isClicked(mouseX, mouseY) && turn == 0) {
                    betAmount += 10;
                }
                if (betButton100.isClicked(mouseX, mouseY) && turn == 0) {
                    betAmount += 100;
                }
                if (betButton1000.isClicked(mouseX, mouseY) && turn == 0) {
                    betAmount += 1000;
                }
                if (betButton10000.isClicked(mouseX, mouseY) && turn == 0) {
                    betAmount += 10000;
                }
                if (betButtonRestart.isClicked(mouseX, mouseY) && turn == 0) {
                    betAmount = 0;
                }
            }
        }
        if (betConfirmed && !openedCards) {
            for (Card &card: playerCards) {
                card.visible = true;
            }
            computerCards[0].visible = true;
            computerCards[1].visible = true;
            openedCards = true;
            updateComputerScore(computerScore, computerCards, computerBust);
            updatePlayerScore(playerScore, playerCards, playerBust);
            if (playerScore == 21 && computerScore != 21) {
                playerWin(gameOver, stateText, renderer, betAmount, money);
            }
            if (computerScore == 21 && playerScore == 21) {
                tie(gameOver, stateText, renderer, betAmount, money);
            }
            if (computerScore == 21 && playerScore != 21) {
                dealerWin(gameOver, stateText, renderer, betAmount);
            } else {
                computerCards[1].visible = false;
                updateComputerScore(computerScore, computerCards, computerBust);
            }
        }
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_SetRenderDrawColor(renderer, 47, 107, 77, 255);
        SDL_RenderClear(renderer);
        if (!betConfirmed) {
            betButton1.render(renderer);
            betButton10.render(renderer);
            betButton100.render(renderer);
            betButton1000.render(renderer);
            betButton10000.render(renderer);
            betButtonRestart.render(renderer);
        }
        label.render(renderer);
        playerScoreText.render(renderer, playerScore);
        computerScoreText.render(renderer, computerScore);
        moneyText.render(renderer, money);
        betText.render(renderer, betAmount);
        hitButton.render(renderer);
        standButton.render(renderer);
        if (!betConfirmed) {
            startButton.render(renderer);
        }

        for (Card &card: playerCards) {
            card.render(renderer);
        }
        for (Card &card: computerCards) {
            card.render(renderer);
        }
        if (gameOver) {
            restartButton.render(renderer);
        }
        stateText.render(renderer);
        if (turn == 1 && !gameOver) {
            if (!playerBust) {
                for (Card &card: computerCards) {
                    if (!card.visible) {
                        card.visible = true;
                    }
                }
                updateComputerScore(computerScore, computerCards, computerBust);
                if (computerScore < 17) {
                    hitComputer(computerCards, cards, renderer, computerScore, computerBust);
                }
                if (computerBust) {
                    playerWin(gameOver, stateText, renderer, betAmount, money);
                }
                if (!computerBust && computerScore >= 17) {
                    updateComputerScore(computerScore, computerCards, computerBust);
                    updatePlayerScore(playerScore, playerCards, playerBust);
                    std::string win = calculateWin(playerScore, computerScore);
                    if (win == "D") {
                        dealerWin(gameOver, stateText, renderer, betAmount);
                    } else if (win == "P") {
                        playerWin(gameOver, stateText, renderer, betAmount, money);
                    } else {
                        tie(gameOver, stateText, renderer, betAmount, money);
                    }
                }
            } else {
                for (Card &card: computerCards) {
                    if (!card.visible) {
                        card.visible = true;
                    }
                }
                updateComputerScore(computerScore, computerCards, computerBust);
                dealerWin(gameOver, stateText, renderer, betAmount);
            }
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(18);
    }
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    TTF_Quit();
    Mix_FreeMusic(bgm);
    Mix_CloseAudio();
    return 0;
}
