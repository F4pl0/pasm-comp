# PicoComputer Assembly instructions list
| Instruction | Meaning | Opcode | Syntax | Example |
|-------------|---------|--------|--------|---------|
| MOV         | Moves numbers to address/register | 0000   | MOV x,y<br>MOV (x),n<br>MOV x,y,#z<br>MOV (x),(y),n | `MOV rax,15`  Moves 15 to `rax` register<br>`MOV (rax),5` Moves 5 to address stored in `rax`|
| ADD         | Adds two numbers | 0001 - With Constant<br>1001 - Without constant | ADD x,y,z | `ADD rax,rbx,5` - Adds value in `rbx` with 5 and stores it in `rax` register |
| SUB         | Subtracts two numbers | 0010 - With Constant<br>1010 - Without constant | SUB x,y,z | `SUB rax,rbx,5` - Subtracts the 5 from value stored in `rbx` register and stores it in `rax` register |
| MUL         | Multiplies two numbers | 0011 - With Constant<br>1011 - Without constant | MUL x,y,z | `MUL rax,rbx,5` - Multiples value stored in `rbx` with 5 and stores it in `rax` register |
| DIV         | Divides two numbers | 0100 - With Constant<br>1100 - Without constant | DIV x,y,z | `DIV rax,rbx,5` - Divides value stored in `rbx` register by 5 and stores the result in `rax` register |
| BEQ         | Branch if equal | 0101 | BEQ x,y,br_name | `BEQ rax,5,br_equal` - Will continue execution at `br_equal` point if `rax` is equal to 5 |
| BGT         | Branch if greater | 0110 | BGT x,y,br_name | `BGT rax,5,br_greater` - Will continue execution at `br_greater` point if `rax` is greater than 5 |
| IN          | Input value to register | 0111 | IN x | `IN rax` - Input the value to be stored in the `rax` register |
| OUT         | Output the value | 1000 | OUT x | `OUT rax` - Output the value stored in the `rax` memory |
| JSR         | Jump to subroutine | 1101 | JSR sub_name | `JSR sub_print` - Jump to the `sub_print` subroutine |
| RTS         | Return from subroutine | 1110 | RTS | `RTS` - Return to the address after `JSR` call |
| STOP        | Stops the application from running | 1111 | STOP x,y,z | `STOP` - Stops the program<br>`STOP rax,rbx` - Stops the program and outputs the `rax` and `rbx` register |