    ORG $1000

START:
    LEA ARRAY, A0
    MOVE.L #0, D0
    MOVE.L #4, D1

SUM_LOOP:
    ADD.L (A0)+, D0
    SUBI.L #1, D1
    BNE SUM_LOOP

    MOVE.L D0, $2000

    STOP #$2700

ARRAY:
    DC.L 1, 2, 3, 4

END