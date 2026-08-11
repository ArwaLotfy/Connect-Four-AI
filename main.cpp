// =====================================================
//  Connect Four - SFML 3 GUI
//  Drop this file in as your new main.cpp.
//  All other files (Board, AlphaBeta, etc.) are unchanged.
// =====================================================

#include <SFML/Graphics.hpp>
#include <string>
#include <chrono>
#include <thread>

#include "../include/Board.h"
#include "../include/AlphaBeta.h"
#include "../include/Statistics.h"


// ─────────────────────────────────────────────
//  Layout constants
// ─────────────────────────────────────────────
static const int CELL_SIZE = 90;          // pixels per cell
static const int RADIUS = 36;          // coin radius
static const int TOP_PANEL = 110;         // header height (menu / status)
static const int WIN_W = COLS * CELL_SIZE;          // 630
static const int WIN_H = TOP_PANEL + ROWS * CELL_SIZE; // 650

// ─────────────────────────────────────────────
//  Colours
// ─────────────────────────────────────────────
static const sf::Color COL_BG{ 15,  15,  30 };
static const sf::Color COL_BOARD{ 30,  80, 180 };
static const sf::Color COL_EMPTY{ 15,  15,  30 };
static const sf::Color COL_PLAYER{ 230,  60,  60 };   // red
static const sf::Color COL_AI{ 230, 200,  30 };   // yellow
static const sf::Color COL_HOVER{ 180, 180, 180, 120 };
static const sf::Color COL_WIN_HL{ 255, 255, 255 };
static const sf::Color COL_TEXT{ 220, 220, 220 };

static const int AI_DEPTH = 5;

// ─────────────────────────────────────────────
//  Game state
// ─────────────────────────────────────────────
enum class Screen { MENU, CHOOSE_STARTER, PLAYING, GAME_OVER };

struct GameState
{
    Screen     screen = Screen::MENU;
    Board      board;
    Statistics stats;
    AlphaBeta* ai = nullptr;

    int  playerPiece = 1;
    int  aiPiece = 2;
    bool isPlayerTurn = true;
    bool aiThinking = false;

    std::string statusMsg;
    std::string resultMsg;

    int  hoverCol = -1;   // column the mouse is over

    // Track winning cells for highlight
    bool hasWin = false;
    int  winCells[4][2] = {};   // up to 4 cells

    void reset()
    {
        board = Board();
        stats = Statistics();
        delete ai;
        ai = new AlphaBeta(aiPiece, playerPiece);
        hasWin = false;
        hoverCol = -1;
        statusMsg = "";
        resultMsg = "";
    }
};

// ─────────────────────────────────────────────
//  Helpers: centred text
// ─────────────────────────────────────────────
void drawCentredText(sf::RenderWindow& win, const sf::Font& font,
    const std::string& str, unsigned size,
    sf::Color colour, float y)
{
    sf::Text t(font, str, size);
    t.setFillColor(colour);
    sf::FloatRect b = t.getLocalBounds();
    t.setOrigin({ b.position.x + b.size.x / 2.f,
                  b.position.y + b.size.y / 2.f });
    t.setPosition({ WIN_W / 2.f, y });
    win.draw(t);
}

// ─────────────────────────────────────────────
//  Draw a rounded-rect button, return bounds
// ─────────────────────────────────────────────
sf::FloatRect drawButton(sf::RenderWindow& win, const sf::Font& font,
    const std::string& label,
    float cx, float cy, float w, float h,
    sf::Color bg, sf::Color fg)
{
    sf::RectangleShape rect({ w, h });
    rect.setOrigin({ w / 2.f, h / 2.f });
    rect.setPosition({ cx, cy });
    rect.setFillColor(bg);
    rect.setOutlineColor(sf::Color(255, 255, 255, 60));
    rect.setOutlineThickness(1.f);
    win.draw(rect);

    sf::Text t(font, label, 22);
    t.setFillColor(fg);
    sf::FloatRect b = t.getLocalBounds();
    t.setOrigin({ b.position.x + b.size.x / 2.f,
                  b.position.y + b.size.y / 2.f });
    t.setPosition({ cx, cy });
    win.draw(t);

    return rect.getGlobalBounds();
}

// ─────────────────────────────────────────────
//  Draw the board grid + coins
// ─────────────────────────────────────────────
void drawBoard(sf::RenderWindow& win, const GameState& gs)
{
    // Board background
    sf::RectangleShape bg({ (float)WIN_W, (float)(ROWS * CELL_SIZE) });
    bg.setPosition({ 0.f, (float)TOP_PANEL });
    bg.setFillColor(COL_BOARD);
    win.draw(bg);

    // Hover column highlight
    if (gs.hoverCol >= 0 && gs.isPlayerTurn
        && gs.screen == Screen::PLAYING
        && gs.board.isValidMove(gs.hoverCol))
    {
        sf::RectangleShape hl({ (float)CELL_SIZE, (float)(ROWS * CELL_SIZE) });
        hl.setPosition({ (float)(gs.hoverCol * CELL_SIZE), (float)TOP_PANEL });
        hl.setFillColor(COL_HOVER);
        win.draw(hl);
    }

    // Cells
    for (int row = 0; row < ROWS; ++row)
    {
        for (int col = 0; col < COLS; ++col)
        {
            int cell = gs.board.getCell(row, col);

            // Board row 0 = bottom visually → flip
            float px = col * CELL_SIZE + CELL_SIZE / 2.f;
            float py = TOP_PANEL + (ROWS - 1 - row) * CELL_SIZE + CELL_SIZE / 2.f;

            sf::CircleShape circle(RADIUS);
            circle.setOrigin({ (float)RADIUS, (float)RADIUS });
            circle.setPosition({ px, py });

            if (cell == 0)
                circle.setFillColor(COL_EMPTY);
            else if (cell == gs.playerPiece)
                circle.setFillColor(COL_PLAYER);
            else
                circle.setFillColor(COL_AI);

            // Win highlight ring
            if (gs.hasWin)
            {
                for (int k = 0; k < 4; ++k)
                {
                    if (gs.winCells[k][0] == row && gs.winCells[k][1] == col)
                    {
                        circle.setOutlineColor(COL_WIN_HL);
                        circle.setOutlineThickness(4.f);
                    }
                }
            }

            win.draw(circle);
        }
    }

    // Column numbers at the bottom
    // (drawn inside the board area)
}

// ─────────────────────────────────────────────
//  Column from mouse x
// ─────────────────────────────────────────────
int mouseToCol(int mx)
{
    int col = mx / CELL_SIZE;
    if (col < 0 || col >= COLS) return -1;
    return col;
}

// ─────────────────────────────────────────────
//  Find winning cells (for highlight)
//  Returns true and fills cells[4][2] if found.
// ─────────────────────────────────────────────
bool findWinCells(const Board& board, int piece, int cells[4][2])
{
    // Horizontal
    for (int r = 0; r < ROWS; ++r)
        for (int c = 0; c <= COLS - 4; ++c)
            if (board.getCell(r, c) == piece && board.getCell(r, c + 1) == piece &&
                board.getCell(r, c + 2) == piece && board.getCell(r, c + 3) == piece)
            {
                for (int k = 0;k < 4;++k) { cells[k][0] = r;cells[k][1] = c + k; } return true;
            }

    // Vertical
    for (int c = 0; c < COLS; ++c)
        for (int r = 0; r <= ROWS - 4; ++r)
            if (board.getCell(r, c) == piece && board.getCell(r + 1, c) == piece &&
                board.getCell(r + 2, c) == piece && board.getCell(r + 3, c) == piece)
            {
                for (int k = 0;k < 4;++k) { cells[k][0] = r + k;cells[k][1] = c; } return true;
            }

    // Diagonal /
    for (int r = 0; r <= ROWS - 4; ++r)
        for (int c = 0; c <= COLS - 4; ++c)
            if (board.getCell(r, c) == piece && board.getCell(r + 1, c + 1) == piece &&
                board.getCell(r + 2, c + 2) == piece && board.getCell(r + 3, c + 3) == piece)
            {
                for (int k = 0;k < 4;++k) { cells[k][0] = r + k;cells[k][1] = c + k; } return true;
            }

    // Diagonal
    for (int r = 3; r < ROWS; ++r)
        for (int c = 0; c <= COLS - 4; ++c)
            if (board.getCell(r, c) == piece && board.getCell(r - 1, c + 1) == piece &&
                board.getCell(r - 2, c + 2) == piece && board.getCell(r - 3, c + 3) == piece)
            {
                for (int k = 0;k < 4;++k) { cells[k][0] = r - k;cells[k][1] = c + k; } return true;
            }

    return false;
}

// ─────────────────────────────────────────────
//  MAIN
// ─────────────────────────────────────────────
int main()
{
    sf::RenderWindow window(
        sf::VideoMode({ (unsigned)WIN_W, (unsigned)WIN_H }),
        "Connect Four  —  AI",
        sf::Style::Titlebar | sf::Style::Close
    );
    window.setFramerateLimit(60);

    // Load font — place "arial.ttf" (or any .ttf) in the same folder as the exe.
    // If missing we fall back to the built-in bitmap font (no TTF).
    sf::Font font;
    bool fontLoaded = font.openFromFile("arial.ttf");
    if (!fontLoaded)
        fontLoaded = font.openFromFile("C:/Windows/Fonts/arial.ttf");

    GameState gs;
    gs.ai = new AlphaBeta(gs.aiPiece, gs.playerPiece);

    // ── Button bounds (computed each frame where needed) ──────────────────
    sf::FloatRect btnStart, btnExit, btnPlayer, btnAI, btnMenu, btnReplay;

    while (window.isOpen())
    {
        // ── Event loop ────────────────────────────────────────────────────
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            // Mouse move → update hover column
            if (auto* mm = event->getIf<sf::Event::MouseMoved>())
            {
                gs.hoverCol = mouseToCol(mm->position.x);
            }

            if (auto* mc = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mc->button != sf::Mouse::Button::Left) continue;
                sf::Vector2f mp{ (float)mc->position.x, (float)mc->position.y };

                // ── MENU ──────────────────────────────────────────────
                if (gs.screen == Screen::MENU)
                {
                    if (btnStart.contains(mp))
                        gs.screen = Screen::CHOOSE_STARTER;
                    else if (btnExit.contains(mp))
                        window.close();
                }

                // ── CHOOSE STARTER ───────────────────────────────────
                else if (gs.screen == Screen::CHOOSE_STARTER)
                {
                    if (btnPlayer.contains(mp))
                    {
                        gs.reset();
                        gs.isPlayerTurn = true;
                        gs.statusMsg = "Your turn  (Red)";
                        gs.screen = Screen::PLAYING;
                    }
                    else if (btnAI.contains(mp))
                    {
                        gs.reset();
                        gs.isPlayerTurn = false;
                        gs.statusMsg = "AI is thinking...";
                        gs.screen = Screen::PLAYING;
                        gs.aiThinking = true;
                    }
                }

                // ── PLAYING — player click ────────────────────────────
                else if (gs.screen == Screen::PLAYING
                    && gs.isPlayerTurn
                    && !gs.aiThinking)
                {
                    // Only accept clicks inside the board area
                    if (mp.y >= TOP_PANEL)
                    {
                        int col = mouseToCol((int)mp.x);
                        if (col >= 0 && gs.board.isValidMove(col))
                        {
                            int row = gs.board.getNextOpenRow(col);
                            gs.board.dropPiece(row, col, gs.playerPiece);

                            if (gs.board.checkWin(gs.playerPiece))
                            {
                                findWinCells(gs.board, gs.playerPiece, gs.winCells);
                                gs.hasWin = true;
                                gs.resultMsg = "You win! \xF0\x9F\x8E\x89";
                                gs.screen = Screen::GAME_OVER;
                            }
                            else if (gs.board.isBoardFull())
                            {
                                gs.resultMsg = "It's a draw!";
                                gs.screen = Screen::GAME_OVER;
                            }
                            else
                            {
                                gs.isPlayerTurn = false;
                                gs.statusMsg = "AI is thinking...";
                                gs.aiThinking = true;
                            }
                        }
                    }
                }

                // ── GAME OVER ─────────────────────────────────────────
                else if (gs.screen == Screen::GAME_OVER)
                {
                    if (btnReplay.contains(mp))
                        gs.screen = Screen::CHOOSE_STARTER;
                    else if (btnMenu.contains(mp))
                        gs.screen = Screen::MENU;
                }
            }
        }

        // ── AI move (processed outside event loop to avoid blocking) ──────
        if (gs.aiThinking && gs.screen == Screen::PLAYING)
        {
            gs.aiThinking = false;

            int aiCol = gs.ai->getBestMove(gs.board, AI_DEPTH, gs.stats);
            int aiRow = gs.board.getNextOpenRow(aiCol);
            gs.board.dropPiece(aiRow, aiCol, gs.aiPiece);

            if (gs.board.checkWin(gs.aiPiece))
            {
                findWinCells(gs.board, gs.aiPiece, gs.winCells);
                gs.hasWin = true;
                gs.resultMsg = "AI wins!";
                gs.screen = Screen::GAME_OVER;
            }
            else if (gs.board.isBoardFull())
            {
                gs.resultMsg = "It's a draw!";
                gs.screen = Screen::GAME_OVER;
            }
            else
            {
                gs.isPlayerTurn = true;
                gs.statusMsg = "Your turn  (Red)";
            }
        }

        // ── Draw ──────────────────────────────────────────────────────────
        window.clear(COL_BG);

        // ── MENU SCREEN ──────────────────────────────────────────────────
        if (gs.screen == Screen::MENU)
        {
            drawCentredText(window, font, "CONNECT FOUR", 42,
                sf::Color(100, 160, 255), 80.f);
            drawCentredText(window, font, "Player  vs  AI", 22,
                COL_TEXT, 130.f);

            btnStart = drawButton(window, font, "Start Game",
                WIN_W / 2.f, 260.f, 220.f, 55.f,
                sf::Color(40, 120, 60), COL_TEXT);
            btnExit = drawButton(window, font, "Exit",
                WIN_W / 2.f, 340.f, 220.f, 55.f,
                sf::Color(100, 30, 30), COL_TEXT);
        }

        // ── CHOOSE STARTER ───────────────────────────────────────────────
        else if (gs.screen == Screen::CHOOSE_STARTER)
        {
            drawCentredText(window, font, "Who goes first?", 34,
                COL_TEXT, 80.f);

            btnPlayer = drawButton(window, font, "I go first  (Red)",
                WIN_W / 2.f, 230.f, 280.f, 60.f,
                sf::Color(160, 40, 40), COL_TEXT);
            btnAI = drawButton(window, font, "AI goes first  (Yellow)",
                WIN_W / 2.f, 320.f, 280.f, 60.f,
                sf::Color(140, 120, 20), COL_TEXT);
        }

        // ── PLAYING / GAME OVER ──────────────────────────────────────────
        else
        {
            // Status bar
            std::string topMsg = (gs.screen == Screen::GAME_OVER)
                ? gs.resultMsg
                : gs.statusMsg;
            sf::Color topCol = (gs.screen == Screen::GAME_OVER)
                ? sf::Color(100, 220, 100)
                : COL_TEXT;
            drawCentredText(window, font, topMsg, 26, topCol, 30.f);

            // Legend
            {
                // Red circle + label
                sf::CircleShape rc(9.f);
                rc.setFillColor(COL_PLAYER);
                rc.setPosition({ WIN_W / 2.f - 120.f, 55.f });
                window.draw(rc);
                sf::Text lt(font, "You", 18);
                lt.setFillColor(COL_PLAYER);
                lt.setPosition({ WIN_W / 2.f - 105.f, 54.f });
                window.draw(lt);

                sf::CircleShape yc(9.f);
                yc.setFillColor(COL_AI);
                yc.setPosition({ WIN_W / 2.f + 50.f, 55.f });
                window.draw(yc);
                sf::Text yt(font, "AI", 18);
                yt.setFillColor(COL_AI);
                yt.setPosition({ WIN_W / 2.f + 65.f, 54.f });
                window.draw(yt);
            }

            drawBoard(window, gs);

            // Column numbers
            for (int c = 0; c < COLS; ++c)
            {
                sf::Text ct(font, std::to_string(c), 16);
                ct.setFillColor(sf::Color(180, 180, 180, 180));
                ct.setPosition({ c * CELL_SIZE + CELL_SIZE / 2.f - 5.f,
                                  (float)(TOP_PANEL + ROWS * CELL_SIZE + 4) });
                // draw inside board instead:
                ct.setPosition({ c * CELL_SIZE + CELL_SIZE / 2.f - 5.f, 88.f });
                window.draw(ct);
            }

            // Game-over overlay buttons
            if (gs.screen == Screen::GAME_OVER)
            {
                // Semi-transparent overlay
                sf::RectangleShape overlay({ (float)WIN_W, (float)WIN_H });
                overlay.setFillColor(sf::Color(0, 0, 0, 160));
                window.draw(overlay);

                drawCentredText(window, font, gs.resultMsg, 46,
                    sf::Color(100, 220, 100), WIN_H / 2.f - 60.f);

                btnReplay = drawButton(window, font, "Play Again",
                    WIN_W / 2.f, WIN_H / 2.f + 20.f,
                    200.f, 52.f,
                    sf::Color(40, 100, 40), COL_TEXT);
                btnMenu = drawButton(window, font, "Main Menu",
                    WIN_W / 2.f, WIN_H / 2.f + 90.f,
                    200.f, 52.f,
                    sf::Color(50, 50, 100), COL_TEXT);
            }
        }

        window.display();
    }

    delete gs.ai;
    return 0;
}
