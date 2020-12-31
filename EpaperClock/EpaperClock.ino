
// the setup function runs once when you press reset or power the board
void setup()
{
	
    while (true)
    {
        
    }
}


void loop() 
{
	// this shall not be reached - if however, then reset here.
    void(* resetFunc) (void) = 0;//declare reset function at address 0
	resetFunc(); //call reset  
}

