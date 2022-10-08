    beq x0, x0, LBL2
LBL1:
    addi x1, x2, 20
    addi x2, x3, 20
LBL2:
    beq x0, x0, LBL1
