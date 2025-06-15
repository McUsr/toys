/* An eclectic collection of macros as I get hold of them. */
/* The two below are from intrusive linked lists. */
#define OFFSETOF(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#define CONTAINER_OF(ptr, type, member) (type *) ((char *) (ptr) - OFFSETOF(type, member))
#define FIELD_SIZEOF(t, f) (sizeof(((t*)0)->f))
// finds the size of a member in a struct
// https://stackoverflow.com/questions/3553296/sizeof-single-struct-member-in-c/3553321#3553321

// man 3 regex but changed the name to the "standard" NRELMS.
#define NRELMS(arr) (sizeof((arr)) / sizeof((arr)[0]))
// https://stackoverflow.com/questions/3599160/how-can-i-suppress-unused-parameter-warnings-in-c
#define UNUSED(x) (void) (x)
// https://gcc.gnu.org/onlinedocs/gcc-4.8.5/cpp/Stringification.html
#define xstr(s) str(s)
#define str(s) #s

#define WARN_IF(EXP) \
     do { if (EXP) \
             fprintf (stderr, "Warning: " #EXP "\n"); } \
     while (0)

// Effective "C" p.p 179, maybe rework for different scenarios
#define PASTE(x,y) x ## _ ## y

// Exit status for failing malloc etc.
#define TYPE2_ERROR 2

// macros from :
// https://www.codeproject.com/Articles/567335/Essential-Macros-for-C-Programming
/// Return min of two numbers. Commonly used but never defined as part of standard headers
#ifndef MIN
#define MIN( n1, n2 )   ((n1) > (n2) ? (n2) : (n1))
#endif

/// Return max of two numbers. Commonly used but never defined as part of standard headers
#ifndef MAX
#define MAX( n1, n2 )   ((n1) > (n2) ? (n1) : (n2))
#endif

// Aligns the supplied size to the specified PowerOfTwo
#define ALIGN_SIZE( sizeToAlign, PowerOfTwo )       \
        (((sizeToAlign) + (PowerOfTwo) - 1) & ~((PowerOfTwo) - 1))

// Checks whether the supplied size is aligned to the specified PowerOfTwo
#define IS_SIZE_ALIGNED( sizeToTest, PowerOfTwo )  \
        (((sizeToTest) & ((PowerOfTwo) - 1)) == 0)

// Macros related to "struct"

/// Obtain the offset of a field in a struct
#define GET_FIELD_OFFSET( StructName, FieldName ) \
        ((short)(long)(&((StructName *)NULL)->FieldName))

/// Obtain the struct element at the specified offset given the struct ptr
#define GET_FIELD_PTR( pStruct, nOffset ) \
        ((void *)(((char *)pStruct) + (nOffset)))

/**
Allocates a structure given the structure name and returns a pointer to
that allocated structure.

The main benefit is there is no need to cast the returned pointer, to the
structure type.

@param StructName the name of the structure
@return pointer to allocated structure if successful, else NULL.
@see INIT_STRUCT
*/
#define ALLOC_STRUCT( StructName ) ((StructName *)malloc( sizeof( StructName )))

/**
Initializes the given structure to zeroes using memset().

@param pStruct the pointer to structure that has to be initialized
@see ALLOC_STRUCT
*/
#define INIT_STRUCT( pStruct ) (memset( pStruct, '\0', sizeof( *(pStruct) )))

/// Determine whether the given signed or unsigned integer is odd.
#define IS_ODD( num )   ((num) & 1)

/// Determine whether the given signed or unsigned integer is even.
#define IS_EVEN( num )  (!IS_ODD( (num) ))

/**
Determine whether the given number is between the other two numbers
(both inclusive).
*/
#define IS_BETWEEN( numToTest, numLow, numHigh ) \
        ((unsigned char)((numToTest) >= (numLow) && (numToTest) <= (numHigh)))


/**
Determines whether the memory architecture of current processor is LittleEndian.

Optimizing compiler should be able to reduce this macro to a boolean constant
TRUE or FALSE.

@return 1 if LittleEndian, else 0
*/
#define IS_LITTLE_ENDIAN()  (((*(short *)"21") & 0xFF) == '2')

/**
Determines whether the memory architecture of current processor is BigEndian.

Optimizing compiler should be able to reduce this macro to a boolean constant
TRUE or FALSE.

@return 1 if BigEndian, else 0
*/
#define IS_BIG_ENDIAN()     (((*(short *)"21") & 0xFF) == '1')

/**
Change this macro to change the default endian format. In this example,
the default endian format is Little Endian.

Optimizing compiler should be able to reduce this macro to a boolean constant
TRUE or FALSE.

@return 1 if the curren endian format is the default format, else 0
*/
#define IS_DEFAULT_ENDIAN() IS_LITTLE_ENDIAN()

/**
Reverses the bytes of the supplied byte array.
*/
#define REVERSE_BYTE_ARRAY( ByteArray, Size )                               \
        if (!IS_DEFAULT_ENDIAN())                                           \
        {                                                                   \
            int     _i, _j;                                                 \
            char    _cTmp;                                                  \
            for (_i = 0, _j = (Size) - 1; _i < _j; _i++, _j--)              \
            {                                                               \
                _cTmp = ((char *)(ByteArray))[ _i ];                        \
                ((char *)(ByteArray))[ _i ] = ((char *)(ByteArray))[ _j ];  \
                ((char *)(ByteArray))[ _j ] = _cTmp;                        \
            }                                                               \
        }

/**
If the current machine is not default endian, re-arranges the bytes of the
given number. Does nothing if the current machine is default endian.

Use this for number variable whose size is greater than 32 bits.

For 16 and 32 bit numbers CONVERT_NUM16() and CONVERT_NUM32() are recommended.
*/
#define CONVERT_NUM( n )    REVERSE_BYTE_ARRAY( (&(n)), sizeof( n ))

/**
If the current machine is not default endian, re-arranges the bytes of the
given 16-bit number. Does nothing if the current machine is default endian.
*/
#define CONVERT_NUM16( n )  ((void)(IS_DEFAULT_ENDIAN() ? (n)       \
        : ((n) = ((((n) & 0x00FF) << 8) | (((n) & 0xFF00) >> 8)))))

/**
If the current machine is not default endian, re-arranges the bytes of the
given 32-bit number. Does nothing if the current machine is default endian.
*/
#define CONVERT_NUM32( n )  ((void)(IS_DEFAULT_ENDIAN() ? (n)               \
        : ((n) = ((((n) & 0x000000FF) << 24) | (((n) & 0x0000FF00) << 8)    \
        | (((n) & 0xFF0000) >> 8) | (((n) & 0xFF000000) >> 24)))))

/**
If the current machine is not default endian, re-arranges the bytes of the
given 32-bit floating point number. Does nothing if the current machine is
default endian.
*/
#define CONVERT_FLOAT( f )  CONVERT_NUM32( (*(long *)&(f) ))

/**
If the current machine is not default endian, re-arranges the bytes of the
given 64-bit floating point number. Does nothing if the current machine is
default endian.
*/
#define CONVERT_DOUBLE( d ) CONVERT_NUM( d )

/**
If the current machine is not default endian, re-arranges the bytes of the
given 64-bit point number. Does nothing if the current machine is
default endian.
*/
#define CONVERT_NUM64( n )  CONVERT_NUM( n )
/* macro for finding field size of a struct member, original declared
 * in <linux/stddef.h>
 * https://stackoverflow.com/questions/3553296/sizeof-single-struct-member-in-c/3553321#3553321
 */
#define fldsiz(name, field) \
    (sizeof(((struct name *)0)->field))

/**
 * Macro for defining vectors of some type: */
 #define vec(T) struct { T *data; ptrdiff_t len, cap; }

/**
 * Example of usage: */
 /* vec(long) l = {0};  // using directly
 * typedef vec(int) int_array;  // declaring a type
 */
/**
 * Macros for comparing floats based on some epsilon
 * that defines the number of digits precision used for
 * comparision.
 */

/* double epsilon = 1e-10;  // 10 digits precision */
#define COMPARE_FLOATS(a, b, epsilon) (fabs(a - b) <= epsilon * fabs(a))
#define DIFFERENT_FLOATS(a, b, epsilon) (fabs(a - b) >= epsilon * fabs(a))

/**
Macros to make pesky long gcc compiler attributes more edible.
*/
#ifdef __GNUC__
#define CONSTRUCTOR __attribute__((constructor))
#define NORETURN __attribute__ ((__noreturn__))
#define WEAK __attribute__ ((__weak__))
#else
#define CONSTRUCTOR
#define NORETURN
#define WEAK
#endif
