// ================================================================================================
// Filename: "ClientEngine.cpp"
// ================================================================================================
// Author(s): Travis Smith
// Last Modified: April 04, 2014
// ================================================================================================
// This is the implementation file for the ClientEngine class. For class description see the header
// file "ClientEngine.h"
// ================================================================================================

#include "ClientEngine.h"

// Initialize Constants

// ===== Constructor ==============================================================================
// The constructor will set-up the GUI and initialize all game values for the client. It will start
// the engine and begin rendering and checking for data from server.
//
// Input: none
// Output: none
//  ===============================================================================================
ClientEngine::ClientEngine(void)
{
}

// ===== Destructor ===============================================================================
// The destructor will ensure all dynamically allocated memory is released.
// ================================================================================================
ClientEngine::~ClientEngine(void)
{
}

// ===== Run ======================================================================================
// This method will be the "infinite" loop that starts when the engine starts. Every iteration it
// will check for data from server, update the game state based on server data, respond to user
// input, render the main window, and send an update to the server.
//
// Input: none
// Output: none
// ================================================================================================
void ClientEngine::Run(void)
{
}

// ===== UpdateGameData ===========================================================================
// This method take a GameData structure that was received by the server and use it to update all
// gameState variables on the client. It will call the InterpolateXXXXX() methods. It will also 
// check for scores, and winning conditions.
//
// Input: none
// Output: none
// ================================================================================================
void ClientEngine::UpdateGameData()
{
}

// ===== Render ===================================================================================
// Method will render the main client window.
//
// Input: none
// Output: none
// ================================================================================================
void ClientEngine::Render(void)
{
}