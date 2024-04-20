/* NAME : SUFIYAN ATTAR
DATE : 20/04/2024
*/

#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>
#include "common.h"

/* Function Definiation */

/* Read and validate command line argument
 * Input: Command line Argument count and Arguments wtih File name info
 * Output: File names stored in encoded Info
 * Return: e_success or e_failure
 */

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    //check the extension of files like .bmp .txt

    if ( strstr(argv[2],".bmp") != NULL)
    {
	encInfo -> src_image_fname = argv[2];
    }
    else
    {
	return e_failure;
    }


    if ( argv[3] !=  NULL)
    {
	encInfo -> secret_fname = argv[3];
	strcpy(encInfo -> extn_secret_file,strchr(argv[3],'.'));
    }
    else
    {
	return e_failure;
    }

    if (argv[4] != NULL && strstr(argv[4],".bmp") != NULL)
    {
	encInfo -> stego_image_fname = argv[4];
	printf("INFO: Output File is mentioned named as %s\n",encInfo -> stego_image_fname);
    }
    else
    {
	encInfo -> stego_image_fname = "steged_image.bmp"; 
	printf("INFO: Output File not mentioned. Creating steged_img.bmp as default\n");
    }

    return e_success;
}

/* Encoding the magic string,extension of secret file,size of extension,size of secret file & data of secret file to stego image
 * Input: FILE info of image, secret file and stego image
 * Output: Encodes the data in secret to stego image
 * Return: e_success or e_failure;
 */

Status do_encoding(EncodeInfo *encInfo)
{
    //open the files
    if (open_files(encInfo) == e_success)
    {
	printf("INFO: Done\n");

	printf("INFO: ## Encoding Procedure Started ##\n");

	//check capacity of .bmp file and .txt file
	if (check_capacity(encInfo) == e_success)
	{
	    printf("INFO: Copying Image Header\n");

	    //copy 54 bytes from source bmp file to output bmp file
	    if (copy_bmp_header(encInfo -> fptr_src_image,encInfo -> fptr_stego_image) == e_success)
	    {
		printf("INFO: Done\n");

		printf("INFO: Encoding Magic String Signature\n");

		//encode magic string
		if (encode_magic_string(MAGIC_STRING, encInfo) == e_success)
		{
		    printf("INFO: Done\n");

		    printf("INFO: Encoding %s File Extenstion Size\n",encInfo -> secret_fname);

		    //encode secret file extension size
		    if ((encode_secret_file_extn_size(strlen(encInfo -> extn_secret_file), encInfo)) == e_success)
		    {
			printf("INFO: Done\n");

			printf("INFO: Encoding %s File Extenstion\n",encInfo -> secret_fname);

			//encode secret file extension
			if ((encode_secret_file_extn(encInfo -> extn_secret_file,encInfo)) == e_success)
			{
			    printf("INFO: Done\n");

			    printf("INFO: Encoding %s File Size\n",encInfo -> secret_fname);

			    //encode secret file size
			    if ((encode_secret_file_size(encInfo -> size_secret_file,encInfo)) == e_success)
			    {
				printf("INFO: Done\n");

				printf("INFO: Encoding %s File Data\n",encInfo -> secret_fname);

				//encode secret secret file data
				if ((encode_secret_file_data(encInfo)) == e_success)
				{
				    printf("INFO: Done\n");

				    printf("INFO: Copying Left Over Data\n");

				    //copy leftover data
				    if ((copy_remaining_img_data(encInfo -> fptr_src_image, encInfo -> fptr_stego_image)) == e_success)
				    {
					printf("INFO: Done\n");
					return e_success;

				    }
				    else
				    {
					printf("ERROR: Failed to Copy Left Over Data\n");
					return e_failure;
				    }

				}
				else
				{
				    printf("ERROR: Failed to Encode %s File Data",encInfo -> secret_fname);
				    return e_failure;
				}
			    }
			    else
			    {
				printf("ERROR: Failed to Encode %s File Size\n",encInfo -> secret_fname);
				return e_failure;
			    }
			}
			else
			{
			    printf("ERROR: Failed to Encode %s File Extenstion\n",encInfo -> secret_fname);
			    return e_failure;
			}
		    }
		    else
		    {
			printf("ERROR: Failed to Encode %s File Extenstion Size\n",encInfo -> secret_fname);
			return e_failure;
		    }
		}
		else
		{
		    printf("ERROR: Failed to Encode Magic String Signature\n");
		    return e_failure;
		}
	    }
	    else
	    {
		printf("ERROR: Failed to Copy Image Header\n");
		return e_failure;
	    }
	} 
	else
	{
	    printf("ERROR: Failed to Check Capacity\n");
	    return e_failure;
	}
    }
    else
    {
	printf("ERROR: Failed to Open Files\n");
	return e_failure;
    }
}

/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

Status open_files(EncodeInfo *encInfo)
{
    printf("INFO: Opening required files\n");
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

	return e_failure;
    }

    printf("INFO: Opened %s\n",encInfo->src_image_fname);

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

	return e_failure;
    }

    printf("INFO: Opened %s\n",encInfo->secret_fname);

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

	return e_failure;
    }
    printf("INFO: Opened %s\n", encInfo->stego_image_fname);

    // No failure return e_success
    return e_success;
}

/* To check the capacity of source image to handle secret data
 * Input: File info source image, stego image and secret file
 * Output: Get Source image capacity and store in image_capacity
 * Return: e_success or e_failure
 */

Status check_capacity(EncodeInfo *encInfo)
{

    printf("INFO: Checking for %s size\n",encInfo -> secret_fname);
    encInfo -> size_secret_file = get_file_size(encInfo -> fptr_secret);

    if (encInfo -> size_secret_file != 0)
    {
	printf("INFO: Done. Not Empty\n");
    }
    else
    {
	printf("ERROR: File Is Empty\n");
	return e_failure;
    }

    encInfo -> image_capacity = get_image_size_for_bmp(encInfo -> fptr_src_image);

    printf("INFO: Checking for %s capacity to handle %s\n",encInfo -> src_image_fname,encInfo -> secret_fname);
    if (encInfo -> image_capacity > (2+4+4+4+encInfo -> size_secret_file)*8)
    {
	printf("INFO: Done. Found OK\n");
    }
    else
    {
	printf("ERROR: %s doesn't have the capacity to encode %s\n",encInfo -> src_image_fname,encInfo -> secret_fname);
	return e_failure;
    }

    return e_success;
}

/* Get the file size 
 * Input:  File pointer
 * Return: Size of file in bytes
 */

uint get_file_size(FILE *fptr)
{
    fseek(fptr,0,SEEK_END);
    return ftell(fptr);
}

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */

uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    //printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    //printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* Copy the header bytes data to stego image
 * Input: FILE pointers source and stego image
 * Output: Copies header data of source image to stego image
 * Return: e_success or e_failure
 */

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char str[54];
    rewind(fptr_src_image);
    rewind(fptr_dest_image);
    fread(str, sizeof(char), 54, fptr_src_image);
    fwrite(str, sizeof(char), 54, fptr_dest_image);
    return e_success;
}

/* Encode Magic string in stego image
 * Input: magic string and FILEs info
 * Output: Encode magic character '*' in stego image first 8 bytes from image data
 * Return: e_success or e_failure
 */

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    encode_data_to_image(MAGIC_STRING, strlen(MAGIC_STRING),encInfo -> fptr_src_image, encInfo -> fptr_stego_image);

    return e_success;
}

/* Encode data to image data
 * Input: secret file extension/Secret data, length of extension/secret data size, File pointer of source and stego image files
 * Output: Gets source image data and encodes it with secret data by calling another function
 * Return: e_success or e_failure
 */
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char str[8];
    for (int i=0;i < size;i++)
    {
	fread(str,sizeof(char),8,fptr_src_image);

	encode_byte_to_lsb(data[i],str);

	fwrite(str,sizeof(char),8,fptr_stego_image);
    }

}

/* Encodes 1 byte data to image 8 byte data
 * Input: Secret 1 byte data and Image 8 byte data
 * Output: Encode data to image_buffer
 * Return: e_success or e_failure
 */

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for (int i=7;i>=0;i--)
    {
	image_buffer[i] = (( data & (1<<i)) >> i) | (image_buffer[i] & 0xFE);
    }
}

/* Encode the secret file extenstion size to stego image
 * Input: FILEs info
 * Output: Encode file extenstion size to stego image 
 * Return: e_success or e_failure
 */

Status encode_secret_file_extn_size(uint size, EncodeInfo *encInfo)
{
    char str[32];

    fread(str,sizeof(char),32,encInfo -> fptr_src_image);

    encode_size_to_lsb(size, str);

    fwrite(str,sizeof(char),32,encInfo -> fptr_stego_image);

    return e_success;
}

/* Encodes 4 byte data to image 32 byte data
 * Input: Secret 4 byte data and Image 32 byte data
 * Output: Encode data to image_buffer
 * Return: e_success or e_failure
 */
Status encode_size_to_lsb(uint size, char *image_buffer)
{
    for (int i=31;i>=0;i--)
    {
	image_buffer[i] = ((size & (1<<i)) >> i) | (image_buffer[i] & 0xFE);
    }
}

/* Encode file extenstion to stego image
 * Input: Files info
 * Output: Copy source image data to stego image with encoded file extenstion
 * Return: e_success or e_failure
 */

Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo)
{
    encode_data_to_image(file_extn, strlen(file_extn),encInfo -> fptr_src_image, encInfo -> fptr_stego_image);

    return e_success;
}

/* Encode secret file size data to stego image
 * Input: Files info
 * Output: Encode secret file size data to stego image
 * Return: e_success or e_failure
 */

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    char str[32];

    fread(str,sizeof(char),32,encInfo -> fptr_src_image);

    encode_size_to_lsb(file_size, str);

    fwrite(str,sizeof(char),32,encInfo -> fptr_stego_image);

    return e_success;
}

/* Encode secret file data to stego image file
 * Input: Files info
 * Output: Encodes secret data to stego image
 * Return: e_success or e_failure
 */

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char str[encInfo -> size_secret_file];

    fseek(encInfo -> fptr_secret,0,SEEK_SET);

    fread(str,1,encInfo -> size_secret_file,encInfo -> fptr_secret);

    encode_data_to_image(str,encInfo -> size_secret_file,encInfo -> fptr_src_image,encInfo -> fptr_stego_image);

    return e_success;
}

/* Copy remaining source image data to stego image
 * Input: File pointer of source and stego image
 * Output: Remaining image data copied from source image
 * Return: e_success or e_failure
 */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;

    while (fread(&ch,sizeof(char),1,fptr_src) > 0)
    {
	fwrite(&ch,sizeof(char),1,fptr_dest);
    }
    return e_success;
}
