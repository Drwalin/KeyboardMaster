// Copyright (C) 2017 by Marek Zalewski

#include <cstring>
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <fstream>
#include <map>

#ifdef WIN
#include <conio.h>
#include <windows.h>

void gotoxy( int x, int y )
{
  COORD coord;
  coord.X = x;
  coord.Y = y;
  SetConsoleCursorPosition( GetStdHandle(STD_OUTPUT_HANDLE), coord );
}

#else
#include "gconio.h"

void gotoxy( int x, int y )
{
	printf( "%c[%d;%df", 0x1B, y, x );
}

#endif

void Delay( int ms )
{
	std::this_thread::sleep_for( std::chrono::milliseconds( ms ) );
}

int GetTime()
{
	return ( clock() * 1000 ) / CLOCKS_PER_SEC;
}

void Clear()
{
#ifndef WIN
	system( "clear" );
#else
	system( "cls" );
#endif
}

//////////////////
////// main //////
//////////////////


void LoadData( std::string fileName, bool check );
void GetString( std::string & output, std::string & input, int & ms, bool changeLetters );

int main( int argc, char ** argv )
{
	srand( time( NULL ) );
#ifndef WIN
	initscr();
#endif
	
	int i, j;
	
	int timeout = 0, wrong = 0, correct = 0;
	#define SUM (timeout+wrong+correct)
	
	int acceptTimeTo = 0, cooldown;
	char sign;
	
	bool isWrong = false;
	
	bool t = true;
	int xpos = 0;
	std::string correctForm = "", printString;
	
	
	std::map < int, bool > mapArgs;
	for( i = 0; i < argc; ++i )
	{
		mapArgs[i] = false;
	}
	mapArgs[0] = true;
	
	bool elven = false;
	bool changeLetters = true;
	bool timeLimit = false, timeLimitSet = true;
	bool fastLoad = false;
	for( i = 1; i < argc; ++i )
	{
		if( !strcmp(argv[i],".f") )
		{
			fastLoad = true;
			mapArgs[i] = true;
		}
		else if( !strcmp(argv[i],".c") )
		{
			changeLetters = true;
			mapArgs[i] = true;
		}
		else if( !strcmp(argv[i],".nc") )
		{
			changeLetters = false;
			mapArgs[i] = true;
		}
		else if( !strcmp(argv[i],".l") )
		{
			timeLimit = true;
			mapArgs[i] = true;
			timeLimitSet = true;
		}
		else if( !strcmp(argv[i],".nl") )
		{
			timeLimit = false;
			mapArgs[i] = true;
			timeLimitSet = true;
		}
		else if( !strcmp(argv[i],".elven") )
		{
			elven = true;
			mapArgs[i] = true;
		}
	}
	
	if( elven )
	{
		LoadData( "elvenNames.txt", false );
	}
	else
	{
		if( argc > 1 )
		{
			std::ifstream file( argv[1] );
			if( file.good() )
			{
				LoadData( argv[1], !fastLoad );
				file.close();
			}
			else
				LoadData( "dictionary.txt", !fastLoad );
		}
		else
			LoadData( "dictionary.txt", !fastLoad );
	}
	
	if( !timeLimitSet )
	{
		printf( "\n Do you want time limit? (T/N)" );
		sign = getch();
		if( sign == 't' || sign == 'T' )
			timeLimit = true;
	}
	
#define DELAY_TIME 1000
	
	while( true )
	{
		if( ( timeLimit && acceptTimeTo - GetTime() < 0 ) || t || isWrong || xpos == correctForm.size() )
		{
			if( t )
			{
				t = false;
			}
			else if( isWrong )
			{
				Clear();
				gotoxy( 10, 10 );
				printf( "Wrong answer!" );
				Delay( DELAY_TIME );
				++wrong;
				isWrong = false;
			}
			else if( xpos == correctForm.size() )
			{
				Clear();
				gotoxy( 10, 10 );
				printf( "Correct!" );
				Delay( DELAY_TIME );
				++correct;
			}
			else
			{
				Clear();
				gotoxy( 10, 10 );
				printf( "Time out!" );
				Delay( DELAY_TIME );
				++timeout;
			}
			
			GetString( printString, correctForm, cooldown, changeLetters );
			Clear();
			gotoxy( 10, 10 );
			printf( "%s", printString.c_str() );
			gotoxy( 10, 11 );
			//printf( "\"%s\"", correctForm.c_str() );
			acceptTimeTo = GetTime() + cooldown;
			xpos = 0;
			
			{
				gotoxy( 60, 6 );
				printf( "Timed out: %5.0i", timeout );
				gotoxy( 60, 7 );
				printf( "Wrong:     %5.0i", wrong );
				gotoxy( 60, 8 );
				printf( "Correct:   %5.0i", correct );
				gotoxy( 60, 9 );
				printf( "All:      /%5.0i", SUM );
			}
			
			gotoxy( 11, 12 );
			
			while( kbhit() ) getch();
		}
		
		if( kbhit() )
		{
			sign = 0;
			sign = getch();
			if( sign == correctForm[xpos] )
			{
				printf( "%c", sign );
				++xpos;
				gotoxy( 11 + xpos, 12 );
			}
			else
			{
				isWrong = true;
			}
		}
		else
		{
			Delay( 40 );
		}
	}
	
#ifndef WIN
	endwin();
#endif
	return 0;
}

std::vector < std::string > dictionary;

bool ContainWord( std::string& str, const char * key )
{
	register int kl = strlen( key );
	register int i, j, size = str.size();
	for( i = 0, j = 0; i < size; ++i )
	{
		if( str[i] == key[j] || int(str[i]) + (int('z')-int('Z')) == key[j] )
			++j;
		else
			j = 0;
		if( j >= kl )
			return true;
	}
	return false;
}

bool IsPolite( std::string& str )
{
	if( ContainWord( str, "311" ) )			return false;
	
	if( ContainWord( str, "fuck" ) )		return false;
	if( ContainWord( str, "dick" ) )		return false;
	if( ContainWord( str, "cock" ) )		return false;
	if( ContainWord( str, "sex" ) )			return false;
	if( ContainWord( str, "tit" ) )			return false;
	if( ContainWord( str, "vagin" ) )		return false;
	if( ContainWord( str, "virgin" ) )		return false;
	if( ContainWord( str, "penis" ) )		return false;
	if( ContainWord( str, "boob" ) )		return false;
	if( ContainWord( str, "bob" ) )			return false;
	if( ContainWord( str, "pussy" ) )		return false;
	if( ContainWord( str, "dildo" ) )		return false;
	if( ContainWord( str, "anal" ) )		return false;
	if( ContainWord( str, "suck" ) )		return false;
	if( ContainWord( str, "sack" ) )		return false;
	if( ContainWord( str, "piss" ) )		return false;
	if( ContainWord( str, "slut" ) )		return false;
	if( ContainWord( str, "cum" ) )			return false;
	if( ContainWord( str, "cunt" ) )		return false;
	if( ContainWord( str, "gay" ) )			return false;
	if( ContainWord( str, "shit" ) )		return false;
	
	if( ContainWord( str, "lesb" ) )		return false;
	if( ContainWord( str, "69" ) )			return false;
	if( ContainWord( str, "porn" ) )		return false;
	if( ContainWord( str, "pedo" ) )		return false;
	
	if( ContainWord( str, "gei" ) )			return false;
	if( ContainWord( str, "gej" ) )			return false;
	if( ContainWord( str, "pierd" ) )		return false;
	if( ContainWord( str, "cip" ) )			return false;
	if( ContainWord( str, "jeb" ) )			return false;
	if( ContainWord( str, "kurew" ) )		return false;
	if( ContainWord( str, "pieprz" ) )		return false;
	if( ContainWord( str, "pizd" ) )		return false;
	if( ContainWord( str, "rucha" ) )		return false;
	if( ContainWord( str, "kurw" ) )		return false;
	if( ContainWord( str, "sukin" ) )		return false;
	if( ContainWord( str, "dup" ) )			return false;
	if( ContainWord( str, "huj" ) )			return false;
	if( ContainWord( str, "kór" ) )			return false;
	if( ContainWord( str, "hój" ) )			return false;
	if( ContainWord( str, "koorw" ) )		return false;
	if( ContainWord( str, "hooj" ) )		return false;
	if( ContainWord( str, "suka" ) )		return false;
	if( ContainWord( str, "wytrysk" ) )		return false;
	if( ContainWord( str, "dziwk" ) )		return false;
	if( ContainWord( str, "dziwe" ) )		return false;
	return true;
}

void LoadData( std::string fileName, bool check )
{
	int i, j, beginTime = GetTime();
	std::string str;
	std::fstream file;
	file.open( fileName.c_str(), std::ios::in );
	if( file.good() )
	{
		while( file >> str )
		{
			if( check )
			{
				if( str.size() > 4 )
				{
					for( i = 0, j = 0; i < str.size(); ++i )
						if( str[i] >= '0' && str[i] <= '9' )
							++j;
					if( j <= 2 )
					{
						if( !IsPolite( str ) )
						{
							printf( "\n Unpolite id: %6.0i  \"%s\"", dictionary.size(), str.c_str() );
							goto endWHILE;
						}
						dictionary.resize( dictionary.size() + 1 );
						dictionary.back() = str;
					}
				}
				endWHILE:
				i;
			}
			else
			{
				dictionary.resize( dictionary.size() + 1 );
				dictionary.back() = str;
			}
		}
		file.close();
		
		if( check )
		{
			file.open( fileName.c_str(), std::ios::out );
			if( file.good() )
			{
				for( i = 0; i < dictionary.size(); ++i )
				{
					file << dictionary[i];
					file << "\n";
				}
			}
			file.close();
		}
	}
	beginTime = GetTime() - beginTime;
	
	printf( "\n\n Press any key\n Done in: %ims", beginTime );
	getch();
}

void GetString( std::string & output, std::string & input, int & ms, bool changeLetters )
{
	int i, j, k, l, a, b, c;
	char * str = new char[1024];
	__GetStringBegin__OF__Function_:
		output = "";
	switch( rand() % 10 )
	{
	case 0:
	case 1:
	case 2:
	case 3:
		
		if( dictionary.size() )
		{
			i = 1;
			output += dictionary[rand()%dictionary.size()];
			while( rand() % 3 == 0 )
			{
				++i;
				output += " ";
				output += dictionary[rand()%dictionary.size()];
			}
			ms = 5000 * i / 3 + (output.size()*1400);
		}
		else
		{
			k = ( rand() % 5 ) + 3;
			for( i = 0; i < k; ++i )
				output += (char)((rand()%('~'-' '))+' ');
			ms = 4000 + (output.size()*1000);
		}
		
		if( changeLetters )
		{
			for( i = 0; i < output.size(); ++i )
			{
				if( rand() % 5 == 0 )
				{
					if( output[i] >= 'A' && output[i] <= 'Z' )
						output[i] = output[i] + ('z'-'Z');
					else if( output[i] >= 'a' && output[i] <= 'z' )
						output[i] = output[i] - ('z'-'Z');
				}
			}
		}
		
		input = output;
		output = "\"" + output + "\"";
		break;
		
	case 4:
	case 5:
		k = ( rand() % 5 ) + 3;
		for( i = 0; i < k; ++i )
			output += (char)((rand()%('~'-' '))+' ');
		input = output;
		ms = 4000 + (input.size()*1000);
		
		output = "\"" + output + "\"";
		
		break;
		
	case 6:
	case 7:
	case 8:
	case 9:
	default:
		
		switch( rand() % 6 )
		{
		case 0:	// +
			a = rand() % 100;
			b = rand() % 100;
			sprintf( str, "%i + %i = ", a, b );
			output = str;
			sprintf( str, "%i", a + b );
			input = str;
			ms = 4000;
			
			break;
		case 1:	// -
			a = rand() % 100;
			b = rand() % 100;
			sprintf( str, "%i - %i = ", a, b );
			output = str;
			sprintf( str, "%i", a - b );
			input = str;
			ms = 4000;
			
			break;
		case 2:	// *
			a = rand() % 27;
			b = rand() % 27;
			sprintf( str, "%i * %i = ", a, b );
			output = str;
			sprintf( str, "%i", a * b );
			input = str;
			ms = 4000;
			
			break;
		case 3:	// /
			a = rand() % 100;
			b = (rand() % 27) + 1;
			sprintf( str, "%i / %i = ", a, b );
			output = str;
			sprintf( str, "%i", int( ( float(a) / float(b) ) + 0.5f ) );
			input = str;
			ms = 4000;
			
			break;
		case 4:	// ^
			a = rand() % 18;
			b = rand() % 4;
			sprintf( str, "%i ^ %i = ", a, b );
			output = str;
			c = 1;
			for( i = 0; i < b; ++i )
				c *= a;
			sprintf( str, "%i", c );
			input = str;
			ms = 7000;
			
			break;
		case 5:	// sqrt
			a = rand() % 311;
			sprintf( str, "sqrt( %i ) = ", a );
			output = str;
			sprintf( str, "%i", int( sqrt(float(a)) + 0.5f ) );
			input = str;
			ms = 7000;
			
			break;
		case 6:	// %
			a = rand() % 80;
			b = (rand() % 27) + 1;
			sprintf( str, "%i %c %i = ", a, '%', b );
			output = str;
			sprintf( str, "%i", a % b );
			input = str;
			ms = 4000;
			
			break;
		}
		
	}
	
	if( !IsPolite( output ) )
		goto __GetStringBegin__OF__Function_;
	
	delete[] str;
}




