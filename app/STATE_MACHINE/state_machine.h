#ifndef STATE_MACHINE_H

#define STATE_MACHINE_H
    
    typedef enum
    {
        RESET,
        SETUP,
        OFFLINE,
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