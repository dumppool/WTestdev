#include "TerrainSquare.h"

char g_cQTFanCode[] = { 10, 8, 8, 12, 8, 0, 12, 14, 8, 12, 0, 14, 12, 14, 14, 0 };
char g_cQTFanStart[]= { 3,  3, 0,  3, 1, 0,  0,  3, 2,  2, 0,  2,  1,  1,  0, 0 };

TerrainSquare::TerrainSquare(TerrainLattice* lattice,TerrainCamera* camera)
{
	this->lattice=lattice;
	this->camera=camera;
	init();
}

TerrainSquare::~TerrainSquare(void)
{
}

void TerrainSquare::init(){
	memset(matrix,true,sizeof(matrix));
	memset(Rmatrix,0.0f,sizeof(Rmatrix));
	memset(Hmatrix,0.0f,sizeof(Hmatrix));
	MinResolution=1.0f;
	DesiredResolution=50.0f;
	roughness();
	m_iTrisPerFrame=0.0f;
}

void TerrainSquare::shutdown(){
	free(matrix);
	free(Rmatrix);
	free(Hmatrix);
}

void TerrainSquare::render(){
	refineNode(lattice->width/2,lattice->height/2,lattice->width);
	renderNode(lattice->width/2,lattice->height/2,lattice->width);
}

void TerrainSquare::RenderVertex(int x,int z){
	glTexCoord2f((float)z/(float)(lattice->height),(float)x/(float)(lattice->width));
	glNormal3f(lattice->lattice[x][z].vector->x,lattice->lattice[x][z].vector->y,lattice->lattice[x][z].vector->z);
	glVertex3f(lattice->lattice[x][z].x,lattice->lattice[x][z].y,lattice->lattice[x][z].z);
}

void TerrainSquare::renderNode(int x,int z,int EdgeSize){
	float fChildOffset;
	float fEdgeOffset;
	int iStart, iFanCode;
	int iChildEdgeSize;
	int iChildOffset;
	int iEdgeOffset;
	int iAdjOffset;
	int iFanLength;
	int iFanPosition;
	int bSubdivide;
	int iX, iZ;

	iX= ( int )x;
	iZ= ( int )z;

	iEdgeOffset= ( EdgeSize)/2;
	fEdgeOffset= ( EdgeSize)/2.0f;

	iAdjOffset= EdgeSize;

	bSubdivide= GetMatrixData( iX, iZ );

	if( bSubdivide )
	{
		if( EdgeSize<=2 )
		{
			glBegin( GL_TRIANGLE_FAN );

			RenderVertex(x, z);

			RenderVertex( x-fEdgeOffset, z-fEdgeOffset);

			if( ( ( iZ-iAdjOffset )<0 ) || GetMatrixData( iX, iZ-iAdjOffset )!=0 )
			{
				RenderVertex( x, z-fEdgeOffset);
				m_iTrisPerFrame++;
			}

			RenderVertex( x+fEdgeOffset, z-fEdgeOffset);
			m_iTrisPerFrame++;

			if( ( ( iX+iAdjOffset )>=lattice->width+1) || GetMatrixData( iX+iAdjOffset, iZ )!=0 )
			{
				RenderVertex( x+fEdgeOffset, z);
				m_iTrisPerFrame++;
			}

			RenderVertex( x+fEdgeOffset, z+fEdgeOffset);
			m_iTrisPerFrame++;

			if( ( ( iZ+iAdjOffset )>=lattice->width+1 ) || GetMatrixData( iX, iZ+iAdjOffset )!=0 )
			{
				RenderVertex( x, z+fEdgeOffset);
				m_iTrisPerFrame++;
			}

			RenderVertex( x-fEdgeOffset, z+fEdgeOffset);
			m_iTrisPerFrame++;

			if( ( ( iX-iAdjOffset )<0 ) || GetMatrixData( iX-iAdjOffset, iZ )!=0 )
			{
				RenderVertex( x-fEdgeOffset, z);
				m_iTrisPerFrame++;
			}

			RenderVertex( x-fEdgeOffset, z-fEdgeOffset);
			m_iTrisPerFrame++;
			glEnd( );
			return;

		}

		else
		{
			iChildOffset= ( EdgeSize)/4;
			fChildOffset= ( float )iChildOffset;

			iChildEdgeSize= ( EdgeSize)/2;

			iFanCode = ( GetMatrixData( iX+iChildOffset, iZ+iChildOffset )!=0 )*8;
			iFanCode|= ( GetMatrixData( iX-iChildOffset, iZ+iChildOffset )!=0 )*4;
			iFanCode|= ( GetMatrixData( iX-iChildOffset, iZ-iChildOffset )!=0 )*2;
			iFanCode|= ( GetMatrixData( iX+iChildOffset, iZ-iChildOffset )!=0 );

			if( iFanCode==QT_NO_FAN )
			{
				renderNode( x-fChildOffset, z-fChildOffset, iChildEdgeSize );
				renderNode( x+fChildOffset, z-fChildOffset, iChildEdgeSize );
				renderNode( x-fChildOffset, z+fChildOffset, iChildEdgeSize );
				renderNode( x+fChildOffset, z+fChildOffset, iChildEdgeSize );
				return;
			}

			if( iFanCode==QT_LL_UR )
			{
				glBegin( GL_TRIANGLE_FAN );
					RenderVertex( x, z);
					RenderVertex( x+fEdgeOffset, z);
					RenderVertex( x+fEdgeOffset, z+fEdgeOffset);
					m_iTrisPerFrame++;
					RenderVertex( x, z+fEdgeOffset);
					m_iTrisPerFrame++;
				glEnd( );

	
				glBegin( GL_TRIANGLE_FAN );
					RenderVertex( x, z);
					RenderVertex( x-fEdgeOffset, z);
					RenderVertex( x-fEdgeOffset, z-fEdgeOffset);
					m_iTrisPerFrame++;
					RenderVertex( x, z-fEdgeOffset);
					m_iTrisPerFrame++;
				glEnd( );

				renderNode( x-fChildOffset, z+fChildOffset, iChildEdgeSize );
				renderNode( x+fChildOffset, z-fChildOffset, iChildEdgeSize );
				return;

			}

			if( iFanCode==QT_LR_UL )
			{
				glBegin( GL_TRIANGLE_FAN );
					RenderVertex( x, z);
					RenderVertex( x, z+fEdgeOffset);
					RenderVertex( x-fEdgeOffset, z+fEdgeOffset);
					m_iTrisPerFrame++;
					RenderVertex( x-fEdgeOffset, z);
					m_iTrisPerFrame++;
				glEnd( );

				glBegin( GL_TRIANGLE_FAN );
					RenderVertex( x, z);
					RenderVertex( x, z-fEdgeOffset);
					RenderVertex( x+fEdgeOffset, z-fEdgeOffset);
					m_iTrisPerFrame++;
					RenderVertex( x+fEdgeOffset, z);
					m_iTrisPerFrame++;
				glEnd( );

				renderNode( x+fChildOffset, z+fChildOffset, iChildEdgeSize );
				renderNode( x-fChildOffset, z-fChildOffset, iChildEdgeSize );
				return;
			}

			if( iFanCode==QT_COMPLETE_FAN )
			{
				glBegin( GL_TRIANGLE_FAN );
					RenderVertex( x, z);
					RenderVertex( x-fEdgeOffset, z-fEdgeOffset);
					if( ( ( iZ-iAdjOffset )<0 ) || GetMatrixData( iX, iZ-iAdjOffset )!=0 )
					{
						RenderVertex( x, z-fEdgeOffset);
						m_iTrisPerFrame++;
					}
					RenderVertex( x+fEdgeOffset, z-fEdgeOffset);
					m_iTrisPerFrame++;
					if( ( ( iX+iAdjOffset )>=lattice->width+1 ) || GetMatrixData( iX+iAdjOffset, iZ )!=0 )
					{
						RenderVertex( x+fEdgeOffset, z);
						m_iTrisPerFrame++;
					}
					RenderVertex( x+fEdgeOffset, z+fEdgeOffset);
					m_iTrisPerFrame++;
					if( ( ( iZ+iAdjOffset )>=lattice->width+1 ) || GetMatrixData( iX, iZ+iAdjOffset )!=0 )
					{
						RenderVertex( x, z+fEdgeOffset);
						m_iTrisPerFrame++;
					}
					RenderVertex( x-fEdgeOffset, z+fEdgeOffset);
					m_iTrisPerFrame++;
					if( ( ( iX-iAdjOffset )<0 ) || GetMatrixData( iX-iAdjOffset, iZ )!=0 )
					{
						RenderVertex( x-fEdgeOffset, z);
						m_iTrisPerFrame++;
					}
					RenderVertex( x-fEdgeOffset, z-fEdgeOffset);
					m_iTrisPerFrame++;
				glEnd( );
				return;
			}

			iStart= g_cQTFanStart[iFanCode];
			iFanLength= 0;
			while( !( ( ( long )g_cQTFanCode[iFanCode] )&( 1<<iFanLength ) ) && iFanLength<8 )
				iFanLength++;

			glBegin( GL_TRIANGLE_FAN );
				RenderVertex( x, z);
				for( iFanPosition=iFanLength; iFanPosition>0; iFanPosition-- )
				{
					switch( iStart )
					{
					case QT_LR_NODE:
						if( ( ( iZ-iAdjOffset )<0 ) || GetMatrixData( iX, iZ-iAdjOffset )!=0 || iFanPosition==iFanLength )
						{
							RenderVertex( x, z-fEdgeOffset);
							m_iTrisPerFrame++;
						}
						RenderVertex( x+fEdgeOffset, z-fEdgeOffset);
						m_iTrisPerFrame++;
						if( iFanPosition==1 )
						{
							RenderVertex( x+fEdgeOffset, z);
							m_iTrisPerFrame++;
						}
						break;
					case QT_LL_NODE:
						if( ( ( x-iAdjOffset )<0 ) || GetMatrixData( iX-iAdjOffset, iZ )!=0 || iFanPosition==iFanLength )
						{
							RenderVertex( x-fEdgeOffset, z);
							m_iTrisPerFrame++;
						}
						RenderVertex( x-fEdgeOffset, z-fEdgeOffset);
						m_iTrisPerFrame++;
						if( iFanPosition==1 )
						{
							RenderVertex( x, z-fEdgeOffset);
							m_iTrisPerFrame++;
						}
						break;
					case QT_UL_NODE:
						if( ( ( iZ+iAdjOffset )>=lattice->width+1 ) || GetMatrixData( iX, iZ+iAdjOffset )!=0 || iFanPosition==iFanLength )
						{
							RenderVertex( x, z+fEdgeOffset);
							m_iTrisPerFrame++;
						}
						RenderVertex( x-fEdgeOffset, z+fEdgeOffset);
						if( iFanPosition==1 )
						{
							RenderVertex( x-fEdgeOffset, z);
							m_iTrisPerFrame++;
						}
						break;
					case QT_UR_NODE:
						if( ( ( iX+iAdjOffset )>=lattice->width+1 ) || GetMatrixData( iX+iAdjOffset, iZ )!=0 || iFanPosition==iFanLength )
						{
							RenderVertex( x+fEdgeOffset, z);
							m_iTrisPerFrame++;
						}
						RenderVertex( x+fEdgeOffset, z+fEdgeOffset);
						m_iTrisPerFrame++;
						if( iFanPosition==1 )
						{
							RenderVertex( x, z+fEdgeOffset);
							m_iTrisPerFrame++;
						}
						break;
					}
					iStart--;
					iStart&= 3;
				}
			glEnd( );

			for( iFanPosition=( 4-iFanLength ); iFanPosition>0; iFanPosition-- )
			{
				switch( iStart )
				{
				case QT_LR_NODE:
					renderNode( x+fChildOffset, z-fChildOffset, iChildEdgeSize );
					break;
				case QT_LL_NODE:
					renderNode( x-fChildOffset, z-fChildOffset, iChildEdgeSize );
					break;
				case QT_UL_NODE:
					renderNode( x-fChildOffset, z+fChildOffset, iChildEdgeSize );
					break;
				case QT_UR_NODE:
					renderNode( x+fChildOffset, z+fChildOffset, iChildEdgeSize );
					break;
				}
				iStart--;
				iStart&= 3;
			}
			return;
		}
	}
}

void TerrainSquare::update(){
	refineNode(lattice->width/2,lattice->width/2,lattice->width);
}

void TerrainSquare::refineNode(int xOff,int zOff,int EdgeSize){
	bool subDivided;
	float viewDistance=abs(camera->point->x-lattice->lattice[xOff][zOff].x)+abs(camera->point->y-lattice->lattice[xOff][zOff].y)+abs(camera->point->z-lattice->lattice[xOff][zOff].z);
	float f=viewDistance/(EdgeSize*MinResolution*max(DesiredResolution*Rmatrix[xOff][zOff],1.0f));
	if(f<1.0f){
		subDivided=true;
	}else{
		subDivided=false;
	}
	if(subDivided&&EdgeSize>2){
		matrix[xOff][zOff]=true;
		float ChildOffset=(float)(EdgeSize>>2);
		int ChildSize=EdgeSize>>1;

		refineNode(xOff+ChildOffset,zOff+ChildOffset,ChildSize);
		refineNode(xOff+ChildOffset,zOff-ChildOffset,ChildSize);
		refineNode(xOff-ChildOffset,zOff-ChildOffset,ChildSize);
		refineNode(xOff-ChildOffset,zOff+ChildOffset,ChildSize);
	}else{
		matrix[xOff][zOff]=false;
	}
}
void TerrainSquare::roughness()
{	
	int EdgeOffset;
	int ChildOffset;
	float d2 ,ld2, dh, lh;
	float K;

	for (int size = 2; size <lattice->width; size*=2)
	{
		EdgeOffset = size >> 1;
		ChildOffset = size >> 2;

		for (int z = EdgeOffset; z < lattice->width; z += size)
		{
			for (int x = EdgeOffset; x < lattice->width; x += size)
			{
				//RIGHT
				ld2 = abs( ( GetHeight(x-EdgeOffset, z+EdgeOffset) + GetHeight(x+EdgeOffset, z+EdgeOffset) )*0.5f - GetHeight(x, z+EdgeOffset));
				// TOP
				dh = abs( (GetHeight(x+EdgeOffset, z+EdgeOffset) + GetHeight(x+EdgeOffset, z-EdgeOffset))*0.5f - GetHeight(x+EdgeOffset, z));
				ld2 = max(ld2, dh);
				// LEFT
				dh = abs( (GetHeight(x-EdgeOffset, z-EdgeOffset) + GetHeight(x+EdgeOffset, z-EdgeOffset))*0.5f - GetHeight(x, z-EdgeOffset));
				ld2 = max(ld2, dh);
				// BOTTOM
				dh = abs( (GetHeight(x-EdgeOffset, z-EdgeOffset) + GetHeight(x-EdgeOffset, z+EdgeOffset))*0.5f - GetHeight(x-EdgeOffset, z));
				ld2 = max(ld2, dh);
				// RIGHT DIAGONAL
				dh = abs( (GetHeight(x-EdgeOffset, z-EdgeOffset) + GetHeight(x+EdgeOffset, z+EdgeOffset))*0.5f - GetHeight(x, z));
				ld2 = max(ld2, dh);
				// LEFT DIAGONAL
				dh = abs( (GetHeight(x+EdgeOffset, z-EdgeOffset) + GetHeight(x-EdgeOffset, z+EdgeOffset))*0.5f - GetHeight(x, z));
				ld2 = max(ld2, dh);

				if (size == 2)
				{
					d2 = ld2;
					// TOP RIGHT
					lh = GetHeight(x+EdgeOffset, z+EdgeOffset);
					// RIGHT
					lh = max(lh, GetHeight(x+EdgeOffset, z));
					// BOTTOM RIGHT
					lh = max(lh, GetHeight(x+EdgeOffset, z-EdgeOffset));
					// BOTTOM
					lh = max(lh, GetHeight(x, z-EdgeOffset));
					// BOTTOM LEFT
					lh = max(lh, GetHeight(x-EdgeOffset, z-EdgeOffset));
					// LEFT
					lh = max(lh, GetHeight(x-EdgeOffset, z));
					// TOP LEFT
					lh = max(lh, GetHeight(x-EdgeOffset, z+EdgeOffset));
					// TOP 
					lh = max(lh, GetHeight(x+EdgeOffset, z+EdgeOffset));

					Hmatrix[x][z]=lh;
				}
				else
				{
					K = (float)MinResolution/(2.0f*(MinResolution-2));

					d2 = max(K*GetMatrixData(x,z), ld2);

					d2 = max(K*GetMatrixData(x-EdgeOffset,z), ld2);
					d2 = max(K*GetMatrixData(x+EdgeOffset,z), ld2);
					d2 = max(K*GetMatrixData(x,z+EdgeOffset), ld2);
					d2 = max(K*GetMatrixData(x,z-EdgeOffset), ld2);

					lh = GetHeight(x+ChildOffset, z+ChildOffset);
					lh = max(lh, GetHeight(x+EdgeOffset, z+EdgeOffset));
					lh = max(lh, GetHeight(x+EdgeOffset, z-EdgeOffset));
					lh = max(lh, GetHeight(x-EdgeOffset, z-EdgeOffset));
					lh = max(lh, GetHeight(x-EdgeOffset, z+EdgeOffset));
					Hmatrix[x][z]=lh;
				}

				Rmatrix[x][z]=d2;
				Rmatrix[x-EdgeOffset][z-EdgeOffset]=d2;
				Rmatrix[x+EdgeOffset][z-EdgeOffset]=d2;
				Rmatrix[x+EdgeOffset][z+EdgeOffset]=d2;
				Rmatrix[x-EdgeOffset][z+EdgeOffset]=d2;
			}
		}		
	}
}
float TerrainSquare::GetHeight(int x,int z){
	return lattice->lattice[x][z].y;
}
bool TerrainSquare::GetMatrixData(int x, int y){
	return matrix[x][y];
}