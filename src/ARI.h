/**
 * Classes For Arithmetic Compression
*/ 

#ifndef ARITHCODER_H
#define ARITHCODER_H

#include "IOStream.h"
#include "SimpleBitIOStream.h"
#include "Exception.h"
#include "Model.h"


class Model; /* Forward Declaration.*/

class ARI_encoder {
    Model* model;
    InputStream* in;
    SimpleBitOutputStream* out;
    unsigned long low;                              // Lower bound of the encoding Interval
    unsigned long high;                             // Upper bound of the encoding Interval
    unsigned long bits_count;                       // underflow bits counter
    unsigned long total_bits;
    void check_ratio();
    
    public:
        ARI_encoder();
        ~ARI_encoder();
        // Encode a symbol according to its statistics
        void encode_symbol(unsigned long sym_low, unsigned long sym_high, unsigned long context_totals);
        // Encode the Whole Stream
        void encode(InputStream* _in, SimpleBitOutputStream* _out);
};

class ARI_decoder {
    Model* model;
    SimpleBitInputStream* in;
    OutputStream *out;
    unsigned long low;    /* lower bound of the encoding interval */
    unsigned long high;   /* upper half of the encoding interval */
    unsigned long buffer; /* the first 31 bits of the encoded number */

    public:
        ARI_decoder();
        ~ARI_decoder();
        unsigned long get_count(unsigned long context_totals);
        void decode_symbol(unsigned long sym_low, unsigned long sym_high, unsigned long context);
        void decode(SimpleBitInputStream* _in, OutputStream* _out);
};

#endif