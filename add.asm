    ORG $1000          

START:
        MOVE.L #5, D0       
        MOVE.L #10, D1      
        ADD.L D1, D0        
        MOVE.L D0, $2000
        RTS

        STOP #$2700        
