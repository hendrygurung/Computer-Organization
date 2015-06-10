// Hendry Gurung

#include <stdio.h>

void make_pixel (
int r, // red intensity
int g, // green intensity
int b ) // blue intensity
{
 	printf("%c%c%c", r, g, b);
}

void make_ppm_header (int width, int height){
 	printf("P6\n");
 	printf("%d %d %d\n", width, height, 255);
}

void make_france_flag ( int width ){
	int height; 
	int i, j; 
	
	height = width * 2 	  / 3;
	make_ppm_header(width, height);

	for(i = 0; i < height; i++){
		for(j = 0; j < width; j++){
			
			if( j < width/3 ){	//First third
				make_pixel(0,85,164);
			}
			else if( j < 2 * (width/3) ){//Second third
				make_pixel(255,255,255);
			}
			else{				//last third
				make_pixel(250,60,50);
			}
		}
	}
}

void make_germany_flag ( int width ){
	int height; 
	int i, j; 
	
	height = width * 3/5 ;
	make_ppm_header(width, height);

	for(i = 0; i < height; i++){
		for(j = 0; j < width; j++){
			
			if( i < height/3 ){	//First third
				make_pixel(0,0,0);
			}
			else if( i < 2 * (height/3) ){//Second third
				make_pixel(208,0,0);
			}
			else{				//last third
				make_pixel(255,206,0);
			}
		}
	}
}

void make_lithuania_flag ( int width ){
	int height; 
	int i, j; 
	
	height = width * 3/5 ;
	make_ppm_header(width, height);

	for(i = 0; i < height; i++){
		for(j = 0; j < width; j++){
			
			if( i < height/3 ){	//First third
				make_pixel(253,185,19);
			}
			else if( i < 2 * (height/3) ){//Second third
				make_pixel(0,106,68);
			}
			else{				//last third
				make_pixel(193,39,45);
			}
		}
	}
}


void make_ppm_image (int country_code, int width){
	if (country_code==0){
        make_france_flag(width);
    }
    else if (country_code==1){
    	make_germany_flag(width);
    }
    else if (country_code==2){
        make_lithuania_flag(width);
    }
    else{
    	printf("Not a valid country flag code\n");
    }
}




int main(){
int width;
int country_code;

/* Read image dimensions and pixel color */
scanf("%d%d", &country_code, &width);

fprintf(stderr, "Making country %d width %d\n", 
country_code, width);

/* Write the image data */
make_ppm_image(country_code, width);

return 0;
}
