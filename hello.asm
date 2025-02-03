;----------------------------------------------------------------------------------
;                       COPYRIGHT (C) HARRY CLARK 2025
;----------------------------------------------------------------------------------
;                       LIB68K HELLO WORLD SAMPLE
;----------------------------------------------------------------------------------

    ORG     $0000           
    DC.L    $FFFFFE         
    DC.L    START          

    ORG     $1000          

MESSAGE:
    DC.B    'HELLO LIB68K', 0

START:
    LEA     MESSAGE, A0
    MOVE.B  #9, D0
    TRAP    #15

    MOVE.B  #0, D0
    TRAP    #15
    BRA     END

END:
    RTS
