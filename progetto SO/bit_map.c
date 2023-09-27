#include <assert.h>
#include "bit_map.h"

// returns the number of bytes to store bits booleans
int BitMap_getBytes(int bits){
  assert(bits >= 0);
  return (bits + 7) / 8;
}

// initializes a bitmap on an external array
void BitMap_init(BitMap* bit_map, int num_bits, uint8_t* buffer){
  assert(num_bits > 0);
  bit_map->buffer = buffer;
  bit_map->num_bits = num_bits;
  bit_map->buffer_size = BitMap_getBytes(num_bits);
}

// sets a the bit bit_num in the bitmap
// status= 0 or 1
void BitMap_setBit(BitMap* bit_map, int bit_num, int status){
  // get byte
  int byte_num = bit_num >> 3;
  assert(byte_num < bit_map->buffer_size);
  int bit_in_byte = bit_num%8;
  if (status) {
    bit_map->buffer[byte_num] |= (1 << bit_in_byte);
  } else {
    bit_map->buffer[byte_num] &= ~(1 << bit_in_byte);
  }
}

// inspects the status of the bit bit_num
int BitMap_bit(const BitMap* bit_map, int bit_num){
  int byte_num = bit_num >> 3; 
  assert(byte_num < bit_map->buffer_size);
  int bit_in_byte = bit_num%8;
  return (bit_map->buffer[byte_num] & (1<<bit_in_byte)) != 0;
}