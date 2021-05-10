// Adaptive order-N Context Model

#ifndef MODEL_H
#define MODEL_H

#include "Exception.h"
#include "ARI.h"

struct Settings {
    static const int ORDER = 5;
    static const int SYMBOLS = 256;
};

/**
 * A Circular buffer for storing last input bytes that are used to generate 
 * new Context tree  after all memory allocated for the contexts is consumed.
*/ 
class CircularBuffer {
public:
    static const long SIZE = 8192;
private: 
    unsigned char data[SIZE];
    long insert_pos;
public:
    CircularBuffer();
    unsigned char at(long i); // Returns char at pos i
    void add(unsigned char c);
    long get_insert_pos() {
        return insert_pos;
    }
    void reset() {
        insert_pos = 0;
    }
};

/** A Context Tree data Struture for Context Handling */
class ContextTree {
    public:
        static const long NODES = 2000000;
    private:
        struct Node {
            short int symbol;
            unsigned long count;                                                    // Symbol Count
            long down;                                                              // Pointer to next level
            long right;                                                             // Pointer to the Next Node on the same level
            long vine;                                                              // Vine Pointer
            void set(int, unsigned long, long, long, long);
        } nodes[NODES];
        
        short int exclusion_list[Settings::SYMBOLS];
        int exclusion_pos;
        int max_index;
        long base;                                                                  // Base Pointer
        long insert_node;                                                           // Root Node where new Child is to be inserted
        int depth;                                                                  // Tree Depth (Varies from 0 to Model::ORDER + 1)
        long vine;
        long last;
        long get_start();
        bool search_exclusion(short int c, int max_index);
        /* Search all childs of node n for symbol s and, if successful, store
        * the statistics to the pointers and return the number of this node.
        * Otherwise store the statistics of the escape symbol and return -1. */
       long search_by_symbol(long n, int s, unsigned long* sym_low, unsigned long* sym_high, unsigned long* context_totals);
       long search_by_count(long n, unsigned long count, unsigned long* sym_low, unsigned long* sym_high);
    
    public:
        ContextTree();
        unsigned long get_context_totals();
        bool is_full() {
            return insert_node == NODES;
        }
        // Initialize the Tree and if buffer is not NULL, build the Tree
        // According to the last CircularBuffer::SIZE input Characters
        void build(CircularBuffer* buffer);
        /* Try to add a symbol to the tree and if the pointers are not NULL, 
        * store the symbol statistics. Return false if the symbol is not
        * found in the current context (the pointers will then contain the
        * statistics for the escape symbol instead). Otherwise return true. */
       bool add_by_symbol(int s, unsigned long* sym_low, unsigned long* sym_high, unsigned long* context_totals);
       int add_by_count(unsigned long count, unsigned long* sym_low, unsigned long* sym_high, unsigned long context_totals);
       void flush();
};

/** Forward Declaration */
class ARI_encoder;
class ARI_decoder;

class Model {
    public:
        static const int ID_EOF = Settings::SYMBOLS;
        static const int ID_FLUSH = Settings::SYMBOLS + 1;
        static const int ID_ESCAPE = Settings::SYMBOLS + 2;
    private: 
        ContextTree* tree;
        CircularBuffer* buffer;
    public: 
        Model();
        ~Model();
        // Add a Symbol to the Model and return its statstics; when compressing
        void add(ARI_encoder* enc, int s, unsigned long* sym_low, unsigned long* sym_high, unsigned long* context_totals);
        // Add a Symbol to the Model and return its statistics; when decompressing
        int add(ARI_decoder* dec);
        void flush() {
            tree -> flush();
        }
};

#endif