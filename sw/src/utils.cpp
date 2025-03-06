#include<utils.h>
#include<memory_allocator.h>
void check_bytes(const unsigned char img0[],const unsigned char img1[], size_t lenght)
{
	size_t right_bytes=0;
	unsigned char tollerance=BYTES_TOLLERANCE;
	unsigned char lower, upper;
	float percent;
	for (size_t i = 0; i < lenght; ++i) {
		//printf("%d - %d\n",img0[i], img1[i]);
		lower= ( img0[i] > tollerance ) ? img0[i]-tollerance : 0;
		upper= ( (255-img0[i]) <= tollerance)  ? 255 : img0[i]+tollerance;
		if ((img1[i] >= lower) && (img1[i] <= upper)) {
			right_bytes++;
		}
	}
	percent = (float) right_bytes / lenght *100;
	printf("|-> with tolerance [%u] (%.2f%%)\t%lu / %lu\n",tollerance, percent, right_bytes,lenght);
}

void confront_images(addr_t img0,const unsigned char img1[], size_t lenght)
{
	for(size_t i = 0; i < lenght; ++i){
		printf("-> [%d]\t[%d]\t",*(unsigned char *) (img0.virt_addr + i), img1[i]);
		if (*((unsigned char *) (img0.virt_addr + i)) == img1[i]){
			printf("uguali\n");
		}else{
			printf("ERRORE\n");
		}
	}

}

void max_byte_count(const unsigned char img0[], const unsigned char ref_img[], size_t lenght)
{
	size_t img=0, ref=0;
	float percent;

	for(size_t i=0; i<lenght;i++){
		if( img0[i] == 255)
			img++;
		if(ref_img[i] == 255)
			ref++;
	}
	percent = (float) img / ref *100;
	printf("|-> 'FF' bytes are [%lu kernel - %lu reference] (%.2f%%) on %lu \n", img, ref, percent, lenght);
}

void zero_fill(unsigned char img[], size_t lenght)
{
	for(size_t i=0; i!= lenght; i++){
		img[i] = 0;
	}
}