//Baolin Chang
//CS135
//Project 2 Task C
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

string addSpaces(string s, int width);
string HalfSpaces(string s, int width);
void align_Header(string header_align, string line, string &newstring, bool double_spaced);
void align_Body(string body_align, string s, string &newstring, int width, bool double_spaced);

//for left and right alignment
string addSpaces(string s, int width){
    string spaces;

    for(int i = 0; i < width - s.length() - 1; i++){
        spaces += " ";
    }
    return spaces;
}

//for centering
string HalfSpaces(string s, int width){
    string halfspaces;

    int length = (int)(.5 * (width - s.length()) + 0.5);
    for(int i = 0; i < length - 1; i++){
        halfspaces += " ";
    }

    return halfspaces;
}

void align_Header(string header_align, string line, string &newstring, int width, bool double_spaced){
    //if title is the first line, then dont add extra space before it
    if(newstring.length() == 0){
            if(header_align == "right")
                newstring += addSpaces(line,width) + line + '\n';
            else if(header_align == "left")
                newstring += line + addSpaces(line,width) + '\n';
            else if(header_align == "center")
                newstring += HalfSpaces(line,width) + line + HalfSpaces(line,width) + '\n';
    }
    else{
        if(double_spaced){
            if(header_align == "right")
                newstring += '\n' + addSpaces(line,width) + line + '\n';
            else if(header_align == "left")
                newstring += '\n' + line + addSpaces(line,width) + '\n';
            else if(header_align == "center")
                newstring += '\n' + HalfSpaces(line,width) + line + HalfSpaces(line,width) + '\n';
        }
    }
    
}

void align_Body(string body_align, string s, string &newstring, int width, bool double_spaced){
    if(double_spaced){
        if(body_align == "left")
            newstring += '\n' + s + addSpaces(s, width) + '\n';
        else if(body_align == "right")
            newstring += '\n' + addSpaces(s, width) + s + '\n';
        else   
            newstring += '\n' + HalfSpaces(s, width) + s + HalfSpaces(s, width) + '\n';
    }
    else{
        if(body_align == "left")
            newstring += s + addSpaces(s, width) + '\n';
        else if(body_align == "right")
            newstring += addSpaces(s, width) + s + '\n';
        else   
            newstring += HalfSpaces(s, width) + s + HalfSpaces(s, width) + '\n';
    }
}

int main()
{
    string string_width, string_outfile, words, header_align, body_align; 
    string filename = "input.txt";
    bool printed = false;
    // cout << "Enter the Input Filename: ";
    // cin >> filename;

    ifstream infile;
    infile.open(filename);
    getline(infile, string_width, ';');
    int width = stoi(string_width);

    getline(infile, body_align, ';');
    if(body_align.length() == 0){
        body_align = "left";
    }

    getline(infile, header_align, ';');
        if(header_align.length() == 0)
            header_align = body_align;

    string string_to_fill, string_double_spaced;
    bool to_fill, double_spaced;

    getline(infile,string_to_fill,';');
        if(string_to_fill == "true")
            to_fill = true;
        else
            to_fill = false;
        //cout<<"to_fill: "<<to_fill<<endl;
    getline(infile, string_double_spaced, ';');
        if(string_double_spaced == "true")
            double_spaced = true;
        else
            double_spaced = false;
        //cout<<"double_spaced: "<<double_spaced<<endl;
    getline(infile, string_outfile, ';');
    ofstream outfile;
    outfile.open(string_outfile);
    string firstLine;
    getline(infile, firstLine);

    string line, newstring;
    string paragraph;
    string words_list;
    int temp_width = width;
    bool seenValidLine = false;
    bool endLine = false;
    int lineCount = 0;
    bool isTitle = false;
    bool dontPrint = false;
    string dummyLine;

    while (getline(infile, line)){
        //cout<<line<<endl;
        //if line is a title
        if(isupper(line[0]) && isupper(line[1])){
            cout<<"hi im a title"<<endl;
            isTitle = true;
            if(isTitle){
                isTitle = false;
                align_Header(header_align, line, newstring, width, double_spaced);
            }
            continue;
        }
//if you don't have to fill, do default
    if(!to_fill){
        if(line.length() == 0){
            //add remaining words_list string before breaking new line
            if (words_list.length() != 0){ 
                align_Body(body_align, words_list, newstring, width, double_spaced);
                //reset words_list
                words_list = "";
                temp_width = width;
                }
            newstring += '\n';
        }

        // line is shorter than width, so print it out
        else if(words_list.length() + line.length() + 1 < width ){
            align_Body(body_align, words_list + line, newstring, width, double_spaced);
            words_list = "";
            temp_width = width;
        }

        else if(words_list.length() + line.length() + 1 >= width){
        
            stringstream ss(line);
            string word;

            while(ss >> word){
                int difference = temp_width - (word.length() + 1); 

                if(difference >= 0){

                    if(words_list.empty()){
                        words_list = word;
                    }
                    else
                        words_list += " " + word ;

                    temp_width = temp_width - (word.length() + 1);
                }
                else{
                    align_Body(body_align, words_list, newstring, width, double_spaced);

                    temp_width = width - (word.length() + 1);
                    words_list = word;

                }
            }
        }
    }
        //to_fill
    if(to_fill){
        //confused as to why it only iterates once? for input.txt
        //update: it only bugs out when double_spaced is true. if its false, it works fine
        if(line.length() == 0){
            newstring += '\n';
            continue;
        }
        cout<<"hi i make it to fill"<<endl;
        string tempstring;
        int charcount = 0;
            for(int i = 0; i < line.length(); i++){
                if(charcount < width - 1){
                    tempstring += line[i];
                    align_Body(body_align, tempstring, newstring, width, double_spaced);
                    charcount++;
                    //newstring += line[i];
                }
                else{
                    newstring += "-" + '\n';
                    charcount = 0;
                }
            }
        }
    }
    /*
    have a for loop that loops through the length of each line
        take in each char one by one, and increase charcount. 
        if charcount < width - 1, add a dash and skip to the next line
    */

    if(words_list.length() > 0){
        if(double_spaced){
            newstring += '\n';
        }
        if(body_align == "left")
        newstring += words_list + addSpaces(words_list, width);
        if(body_align == "right")
            newstring += addSpaces(words_list, width) + words_list;
        if(body_align ==  "center"){
            newstring += HalfSpaces(words_list, width) + words_list + HalfSpaces(words_list, width);
        }
    }

        while(newstring[newstring.length()-1] == '\n'){
            newstring = newstring.substr(0, newstring.length()-1);
    }
    
    outfile<<newstring;
    cout<<newstring;
} 