; Robert Judka
; Lab 8, LC3
;
; quad_poly.asm -- Evaluating a Quadratic Polynomial

        .ORIG    x3000

; Main
; R1 holds the counter
; R0 holds the value to multiply by (add the number as many times as the counter)
; R2 holds the total sums of all 3 coeeficients
;
	LD	R0, A		; R0 = A
	LD	R1, X_1		; R1 = X_1
	JSR	MULT		; R1 = A * X_1
	LD	R0, X_1		; R0 = X_1
	JSR	MULT		; TEMP = A * X_1^2
	ST	R1, TEMP	
	LD	R2, TEMP
	
	LD	R0, B		; R0 = B
	LD	R1, X_1		; R1 = X_1
	JSR	MULT		; R0 = B * X_1
	ADD	R2, R1, R2	; TEMP = A * X_1^2 + B * X_1
	LD	R0, C		; R0 = C
	ADD	R2, R2, R0	; Add C to TEMP
	ST	R2, Y_1		; Saves answer to polynomial in R2
	HALT

; poly(x) = -4*X^2 + 3*X - 5
;
A	.FILL	-4		; -4*X^2
B	.FILL	3		; + 3*X
C	.FILL	-5		; - 5

; To calculate: Y1 = poly(X1)
;
X_1	.FILL	-2
Y_1	.BLKW	1

; Multiply
; Set R1 = R1 * R0 (if R0 >= 0); set R1 = 0 if Y < 0.
;
; Property:
;    Y  < 0 implies product = 0, k = Y
;    Y >= 0 implies product = X * (Y-k) and 0 <= k <= Y
;

MULT     ST      R5, MU_R5	; Save registers
         ST      R4, MU_R4

; Establish product and k properties
;
         AND     R5, R5, 0      ; product = 0
         ADD     R4, R1, 0      ; counter k = X_1

; If Y < 0, product = 0; otherwise, maintain
; product = X * (Y - k) while k = Y, Y-1, ....
; Stop when k = 0
;

	 BRN	 NEG
MU_Loop	 BRZ     MU_Done        ; until k <= 0
         ADD     R5, R5, R0     ; product += X_1
         ADD     R4, R4, -1     ; k--
         BR      MU_Loop

NEG	 BRZ	MU_Done		; unil k >= 0
	 ADD	R5, R5, R0	; product -= X_1
	 ADD	R4, R4, -1	; k++
	 BR 	NEG

; After loop, R5 = X * Y (if Y >= 0) or 0 (if Y < 0)
; Set R1 to result, restore registers, and return
;
MU_Done  ADD     R1, R5, 0      ; R1 = X * Y 
         LD      R4, MU_R4      ; Restore registers
         LD      R5, MU_R5
         JMP     R7             ; return to caller

; Save area for registers
;
MU_R4   .BLKW   1
MU_R5   .BLKW   1
TEMP	.BLKW	1

.END