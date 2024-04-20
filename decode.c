#include <stdio.h>
#include "decode.h"
#include "types.h"
#include <string.h>
#include "common.h"

/* Function Definiation */

/* Get File names and validate
 * Input: Stego Image file name and Output file name
 * Output: Stores the above files names
 * Return Value: e-success or e_failure, on file errors
 */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if ( strstr(argv[2],".bmp") != NULL)
    {
	decInfo -> stego_image_fname = argv[2];
    }
    else
    {
	return e_failure;
    }

    if (argv[3] != NULL && strstr(argv[3],".txt") != NULL)
    {
	decInfo -> decoded_fname = argv[3];
    }
    else
    {
	decInfo -> decoded_fname = "decoded.txt";
	printf("INFO: Output File not mentioned. Creating %s as default\n",decInfo -> decoded_fname);
    }

    return e_success;

}

/* Decoding stego image to another file
 * Input: File info of stego image and output file
 * Output: Decoded message copied in output file
 * Return: e_success or e_failure, on file error
 */

Status do_decoding(DecodeInfo *decInfo)
{
    printf("INFO: ## Decoding Procedure Started ##\n");

    //open the files
    if((open_decode_files(decInfo)) == e_success)
    {
	printf("INFO: Done. Opened all required files\n");

	printf("INFO: Decoding Magic String Signature\n");

	//decode magic string
	if ((decode_magic_string(MAGIC_STRING, decInfo)) == e_success)
	{
	    printf("INFO: Done\n");

	    //decode secret file extension size
	    if ((decode_secret_file_extn_size(decInfo)) == e_success)
	    {

		//decode secret file extension
		if((decode_secret_file_extn(decInfo -> image_data_size, decInfo)) == e_success)
		{
		    printf("INFO: Decoding File Size\n");

		    //decode secret file size
		    if((decode_secret_file_size(decInfo)) == e_success)
		    {
			printf("INFO: Done\n");

			printf("INFO: Decoding File Data\n");

			//decode secret file data
			if ((decode_secret_file_data(decInfo)) == e_success)
			{
			    printf("INFO: Done\n");
			    return e_success;
			}
			else
			{
			    printf("ERROR: Failed to decode file data\n");
			    return e_failure;
			}

		    }
		    else
		    {
			printf("ERROR: Failed to decode file size\n");
			return e_failure;
		    }
		}
		else
		{
		    printf("ERROR: Failed to decode secret file extension\n");
		    return e_failure;

		}
	    }
	    else
	    {
		printf("ERROR: Failed to decode secret file extension size\n");
		return e_failure;

	    }
	}
	else
	{
	    printf("ERROR: Failed to decode Magic String\n");
	    return e_failure;
	}
    }
    else
    {
	printf("ERROR: Failed to open required files\n");
	return e_failure;
    }
}

/* Get File pointers for i/p and o/p files
 * Inputs: Stego Image file, Output file
 * Ouput: File pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

Status open_decode_files(DecodeInfo *decInfo)
{
    printf("INFO: Opening required files\n");
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    if (decInfo->fptr_stego_image == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);

	return e_failure;
    }

    printf("INFO: Opened %s\n",decInfo -> decoded_fname);

    decInfo->fptr_decoded_file = fopen(decInfo -> decoded_fname, "w");
    if (decInfo->fptr_decoded_file == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n",decInfo -> decoded_fname);

	return e_failure;
    }

    return e_success;

}

/* Decodes the magic string from stego image
 * Input: Magic string and File info of input and output
 * Output: Proceed decoing if magic string if found or else stop decoding
 * Return: e_success or e_failure
 */

Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo)
{
    fseek(decInfo -> fptr_stego_image, 54, SEEK_SET);

    for (int i=0;magic_string[i] != '\0';i++)
    {
	fread(decInfo -> image_data,sizeof(char),8,decInfo -> fptr_stego_image);

	if ((decode_byte_from_lsb(decInfo -> decode_data,decInfo -> image_data)) == e_success)
	{
	    if (decInfo -> decode_data[0] == magic_string[i])
	    {
		continue;
	    }
	    else
	    {
		fprintf(stderr,"Error: Failed to match the decoded data to magic string\n");
		return e_failure;
	    }
	}
	else
	{
	    fprintf(stderr,"Error: Failed to match the decoded data to magic string\n");
	    return e_failure;
	}

    }
    return e_success;
}

/* Get lsb bit from stego image byte & append this lsb bits to make one byte 
 * Input: Image_data array and decode_data character
 * Output: Decode the image_data and stores the 1 byte data in decode_data
 * Return: e_success or e_failure
 */

Status decode_byte_from_lsb(char *decode_data, char *image_data)
{
    decode_data[0]=0;
    for (int i=0;i<=7;i++)
    {
	decode_data[0] = decode_data[0] | ((image_data[i] & 0x01) << i);
    }
    return e_success;

}

/* Decode file extenstion size from stego image
 * Input: File info of stego image and output file
 * Output: Decode the extenstion size from stego image and store in image_data_size
 * Return: e_success or e_failure
 */

Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    char str[FILE_EXTN_SIZE];

    fread(str,sizeof(char),32,decInfo -> fptr_stego_image);

    decode_size_from_lsb(str,decInfo);

    return e_success;

}


/* Get lsb bit from stego image byte & append this lsb bits  
 * Input: Image_data array and file info
 * Output: Decode the stego image data and stores the 4 byte data in image data size
 * Return: e_success or e_failure
 */

Status decode_size_from_lsb(char *buffer, DecodeInfo *decInfo)
{
    decInfo -> image_data_size=0;

    for (int i=0;i<=31;i++)
    {
	decInfo -> image_data_size = (decInfo -> image_data_size) | ((buffer[i] & 0x01) << i);
    }
}

/* Decode file extenstion from stego image
 * Input: Extenstion Size and File info of stego image
 * Output: Decodes the file extenstion and store in extn_output_file
 * Return: e_success or e_failure
 */

Status decode_secret_file_extn(uint extn_size, DecodeInfo *decInfo)
{

    for (int i=0;i<extn_size;i++)
    {
	fread(decInfo -> image_data,sizeof(char),8,decInfo -> fptr_stego_image);

	if ((decode_byte_from_lsb(decInfo -> decode_data,decInfo -> image_data)) == e_success)
	{
	    decInfo -> extn_decoded_file[i] = decInfo -> decode_data[0];
	}
	else
	{
	    return e_failure;
	}
    }
     //printf("extn : %s\n",decInfo -> extn_decoded_file);
    return e_success;
}

/* Decode file size from stego image
 * Input: FILE info of stego image and output file
 * Output: Decodes the file image and store in image_data_size
 * Return: e_success or e_failure
 */

Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char str[FILE_SIZE];
    fread(str,sizeof(char),32,decInfo -> fptr_stego_image);
    decode_size_from_lsb(str,decInfo);
    return e_success;
}

/* Decode file data from stego image
 * Input: FILE info of stego image and output decode file
 * Output: Write decode data in the output file
 * Return: e_success or e_failure
 */

Status decode_secret_file_data(DecodeInfo *decInfo)
{
    for (int i=0;i<decInfo -> image_data_size;i++)
    {
	fread(decInfo -> image_data,sizeof(char),8,decInfo -> fptr_stego_image);

	if ((decode_byte_from_lsb(decInfo -> decode_data,decInfo -> image_data)) == e_success)
	{
	    fwrite(decInfo -> decode_data,sizeof(char),1,decInfo -> fptr_decoded_file);
	}
	else
	{
	    return e_failure;
	}
    }
    return e_success;
}
