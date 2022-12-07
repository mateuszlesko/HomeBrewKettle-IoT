#ifndef STATE_MACHINE

#define STATE_MACHINE
    
    typedef enum
    {
        RESET,
        OFFLINE,
        SETUP,
        ONLINE,
        IDLE,
        PROCCEED,
        FINISH
    
    }SystemState;

    typedef enum
    {
        RESETING,
        CONFIGURING,
        ESTABLISH_CONNECTION,
        GET_PROCEDURE,
        FINISH_PROCEDURE
    }SystemEvent;

    

#endif