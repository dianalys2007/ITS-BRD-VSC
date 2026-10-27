#include "stack.h"
#include "display.h"
#include "calculator.h"
#include "token.h"
#include "utils.h"
#include "errornew.h"
#include "limits.h"



int processToken(T_token t) {
    int a, b; //Operanden vom stack
    char buffer[30]; //zur umwandlung von int zu string
    switch (t.tok) {

         
        case NUMBER: //auf stack legen
            push(t.val);
            break;
        
        case PLUS:
            int error;
            error = pop(&a); 
            if (error != ERROR_GOOD) {
                return error; 
            }

            error = pop(&b);
            if (error != ERROR_GOOD) {
                return error; 
            }

            if ((b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b)) { //Überlauf prüfen
               // printStdout("Overflow");
                return ERROR_MATH_ERROR;
            }

            
            push(b+a); //ergenbis auf stack legen
            break;

        case MINUS:
        
            error = pop(&a); 
            if (error != ERROR_GOOD) {
                return error; 
            }

            error = pop(&b);
            if (error != ERROR_GOOD) {
                return error; 
            }

            if ((a < 0 && b > INT_MAX + a) || (a > 0 && b < INT_MIN + a)) { //Überlauf prüfen
                // printStdout("Overflow");
                return ERROR_MATH_ERROR;// arithmetic overflow
            }
            

            push(b-a);
            break;

        case MULT:
            error = pop(&a); 
            if (error != ERROR_GOOD) {
                return error; 
            }

            error = pop(&b);
            if (error != ERROR_GOOD) {
                return error; 
            }
            

            if (a != 0 && b != 0) { //Überlauf prüfen
                if (b > INT_MAX / a || b < INT_MIN / a) {
                   // printStdout("Overflow");
                    return ERROR_MATH_ERROR;// arithmetic overflow
                }///
            }

            push(b*a);
            break;
            
        case DIV:
            error = pop(&a); 
            if (error != ERROR_GOOD) {
                return error; 
            }

            error = pop(&b);
            if (error != ERROR_GOOD) {
                return error; 
            }
            if (a == 0) { 
                // printStdout("null division");    
                return ERROR_DIV_ZERO;
            }

            if (b == INT_MIN && a == -1) {
                // printStdout("Overflow");
                return ERROR_MATH_ERROR;// arithmetic overflow
            }

            push(b/a);
            break;

        case PRT: //nocmal anschauen 
            error = pop(&a); 
            if (error != ERROR_GOOD) {
            
                return error; 
                
            }

            intToString(a, buffer); //int zu string umwandeln
            printStdout(buffer); //ausgabe auf dem display
            printStdout("\n"); 
            push(a);
            break;

        case CLEAR:
            int a; 
            while (!isEmpty()) {
                pop(&a);
            }
            clearStdout();
            break; 
            
        case SWAP: //die obersten zwei elemente tauschen 
            error = pop(&a); 
            if (error != ERROR_GOOD) {
                return error; 
            }

            error = pop(&b);
            if (error != ERROR_GOOD) {
                return error; 
            }
            push(a);
            push(b);
            break;
            
        case DOUBLE: //oberstes element duplizieren
            error = pop(&a); 
            if (error != ERROR_GOOD) {
            
                return error; 
                
            }
            push(a);
            push(a);
            
            break;

        case OVERFLOW: //fehler beim einlesen 
          //  printStdout("Input overflow");
            return ERROR_STACK_OVERFLOW;

        case PRT_ALL:
            if (isEmpty()) {
                // printStdout("Stack empty");
                return ERROR_STACK_UNDERFLOW;
            }

            int size = getSize();
            int value;
            char buffer[30];

            //int temp [STACK_SIZE];
            //int i = 0;

           
            for (int i = size - 1; i >= 0; i--) {
                if (peekAt(i, &value) == ERROR_GOOD) {
                
                    intToString(value, buffer);
                    printStdout(buffer);
                    printStdout("\n"); 
                }
                
            
            }
            break;      

        default:
            printStdout("Unexpected token");    
            


    } 
    return ERROR_GOOD;
}