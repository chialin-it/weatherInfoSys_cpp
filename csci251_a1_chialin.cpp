//UOW ID: 6097881
//Name: Lee Chia Lin
//CSCI251 Advanced Programming Assignment 1

// g++ csci251_a1_chialin.cpp -o csci251_a1_chialin.exe
// ./csci251_a1_chialin.exe

/*Additional note
The codes are arranged in this order:
header
global variables
declare main functions
struct
struct global array
int main()
definition of struct functions
definition of main functions
*/

#include <string>
#include <iostream>
#include <fstream>
#include <vector> 
#include <sstream>
#include <iomanip> 	//to use setprecision(2) to get 2 decimal place

using namespace std;

//global variables
int xyRange [4];	//to store upp and lower bound of x and y, eg. x is 0-6, y is 0-8, xyRange [0, 6, 0, 8]
int xSize, ySize;
string filesArray [3];	//to store the 3 data filenames from input file

//define functions
void displayMenu();
vector<string> splitStringsByToken(string str, char dl); 
void addSumToCity(string filename, int row, int col, string str);
void readFile(string filename);
bool processConfigFile();
void runAllStructCompute();
void displayGrid(string tableType);
void displayReport();
void deallocateMemory();

struct GridStruct
{
	//variables	
	int x = -1;
	int y = -1; 

	int cityId;
	string cityName = "";

	int cloudValue;
	int cloudIndex;
	char cloudLMH;

	int pressureValue;
	int pressureIndex;
	char pressureLMH;

	//functions
	int computeIndex(int value);
	char computeLMH(int value);
	void computeAll();	//runs all compute functions in this struct
	string toString();
};

struct CityStruct
{
	//variables	
	string name = "";
	GridStruct *cityGrid = nullptr;
	GridStruct *cityGridPerimeter = nullptr;
	int gridSize;
	int cityGrid_index = 0;
	int cityGridPerimeter_index = 0;

	double cloudSum = 0;
	double pressureSum = 0;
	double avgCC;
	double avgP;
	char ACC_LMH;
	char AP_LMH;
	double rainProb;
	string ASCII;
	
	//functions
	void initArrays();
	void addCityGrid(GridStruct g);
	void addGridPerimeter(GridStruct g);
	
	void constructGridPerimeter();
	void addCloudSum(int num);
	void addPressureSum(int num);

	double getTotalGrid();
	double getCloudSum();
	double getPSum();

	void computeAvgCC();
	void computeAvgP();
	char computeLMH(double value);
	void computeRainProb();
	void computeAll();	//runs all compute functions in this struct

	//formatted for displaying city summary report
	string toString();
};

// global array of struct
GridStruct **xyGrid2D = nullptr;
CityStruct *cityStructArray = nullptr;

int main ()
{ 
	cout << "Welcome to Weather Information Processing System!" << endl << endl;
	int usersChoice = 0;

	//for checking input validation
	bool cinValid = false;
	bool dataInput = false;

	do
	{
		displayMenu();
		cin >> usersChoice;
		
		//check if input usersChoice is valid
		while (cinValid == false)
		{
			if(cin.fail())	//if cin >> usersChoice is not int
			{			
				cin.clear();
				cin.ignore();
				cin >> usersChoice;	//wait for a new userChoice input
			}
			//if not in choice 1-8
			else if(usersChoice <= 0 || usersChoice >=9)
			{
				cout << "Please enter only choice 1-8: ";
				cin >> usersChoice;
			}
			//if not config file was read yet, usersChoice not 1 or 8
			else if(usersChoice != 1 && dataInput == false && usersChoice != 8)			
			{
				cout << "Please enter choice 1 and read data first: ";
				cin >> usersChoice;
			}
			//passes all checks
			else
			{
				cinValid = true;
			}
		}

		cinValid = false;	//reset cinValid = false to do the check again for next loop 

		//if there is already data of a config file and usersChoice = 1
		if(usersChoice == 1 && dataInput == true)
		{
			cout << endl << "*Note: A configuration file has already been read." << endl;
			cout << "Please quit and rerun the program if you would like to read in another configuration file." << endl << endl;
			cout << "To continue with the configuration file that was read, return to main menu and select choice 2-7" << endl;			
			
			cout << endl << "Press <enter> to go back to main menu ...";
			cin.ignore();
			cin.ignore();
			cout << endl << endl;
		}
		else		
		{
			switch (usersChoice)
			{
				case 1: cout << endl << "[ Read in and process a configuration file ]" << endl;			
					dataInput = processConfigFile();	
					//processConfigFile returns a bool, sets dataInput to true if all files are successfully read
					break;

				case 2: cout << endl << "[ Display city map ]" << endl;
					displayGrid("cityMap");		
					break;

				case 3: cout << endl << "[ Display cloud coverage map (cloudiness index) ]" << endl;
					displayGrid("cloudIndex");	
					break;

				case 4: cout << endl << "[ Display cloud coverage map (LMH index) ]" << endl;
					displayGrid("cloudLMH");	
					break;

				case 5: cout << endl << "[ Display atmospheric pressure map (pressure index) ]" << endl;
					displayGrid("pressureIndex");	
					break;

				case 6: cout << endl << "[ Display atmospheric pressure map (LMH index) ]" << endl;
					displayGrid("pressureLMH");	
					break;

				case 7: cout << endl << "[ Show weather forecast summary report ]" << endl;
					displayReport ();		
					break;
			
				case 8: cout << endl << "Have a nice day!"  << endl << endl;      
			}

			//make an input for user to Press <enter> for option 2-7
			if(usersChoice >=2 && usersChoice <=7)
			{		
				cout << "Press <enter> to go back to main menu ...";
				cin.ignore();
				cin.ignore();
				cout << endl << endl;
			}
		}
	}while (usersChoice != 8);
	
	cout << endl;
	deallocateMemory();
	
	return (0);
}

//-----------------------------------------------GridStruct functions-----------------------------------
//compute cloudIndex and pressureIndex
int GridStruct::computeIndex(int value)
{
	int index;	
	
	if(value >= 0 && value < 10)
		index = 0;

	else if(value >= 10 && value < 20)
		index = 1;

	else if(value >= 20 && value < 30)
		index = 2;

	else if(value >= 30 && value < 40)
		index = 3;

	else if(value >= 40 && value < 50)
		index = 4;

	else if(value >= 50 && value < 60)
		index = 5;

	else if(value >= 60 && value < 70)
		index = 6;

	else if(value >= 70 && value < 80)
		index = 7;

	else if(value >= 80 && value < 90)
		index = 8;

	else if(value >= 90 && value < 100)
		index = 9;
	
	return index;
}

//compute cloudLMH and pressureLMH
char GridStruct::computeLMH(int value)
{
	char LMH;

	if(value >= 0 && value < 35)
		LMH = 'L';

	else if(value >= 35 && value < 65)
		LMH = 'M';

	else if(value >= 65 && value < 100)
		LMH = 'H';

	return LMH;
}

//runs all computate functions in GridStruct
void GridStruct::computeAll()
{
	cloudIndex = computeIndex(cloudValue);
	cloudLMH = computeLMH(cloudValue);

	pressureIndex = computeIndex(pressureValue);
	pressureLMH = computeLMH(pressureValue);
}

string GridStruct::toString()
{
	ostringstream oss;

	oss << "x: " << x << "\t";
	oss << "y: " << y << endl;
	oss << "city: " << cityId << " \t\t";
	oss << "name: " << cityName << endl;

	oss << "cloudValue: " << cloudValue << "\t\t";
	oss << "cloudIndex: " << cloudIndex << "\t\t";
	oss << "cloudLMH: " << cloudLMH<< endl;

	oss << "pressureValue: " << pressureValue << "\t";
	oss << "pressureIndex: " << pressureIndex << "\t";
	oss << "pressureLMH: " << pressureLMH << endl;

	oss << "------------------------------------------------------------------------" << endl;
	return (oss.str());
}

//----------------------------------------------CityStruct functions----------------------------------------
//create the [] for storing grids and perimeter of city
void CityStruct::initArrays()
{
	cityGrid = new GridStruct [gridSize]();
	cityGridPerimeter = new GridStruct [gridSize]();
}

void CityStruct::addCityGrid(GridStruct g)
{
	cityGrid[cityGrid_index] = g;
	cityGrid_index++;
}

void CityStruct::addGridPerimeter(GridStruct g)
{
	cityGridPerimeter[cityGridPerimeter_index] = g;
	cityGridPerimeter_index++;
}

void CityStruct::addCloudSum(int num)
{
	cloudSum = cloudSum + num;
}

void CityStruct::addPressureSum(int num)
{
	pressureSum = pressureSum + num;
}

//find the grids that are perimeter of this city
void CityStruct::constructGridPerimeter()
{
	int minX, maxX, minY, maxY;
	minX = maxX = cityGrid[0].x;
	minY = maxY = cityGrid[0].y;

	for(int i=0; i<gridSize; i++)
	{
		if(cityGrid[i].x != -1 && cityGrid[i].x < minX)
		{	minX = cityGrid[i].x;	}

		if(cityGrid[i].y != -1 && cityGrid[i].y < minY)
		{	minY = cityGrid[i].y;	}

		if(cityGrid[i].x != -1 && cityGrid[i].x > maxX)
		{	maxX = cityGrid[i].x;	}

		if(cityGrid[i].y != -1 && cityGrid[i].y > maxY)
		{	maxY = cityGrid[i].y;	}
	}
	
	for(int r=0; r<ySize; r++)
	{
		for(int c=0; c<xSize; c++)
		{	
			GridStruct g = xyGrid2D[r][c];
	
			if(g.y == minY-1 || g.y == maxY+1)
			{
				if(g.x >= minX-1 && g.x <= maxX+1)
				{
					addGridPerimeter(g);
				}
			}
			if(g.x == minX-1 || g.x == maxX+1)
			{
				if(g.y > minY-1 && g.y < maxY+1)
				{
					addGridPerimeter(g);
				}
			}
		}
	}
	cout << endl;
}

//calculations
double CityStruct::getTotalGrid()
{	
	return cityGrid_index + cityGridPerimeter_index;	
}

double CityStruct::getCloudSum()
{
	double totalSum = cloudSum;

	for(int i=0; i<cityGridPerimeter_index; i++)
	{
		totalSum = totalSum + cityGridPerimeter[i].cloudValue;
	}
	return totalSum;
}

void CityStruct::computeAvgCC()
{
	avgCC = getCloudSum()/getTotalGrid();
}

double CityStruct::getPSum()
{
	double totalSum = pressureSum;

	for(int i=0; i<cityGridPerimeter_index; i++)
	{
		totalSum = totalSum + cityGridPerimeter[i].pressureValue;
	}
	return totalSum;
}


void CityStruct::computeAvgP()
{
	avgP = getPSum()/getTotalGrid();
}

//compute ACC_LMH and AP_LMH
char CityStruct::computeLMH(double value)
{
	char LMH;

	if(value >= 0 && value < 35)
		LMH = 'L';

	else if(value >= 35 && value < 65)
		LMH = 'M';

	else if(value >= 65 && value < 100)
		LMH = 'H';

	return LMH;
}

//compute rain probability
void CityStruct::computeRainProb()
{
	if(AP_LMH == 'L' && ACC_LMH == 'H')
	{
		rainProb = 90.00;
		ASCII = "~~~~\n~~~~~\n\\\\\\\\\\";
	}
	else if(AP_LMH == 'L' && ACC_LMH == 'M')
	{
		rainProb = 80.00;
		ASCII = "~~~~\n~~~~~\n \\\\\\\\";
	}
	else if(AP_LMH == 'L' && ACC_LMH == 'L')
	{
		rainProb = 70.00;
		ASCII = "~~~~\n~~~~~\n  \\\\\\";
	}
	else if(AP_LMH == 'M' && ACC_LMH == 'H')
	{
		rainProb = 60.00;
		ASCII = "~~~~\n~~~~~\n   \\\\";
	}
	else if(AP_LMH == 'M' && ACC_LMH == 'M')
	{
		rainProb = 50.00;
		ASCII = "~~~~\n~~~~~\n    \\";
	}
	else if(AP_LMH == 'M' && ACC_LMH == 'L')
	{
		rainProb = 40.00;
		ASCII = "~~~~\n~~~~~";
	}
	else if(AP_LMH == 'H' && ACC_LMH == 'H')
	{
		rainProb = 30.00;
		ASCII = "~~~\n~~~~";
	}
	else if(AP_LMH == 'H' && ACC_LMH == 'M')
	{
		rainProb = 20.00;
		ASCII = "~~\n~~~";
	}
	else	//AP_LMH == 'H' && ACC_LMH == 'L'
	{
		rainProb = 10.00;
		ASCII = "~\n~~";
	}
}

//runs all compute function in CityStruct
void CityStruct::computeAll()
{
	computeAvgCC();	
	computeAvgP();
	ACC_LMH = computeLMH(avgCC);
	AP_LMH = computeLMH(avgP);
	computeRainProb();
}

//formatted for summary report
string CityStruct::toString()
{
	ostringstream oss;

	oss << "City Name\t: " << name << endl;
	oss << "City ID\t\t: " << cityGrid[0].cityId << endl;
	
	oss << fixed << setprecision(2);	//set all output of double to 2 decimal, #include <iomanip>;
	oss << "Ave. Cloud Cover (ACC)\t: " << avgCC << " (" << ACC_LMH << ")" << endl;
	oss << "Ave. Pressure\t(AP)\t: " << avgP << " (" << AP_LMH << ")" << endl;
	oss << "Probability of Rain (%)\t: " << rainProb << endl;
	oss << ASCII << endl;

	oss << endl;
	return (oss.str());
}

//-----------------------------------------------------------main functions--------------------------------------
void displayMenu()
{
	cout << "Student ID	: 6097881" << endl;
	cout << "Student Name	: Lee Chia Lin" << endl;	
	cout << "---------------------------------------" << endl;
	cout << "Welcome to Weather Information Processing System!" << endl << endl;

	cout << "1) Read in and process a configuration file" << endl;
	cout << "2) Display city map" << endl;
	cout << "3) Display cloud coverage map (cloudiness index)" << endl;
	cout << "4) Display cloud coverage map (LMH index)" << endl;
	cout << "5) Display atmospheric pressure map (pressure index)" << endl;
	cout << "6) Display atmospheric pressure map (LMH index)" << endl; 
	cout << "7) Show weather forecast summary report" << endl; 
	cout << "8) Quit" << endl; 
	
	cout << endl;	
	cout << "Please enter your choice: ";

}   // end displayMenu() ...

//tokenize the strings
vector<string> splitStringsByToken(string str, char dl) 
{
	size_t pos = 0;
	string token;
	vector<string> substr_list; 

	while ((pos = str.find(dl)) != string::npos)
	{
		token = str.substr(0,pos);
		substr_list.push_back(token);
		str.erase(0,pos + 1);	  
	}
	substr_list.push_back(str);
	
	return substr_list;
}

//to sum cloud cover value and pressure value of a city
void addSumToCity(string filename, int row, int col, string str)
{
	if(xyGrid2D[row][col].cityName != "")
	{
		for(int c=0; c<xSize*ySize; c++ )
		{	
			if(cityStructArray[c].name == xyGrid2D[row][col].cityName)
			{
				if(filename == filesArray[1])
					cityStructArray[c].addCloudSum(stoi(str));
				else
					cityStructArray[c].addPressureSum(stoi(str));
			}
		}
	}

}

//each call reads only 1 file
//for processing the filename.txt mentioned in the configuration file
void readFile(string filename)
{
	fstream inputFile (filename.c_str(), fstream::in);

	string aLine;	
	int xAxis, yAxis, col, row;
	bool cityExist = false;
	int cityArray_index = 0;

	while (getline (inputFile, aLine))
	{
		vector<string> res = splitStringsByToken(aLine, '-');
		for (string str : res) 
		{	
			if(str[0] == '[')
			{
				string stringX = str.substr(1, str.find(',')-1);
	
				int yEndPos = str.length() - stringX.length() - 4;
				string stringY = str.substr(str.find(',')+2, yEndPos);

				//convert string to int	
				xAxis = stoi(stringX);
				yAxis = stoi(stringY);
	
				//for easier input of data to xyGrid2D[row][col] later
				col = xAxis - xyRange[0];	//xyRange[0] is the lower limit of xAxis
				row = xyRange[3] - yAxis;	//xyRange[3] is the upper limit of yAxis
			}
			else if(isdigit(str[0]))
			{
			
				if(filename == filesArray[0])
				{
					xyGrid2D[row][col].x = xAxis;
					xyGrid2D[row][col].y = yAxis;

					xyGrid2D[row][col].cityId = stoi(str);	
					xyGrid2D[row][col].cityName = res[2];

					int cityStruct_index;
					//check if city already exist in CityStruct
					for(int c=0; c<xSize*ySize; c++ )
					{	
						if(cityStructArray[c].name == res[2])
						{
							cityExist = true;
							cityStruct_index = c;	
						}
					}

					//record info if city doesnt exist yet
					if(cityExist == false)
					{
						cityStructArray[cityArray_index].name = res[2];
						cityStructArray[cityArray_index].gridSize = xSize*ySize;
						cityStructArray[cityArray_index].initArrays();
						cityStruct_index = cityArray_index;
						cityArray_index++;	
					}
					cityStructArray[cityStruct_index].addCityGrid(xyGrid2D[row][col]);
				}
				
				else if(filename == filesArray[1])
				{			
					if(xyGrid2D[row][col].x == -1 && xyGrid2D[row][col].y == -1)
					{
						xyGrid2D[row][col].x = xAxis;
						xyGrid2D[row][col].y = yAxis;
						
						xyGrid2D[row][col].cityName = "";
					}
					xyGrid2D[row][col].cloudValue = stoi(str);	
					addSumToCity(filename, row, col, str);
				}
				else if(filename == filesArray[2])
				{
					xyGrid2D[row][col].pressureValue = stoi(str);	
					addSumToCity(filename, row, col, str);	
				}
			}
			//prepare for next loop, reset to false
			cityExist = false;
		}
	}
	cout << filename << "... done!" << endl;
}

//process the whole config file and stores all the necessary data
bool processConfigFile()
{
	cout << "Please enter input filename : ";

	string inputFilename;
	cin >> inputFilename;
	fstream inputFile (inputFilename.c_str(), fstream::in);
	
	string aLine;
	cout << endl;
		
	int xyRange_counter = 0;
	int files_counter = 0;
	bool readFileComplete = false;

	while (getline (inputFile, aLine))
	{
		if(aLine[0] != '/' && aLine.find("Grid") != string::npos)
		{	
			string a = aLine.substr(aLine.find("=") + 1); 
			vector<string> res = splitStringsByToken(a, '-');
			for (string x : res) 
			{			
				xyRange[xyRange_counter] = stoi(x);	//convert string to int
				xyRange_counter++;
			}

			if(xyRange_counter == 2)
			{
				cout << "Reading in GridX_IdxSize : "; 
				cout << xyRange[0] << "-" << xyRange[1] << " ... done!" << endl;
			}
			else if(xyRange_counter == 4)
			{
				cout << "Reading in GridY_IdxSize : "; 
				cout << xyRange[2] << "-" << xyRange[3] << " ... done!" << endl;
				cout << endl;
			}
		}
		else
		{
			//reading values for range of x and y are completed, when xyRange_counter == 4
			//calculate range and initialize xyGrid2D
			if(xyRange_counter == 4 && xyGrid2D == nullptr) 			
			{
				//add 1 to include the upper limit
				xSize = xyRange[1] - xyRange[0] + 1;
				ySize = xyRange[3] - xyRange[2] + 1;

				if(xSize != 0 && ySize !=0)
				{
					//initialize xyGrid2D
					xyGrid2D = new GridStruct *[ySize];

					for (int r=0; r<ySize; r++)
						xyGrid2D [r] = new GridStruct [xSize];
			
					cityStructArray = new CityStruct [xSize*ySize];
				}		
				cout << "Storing data from input file :" << endl;
				xyRange_counter++;
			}

			size_t pos = aLine.find(".txt");

			if (pos != string::npos)
			{
				filesArray [files_counter] = aLine;
				files_counter++;				
				readFile(aLine);

				if(files_counter == 3)
					readFileComplete = true;
			}
		}
	}	

	if(readFileComplete == true)
	{
		runAllStructCompute();		
		cout << "All records successfully stored. Going back to main menu ..." << endl << endl;		
	} 
	else
	{
		cout << "Storing of record incomplete. Going back to main menu ..." << endl << endl;
	}
	return readFileComplete;
}

//run the struct functions for computation which includes, e.g computing of pressure index
void runAllStructCompute()
{
	int city_counter = 0;	
	//using cityStructArray[i+j] will cause repetition, therefore need counter

	for (int i=0; i<ySize; i++)
	{
		for(int j=0; j<xSize; j++)
		{
			xyGrid2D [i][j].computeAll();

			if(cityStructArray[city_counter].name != "")
			{	
				cityStructArray[city_counter].constructGridPerimeter();
				cityStructArray[city_counter].computeAll();
			}
			city_counter++;
		}	
	}
}

//for displaying the information in grid view
void displayGrid(string tableType)
{
	cout << endl;
	
	cout << "\t# ";
	for(int i=0; i<xSize+1; i++)
		cout << " # ";
	cout << endl;

	for(int r=0; r<ySize; r++)
	{
		int yAxis_value = xyRange[3]-r;		
		cout << "    " << yAxis_value << "\t# ";
		
		for(int c=0; c<xSize; c++)
		{
			if(tableType == "cloudLMH")						
				cout << " " << xyGrid2D[r][c].cloudLMH << " ";
			else if(tableType == "cloudIndex")
				cout << " " << xyGrid2D[r][c].cloudIndex << " ";
			else if(tableType == "pressureLMH")
				cout << " " << xyGrid2D[r][c].pressureLMH << " ";
			else if (tableType == "pressureIndex")
				cout << " " << xyGrid2D[r][c].pressureIndex << " ";
			else	//tableType == "cityMap"
			{
				if(xyGrid2D[r][c].cityName != "")
					cout << " " << xyGrid2D[r][c].cityId << " ";
				else
					cout << "   ";
			}
		}
		cout << " #" << endl;
	}
	cout << "\t# ";
	for(int i=0; i<xSize+1; i++)
		cout << " # ";
	cout << endl;

	cout << "\t  ";
	for(int i=xyRange[0]; i<=xyRange[1]; i++)
		cout << " " << i << " ";

	cout << endl << endl;
}

//display summary report
void displayReport()
{
	cout << endl << "Weather Forecast Summary Report" << endl;
	cout << "--------------------------------" << endl;

	for(int c=0; c<xSize*ySize; c++ )
	{	
		if(cityStructArray[c].name != "")
		{	
			cout << cityStructArray[c].toString();
			cout << endl;	
		}
	}
}

void deallocateMemory()
{
	delete [] cityStructArray;

	if (xSize <= 0)
		return;

	for (int i=0; i<ySize; i++)
	{
		delete [] xyGrid2D [i];
	}
	
	delete [] xyGrid2D;
}
