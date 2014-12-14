.code32
#define DEFFUNCTION(name) .global name; .type name @function; name
#define FUNCTION(name) DEFFUNCTION(name): pushl %ebp; movl %esp, %ebp
#define ENDFUNCTION(name) popl %ebp; ret
#define ALIAS(name,for) .global name; .set name, for
