#include "TerrainLattice.h"

TerrainLattice::TerrainLattice(int Lheight,int Lwidth){
	height=Lheight;
	width=Lwidth;
	lattice=new TerrainPoint*[height+1];
	vector = new TerrainVector*[height+1];
	for(int i=0;i<height+1;i++){
		lattice[i]=new TerrainPoint[width+1];
		vector[i]=new TerrainVector[width+1];
	}
	bytesperrow=((width * 3 + 3) / 4) * 4 - (width * 3 % 4);
}

TerrainLattice::TerrainLattice(Image *image){
	height=image->height;
	width=image->width;
	lattice=new TerrainPoint*[height+1];
	vector = new TerrainVector*[height+1];
	for(int i=0;i<height+1;i++){
		lattice[i]=new TerrainPoint[width+1];
		vector[i]=new TerrainVector[width+1];
	}
	/*for(int i=0;i<height*2;height++){
		vector[i]=new TerrainVector[width*2];
	}*/
	bytesperrow=((width * 3 + 3) / 4) * 4 - (width * 3 % 4);
	setDepth(image);
	initVector();
	initSmooth();
}

TerrainLattice::~TerrainLattice(void)
{
}

void TerrainLattice::setDepth(Image *image){
	
	for(int i=0;i<height+1;i++){
		for(int j=0;j<width+1;j++){
			lattice[i][j].x=(-height*0.8f+j*(1.6*width/(width-1)))/2;
			lattice[i][j].z=( width*0.8f-i*(1.6*width/(width-1)))/2;
			lattice[i][j].y=(float)(unsigned char)(image->pixels[(i*width+j)*3])/7;
			if(i==height){
				lattice[i][j].y=lattice[i-1][j].y-0.5f;
			}
			if(j==width){
				lattice[i][j].y=lattice[i][j-1].y-0.5f;
			}
		}
	}
}

void TerrainLattice::initVector(){
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
			int firstI,firstJ,secondI,secondJ;
			if(i==height-1&&j==width-1){
				firstI=i;
				firstJ=j-1;
				secondI=i-1;
				secondJ=j;
			}else{
				if(i==height-1){
					firstI=i-1;
					firstJ=j+1;
					secondI=i;
					secondJ=j+1;
				}else{
					if(j==width-1){
						firstI=i+1;
						firstJ=j;
						secondI=i+1;
						secondJ=j-1;
					}else{
						firstI=i;
						firstJ=j+1;
						secondI=i+1;
						secondJ=j;
					}
				}
			}
			double X1,Y1,Z1,X2,Y2,Z2;
			X1=lattice[i][j].x-lattice[firstI][firstJ].x;
			Y1=lattice[i][j].y-lattice[firstI][firstJ].y;
			Z1=lattice[i][j].z-lattice[firstI][firstJ].z;

			X2=lattice[i][j].x-lattice[secondI][secondJ].x;
			Y2=lattice[i][j].y-lattice[secondI][secondJ].y;
			Z2=lattice[i][j].z-lattice[secondI][secondJ].z;

			TerrainVector *vector1=new TerrainVector(X1,Y1,Z1);
			TerrainVector *vector2=new TerrainVector(X2,Y2,Z2);
			TerrainVector *normalVector=vector1->cross(vector2)->getNormalize();
			lattice[i][j].setVector(normalVector);
		}
	}
}

void TerrainLattice::initSmooth(){
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
			//left-bottom 
			if(i==0&&j==0){
				lattice[i][j].vector->x=(lattice[i][j].vector->x+0.5*lattice[i+1][j].vector->x+0.5*lattice[i][j+1].vector->x)/3;
				lattice[i][j].vector->y=(lattice[i][j].vector->y+0.5*lattice[i+1][j].vector->y+0.5*lattice[i][j+1].vector->y)/3;
				lattice[i][j].vector->z=(lattice[i][j].vector->z+0.5*lattice[i+1][j].vector->z+0.5*lattice[i][j+1].vector->z)/3;
				continue;
			}
			//right-bottom
			if(i==0&&j==width-1){
				lattice[i][j].vector->x=(lattice[i][j].vector->x+0.5*lattice[i][j-1].vector->x+0.5*lattice[i+1][j-1].vector->x+0.5*lattice[i+1][j].vector->x)/4;
				lattice[i][j].vector->y=(lattice[i][j].vector->y+0.5*lattice[i][j-1].vector->y+0.5*lattice[i+1][j-1].vector->y+0.5*lattice[i+1][j].vector->y)/4;
				lattice[i][j].vector->z=(lattice[i][j].vector->z+0.5*lattice[i][j-1].vector->z+0.5*lattice[i+1][j-1].vector->z+0.5*lattice[i+1][j].vector->z)/4;
				continue;
			}
			//left-top
			if(i==height-1&&j==0){
				lattice[i][j].vector->x=(lattice[i][j].vector->x+0.5*lattice[i][j+1].vector->x+0.5*lattice[i-1][j+1].vector->x+0.5*lattice[i-1][j].vector->x)/4;
				lattice[i][j].vector->y=(lattice[i][j].vector->y+0.5*lattice[i][j+1].vector->y+0.5*lattice[i-1][j+1].vector->y+0.5*lattice[i-1][j].vector->y)/4;
				lattice[i][j].vector->z=(lattice[i][j].vector->z+0.5*lattice[i][j+1].vector->z+0.5*lattice[i-1][j+1].vector->z+0.5*lattice[i-1][j].vector->z)/4;
				continue;
			}
			//right-top
			if(i==height-1&&j==width-1){
				lattice[i][j].vector->x=(lattice[i][j].vector->x+0.5*lattice[i-1][j].vector->x+0.5*lattice[i][j-1].vector->x)/3;
				lattice[i][j].vector->y=(lattice[i][j].vector->y+0.5*lattice[i-1][j].vector->y+0.5*lattice[i][j-1].vector->y)/3;
				lattice[i][j].vector->z=(lattice[i][j].vector->z+0.5*lattice[i-1][j].vector->z+0.5*lattice[i][j-1].vector->z)/3;
				continue;
			}
			//bottom
			if(i==0){
				lattice[i][j].vector->x=(lattice[i][j].vector->x+0.5*lattice[i][j-1].vector->x+0.5*lattice[i+1][j-1].vector->x+0.5*lattice[i+1][j].vector->x+0.5*lattice[i][j+1].vector->x)/5;
				lattice[i][j].vector->y=(lattice[i][j].vector->y+0.5*lattice[i][j-1].vector->y+0.5*lattice[i+1][j-1].vector->y+0.5*lattice[i+1][j].vector->y+0.5*lattice[i][j+1].vector->y)/5;
				lattice[i][j].vector->z=(lattice[i][j].vector->z+0.5*lattice[i][j-1].vector->z+0.5*lattice[i+1][j-1].vector->z+0.5*lattice[i+1][j].vector->z+0.5*lattice[i][j+1].vector->z)/5;
				continue;
			}
			//top
			if (i==height-1)
			{
				lattice[i][j].vector->x=(lattice[i][j].vector->x+0.5*lattice[i][j-1].vector->x+0.5*lattice[i-1][j].vector->x+0.5*lattice[i-1][j+1].vector->x+0.5*lattice[i][j+1].vector->x)/5;
				lattice[i][j].vector->y=(lattice[i][j].vector->y+0.5*lattice[i][j-1].vector->y+0.5*lattice[i-1][j].vector->y+0.5*lattice[i-1][j+1].vector->y+0.5*lattice[i][j+1].vector->y)/5;
				lattice[i][j].vector->z=(lattice[i][j].vector->z+0.5*lattice[i][j-1].vector->z+0.5*lattice[i-1][j].vector->z+0.5*lattice[i-1][j+1].vector->z+0.5*lattice[i][j+1].vector->z)/5;
				continue;
			}
			//left
			if(j==0){
				lattice[i][j].vector->x=(lattice[i][j].vector->x+0.5*lattice[i][j+1].vector->x+0.5*lattice[i+1][j].vector->x+0.5*lattice[i-1][j+1].vector->x+0.5*lattice[i-1][j].vector->x)/5;
				lattice[i][j].vector->y=(lattice[i][j].vector->y+0.5*lattice[i][j+1].vector->y+0.5*lattice[i+1][j].vector->y+0.5*lattice[i-1][j+1].vector->y+0.5*lattice[i-1][j].vector->y)/5;
				lattice[i][j].vector->z=(lattice[i][j].vector->z+0.5*lattice[i][j+1].vector->z+0.5*lattice[i+1][j].vector->z+0.5*lattice[i-1][j+1].vector->z+0.5*lattice[i-1][j].vector->z)/5;
				continue;
			}
			//right
			if(j==width-1){
				lattice[i][j].vector->x=(lattice[i][j].vector->x+0.5*lattice[i][j-1].vector->x+0.5*lattice[i+1][j-1].vector->x+0.5*lattice[i+1][j].vector->x+0.5*lattice[i-1][j].vector->x)/5;
				lattice[i][j].vector->y=(lattice[i][j].vector->y+0.5*lattice[i][j-1].vector->y+0.5*lattice[i+1][j-1].vector->y+0.5*lattice[i+1][j].vector->y+0.5*lattice[i-1][j].vector->y)/5;
				lattice[i][j].vector->z=(lattice[i][j].vector->z+0.5*lattice[i][j-1].vector->z+0.5*lattice[i+1][j-1].vector->z+0.5*lattice[i+1][j].vector->z+0.5*lattice[i-1][j].vector->z)/5;
				continue;
			}
			//other points
			lattice[i][j].vector->x=(lattice[i][j].vector->x+0.5*lattice[i][j-1].vector->x+0.5*lattice[i-1][j].vector->x+0.5*lattice[i+1][j].vector->x+0.5*lattice[i][j+1].vector->x+0.5*lattice[i-1][j-1].vector->x+0.5*lattice[i-1][j+1].vector->x+0.5*lattice[i+1][j].vector->x+0.5*lattice[i+1][j+1].vector->x)/9;
			lattice[i][j].vector->y=(lattice[i][j].vector->y+0.5*lattice[i][j-1].vector->y+0.5*lattice[i-1][j].vector->y+0.5*lattice[i+1][j].vector->y+0.5*lattice[i][j+1].vector->y+0.5*lattice[i-1][j-1].vector->y+0.5*lattice[i-1][j+1].vector->y+0.5*lattice[i+1][j].vector->y+0.5*lattice[i+1][j+1].vector->x)/9;
			lattice[i][j].vector->z=(lattice[i][j].vector->z+0.5*lattice[i][j-1].vector->z+0.5*lattice[i-1][j].vector->z+0.5*lattice[i+1][j].vector->z+0.5*lattice[i][j+1].vector->z+0.5*lattice[i-1][j-1].vector->z+0.5*lattice[i-1][j+1].vector->z+0.5*lattice[i+1][j].vector->z+0.5*lattice[i+1][j+1].vector->x)/9;
		}
	}
}
void TerrainLattice::reset(){
	initVector();
	initSmooth();
}