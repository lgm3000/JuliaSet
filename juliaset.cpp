#include<cstdlib>
#include<cstdio>
#include<cstring>
#include<string>
#include<iostream>
#include<complex>
#define NUM 500

static double julia_c1[1000];
static double julia_c2[1000];

int juliavalue(const int& w,const int& h,int x,int y,const int& iteration);
void juliaset(const int& w,const int& h,int** r,int** g,int** b,const int& iteration);
void generate_bmp(FILE *f,const int& w,const int& h,int** const red,int** const green,int** const blue);
std::string rFilename(int a);

int main(){
	FILE *f;
	int w,h;
	int **red,**green,**blue;

	scanf("%d %d",&w,&h);

	red = new int *[w];
	for(int i=0; i<w; i++) red[i] = new int [h];

	green = new int *[w];
	for(int i=0; i<w; i++) green[i] = new int [h];

	blue = new int *[w];
	for(int i=0; i<w; i++) blue[i] = new int [h];
	
	for(int i=0; i<NUM; i++){
		julia_c1[i] = 0-0.002*i;
		julia_c2[i] = 0.002*i;
	}


	for(int i=0; i<NUM; i++){
		f = fopen(rFilename(i).c_str(),"wb");
	
		juliaset(w,h,red,green,blue,i);
		generate_bmp(f,w,h,red,green,blue);

		fclose(f);
	}
}

std::string rFilename(int a){
	std::stringstream ss;
	ss << "./juliasets/bmp" << a << ".bmp";
	return ss.str();

}

void juliaset(const int& w,const int& h,int** r,int** g,int** b,const int& iteration){
	for(int i=0; i<w ; i++)
		for(int j=0; j<h ; j++){
			int val = juliavalue(w,h,i,j,iteration);
			r[i][j] = val * 255;
			g[i][j] = 0;
			b[i][j] = 0;
		}
}

int juliavalue(const int& w,const int& h,int x,int y,const int& iteration){
	const double scale = 1.5;
	double fx = scale * (double)(w/2 - x)/(w/2);
	double fy = scale * (double)(h/2 - y)/(h/2);

	std::complex<double> c (julia_c1[iteration],julia_c2[iteration]);
	std::complex<double> a (fx,fy);

	for (int i=0; i<80; i++){
		a = a*a + c;
		if(abs(a) > 1000) return 0;
	}
	return 1;
}

void generate_bmp(FILE *f,const int& w,const int& h,int** const red,int** const green,int** const blue){

	unsigned char *img = NULL;
	int filesize = 54 + 3*w*h;  //w is your image width, h is image height, both int
	if( img )
	    free( img );
	img = (unsigned char *)malloc(3*w*h);
	memset(img,0,sizeof(img));	



	for(int i=0; i<w; i++)
	{
	    for(int j=0; j<h; j++)
		{
		    int x,y,r,g,b;
		    x=i; y=(h-1)-j;
 		    r = red[i][j];
		    g = green[i][j];
		    b = blue[i][j];
		    if (r > 255) r=255;
		    if (g > 255) g=255;
		    if (b > 255) b=255;
		    img[(x+y*w)*3+2] = (unsigned char)(r);
		    img[(x+y*w)*3+1] = (unsigned char)(g);
		    img[(x+y*w)*3+0] = (unsigned char)(b);
		}
	}

	unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
	unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
	unsigned char bmppad[3] = {0,0,0};

	bmpfileheader[ 2] = (unsigned char)(filesize    );
	bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
	bmpfileheader[ 4] = (unsigned char)(filesize>>16);
	bmpfileheader[ 5] = (unsigned char)(filesize>>24);

	bmpinfoheader[ 4] = (unsigned char)(       w    );
	bmpinfoheader[ 5] = (unsigned char)(       w>> 8);
	bmpinfoheader[ 6] = (unsigned char)(       w>>16);
	bmpinfoheader[ 7] = (unsigned char)(       w>>24);
	bmpinfoheader[ 8] = (unsigned char)(       h    );
	bmpinfoheader[ 9] = (unsigned char)(       h>> 8);
	bmpinfoheader[10] = (unsigned char)(       h>>16);
	bmpinfoheader[11] = (unsigned char)(       h>>24);


	fwrite(bmpfileheader,1,14,f);
	fwrite(bmpinfoheader,1,40,f);
	for(int i=0; i<h; i++)
	{
	    fwrite(img+(w*(h-i-1)*3),3,w,f);
	    fwrite(bmppad,1,(4-(w*3)%4)%4,f);
	}
}


