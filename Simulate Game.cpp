#define isDown(b) input->buttons[b].isDown
#define isPressed(b) (input->buttons[b].isDown && input->buttons[b].changed)
#define released(b) (!input->buttons[b].isDown && input->buttons[b].changed)

internal int 
startMenu(Input* input)
{
    int choice = 0;//needs tweaking 
    //I need to come up with a way to figure out how to change the menu if there's a game or not
    //drawmenu(choice)
    //if W change choice
    //if S change choice
    return choice;
}

internal int 
startingSequence(Input* input)
{
    bool play = true;
    //All should be able to be skipped with non-L and non-R buttons
    //Gamefreak logo and animation
    while (play)
    {
        if (isDown(BUTTON_E) || isDown(BUTTON_Q)) break;
    }
    
    //opening animation and title
    while (play)
    {
        if (isDown(BUTTON_E) || isDown(BUTTON_Q)) break;
    }

    //title screen 
    while (play)
    {
        if (isDown(BUTTON_E) || isDown(BUTTON_Q)) break;
    }

    //menu screen
    return startMenu(input);
}

internal void
simulateGame(Input* input)
{
    
}