#include <iostream>
#include<fstream>
#include<cmath>
using namespace std;

int main()

{

    // Generating chirp signal (t = 0 to 20)

 int index;

 double x[20];

	for(index=0;index<20;index=index+1)

	{

		x[index]=cos((4*index)+10);

     

	}

 int n=20;

 int i;

 int k;

 double s;

 double y[20];

  s = sqrt ( 2.0 );

/*

  Initialize.

*/

	for ( i = 0; i < n; i++ )

	{

		y[i] = 0.0;

	}

/*

  Determine K, the largest power of 2 such that K <= N.

*/

  k = 1;

	while ( k * 2 <= n )

	{

		k = k * 2;

	}

  

	while ( 1 < k )

	{

		k = k / 2;

		for ( i = 0; i < k; i++ )

		{

			y[i]   = ( x[2*i] + x[2*i+1] ) / s;

			y[i+k] = ( x[2*i] - x[2*i+1] ) / s;

		}

		for ( i = 0; i < k * 2; i++ )

		{

			x[i] = y[i];

		}

	}
	
  
	ofstream fout;
	fout.open("test.txt");	//opening an output stream for file test.txt
	
	/*checking whether file could be opened or not. If file does not exist or don't have write permissions, file
  stream could not be opened.*/
  if(fout.is_open())
	{
    //file opened successfully so we are here
    cout << "File Opened successfully!!!. Writing data from array to file" << endl;

		for(i = 0; x[i] != '\0'; i++)
		{
      fout<<x[i]; //writing ith character of array in the file
		}
    cout << "Array data successfully saved into the file test.txt" << endl;
    fout.close();
	}
	else //file could not be opened
	{
		cout << "File could not be opened." << endl;
	}
	return 0;
}
