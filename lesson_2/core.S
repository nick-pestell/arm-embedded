// These instructions define attributes of our chip and
// the assembly language we'll use:
.syntax unified
.cpu cortex-m7
.fpu softvfp
.thumb

// Global memory locations.
.global reset_handler

/*
 * The Reset handler. Called on reset.
 */
.type reset_handler, %function
reset_handler:
  // Set the stack pointer to the end of the stack.
  // The '_estack' value is defined in our linker script.
  LDR  r0, =_estack // loads the _estack address (RAM) (end of stack) into register r0
  MOV  sp, r0 // assigns the stack pointer (SP) the value of end of stack
  // sp is a special register which refers to the stack pointer

  // Copy data from flash to RAM data init section.
  // R2 will store our progress along the sidata section.
  MOVS r0, #0 //loads the literal 0 into register r0

  // Load the start/end addresses of the data section,
  // and the start of the data init section.
  LDR  r1, =_sdata // loads the contents of _sdata (start of data address in RAM) into register r1
  LDR  r2, =_edata // loads the contents of _edata (end of data address in RAM) into register r2
  LDR  r3, =_sidata //loads the contents of _sidata (start of initialized data address (i think in FLASH)) into register r3
  B    copy_sidata_loop // branches to the copy_sidata_loop loop

  copy_sidata:
    // Offset the data init section by our copy progress.
    LDR  r4, [r3, r0] // loads the contents of the address held in r3+r0 into r4 (this starts as _sidata+0, i.e. start of initialised data, in FLASH)
    // Copy the current word into data, and increment.
    STR  r4, [r1, r0] // stores the contents of r4 into the address held in r1+r0 (this starts at _sdata+0, i.e. start of data, in RAM)
    ADDS r0, r0, #4 // r0 = r0 + 4. increments r0 by the literal 4, used for working through address of memory in FLASH and RAM

  copy_sidata_loop:
    // Unless we've copied the whole data section, copy the
    // next word from sidata->data.
    ADDS r4, r0, r1 //  r4 = r0 + r1. r1 was originally holding the address of the s_data. This adds 4 bytes so essentially moves r4 through RAM towards _edata
    CMP  r4, r2 // compares r4 to _edata
    BCC  copy_sidata // branches up to the copy_sidata function conditional on r4 being different to _edata

  // Once we are done copying the data section into RAM,
  // move on to filling the BSS section with 0s.
  MOVS r0, #0 // loads literal 0 in register r0
  LDR  r1, =_sbss // loads the contents of _sbss (the address of start of uninitialised data (RAM)) into register r1
  LDR  r2, =_ebss // loads the contents of _ebss (the address of end of uninitialised data (RAM)) into register r2
  B    reset_bss_loop // branches to the reset_bss_loop loop

  // Fill the BSS segment with '0's.
  reset_bss:
    // Store a 0 and increment by a word.
    STR  r0, [r1] // stores the contents of r0 (literal 0) into register r1 (which starts as _sbss (the start of uninitialised data (RAM)
    ADDS r1, r1, #4 // adds 4 bytes to the address in r1, i.e. moving through uninitialised data in memory

  reset_bss_loop:
    // We'll use R1 to count progress here; if we aren't
    // done, reset the next word and increment.
    CMP  r1, r2 // compares r1 to r2 
    BCC  reset_bss // branches to reset_bss function conditional on r1 being different to r2 , i.e. checks if we've reached the address loaded into r2 (end of uninitialised data)

  // Branch to the 'main' method.
  B    main
.size reset_handler, .-reset_handler
