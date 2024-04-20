#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4
#define FILE_EXTN_SIZE 32
#define FILE_SIZE 32

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

typedef struct _DecodeInfo
{
    /* Encoded stego image info */
    char *stego_image_fname;
    FILE *fptr_stego_image;
    uint image_data_size;
    char image_data[MAX_IMAGE_BUF_SIZE];

    /* Decoded output file info */
    char *decoded_fname;
    FILE *fptr_decoded_file;
    char decode_data[MAX_SECRET_BUF_SIZE];
    char extn_decoded_file[MAX_FILE_SUFFIX];

} DecodeInfo;


/* decoding function prototype */

/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the encoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_decode_files(DecodeInfo *decInfo);

/* Store Magic String */
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo);

/* Encode secret file extenstion size*/
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/* Encode secret file extenstion */
Status decode_secret_file_extn(uint extn_size, DecodeInfo *decInfo);

/* Encode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Encode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Encode a byte into LSB of image data array */
Status decode_byte_from_lsb(char *decoded_data, char *image_data);

/* Encode a size into LSB of image data array */
Status decode_size_from_lsb(char *buffer, DecodeInfo *decInfo);


#endif
