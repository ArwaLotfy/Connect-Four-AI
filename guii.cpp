// =====================================================
//  Connect Four  -  SFML 3  (NO FONT REQUIRED)
//  All UI uses shapes and colors - zero text rendering
// =====================================================

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <SFML/Graphics.hpp>

#include "../include/Board.h"
#include "../include/AlphaBeta.h"
#include "../include/Statistics.h"

// ─────────────────────────────────────────────
//  Layout
// ─────────────────────────────────────────────
const int CELL_SIZE = 100;
const int WIDTH     = COLS * CELL_SIZE;           // 700
const int HEIGHT    = (ROWS + 1) * CELL_SIZE;     // 700
int gameMode = 0; // 1 = PvP, 2 = PvAI
// ─────────────────────────────────────────────
//  Player IDs
// ─────────────────────────────────────────────
const int EMPTY_CELL = 0;
const int HUMAN      = 1;
const int AI_PLAYER  = 2;

// ─────────────────────────────────────────────
//  Colours
// ─────────────────────────────────────────────
const sf::Color BG_COLOR    ( 20,  20,  40);
const sf::Color BOARD_COLOR ( 30, 100, 200);
const sf::Color EMPTY_COLOR ( 20,  20,  40);
const sf::Color HUMAN_COLOR (220,  50,  50);   // red
const sf::Color AI_COLOR    (220, 220,  50);   // yellow

// ─────────────────────────────────────────────
//  Draw a row of small circles as a turn indicator
//  in the status bar.  color = whose turn it is.
// ─────────────────────────────────────────────
void drawStatusBar(sf::RenderWindow& window,
                   bool gameOver, int winner,
                   int turn, bool aiThinking)
{
    // Background strip
    sf::RectangleShape bar({ (float)WIDTH, (float)CELL_SIZE });
    bar.setFillColor(sf::Color(10, 10, 25));
    bar.setPosition({ 0.f, 0.f });
    window.draw(bar);

    if (gameOver)
    {
        if (winner == HUMAN)
        {
            // Green banner = human wins
            sf::RectangleShape banner({ (float)WIDTH, (float)CELL_SIZE });
            banner.setFillColor(sf::Color(30, 160, 30));
            banner.setPosition({ 0.f, 0.f });
            window.draw(banner);

            // Trophy shape: big red circle in center
            sf::CircleShape trophy(30.f);
            trophy.setOrigin({ 30.f, 30.f });
            trophy.setFillColor(HUMAN_COLOR);
            trophy.setOutlineColor(sf::Color::White);
            trophy.setOutlineThickness(4.f);
            trophy.setPosition({ WIDTH / 2.f, CELL_SIZE / 2.f });
            window.draw(trophy);

            // Star points around it
            for (int i = 0; i < 5; ++i)
            {
                float angle = i * 72.f * 3.14159f / 180.f;
                sf::CircleShape star(6.f);
                star.setOrigin({ 6.f, 6.f });
                star.setFillColor(sf::Color::White);
                star.setPosition({
                    WIDTH / 2.f + std::cos(angle) * 50.f,
                    CELL_SIZE / 2.f + std::sin(angle) * 50.f
                });
                window.draw(star);
            }
        }
        else if (winner == AI_PLAYER)
        {
            // Red banner = AI wins
            sf::RectangleShape banner({ (float)WIDTH, (float)CELL_SIZE });
            banner.setFillColor(sf::Color(160, 30, 30));
            banner.setPosition({ 0.f, 0.f });
            window.draw(banner);

            // Big yellow circle = AI
            sf::CircleShape aiCircle(30.f);
            aiCircle.setOrigin({ 30.f, 30.f });
            aiCircle.setFillColor(AI_COLOR);
            aiCircle.setOutlineColor(sf::Color::White);
            aiCircle.setOutlineThickness(4.f);
            aiCircle.setPosition({ WIDTH / 2.f, CELL_SIZE / 2.f });
            window.draw(aiCircle);

            // X marks around it (AI wins)
            for (int i = 0; i < 4; ++i)
            {
                float angle = i * 90.f * 3.14159f / 180.f;
                sf::RectangleShape x1({ 20.f, 4.f });
                x1.setOrigin({ 10.f, 2.f });
                x1.setFillColor(sf::Color::White);
                x1.setRotation(sf::degrees(45.f));
                x1.setPosition({
                    WIDTH / 2.f + std::cos(angle) * 52.f,
                    CELL_SIZE / 2.f + std::sin(angle) * 52.f
                });
                window.draw(x1);
            }
        }
        else
        {
            // Grey banner = draw
            sf::RectangleShape banner({ (float)WIDTH, (float)CELL_SIZE });
            banner.setFillColor(sf::Color(80, 80, 80));
            banner.setPosition({ 0.f, 0.f });
            window.draw(banner);

            // Two equal circles = draw
            sf::CircleShape c1(22.f), c2(22.f);
            c1.setOrigin({ 22.f, 22.f });
            c2.setOrigin({ 22.f, 22.f });
            c1.setFillColor(HUMAN_COLOR);
            c2.setFillColor(AI_COLOR);
            c1.setPosition({ WIDTH / 2.f - 35.f, CELL_SIZE / 2.f });
            c2.setPosition({ WIDTH / 2.f + 35.f, CELL_SIZE / 2.f });
            window.draw(c1);
            window.draw(c2);
        }

        // "Press R" indicator: small R-shaped dots at bottom-right
        for (int i = 0; i < 3; ++i)
        {
            sf::CircleShape dot(5.f);
            dot.setOrigin({ 5.f, 5.f });
            dot.setFillColor(sf::Color(200, 200, 200, 180));
            dot.setPosition({ WIDTH - 40.f + i * 14.f, CELL_SIZE - 15.f });
            window.draw(dot);
        }
    }
    else if (aiThinking)
    {
        // Pulsing yellow dots = AI thinking
        for (int i = 0; i < 7; ++i)
        {
            sf::CircleShape dot(8.f);
            dot.setOrigin({ 8.f, 8.f });
            dot.setFillColor(sf::Color(220, 220, 50, 120 + i * 15));
            dot.setPosition({ WIDTH / 2.f - 60.f + i * 20.f,
                               CELL_SIZE / 2.f });
            window.draw(dot);
        }
    }
    else if (turn == HUMAN)
    {
        // Red disc preview in status bar = your turn
        // Arrow pointing down
        sf::ConvexShape arrow;
        arrow.setPointCount(3);
        arrow.setPoint(0, {  0.f,  0.f });
        arrow.setPoint(1, { 30.f,  0.f });
        arrow.setPoint(2, { 15.f, 22.f });
        arrow.setFillColor(HUMAN_COLOR);
        arrow.setPosition({ WIDTH / 2.f - 15.f, 10.f });
        window.draw(arrow);

        // Small red circle below arrow
        sf::CircleShape disc(16.f);
        disc.setOrigin({ 16.f, 16.f });
        disc.setFillColor(HUMAN_COLOR);
        disc.setPosition({ WIDTH / 2.f, CELL_SIZE / 2.f + 10.f });
        window.draw(disc);
    }
}

// ─────────────────────────────────────────────
//  Draw board
// ─────────────────────────────────────────────
void drawBoard(sf::RenderWindow& window, Board& board, int hoverCol,
               bool gameOver, int winner, int turn, bool aiThinking)
{
    window.clear(BG_COLOR);

    // Status bar
    drawStatusBar(window, gameOver, winner, turn, aiThinking);

    // Blue board
    sf::RectangleShape bg({ (float)WIDTH, (float)(ROWS * CELL_SIZE) });
    bg.setFillColor(BOARD_COLOR);
    bg.setPosition({ 0.f, (float)CELL_SIZE });
    window.draw(bg);

    // Hover highlight + preview disc
    if (!gameOver && turn == HUMAN &&
        hoverCol >= 0 && hoverCol < COLS && board.isValidMove(hoverCol))
    {
        sf::RectangleShape colHL({ (float)CELL_SIZE, (float)(ROWS * CELL_SIZE) });
        colHL.setFillColor(sf::Color(255, 255, 255, 30));
        colHL.setPosition({ (float)(hoverCol * CELL_SIZE), (float)CELL_SIZE });
        window.draw(colHL);

        sf::CircleShape preview(40.f);
        preview.setOrigin({ 40.f, 40.f });
        preview.setFillColor(sf::Color(220, 50, 50, 160));
        preview.setPosition({ hoverCol * CELL_SIZE + CELL_SIZE / 2.f,
                               CELL_SIZE / 2.f });
        window.draw(preview);

        // Arrow
        sf::ConvexShape arrow;
        arrow.setPointCount(3);
        arrow.setPoint(0, {  0.f,  0.f });
        arrow.setPoint(1, { 20.f,  0.f });
        arrow.setPoint(2, { 10.f, 15.f });
        arrow.setFillColor(sf::Color::White);
        arrow.setPosition({
            hoverCol * CELL_SIZE + CELL_SIZE / 2.f - 10.f,
            CELL_SIZE - 20.f
        });
        window.draw(arrow);
    }

    // Cells
    for (int r = 0; r < ROWS; ++r)
    {
        for (int c = 0; c < COLS; ++c)
        {
            sf::CircleShape circle(40.f);
            circle.setOrigin({ 40.f, 40.f });
            circle.setPosition({
                c * CELL_SIZE + CELL_SIZE / 2.f,
                (ROWS - 1 - r) * CELL_SIZE + CELL_SIZE / 2.f + CELL_SIZE
                });            int cell = board.getCell(r, c);
            if      (cell == HUMAN)     circle.setFillColor(HUMAN_COLOR);
            else if (cell == AI_PLAYER) circle.setFillColor(AI_COLOR);
            else                        circle.setFillColor(EMPTY_COLOR);

            window.draw(circle);
        }
    }
}

// ─────────────────────────────────────────────
//  main
// ─────────────────────────────────────────────
int main()
{
    sf::RenderWindow window(
        sf::VideoMode({ (unsigned)WIDTH, (unsigned)HEIGHT }),
        "Connect Four",
        sf::Style::Titlebar | sf::Style::Close
    );
    window.setFramerateLimit(60);

    Board      board;
    Statistics stats;
    AlphaBeta  ai(AI_PLAYER, HUMAN);

    int  turn     = HUMAN;
    bool over     = false;
    int  winner   = EMPTY_CELL;
    int  hoverCol = -1;
    bool aiThink  = false;

    while (window.isOpen())
    {
        // ── Events ────────────────────────────
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (auto* mm = event->getIf<sf::Event::MouseMoved>())
                hoverCol = mm->position.x / CELL_SIZE;

            if (!over && turn == HUMAN)
            {
                if (auto* mb = event->getIf<sf::Event::MouseButtonPressed>())
                {
                    if (mb->button == sf::Mouse::Button::Left)
                    {
                        int col = mb->position.x / CELL_SIZE;
                        if (board.isValidMove(col))
                        {
                            int row = board.getNextOpenRow(col);
                            board.dropPiece(row, col, HUMAN);

                            if (board.checkWin(HUMAN))
                            {
                                over = true; winner = HUMAN;
                            }
                            else if (board.isBoardFull())
                            {
                                over = true; winner = EMPTY_CELL;
                            }
                            else
                            {
                                turn    = AI_PLAYER;
                                aiThink = true;
                            }
                        }
                    }
                }
            }

            // R = restart
            if (auto* kp = event->getIf<sf::Event::KeyPressed>())
            {
                if (kp->code == sf::Keyboard::Key::R)
                {
                    board   = Board();
                    stats   = Statistics();
                    turn    = HUMAN;
                    over    = false;
                    winner  = EMPTY_CELL;
                    aiThink = false;
                }
            }
        }

        // ── AI turn ───────────────────────────
        if (!over && turn == AI_PLAYER && aiThink)
        {
            // Render "thinking" state first
            drawBoard(window, board, -1, false, EMPTY_CELL, AI_PLAYER, true);
            window.display();
            sf::sleep(sf::milliseconds(300));

            int col = ai.getBestMove(board, 5, stats);
            int row = board.getNextOpenRow(col);
            board.dropPiece(row, col, AI_PLAYER);

            if (board.checkWin(AI_PLAYER))
            {
                over = true; winner = AI_PLAYER;
            }
            else if (board.isBoardFull())
            {
                over = true; winner = EMPTY_CELL;
            }
            else
            {
                turn = HUMAN;
            }

            aiThink = false;
        }

        // ── Render ────────────────────────────
        drawBoard(window, board,
                  (!over && turn == HUMAN) ? hoverCol : -1,
                  over, winner, turn, aiThink);

        window.display();
    }

    return 0;
}
