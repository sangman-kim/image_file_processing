//#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef unsigned char   INT8U;
typedef signed short    INT16S;
typedef unsigned short  INT16U;
typedef signed int		INT32S;
typedef unsigned int    INT32U;
typedef float	FP32;


#define IMG_WIDHT  1280
#define IMG_HEIGHT 944

#pragma warning(disable:4996)


INT8U sa_input_image_u8[IMG_WIDHT * IMG_HEIGHT * 3];
INT8U* pa_input_image_u8 = (INT8U*)&sa_input_image_u8[0];


int main()
{
	
	INT32U idx_x_32u = 0;
	
	//Raw FIle read
	printf("Read raw image file...\n\n\n");
	FILE* raw_image_file = fopen("1280_944.rgb ", "r");

	fread(pa_input_image_u8, sizeof(sa_input_image_u8[0]), IMG_WIDHT * IMG_HEIGHT * 3, raw_image_file);

	fclose(raw_image_file);


	//Make Header file
	FILE* raw_header_file = fopen("1280_944_rgb_header.h", "w");
	
	printf("Make header image file...\n\n\n");
	fprintf(raw_header_file, "unsigned char input_image_u8[] = { ");
	
	/*
	for (idx_x_32u = 0; idx_x_32u < (IMG_WIDHT * IMG_HEIGHT * 3 - 1); idx_x_32u++)
	{
		fprintf(raw_header_file, "%03d, ", (INT32U)sa_input_image_u8[idx_x_32u]);

		if (idx_x_32u % 10000 == 0)
		{
			printf("IDX count is %d...\n", idx_x_32u);
		}
	}
	
	fprintf(raw_header_file, "%03d };", (INT32U)sa_input_image_u8[IMG_WIDHT * IMG_HEIGHT * 3 - 1]);
	*/
	for (idx_x_32u = 0; idx_x_32u < (IMG_WIDHT * IMG_HEIGHT - 1); idx_x_32u++)
	{
		fprintf(raw_header_file, "%3d, ", (INT32U)*pa_input_image_u8++);
		fprintf(raw_header_file, "%3d, ", (INT32U)*pa_input_image_u8++);
		fprintf(raw_header_file, "%3d, ", (INT32U)*pa_input_image_u8++);

		if (idx_x_32u % 10000 == 0)
		{
			printf("IDX count is %d...\n", idx_x_32u);
		}
	}
	
	fprintf(raw_header_file, "%3d, ", (INT32U)*pa_input_image_u8++);
	fprintf(raw_header_file, "%3d, ", (INT32U)*pa_input_image_u8++);
	fprintf(raw_header_file, "%3d };", (INT32U)*pa_input_image_u8++);
	
	fclose(raw_header_file);

	
	//Make raw file from generated header file
	printf("Make Rawfile from generated header file...\n\n\n");
	
	FILE* raw_file_from_header = fopen("re_gen_1280_944.rgb", "wb");
	
	fwrite(sa_input_image_u8, sizeof(sa_input_image_u8[0]), IMG_WIDHT * IMG_HEIGHT * 3, raw_header_file);

	fclose(raw_header_file);
	



	return 0;

}
