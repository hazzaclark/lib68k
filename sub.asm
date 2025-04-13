    ORG    $1000

START:    
    MOVE.W  #7,D0          
    MOVE.W  #2,D1         
    SUB.W   D1,D0          
    
    MOVE.W  D0,D7          
    STOP    #$2700