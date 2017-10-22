# include <stdio.h>
# include <stdlib.h>


void ReadText (int[] M, int h, int w)
{
	for (int i=0; i<h ; ++i)
	{
			int j=0;
			while (j < w && M[i*w + j] != 1);
			{
				j++;
			}
			if (j<w)
			{
				int y1 = i;
				while (++i < h)
				{
					j = 0;
					while (j < w && M[i*w + j == 0])
					{
						j++;
					}
					if (j == w)
						break;
				}
				int y2 = i;
				Readline(M,h,w, y1, y2, spacing);
			}
	}
}

void ReadLine(int[] M, int h, int w, int y1, int y2, int spacing)
{
	for (int i = 0; i<w ; ++i)
	{
		int j =  y1;
		while (j<y2 && M[j*w + i ] !=1)
			j++;
		if (j<y2)
		{
			int currentspace = 0;
			int x1 = i;
			while (++i<w)
			{
				j = y1;
				while (j<y2 && M[j*w +i]!= 1)
					j++;
				if (j <y2)
					currentspace = 0;
				else
					++currentspace;	
				if (currentspace == spacing)
					break;
			}
			int x2 = i-spacing+1;
			ReadWord(M, h, w, y1, y2, x1, x2)
		}
	}
}

void  ReadWord(int[] M,int h, int w, int y1, int y2, int x1, int x2)
{
	int coords[4];
	for(int i =x1; i<x2; ++i)
	{
		int j = y1;
		while (j<y2 && M[j*w + i ] !=1)
			j++;
		if (j<y2)
		{
			int begin = i;
			while (++i<x2)
			{
				j = y1;
				while(j<y2 && M[j*w + i != 1]
					j++;
				if (j ==y2)
					break;
			}
			int end = i;
			ShrinkCharacter(M, h, x, y1,y2,begin,end, coords);
		}
	}	
}

void ShrinkCharacter(int[] M, int y1, int y2, int x1, int x2, int[] coords)
{
	int i = y1;
	int j;
	while (i<y2)
	{
		j = x1;
		while (j <x2 && M[i*w + j] != 1)
			++j;
		if (j<x2)
		{
			y1 = i;
			break;
		}
		i++;
		
	}
	i = y2;
	while (i>y1)
	{
		j = x1;
		while (j<x2 && M[i*w + j] != 1)
			++j;
		if (j<x2)
		{
			y2 = i;
			break;
		}
		--i;
	}
	j = x1
	while (j<x2)
	{
		i = y1;
		while (i<=y2 && M[i*w + j] !=1)
			++i;
		if (i<=y2)
		{
			x1 = j;
			break;
		}
		++j;
	}
	j = x2;
	while (j>x1)
	{
		i = y1;
		while(i<=y2 && M[i*w + j] != 1)
			++i
		if (i<=y2)
		{
			x2 = j;
			break;
		}
		--j;
	}
	coords[0] = y1;
	coords[1] = y2;
	coords[2] = x1;
	coords[3] = x2; 
}
